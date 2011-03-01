#include "PipelineGUIControls.h"

PipelineGUIControls::PipelineGUIControls()
{
  m_WhiteMatterLoaded = false;
  m_GreyMatterLoaded = false;
  m_DanielssonMatterLoaded = false;
  m_ImageSegLoaded = false;
  m_ParcelLoad = false;
  m_FinalOutputDirectory = new char[512];
  strcpy(m_CurrentPath,".");
}

PipelineGUIControls::~PipelineGUIControls()
{
  delete m_FinalOutputDirectory;
}


/*************** Useful functions ******************/


//Test if the file name extention is correct
bool PipelineGUIControls::TestExtention(const char *Name)
{
  std::string StringName = Name;
  std::string Slash = "/";
  std::string Dot = ".";

  //First, locate the last "/" and the "."
  int SlashPos = StringName.rfind(Slash,StringName.length());
  int DotPos = StringName.rfind(Dot,StringName.length());	

  //If no dot is found
  if((DotPos < SlashPos) || (DotPos > (int) StringName.length()+1)){
    return false;
  }
  else{
  //get the extention part of the name:
    std::string Ext = StringName.substr(DotPos+1,StringName.length());
  if((Ext == "mha") || (Ext == "gipl") || "gipl.gz"){
      return true;
  }
  else return false;
  }
}

void PipelineGUIControls::ExtractName(char *OriginalName, char* Path, char* FileName)
{
  std::string Name = OriginalName;
  std::string Slash = "/";
  int SlashPosition = Name.rfind(Slash,Name.length());
  std::string Path1;
  Path1 = Name.substr(0,SlashPosition+1);
  std::string NameOnly;
  NameOnly = Name.substr(SlashPosition+1,Name.length()-SlashPosition-1);
  int DotPosition = NameOnly.find_first_of(".",0); 
  NameOnly = NameOnly.erase(DotPosition);
  std::strcpy(Path,Path1.c_str());
  std::strcpy(FileName,NameOnly.c_str());
}

void PipelineGUIControls::AddExtention(const char* OriginalName, const char *Extention, char* OutputName)
{
  std::string FinalName = OriginalName;
  std::string Ext = Extention;

  if(FinalName.length() != 0)
  {
    FinalName.insert(FinalName.size(),Ext);
    FinalName.insert(FinalName.size(),".mha");
    //Converting std::string to char
    if(FinalName.length() < 512) std::strcpy(OutputName,FinalName.c_str());
  }
  else std::cerr << "Invalid Name" << std::endl;
}

void PipelineGUIControls::AddPathAndName(const char *Path, char *Name, char *Output)
{
  std::string Path1 = Path;
  std::string Name1 = Name;
  std::string Slash = "/";
  std::string Final = Path1 + Slash + Name1;
  std::strcpy(Output,Final.c_str());	
}


/*************** The white & grey matter files are loaded **********************/
void PipelineGUIControls::LoadSegFileGUI()
{
  const char *filename = fl_file_chooser("Load Image Seg File","*",m_CurrentPath);
  //Check if the filename existes and if it's correct  
  if(filename){
    strcpy(m_CurrentPath,filename);
    if(TestExtention(filename)){
      g_ImageSegFileNameDisp->value(filename);
      strcpy(m_ImageSegFileName,filename);
      m_ImageSegLoaded = true;
    }
    else
    std::cout << "Invalid file name" << std::endl;
  }
}

void PipelineGUIControls::LoadWhiteMatterGUI()
{
  const char *filename = fl_file_chooser("Load White Matter File","*",m_CurrentPath);
  //Check if the filename existes and if it's correct  
  if(filename){
    strcpy(m_CurrentPath,filename);
    if(TestExtention(filename)){
      g_WhiteMatterFileNameDisp->value(filename);
      strcpy(m_WhiteMatterFileName,filename);
      ExtractName(m_WhiteMatterFileName,m_OriginalWhitePathName,m_OriginalWhiteFileNameOnly);
      m_WhiteMatterLoaded = true;
    }
    else
    std::cout << "Invalid file name" << std::endl;
  }
}

void PipelineGUIControls::LoadGreyMatterGUI()
{
  const char *filename = fl_file_chooser("Load Grey Matter File","*",m_CurrentPath);
  //Check if the filename existe and if it's correct  
  if(filename){
    strcpy(m_CurrentPath,filename);
    if(TestExtention(filename)){
      g_GreyMatterFileNameDisp->value(filename);
      strcpy(m_GreyMatterFileName,filename);
      ExtractName(m_GreyMatterFileName,m_OriginalGreyPathName,m_OriginalGreyFileNameOnly);
      m_GreyMatterLoaded = true;
    }
    else
    std::cout << "Invalid file name" << std::endl;
  }
}

