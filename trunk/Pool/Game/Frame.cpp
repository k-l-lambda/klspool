/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "../Billiards/BldGame.h"

#include <Commdlg.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include "Frame.h"
#include "Geometries.h"
#include "mathTypeConvert.h"


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
, m_fnOnClose(fnOnClose)
, m_RotatingGame(false)
, m_SkyBoxAngle((timeGetTime() % DWORD(Math::PI * 2000 / SKYBOX_ROTATE_SPEED)) * SKYBOX_ROTATE_SPEED / 1000)
, m_FocusDialog(wxID_ANY)
{
}

Frame::~Frame()
{
}

void Frame::createFrameListener()
{
	// This is where we instantiate our own frame listener
	mFrameListener= new OgreListener(this, mWindow, mCamera);
	mRoot->addFrameListener(mFrameListener);
}

void Frame::createScene()
{
	{
#pragma message("Update version info: file:"__FILE__" date:"__DATE__" time:"__TIME__)
		static const char* const VERSIONINFO = "[VERSION] Pool version: File:"__FILE__", Date:"__DATE__", Time:"__TIME__;
		LogManager::getSingleton().logMessage(VERSIONINFO, LML_CRITICAL);
	}

	m_nodeCameraRoot = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	// Create 2 point lights
	Light* l1 = mSceneMgr->createLight("MainLight");
	l1->setAttenuation(100000, 0.4f, 0, 0);
	l1->setSpecularColour(0.3f, 0.3f, 0.3f);
	m_nodeLight = m_nodeCameraRoot->createChildSceneNode();
	m_nodeLight->createChildSceneNode(Vector3(40, 20, 100))->attachObject(l1);

	Light* l2 = mSceneMgr->createLight("AssistLight");
	l2->setDiffuseColour(0.2f, 0.2f, 0.2f);
	l2->setSpecularColour(0.1f, 0.1f, 0.1f);
	m_nodeLight2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	m_nodeLight2->createChildSceneNode(Vector3(-40, 60, 30))->attachObject(l2);

	mSceneMgr->setAmbientLight(ColourValue(0.6f, 0.6f, 0.6f));

	setupGui();

	m_nodeCameraRoot->setOrientation(Quaternion(m_SkyBoxAngle, Vector3::UNIT_Y));
	m_nodeCamera = m_nodeCameraRoot->createChildSceneNode(Vector3(0, 120, 90));
	mCamera->setPosition(m_nodeCamera->_getDerivedPosition());
	mCamera->lookAt(0,0,0);

	createSphere("Sphere", 12, 12, 3);
	m_nodeGame = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	// only a sample
	{
		Ogre::Entity* ball1 = mSceneMgr->createEntity("ball1","Sphere");
		ball1->getSubEntity(0)->setMaterialName("Pool/Balls/P1");
		m_nodeBall_1 = m_nodeGame->createChildSceneNode();
		m_nodeBall_1->attachObject(ball1);
		Ogre::Vector3 position(0, 0, 0);
		m_nodeBall_1->setPosition(position);

		Ogre::Entity* ball2 = mSceneMgr->createEntity("ball2","Sphere");
		ball2->getSubEntity(0)->setMaterialName("Pool/Balls/P2");
		m_nodeBall_2 = m_nodeGame->createChildSceneNode();
		m_nodeBall_2->attachObject(ball2);
		m_nodeBall_2->setPosition(position);

		Ogre::Entity* entGround = mSceneMgr->createEntity("ground","floor200x200.mesh");
		entGround->getSubEntity(0)->setMaterialName("Pool/Table/RedGrass");
		Ogre::SceneNode* groundNode = m_nodeGame->createChildSceneNode("groundnode");

		groundNode->scale(1,0.2,1);

		groundNode->attachObject(entGround);

		groundNode->setPosition(Ogre::Vector3(0, 0, 0));
	}

	mSceneMgr->setSkyBox(true, "Pool/SkyBox");

	mWindow->getViewport(0)->setBackgroundColour(ColourValue(0.2, 0.2, 0.2));

	// init physics system
	m_billiards = new Billiards::BldGame();
	m_billiards->setup();
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
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	// restrict elapsed time by a upper limit
	const Real elapsed = std::min(evt.timeSinceLastFrame, 0.2f);

	{
		// step the world
		m_billiards->simulate();

		{			
			m_nodeBall_1->setPosition(hkVector4ToOgre( m_billiards->getPosOfBall(0) ) );
			m_nodeBall_1->setOrientation( hkQuatToOgre( m_billiards->getRotationOfBall(0) ) );
			
			m_nodeBall_2->setPosition(hkVector4ToOgre( m_billiards->getPosOfBall(1) ) );
			m_nodeBall_2->setOrientation( hkQuatToOgre( m_billiards->getRotationOfBall(1) ) );
		}

		Radian delta = Radian(elapsed * SKYBOX_ROTATE_SPEED);
		m_SkyBoxAngle += delta;

		// light animation
		m_nodeLight2->yaw(delta);
		//m_nodeLight->yaw(delta);

		// rotate camera
		//m_nodeCameraRoot->setOrientation(Quaternion(m_SkyBoxAngle, Vector3::UNIT_Y));
		mCamera->setPosition(m_nodeCamera->_getDerivedPosition()/* + 50*/);
		mCamera->lookAt(m_nodeGame->_getDerivedPosition());
		//mCamera->lookAt(0, 0, 0);
	}
}

