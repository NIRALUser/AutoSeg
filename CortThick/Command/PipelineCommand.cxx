#include "PipelineCommand.h"

#if defined(_WIN32) || defined(WIN32)
#define sep	'\\'
#else
#define sep	'/'
#endif

//CortThickCLP Constructor (SegOption), Input:brain segmented image

PipelineCommand::PipelineCommand(int levelWhite, int levelGrey, const char* SegFile,const char* Output_Directory,
				 bool WhiteComponent,bool GreyComponent,bool VtkFile,
				 const char* ParFileName, bool ParFile,const char *WhiteDistanceFile , 
				 const char *GreyDistanceFile, bool Separe,const char *DistanceImage,const char *GMMapsDistImage,const char *GMMapsAvgDistImage, bool Interp, float Threshold)
{
  strcpy(m_SegFileName,SegFile );    
  strcpy(m_Output_PathName,Output_Directory );
  strcpy(m_ParFileName,ParFileName);
  strcpy(m_WhiteDistanceFile,WhiteDistanceFile);
  strcpy(m_GreyDistanceFile,GreyDistanceFile);
  strcpy(m_DistanceImageMapOnWhite,DistanceImage);
  strcpy(m_GMMapsDistImage,GMMapsDistImage);
  strcpy(m_GMMapsAvgDistImage,GMMapsAvgDistImage);

  m_WhiteComponent = WhiteComponent;
  m_GreyComponent = GreyComponent;
  m_VtkFile = VtkFile;
  m_levelWhite = levelWhite;
  m_levelGrey = levelGrey;
  m_segfile = true;
  m_ParFile = ParFile;
  m_Interp= Interp;
  m_Threshold = Threshold;

  
  if(strlen(m_WhiteDistanceFile)==0)
	m_WriteWhiteDistance = 0;
  else
  	m_WriteWhiteDistance = 1;
  if(strlen(m_GreyDistanceFile)==0)
	m_WriteGreyDistance = 0;
  else
  	m_WriteGreyDistance = 1;
  if(strlen(DistanceImage)==0)
	m_WriteDistanceImageMapOnWhite = 0;
  else
  	m_WriteDistanceImageMapOnWhite = 1;

  if(strlen(m_GMMapsDistImage)==0)
	m_WriteDistanceImageMapOnGrey = 0;
  else
  	m_WriteDistanceImageMapOnGrey = 1;

  if(strlen(m_GMMapsAvgDistImage)==0)
		m_WriteAvgDistanceImageMapOnGrey = 0;
  else
  	m_WriteAvgDistanceImageMapOnGrey = 1;

  m_Separe = Separe;
  ComputeCLP();
}

//CortThickCLP Constructor (SepOption), 2 Inputs:white and grey matter files

PipelineCommand::PipelineCommand(const char *WhiteMatter,const char *GreyMatter,const char* Output_Directory,
				 bool WhiteComponent,bool GreyComponent,bool VtkFile, 
				 const char* ParFileName, bool ParFile, const char *WhiteDistanceFile, 
				 const char *GreyDistanceFile, bool Separe,const char *DistanceImage,const char *GMMapsDistImage,const char *GMMapsAvgDistImage, bool Interp, float Threshold)
{
  strcpy(m_WhiteMatterFileName,WhiteMatter );
  strcpy(m_GreyMatterFileName, GreyMatter);    
  strcpy(m_Output_PathName,Output_Directory );
  strcpy(m_ParFileName,ParFileName);
  strcpy(m_WhiteDistanceFile,WhiteDistanceFile);
  strcpy(m_GreyDistanceFile,GreyDistanceFile);
  strcpy(m_DistanceImageMapOnWhite,DistanceImage);
  strcpy(m_GMMapsDistImage,GMMapsDistImage);
  strcpy(m_GMMapsAvgDistImage,GMMapsAvgDistImage);
  
  m_DanielssonComputed =false;
  m_WhiteComponent = WhiteComponent;
  m_GreyComponent = GreyComponent;
  m_VtkFile = VtkFile;
  m_segfile = false;
  m_ParFile = ParFile;
  m_Separe = Separe;
  m_Interp= Interp;
  m_Threshold = Threshold;

  if(strlen(m_WhiteDistanceFile)==0)
	m_WriteWhiteDistance = 0;
  else
  	m_WriteWhiteDistance = 1;
  if(strlen(m_GreyDistanceFile)==0)
	m_WriteGreyDistance = 0;
  else
  	m_WriteGreyDistance = 1;
  if(strlen(m_DistanceImageMapOnWhite)==0)
	m_WriteDistanceImageMapOnWhite = 0;
  else
  	m_WriteDistanceImageMapOnWhite = 1;
  if(strlen(m_GMMapsDistImage)==0)
	m_WriteDistanceImageMapOnGrey = 0;
  else
  	m_WriteDistanceImageMapOnGrey = 1;
  if(strlen(m_GMMapsAvgDistImage)==0)
	m_WriteAvgDistanceImageMapOnGrey = 0;
  else
  	m_WriteAvgDistanceImageMapOnGrey = 1;
  m_Separe = Separe;

  ComputeCLP();
}
                             

