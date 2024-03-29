/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "../Billiards/Game.h"
#include "../Billiards/GameLayout.h"
#include "../Billiards/AudioSocket.h"

#include "../AudioSystem/PoolAudio.h"
#pragma comment(lib, "AudioSystem.lib")

#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

#include "Frame.h"
#include "Geometries.h"
#include "mathTypeConvert.h"
#include "GameObject.h"
#include "OgreUtil.h"
#include "LuaGameHost.h"
#include "resource.h"

#include <iostream>

static const Real SKYBOX_ROTATE_SPEED = 0.003f;


static CEGUI::MouseButton convertOISMouseButtonToCegui(int buttonID)
{
	switch (buttonID)
	{
	case 0: return CEGUI::LeftButton;
	case 1: return CEGUI::RightButton;
	case 2:	return CEGUI::MiddleButton;
	case 3: return CEGUI::X1Button;
	default: return CEGUI::LeftButton;
	}
}

static const std::string&	getCurrentDirectory()
{
	struct LocalScope
	{
		static std::string	getCurrentDirectory()
		{
			char path[MAX_PATH] = "";
			::GetCurrentDirectory(MAX_PATH, path);

			return path;
		}
	};

	static const std::string s_CurrentDirectory = LocalScope::getCurrentDirectory();
	return s_CurrentDirectory;
}


class OgreListener
	: public ExampleFrameListener
	, public OIS::KeyListener
	, public OIS::MouseListener
{
	friend class Frame;


public:
	OgreListener(Frame* frame, RenderWindow* win, Camera* cam)
		: ExampleFrameListener(win, cam, true, true)
		, m_Frame(frame)
		, m_Exit(false)
	{
		mKeyboard->setEventCallback(this);
		mMouse->setEventCallback(this);
		const_cast<OIS::MouseState&>(mMouse->getMouseState()).X.abs = mMouse->getMouseState().width / 2;
		const_cast<OIS::MouseState&>(mMouse->getMouseState()).Y.abs = mMouse->getMouseState().height / 2;

		mDebugOverlay->getChild("Core/LogoPanel")->hide();
#ifndef	_DEBUG
		mDebugOverlay->hide();
#endif	// !defined(_DEBUG)
	};

	~OgreListener()
	{
	};

	Vector2 getCursorPosition() const
	{
		if(mMouse)
		{
			const OIS::MouseState& mstate = mMouse->getMouseState();
			return Vector2(mstate.X.abs, mstate.Y.abs);
		}

		return Vector2::ZERO;
	};

	unsigned int getWindowWidth() const
	{
		return mMouse ? mMouse->getMouseState().width : 0;
	};

	unsigned int getWindowHeight() const
	{
		return mMouse ? mMouse->getMouseState().height : 0;
	};

	bool mouseIsMoving() const
	{
		return mMouse && (mMouse->getMouseState().X.rel || mMouse->getMouseState().Y.rel || mMouse->getMouseState().Z.rel);
	};

	void exit()
	{
		m_Exit = true;
	};

private:
	virtual bool frameStarted(const FrameEvent& evt)
	{
		if(m_Exit)
			return false;

		m_Frame->frameStarted(evt);

		//if(mKeyboard && mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		//	return false;

		// Call superclass
		return ExampleFrameListener::frameStarted(evt);
	};

	virtual bool frameEnded(const FrameEvent& evt)
	{
		m_Frame->frameEnded(evt);

		return ExampleFrameListener::frameEnded(evt);
	}

	virtual bool keyPressed(const OIS::KeyEvent& e)
	{
		if(e.key == OIS::KC_F8)
			mDebugOverlay->isVisible() ? mDebugOverlay->hide() : mDebugOverlay->show();

		return m_Frame->keyPressed(e);
	};

	virtual bool keyReleased(const OIS::KeyEvent& e)
	{
		return m_Frame->keyReleased(e);
	};


	virtual bool mouseMoved(const OIS::MouseEvent& e)
	{
		return m_Frame->mouseMoved(e);
	};

	virtual bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{
		return m_Frame->mousePressed(e, id);
	};

	virtual bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
	{
		return m_Frame->mouseReleased(e, id);
	};

	virtual void windowClosed(RenderWindow*)
	{
		m_Frame->onQueryExit();
	};

	virtual void windowFocusChange(RenderWindow* rw)
	{
		if(wxWindow* w = wxWindow::FindWindowById(m_Frame->m_FocusDialog))
		{
			::SetForegroundWindow(::HWND(w->GetHandle()));
			w->SetFocus();
			if(wxTopLevelWindow* top = dynamic_cast<wxTopLevelWindow*>(w))
				top->RequestUserAttention();
		}
	};

private:
	Frame*	m_Frame;

	bool	m_Exit;
};



BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_IDLE(Frame::onIdle)
	EVT_CLOSE(Frame::onClose)
END_EVENT_TABLE()


Frame::Frame(const OnCloseFunctor& fnOnClose)
	: wxFrame(NULL, -1, "Pool", wxDefaultPosition, wxSize(800, 640), wxDEFAULT_FRAME_STYLE)
	, m_nodeLight(NULL)
	, m_nodeLight2(NULL)
	, m_nodeGame(NULL)
	, m_nodeCameraRoot(NULL)
	, m_nodeCamera(NULL)
	, m_nodeCue(NULL)
	, m_ImageBall(NULL)
	, m_ImagePoint(NULL)
	, m_ImagePowerSlotBase(NULL)
	, m_ImagePowerSlotSurface(NULL)
	, m_ImagePowerColumn(NULL)
	, m_ImagePowerColumnMax(NULL)
	, m_fnOnClose(fnOnClose)
	, m_RotatingCamera(false)
	, m_PanningCamera(false)
	, m_ShootAble(true)
	, m_PowerControlOn(false)
	, m_SpinControlOn(false)
	, m_SkyBoxAngle((timeGetTime() % DWORD(Math::PI * 2000 / SKYBOX_ROTATE_SPEED)) * SKYBOX_ROTATE_SPEED / 1000)
	, m_FocusDialog(wxID_ANY)
	, m_AmassDistance(0)
	, m_AmassMax(0)
	, m_SpinPoint(0, 0)
	, m_PitchingAngle(0)
{
}

Frame::~Frame()
{
}

void Frame::createFrameListener()
{
	// This is where we instantiate our own frame listener
	mFrameListener = new OgreListener(this, mWindow, mCamera);
	mRoot->addFrameListener(mFrameListener);
}