void Frame::frameEnded(const FrameEvent& evt)
{
	// yield CPU when not busy
	if(!static_cast<const OgreListener*>(mFrameListener)->mouseIsMoving())
		Sleep(15);
}

bool Frame::keyPressed(const OIS::KeyEvent& e)
{
	return true;
}

bool Frame::keyReleased(const OIS::KeyEvent& /*arg*/)
{
	return true;
}

bool Frame::mouseMoved(const OIS::MouseEvent& e)
{
	if(m_RotatingGame)
	{
		// rotate magic cube
		m_nodeCameraRoot->yaw(Radian(-e.state.X.rel * 0.01f), Node::TS_PARENT);
		//m_nodeCamera->pitch(Radian(e.state.Y.rel * 0.01f), Node::TS_PARENT);
	}

	// update camera
	{
		static const Real NEAR_DISTANCE = 24;
		static const Real FAR_DISTANCE = 1200;

		Vector3 camera = m_nodeCamera->getPosition();
		if(m_RotatingGame && e.state.Y.rel)
		{
			/*const Real x0 = camera.x, z0 = camera.z;
			const Real yaw = e.state.X.rel * 0.01f;
			const Real cos_yaw = std::cos(yaw), sin_yaw = std::sin(yaw);
			camera.x = x0 * cos_yaw - z0 * sin_yaw;
			camera.z = z0 * cos_yaw + x0 * sin_yaw;*/
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
		if(e.state.Z.rel)
		{
			camera *= exp(-e.state.Z.rel / 1600.0);
			if(camera.length() < NEAR_DISTANCE)
				camera = camera.normalisedCopy() * NEAR_DISTANCE;
			else if(camera.length() > FAR_DISTANCE)
				camera = camera.normalisedCopy() * FAR_DISTANCE;
		}

		m_nodeCamera->setPosition(camera);
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
		m_RotatingGame = true;

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
		if(m_RotatingGame)
			m_RotatingGame = false;

		break;
	case OIS::MB_Right:

		break;
	}

	return true;
}

bool Frame::onGuiShut(const CEGUI::EventArgs& e)
{
	onQueryExit();

	return true;
}

void Frame::setupGui()
{
	m_GuiRenderer.reset(new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr));
	m_GuiSystem.reset(new CEGUI::System(m_GuiRenderer.get()));
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
	m_GuiSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	m_GuiSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
	m_GuiSystem->setDefaultTooltip("TaharezLook/Tooltip");

	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"Pool.layout"); 
	m_GuiSystem->setGUISheet(sheet);

	CEGUI::Window* root = CEGUI::WindowManager::getSingleton().getWindow("root");

	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Pool/Shut")
		->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Frame::onGuiShut, this));
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
