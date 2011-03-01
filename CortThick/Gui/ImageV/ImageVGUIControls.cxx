#include "ImageVGUIControls.h"



ImageVGUIControls::ImageVGUIControls()
{
  ImageLoaded = 0;
  OverlayDisplaied = false;
  g_ImageBoxX->clickMode(CM_NOP);
  g_ImageBoxY->clickMode(CM_NOP);
  g_ImageBoxZ->clickMode(CM_NOP);
}

ImageVGUIControls::ImageVGUIControls(const char* WhiteMatter,const char* GreyMatter,
       const char* Output_Directory,bool WhiteComponent,bool GreyComponent,bool VtkFile,
				     const char* ParFileName, bool ParFile, bool WhiteDistanceFile, bool GreyDistanceFile, bool Separe, bool DistanceImage, bool WriteDistBoundMapGreyMat,bool Interp,float Threshold)
{
  PipelineCommand newPipelineCommand(WhiteMatter,GreyMatter,Output_Directory,
				     WhiteComponent,GreyComponent,VtkFile,ParFileName, ParFile,WhiteDistanceFile,GreyDistanceFile, Separe, DistanceImage,WriteDistBoundMapGreyMat,Interp,Threshold );
 }

ImageVGUIControls::ImageVGUIControls(int levelWhite,int levelGrey, const char* SegFile,const char* Output_Directory,
				     bool WhiteComponent,bool GreyComponent,bool VtkFile, const char* ParFileName, bool ParFile , bool  WhiteDistanceFile, bool GreyDistanceFile, bool Separe, bool DistanceImage, bool WriteDistBoundMapGreyMat,bool Interp,float Threshold)
{
  PipelineCommand newPipelineCommand(levelWhite,levelGrey,SegFile, Output_Directory,
				     WhiteComponent,GreyComponent,VtkFile,ParFileName, ParFile,WhiteDistanceFile,GreyDistanceFile, Separe, DistanceImage, WriteDistBoundMapGreyMat,Interp,Threshold );
}  

ImageVGUIControls::~ImageVGUIControls()
{}

void ImageVGUIControls::LoadButtonPressed()
{  

  //if there is already an overlay, set it off
	if(OverlayDisplaied){
	  g_ImageBoxX->ViewOverlayData(false);
		g_ImageBoxY->ViewOverlayData(false);
		g_ImageBoxZ->ViewOverlayData(false);
		OverlayDisplaied = false;
	}
	
	//display the file chooser interface
  Fl_File_Chooser fc(".","*",Fl_File_Chooser::SINGLE,"Load a file");
  fc.show();
  
  while(fc.shown())
    Fl::wait();
  
	//if a name has been set
  if(fc.count()){  
		//The name is displayed
    g_fileNameDisp->value(fc.value());
    //Pass the file name and load the Image in the ImageSyl class
    Image1.SetFileName(fc.value());
    DisplayImage(Image1.GetImageSyl());  
  }
}


//This function display the loaded image
void ImageVGUIControls::DisplayImage(ImagePointer CurrentImage)
{  
	//Display the 3 views
  LoadViews(CurrentImage);

	//Get the image spacing and size
  Image1.GetImageProp(CurrentImage);

  GetSpacingGUI();
  GetSizeGUI();

  //Set the value of the scrolling bars
  SetScrollingBarSize();
  SetScrollingBarPosition();

  UpdateImage();
  ShowImage();

  //Activate the click mode for the windows
  g_ImageBoxX->clickMode(CM_SELECT);
  g_ImageBoxY->clickMode(CM_SELECT);
  g_ImageBoxZ->clickMode(CM_SELECT);
}


void ImageVGUIControls::LoadViews(ImagePointer CurrIm)
{
  //The image is loaded in the 3 windows
  g_ImageBoxX->SetInputImage(CurrIm);
  g_ImageBoxX->orientation(0);
  g_ImageBoxY->SetInputImage(CurrIm);
  g_ImageBoxY->orientation(1);
  g_ImageBoxZ->SetInputImage(CurrIm);
  g_ImageBoxZ->orientation(2);
}

void ImageVGUIControls::UpdateImage()
{
  g_ImageBoxX->update();
  g_ImageBoxY->update();
  g_ImageBoxZ->update();
}

void ImageVGUIControls::ShowImage()
{
  g_ImageBoxX->show();  
  g_ImageBoxY->show();  
  g_ImageBoxZ->show();  
}

void ImageVGUIControls::GetSpacingGUI()
{
  g_XSpacing->value(Image1.GetImageSpacing()[0]);
  g_YSpacing->value(Image1.GetImageSpacing()[1]);
  g_ZSpacing->value(Image1.GetImageSpacing()[2]);
}

void ImageVGUIControls::GetSizeGUI()
{
  g_XSize->value(Image1.GetImageSize()[0]);
  g_YSize->value(Image1.GetImageSize()[1]);
  g_ZSize->value(Image1.GetImageSize()[2]);
}

