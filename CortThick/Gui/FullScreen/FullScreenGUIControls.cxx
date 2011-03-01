#include "FullScreenGUIControls.h"

FullScreenGUIControls::FullScreenGUIControls()
{}

FullScreenGUIControls::~FullScreenGUIControls()
{}

void FullScreenGUIControls::SetImageParameters(int size)
{
	ImageSize = size;
}

void FullScreenGUIControls::SetFullScreenImage(ImagePointer ImageDisp,int Orientation)
{
  //Set the slide bar position
	SetSlideBarPos();
	//Disp the image
	g_FullScreenImage->SetInputImage(ImageDisp);
	g_FullScreenImage->orientation(Orientation);
	ChangeSliceGUI();
  g_FullScreenImage->update();
	g_FullScreenImage->show();
  g_FullScreenImage->update();
}

void FullScreenGUIControls::SetSlideBarPos()
{
  g_FullScreenSlider->minimum(0);
  g_FullScreenSlider->maximum(ImageSize-1);
  g_FullScreenSlider->step(1);
	g_FullScreenSlider->value(ImageSize/2);
}

void FullScreenGUIControls::ChangeSliceGUI()
{
	g_FullScreenImage->sliceNum((int)g_FullScreenSlider->value());
	g_FullScreenImage->update();
	g_FullScreenImage->show();
}

void FullScreenGUIControls::SetFullScreenOverlay(OverlayPointer Overlay)
{
	g_FullScreenOpacitySlider->activate();
	g_FullScreenImage->SetInputOverlay(Overlay);
}

void FullScreenGUIControls::FullScreenOpacitySetGUI()
{
	g_FullScreenImage->OverlayOpacity(g_FullScreenOpacitySlider->value());
	g_FullScreenImage->update();
}

void FullScreenGUIControls::ApplyZoomGUI()
{
	double coeff = 1;
	double val = (double)g_ZoomFactor->value();
	if(val >= 0)
		coeff = 0.2 * val + 1.0;
	else
		coeff = 0.09 * val + 1.0;
	g_FullScreenImage->winZoom(coeff);
	g_FullScreenImage->update();
}