void PipelineGUIControls::LoadDanielssonGUI()
{
  const char *filename = fl_file_chooser("Load Danielsson File","*",m_CurrentPath);
  //Check if the filename existe and if it's correct  
  if(filename){
    strcpy(m_CurrentPath,filename);
    if(TestExtention(filename)){
      g_DanielssonFileName->value(filename);
      strcpy(m_DanielssonFileName,filename);
      ExtractName(m_DanielssonFileName,m_OriginalDanielssonPathName,m_OriginalDanielssonFileNameOnly);
      m_DanielssonMatterLoaded = true;
    }
    else
      std::cout << "Invalid file name" << std::endl;
  }
}

void PipelineGUIControls::LoadParcelFileGUI()
{
  const char *filename = fl_file_chooser("Load Parcellation File","*",m_CurrentPath);
  //Check if the filename existe and if it's correct  
  if(filename){
    strcpy(m_CurrentPath,filename);
    if(TestExtention(filename)){
      g_ParcelFilePathName->value(filename);
      strcpy(m_ParcelFileName,filename);
      m_ParcelLoad = true;
    }
    else
      std::cout << "Invalid file name" << std::endl;
  }

}

/**************************************************************/

/************************ Select Output dir *********************/

void PipelineGUIControls::SelectOutputDirectoryGUI()
{
  Fl_File_Chooser fc(m_CurrentPath,"*",Fl_File_Chooser::DIRECTORY,"Set the general output directory");
  std::string Temp = fc.directory();
  if(Temp.length() < 512) std::strcpy(m_CurrentPath,Temp.c_str());
  //Allowed the new directory creation
  fc.newButton->activate();
  fc.show();

  while(fc.shown())
  Fl::wait();
  if(fc.count()){
    m_FinalOutputDirectory = fc.value();
    g_OutputDirectoryDisp->value(m_FinalOutputDirectory);
  }
}

/*****************************************************************/



/****************** Danielson map **************************/

//Activate the parameter buttons
void PipelineGUIControls::SaveDanielsonMapGUI()
{
  if(g_SaveDanielsonMapCheckButton->value())
    g_DanielsonExtentionFileName->activate();
  else
    g_DanielsonExtentionFileName->deactivate();
}

void PipelineGUIControls::DanielssonComputedGUI()
{
  if(g_DanielssonAlreadyCompute->value())
  {
    g_DanielssonFileName->activate();
    g_DanielssonFinder->activate();
  }
  else                        
  {
    g_DanielssonFileName->deactivate();
    g_DanielssonFinder->deactivate();
  }
}


/************************SegImageFile or White and Grey*****************************/
void PipelineGUIControls::TcheckLoadSegFileGUI()
{
  if(g_LoadSegFile->value())
  {
    g_LoadSegFileButton->activate();
    g_ImageSegFileNameDisp->activate();
    g_ValueLabelWhite->activate();
    g_ValueLabelGrey->activate();
    g_LoadWhiteMatterButton->deactivate();
    g_WhiteMatterFileNameDisp->deactivate();
    g_LoadGreyMatterButton->deactivate();
    g_GreyMatterFileNameDisp->deactivate();
  }
  else
  {
    g_LoadSegFileButton->deactivate();
    g_ImageSegFileNameDisp->deactivate();
    g_ValueLabelWhite->deactivate();
    g_ValueLabelGrey->deactivate();
    g_LoadWhiteMatterButton->activate();
    g_WhiteMatterFileNameDisp->activate();
    g_LoadGreyMatterButton->activate();
    g_GreyMatterFileNameDisp->activate();
  } 
}

/*****************************Final Computation*******************************/

void  PipelineGUIControls::WriteVtkFileGUI()
{
  if(g_WriteVtkFile->value())
    g_VtkDirName->activate();
  else
    g_VtkDirName->deactivate();        
        
}


/************************************Map Dan Map on grey********************************/
void PipelineGUIControls::ComputeDanGreyGUI()
{
  if(g_ComputeDanGrey->value()) 
    g_GreyDanFileExtension->activate();
  else g_GreyDanFileExtension->deactivate();
}

/*********************************Parcel Image buttons*********************************/
void PipelineGUIControls::CheckParcelFileGUI()
{
  if(g_checkParcelFile)
  {
    g_LoadParcelFile->activate();
    g_ParcelFilePathName->activate();
  }
  else  
  {
    g_LoadParcelFile->deactivate();
    g_ParcelFilePathName->deactivate();
  }
}        

/****************************** Compute images ***********************************/

