  #ifndef PIPELINEGUICONTROLS_H
#define PIPELINEGUICONTROLS_H

#include "PipelineGUI.h"
#include "ImageSyl.h"

#include <FL/Fl_File_Chooser.H>

/*! \brief Class to control the pipeline functions 
*
* The Pipeline to calculate the Thickness 
*/

class PipelineGUIControls : public PipelineGUI
{
public:
  PipelineGUIControls();
  virtual ~PipelineGUIControls();
        
  void LoadWhiteMatterGUI();
  void LoadGreyMatterGUI();
  void LoadDanielssonGUI();
  void LoadSegFileGUI();
  void SaveDanielsonMapGUI();
  void ComputeGUI();
  void SelectOutputDirectoryGUI();
  void ClosePipelineWidgetGUI();
  void WriteVtkFileGUI();
  void DanielssonComputedGUI();
  void ComputeDanGreyGUI();
  void TcheckLoadSegFileGUI();
  void CheckParcelFileGUI();
  void LoadParcelFileGUI();
	
private:
  
  bool TestExtention(const char *Name);
  void AddExtention(const char* OriginalName, const char *Extention, char* OutputName);
  void ExtractName(char *OriginalName, char* Path, char* FileName);
  void AddPathAndName(const char *Path, char *Name, char *Output);
  void WriteOutputReport(char * Text);
  
  char m_ImageSegFileName[512];
  char m_WhiteMatterFileName[512];
  char m_GreyMatterFileName[512];
  char m_ParcelFileName[512];
  char m_DanielssonFileName[512];
  
  char m_OriginalWhitePathName[512];
  char m_OriginalGreyPathName[512];
  char m_OriginalDanielssonPathName[512];
  char m_OriginalSegPathName[512];
  
  char m_OriginalWhiteFileNameOnly[512];
  char m_OriginalGreyFileNameOnly[512];
  char m_OriginalSegFileNameOnly[512];
  char m_OriginalDanielssonFileNameOnly[512];
  
  const char *m_FinalOutputDirectory;
  char m_CurrentPath[512];
  
  bool m_WhiteMatterLoaded;
  bool m_GreyMatterLoaded;
  bool m_DanielssonMatterLoaded;
  bool m_ImageSegLoaded;
  bool m_ParcelLoad;
  ImageSyl m_Image;
};

#endif
