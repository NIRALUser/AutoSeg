/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: AddAuxDataGUIControls.cxx,v $
  Language:  C++
  Date:      $Date: 2010/07/02 15:24:04 $
  Version:   $Revision: 1.2 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "AddAuxDataGUIControls.h"

AddAuxDataGUIControls::AddAuxDataGUIControls()
  : AddAuxDataGUI()
{}
AddAuxDataGUIControls::AddAuxDataGUIControls(bool _AuxT1Image, bool _AuxT2Image, bool _AuxPDImage, bool _Aux1Image, bool _Aux2Image, bool _Aux3Image, bool _Aux4Image, bool _Aux5Image, bool _Aux6Image, bool _Aux7Image, bool _Aux8Image)
  : AddAuxDataGUI()
{
	
  SetAuxT1Image(_AuxT1Image);
  SetAuxT2Image(_AuxT2Image);
  SetAuxPDImage(_AuxPDImage);  
	
  SetAux1Image(_Aux1Image);
  SetAux2Image(_Aux2Image);
  SetAux3Image(_Aux3Image);
  SetAux4Image(_Aux4Image);
  SetAux5Image(_Aux5Image);
  SetAux6Image(_Aux6Image);
  SetAux7Image(_Aux7Image);
  SetAux8Image(_Aux8Image);

  if (GetAuxT1Image())
  {
    g_AuxT1File->activate();
    g_AuxT1FileDisp->activate();
  }
  else if (GetAuxT2Image())
  {
    g_AuxT2File->activate();
    g_AuxT2FileDisp->activate();
  }
  else if (GetAuxPDImage())
  {
    g_AuxPDFile->activate();
    g_AuxPDFileDisp->activate();
  }
  
  if (GetAux1Image())
  {
    g_Aux1File->activate();
    g_Aux1FileDisp->activate();
    
    if(GetAux2Image())
    {
      g_Aux2File->activate();
      g_Aux2FileDisp->activate();
    }
    if((GetAux3Image()) && (GetAux2Image()))
    {
      g_Aux3File->activate();
      g_Aux3FileDisp->activate();
    }
    if((GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()))
    {
      g_Aux4File->activate();
      g_Aux4FileDisp->activate();
    }
    if((GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()))
    {
      g_Aux5File->activate();
      g_Aux5FileDisp->activate();
    }
    if((GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()))
    {
      g_Aux6File->activate();
      g_Aux6FileDisp->activate();
    }
    if((GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()))
    {
      g_Aux7File->activate();
      g_Aux7FileDisp->activate();
    }
    if((GetAux8Image()) && (GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()))
    {
      g_Aux8File->activate();
      g_Aux8FileDisp->activate();
    }
  }
  if ((GetAux2Image()) && (GetAux1Image()))
  {
    g_Aux2File->activate();
    g_Aux2FileDisp->activate();
    
    if(GetAux3Image())
    {
      g_Aux3File->activate();
      g_Aux3FileDisp->activate();
    }
    if((GetAux4Image()) && (GetAux3Image()))
    {
      g_Aux4File->activate();
      g_Aux4FileDisp->activate();
    }
    if((GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()))
    {
      g_Aux5File->activate();
      g_Aux5FileDisp->activate();
    }
    if((GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()))
    {
      g_Aux6File->activate();
      g_Aux6FileDisp->activate();
    }
    if((GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()))
    {
      g_Aux7File->activate();
      g_Aux7FileDisp->activate();
    }
    if((GetAux8Image()) && (GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()))
    {
      g_Aux8File->activate();
      g_Aux8FileDisp->activate();
    }
  }
  if ((GetAux3Image()) && (GetAux2Image()) && (GetAux1Image()))
  {
    g_Aux3File->activate();
    g_Aux3FileDisp->activate();

    if(GetAux4Image())
    {
      g_Aux4File->activate();
      g_Aux4FileDisp->activate();
    }
    if((GetAux5Image()) && (GetAux4Image()))
    {
      g_Aux5File->activate();
      g_Aux5FileDisp->activate();
    }
    if((GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()))
    {
      g_Aux6File->activate();
      g_Aux6FileDisp->activate();
    }
    if((GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()))
    {
      g_Aux7File->activate();
      g_Aux7FileDisp->activate();
    }
    if((GetAux8Image()) && (GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()))
    {
      g_Aux8File->activate();
      g_Aux8FileDisp->activate();
    }
  }
  if ((GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()) && (GetAux1Image()))
  {
    g_Aux4File->activate();
    g_Aux4FileDisp->activate();
	  
    if(GetAux5Image())
    {
      g_Aux5File->activate();
      g_Aux5FileDisp->activate();
    }
    if((GetAux6Image()) && (GetAux5Image()))
    {
      g_Aux6File->activate();
      g_Aux6FileDisp->activate();
    }
    if((GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()))
    {
      g_Aux7File->activate();
      g_Aux7FileDisp->activate();
    }
    if((GetAux8Image()) && (GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()))
    {
      g_Aux8File->activate();
      g_Aux8FileDisp->activate();
    }
  }
  if ((GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()) && (GetAux1Image()))
  {
    g_Aux5File->activate();
    g_Aux5FileDisp->activate();
	  
    if(GetAux6Image())
    {
      g_Aux6File->activate();
      g_Aux6FileDisp->activate();
    }
    if((GetAux7Image()) && (GetAux6Image()))
    {
      g_Aux7File->activate();
      g_Aux7FileDisp->activate();
    }
    if((GetAux8Image()) && (GetAux7Image()) && (GetAux6Image()))
    {
      g_Aux8File->activate();
      g_Aux8FileDisp->activate();
    }
  }
  if ((GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()) && (GetAux1Image()))
  {
    g_Aux6File->activate();
    g_Aux6FileDisp->activate();
	  
    if(GetAux7Image())
    {
      g_Aux7File->activate();
      g_Aux7FileDisp->activate();
    }
    if((GetAux8Image()) && (GetAux7Image()))
    {
      g_Aux8File->activate();
      g_Aux8FileDisp->activate();
    }
  }
  if ((GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()) && (GetAux1Image()))
  {
	  g_Aux7File->activate();
	  g_Aux7FileDisp->activate();
	  
	  if(GetAux8Image())
	  {
		  g_Aux8File->activate();
		  g_Aux8FileDisp->activate();
	  }
  }
  if ((GetAux8Image()) && (GetAux7Image()) && (GetAux6Image()) && (GetAux5Image()) && (GetAux4Image()) && (GetAux3Image()) && (GetAux2Image()) && (GetAux1Image()))
  {
	  g_Aux8File->activate();
	  g_Aux8FileDisp->activate();
  }
 
  SetAuxT1File("");
  SetAuxT2File("");
  SetAuxPDFile("");
  SetAux1File("");
  SetAux2File("");
  SetAux3File("");
  SetAux4File("");
  SetAux5File("");
  SetAux6File("");
  SetAux7File("");
  SetAux8File("");
}

