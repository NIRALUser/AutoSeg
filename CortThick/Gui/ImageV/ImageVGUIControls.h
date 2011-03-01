#ifndef IMAGEGUICONTROLS_H
#define IMAGEGUICONTROLS_H

#include "ImageVGUI.h"
#include "FullScreenGUI.h"
#include "FullScreenGUIControls.h"
#include "PipelineGUIControls.h"
#include "ImageSyl.h"
#include "PipelineCommand.h"

#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Button.H>

#include <GLSliceView.h>

/*! \brief Visualization tool class
* 
*/

typedef itk::Image<PixelType,Dimension> ImageType;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::Image<OverlayPixelType,Dimension>   OverlayType;
typedef ImageType::Pointer ImagePointer;
typedef ReaderType::Pointer ReaderPointer;


class ImageVGUIControls : public ImageVGUI
{

public:
  
  ImageVGUIControls();
  ImageVGUIControls(const char*,const char*,const char*, bool, bool, bool, const char*, bool, bool, bool, bool, bool, bool,bool,float);
  ImageVGUIControls( int , int, const char* , const char*, bool, bool, bool, const char*, bool, bool, bool, bool, bool, bool,bool,float); 
  virtual ~ImageVGUIControls();

  virtual void LoadButtonPressed(); 
  void GetSpacingGUI();
  void GetSizeGUI();
  void ChangeSliceGUIX();
  void ChangeSliceGUIY();
  void ChangeSliceGUIZ();
  void SaveImageGUI();
  void LoadOverlayImage();
  void SetOverlayOpacityGUI();
  void FullScreenGUI(int view);
  void SetPipelineGUI();
              
    
private:
  void UpdateImage();
  void LoadViews(ImagePointer CurrIm);
  void DisplayImage(ImagePointer CurrIm); 
  void DisplayOverlay();
  void ShowImage();
  void SetScrollingBarSize();
  void SetScrollingBarPosition();

	
  //check if both grey and white matter are load	
  int ImageLoaded;
 
  ImageSyl Image1;
	
	bool OverlayDisplaied;

  bool TestExtention(const char *Name);
  const char* SaveFile(const char*);
};


#endif
