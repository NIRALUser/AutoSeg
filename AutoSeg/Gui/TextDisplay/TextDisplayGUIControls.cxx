/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: TextDisplayGUIControls.cxx,v $
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

#include "TextDisplayGUIControls.h"


TextDisplayGUIControls::TextDisplayGUIControls()
  : TextDisplayGUI()
{}

TextDisplayGUIControls::~TextDisplayGUIControls()
{}

void TextDisplayGUIControls::HideButtonPressed()
{
  g_MainWindow->hide();
}

void TextDisplayGUIControls::ShowButtonPressed()
{
  g_MainWindow->show();
}