void Frame::createScene()
{
	{
#pragma message("Update version info: file:"__FILE__" date:"__DATE__" time:"__TIME__)
		static const char* const VERSIONINFO = "[VERSION] Pool version: File:"__FILE__", Date:"__DATE__", Time:"__TIME__;
		LogManager::getSingleton().logMessage(VERSIONINFO, LML_CRITICAL);
	}

	// set icon for render window
	::HWND hwnd = getRenderWindowHandle(mWindow);
	if(hwnd)
	{
		::SendMessage(hwnd, WM_SETICON, ICON_BIG, ::LPARAM(::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_LOGO))));
	}

	m_nodeCameraRoot = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	// setup lights
	{
		Light* l2 = mSceneMgr->createLight("MainLight");
		l2->setType(Light::LT_SPOTLIGHT);
		l2->setDiffuseColour(1.0f, 0.9f, 0.7f);
		l2->setSpecularColour(1.0f, 0.9f, 0.7f);
		l2->setDirection(0, -1, 0);
		l2->setSpotlightRange(Radian(Math::PI * 0.1f), Radian(Math::PI * 0.60f));
		//l2->setAttenuation(1000, 0.9f, 0, 0);
		l2->setCastShadows(true);
		m_nodeLight2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		m_nodeLight2->createChildSceneNode(Vector3(8, 20, 0))->attachObject(l2);

		Light* l3 = mSceneMgr->createLight("MainLight2");
		l3->setType(Light::LT_SPOTLIGHT);
		l3->setDiffuseColour(1.0f, 0.9f, 0.7f);
		l3->setSpecularColour(1.0f, 0.9f, 0.7f);
		l3->setDirection(0, -1, 0);
		l3->setSpotlightRange(Radian(Math::PI * 0.1f), Radian(Math::PI * 0.60f));
		l3->setCastShadows(true);
		SceneNode* nodeLight3 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		nodeLight3->createChildSceneNode(Vector3(-8, 20, 0))->attachObject(l3);

		Light* l1 = mSceneMgr->createLight("AssistLight");
		l1->setAttenuation(1000, 0.4f, 0, 0);
		l1->setSpecularColour(0.1f, 0.1f, 0.1f);
		l1->setCastShadows(true);
		l1->setVisible(false);
		m_nodeLight = m_nodeCameraRoot->createChildSceneNode();
		m_nodeLight->createChildSceneNode(Vector3(40, 20, 100))->attachObject(l1);
	}

	mSceneMgr->setAmbientLight(ColourValue(0.4f, 0.4f, 0.4f));
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	mSceneMgr->setShadowColour(ColourValue(0.4f, 0.4f, 0.4f));
	//mSceneMgr->setShadowTextureSettings(1024, 2);

	setupGui();

	m_nodeCameraRoot->setOrientation(Quaternion(m_SkyBoxAngle, Vector3::UNIT_Y));
	m_nodeCameraRoot->setPosition(0, 8, 0);
	m_nodeCamera = m_nodeCameraRoot->createChildSceneNode(Vector3(0, 18, 30));
	mCamera->setPosition(m_nodeCamera->_getDerivedPosition());
	mCamera->lookAt(m_nodeCameraRoot->_getDerivedPosition());

	createSphere("Sphere", 30, 30, 1);
	m_nodeGame = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	m_nodeCue = m_nodeGame->createChildSceneNode("cue");
	m_nodeCue->createChildSceneNode()->createChildSceneNode(Vector3(-0.8, -0.948, 4.992))->attachObject(mSceneMgr->createEntity("cue", "cue.mesh"));
	m_nodeCue->setPosition(0, 8, 8);
	m_nodeCue->getChild(0)->getChild(0)->setOrientation(Quaternion(Radian(Math::HALF_PI), Vector3::UNIT_Y));
	m_nodeCue->setVisible(m_PowerControlOn || m_SpinControlOn);

	// table & ground
	{
		Ogre::SceneNode* nodeTable = m_nodeGame->createChildSceneNode("table");
		nodeTable->attachObject(mSceneMgr->createEntity("table", "table.mesh"));
		nodeTable->setPosition(0, 8, 0);

		Ogre::Entity* entGround = mSceneMgr->createEntity("ground", "floor200x200.mesh");
		entGround->getSubEntity(0)->setMaterialName("Pool/Table/RedGrass");
		Ogre::SceneNode* groundNode = m_nodeGame->createChildSceneNode("groundnode");

		groundNode->scale(1, 0.2f, 1);
		groundNode->attachObject(entGround);
		groundNode->setPosition(Ogre::Vector3(0, 0, 0));
	}

	mSceneMgr->setSkyBox(true, "Pool/SkyBox");

	mWindow->getViewport(0)->setBackgroundColour(ColourValue(0.2, 0.2, 0.2));

	{
		// init audio system
		PoolAudio::instance().init(4, 3);

		PoolAudio::instance().loadWavFile("Media/sound/collide.wav");
		PoolAudio::instance().loadWavFile("Media/sound/hit.wav");
		PoolAudio::instance().loadWavFile("Media/sound/collide2.wav");
		PoolAudio::instance().loadWavFile("Media/sound/Pocket.wav");

		struct LocalScope
		{
			static void playSound(const std::string& name, const Billiards::Vector position, Billiards::Real volume)
			{
				static std::map<std::string, int> s_SoundMap = boost::assign::map_list_of
					("collide b-b", 0)
					("hit", 1)
					("bound", 2)
					("falling", 3)
					;

				if(!s_SoundMap.count(name))
					throw std::runtime_error("no sound named \"" + name + "\"");

				PoolAudio::instance().playSound(s_SoundMap[name], position.ptr(), volume);
			};
		};
		Billiards::AudioSocket::instance().playSound.connect(&LocalScope::playSound);
	}

	// init physics system
	m_Game.reset(new Billiards::Game(boost::bind(&Frame::createGameObject, this, _1)));
	m_Game->loadBallConfigSet("std");

	m_GameHost.reset(new LuaGameHost("./Hosts/test.lua"));
	m_GameHost->initialize(m_Game.get());
}

