/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: AddDataGUIControls.h,v $
  Language:  C++
  Date:      $Date: 2010/02/11 17:09:48 $
  Version:   $Revision: 1.2 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef ADDDATAGUICONTROLS_H
#define ADDDATAGUICONTROLS_H

#include <cstring>
#include <iostream>
#include <FL/Fl_File_Chooser.H>

#include "AddDataGUI.h"

class AddDataGUIControls : public AddDataGUI
{
 public:
  
  AddDataGUIControls();
  AddDataGUIControls(bool _T2Image, bool _PDImage);
  virtual ~AddDataGUIControls();

  //User Interface
  void OKButtonPressed();
  void CancelButtonPressed();

  void SetT1FileGUI();
  void SetT2FileGUI();
  void SetPDFileGUI();  

  // Get Files
  char *GetT1File(){return m_T1File;};
  char *GetT2File(){return m_T2File;};
  char *GetPDFile(){return m_PDFile;};
  
  
 private:

  bool GetT2Image(){return m_IsT2Image;};
  bool GetPDImage(){return m_IsPDImage;};

  void SetT2Image(bool _T2Image){m_IsT2Image = _T2Image;};
  void SetPDImage(bool _PDImage){m_IsPDImage = _PDImage;};
  
  void SetT1File(const char *_T1File){std::strcpy(m_T1File,_T1File);};
  void SetT2File(const char *_T2File){std::strcpy(m_T2File,_T2File);};
  void SetPDFile(const char *_PDFile){std::strcpy(m_PDFile,_PDFile);};

  bool CheckInput();
  
  char m_T1File[512];
  char m_T2File[512];
  char m_PDFile[512];

  bool m_IsT2Image;
  bool m_IsPDImage;  
};

#endif
