
/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: main.cxx,v $
  Language:  C++
  Date:      $Date: 2010/06/30 13:19:06 $
  Version:   $Revision: 1.5 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>

#include "AutoSegGUIControls.h"

#define AUTOSEG_VERSION "1.2"

void PrintHelp(char* progname)
{
	std::cerr<<"Usage: "<<progname<<std::endl;
	std::cerr<<"Available options:"<<std::endl;
	std::cerr<<"  [ -h ] or [ --help ]"<<std::endl;
	std::cerr<<"    = List options"<<std::endl;
	std::cerr<<"  [ -v ] or [ --version ]"<<std::endl;
	std::cerr<<"    = Return the version number"<<std::endl;
	std::cerr<<std::endl;
}

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		const char *AutoSegHome = "AUTOSEG_HOME";
		char *AutoSegPath = NULL;

		AutoSegPath = getenv(AutoSegHome);
		if (AutoSegPath != NULL)
		{
			AutoSegGUIControls *MainWindow = new AutoSegGUIControls(AutoSegPath);  
			Fl::scheme("plastic");
			Fl::run();
			delete MainWindow;
			return 0;
		}
		else
		{
			std::cerr<<"The environment variable 'AUTOSEG_HOME' needs to be set"<<std::endl;
			std::cerr<<"bash usage : export AUTOSEG_HOME=<InputDirectory>"<<std::endl;
			std::cerr<<"tcsh usage : setenv AUTOSEG_HOME <InputDirectory>"<<std::endl;
			return -1;
		}
	}
  
	else if ( (argc == 2) && ( (!strcmp(argv[1],"-v")) || (!strcmp(argv[1],"--version")) ) )
	{ 
		std::cout<<"AutoSeg "<<AUTOSEG_VERSION<<" - Compiled on: " << __DATE__ << " - "<< __TIME__ <<std::endl;
		return 0; 
	}
	else
	{
		PrintHelp(argv[0]);
		return -1;
	}
}