void Frame::onIdle(wxIdleEvent& e)
{
	wxWindow* focus = wxGetActiveWindow();
	if(!focus || focus == this)
	{
		if(mWindow->isActive())
		{
			if(mRoot && mRoot->renderOneFrame())
				e.RequestMore();
		}
	}
	else
	{
		m_FocusDialog = focus->GetId();
	}
}

void Frame::onClose(wxCloseEvent&)
{
	onQueryExit();
}


void Frame::frameStarted(const FrameEvent& evt)
{
	m_timer.reset();

	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	// restrict elapsed time by a upper limit
	const Real elapsed = std::min(evt.timeSinceLastFrame, 0.06f);

	{
		// step the world
		m_Game->simulate(elapsed);

		Radian delta = Radian(elapsed * SKYBOX_ROTATE_SPEED);
		m_SkyBoxAngle += delta;

		// light animation
		//m_nodeLight2->yaw(delta);
		//m_nodeLight->yaw(delta);

		// update cue
		m_nodeCue->setVisible(m_PowerControlOn || m_SpinControlOn);
		if(m_PowerControlOn || m_SpinControlOn)
		{
			const Real radius = m_Game->getMainBall()->getRadius();
			m_nodeCue->getChild(0)->setPosition(m_SpinPoint.x * radius, m_SpinPoint.y * radius, m_AmassDistance + m_Game->getMainBall()->getRadius());
			m_nodeCue->setOrientation(m_nodeCameraRoot->getOrientation());
			m_nodeCue->pitch(-m_PitchingAngle);
		}

		// rotate camera
		//m_nodeCameraRoot->setOrientation(Quaternion(m_SkyBoxAngle, Vector3::UNIT_Y));
		mCamera->setPosition(m_nodeCamera->_getDerivedPosition());
		mCamera->lookAt(m_nodeCameraRoot->_getDerivedPosition());

		// update camera position in audio system
		PoolAudio::instance().setListenerPosition(mCamera->getPosition().ptr());
	}
}

void Frame::frameEnded(const FrameEvent& evt)
{
	// yield CPU when not busy
	if(!static_cast<const OgreListener*>(mFrameListener)->mouseIsMoving())
		Sleep(15);

	//while(m_timer.getMilliseconds() < 1000/60)
	//	Sleep(5);
}

