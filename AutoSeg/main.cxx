/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: main.cxx,v $
  Language:  C++
  Date:      $Date: 2010/06/30 13:19:06 $
  Version:  
  Author:    Clement Vachet, Jiahui Wang

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>
#include "argio.h"

#include "AutoSegGUIControls.h"
#include "AutoSegComputation.h"
#include <itksys/SystemTools.hxx>

#define AUTOSEG_VERSION "3.3.1"

void PrintHelp(char* progname)
{
    std::cout << "AutoSeg "<<AUTOSEG_VERSION<<" - Compiled on: " << __DATE__ << " - "<< __TIME__ <<std::endl;
    std::cout << " performs automatic brain tissue classification and structural segmentation" << std::endl;
    std::cout << "usage: AutoSeg [-computationFile computationFile] [-parameterFile parameterFile]"<< std::endl; std::cout << std::endl; std::cout << "-computationFile	computation file" << std::endl;
    std::cout << "-parameterFile	parameter file" << std::endl;
    std::cout << "-gui	                open AutoSeg interface" << std::endl;
    std::cout << "-version	        version number" << std::endl;
    std::cout << "-logFile              log file for standard logging (not during execution)" << std::endl;
    std::cout << "-h/-help              print this info" << std::endl;
    std::cout << std::endl << std::endl;
}

int main(int argc, char *argv[])
{
  if (argc <=1 || ipExistsArgument(argv, "-h") || ipExistsArgument(argv, "--help"))
  {
    PrintHelp(argv[0]);
    exit(0);
  }
  bool gui = ipExistsArgument(argv, "-gui");
  bool version = ipExistsArgument(argv, "-version");
  const char *computationFile = ipGetStringArgument(argv, "-computationFile", NULL);
  const char *parameterFile = ipGetStringArgument(argv, "-parameterFile", NULL);
  std::string logFilename = ipGetStringArgument(argv, "-logFile", "");
  std::string logErrorFilename ;
  if( !logFilename.empty() )
  {
    logErrorFilename = std::string(logFilename) + ".err";
  }

  std::string executableFullPath = itksys::SystemTools::CollapseFullPath( argv[0] ).c_str() ;
  std::string executableDirectory ;
  std::string filename ;
  itksys::SystemTools::SplitProgramPath( executableFullPath , executableDirectory , filename ) ;
  std::string originalPATH ;
  itksys::SystemTools::GetEnv("PATH", originalPATH );
  #ifdef WIN32
  itksys::SystemTools::PutEnv( std::string( "PATH=" ) + executableDirectory + ";" + originalPATH ) ;
  #else
  itksys::SystemTools::PutEnv( std::string( "PATH=" ) + executableDirectory + ":" + originalPATH ) ;
  #endif
  if (gui)
  {
    std::string AutoSegPath ;
    itksys::SystemTools::GetEnv("AUTOSEG_HOME" , AutoSegPath ) ;
    if( AutoSegPath.empty() )
    {
       AutoSegPath = executableDirectory.c_str() ;
       std::cout << "The environment variable 'AUTOSEG_HOME' was not set." << std::endl ;
       std::cout << "Using default value (directory containing current executable): "
                 << AutoSegPath << std::endl ;
       std::cout << "Instructions to set the environment variable:" << std::endl ;
       std::cout << "bash usage : export AUTOSEG_HOME=<InputDirectory>" << std::endl ;
       std::cout << "tcsh usage : setenv AUTOSEG_HOME <InputDirectory>" << std::endl ;
    }
    AutoSegGUIControls *MainWindow = new AutoSegGUIControls(AutoSegPath,
                                                            AUTOSEG_VERSION ,
                                                            computationFile ,
                                                            parameterFile,
                                                            logFilename,
                                                            logErrorFilename
                                                            );
    Fl::scheme("plastic");
    Fl::run();
    delete MainWindow;
    return 0;
  }
  else if ( computationFile && parameterFile)
  {
    AutoSegComputation m_Computation;
    m_Computation.SetStdOutLogFile( logFilename ) ;
    m_Computation.SetStdErrLogFile( logErrorFilename ) ;
    m_Computation.ComputationWithoutGUI(computationFile, parameterFile);
  }
  else if (version)
  { 
    std::cout<<"AutoSeg "<<AUTOSEG_VERSION<<" - Compiled on: " << __DATE__ << " - "<< __TIME__ <<std::endl;
  }
  else
  {
    PrintHelp(argv[0]);
  }
}