AddAuxDataGUIControls::~AddAuxDataGUIControls()
{}

void AddAuxDataGUIControls::OKButtonPressed()
{
	bool InputChecked;
 
	InputChecked = CheckInput();

	if (InputChecked == false) 
	{
		if (GetAuxT1Image())
		SetAuxT1File(g_AuxT1FileDisp->value());
		if (GetAuxT2Image())
		SetAuxT2File(g_AuxT2FileDisp->value());
		if (GetAuxPDImage())
		SetAuxPDFile(g_AuxPDFileDisp->value());
		if (GetAux1Image())
		SetAux1File(g_Aux1FileDisp->value());
		if (GetAux2Image())
		SetAux2File(g_Aux2FileDisp->value());
		if (GetAux3Image())
		SetAux3File(g_Aux3FileDisp->value());
		if (GetAux4Image())
		SetAux4File(g_Aux4FileDisp->value());
		if (GetAux5Image())
		SetAux5File(g_Aux5FileDisp->value());
		if (GetAux6Image())
		SetAux6File(g_Aux6FileDisp->value());
		if (GetAux7Image())
		SetAux7File(g_Aux7FileDisp->value());
		if (GetAux8Image())
		SetAux8File(g_Aux8FileDisp->value());
		g_MainWindow->hide();
	}
}