bool Frame::keyPressed(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	/*case OIS::KC_SPACE:
		{
			Vector3 front = m_nodeCameraRoot->_getDerivedPosition() - m_nodeCamera->_getDerivedPosition();
			front.normalise();
			m_Game->shot(ogre2Bld(front * 60), ogre2Bld(-front));
		}

		break;*/
	case OIS::KC_R:
		//TODO:
		m_GameHost->initialize(m_Game.get());

		break;
	case OIS::KC_S:
		{
			ShadowTechnique tec = mSceneMgr->getShadowTechnique();
			mSceneMgr->setShadowTechnique(tec ? SHADOWTYPE_NONE : SHADOWTYPE_STENCIL_ADDITIVE);
		}

		break;
	case OIS::KC_LCONTROL:
		if(m_ShootAble && !m_SpinControlOn)
		{
			m_PowerControlOn = true;
			m_RotatingCamera = false;
			m_AmassMax = m_AmassDistance = 0.2f;

			m_nodeCameraRoot->setPosition(bld2Ogre(m_Game->getMainBall()->getPosition()));
			m_nodeCue->setPosition(bld2Ogre(m_Game->getMainBall()->getPosition()));
			m_nodeCue->setOrientation(m_nodeCameraRoot->getOrientation());
			//m_nodeCue->getChild(0)->setPosition(0, 0, m_Game->getMainBall()->getRadius());

			m_GuiSystem->setDefaultMouseCursor(CEGUI::BlankMouseCursor);

			updateGui();
		}

		break;
	case OIS::KC_SPACE:
		if(m_ShootAble && !m_PowerControlOn)
		{
			m_SpinControlOn = true;
			m_AmassMax = m_AmassDistance = 0.2f;

			m_nodeCue->setPosition(bld2Ogre(m_Game->getMainBall()->getPosition()));
			m_nodeCue->setOrientation(m_nodeCameraRoot->getOrientation());

			updateGui();

			m_GuiSystem->setDefaultMouseCursor(CEGUI::BlankMouseCursor);

			//TODO
		}

		break;
	}

	return true;
}

bool Frame::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_LCONTROL:
		m_PowerControlOn = false;

		m_GuiSystem->setDefaultMouseCursor("TaharezLook", "MouseArrow");

		updateGui();

		m_ImagePowerSlotSurface->setHeight(CEGUI::UDim(0.773f, 0));

		m_AmassDistance = 0;
		m_AmassMax = 0;

		break;

	case OIS::KC_SPACE:
		m_GuiSystem->setDefaultMouseCursor("TaharezLook", "MouseArrow");

		m_SpinControlOn = false;
		updateGui();

		break;
	}

	return true;
}

bool Frame::mouseMoved(const OIS::MouseEvent& e)
{
	// update camera
	{
		// yaw camera
		if(m_RotatingCamera)
			m_nodeCameraRoot->yaw(Radian(-e.state.X.rel * 0.01f), Node::TS_PARENT);

		Vector3 camera = m_nodeCamera->getPosition();

		// pitch camera
		if(m_RotatingCamera && e.state.Y.rel)
		{
			const Real pitch = e.state.Y.rel * 0.01f;
			const Real cos_pitch = std::cos(pitch), sin_pitch = std::sin(pitch);
			const Real z = camera.z * cos_pitch - camera.y * sin_pitch;
			const Real y = camera.y * cos_pitch + camera.z * sin_pitch;
			if(z > 0)
			{
				camera.z = z;
				camera.y = y;
			}
		}

		// zoom in/out
		if(!m_PowerControlOn && !m_SpinControlOn && e.state.Z.rel)
		{
			static const Real NEAR_DISTANCE = 0.1;
			static const Real FAR_DISTANCE = 1200;

			camera *= std::exp(-e.state.Z.rel / 1600.0);
			if(camera.length() < NEAR_DISTANCE)
				camera = camera.normalisedCopy() * NEAR_DISTANCE;
			else if(camera.length() > FAR_DISTANCE)
				camera = camera.normalisedCopy() * FAR_DISTANCE;
		}

		// pan camera
		if(m_PanningCamera)
		{
			static const Real s_PanSpeed = 0.03f;

			const Real yaw = -m_nodeCameraRoot->getOrientation().getYaw().valueRadians() + Math::PI;
			const Vector3 offset(std::cos(yaw) * e.state.X.rel * s_PanSpeed, e.state.Y.rel * s_PanSpeed, std::sin(yaw) * e.state.X.rel * s_PanSpeed);
			m_nodeCameraRoot->translate(offset);
		}

		m_nodeCamera->setPosition(camera);
	}

	if(m_PowerControlOn)
	{	
		m_AmassDistance += e.state.Y.rel * 0.04f;

		m_PitchingAngle += Radian(e.state.Z.rel * 0.0006f);
		m_PitchingAngle = std::max(m_PitchingAngle, Radian(Math::HALF_PI * -0.06f));
		m_PitchingAngle = std::min(m_PitchingAngle, Radian(Math::HALF_PI * 0.92f));

		if(e.state.Y.rel > 0)
			m_AmassMax = m_AmassDistance;

		// shot
		if(m_AmassDistance < 0)
		{
			Vector3 front = m_nodeCameraRoot->_getDerivedPosition() - m_nodeCamera->_getDerivedPosition();
			assert(front.x != 0 || front.z != 0);
			front.y = std::sqrt(front.x * front.x + front.z * front.z) * std::tan(m_PitchingAngle.valueRadians());
			front.normalise();

			const Vector3 right = front.crossProduct(Vector3::UNIT_Y);
			const Vector3 up = right.crossProduct(front);

			Vector3 touchpos = -front * std::sqrt(1 - m_SpinPoint.length()) + right * m_SpinPoint.x + up * m_SpinPoint.y;

			Real power = std::pow(std::abs(-e.state.Y.rel), 0.8) * std::pow(m_AmassMax, 0.4f) * 0.6f;
			m_Game->shot(ogre2Bld(front * power * 20), ogre2Bld(touchpos));

			m_SpinPoint = Vector2::ZERO;
			m_PitchingAngle = 0;
			m_PowerControlOn = false;
		}

		Real rad = (1 - m_AmassDistance / 15);
		if(rad > 1)
			rad = 1;
		CEGUI::UDim height(rad * 0.773, 0);
		m_ImagePowerSlotSurface->setHeight(height);

		updateGui();
	}

	if(m_SpinControlOn)
	{
		static const Real spinmax = 0.7f;

		m_SpinPoint.x += e.state.X.rel * 0.006;
		m_SpinPoint.y -= e.state.Y.rel * 0.006;

		m_PitchingAngle += Radian(e.state.Z.rel * 0.0006f);
		m_PitchingAngle = std::max(m_PitchingAngle, Radian(Math::HALF_PI * -0.06f));
		m_PitchingAngle = std::min(m_PitchingAngle, Radian(Math::HALF_PI * 0.92f));

		Real sl = m_SpinPoint.squaredLength();
		if(sl > spinmax)
		{
			const Real rate = std::sqrt(spinmax / sl);

			m_SpinPoint *= rate;
		}

		updateGui();
	}

	CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel);

	return true;
}

