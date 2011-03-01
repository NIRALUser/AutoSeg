/*=========================================================================

  Program:   CortThick
  Module:    $RCSfile: CortThickCLP.cxx,v $
  Language:  C++
  Date:      $Date: 2009/06/01 16:28:07 $
  Version:   $Revision: 1.3 $
  Author:    Delphine Ribes

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>
#include <cstring>

#include "PipelineCommand.h"

#include "CortThickCLPCLP.h"

#define IV_VERSION "1.0"


int main(int argc, char *argv[])
{
  PARSE_ARGS;

  bool SegImage = false;
  bool SepImage = false;
  bool ParFile = false;    
  

  if (!SegmentationInput.empty())
    SegImage = true;
  if ( (!WhiteMatterInput.empty()) && (!GreyMatterInput.empty()))
    SepImage = true;
  
  if(!ParcellationOption.empty())
    ParFile = true;
 

  std::cout<<"WhiteMatterInput: "<<WhiteMatterInput.c_str()<<std::endl;
  std::cout<<"GreyMatterInput: "<<GreyMatterInput.c_str()<<std::endl;
  std::cout<<"WhiteMatterLabel: "<<WhiteMatterLabel<<std::endl;
  std::cout<<"GreyMatterLabel: "<<GreyMatterLabel<<std::endl;
  std::cout<<"SegmentationInput: "<<SegmentationInput.c_str()<<std::endl;
  std::cout<<"Output: "<<Output.c_str()<<std::endl;
  std::cout<<"WhiteMatterComponentOption: "<<WhiteMatterComponentOption<<std::endl;
  std::cout<<"GreyMatterComponentOption: "<<GreyMatterComponentOption<<std::endl;
  std::cout<<"VTKOption: "<<VTKOption<<std::endl;
  std::cout<<"ParcellationOption: "<<ParcellationOption.c_str()<<std::endl;
  std::cout<<"ParFile: "<<ParFile<<std::endl;
  std::cout<<"SaveWM: "<<WhiteMatterFile<<std::endl;
  std::cout<<"SaveGM: "<<GMMapsAvgDist<<std::endl;
  //std::cout<<"GreyMatterOption: "<<GreyMatterOption<<std::endl;
  std::cout<<"BoundaryVsInteriorOption: "<<BoundaryVsInteriorOption<<std::endl;
  //std::cout<<"SdmOption: "<<SdmOption<<std::endl;
  //std::cout<<"GreyMatterMapsOption: "<<GreyMatterMapsOption<<std::endl<<std::endl;
  
  int SegImageSize = SegmentationInput.length();
  std::cout<<"SegImageSize: "<<SegImageSize<<std::endl;
  std::cout<<"SegImage: "<<SegImage<<std::endl<<std::endl;
  
  int WhiteMatterImageSize = WhiteMatterInput.length();
  int GreyMatterImageSize = GreyMatterInput.length();
  std::cout<<"WhiteMatterImageSize: "<<WhiteMatterImageSize<<std::endl;
  std::cout<<"GreyMatterImageSize: "<<GreyMatterImageSize<<std::endl;
  std::cout<<"SepImage: "<<SepImage<<std::endl;
    
    //AJOUTER POUR LES IMAGEVGUICONTROLS FUNCTION UN NOUVEAU PARAMETRE BOOL "WriteDistBoundMapGreyMat" 
    // ENSUITE LES AJOUTER DANS LE CONSTRUCTEUR DE IMAGEVGUICONTROLS
    // PUIS DANS LE CONSTRUCTEUR DE PIPELINECOMMAND, ICI CREER LES NOUVEAUX NOMS POUR LES FICHIERS DANS COMPUTEGUI
    // ET METTRE A JOUR DANS IMAGESYL !

    //Run Cortical Thickness Programm
    if(SepImage)
      PipelineCommand CortThickCLPPipeline(WhiteMatterInput.c_str(),GreyMatterInput.c_str(),Output.c_str(), WhiteMatterComponentOption,GreyMatterComponentOption,VTKOption, ParcellationOption.c_str(), ParFile, WhiteMatterFile.c_str(),GreyMatterFile.c_str(), BoundaryVsInteriorOption,SdmFile.c_str() ,GMMapsDist.c_str(),GMMapsAvgDist.c_str(),Interp,Threshold);
    else if(SegImage)
      PipelineCommand CortThickCLPPipeline( WhiteMatterLabel, GreyMatterLabel, SegmentationInput.c_str(), Output.c_str(), WhiteMatterComponentOption,GreyMatterComponentOption,VTKOption, ParcellationOption.c_str(), ParFile, WhiteMatterFile.c_str(),GreyMatterFile.c_str(), BoundaryVsInteriorOption, SdmFile.c_str(),GMMapsDist.c_str(),GMMapsAvgDist.c_str(),Interp,Threshold);
         
    return 0;  
}