void AddAuxDataGUIControls::CancelButtonPressed()
{
  g_MainWindow->hide();
}

bool AddAuxDataGUIControls::CheckInput()
{
  bool Warning = false;

  if ( (GetAuxT1Image()) && std::strlen(g_AuxT1FileDisp->value()) == 0)//////////////////(GetAuxT1Image()) &&
    {
      fl_message("Please, set the Auxiliary T1 image...");
      Warning = true;
    }  
  else if ( (GetAuxT2Image()) && (std::strlen(g_AuxT2FileDisp->value()) == 0))//////////////////////
    {
      fl_message("Please, set the AuxT2 image...");
      Warning = true;
    }
  else if ( (GetAuxPDImage()) && (std::strlen(g_AuxPDFileDisp->value()) == 0))
    {
      fl_message("Please, set the AuxPD image...");
      Warning = true;
    }
  
  else if ( (GetAux1Image()) && (std::strlen(g_Aux1FileDisp->value()) == 0))
    {
      fl_message("Please, set the Aux1 image...");
      Warning = true;
    }
  else if ( (GetAux2Image()) && (std::strlen(g_Aux2FileDisp->value()) == 0))
    {
      fl_message("Please, set the Aux2 image...");
      Warning = true;
    }
    else if ( (GetAux3Image()) && (std::strlen(g_Aux3FileDisp->value()) == 0))
    {
	    fl_message("Please, set the Aux3 image...");
	    Warning = true;
    }
    else if ( (GetAux4Image()) && (std::strlen(g_Aux4FileDisp->value()) == 0))
    {
	    fl_message("Please, set the Aux4 image...");
	    Warning = true;
    }
    else if ( (GetAux5Image()) && (std::strlen(g_Aux5FileDisp->value()) == 0))
    {
	    fl_message("Please, set the Aux5 image...");
	    Warning = true;
    }
    else if ( (GetAux6Image()) && (std::strlen(g_Aux6FileDisp->value()) == 0))
    {
	    fl_message("Please, set the Aux6 image...");
	    Warning = true;
    }
    else if ( (GetAux7Image()) && (std::strlen(g_Aux7FileDisp->value()) == 0))
    {
	    fl_message("Please, set the Aux7 image...");
	    Warning = true;
    }
    else if ( (GetAux8Image()) && (std::strlen(g_Aux8FileDisp->value()) == 0))
    {
	    fl_message("Please, set the Aux8 image...");
	    Warning = true;
    }
  return Warning;
}