/*
* Constructor - depending on the input : here with two separate images : white and grey matter file
*/
PipelineCommand::PipelineCommand(const char *WhiteMatter,const char *GreyMatter,const char* Output_Directory,
				 bool WhiteComponent,bool GreyComponent,bool VtkFile, 
				 const char* ParFileName, bool ParFile, bool WriteWhiteDistance, 
				 bool WriteGreyDistance, bool Separe, bool DistanceImage,bool GreyDistanceImages, bool Interp, float Threshold)
{
  strcpy(m_WhiteMatterFileName,WhiteMatter );
  strcpy(m_GreyMatterFileName, GreyMatter);    
  strcpy(m_Output_PathName,Output_Directory );
  strcpy(m_ParFileName,ParFileName);
  
  m_DanielssonComputed =false;
  m_WhiteComponent = WhiteComponent;
  m_GreyComponent = GreyComponent;
  m_VtkFile = VtkFile;
  m_segfile = false;
  m_ParFile = ParFile;
  m_WriteWhiteDistance = WriteWhiteDistance;
  m_WriteGreyDistance = WriteGreyDistance;
  m_Separe = Separe;
  m_WriteDistanceImageMapOnWhite = DistanceImage;  
  m_WriteDistanceImageMapOnGrey = GreyDistanceImages;
  m_Interp= Interp;
  m_Threshold = Threshold;

  ComputeGUI();
}

/*
* Constructor - depending on the input : here with one image : a brain segmented image
*/
PipelineCommand::PipelineCommand(int levelWhite, int levelGrey, const char* SegFile,const char* Output_Directory,
				 bool WhiteComponent,bool GreyComponent,bool VtkFile,
				 const char* ParFileName, bool ParFile, bool WriteWhiteDistance, 
				 bool WriteGreyDistance, bool Separe, bool DistanceImage, bool GreyDistanceImages, bool Interp, float Threshold)
{
  strcpy(m_SegFileName,SegFile );    
  strcpy(m_Output_PathName,Output_Directory );
  strcpy(m_ParFileName,ParFileName);
  
  m_WhiteComponent = WhiteComponent;
  m_GreyComponent = GreyComponent;
  m_VtkFile = VtkFile;
  m_levelWhite = levelWhite;
  m_levelGrey = levelGrey;
  m_segfile = true;
  m_ParFile = ParFile;
  m_WriteWhiteDistance = WriteWhiteDistance;
  m_WriteGreyDistance = WriteGreyDistance;
  m_Separe = Separe;
  m_WriteDistanceImageMapOnWhite = DistanceImage;
  m_WriteDistanceImageMapOnGrey = GreyDistanceImages;
  m_Interp= Interp;
  m_Threshold = Threshold;

  ComputeGUI();
}
                             
/*
* Destructor -
*/
PipelineCommand::~PipelineCommand()
{
}


/*************** Useful functions ******************/

//Test if the file name extention is correct
bool PipelineCommand::TestExtention(const char *Name)
{
  std::string StringName = Name;
	std::string Dot = ".";
	
	//First, locate the last "/" and the "."
	unsigned int SepPos = StringName.rfind(sep,StringName.length());
	unsigned int DotPos = StringName.rfind(Dot,StringName.length());	
		
	//If no dot is found
	if((DotPos < SepPos) || (DotPos > StringName.length()+1)){
		return false;
	}
	else{
		//get the extention part of the name:
		std::string Ext = StringName.substr(DotPos+1,StringName.length());
	if((Ext == "mha") || (Ext == "gipl")){
			return true;
		}
		else return false;
	}
}