void ImageVGUIControls::SetScrollingBarSize()
{
  //Set the scrollingbar values according to the image size
  g_SliceNumberX->maximum(Image1.GetImageSize()[0]-1);
  g_SliceNumberX->minimum(0);
  g_SliceNumberX->step(1);
  g_SliceNumberY->maximum(Image1.GetImageSize()[1]-1);
  g_SliceNumberY->minimum(0);
  g_SliceNumberY->step(1);
  g_SliceNumberZ->maximum(Image1.GetImageSize()[2]-1);
  g_SliceNumberZ->minimum(0);
  g_SliceNumberZ->step(1);
}

//When the slices are loaded, it is the middle one which is displayed
void ImageVGUIControls::SetScrollingBarPosition()
{
  g_SliceNumberX->value(((int)(Image1.GetImageSize()[0]/2)));
  ChangeSliceGUIX();
  g_SliceNumberY->value(((int)(Image1.GetImageSize()[1]/2)));
  ChangeSliceGUIY();
  g_SliceNumberZ->value(((int)(Image1.GetImageSize()[2]/2)));
  ChangeSliceGUIZ();
}

//Update the displayed slice according to the slicer value
void ImageVGUIControls::ChangeSliceGUIX()
{
  g_ImageBoxX->sliceNum((int)g_SliceNumberX->value());
  g_ImageBoxX->update();
}
void ImageVGUIControls::ChangeSliceGUIY()
{
  g_ImageBoxY->sliceNum((int)g_SliceNumberY->value());
  g_ImageBoxY->update();
}
void ImageVGUIControls::ChangeSliceGUIZ()
{
  g_ImageBoxZ->sliceNum((int)g_SliceNumberZ->value());
  g_ImageBoxZ->update();
}

//Generic function used to save the files
const char *ImageVGUIControls::SaveFile(const char *Text)
{
  const char *filename = fl_file_chooser(Text,"*",".");
  if(filename != NULL){
	  //if the extention is correct
		if(TestExtention(filename))	{
			return filename;
			}
		else{
			Fl::warning("Uncorrect file name");
		 	return NULL;	
		}
	}
	else return NULL;
}

//Save the current displayed image
void ImageVGUIControls::SaveImageGUI()
{
	Image1.SaveImage(SaveFile("Save current displayed file"));
}

//Test if the name extention is known
bool ImageVGUIControls::TestExtention(const char *Name)
{
  std::string StringName = Name;
  std::string Slash = "/";
  std::string Dot = ".";
  
  //First, locate the last "/" and the "."
  int SlashPos = StringName.rfind(Slash,StringName.length());
  int DotPos = StringName.rfind(Dot,StringName.length());	
  
  //If no dot is found
  if((DotPos < SlashPos) || (DotPos > (int)(StringName.length()+1))){
		return false;
  }
  else{
    //get the extention part of the name:
    std::string Ext = StringName.substr(DotPos+1,(StringName.length()));
    if((Ext == "mha") || (Ext == "gipl")){
      return true;
    }
    else return false;
  }
}


//Load and display the overlay
void ImageVGUIControls::LoadOverlayImage()
{
  Fl_File_Chooser fc(".","*",Fl_File_Chooser::SINGLE,"Load a file");
  fc.show();
  
  while(fc.shown())
    Fl::wait();
  if(fc.count()){
	  Image1.SetOverlayFileName(fc.value());
 		DisplayOverlay();
	}
}

void ImageVGUIControls::DisplayOverlay()
{
  g_ImageBoxX->SetInputOverlay(Image1.GetOverlaySyl());
  g_ImageBoxY->SetInputOverlay(Image1.GetOverlaySyl());
  g_ImageBoxZ->SetInputOverlay(Image1.GetOverlaySyl());
	OverlayDisplaied = true;
  UpdateImage();
}

void ImageVGUIControls::SetOverlayOpacityGUI()
{
  g_ImageBoxX->OverlayOpacity(g_OverlayOpacity->value());
  g_ImageBoxY->OverlayOpacity(g_OverlayOpacity->value());
  g_ImageBoxZ->OverlayOpacity(g_OverlayOpacity->value());
  UpdateImage();
}

void ImageVGUIControls::FullScreenGUI(int view)
{

	//First i have to create the new widget
  FullScreenGUIControls FullScreenWindow;
  FullScreenWindow.g_FullScreenWindow->show();

  //Then transfert the image
	FullScreenWindow.SetImageParameters(Image1.GetImageSize()[view]);
	FullScreenWindow.SetFullScreenImage(Image1.GetImageSyl(),view);
	//if there is an overlay
	if(OverlayDisplaied){
		FullScreenWindow.SetFullScreenOverlay(Image1.GetOverlaySyl());
	}


	//And then display the widget
	Fl::run();

}

void ImageVGUIControls::SetPipelineGUI()
{
	PipelineGUIControls PipelineSetup;
	PipelineSetup.g_PipelineWindow->show();
	Fl::run();

}




