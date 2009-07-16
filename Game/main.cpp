/*
**	This source file is part of FrameTemplate.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include <ctime>

#include <Ogre.h>

#ifdef	_DEBUG
#	pragma comment(lib, "OgreMain_d.lib")
#	pragma comment(lib, "OIS_d.lib")
#	pragma comment(lib, "CEGUIBase_d.lib")
#	pragma comment(lib, "OgreGUIRenderer_d.lib")
#else
#	pragma comment(lib, "OgreMain.lib")
#	pragma comment(lib, "OIS.lib")
#	pragma comment(lib, "CEGUIBase.lib")
#	pragma comment(lib, "OgreGUIRenderer.lib")
#endif


#if defined(DEBUG) | defined(_DEBUG)
#	include <crtdbg.h>
#endif


#define	_CONSOLE
#define	WRITE_STDOULINE_FILES


#include "Frame.h"

#include <wx\evtloop.h>


IMPLEMENT_APP(wxApp)



extern "C" __declspec(dllexport) int	startup()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	if(wxInitialize())
	{
		wxSetInstance(GetModuleHandle(NULL));

		try
		{
			wxEventLoop evtloop;

			// Create application object
			Frame frame(boost::bind(&wxEventLoop::Exit, &evtloop, 0));

			//frame.go();
			if(frame.setup())
			{
				evtloop.Run();
			}
		}
		catch(const Exception& e)
		{
			MessageBox(NULL, e.getFullDescription().c_str(), "An OGRE exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}
		catch(const std::exception& e)
		{
			MessageBox(NULL, e.what(), "An standard exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}

		wxUninitialize();
	}
	else
	{
		std::cout << "wxInitialize failed." << std::endl;
		system("pause");

		return 1;
	}


	return 0;
}


#ifdef	_CONSOLE
int main(int argc, char **argv)
#else
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#endif
{
	return startup();
}