/*const char *PipelineCommand::ChooseDirectory(char * Text)
{
	Fl_File_Chooser fc(".","*",Fl_File_Chooser::DIRECTORY,Text);
	//Allowed the new directory creation
	fc.newButton->activate();
  fc.show();
  
  while(fc.shown())
    Fl::wait();
	return fc.value(); 
}*/

//Extract output file name

//Extract the name of the file, delete the path name, just keep the file name
void PipelineCommand::ExtractName(char *OriginalName, char* Path, char* FileName)
{
	std::string Name = OriginalName;
  //find the last /
  int SepPosition = Name.rfind(sep,Name.length());
	std::string Path1;
	Path1 = Name.substr(0,SepPosition+1);
	std::string NameOnly;
  //keep only the name
	NameOnly = Name.substr(SepPosition+1,Name.length()-SepPosition-1);
  int DotPosition = NameOnly.find_first_of(".",0); 
  //erase the extension
  NameOnly = NameOnly.erase(DotPosition);
  std::strcpy(Path,Path1.c_str());
	std::strcpy(FileName,NameOnly.c_str());
}

void PipelineCommand::AddExtention(const char* OriginalName, const char *Extention, char* OutputName)
{
	std::string FinalName = OriginalName;
	std::string Ext = Extention;
	
	if(FinalName.length() != 0){
    //insert the extension
    FinalName.insert(FinalName.size(),Ext);
    FinalName.insert(FinalName.size(),".mha");

		//Converting std::string to char
		if(FinalName.length() < 512) std::strcpy(OutputName,FinalName.c_str());
  }
	else std::cerr << "Invalid Name" << std::endl;
}

void PipelineCommand::AddPathAndName(const char *Path, char *Name, char *Output)
{
	std::string Path1 = Path;
	std::string Name1 = Name;
	std::string Final = Path1 + sep + Name1;
	std::strcpy(Output,Final.c_str());	
}



/****************************** Compute images ***********************************/

