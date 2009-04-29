/*
**	This source file is part of Pool.
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
//#define	WRITE_BALLCONFIGSET_FILES


#include "Frame.h"

#include <wx\evtloop.h>


IMPLEMENT_APP(wxApp)



#ifdef	WRITE_BALLCONFIGSET_FILES

#	include "..\Billiards\BallConfig.h"

static void	saveBallConfigSet(const std::string& name, const Billiards::BallConfigSet& set)
{
	Billiards::BallConfigSet::saveFile("./BallConfigSets/" + name + ".xml", set);
}

static void	writeBallConfigSetFiles()
{
	Billiards::BallConfig balls[] =
	{
		{"Main", 0.27f, 1, "White"},
		{"1#", 0.27f, 1, "P1"},
		{"2#", 0.27f, 1, "P2"},
		{"3#", 0.27f, 1, "P3"},
	};

	Billiards::BallConfigSet stdset;
	stdset.NameSpace = "std";
	for(size_t i = 0; i < sizeof(balls) / sizeof(Billiards::BallConfig); ++ i)
		stdset.insert(balls[i]);

	saveBallConfigSet("std", stdset);
}

#endif	// defined(WRITE_BALLCONFIGSET_FILES)


extern "C" __declspec(dllexport) int	startup()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

#ifdef	WRITE_BALLCONFIGSET_FILES
	writeBallConfigSetFiles();
#endif	// defined(WRITE_BALLCONFIGSET_FILES)

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
		catch(const Ogre::Exception& e)
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
