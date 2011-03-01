/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: AddAuxDataGUIControls.h,v $
  Language:  C++
  Date:      $Date: 2010/06/30 13:20:38 $
  Version:   $Revision: 1.1 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef ADDAUXDATAGUICONTROLS_H
#define ADDAUXDATAGUICONTROLS_H

#include <cstring>
#include <iostream>
#include <FL/Fl_File_Chooser.H>

#include "AddAuxDataGUI.h"

class AddAuxDataGUIControls : public AddAuxDataGUI
{
 public:
  
  AddAuxDataGUIControls();
  AddAuxDataGUIControls(bool _AuxT1Image, bool _AuxT2Image, bool _AuxPDImage,bool _Aux1Image, bool _Aux2Image, bool _Aux3Image, bool _Aux4Image, bool _Aux5Image, bool _Aux6Image, bool _Aux7Image, bool _Aux8Image);
  virtual ~AddAuxDataGUIControls();

  //User Interface
  void OKButtonPressed();
  void CancelButtonPressed();

  void SetAuxT1FileGUI();
  void SetAuxT2FileGUI();
  void SetAuxPDFileGUI();
  void SetAux1FileGUI();
  void SetAux2FileGUI();
  void SetAux3FileGUI();
  void SetAux4FileGUI();
  void SetAux5FileGUI();
  void SetAux6FileGUI();
  void SetAux7FileGUI();
  void SetAux8FileGUI();


  // Get Files

  char *GetAuxT1File(){return m_AuxT1File;};
  char *GetAuxT2File(){return m_AuxT2File;};
  char *GetAuxPDFile(){return m_AuxPDFile;};
  char *GetAux1File(){return m_Aux1File;};
  char *GetAux2File(){return m_Aux2File;};
  char *GetAux3File(){return m_Aux3File;};
  char *GetAux4File(){return m_Aux4File;};
  char *GetAux5File(){return m_Aux5File;};
  char *GetAux6File(){return m_Aux6File;};
  char *GetAux7File(){return m_Aux7File;};
  char *GetAux8File(){return m_Aux8File;};
  
 private:

  bool GetAuxT1Image(){return m_IsAuxT1Image;};
  bool GetAuxT2Image(){return m_IsAuxT2Image;};
  bool GetAuxPDImage(){return m_IsAuxPDImage;};
  bool GetAux1Image(){return m_IsAux1Image;};
  bool GetAux2Image(){return m_IsAux2Image;};
  bool GetAux3Image(){return m_IsAux3Image;};
  bool GetAux4Image(){return m_IsAux4Image;};
  bool GetAux5Image(){return m_IsAux5Image;};
  bool GetAux6Image(){return m_IsAux6Image;};
  bool GetAux7Image(){return m_IsAux7Image;};
  bool GetAux8Image(){return m_IsAux8Image;};

  void SetAuxT1Image(bool _AuxT1Image){m_IsAuxT1Image = _AuxT1Image;};
  void SetAuxT2Image(bool _AuxT2Image){m_IsAuxT2Image = _AuxT2Image;};
  void SetAuxPDImage(bool _AuxPDImage){m_IsAuxPDImage = _AuxPDImage;};
  void SetAux1Image(bool _Aux1Image){m_IsAux1Image = _Aux1Image;};
  void SetAux2Image(bool _Aux2Image){m_IsAux2Image = _Aux2Image;};
  void SetAux3Image(bool _Aux3Image){m_IsAux3Image = _Aux3Image;};
  void SetAux4Image(bool _Aux4Image){m_IsAux4Image = _Aux4Image;};
  void SetAux5Image(bool _Aux5Image){m_IsAux5Image = _Aux5Image;};
  void SetAux6Image(bool _Aux6Image){m_IsAux6Image = _Aux6Image;};
  void SetAux7Image(bool _Aux7Image){m_IsAux7Image = _Aux7Image;};
  void SetAux8Image(bool _Aux8Image){m_IsAux8Image = _Aux8Image;};

  void SetAuxT1File(const char *_AuxT1File){std::strcpy(m_AuxT1File,_AuxT1File);};
  void SetAuxT2File(const char *_AuxT2File){std::strcpy(m_AuxT2File,_AuxT2File);};
  void SetAuxPDFile(const char *_AuxPDFile){std::strcpy(m_AuxPDFile,_AuxPDFile);};
  void SetAux1File(const char *_Aux1File){std::strcpy(m_Aux1File,_Aux1File);};
  void SetAux2File(const char *_Aux2File){std::strcpy(m_Aux2File,_Aux2File);};
  void SetAux3File(const char *_Aux3File){std::strcpy(m_Aux3File,_Aux3File);};
  void SetAux4File(const char *_Aux4File){std::strcpy(m_Aux4File,_Aux4File);};
  void SetAux5File(const char *_Aux5File){std::strcpy(m_Aux5File,_Aux5File);};
  void SetAux6File(const char *_Aux6File){std::strcpy(m_Aux6File,_Aux6File);};
  void SetAux7File(const char *_Aux7File){std::strcpy(m_Aux7File,_Aux7File);};
  void SetAux8File(const char *_Aux8File){std::strcpy(m_Aux8File,_Aux8File);};

  bool CheckInput();

  char m_AuxT1File[512];
  char m_AuxT2File[512];
  char m_AuxPDFile[512];
  char m_Aux1File[512];
  char m_Aux2File[512];
  char m_Aux3File[512];
  char m_Aux4File[512];
  char m_Aux5File[512];
  char m_Aux6File[512];
  char m_Aux7File[512];
  char m_Aux8File[512];

  bool m_IsAuxT1Image;
  bool m_IsAuxT2Image;
  bool m_IsAuxPDImage;
  bool m_IsAux1Image;
  bool m_IsAux2Image;
  bool m_IsAux3Image;
  bool m_IsAux4Image;
  bool m_IsAux5Image;
  bool m_IsAux6Image;
  bool m_IsAux7Image;
  bool m_IsAux8Image;
};

#endif
