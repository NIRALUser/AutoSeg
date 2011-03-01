/*=========================================================================
  Program:   AutoSeg
  Module:    $RCSfile: ResizeBrowser.cxx,v $
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

#include "ResizeBrowser.h"

ResizeBrowser::ResizeBrowser(int X,int Y,int W,int H)
	: Fl_Browser(X,Y,W,H)
{
	_colsepcolor = Fl_Color(FL_BLACK);
	_last_cursor = FL_CURSOR_DEFAULT;
	_showcolsep  = 0;
	_dragging    = 0;
	_nowidths[0] = 0;
	_widths      = _nowidths;
}

ResizeBrowser::~ResizeBrowser()
{

}



void ResizeBrowser::change_cursor(Fl_Cursor newcursor) 
{
	if ( newcursor != _last_cursor ) 
	{
		fl_cursor(newcursor, FL_BLACK, FL_WHITE);
		_last_cursor = newcursor;
	}
}

int ResizeBrowser::which_col_near_mouse() 
{
	int X,Y,W,H;
	int MouseX, ColX, Diff, t;
	Fl_Browser::bbox(X,Y,W,H);		// area inside browser's box()
  
  // EVENT NOT INSIDE BROWSER AREA? (eg. on a scrollbar)
	if ( ! Fl::event_inside(X,Y,W,H) ) 
		return(-1);
	else
	{
		MouseX = Fl::event_x() + hposition();
		ColX = this->x();
		for ( t = 0; _widths[t]; t++ ) 
		{
			ColX += _widths[t];
			Diff = MouseX - ColX;
      // MOUSE 'NEAR' A COLUMN? return column #
			if ( Diff >= -4 && Diff <= 4 ) 
				return(t);    
		}  
		return(-1);
	}
}


int ResizeBrowser::handle(int e) 
{
	int Ret, WhichCol, MouseX, NewWidth, t;

  // Not showing column separators? Use default Fl_Browser::handle() logic
	if ( ! showcolsep() ) 
		return (Fl_Browser::handle(e));

  // Handle column resizing
	Ret = 0;
	switch (e) 
	{
		case FL_ENTER: 
		{
			Ret = 1;
			break;
		}
		case FL_MOVE: 
		{
			if ( which_col_near_mouse() >= 0 ) 
				change_cursor(FL_CURSOR_WE);
			else 
				change_cursor(FL_CURSOR_DEFAULT);
			Ret = 1;
			break;
		}
		case FL_PUSH: 
		{
			WhichCol = which_col_near_mouse();
			if ( WhichCol >= 0 ) 
			{
      // CLICKED ON RESIZER? START DRAGGING
				Ret = 1;
				_dragging = 1;
				_dragcol = WhichCol;
				change_cursor(FL_CURSOR_DEFAULT);
			}
			break;
		}
		case FL_DRAG: 
		{
			if ( _dragging ) 
			{
				Ret = 1;
      // Sum up column widths to determine position
				MouseX = Fl::event_x() + hposition();
				NewWidth = MouseX - x();
				for ( t = 0; _widths[t] && t<_dragcol; t++ ) 
					NewWidth -= _widths[t];
      
				if ( NewWidth > 0 ) 
				{
	// Apply new width, redraw interface
					_widths[_dragcol] = NewWidth;
					if ( _widths[_dragcol] < 2 ) 
						_widths[_dragcol] = 2;
					redraw();
				}
			}
			break;
		}
		case FL_LEAVE:
		case FL_RELEASE: 
		{
			_dragging = 0;				// disable drag mode
			change_cursor(FL_CURSOR_DEFAULT);	// ensure normal cursor
			Ret = 1;
			break;
		}
	}
	if ( _dragging ) 
		return(1);			// dragging? don't pass event to Fl_Browser
	return(Fl_Browser::handle(e) ? 1 : Ret);
}


void ResizeBrowser::draw() 
{
	int ColX, X, Y, W, H, t;

  // DRAW BROWSER
	Fl_Browser::draw();
	if ( _showcolsep ) 
	{
    // DRAW COLUMN SEPARATORS
		ColX = this->x() - hposition();
		Fl_Browser::bbox(X,Y,W,H);
		fl_color(_colsepcolor);
		for ( t = 0; _widths[t]; t++ ) 
		{
			ColX += _widths[t];
			if ( ColX > X && ColX < (X+W) ) 
				fl_line(ColX, Y, ColX, Y+H-1);
		}
	}
}


void ResizeBrowser::column_widths(int *val) 
{
	_widths = val;
	Fl_Browser::column_widths(val);
}
