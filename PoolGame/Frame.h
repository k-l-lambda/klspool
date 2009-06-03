/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__FRAME_H__
#define	__FRAME_H__



#include "ExampleApplication.h"
#include "OgreCEGUIRenderer.h"
#include "CEGUI/CEGUIImagesetManager.h"


namespace Billiards
{
	class Game;
	struct IVisualObject;
	struct VisualObjectParameters;
}


class Frame
	: public wxFrame
	, public ExampleApplication
{
	friend class OgreListener;


public:
	typedef	boost::function<void ()>						OnCloseFunctor;

	typedef	boost::shared_ptr<Billiards::IVisualObject>		VisualObjectPtr;
	typedef	boost::shared_ptr<Billiards::Game>				GamePtr;

public:
	explicit Frame(const OnCloseFunctor& fnOnClose);
	~Frame();

private:
	virtual void createFrameListener();

	// Just override the mandatory create scene method
	virtual void createScene();

private:
	void	onIdle(wxIdleEvent& e);
	void	onClose(wxCloseEvent& e);

private:
	void	frameStarted(const FrameEvent& evt);
	void	frameEnded(const FrameEvent& evt);

	bool	keyPressed(const OIS::KeyEvent& e);
	bool	keyReleased(const OIS::KeyEvent& /*arg*/);

	bool	mouseMoved(const OIS::MouseEvent &arg);
	bool	mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool	mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	bool	onGuiShut(const CEGUI::EventArgs& e);

private:
	void	setupGui();

	OgreListener*	getListener() const;

	Vector2	getCursorPosition() const;

	Vector2	worldToViewport(const Vector3& p) const;
	CEGUI::UVector2	oisToCegui(const Vector2& p) const;

	void	onQueryExit();

	VisualObjectPtr	createGameObject(const Billiards::VisualObjectParameters& param);

private:
	// UI commands

private:
	SceneNode*							m_nodeLight;
	SceneNode*							m_nodeLight2;
	SceneNode*							m_nodeGame;
	SceneNode*							m_nodeCameraRoot;
	SceneNode*							m_nodeCamera;
	SceneNode*							m_nodeCue;

	boost::shared_ptr<CEGUI::Renderer>	m_GuiRenderer;
	boost::shared_ptr<CEGUI::System>	m_GuiSystem;

	Ogre::Vector2                       m_PointPosition;

	CEGUI::Window*                      m_ImagePoint;
    CEGUI::Window*                      m_ImagePowerSlotBase;
    CEGUI::Window*                      m_ImageBall;
    CEGUI::Window*                      m_ImagePowerSlotSurface;

	OnCloseFunctor						m_fnOnClose;

	bool								m_RotatingCamera;
	bool								m_PanningCamera;
	bool								m_Staring;

	bool                                m_ShootAble;
	bool                                m_PowerControlerOn;
	bool                                m_PointControlerOn;

	Radian								m_SkyBoxAngle;

	int									m_FocusDialog;

	GamePtr								m_Game;

	Ogre::Timer							m_timer;

	Real								m_AmassDistance;
	Real								m_AmassMax;

	const Real                          m_DistanceMax;

	DECLARE_EVENT_TABLE()
};



#endif	// !defined(__FRAME_H__)