bool Frame::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertOISMouseButtonToCegui(id));

	switch(id)
	{
	case OIS::MB_Left:
		if(!m_PowerControlOn)
			m_RotatingCamera = true;

		break;
	case OIS::MB_Right:
		if(!m_PowerControlOn)
			m_PanningCamera = true;

		break;
	}

	return true;
}

bool Frame::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertOISMouseButtonToCegui(id));

	switch(id)
	{
	case OIS::MB_Left:
		m_RotatingCamera = false;

		break;
	case OIS::MB_Right:
		m_PanningCamera = false;

		break;
	}

	return true;
}

bool Frame::onGuiShut(const CEGUI::EventArgs& e)
{
	onQueryExit();

	return true;
}

bool Frame::onGuiLoadHost(const CEGUI::EventArgs&)
{
	std::string dir = getCurrentDirectory() + "\\Hosts\\";
	wxFileDialog dialog(NULL, "Choost a host script", dir, "", "*.lua", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if(dialog.ShowModal() == wxID_OK)
	{
		try
		{
			m_GameHost.reset(new LuaGameHost(dialog.GetPath().ToAscii()));
			m_GameHost->initialize(m_Game.get());
		}
		catch(const std::exception& e)
		{
			wxMessageBox(std::string("Error: ") + e.what(), "Host Script Loading Failed", wxOK | wxICON_ERROR);
		}
	}

	return true;
}

void Frame::setupGui()
{
	m_GuiRenderer.reset(new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr));
	m_GuiSystem.reset(new CEGUI::System(m_GuiRenderer.get()));
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"PoolGame.scheme");

	m_GuiSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	m_GuiSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
	m_GuiSystem->setDefaultTooltip("TaharezLook/Tooltip");

	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"Pool.layout"); 
	m_GuiSystem->setGUISheet(sheet);

	m_ImageBall = CEGUI::WindowManager::getSingleton().getWindow("Pool/SpinBackground");
	m_ImagePoint = CEGUI::WindowManager::getSingleton().getWindow("Pool/SpinPoint");
	m_ImagePowerSlotBase = CEGUI::WindowManager::getSingleton().getWindow("Pool/PowerSlot");
	m_ImagePowerSlotSurface = CEGUI::WindowManager::getSingleton().getWindow("Pool/PowerSlot_Surface");
	m_ImagePowerColumn = CEGUI::WindowManager::getSingleton().getWindow("Pool/PowerColumn");
	m_ImagePowerColumnMax = CEGUI::WindowManager::getSingleton().getWindow("Pool/PowerColumnMax");

	m_ImagePowerSlotBase->setVisible(false);
	m_ImagePowerSlotSurface->setVisible(false);
	m_ImagePowerColumn->setVisible(false);
	m_ImagePowerColumnMax->setVisible(false);

	m_ImageBall->setVisible(false);
	m_ImagePoint->setVisible(false);

	CEGUI::Window* root = CEGUI::WindowManager::getSingleton().getWindow("root");

	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Pool/Shut")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiShut, this));
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Pool/LoadHost")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiLoadHost, this));
}