void PipelineCommand::ComputeGUI()
{
	
  //Load files
  if(m_segfile)
    {
      ExtractName(m_SegFileName,m_OriginalSegPathName,m_OriginalSegFileNameOnly);
      m_Image.SegImageWhiteAndGrey(m_SegFileName,m_levelWhite, m_levelGrey );
    }
  else
    {  
      ExtractName(m_WhiteMatterFileName,m_OriginalWhitePathName,m_OriginalWhiteFileNameOnly);
      ExtractName(m_GreyMatterFileName,m_OriginalGreyPathName,m_OriginalGreyFileNameOnly);                        
      m_Image.LoadWhiteMatterFile(m_WhiteMatterFileName);
      m_Image.LoadGreyMatterFile(m_GreyMatterFileName);                
    }
  
  //Connective component
  if(m_WhiteComponent)
    {
      std::cout<<" White connective component "<<std::endl;
      m_Image.SetWhiteMatterImage(m_Image.ConnectiveComponent(m_Image.GetWhiteMatterImage()));
    }
  if(m_GreyComponent)
    {
      std::cout<<" Grey connective component "<<std::endl;
      m_Image.SetGreyMatterImage(m_Image.ConnectiveComponent(m_Image.GetGreyMatterImage()));
    }
  
  //boundary computation
  std::cout<<" Boundary Computation "<<std::endl;
  m_Image.BoundaryComputation();
  
  //Danielson map computation                 
  std::cout<<" Danielsson Map "<<std::endl;
  if(m_segfile)   AddExtention(m_OriginalSegFileNameOnly,"-Danielsson",m_DanielssonFileName);
  else  AddExtention(m_OriginalWhiteFileNameOnly,"-Danielsson",m_DanielssonFileName);
  AddPathAndName(m_Output_PathName, m_DanielssonFileName, m_DanielssonFileName);
  m_Image.ComputeDM(m_DanielssonFileName,false);
  
  char WhiteFinalDistanceName[512];
  char VtkFileName[512];
  char AvgFileName[512];
  char DanGreyFileName[512];
 
  if(m_segfile)
  {
    AddExtention(m_OriginalSegFileNameOnly, "-WhiteMatDistanceMap",WhiteFinalDistanceName);
    AddExtention(m_OriginalSegFileNameOnly,"-VectorFile",VtkFileName);
    AddExtention(m_OriginalSegFileNameOnly,"-DistanceMapOnWhiteAvg",AvgFileName);
    AddExtention(m_OriginalSegFileNameOnly,"-GreyDan",DanGreyFileName);
    AddExtention(m_OriginalSegFileNameOnly,"-DistanceMapOnWhite",m_DistanceImageFileName);
    AddExtention(m_OriginalSegFileNameOnly,"-DistanceMapOnGrey",m_DistanceMapOnGrey);
    AddExtention(m_OriginalSegFileNameOnly,"-DistanceMapAverageOnGrey",m_DistanceMapAverageOnGrey);
  }  
  else
  {    
    AddExtention(m_OriginalWhiteFileNameOnly, "-WhiteMatDistanceMap",WhiteFinalDistanceName);
    AddExtention(m_OriginalWhiteFileNameOnly,"-VectorFile",VtkFileName);
    AddExtention(m_OriginalWhiteFileNameOnly,"-DistanceMapOnWhiteAvg",AvgFileName);
    AddExtention(m_OriginalGreyFileNameOnly,"-GreyDan",DanGreyFileName);
    AddExtention(m_OriginalGreyFileNameOnly,"-DistanceMapOnWhite",m_DistanceImageFileName);
    AddExtention(m_OriginalGreyFileNameOnly,"-DistanceMapOnGrey",m_DistanceMapOnGrey);
    AddExtention(m_OriginalGreyFileNameOnly,"-DistanceMapAverageOnGrey",m_DistanceMapAverageOnGrey);
  }      
  
  AddPathAndName(m_Output_PathName,WhiteFinalDistanceName,WhiteFinalDistanceName);
  AddPathAndName(m_Output_PathName,VtkFileName,VtkFileName);                        
  AddPathAndName(m_Output_PathName,AvgFileName,AvgFileName);
  AddPathAndName(m_Output_PathName,DanGreyFileName,DanGreyFileName);
  AddPathAndName(m_Output_PathName,m_DistanceImageFileName,m_DistanceImageFileName);
  AddPathAndName(m_Output_PathName,m_DistanceMapOnGrey,m_DistanceMapOnGrey);
  AddPathAndName(m_Output_PathName,m_DistanceMapAverageOnGrey,m_DistanceMapAverageOnGrey);

  //Compute the distance map
  m_Image.ComputeDistance(WhiteFinalDistanceName,
			  VtkFileName,
			  AvgFileName,
			  DanGreyFileName,
			  m_ParFileName,
			  m_DistanceImageFileName,        
			  m_WriteWhiteDistance,
			  m_VtkFile,
			  m_WriteGreyDistance,
			  m_ParFile,
			  m_Separe,
			  m_WriteDistanceImageMapOnWhite,
			  m_DistanceMapOnGrey,
			  m_DistanceMapAverageOnGrey,
			  m_WriteDistanceImageMapOnGrey,
			  m_WriteAvgDistanceImageMapOnGrey,
			  m_Interp,
			  m_Threshold);
 }

/**********************************************************************************/


/****************************** Compute images for CortThickCLP***********************************/

