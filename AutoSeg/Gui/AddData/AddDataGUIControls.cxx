/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: AddDataGUIControls.cxx,v $
  Language:  C++
  Date:      $Date: 2011/01/27 15:07:54 $
  Version:   $Revision: 1.6 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "AddDataGUIControls.h"

AddDataGUIControls::AddDataGUIControls()
  : AddDataGUI()
{}

AddDataGUIControls::AddDataGUIControls(bool _T2Image, bool _PDImage)
  : AddDataGUI()
{
  SetT2Image(_T2Image);
  SetPDImage(_PDImage);

  if (GetT2Image())
  {
    g_T2File->activate();
    g_T2FileDisp->activate();
  }
  if (GetPDImage())
  {
    g_PDFile->activate();
    g_PDFileDisp->activate();
  }

  SetT1File("");
  SetT2File("");
  SetPDFile("");
}

AddDataGUIControls::~AddDataGUIControls()
{}

void AddDataGUIControls::OKButtonPressed()
{
  bool InputChecked;
 
  InputChecked = CheckInput();

  if (InputChecked == false) 
    {
      SetT1File(g_T1FileDisp->value());
      if (GetT2Image())
	SetT2File(g_T2FileDisp->value());
      if (GetPDImage())
	SetPDFile(g_PDFileDisp->value());
      g_MainWindow->hide();  
    }
}

void AddDataGUIControls::CancelButtonPressed()
{
  g_MainWindow->hide();
}

bool AddDataGUIControls::CheckInput()
{
  bool Warning = false;

  if (std::strlen(g_T1FileDisp->value()) == 0)
    {
      fl_message("Please, set the T1 image...");
      Warning = true;
    }
  else if ( (GetT2Image()) && (std::strlen(g_T2FileDisp->value()) == 0))
    {
      fl_message("Please, set the T2 image...");
      Warning = true;
    }
  else if ( (GetPDImage()) && (std::strlen(g_PDFileDisp->value()) == 0))
    {
      fl_message("Please, set the PD image...");
      Warning = true;
    }  
  return Warning;
}

void AddDataGUIControls::SetT1FileGUI()
{
  char *T1Image = NULL;
  
  T1Image = fl_file_chooser("Set a T1 file", "Images (*.{gipl,gipl.gz,mhd,mha,img,hdr,nhdr,nrrd,nii})",NULL);
  if (T1Image != NULL)
    {
      g_T1FileDisp->value(T1Image); 
      g_T1FileDisp->position(g_T1FileDisp->size());
    }
}

void AddDataGUIControls::SetT2FileGUI()
{
  char *T2Image = NULL;
  
  T2Image = fl_file_chooser("Set a T2 file", "Images (*.{gipl,gipl.gz,mhd,mhd,mha,img,hdr,nhdr,nrrd,nii})",NULL);
  if (T2Image != NULL)
    {
      g_T2FileDisp->value(T2Image);
      g_T2FileDisp->position(g_T2FileDisp->size());
    }
}

void AddDataGUIControls::SetPDFileGUI()
{
  char *PDImage = NULL;

  PDImage = fl_file_chooser("Set a PD file", "Images (*.{gipl,gipl.gz,mhd,mhd,mha,img,hdr,nhdr,nrrd,nii})",NULL);
  if (PDImage != NULL)
    {
      g_PDFileDisp->value(PDImage);     
      g_PDFileDisp->position(g_PDFileDisp->size());
    }
}






