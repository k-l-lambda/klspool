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


namespace Billiards
{
	class Game;
	struct IVisualObject;
	struct VisualObjectParameters;
}

struct GameHost;


class Frame
	: public wxFrame
	, public ExampleApplication
{
	friend class OgreListener;


public:
	typedef	boost::function<void ()>						OnCloseFunctor;

	typedef	boost::shared_ptr<Billiards::IVisualObject>		VisualObjectPtr;
	typedef	boost::shared_ptr<Billiards::Game>				GamePtr;
	typedef	boost::shared_ptr<GameHost>						GameHostPtr;

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

	OnCloseFunctor						m_fnOnClose;

	bool								m_RotatingCamera;
	bool								m_PanningCamera;
	bool								m_Staring;

	Radian								m_SkyBoxAngle;

	int									m_FocusDialog;

	GamePtr								m_Game;

	GameHostPtr							m_GameHost;

	Ogre::Timer							m_timer;

	Real								m_AmassDistance;
	Real								m_AmassMax;

	DECLARE_EVENT_TABLE()
};



#endif	// !defined(__FRAME_H__)