void PipelineCommand::ComputeCLP()
{
	
  //Load files
  if(m_segfile)
    {
      ExtractName(m_SegFileName,m_OriginalSegPathName,m_OriginalSegFileNameOnly);
      m_Image.SegImageWhiteAndGrey(m_SegFileName,m_levelWhite, m_levelGrey );
    }
  else
    {  
      ExtractName(m_WhiteMatterFileName,m_OriginalWhitePathName,m_OriginalWhiteFileNameOnly);
      ExtractName(m_GreyMatterFileName,m_OriginalGreyPathName,m_OriginalGreyFileNameOnly);                        
      m_Image.LoadWhiteMatterFile(m_WhiteMatterFileName);
      m_Image.LoadGreyMatterFile(m_GreyMatterFileName);                
    }
  
  //Connective component
  if(m_WhiteComponent)
    {
      std::cout<<" White connective component "<<std::endl;
      m_Image.SetWhiteMatterImage(m_Image.ConnectiveComponent(m_Image.GetWhiteMatterImage()));
    }
  if(m_GreyComponent)
    {
      std::cout<<" Grey connective component "<<std::endl;
      m_Image.SetGreyMatterImage(m_Image.ConnectiveComponent(m_Image.GetGreyMatterImage()));
    }
  
  //boundary computation
  std::cout<<" Boundary Computation "<<std::endl;
  m_Image.BoundaryComputation();
  
  //Danielson map computation                 
  std::cout<<" Danielsson Map "<<std::endl;
  if(m_segfile)   AddExtention(m_OriginalSegFileNameOnly,"-Danielsson",m_DanielssonFileName);
  else  AddExtention(m_OriginalWhiteFileNameOnly,"-Danielsson",m_DanielssonFileName);
  AddPathAndName(m_Output_PathName, m_DanielssonFileName, m_DanielssonFileName);
  m_Image.ComputeDM(m_DanielssonFileName,false);
  
  char WhiteFinalDistanceName[512];
  char VtkFileName[512];
  char AvgFileName[512];
  char DanGreyFileName[512];
 
  if(m_segfile)
  {
    AddExtention(m_OriginalSegFileNameOnly, "-WhiteMatDistanceMap",WhiteFinalDistanceName);
    AddExtention(m_OriginalSegFileNameOnly,"-VectorFile",VtkFileName);
    AddExtention(m_OriginalSegFileNameOnly,"-DistanceMapOnWhiteAvg",AvgFileName);
    AddExtention(m_OriginalSegFileNameOnly,"-GreyDan",DanGreyFileName);
    AddExtention(m_OriginalSegFileNameOnly,"-DistanceMapOnWhite",m_DistanceImageFileName);
    AddExtention(m_OriginalSegFileNameOnly,"-DistanceMapOnGrey",m_DistanceMapOnGrey);
    AddExtention(m_OriginalSegFileNameOnly,"-DistanceMapAverageOnGrey",m_DistanceMapAverageOnGrey);
  }  
  else
  {    
    AddExtention(m_OriginalWhiteFileNameOnly, "-WhiteMatDistanceMap",WhiteFinalDistanceName);
    AddExtention(m_OriginalWhiteFileNameOnly,"-VectorFile",VtkFileName);
    AddExtention(m_OriginalWhiteFileNameOnly,"-DistanceMapOnWhiteAvg",AvgFileName);
    AddExtention(m_OriginalGreyFileNameOnly,"-GreyDan",DanGreyFileName);
    AddExtention(m_OriginalGreyFileNameOnly,"-DistanceMapOnWhite",m_DistanceImageFileName);
    AddExtention(m_OriginalGreyFileNameOnly,"-DistanceMapOnGrey",m_DistanceMapOnGrey);
    AddExtention(m_OriginalGreyFileNameOnly,"-DistanceMapAverageOnGrey",m_DistanceMapAverageOnGrey);
  }      
  
  if(m_WriteWhiteDistance)
  	strcpy(AvgFileName,m_WhiteDistanceFile);
  else
  	AddPathAndName(m_Output_PathName,AvgFileName,AvgFileName);	
	
  if(m_WriteGreyDistance)
  	strcpy(DanGreyFileName,m_GreyDistanceFile);
  else
	AddPathAndName(m_Output_PathName,DanGreyFileName,DanGreyFileName);
  if(m_WriteDistanceImageMapOnWhite)
  	strcpy(m_DistanceImageFileName,m_DistanceImageMapOnWhite);
  else
  	AddPathAndName(m_Output_PathName,m_DistanceImageFileName,m_DistanceImageFileName);
  if(m_WriteDistanceImageMapOnGrey)
	strcpy(m_DistanceMapOnGrey,m_GMMapsDistImage);
  else
	AddPathAndName(m_Output_PathName,m_DistanceMapOnGrey,m_DistanceMapOnGrey);
  if(m_WriteAvgDistanceImageMapOnGrey)
	strcpy(m_DistanceMapAverageOnGrey,m_GMMapsAvgDistImage);
  else
	AddPathAndName(m_Output_PathName,m_DistanceMapOnGrey,m_DistanceMapOnGrey);


  AddPathAndName(m_Output_PathName,WhiteFinalDistanceName,WhiteFinalDistanceName);
  AddPathAndName(m_Output_PathName,VtkFileName,VtkFileName);
  //Compute the distance map
  m_Image.ComputeDistance(WhiteFinalDistanceName,
			  VtkFileName,
			  AvgFileName,
			  DanGreyFileName,
			  m_ParFileName,
			  m_DistanceImageFileName,        
			  m_WriteWhiteDistance,
			  m_VtkFile,
			  m_WriteGreyDistance,
			  m_ParFile,
			  m_Separe,
			  m_WriteDistanceImageMapOnWhite,
			  m_DistanceMapOnGrey,
			  m_DistanceMapAverageOnGrey,
			  m_WriteDistanceImageMapOnGrey,
			  m_WriteAvgDistanceImageMapOnGrey,
			  m_Interp,
			  m_Threshold);
 }

/**********************************************************************************/
