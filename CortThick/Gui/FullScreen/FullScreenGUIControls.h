#ifndef FULLSCREENGUICONTROLS_H
#define FULLSCREENGUICONTROLS_H

#include <GLSliceView.h>
#include "FullScreenGUI.h"
#include "ImageSyl.h"

/*! \brief Full screen visualization class
*
* 
* 
*/


class FullScreenGUIControls : public FullScreenGUI
{

public:
	FullScreenGUIControls();
	virtual ~FullScreenGUIControls();
	void SetImageParameters(int size);
	void SetFullScreenImage(ImagePointer Im,int Orientation);
	void ChangeSliceGUI();
	void SetFullScreenOverlay(OverlayPointer Overlay);
	void FullScreenOpacitySetGUI();
	void ApplyZoomGUI();

private:
	int ImageSize;
  void SetSlideBarPos();

};



#endif
