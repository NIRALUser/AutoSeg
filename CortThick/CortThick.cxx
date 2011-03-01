/*=========================================================================

  Program:   CortThick
  Module:    $RCSfile: CortThick.cxx,v $
  Language:  C++
  Date:      $Date: 2009/06/02 13:59:02 $
  Version:   $Revision: 1.4 $
  Author:    Delphine Ribes

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>


#include "CommandLineArgumentParser.h"
#include "PipelineCommand.h"

#define IV_VERSION "1.0"

void Help()
{
  std::cout<<" CortThick "<<IV_VERSION<<" - Compiled on: " << __DATE__ << " - "<< __TIME__ <<std::endl;
  std::cout<<"                 ------------------------------- "<<std::endl;
  std::cout<<"usage : CortThick -seg <SegImageFileName> <labelWhite> <labelGrey> <OutputDirectory> [options]"<<std::endl;
  std::cout<<"usage : CortThick -sep <WhiteMatterFileName> <GreyMatterFileName> <OutputDirectory> [options]"<<std::endl;
  
  std::cout << std::endl;
  std::cout << "Input:" << std::endl;
  std::cout << "-seg <WhiteMatterFileName> <GreyMatterFileName> <OutputDirectory>"<<std::endl;
  std::cout << "-sep <WhiteMatterFileName> <GreyMatterFileName> <OutputDirectory>"<<std::endl;
  
  std::cout<< std::endl;
  std::cout<<"CortThick -version :" << IV_VERSION <<std::endl;
  
  std::cout<<std::endl;  
  std::cout<<"Options: "<<std::endl;
  std::cout<<" -par <ParcellationFileName>  \t Parcellation file "<<std::endl;
  std::cout<<" -Wm \t write white matter distance map image average along boundary "<<std::endl;
  std::cout<<" -Gm \t write danielson map on the grey matter "<<std::endl;
  std::cout<<" -Wc: \t WhiteMatterComponent"<<std::endl;
  std::cout<<" -Gc: \t GreyMatterComponent"<<std::endl;
  std::cout<<" -Vtk: \t Write VtkFile "<<std::endl; 
  std::cout<<" -Sdm: \t Save cortical thickness on the white matter border (values in the histogram) "<<std::endl;
  std::cout<<" -BvsI:\t Write two images : boundary cortical thickness and non boundary cortical thickness " <<std::endl; 
  std::cout<<" -GMMaps: \t Write two images: the distance map on grey matter boundary and the average values along grey matter boundary " <<std::endl;
  std::cout<<" -Interp: \t Gather interpolated cortical thickness in 2 additional csv files" <<std::endl;
  std::cout<<"      -Threshold: \t Set the threshold used to match the cortical thickness map with the parcellation (default: 1.8mm) " <<std::endl;
}  

int main(int argc, char *argv[])
{
  if(argc == 1)
  {
    std::cerr<<"Not enough parameter"<<std::endl;
    Help();
    return -1;
  }
  
  if ((argc==2) && (!strcmp(argv[1],"-version")))
  {
    std::cout << "CortThick " << IV_VERSION << " - Compiled on: " << __DATE__ << " - " <<__TIME__  << std::endl;
    return 0;
  }
  else
  {     
    CommandLineArgumentParser parser;
    parser.AddOption("-sep",3);
    parser.AddOption("-seg",4);
    parser.AddOption("-Wc",0);
    parser.AddOption("-Gc",0);
    parser.AddOption("-Vtk",0);
    parser.AddOption("-h",0);
    parser.AddOption("-par",1);
    parser.AddOption("-Wm",0);
    parser.AddOption("-Gm",0);
    parser.AddOption("-BvsI",0);
    parser.AddSynonim("-h","-help");
    parser.AddOption("-Sdm",0);
    parser.AddOption("-GMMaps",0);
    parser.AddOption("-Interp",0);
    parser.AddOption("-Threshold",1);
                
    CommandLineArgumentParseResult parseResult;
        
    if(!parser.TryParseCommandLine(argc,argv,parseResult))
    {
      Help();
      return -1;
    }
    else
    {
      if (parseResult.IsOptionPresent("-h"))
      {
        Help();
        return 0;
      }        
    }
    
    bool SegImage = false;
    bool SepImage = false;
    bool WhiteComponent = false;
    bool GreyComponent = false;
    bool VtkFile = false;
    bool ParFile = false;
    bool WriteWhiteDistance = false;
    bool WriteGreyDistance = false;
    bool WriteBoundThickAndNonBoundThick = false;
    bool WriteDistanceMapWhiteImage = false;
    bool WriteDistBoundMapGreyMat = false;
    bool Interp = false;

    float Threshold = 1.8;

    const char* White;
    const char* grey;
    const char* output;
    const char* Segfile;
    const char* levelWhite;
    const char* levelGrey;
    const char* ParFileName;
    
    if(parseResult.IsOptionPresent("-seg"))
    {
      SegImage = true;
      Segfile = (parseResult.GetOptionParameter("-seg",0));
      levelWhite = (parseResult.GetOptionParameter("-seg",1));
      levelGrey = (parseResult.GetOptionParameter("-seg",2));
      output = (parseResult.GetOptionParameter("-seg",3));
    }
    if(parseResult.IsOptionPresent("-Wc"))    WhiteComponent  = true;   
    if(parseResult.IsOptionPresent("-Gc"))    GreyComponent  = true;   
    if(parseResult.IsOptionPresent("-Vtk"))   VtkFile  = true;   
    if(parseResult.IsOptionPresent("-sep"))
    {
      White = (parseResult.GetOptionParameter("-sep",0));
      grey = (parseResult.GetOptionParameter("-sep",1));
      output = (parseResult.GetOptionParameter("-sep",2));
      SepImage = true;
    }
    if(parseResult.IsOptionPresent("-par"))
    {
      ParFileName = (parseResult.GetOptionParameter("-par",0));
      ParFile = true;
    }
    else if(!parseResult.IsOptionPresent("-par"))   ParFileName = "retret"; //avoid segmenation fault
    if(parseResult.IsOptionPresent("-Wm"))   WriteWhiteDistance = true;
    if(parseResult.IsOptionPresent("-Gm"))   WriteGreyDistance = true;  
    if(parseResult.IsOptionPresent("-BvsI"))  WriteBoundThickAndNonBoundThick  = true; 
    if(parseResult.IsOptionPresent("-Sdm")) WriteDistanceMapWhiteImage = true;   
    if(parseResult.IsOptionPresent("-GMMaps")) WriteDistBoundMapGreyMat = true;
    if(parseResult.IsOptionPresent("-Interp")) Interp = true;
    if(parseResult.IsOptionPresent("-Threshold"))
    {
	Threshold=atof(parseResult.GetOptionParameter("-Threshold",0));
    }

    //AJOUTER POUR LES IMAGEVGUICONTROLS FUNCTION UN NOUVEAU PARAMETRE BOOL "WriteDistBoundMapGreyMat" 
    // ENSUITE LES AJOUTER DANS LE CONSTRUCTEUR DE IMAGEVGUICONTROLS
    // PUIS DANS LE CONSTRUCTEUR DE PIPELINECOMMAND, ICI CREER LES NOUVEAUX NOMS POUR LES FICHIERS DANS COMPUTEGUI
    // ET METTRE A JOUR DANS IMAGESYL !

    //Run Cortical Thickness Programm
    if(SepImage)
       PipelineCommand CortThickPipeline(White,grey,output,WhiteComponent,GreyComponent,VtkFile, ParFileName, ParFile,WriteWhiteDistance,WriteGreyDistance, WriteBoundThickAndNonBoundThick,WriteDistanceMapWhiteImage,WriteDistBoundMapGreyMat,Interp, Threshold );
    else if(SegImage)
      PipelineCommand CortThickPipeline( atoi(levelWhite), atoi(levelGrey), Segfile,output, WhiteComponent, GreyComponent, VtkFile, ParFileName, ParFile,WriteWhiteDistance,WriteGreyDistance,WriteBoundThickAndNonBoundThick, WriteDistanceMapWhiteImage, WriteDistBoundMapGreyMat,Interp, Threshold  );
    
    return 0;
  }
}