void AddAuxDataGUIControls::SetAuxT1FileGUI()
{
	char *AuxT1Image = NULL;
  
	AuxT1Image = fl_file_chooser("Set a Auxiliairy T1 file", "Images (*.{gipl,gipl.gz,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
	if (AuxT1Image != NULL)
	{
		g_AuxT1FileDisp->value(AuxT1Image); 
		g_AuxT1FileDisp->position(g_AuxT1FileDisp->size());
	}
}

void AddAuxDataGUIControls::SetAuxT2FileGUI()
{
	char *AuxT2Image = NULL;
  
	AuxT2Image = fl_file_chooser("Set a Auxiliairy T2 file", "Images (*.{gipl,gipl.gz,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
	if (AuxT2Image != NULL)
	{
		g_AuxT2FileDisp->value(AuxT2Image); 
		g_AuxT2FileDisp->position(g_AuxT2FileDisp->size());
	}
}

void AddAuxDataGUIControls::SetAuxPDFileGUI()
{
	char *AuxPDImage = NULL;
  
	AuxPDImage = fl_file_chooser("Set a Auxiliairy PD file", "Images (*.{gipl,gipl.gz,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
	if (AuxPDImage != NULL)
	{
		g_AuxPDFileDisp->value(AuxPDImage); 
		g_AuxPDFileDisp->position(g_AuxPDFileDisp->size());
	}
}

void AddAuxDataGUIControls::SetAux1FileGUI()
{
  char *Aux1Image = NULL;
  
  Aux1Image = fl_file_chooser("Set a Aux1 file", "Images (*.{gipl,gipl.gz,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
  if (Aux1Image != NULL)
    {
      g_Aux1FileDisp->value(Aux1Image); 
      g_Aux1FileDisp->position(g_Aux1FileDisp->size());
    }
}

void AddAuxDataGUIControls::SetAux2FileGUI()
{
  char *Aux2Image = NULL;
  
  Aux2Image = fl_file_chooser("Set a Aux2 file", "Images (*.{gipl,gipl.gz,mhd,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
  if (Aux2Image != NULL)
    {
      g_Aux2FileDisp->value(Aux2Image);
      g_Aux2FileDisp->position(g_Aux2FileDisp->size());
    }
}

void AddAuxDataGUIControls::SetAux3FileGUI()
{
  char *Aux3Image = NULL;

  Aux3Image = fl_file_chooser("Set a Aux3 file", "Images (*.{gipl,gipl.gz,mhd,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
  if (Aux3Image != NULL)
    {
      g_Aux3FileDisp->value(Aux3Image);     
      g_Aux3FileDisp->position(g_Aux3FileDisp->size());
    }
}

void AddAuxDataGUIControls::SetAux4FileGUI()
{
	char *Aux4Image = NULL;

	Aux4Image = fl_file_chooser("Set a Aux4 file", "Images (*.{gipl,gipl.gz,mhd,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
	if (Aux4Image != NULL)
	{
		g_Aux4FileDisp->value(Aux4Image);     
		g_Aux4FileDisp->position(g_Aux4FileDisp->size());
	}
}

void AddAuxDataGUIControls::SetAux5FileGUI()
{
	char *Aux5Image = NULL;

	Aux5Image = fl_file_chooser("Set a Aux5 file", "Images (*.{gipl,gipl.gz,mhd,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
	if (Aux5Image != NULL)
	{
		g_Aux5FileDisp->value(Aux5Image);     
		g_Aux5FileDisp->position(g_Aux5FileDisp->size());
	}
}

void AddAuxDataGUIControls::SetAux6FileGUI()
{
	char *Aux6Image = NULL;

	Aux6Image = fl_file_chooser("Set a Aux6 file", "Images (*.{gipl,gipl.gz,mhd,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
	if (Aux6Image != NULL)
	{
		g_Aux6FileDisp->value(Aux6Image);     
		g_Aux6FileDisp->position(g_Aux6FileDisp->size());
	}
}

void AddAuxDataGUIControls::SetAux7FileGUI()
{
	char *Aux7Image = NULL;

	Aux7Image = fl_file_chooser("Set a Aux7 file", "Images (*.{gipl,gipl.gz,mhd,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
	if (Aux7Image != NULL)
	{
		g_Aux7FileDisp->value(Aux7Image);     
		g_Aux7FileDisp->position(g_Aux7FileDisp->size());
	}
}


void AddAuxDataGUIControls::SetAux8FileGUI()
{
	char *Aux8Image = NULL;

	Aux8Image = fl_file_chooser("Set a Aux8 file", "Images (*.{gipl,gipl.gz,mhd,mhd,mha,img,hdr,nhdr,nrrd})",NULL);
	if (Aux8Image != NULL)
	{
		g_Aux8FileDisp->value(Aux8Image);     
		g_Aux8FileDisp->position(g_Aux8FileDisp->size());
	}
}