void PipelineGUIControls::ComputeGUI()
{
  if(g_LoadSegFile->value())
  {
    //load Seg File
    if(m_ImageSegLoaded)
    {
      m_Image.SegImageWhiteAndGrey(m_ImageSegFileName,g_ValueLabelWhite->value(), g_ValueLabelGrey->value() );
      ExtractName(m_ImageSegFileName,m_OriginalSegPathName,m_OriginalSegFileNameOnly);
    }  
  }  
  //Load files
  else if((m_WhiteMatterLoaded) && (m_GreyMatterLoaded))
  {
    m_Image.LoadWhiteMatterFile(m_WhiteMatterFileName);
    m_Image.LoadGreyMatterFile(m_GreyMatterFileName);
    ExtractName(m_WhiteMatterFileName,m_OriginalWhitePathName,m_OriginalWhiteFileNameOnly);
	  ExtractName(m_GreyMatterFileName,m_OriginalGreyPathName,m_OriginalGreyFileNameOnly);    
  }
  else std::cerr << "The white and grey matter files have to be set" << std::endl;
   
   //Connective component
  if(g_WhiteMatterConnectiveComponentButton->value())
  {	
    m_Image.SetWhiteMatterImage(m_Image.ConnectiveComponent(m_Image.GetWhiteMatterImage()));
    std::cout<<" White Connective Component "<<std::endl;
  }        
  if(g_GreyMatterConnectiveComponentButton->value())
  {       
    m_Image.SetGreyMatterImage(m_Image.ConnectiveComponent(m_Image.GetGreyMatterImage()));
    std::cout<<" Grey Connective Component "<<std::endl;
  }        

  //Danielson map computation
  //If the image is saved the extention is added to the name  
        
  if(g_DanielssonAlreadyCompute->value())//if Danielsson Map is already compute
  {
    if(m_DanielssonMatterLoaded)
    {
      m_Image.LoadDanielssonMapFile(m_DanielssonFileName);
      std::cout<<" Danielsson Already Compute "<<std::endl;
    }        
    else std::cerr<<" Danielsson doesn't loaded "<<std::endl;
    if(g_SaveDanielsonMapCheckButton->value())
    {
      if(m_ImageSegLoaded)   AddExtention(m_OriginalSegFileNameOnly,g_DanielsonExtentionFileName->value(),m_DanielssonFileName);
      else  AddExtention(m_OriginalWhiteFileNameOnly,g_DanielsonExtentionFileName->value(),m_DanielssonFileName);
      AddPathAndName(m_FinalOutputDirectory, m_DanielssonFileName, m_DanielssonFileName);
      m_Image.WriteDanielsson(m_DanielssonFileName);
    }
  }        
  else 
  {
    std::cout<<" Compute Danielsson "<<std::endl;                               
    if(g_SaveDanielsonMapCheckButton->value())
    {
      if(m_ImageSegLoaded)   AddExtention(m_OriginalSegFileNameOnly,g_DanielsonExtentionFileName->value(),m_DanielssonFileName);
      else  AddExtention(m_OriginalWhiteFileNameOnly,g_DanielsonExtentionFileName->value(),m_DanielssonFileName);	            
	    AddPathAndName(m_FinalOutputDirectory, m_DanielssonFileName, m_DanielssonFileName);
    }    
    m_Image.ComputeDM(m_DanielssonFileName,g_SaveDanielsonMapCheckButton->value());           
  }

  //boundary computation
  std::cout<<" Boundary Computation "<<std::endl;
  m_Image.BoundaryComputation();


  //Final distance map
  //First, the file names:
  char WhiteFinalDistanceName[512];
  char VtkFileName[512];
  char AvgFileName[512];
  char DanGreyFileName[512];

  if(m_ImageSegLoaded)
  {
    AddExtention(m_OriginalSegFileNameOnly,g_WMFinalDistanceExtentionFileName->value(),WhiteFinalDistanceName);
    AddExtention(m_OriginalSegFileNameOnly,g_VtkDirName->value(),VtkFileName);
    AddExtention(m_OriginalSegFileNameOnly,g_FileExtensionAvgImage->value(),AvgFileName);
    AddExtention(m_OriginalSegFileNameOnly,g_GreyDanFileExtension->value(),DanGreyFileName);
  }
  else
  {
    AddExtention(m_OriginalWhiteFileNameOnly,g_WMFinalDistanceExtentionFileName->value(),WhiteFinalDistanceName);
    AddExtention(m_OriginalWhiteFileNameOnly,g_VtkDirName->value(),VtkFileName);
    AddExtention(m_OriginalWhiteFileNameOnly,g_FileExtensionAvgImage->value(),AvgFileName);
    AddExtention(m_OriginalGreyFileNameOnly,g_GreyDanFileExtension->value(),DanGreyFileName);  
  }
         

  AddPathAndName(m_FinalOutputDirectory,WhiteFinalDistanceName,WhiteFinalDistanceName);
  AddPathAndName(m_FinalOutputDirectory,VtkFileName,VtkFileName);
  AddPathAndName(m_FinalOutputDirectory,AvgFileName,AvgFileName);
  AddPathAndName(m_FinalOutputDirectory,DanGreyFileName,DanGreyFileName);
 
  //Compute the distance map
  m_Image.ComputeDistance( WhiteFinalDistanceName,
			   VtkFileName,
			   AvgFileName,
			   DanGreyFileName,
			   m_ParcelFileName,
			   "unvalidoptionforGUI",
			   g_AverageComputation->value(),
			   g_WriteVtkFile->value(),
			   g_ComputeDanGrey->value(),
			   g_checkParcelFile->value(),
			   false,
			   false,
			   "unvalidoptionforGUI",
			   "unvalidoptionforGUI",
			   false,false,false,0);

}


/****************** Close the widget ****************************/

void PipelineGUIControls::ClosePipelineWidgetGUI()
{
  g_PipelineWindow->hide();
}

/****************************************************************/



