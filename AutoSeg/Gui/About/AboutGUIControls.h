/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: AboutGUIControls.h,v $
  Language:  C++
  Date:      $Date: 2007/11/26 23:17:22 $
  Version:   $Revision: 1.1 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef ABOUTGUICONTROLS_H
#define ABOUTGUICONTROLS_H

#include <iostream>
#include <FL/Fl_Group.H>

#include "AboutGUI.h"


class AboutGUIControls : public AboutGUI
{
 public:
  
  AboutGUIControls();
  virtual ~AboutGUIControls();

  void OKButtonPressed();
};

#endif
