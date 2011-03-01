#ifndef PipelineCommand_H
#define PipelineCommand_H

#include "ImageSyl.h"

//#include <FL/Fl_File_Chooser.H>

/*! \brief pipeline command class
*
* this class is used for the command line version of CortThick. 
* 
* 
*/

class PipelineCommand 
{
public:
 
  //CortThickCLP Constructors
  PipelineCommand(int,int,const char*,const char*,bool,bool,bool,const char*,bool,const char*,const char*,bool,const char *,const char*,const char*,bool,float);
  PipelineCommand(const char*,const char*,const char*,bool,bool,bool,const char*, bool,const char*,const char*,bool,const char *,const char*,const char*,bool,float);

  //CortThick Constructors
  PipelineCommand(const char*,const char*,const char*,bool,bool,bool, const char*, bool, bool, bool, bool, bool, bool,bool,float);
  PipelineCommand(int, int,const char*,const char*,bool,bool,bool, const char*, bool, bool, bool, bool, bool, bool,bool,float);
  ~PipelineCommand();
  
  void ComputeGUI();
  void ComputeCLP();

  bool m_Separe;	

private:
	
  /** To test the Extension of the input */
  bool TestExtention(const char *Name);
  
  /** Choose the directory where the file is located */
//  const char *ChooseDirectory(char * Text);

  /** Add an extension at the output */
  void AddExtention(const char* OriginalName, const char *Extention, char* OutputName);
  
  /** To extract the name of the intput, means delete the path name and just keep the name */
  void ExtractName(char *OriginalName, char* Path, char* FileName);
  
  /**Add the path name to the output */
  void AddPathAndName(const char *Path, char *Name, char *Output);
  void WriteOutputReport(char * Text);
	
  char m_WhiteMatterFileName[512];
  char m_GreyMatterFileName[512];
  char m_Output_PathName[512];
  char m_DanielssonFileName[512];
  char m_SegFileName[512];
  char m_ParFileName[512];
  char m_DistanceImageFileName[512]; 
  char m_DistanceMapOnGrey[512];
  char m_DistanceMapAverageOnGrey[512];
  
  int m_levelWhite;
  int m_levelGrey;
              
  bool m_DanielssonComputed;
  bool m_WhiteComponent;
  bool m_GreyComponent;
  bool m_VtkFile;
  bool m_segfile;
  bool m_ParFile;
  bool m_WriteWhiteDistance;
  char m_WhiteDistanceFile[512];
  char m_GreyDistanceFile[512];
  char m_DistanceImageMapOnGrey[512];
  char m_DistanceImageMapOnWhite[512];
  char m_GMMapsDistImage[512];
  char m_GMMapsAvgDistImage[512];
  bool m_WriteGreyDistance;
  bool m_WriteDistanceImageMapOnWhite; 
  bool m_WriteDistanceImageMapOnGrey;
  bool m_WriteAvgDistanceImageMapOnGrey;
  bool m_Interp;

  float m_Threshold;

  char m_OriginalWhitePathName[512];
  char m_OriginalGreyPathName[512];
  char m_OriginalSegPathName[512];
  char m_OriginalWhiteFileNameOnly[512];
  char m_OriginalGreyFileNameOnly[512];
  char m_OriginalSegFileNameOnly[512];

  ImageSyl m_Image;
};

#endif