void Frame::updateGui()
{
	m_ImageBall->setVisible(m_SpinControlOn);
	m_ImagePoint->setVisible(m_SpinControlOn);

	{
		static const Real radius = 45;
		static const Real centerx = 50;
		static const Real centery = 90;

		CEGUI::UDim dx(0, centerx + m_SpinPoint.x * radius);
		CEGUI::UDim dy(0, centery - m_SpinPoint.y * radius);
		m_ImagePoint->setPosition(CEGUI::UVector2(dx, dy));
	}

	m_ImagePowerSlotBase->setVisible(m_PowerControlOn);
	m_ImagePowerSlotSurface->setVisible(m_PowerControlOn);
	m_ImagePowerColumn->setVisible(m_PowerControlOn);
	m_ImagePowerColumnMax->setVisible(m_PowerControlOn);

	m_ImagePowerColumn->setHeight(CEGUI::UDim(std::pow(m_AmassDistance, 0.6f) * 0.12f, 0));
	m_ImagePowerColumnMax->setHeight(CEGUI::UDim(std::pow(m_AmassMax, 0.6f) * 0.12f, 0));
}

OgreListener* Frame::getListener() const
{
	return dynamic_cast<OgreListener*>(mFrameListener);
}

Vector2 Frame::getCursorPosition() const
{
	return static_cast<const OgreListener*>(mFrameListener)->getCursorPosition();
}

Vector2 Frame::worldToViewport(const Vector3& p) const
{
	Matrix4 view = mCamera->getViewMatrix();
	Matrix4 projection = mCamera->getProjectionMatrix();

	//Vector3 v = (view * projection) * p;
	Vector3 v = projection * view * p;

	const OgreListener* listener = static_cast<const OgreListener*>(mFrameListener);

	return Vector2((v.x + 1) * listener->getWindowWidth() / 2,
		(1 - v.y) * listener->getWindowHeight() / 2);
}

CEGUI::UVector2 Frame::oisToCegui(const Vector2& p) const
{
	const OgreListener* listener = static_cast<const OgreListener*>(mFrameListener);

	return CEGUI::UVector2(CEGUI::UDim((p.x - 5) / listener->getWindowWidth(), 0),
		CEGUI::UDim((p.y - 18) / listener->getWindowHeight(), 0));
}

void Frame::onQueryExit()
{
	OgreListener* listener = static_cast<OgreListener*>(mFrameListener);
	listener->exit();

	m_fnOnClose();
}

Frame::VisualObjectPtr Frame::createGameObject(const Billiards::VisualObjectParameters& param)
{
	return VisualObjectPtr(new GameObject(m_nodeGame, param));
}
