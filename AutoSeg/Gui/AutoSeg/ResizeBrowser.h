/*=========================================================================
  Program:   AutoSeg
  Module:    $RCSfile: ResizeBrowser.h,v $
  Language:  C++
  Date:      $Date: 2010/06/30 13:19:06 $
  Version:   $Revision: 1.2 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef RESIZEBROWSER_H
#define RESIZEBROWSER_H

#include <FL/Fl.H>
#include <FL/Fl_Browser.H>
#include <FL/fl_draw.H>


class ResizeBrowser : public Fl_Browser
{

	public:
  
		ResizeBrowser(int X,int Y,int W,int H);
		~ResizeBrowser();

  // GET/SET COLUMN SEPARATOR LINE COLOR
		Fl_Color colsepcolor() const {return(_colsepcolor);}
		void colsepcolor(Fl_Color val) {_colsepcolor = val;}
  
  // GET/SET DISPLAY OF COLUMN SEPARATOR LINES
  //     1: show lines, 0: don't show lines
		int showcolsep() const {return(_showcolsep);}
		void showcolsep(int val) {_showcolsep = val;}
  
  // GET/SET COLUMN WIDTHS ARRAY
  //    Just like fltk method, but array is non-const.
		int *column_widths() const {return(_widths);}
		void column_widths(int *val);

	protected:
  // MANAGE EVENTS TO HANDLE COLUMN RESIZING
		int handle(int e);
		void draw() ;

	private:
  
		Fl_Color  _colsepcolor;	// color of column separator lines 
		int       _showcolsep;	// flag to enable drawing column separators
		Fl_Cursor _last_cursor;	// saved cursor state info
		int       _dragging;	// 1=user dragging a column
		int       _dragcol;		// col# user is currently dragging
		int      *_widths;		// pointer to user's width[] array
		int       _nowidths[1];	// default width array (non-const)
  
  // CHANGE CURSOR
  // Does nothing if cursor already set to value specified.
		void change_cursor(Fl_Cursor newcursor);
  
  // RETURN THE COLUMN MOUSE IS 'NEAR'
  //     Returns -1 if none.
		int which_col_near_mouse();  
};


#endif
