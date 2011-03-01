/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: TextDisplayGUIControls.h,v $
  Language:  C++
  Date:      $Date: 2007/11/26 23:18:46 $
  Version:   $Revision: 1.1 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef TEXTDISPLAYGUICONTROLS_H
#define TEXTDISPLAYGUICONTROLS_H


#include "TextDisplayGUI.h"


class TextDisplayGUIControls : public TextDisplayGUI
{
 public:
  TextDisplayGUIControls();
  virtual ~TextDisplayGUIControls();

  void HideButtonPressed();
  void ShowButtonPressed();
};

#endif

