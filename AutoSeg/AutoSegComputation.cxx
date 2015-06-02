/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: AutoSegComputation.cxx,v $
  Language:  C++
  Date:      $Date: 2011/04/19 08:04:40 $
  Version:   $Revision: 1.45 $
  Author:    Clement Vachet

  Updates:   1. Set ${ANTSIteration} earlier (10-30-2013)
             2. Modified the ANTS registration for TissueSegAtlas with additonal options of --initial-affine <transform from BRAINSFit> and --continue-affine false (10-30-2013)
             3. Fixed the fixed image of ANTS for multi-atlas seg (10-30-2013)  
             4. Fixed the ABC Affine registration issue (11-01-2013)  
             5. Recalculate atlas-atlas registration, atlas-atlas energy, atlas-target energy modified (11-01-2013)  
             6. Fixed the identification of skull stripped t1 and t2 images (11-05-2013)  
             7. Changed the -threshold option of ImageMath for skull stripping to 1,10000 from 1,3 (11-22-2013)  
             8. Modified the size of m_MultiAtlasList. So it can present the data corrctly when process multiple cases (11-26-2013)  
             9. Added an option to use/unuse brainmask for the tisssue segmentation (11-27-2013)  
            10. Second loop of the tisssue segmentation will not use brainmask and continueaffine (11-27-2013)  
            11. Replaced m_MultiAtlasList with m_MultiAtlasT1List and m_MultiAtlasT2List. (12-2-2013)  
            12. Modified the identification of skull stripped t1 and t2 cases. (12-4-2013)  
            13. Modified the name of multi-modality atlases images in computing energies. (1-15-2014)  
            14. ANTS parameters for the 2nd modality. (1-22-2014)  


  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "AutoSegComputation.h"
#include <ctime>

#ifdef _POSIX_SOURCE

//for killing child processes in UNIX
#include <signal.h>
#include <unistd.h>

#endif

/*
bool stringCompare( const std::string &left, const std::string &right )
{
    for( std::string::const_iterator lit = left.begin(), rit = right.begin(); lit != left.end() && rit != right.end(); ++lit, ++rit )
        if( tolower( *lit ) < tolower( *rit ) )
            return 1;
        else if( tolower( *lit ) > tolower( *rit ) )
            return 0;
    if( left.size() < right.size() )
        return 1;

    return 0;
}
*/

AutoSegComputation::AutoSegComputation()
{
  m_AllocationData=0;
  m_AllocationAuxData=0;
  m_NbTissueClass = 0;
  m_NbStrippedTissueClass = 0;
  m_IsAuxT1Image = false;
  m_IsAuxT2Image = false;
  m_IsAuxPDImage = false;
  m_IsAux1Image = false;
  m_IsAux2Image = false;
  m_IsAux3Image = false;
  m_IsAux4Image = false;
  m_IsAux5Image = false;
  m_IsAux6Image = false;
  m_IsAux7Image = false;
  m_IsAux8Image = false;
  m_AtlasLoop[0] = '\0';

  ToolsPairType toolPair ;
  std::vector< std::string > toolNames ;
  //ResampleVolume2
  toolNames.push_back( "ResampleScalarVectorDWIVolume" ) ;
  toolNames.push_back( "ResampleVolume2" ) ;
  toolPair = std::make_pair( "ResampleVolume2" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //convertITKformats
  toolNames.push_back( "convertITKformats" ) ;
  toolPair = std::make_pair( "convertITKformats" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //ABC
  toolNames.push_back( "ABC" ) ;
  toolPair = std::make_pair( "ABC" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //ANTS
  toolNames.push_back( "ANTS" ) ;
  toolPair = std::make_pair( "ANTS" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //WarpImageMultiTransform
  toolNames.push_back( "WarpImageMultiTransform" ) ;
  toolPair = std::make_pair( "WarpImageMultiTransform" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //BRAINSDemonWarp
  toolNames.push_back( "BRAINSDemonWarp" ) ;
  toolPair = std::make_pair( "BRAINSDemonWarp" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //BRAINSFit
  toolNames.push_back( "BRAINSFit" ) ;
  toolPair = std::make_pair( "BRAINSFit" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //BRAINSResample
  toolNames.push_back( "BRAINSResample" ) ;
  toolPair = std::make_pair( "BRAINSResample" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //CortThickCLP
  toolNames.push_back( "CortThickCLP" ) ;
  toolPair = std::make_pair( "CortThickCLP" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //ImageMath
  toolNames.push_back( "ImageMath" ) ;
  toolPair = std::make_pair( "ImageMath" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //ImageStat
  toolNames.push_back( "ImageStat" ) ;
  toolPair = std::make_pair( "ImageStat" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //IntensityRescaler
  toolNames.push_back( "IntensityRescaler" ) ;
  toolPair = std::make_pair( "IntensityRescaler" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //MultiAtlasSeg
  toolNames.push_back( "MultiAtlasSeg" ) ;
  toolPair = std::make_pair( "MultiAtlasSeg" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //N4ITKBiasFieldCorrection
  toolNames.push_back( "N4ITKBiasFieldCorrection" ) ;
  toolPair = std::make_pair( "N4ITKBiasFieldCorrection" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //neoseg
  toolNames.push_back( "neoseg" ) ;
  toolPair = std::make_pair( "neoseg" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //SegPostProcessCLP
  toolNames.push_back( "SegPostProcessCLP" ) ;
  toolPair = std::make_pair( "SegPostProcessCLP" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //WarpTool
  toolNames.push_back( "WarpTool" ) ;
  toolPair = std::make_pair( "WarpTool" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //imconvert3
  toolNames.push_back( "imconvert3" ) ;
  toolPair = std::make_pair( "imconvert3" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //fWarp
  toolNames.push_back( "fWarp" ) ;
  toolPair = std::make_pair( "fWarp" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
  //txApply
  toolNames.push_back( "txApply" ) ;
  toolPair = std::make_pair( "txApply" , toolNames ) ;
  m_RequiredTools.insert( toolPair ) ;
  toolNames.clear() ;
}

AutoSegComputation::~AutoSegComputation()
{
  if(m_AllocationData==1)
    DesallocateDataList();
  if(m_AllocationAuxData==1)
    DesallocateAuxDataList();
}

void AutoSegComputation::SetBMSAutoSegFile()
{
  std::strcpy(m_BMSAutoSegFile, GetProcessDataDirectory());
  std::strcat(m_BMSAutoSegFile, "AutoSeg.bms");
}

void AutoSegComputation::SetBMSAutoSegMainFile()
{
  std::strcpy(m_BMSAutoSegMainFile, GetProcessDataDirectory());
  std::strcat(m_BMSAutoSegMainFile, "MainAutoSeg.bms");  
}

void AutoSegComputation::SetBMSAutoSegAuxFile()
{
  std::strcpy(m_BMSAutoSegAuxFile, GetProcessDataDirectory());
  std::strcat(m_BMSAutoSegAuxFile, "AuxAutoSeg.bms");  
}

void AutoSegComputation::SetBMSAutoSegMRMLSourceFile()
{
  std::strcpy(m_BMSAutoSegMRMLSourceFile, GetProcessDataDirectory());
  std::strcat(m_BMSAutoSegMRMLSourceFile, "SourceMRMLAutoSeg.bms");  
}

void AutoSegComputation::SetBMSAutoSegMRMLParcelFile()
{
  std::strcpy(m_BMSAutoSegMRMLParcelFile, GetProcessDataDirectory());
  std::strcat(m_BMSAutoSegMRMLParcelFile, "ParcelMRMLAutoSeg.bms");  
}

void AutoSegComputation::SetBMSAutoSegMRMLAllROIFile()
{
  std::strcpy(m_BMSAutoSegMRMLAllROIFile, GetProcessDataDirectory());
  std::strcat(m_BMSAutoSegMRMLAllROIFile, "AllROIMRMLAutoSeg.bms");  
}

void AutoSegComputation::SetDataFile()
{
  std::strcpy(m_DataFile, GetProcessDataDirectory());
  std::strcat(m_DataFile, "AutoSeg_Data.txt");
}

void AutoSegComputation::SetAuxDataFile()
{
  std::strcpy(m_AuxDataFile, GetProcessDataDirectory());
  std::strcat(m_AuxDataFile, "AutoSeg_AuxData.txt");
}

void AutoSegComputation::SetParameterFile()
{
  std::strcpy(m_ParameterFile, GetProcessDataDirectory());
  std::strcat(m_ParameterFile, "AutoSeg_Parameters.txt");
}

void AutoSegComputation::SetParameterFile(const char *_ParameterFile)
{
  std::strcpy(m_ParameterFile, _ParameterFile);
}

void AutoSegComputation::SetComputationFile()
{
  std::strcpy(m_ComputationFile, GetProcessDataDirectory());
  std::strcat(m_ComputationFile, "AutoSeg_Computation.txt");
}

void AutoSegComputation::SetComputationFile(const char *_ComputationFile)
{
  std::strcpy(m_ComputationFile, _ComputationFile);
}

void AutoSegComputation::SetLogFile()
{
  std::strcpy(m_LogFile, GetProcessDataDirectory());
  std::strcat(m_LogFile, "AutoSeg.log");
}

void AutoSegComputation::AllocateDataList()
{
    int DataNumber;

    if ( !GetT2Image() && !GetPDImage()) {
        m_T1List = new char *[GetNbData()];
        for (DataNumber = 0; DataNumber < GetNbData(); DataNumber++)
        {
            m_T1List[DataNumber] = new char[2048];
            std::strcpy(m_T1List[DataNumber], "");
        }
        if (GetT2Image())
        {
            m_T2List = new char *[GetNbData()];
            for (DataNumber = 0; DataNumber < GetNbData(); DataNumber++)
            {
              m_T2List[DataNumber] = new char[2048];  
              std::strcpy(m_T2List[DataNumber], "");
            }
        }
        else
            m_T2List=NULL;
        if (GetPDImage())
        {
            m_PDList = new char *[GetNbData()];
            for (DataNumber = 0; DataNumber < GetNbData(); DataNumber++)
            {
                m_PDList[DataNumber] = new char[2048];
                std::strcpy(m_PDList[DataNumber], "");
            }
        }
        else
            m_PDList=NULL;
        m_AllocationData=1;

        m_MultiAtlasT1List = new char *[GetNbData()];
        for (DataNumber = 0; DataNumber < GetNbData(); DataNumber++)
        {
            m_MultiAtlasT1List[DataNumber] = new char[2048];
            std::strcpy(m_MultiAtlasT1List[DataNumber], "");
        }
        m_MultiAtlasT2List = new char *[GetNbData()];
        for (DataNumber = 0; DataNumber < GetNbData(); DataNumber++)
        {
            m_MultiAtlasT2List[DataNumber] = new char[2048];
            std::strcpy(m_MultiAtlasT2List[DataNumber], "");
        }
    }
    else if (GetT2Image() && GetPDImage())  {
        m_T1List = new char *[GetNbData() * 3];
        for (DataNumber = 0; DataNumber < GetNbData() * 3; DataNumber++)
        {
            m_T1List[DataNumber] = new char[2048];
            std::strcpy(m_T1List[DataNumber], "");
        }
        if (GetT2Image())
        {
            m_T2List = new char *[GetNbData() * 3];
            for (DataNumber = 0; DataNumber < GetNbData() * 3; DataNumber++)
            {
                m_T2List[DataNumber] = new char[2048];  
                std::strcpy(m_T2List[DataNumber], "");
            }
        }
        else
            m_T2List=NULL;
        if (GetPDImage())
        {
            m_PDList = new char *[GetNbData() * 3];
            for (DataNumber = 0; DataNumber < GetNbData() * 3; DataNumber++)
            {
                m_PDList[DataNumber] = new char[2048];
                std::strcpy(m_PDList[DataNumber], "");
            }
        }
        else
            m_PDList=NULL;
        m_AllocationData=1;

        m_MultiAtlasT1List = new char *[GetNbData() * 3];
        for (DataNumber = 0; DataNumber < GetNbData() * 3; DataNumber++)
        {
            m_MultiAtlasT1List[DataNumber] = new char[2048];
            std::strcpy(m_MultiAtlasT1List[DataNumber], "");
        }
        m_MultiAtlasT2List = new char *[GetNbData() * 3];
        for (DataNumber = 0; DataNumber < GetNbData() * 3; DataNumber++)
        {
            m_MultiAtlasT2List[DataNumber] = new char[2048];
            std::strcpy(m_MultiAtlasT2List[DataNumber], "");
        }
    }  
    else if (GetT2Image()) {
        m_T1List = new char *[GetNbData() * 2];
        for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
        {
            m_T1List[DataNumber] = new char[2048];
            std::strcpy(m_T1List[DataNumber], "");
        }
        if (GetT2Image())
        {
            m_T2List = new char *[GetNbData() * 2];
            for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
            {
                m_T2List[DataNumber] = new char[2048];  
                std::strcpy(m_T2List[DataNumber], "");
            }
        }
        else
            m_T2List=NULL;
        if (GetPDImage())
        {
            m_PDList = new char *[GetNbData() * 2];
            for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
            {
                m_PDList[DataNumber] = new char[2048];
                std::strcpy(m_PDList[DataNumber], "");
            }
        }
        else
            m_PDList=NULL;
        m_AllocationData=1;

        m_MultiAtlasT1List = new char *[GetNbData() * 2];
        for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
        {
            m_MultiAtlasT1List[DataNumber] = new char[2048];
            std::strcpy(m_MultiAtlasT1List[DataNumber], "");
        }
        m_MultiAtlasT2List = new char *[GetNbData() * 2];
        for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
        {
            m_MultiAtlasT2List[DataNumber] = new char[2048];
            std::strcpy(m_MultiAtlasT2List[DataNumber], "");
        }
    }
    else {
        m_T1List = new char *[GetNbData() * 2];
        for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
        {
            m_T1List[DataNumber] = new char[2048];
            std::strcpy(m_T1List[DataNumber], "");
        }
        if (GetT2Image())
        {
            m_T2List = new char *[GetNbData() * 2];
            for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
            {
                m_T2List[DataNumber] = new char[2048];  
                std::strcpy(m_T2List[DataNumber], "");
            }
        }
        else
            m_T2List=NULL;
        if (GetPDImage())
        {
            m_PDList = new char *[GetNbData() * 2];
            for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
            {
                m_PDList[DataNumber] = new char[2048];
                std::strcpy(m_PDList[DataNumber], "");
            }
        }
        else
            m_PDList=NULL;
        m_AllocationData=1;

        m_MultiAtlasT1List = new char *[GetNbData() * 2];
        for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
        {
            m_MultiAtlasT1List[DataNumber] = new char[2048];
            std::strcpy(m_MultiAtlasT1List[DataNumber], "");
        }
        m_MultiAtlasT2List = new char *[GetNbData() * 2];
        for (DataNumber = 0; DataNumber < GetNbData() * 2; DataNumber++)
        {
            m_MultiAtlasT2List[DataNumber] = new char[2048];
            std::strcpy(m_MultiAtlasT2List[DataNumber], "");
        }
    }

}

void AutoSegComputation::AllocateAuxDataList()
{
  int DataNumber;
  if (m_Manually)
  {
    if (GetAuxT1Image())
    {
      m_AuxT1List = new char *[GetNbAuxData()];
      for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
      {
	m_AuxT1List[DataNumber] = new char[2048];
	std::strcpy(m_AuxT1List[DataNumber], "");
      }
    }
    if (GetAuxT2Image())
    {
      m_AuxT2List = new char *[GetNbAuxData()];
      for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
      {
	m_AuxT2List[DataNumber] = new char[2048];
	std::strcpy(m_AuxT2List[DataNumber], "");
      }
    } 
    if (GetAuxPDImage())
    {
      m_AuxPDList = new char *[GetNbAuxData()];
      for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
      {
	m_AuxPDList[DataNumber] = new char[2048];
	std::strcpy(m_AuxPDList[DataNumber], "");
      }
    }
  }
  if (GetAux1Image())
  {
    m_Aux1List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux1List[DataNumber] = new char[2048];
      std::strcpy(m_Aux1List[DataNumber], "");
    }
  }
  if (GetAux2Image())
  {
    m_Aux2List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux2List[DataNumber] = new char[2048];
      std::strcpy(m_Aux2List[DataNumber], "");
    }
  }
  if (GetAux3Image())
  {
    m_Aux3List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux3List[DataNumber] = new char[2048];
      std::strcpy(m_Aux3List[DataNumber], "");
    }
  }
  if (GetAux4Image())
  {
    m_Aux4List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux4List[DataNumber] = new char[2048];
      std::strcpy(m_Aux4List[DataNumber], "");
    }
  }
  if (GetAux5Image())
  {
    m_Aux5List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux5List[DataNumber] = new char[2048];
      std::strcpy(m_Aux5List[DataNumber], "");
    }
  }
  if (GetAux6Image())
  {
    m_Aux6List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux6List[DataNumber] = new char[2048];
      std::strcpy(m_Aux6List[DataNumber], "");
    }
  }
  if (GetAux7Image())
  {
    m_Aux7List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux7List[DataNumber] = new char[2048];
      std::strcpy(m_Aux7List[DataNumber], "");
    }
  }
  if (GetAux8Image())
  {
    m_Aux8List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux8List[DataNumber] = new char[2048];
      std::strcpy(m_Aux8List[DataNumber], "");
    }
  }
  m_AllocationAuxData=1;
}

void AutoSegComputation::DesallocateDataList()
{
  int DataNumber;

  if (m_T1List)
  {
    for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
      delete[] m_T1List[DataNumber];
    delete []m_T1List;
    m_T1List = NULL;
  }
  if (m_T2List)
  {
    for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
      delete[] m_T2List[DataNumber];
    delete []m_T2List;
    m_T2List = NULL;
  }
  if (m_PDList)
  {
    for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
      delete[] m_PDList[DataNumber];
    delete []m_PDList;
    m_PDList = NULL;
  }
}

void AutoSegComputation::DesallocateAuxDataList()
{
  int DataNumber;
  if (m_Manually)
  {
    if (m_AuxT1List)
    {
      for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
	delete[] m_AuxT1List[DataNumber];
      delete []m_AuxT1List;  
      m_AuxT1List = NULL;
    }
    if (m_AuxT2List)
    {
      for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
	delete[] m_AuxT2List[DataNumber];
      delete []m_AuxT2List;  
      m_AuxT2List = NULL;
    }
    if (m_AuxPDList)
    {
      for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
	delete[] m_AuxPDList[DataNumber];
      delete []m_AuxPDList;
      m_AuxPDList = NULL;
    }
  }
  if (m_Aux1List)
  {
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
      delete[] m_Aux1List[DataNumber];
    delete []m_Aux1List;  
    m_Aux1List = NULL;
  }
  if (m_Aux2List)
  {
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
      delete[] m_Aux2List[DataNumber];
    delete []m_Aux2List;  
    m_Aux2List = NULL;
  }
  if (m_Aux3List)
  {
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
      delete[] m_Aux3List[DataNumber];
    delete []m_Aux3List;
    m_Aux3List = NULL;
  }
  if (m_Aux4List)
  {
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
      delete[] m_Aux4List[DataNumber];
    delete []m_Aux4List;
    m_Aux4List = NULL;
  }
  if (m_Aux5List)
  {
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
      delete[] m_Aux5List[DataNumber];
    delete []m_Aux5List;
    m_Aux5List = NULL;
  }
  if (m_Aux6List)
  {
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
      delete[] m_Aux6List[DataNumber];
    delete []m_Aux6List;
    m_Aux6List = NULL;
  }
  if (m_Aux7List)
  {
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
      delete[] m_Aux7List[DataNumber];
    delete []m_Aux7List;
    m_Aux7List = NULL;
  }
  if (m_Aux8List)
  {
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
      delete[] m_Aux8List[DataNumber];
    delete []m_Aux8List;
    m_Aux8List = NULL;
  }
}

//_GUIMode=1 : With GUI
//_GUIMode=0 : Without GUI
void AutoSegComputation::SetDataList(const char *_Data, int _DataNumber, bool _GUIMode)
{
    if (_GUIMode==0)
    {
        if ( !GetT2Image() && !GetPDImage()) {
            SetData(_Data, m_T1List[_DataNumber]);
            SetData(_Data, m_MultiAtlasT1List[_DataNumber]);
        }
        else if (GetT2Image() && GetPDImage()) 
            SetData(_Data, m_T1List[_DataNumber], m_T2List[_DataNumber], m_PDList[_DataNumber]);
        else if (GetT2Image()) { //No PD image, but T2 image is present
            SetData(_Data, m_T1List[_DataNumber], m_T2List[_DataNumber]);
            SetData(_Data, m_MultiAtlasT1List[_DataNumber], m_MultiAtlasT2List[_DataNumber]);
            std::cout << "T1 data " << _DataNumber << " : " << m_MultiAtlasT1List[_DataNumber] << "  T2 data: " << m_MultiAtlasT2List[_DataNumber] << std::endl;
        }
        else
            SetData(_Data, m_T1List[_DataNumber], m_PDList[_DataNumber]);
    }
    else
    {
        if ( !GetT2Image() && !GetPDImage()) {
            SetData(_Data, m_T1List[_DataNumber]);
            SetData(_Data, m_MultiAtlasT1List[_DataNumber]);
        }
        else if (GetT2Image() && GetPDImage()) 
            SetData(_Data, m_T1List[_DataNumber], m_T2List[_DataNumber], m_PDList[_DataNumber]);
        else if (GetT2Image()) {
            SetData(_Data, m_T1List[_DataNumber], m_T2List[_DataNumber]);
            SetData(_Data, m_MultiAtlasT1List[_DataNumber], m_MultiAtlasT2List[_DataNumber]);
        }
        else
            SetData(_Data, m_T1List[_DataNumber], m_PDList[_DataNumber]);
    }
}

void AutoSegComputation::SetAtlasList(const char *_Data, int _DataNumber)
{
    m_AtlasList[_DataNumber] = new char[FILE_LIST_LENGTH]; 
    for (int j = 0; j < FILE_LIST_LENGTH; j++)
        m_AtlasList[_DataNumber][j] = ' ';
    SetData(_Data, m_AtlasList[_DataNumber]);
}

void AutoSegComputation::Set2ndAtlasList(const char *_Data, int _DataNumber)
{
    m_2ndAtlasList[_DataNumber] = new char[FILE_LIST_LENGTH]; 
    for (int j = 0; j < FILE_LIST_LENGTH; j++)
        m_2ndAtlasList[_DataNumber][j] = ' ';
    SetData(_Data, m_2ndAtlasList[_DataNumber]);
}

void AutoSegComputation::SetAtlasLabelList(const char *_Data, int _DataNumber)
{
    m_AtlasLabelList[_DataNumber] = new char[FILE_LIST_LENGTH];
    for (int j = 0; j < FILE_LIST_LENGTH; j++)
        m_AtlasLabelList[_DataNumber][j] = ' ';
    SetData(_Data, m_AtlasLabelList[_DataNumber]);
}

void AutoSegComputation::SetAtlasParcellationList(const char *_Data, int _DataNumber)
{
    m_AtlasParcellationList[_DataNumber] = new char[FILE_LIST_LENGTH];
    for (int j = 0; j < FILE_LIST_LENGTH; j++)
        m_AtlasParcellationList[_DataNumber][j] = ' ';
    SetData(_Data, m_AtlasParcellationList[_DataNumber]);
}

int AutoSegComputation::SetNbTissueClass(const char *_Directory)
{
    DIR *dir;
    struct dirent *ent;

    m_NbTissueClass = 0;
    if ((dir = opendir (_Directory)) != NULL)
    {
        std::string filename;
        while ((ent = readdir (dir)) != NULL)
        {
            filename = std::string(ent->d_name);
            int nbClass = 0;
            char extension[100];
            int numVal = sscanf(filename.c_str(),"%d.%s",&nbClass, extension);
            if( numVal == 2 && nbClass != 0 && !strcmp(extension,"mha"))
            {
                m_NbTissueClass++;
            }
        }
        closedir (dir);
    }
    else
    {
        return 1 ;
    }
    if( m_NbTissueClass == 0 )
    {
        return 2 ;
    }
    std::cout << "number of Tissues: " << m_NbTissueClass << std::endl;
    return 0 ;
}

int AutoSegComputation::SetNbStrippedTissueClass(const char *_Directory)
{
    DIR *dir;
    struct dirent *ent;

    m_NbStrippedTissueClass = 0;
    if ((dir = opendir (_Directory)) != NULL)
    {
        std::string filename;
        while ((ent = readdir (dir)) != NULL)
        {
            filename = std::string(ent->d_name);
            int nbClass = 0;
            char extension[100];
            int numVal = sscanf(filename.c_str(),"%d.%s",&nbClass, extension);
            if( numVal == 2 && nbClass != 0 && !strcmp(extension,"mha"))
            {
                m_NbStrippedTissueClass++;
            }
        }
        closedir (dir);
    }
    else
    {
        return 1 ;
    }
    std::cout << "number of Stripped Tissues: " << m_NbStrippedTissueClass << std::endl;
    return 0 ;
}

void AutoSegComputation::SetWarpedAtlasList(const char *_Directory )
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                i++;
            }
        }
        SetNbWarpedAtlas(i);
    }
    closedir (dir);

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                m_WarpedAtlasList[i] = new char[FILE_LIST_LENGTH];
                for (int j = 0; j < FILE_LIST_LENGTH; j++)
                    m_WarpedAtlasList[i][j] = ' ';
                SetData(filename.c_str(), m_WarpedAtlasList[i]);
                i++;
            }
        }
        SortStringList(m_WarpedAtlasList, i);
    }
    closedir (dir);

}

void AutoSegComputation::SetWarpedAtlasTrainToTrainList(const char *_Directory )
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                i++;
            }
        }
        SetNbWarpedAtlasTrainToTrain(i);
    }
    closedir (dir);

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                m_WarpedAtlasTrainToTrainList[i] = new char[FILE_LIST_LENGTH];
                for (int j = 0; j < FILE_LIST_LENGTH; j++)
                    m_WarpedAtlasTrainToTrainList[i][j] = ' ';
                SetData(filename.c_str(), m_WarpedAtlasTrainToTrainList[i]);
                i++;
            }
        }
        SortStringList(m_WarpedAtlasTrainToTrainList, i);
    }
    closedir (dir);
}

void AutoSegComputation::SetWarpedLabelList(const char *_Directory )
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                i++;
            }
        }
        SetNbWarpedLabel(i);
    }
    closedir (dir);

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                m_WarpedLabelList[i] = new char[FILE_LIST_LENGTH];
                for (int j = 0; j < FILE_LIST_LENGTH; j++)
                    m_WarpedLabelList[i][j] = ' ';
                SetData(filename.c_str(), m_WarpedLabelList[i]);
                i++;
            }
        }
        SortStringList(m_WarpedLabelList, i);
    }
    closedir (dir);

}

void AutoSegComputation::SetDeformationFieldList(const char *_Directory )
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                i++;
            }
        }
        SetNbDeformationField(i);
    }
    closedir (dir);

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                m_DeformationFieldList[i] = new char[FILE_LIST_LENGTH];
                for (int j = 0; j < FILE_LIST_LENGTH; j++)
                    m_DeformationFieldList[i][j] = ' ';
                SetData(filename.c_str(), m_DeformationFieldList[i]);
                i++;
            }
        }
        SortStringList(m_DeformationFieldList, i);
    }
    closedir (dir);
}

void AutoSegComputation::SetDeformationFieldTrainToTrainList(const char *_Directory )
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                i++;
            }
        }
        SetNbDeformationFieldTrainToTrain(i);
    }
    closedir (dir);

    if ((dir = opendir (_Directory)) != NULL) {
        std::string filename;
        int i = 0;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                m_DeformationFieldTrainToTrainList[i] = new char[FILE_LIST_LENGTH];
                for (int j = 0; j < FILE_LIST_LENGTH; j++)
                    m_DeformationFieldTrainToTrainList[i][j] = ' ';
                SetData(filename.c_str(), m_DeformationFieldTrainToTrainList[i]);
                i++;
            }
        }
        SortStringList(m_DeformationFieldTrainToTrainList, i);
    }
    closedir (dir);
}

void AutoSegComputation::SortStringList(char **strList, int size)
{
    std::vector<std::string> stringarray;

    for (int i = 0; i < size ; i++){
        stringarray.push_back(strList[i]);
    }
    //std::sort(stringarray.begin(), stringarray.end(), stringCompare);
    std::sort(stringarray.begin(), stringarray.end());
         
    int j = 0;
    for (std::vector<std::string>::iterator it=stringarray.begin(); it!=stringarray.end(); ++it) {
        std::string tmp = *it;
        strcpy(strList[j], tmp.c_str());
        //std::cout << strList[j] << std::endl;
        j++;
    }
}

void AutoSegComputation::SetAuxDataList(const char *_Data, int _DataNumber)
{
  if ( !GetAux1Image() && !GetAux2Image() && !GetAux3Image() && !GetAux4Image() && !GetAux5Image() && !GetAux6Image() && !GetAux7Image() && !GetAux8Image()) 
  {
    if (!m_Manually)
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_T1List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_T2List[_DataNumber]);
      else
	SetAuxData(_Data, m_PDList[_DataNumber]);
    }
    else
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_AuxT1List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_AuxT2List[_DataNumber]);
      else
	SetAuxData(_Data, m_AuxPDList[_DataNumber]);
    }
  }
  else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image() && GetAux5Image() && GetAux6Image() && GetAux7Image() && GetAux8Image()) 
  {
    if (!m_Manually)
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_T1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber], m_Aux8List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_T2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber], m_Aux8List[_DataNumber]);
      else
	SetAuxData(_Data, m_PDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber], m_Aux8List[_DataNumber]);
    }
    else
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_AuxT1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber], m_Aux8List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_AuxT2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber], m_Aux8List[_DataNumber]);
      else
	SetAuxData(_Data, m_AuxPDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber], m_Aux8List[_DataNumber]);
    }
  }
  else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image() && GetAux5Image() && GetAux6Image() && GetAux7Image()) 
  {
    if (!m_Manually)
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_T1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_T2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber]);
      else
	SetAuxData(_Data, m_PDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber]);
    }
    else
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_AuxT1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_AuxT2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber]);
      else
	SetAuxData(_Data, m_AuxPDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber], m_Aux7List[_DataNumber]);
    }
  }
  else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image() && GetAux5Image() && GetAux6Image())
  {
    if (!m_Manually)
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_T1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_T2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber]);
      else
	SetAuxData(_Data, m_PDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber]);
    }
    else
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_AuxT1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_AuxT2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber]);
      else
	SetAuxData(_Data, m_AuxPDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber], m_Aux6List[_DataNumber]);
    }
  }
  else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image() && GetAux5Image())
  {
    if (!m_Manually)
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_T1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_T2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber]);
      else
	SetAuxData(_Data, m_PDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber]);
    }
    else
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_AuxT1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_AuxT2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber]);
      else
	SetAuxData(_Data, m_AuxPDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber], m_Aux5List[_DataNumber]);
    }
  }
  else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image())
  {
    if (!m_Manually)
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_T1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_T2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber]);
      else
	SetAuxData(_Data, m_PDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber]);
    }
    else
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_AuxT1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_AuxT2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber]);
      else
	SetAuxData(_Data, m_AuxPDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber], m_Aux4List[_DataNumber]);
    }
  }
  else if (GetAux1Image() && GetAux2Image() && GetAux3Image())
  {
    if (!m_Manually)
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_T1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_T2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber]);
      else
	SetAuxData(_Data, m_PDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber]);
    }
    else
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_AuxT1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_AuxT2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber]);
      else
	SetAuxData(_Data, m_AuxPDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber], m_Aux3List[_DataNumber]);
    }
  }
  else if (GetAux1Image() && GetAux2Image()) 
  {
    if (!m_Manually)
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_T1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_T2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber]);
      else
	SetAuxData(_Data, m_PDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber]);
    }
    else
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_AuxT1List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_AuxT2List[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber]);
      else
	SetAuxData(_Data, m_AuxPDList[_DataNumber], m_Aux1List[_DataNumber], m_Aux2List[_DataNumber]);
    }
  }
  else
  {	
    if (!m_Manually)
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_T1List[_DataNumber], m_Aux1List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_T2List[_DataNumber], m_Aux1List[_DataNumber]);
      else
	SetAuxData(_Data, m_PDList[_DataNumber], m_Aux1List[_DataNumber]);
    }
    else
    {
      if (GetAuxT1Image())
	SetAuxData(_Data, m_AuxT1List[_DataNumber], m_Aux1List[_DataNumber]);
      else if (GetAuxT2Image())
	SetAuxData(_Data, m_AuxT2List[_DataNumber], m_Aux1List[_DataNumber]);
      else
	SetAuxData(_Data, m_AuxPDList[_DataNumber], m_Aux1List[_DataNumber]);
    }
  }
}

const char * AutoSegComputation::PrepData (const char *_Data)
{
  std::string * DataString = new std::string(_Data);
  std::size_t pos = DataString->find( "@r" );
  while (pos != string::npos) {
        DataString->replace(pos,2,"  ");
        pos = DataString->find( "@r" );
  }
  //std::cout << "PrepData: " << *DataString << std::endl;
  return DataString->c_str();
}

void AutoSegComputation::SetData(const char *_Data, char *_T1)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s ", _T1);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s ", _T1);
}

void AutoSegComputation::SetData(const char *_Data, char *_T1, char *_T2)
{
  
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s ", _T1, _T2);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_SecondImage)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s ", _T1, _SecondImage);
}

//Set Data: T1, T2 and PD files
void AutoSegComputation::SetData(const char *_Data, char *_T1, char *_T2, char *_PD)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s %s ", _T1, _T2, _PD);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s %s ", _T1, _Aux1, _Aux2);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s %s %s", _T1, _Aux1, _Aux2, _Aux3);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s %s %s %s", _T1, _Aux1, _Aux2, _Aux3, _Aux4);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s %s %s %s %s ", _T1, _Aux1, _Aux2, _Aux3, _Aux4, _Aux5);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5, char *_Aux6)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s %s %s %s %s %s ", _T1, _Aux1, _Aux2, _Aux3, _Aux4, _Aux5, _Aux6);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5, char *_Aux6, char *_Aux7)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s %s %s %s %s %s %s ", _T1, _Aux1, _Aux2, _Aux3, _Aux4, _Aux5, _Aux6, _Aux7);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5, char *_Aux6, char *_Aux7, char *_Aux8)
{
  const char * _prepData = PrepData(_Data);
  sscanf(_prepData, " %s %s %s %s %s %s %s %s %s ", _T1, _Aux1, _Aux2, _Aux3, _Aux4, _Aux5, _Aux6, _Aux7, _Aux8);
}

void AutoSegComputation::DeleteSpaces(char *_Label)
{
  int sizeLabel = 0;
  sizeLabel = strlen(_Label);
  for (int i = 0; i < sizeLabel; i++)
  {
    if (_Label[i] == ' ')
    {
      _Label[i] = '_';
    }
  }
}

// Compute Data automatically
void AutoSegComputation::ComputeData()
{
  SetDataFile();
  SetAuxDataFile();

  std::string ORIG_DATA_DIR =GetDataDirectory();
  std::string OrigT1Cases =GetT1();	
  std::string DataFile =GetDataFile();
  std::string AuxDataFile =GetAuxDataFile();
	
  itksys::Glob glob;

  size_t pos;

  std::vector<std::string> OrigT1CasesList;
  std::string OrigT1Case=ORIG_DATA_DIR;
  OrigT1Case.insert(OrigT1Case.size(),OrigT1Cases);
  glob.RecurseOn();
  glob.FindFiles(OrigT1Case);
  OrigT1CasesList = glob.GetFiles();	//List of T1-weighted image
  std::vector<std::string> T2List;
  std::vector<std::string> PDList;
  std::vector<std::string> AuxT1List;
  std::vector<std::string> AuxT2List;
  std::vector<std::string> AuxPDList;
  std::vector<std::string> Aux1List;
  std::vector<std::string> Aux2List;
  std::vector<std::string> Aux3List;
  std::vector<std::string> Aux4List;
  std::vector<std::string> Aux5List;
  std::vector<std::string> Aux6List;
  std::vector<std::string> Aux7List;
  std::vector<std::string> Aux8List;

  std::ofstream file(DataFile.c_str(), std::ios::out );
  if (file)
  {
    file<<"// Automatic Data Selection"<<std::endl;
    file.close();
  }
  else
    std::cerr<<"Can not open file AutoSeg_Data.txt"<<std::endl;


  std::ofstream Auxfile (AuxDataFile.c_str(), std::ios::out);
  if (Auxfile)
  {
    Auxfile<<"// Automatic Auxiliairy Data Selection"<<std::endl;
    Auxfile.close();
  }
  else
    std::cerr<<"Can not open file AutoSeg_AuxData.txt"<<std::endl;
  

  if (!OrigT1CasesList.empty()){
    for (unsigned int i=0; i<OrigT1CasesList.size(); i++){
      if (GetT2Image() || GetPDImage() || GetAux1Image()){
	pos=(OrigT1CasesList.at(i)).find_last_of("/\\");
	if (GetT2Image()){	
	  itksys::Glob glob1;	
	  std::string OrigT2Cases =GetT2();
	  std::string OrigT2Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	  OrigT2Case.insert(OrigT2Case.size(),OrigT2Cases);
	  if (OrigT2Cases!=""){
	    glob1.FindFiles(OrigT2Case);
	    T2List = glob1.GetFiles();	//List of T2-weighted image
	    if (T2List.empty())
	      std::cerr<<"Error: no T2-weighted image found"<<std::endl;
	  }
	  else
	    std::cerr<<"Error: no T2-weighted image found"<<std::endl;
	}
	if (GetPDImage()){
	  itksys::Glob glob2;	
	  std::string OrigPDCases =GetPD();
	  std::string OrigPDCase=(OrigT1CasesList.at(i)).substr(0,pos+1);
	  OrigPDCase.insert(OrigPDCase.size(),OrigPDCases);
	  if (OrigPDCases!=""){
	    glob2.FindFiles(OrigPDCase);
	    PDList = glob2.GetFiles();	//List of PD-weighted image
	    if (PDList.empty())
	      std::cerr<<"Error: no PD-weighted image found"<<std::endl;
	  }
	  else
	    std::cerr<<"Error: no PD-weighted image found"<<std::endl;
	}
	if(GetAux1Image()){
	  if (GetAuxT1Image()){	
	    itksys::Glob globAuxT1; 
	    std::string OrigAuxT1Cases =GetT1();
	    std::string OrigAuxT1Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAuxT1Case.insert(OrigAuxT1Case.size(),OrigAuxT1Cases);
	    if (OrigAuxT1Cases!=""){
	      globAuxT1.FindFiles(OrigAuxT1Case);
	      AuxT1List = globAuxT1.GetFiles();	
	      if (AuxT1List.empty()){
		std::cerr<<"Error: no AuxT1-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no AuxT1-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAuxT2Image()){
	    itksys::Glob globAuxT2;
	    std::string OrigAuxT2Cases =GetT2();
	    std::string OrigAuxT2Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAuxT2Case.insert(OrigAuxT2Case.size(),OrigAuxT2Cases);
	    if (OrigAuxT2Cases!=""){
	      globAuxT2.FindFiles(OrigAuxT2Case);
	      AuxT2List = globAuxT2.GetFiles();	
	      if (AuxT2List.empty()){
		std::cerr<<"Error: no AuxT2-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no AuxT2-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAuxPDImage()){	
	    itksys::Glob globAuxPD;	
	    std::string OrigAuxPDCases =GetPD();
	    std::string OrigAuxPDCase=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAuxPDCase.insert(OrigAuxPDCase.size(),OrigAuxPDCases);
	    if (OrigAuxPDCases!=""){
	      globAuxPD.FindFiles(OrigAuxPDCase);
	      AuxPDList = globAuxPD.GetFiles();	
	      if (AuxPDList.empty()){
		std::cerr<<"Error: no AuxPD-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no AuxPD-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAux1Image()){	
	    itksys::Glob globAux1;	
	    std::string OrigAux1Cases =GetAux1();
	    std::string OrigAux1Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAux1Case.insert(OrigAux1Case.size(),OrigAux1Cases);
	    if (OrigAux1Cases!=""){
	      globAux1.FindFiles(OrigAux1Case);
	      Aux1List = globAux1.GetFiles();
	      if (Aux1List.empty()){
		std::cerr<<"Error: no Aux1-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no Aux1-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAux2Image()){	
	    itksys::Glob globAux2;	
	    std::string OrigAux2Cases =GetAux2();
	    std::string OrigAux2Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAux2Case.insert(OrigAux2Case.size(),OrigAux2Cases);
	    if (OrigAux2Cases!=""){
	      globAux2.FindFiles(OrigAux2Case);
	      Aux2List = globAux2.GetFiles();	
	      if (Aux2List.empty()){
		std::cerr<<"Error: no Aux2-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no Aux2-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAux3Image()){	
	    itksys::Glob globAux3;	
	    std::string OrigAux3Cases =GetAux3();
	    std::string OrigAux3Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAux3Case.insert(OrigAux3Case.size(),OrigAux3Cases);
	    if (OrigAux3Cases!=""){
	      globAux3.FindFiles(OrigAux3Case);
	      Aux3List = globAux3.GetFiles();	
	      if (Aux3List.empty()){
		std::cerr<<"Error: no Aux3-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no Aux3-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAux4Image()){	
	    itksys::Glob globAux4;	
	    std::string OrigAux4Cases =GetAux4();
	    std::string OrigAux4Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAux4Case.insert(OrigAux4Case.size(),OrigAux4Cases);
	    if (OrigAux4Cases!=""){
	      globAux4.FindFiles(OrigAux4Case);
	      Aux4List = globAux4.GetFiles();	
	      if (Aux4List.empty()){
		std::cerr<<"Error: no Aux4-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no Aux4-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAux5Image()){	
	    itksys::Glob globAux5;	
	    std::string OrigAux5Cases =GetAux5();
	    std::string OrigAux5Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAux5Case.insert(OrigAux5Case.size(),OrigAux5Cases);
	    if (OrigAux5Cases!=""){
	      globAux5.FindFiles(OrigAux5Case);
	      Aux5List = globAux5.GetFiles();	
	      if (Aux5List.empty()){
		std::cerr<<"Error: no Aux5-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no Aux5-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAux6Image()){	
	    itksys::Glob globAux6;	
	    std::string OrigAux6Cases =GetAux6();
	    std::string OrigAux6Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAux6Case.insert(OrigAux6Case.size(),OrigAux6Cases);
	    if (OrigAux6Cases!=""){
	      globAux6.FindFiles(OrigAux6Case);
	      Aux6List = globAux6.GetFiles();	
	      if (Aux6List.empty()){
		std::cerr<<"Error: no Aux6-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no Aux6-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAux7Image()){	
	    itksys::Glob globAux7;	
	    std::string OrigAux7Cases =GetAux7();
	    std::string OrigAux7Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAux7Case.insert(OrigAux7Case.size(),OrigAux7Cases);
	    if (OrigAux7Cases!=""){
	      globAux7.FindFiles(OrigAux7Case);
	      Aux7List = globAux7.GetFiles();	
	      if (Aux7List.empty()){
		std::cerr<<"Error: no Aux7-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no Aux7-weighted image found"<<std::endl;
	    }
	  }
	  if (GetAux8Image()){	
	    itksys::Glob globAux8;	
	    std::string OrigAux8Cases =GetAux8();
	    std::string OrigAux8Case=(OrigT1CasesList.at(i)).substr(0,pos+1);
	    OrigAux8Case.insert(OrigAux8Case.size(),OrigAux8Cases);
	    if (OrigAux8Cases!=""){
	      globAux8.FindFiles(OrigAux8Case);
	      Aux8List = globAux8.GetFiles();	
	      if (Aux8List.empty()){
		std::cerr<<"Error: no Aux8-weighted image found"<<std::endl;
	      }
	    }
	    else{
	      std::cerr<<"Error: no Aux8-weighted image found"<<std::endl;
	    }
	  }
	}
      }
      std::ofstream file(DataFile.c_str(), std::ios::app );
      if (file)
      {	
	file<<(OrigT1CasesList.at(i)).c_str();
	if (GetT2Image()&&!T2List.empty()){	
	  file<<" "<<(T2List.at(0)).c_str();
	}
	if (GetPDImage()&&!PDList.empty()){
	  file<<" "<<(PDList.at(0)).c_str();
	}
	if(GetAux1Image()){
	  std::ofstream Auxfile (AuxDataFile.c_str(), std::ios::app );
	  if (Auxfile)
	  {
	    if (GetAuxT1Image() && !AuxT1List.empty()){
	      Auxfile<<(AuxT1List.at(0)).c_str();
	    }
	    if (GetAuxT2Image()&& !AuxT2List.empty()){
	      Auxfile<<(AuxT2List.at(0)).c_str();
	    }
	    if (GetAuxPDImage()&& !AuxPDList.empty()){
	      Auxfile<<(AuxPDList.at(0)).c_str();
	    }
	    if (GetAux1Image()&& !Aux1List.empty()){
	      Auxfile<<" "<<(Aux1List.at(0)).c_str();
	    }
	    if (GetAux2Image()&& !Aux2List.empty()){
	      Auxfile<<" "<<(Aux2List.at(0)).c_str();
	    }
	    if (GetAux3Image()&& !Aux3List.empty()){
	      Auxfile<<" "<<(Aux3List.at(0)).c_str();
	    }
	    if (GetAux4Image()&& !Aux4List.empty()){
	      Auxfile<<" "<<(Aux4List.at(0)).c_str();
	    }
	    if (GetAux5Image()&& !Aux5List.empty()){
	      Auxfile<<" "<<(Aux5List.at(0)).c_str();
	    }
	    if (GetAux6Image()&& !Aux6List.empty()){
	      Auxfile<<" "<<(Aux6List.at(0)).c_str();
	    }
	    if (GetAux7Image()&& !Aux7List.empty()){
	      Auxfile<<" "<<(Aux7List.at(0)).c_str();
	    }
	    if (GetAux8Image()&& !Aux8List.empty()){
	      Auxfile<<" "<<(Aux8List.at(0)).c_str();
	    }
	    Auxfile<<std::endl;
	    Auxfile.close();
	  }
	  else{
	    std::cerr<<"Can not open file AutoSeg_AuxData.txt"<<std::endl;
	  }
	}
	file<<std::endl;
	file.close();

      }
      else{
	std::cerr<<"Can not open file AutoSeg_Data.txt"<<std::endl;
      }
    }
  }
  else{
    std::cerr<<"Error: no T1-weighted image found"<<std::endl;
  }
	
  m_KillProcess = false;
}

void AutoSegComputation::RandomizeSubjects( int _GUIMode )
{
  // randomize tables

  // 1. copy
  

  //if ( !GetT2Image() && !GetPDImage()) 
  // m_T1List[i]
  // m_T2List[i] 
  // m_PDList[i]
  // for (i = 0; i < GetNbData(); i++)
}

void AutoSegComputation::AddNotRequiredTools( std::string toolName )
{
  m_ToolsNotRequired.push_back(toolName);
}

int AutoSegComputation::FindTools()
{
  m_MissingTools.clear() ;
  for( ToolsMapType::iterator it = m_RequiredTools.begin(); it != m_RequiredTools.end() ; ++it )
  {
    std::string name = it->first ;
    std::vector< std::string > toolNames = it->second ;
    std::string toolPath ;
    for( size_t i = 0 ; i < toolNames.size() ; i++ )
    {
      toolPath = itksys::SystemTools::FindProgram( toolNames[ i ] );
      if( !toolPath.empty() )
      {
        ToolsPairPathsType toolPathPair( name , toolPath ) ;
        m_RequiredToolsPaths.insert( toolPathPair ) ;
        break ;
      }
    }
    if( toolPath.empty() )
    {
      if( find(m_ToolsNotRequired.begin(),m_ToolsNotRequired.end(),name) != m_ToolsNotRequired.end() )//If the tool is not required, we set its path to "" so that the software does not crash when writing the bms script.
      {
        toolPath = name ;
        ToolsPairPathsType toolPathPair( name , toolPath ) ;
        m_RequiredToolsPaths.insert( toolPathPair ) ;
      }
      else
      {
        m_MissingTools.push_back( name ) ;
      }
    }
  }
  return m_MissingTools.size() ;
}

std::vector< std::string > AutoSegComputation::GetMissingTools()
{
  return m_MissingTools ;
}

// Compute Automatic Segmentation
int AutoSegComputation::Computation()
{
  SetParameterFile();
  SetComputationFile();
  if( !FindTools() )
  {
    RunPipeline(1);
    return 0 ;
  }
  else
  {
    m_IsAutoSegInProcess = 0 ;
    return 1 ;
  }
}

// Compute Automatic Segmentation
void AutoSegComputation::ComputationWithoutGUI(const char *_computationFile, const char *_parameterFile)
{

  LoadAuxComputationFile(_computationFile);
  LoadComputationFile(_computationFile);
  LoadParameterFile(_parameterFile);
  SetComputationFile();
  SetParameterFile();
  if( !FindTools() )
  {
    try
    {
      RunPipeline(0);
    }
    catch(const std::exception& ex)
    {
      std::cerr << ex.what() << std::endl;
    }
    catch(...)
    {
      std::cerr << "Error during computation." << std::endl ;
    }
  }
  else
  {
    std::cerr << "Some tools that are required were not found!" << std::endl ;
    for( size_t i = 0 ; i < m_MissingTools.size() ; i++ )
    {
      std::vector< std::string > toolNames = m_RequiredTools.at( m_MissingTools[ i ] ) ;
      for( size_t j = 0 ; j < toolNames.size() ; j++ )
      {
        if( j != 0 )
        {
          std::cerr << " or " ;
        }
        std::cerr << toolNames[ j ] ;
      }
      std::cerr << std::endl ;
    }
  }
}

void AutoSegComputation::RunPipeline( int _GUIMode )
{
  SetBMSAutoSegFile();
  SetBMSAutoSegMainFile();
  if (GetAux1Image())
  {
    SetBMSAutoSegAuxFile();
  }
  SetLogFile();

  if (GetRandomizeSubjects()) 
  {
    RandomizeSubjects(_GUIMode);
  }

  WriteParameterFile(GetParameterFile());
  WriteComputationFile(GetComputationFile());
  WriteBMSAutoSegFile();
  WriteBMSAutoSegMainFile();
  if (GetAux1Image())
  {
    WriteBMSAutoSegAuxFile();
  }
  m_KillProcess = false;
  m_output.clear();
  std::cout << GetBMSAutoSegFile() << std::endl;
  ExecuteBatchMake(GetBMSAutoSegFile(),_GUIMode);
  std::cout << "finished!!!" << std::endl;
}


void AutoSegComputation::ShowDisplay()
{
  TextDisplay.g_MainWindow->show();  
  Fl::check();
}


ITK_THREAD_RETURN_TYPE BatchMakeThreader( void * arg ) 
{
  typedef itk::MultiThreader::ThreadInfoStruct  ThreadInfoType;
  
  ThreadInfoType * infoStruct = static_cast< ThreadInfoType * >( arg );

  AutoSegComputation  * comp  = (AutoSegComputation *) infoStruct->UserData;

  comp->SetIsAutoSegInProcess(true);
 
  std::cout << "starting BatchMake execution as Thread: " << comp->GetCurrentBatchmakeFile() << std::endl;

  comp->GetParser()->Execute(comp->GetCurrentBatchmakeFile());

  std::cout << "finished with BatchMake execution: " << comp->GetCurrentBatchmakeFile() << std::endl;

  comp->SetIsAutoSegInProcess(false); 

  ITK_THREAD_RETURN_TYPE value = 0;
  return value;

}

// Execute Batchmake script
//_GUIMode == 1 with GUI
//_GUIMode == 0 without GUI
void AutoSegComputation::ExecuteBatchMake(char *_Input, int _GUIMode)
{
  // Execute BatchMake
  
  m_GUImode = (bool) _GUIMode;
  if (GetGUIMode())
  {                
    TextDisplay.g_TextDisp->buffer(m_TextBuf); 
    TextDisplay.g_MainWindow->show(); 
  }  

  typedef itk::MultiThreader  ThreaderType;

  ThreaderType::Pointer threader = ThreaderType::New();

  std::string logFilename = std::string(GetLogFile());
  std::string logErrorFilename = logFilename + ".err";

  std::cout << "starting to listen and logging to : " << logFilename << std::endl;
  FILE *fp1 = freopen(logFilename.c_str(),"w",stdout);
  FILE *fp2 = freopen(logErrorFilename.c_str(),"w",stderr);

  m_GUImode = (bool) _GUIMode;
  m_currentBMS = _Input;
  if (GetGUIMode()) {
    m_batchMakeThreadID = threader->SpawnThread( BatchMakeThreader, this );
  } else {
    typedef itk::MultiThreader::ThreadInfoStruct  ThreadInfoType;
    ThreadInfoType infoStruct;
    infoStruct.UserData = this;
    BatchMakeThreader(&infoStruct);
  }

  int curFileLength = 0;

  while (GetIsAutoSegInProcess() && !m_KillProcess) {
    if (GetGUIMode())
      {
	Fl::check();
      }
    
    itksys::SystemTools::Delay(100);

    if (GetGUIMode())
      {
	Fl::check();
	int length = itksys::SystemTools::FileLength(logFilename.c_str());
	if (length != curFileLength) {
	  curFileLength = length;
	  ifstream logInput (GetLogFile());
	  
	  if (logInput) {
	    std::string log((std::istreambuf_iterator<char>(logInput)), std::istreambuf_iterator<char>());
	    
	    m_output = log;
	    
	    m_TextBuf.text(m_output.c_str());
	    TextDisplay.g_TextDisp->scroll(100000,0);
	  }
	  logInput.close();
	}
      }

    if(m_KillProcess)
      {
	std::cout << "Killing BatchMake process " << std::endl;
	m_IsAutoSegInProcess = false;

#ifdef _POSIX_SOURCE
	//set progress manager to stop
	m_Parser.GetScriptActionManager()->GetProgressManager()->Stop();

	// kill all processes with this process as parent       
	pid_t processID = getpid();
	char processIDstring[100];
	sprintf(processIDstring,"%d",processID);
	std::vector<const char*> args;
	args.push_back("pkill");
	args.push_back("-P");
	args.push_back(processIDstring);
	// don't forget this line at the end of the argument list
	args.push_back(0);
	
	m_Process = itksysProcess_New();
	itksysProcess_SetCommand(m_Process, &*args.begin());
	itksysProcess_SetOption(m_Process,itksysProcess_Option_HideWindow,1);
	itksysProcess_Execute(m_Process);
	//itksysProcess_WaitForExit(m_Process, 0);

	threader->TerminateThread(m_batchMakeThreadID);
	SetIsAutoSegInProcess(false); 
#endif
	std::cout << "Killing BatchMake process done" << std::endl;
	break;
      }

  }

  if (GetGUIMode())
    {
      Fl::check();
      int length = itksys::SystemTools::FileLength(logFilename.c_str());
      if (length != curFileLength) {
	curFileLength = length;
	ifstream logInput (GetLogFile());
	
	if (logInput) {
	  std::string log((std::istreambuf_iterator<char>(logInput)), std::istreambuf_iterator<char>());
	  
	  m_output = log;
	  
	  m_TextBuf.text(m_output.c_str());
	  TextDisplay.g_TextDisp->scroll(100000,0);
	}
	logInput.close();
      }
    }

  fclose(fp1);
  fclose(fp2);

  freopen("/dev/null","w",stdout);
  freopen("/dev/null","w",stderr);

}

void AutoSegComputation::StopBatchMake()
{
  m_KillProcess = true;
}

//Write Computation File
void AutoSegComputation::WriteComputationFile(const char *_FileName)
{
  std::ofstream ComputationFile(_FileName); 
  int i;
  ComputationFile<<"//     Automatic Segmentation Computation File"<<std::endl<<std::endl;
  ComputationFile<<"Process Data Directory: "<<GetProcessDataDirectory()<<std::endl<<std::endl;
  ComputationFile<<"// Data"<<std::endl;
  ComputationFile<<"Is T1 Image: 1"<<std::endl;
  ComputationFile<<"Is T2 Image: "<<GetT2Image()<<std::endl;
  ComputationFile<<"Is PD Image: "<<GetPDImage()<<std::endl;  
  ComputationFile<<"// Data AutoSeg Directory"<<std::endl;
  ComputationFile<<"Data AutoSeg Directory: "<<GetDataAutoSegDirectory()<<std::endl<<std::endl;
  ComputationFile<<"// Automatic Data Selection"<<std::endl;
  ComputationFile<<"Data Directory: "<<GetDataDirectory()<<std::endl;
  if (GetAux1Image())
  {
    ComputationFile<<"// AuxData"<<std::endl;
    ComputationFile<<"Is AuxT1 Image: "<<GetAuxT1Image()<<std::endl;
    ComputationFile<<"Is AuxT2 Image: "<<GetAuxT2Image()<<std::endl;
    ComputationFile<<"Is AuxPD Image: "<<GetAuxPDImage()<<std::endl;
    ComputationFile<<"Is Aux1 Image: "<<GetAux1Image()<<std::endl; 	
    ComputationFile<<"Is Aux2 Image: "<<GetAux2Image()<<std::endl;
    ComputationFile<<"Is Aux3 Image: "<<GetAux3Image()<<std::endl;
    ComputationFile<<"Is Aux4 Image: "<<GetAux4Image()<<std::endl;
    ComputationFile<<"Is Aux5 Image: "<<GetAux5Image()<<std::endl;
    ComputationFile<<"Is Aux6 Image: "<<GetAux6Image()<<std::endl;
    ComputationFile<<"Is Aux7 Image: "<<GetAux7Image()<<std::endl;
    ComputationFile<<"Is Aux8 Image: "<<GetAux8Image()<<std::endl<<std::endl;
  } else {
    ComputationFile<<"// AuxData"<<std::endl;
    ComputationFile<<"Is AuxT1 Image: 0"<<std::endl;
    ComputationFile<<"Is AuxT2 Image: 0"<<std::endl;
    ComputationFile<<"Is AuxPD Image: 0"<<std::endl;
    ComputationFile<<"Is Aux1 Image: 0"<<std::endl; 	
    ComputationFile<<"Is Aux2 Image: 0"<<std::endl;
    ComputationFile<<"Is Aux3 Image: 0"<<std::endl;
    ComputationFile<<"Is Aux4 Image: 0"<<std::endl;
    ComputationFile<<"Is Aux5 Image: 0"<<std::endl;
    ComputationFile<<"Is Aux6 Image: 0"<<std::endl;
    ComputationFile<<"Is Aux7 Image: 0"<<std::endl;
    ComputationFile<<"Is Aux8 Image: 0"<<std::endl<<std::endl;
  }
  ComputationFile<<"T1 Files: "<<GetT1()<<std::endl;   
  ComputationFile<<"T2 Files: "<<GetT2()<<std::endl;
  ComputationFile<<"PD Files: "<<GetPD()<<std::endl<<std::endl;
  if (GetAux1Image())
  {
    if (GetAuxT1Image())
      ComputationFile<<"AuxT1 Files: "<<GetT1()<<std::endl;
    else
      ComputationFile<<"AuxT1 Files: "<<std::endl;
    if (GetAuxT2Image())
      ComputationFile<<"AuxT2 Files: "<<GetT2()<<std::endl;
    else
      ComputationFile<<"AuxT2 Files: "<<std::endl;
    if (GetAuxPDImage())
      ComputationFile<<"AuxPD Files: "<<GetPD()<<std::endl;
    else
      ComputationFile<<"AuxPD Files: "<<std::endl;
    ComputationFile<<"Aux1 Files: "<<GetAux1()<<std::endl;
    ComputationFile<<"Aux2 Files: "<<GetAux2()<<std::endl;
    ComputationFile<<"Aux3 Files: "<<GetAux3()<<std::endl;
    ComputationFile<<"Aux4 Files: "<<GetAux4()<<std::endl;
    ComputationFile<<"Aux5 Files: "<<GetAux5()<<std::endl;
    ComputationFile<<"Aux6 Files: "<<GetAux6()<<std::endl;
    ComputationFile<<"Aux7 Files: "<<GetAux7()<<std::endl;
    ComputationFile<<"Aux8 Files: "<<GetAux8()<<std::endl<<std::endl;
    ComputationFile<<"// Auxiliairy Images Labels"<<std::endl;  
    DeleteSpaces(m_Aux1Label);
    ComputationFile<<"Aux1 Label: "<<GetAux1Label()<<std::endl;
    DeleteSpaces(m_Aux2Label);
    ComputationFile<<"Aux2 Label: "<<GetAux2Label()<<std::endl;
    DeleteSpaces(m_Aux3Label);
    ComputationFile<<"Aux3 Label: "<<GetAux3Label()<<std::endl;
    DeleteSpaces(m_Aux4Label);
    ComputationFile<<"Aux4 Label: "<<GetAux4Label()<<std::endl;
    DeleteSpaces(m_Aux5Label);
    ComputationFile<<"Aux5 Label: "<<GetAux5Label()<<std::endl;
    DeleteSpaces(m_Aux6Label);
    ComputationFile<<"Aux6 Label: "<<GetAux6Label()<<std::endl;
    DeleteSpaces(m_Aux7Label);
    ComputationFile<<"Aux7 Label: "<<GetAux7Label()<<std::endl;
    DeleteSpaces(m_Aux8Label);
    ComputationFile<<"Aux8 Label: "<<GetAux8Label()<<std::endl<<std::endl;
  }
  ComputationFile<<"// Computation Options"<<std::endl;
  ComputationFile<<"Compute Volume: "<<GetComputeVolume()<<std::endl;
  ComputationFile<<"Compute cortical thickness: "<<GetComputeCorticalThickness()<<std::endl;
  ComputationFile<<"Recompute: "<<GetRecompute()<<std::endl;
  ComputationFile<<"Use Condor: "<<GetUseCondor()<<std::endl<<std::endl;
  if (GetAux1Image())
  {
    ComputationFile<<"// Regional Histogram Options"<<std::endl;
    ComputationFile<<"-Source Selection-"<<std::endl;
    ComputationFile<<"Atlas Space Image: "<<GetAtlasSpaceImage()<<std::endl;
    ComputationFile<<"Bias Corrected Image: "<<GetBiasCorrectedImage()<<std::endl;
    ComputationFile<<"Skull Stripped Image: "<<GetSkullStrippedImage()<<std::endl;
    ComputationFile<<"-Transformation Selection-"<<std::endl;
    ComputationFile<<"Rigid Transformation: "<<GetRigidTransformation()<<std::endl;
    ComputationFile<<"Affine Transformation: "<<GetAffineTransformation()<<std::endl;
    ComputationFile<<"Bspline Transformation: "<<GetBsplineTransformation()<<std::endl<<std::endl;
  }
  if ( !GetT2Image() && !GetPDImage()) 
  {
    for (i = 0; i < GetNbData(); i++)
      ComputationFile<<"Data: "<<m_T1List[i]<<std::endl<<std::endl;;
  }
  else if (GetT2Image() && GetPDImage()) 
  {
    for (i = 0; i < GetNbData(); i++)
      ComputationFile<<"Data: "<<m_T1List[i]<<" "<<m_T2List[i]<<" "<<m_PDList[i]<<std::endl<<std::endl;;  
  }
  else if (GetT2Image())
  {
    for (i = 0; i < GetNbData(); i++)
      ComputationFile<<"Data: "<<m_T1List[i]<<" "<<m_T2List[i]<<std::endl<<std::endl;;  
  }
  else 
  {
    for (i = 0; i < GetNbData(); i++)
      ComputationFile<<"Data: "<<m_T1List[i]<<" "<<m_PDList[i]<<std::endl<<std::endl;;  
  }

  ComputationFile<<"// Randomize the subject order prior to processing?"<<std::endl;
  ComputationFile<<"Randomize Subject Order: "<<GetRandomizeSubjects()<<std::endl;

  ComputationFile<<"// Multi-Modality Segmentation Options"<<std::endl;
  ComputationFile<<"// Multi vs Single-Atlas Segmentation Options"<<std::endl;
  ComputationFile<<"Compute Multi-modality Single-atlas Segmentation: "<<GetMultiModalitySingleSegmentation()<<std::endl;
  ComputationFile<<"Compute Multi-modality Multi-atlas Segmentation: "<<GetMultiModalityMultiSegmentation()<<std::endl;

  ComputationFile<<"Compute Single-atlas Segmentation: "<<GetSingleAtlasSegmentation()<<std::endl;
  ComputationFile<<"Compute Multi-atlas Segmentation: "<<GetMultiAtlasSegmentation()<<std::endl;
  ComputationFile<<"Conduct Atlas-Atlas Registration: "<<GetMultiAtlasAtlasRegistration()<<std::endl;
  ComputationFile<<"Recalculate Atlas-Target Energy: "<<GetRecalculateAtlasTargetMultiAtlasEnergy()<<std::endl;
  ComputationFile<<"Recalculate Atlas-Atlas Energy: "<<GetRecalculateAtlasAtlasMultiAtlasEnergy()<<std::endl;

  if (GetAux1Image())
  {
    std::cout << "aux data: " << GetNbAuxData() << std::endl;

    if ( !GetAux1Image() && !GetAux2Image() && !GetAux3Image() && !GetAux4Image() && !GetAux5Image() && !GetAux6Image() && !GetAux7Image() && !GetAux8Image())
    {
      if(!m_Manually)
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T1List[i]<<std::endl; 
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T2List[i]<<std::endl; 
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_PDList[i]<<std::endl; 
	}
      }
      else
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT1List[i]<<std::endl; 
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT2List[i]<<std::endl; 
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxPDList[i]<<std::endl; 
	}
      }
    }
    else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image() && GetAux5Image() && GetAux6Image() && GetAux7Image() && GetAux8Image()) 
    {
      if(!m_Manually)
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<" "<<m_Aux8List[i]<<std::endl;  
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<" "<<m_Aux8List[i]<<std::endl;  
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_PDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<" "<<m_Aux8List[i]<<std::endl;  
	}
      }
      else
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<" "<<m_Aux8List[i]<<std::endl;  
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<" "<<m_Aux8List[i]<<std::endl;  
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxPDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<" "<<m_Aux8List[i]<<std::endl;  
	}
      }
    }
    else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image() && GetAux5Image() && GetAux6Image() && GetAux7Image()) 
    {
      if(!m_Manually)
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<std::endl;  
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<std::endl;  
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_PDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<std::endl;  
	}
      }
      else
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<std::endl;  
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<std::endl;  
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxPDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<" "<<m_Aux7List[i]<<std::endl;  
	}
      }
    }
    else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image() && GetAux5Image() && GetAux6Image()) 
    {
      if(!m_Manually)
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<std::endl;  
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<std::endl;  
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_PDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<std::endl;  
	}
      }
      else
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<std::endl;  
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<std::endl;  
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxPDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<" "<<m_Aux6List[i]<<std::endl;  
	}
      }
    }
    else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image() && GetAux5Image()) 
    {
      if(!m_Manually)
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<std::endl; 
	} 
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<std::endl; 
	} 
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_PDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<std::endl; 
	}
      }
      else
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<std::endl; 
	} 
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<std::endl; 
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxPDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<" "<<m_Aux5List[i]<<std::endl; 
	}
      }
    }
    else if (GetAux1Image() && GetAux2Image() && GetAux3Image() && GetAux4Image()) 
    {
      if(!m_Manually)
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<std::endl;  
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<std::endl;  
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_PDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<std::endl;  
	}
      }
      else
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<std::endl;  
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<std::endl;  
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxPDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<" "<<m_Aux4List[i]<<std::endl;  
	}
      }
    }
    else if (GetAux1Image() && GetAux2Image() && GetAux3Image()) 
    {
      if(!m_Manually)
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<std::endl;
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<std::endl;
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_PDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<std::endl;
	}
      }
      else
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<std::endl;
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<std::endl;
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxPDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<" "<<m_Aux3List[i]<<std::endl;
	}
      }
    }
    else if (GetAux1Image() && GetAux2Image()) 
    {
      if(!m_Manually)
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<std::endl;
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<std::endl;
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_PDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<std::endl;
	}
      }
      else
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT1List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<std::endl;
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT2List[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<std::endl;
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxPDList[i]<<" "<<m_Aux1List[i]<<" "<<m_Aux2List[i]<<std::endl;
	}
      }
    }
    else
    {
      if(!m_Manually)
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T1List[i]<<" "<<m_Aux1List[i]<<std::endl;
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_T2List[i]<<" "<<m_Aux1List[i]<<std::endl;
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_PDList[i]<<" "<<m_Aux1List[i]<<std::endl;
	}
      }
      else
      {
	if (GetAuxT1Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT1List[i]<<" "<<m_Aux1List[i]<<std::endl;
	}
	else if (GetAuxT2Image())
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxT2List[i]<<" "<<m_Aux1List[i]<<std::endl;
	}
	else
	{
	  for (i = 0; i < GetNbAuxData(); i++)
	    ComputationFile<<"AuxData: "<<m_AuxPDList[i]<<" "<<m_Aux1List[i]<<std::endl;
	}
      }
    }
  }

  ComputationFile.close();
}

// Write Parameter file
void AutoSegComputation::WriteParameterFile(const char *_FileName)
{
  std::ofstream ParameterFile(_FileName); 
  ParameterFile<<"//     Automatic Segmentation Parameter File"<<std::endl<<std::endl;
  ParameterFile<<"// Atlases"<<std::endl;
  ParameterFile<<"Common Coordinate Image: "<<GetCommonCoordinateImage()<<std::endl;
  ParameterFile<<"Common Coordinate Image Type: "<<GetCommonCoordinateImageType()<<std::endl;
  ParameterFile<<"Tissue Segmentation Atlas Directory: "<<GetTissueSegmentationAtlasDirectory()<<std::endl;
  ParameterFile<<"Tissue Segmentation Atlas Type: "<<GetTissueSegmentationAtlasType()<<std::endl;
  ParameterFile<<"\n// Loop"<<std::endl;
  ParameterFile<<"Loop: "<<GetLoop()<<std::endl;
  ParameterFile<<"Atlas Loop: "<<GetAtlasLoop()<<std::endl;
  ParameterFile<<"Loop - Number of iterations: "<<GetLoopIteration()<<std::endl;
  ParameterFile<<"ROI Atlas File: "<<GetROIAtlasFile()<<std::endl<<std::endl;
  ParameterFile<<"ROI Second Atlas File: "<<GetROIT2AtlasFile()<<std::endl<<std::endl;

  ParameterFile<<"// Subcortical Structures"<<std::endl;  
  ParameterFile<<"Subcortical Structure Segmentation: "<<GetSubcorticalStructureSegmentation()<<std::endl;
  ParameterFile<<"Amygdala Left: "<<GetAmygdalaLeft()<<std::endl;
  ParameterFile<<"Amygdala Right: "<<GetAmygdalaRight()<<std::endl;
  ParameterFile<<"Caudate Left: "<<GetCaudateLeft()<<std::endl;
  ParameterFile<<"Caudate Right: "<<GetCaudateRight()<<std::endl;
  ParameterFile<<"Hippocampus Left: "<<GetHippocampusLeft()<<std::endl;
  ParameterFile<<"Hippocampus Right: "<<GetHippocampusRight()<<std::endl;
  ParameterFile<<"Pallidus Left: "<<GetPallidusLeft()<<std::endl;
  ParameterFile<<"Pallidus Right: "<<GetPallidusRight()<<std::endl;
  ParameterFile<<"Putamen Left: "<<GetPutamenLeft()<<std::endl;
  ParameterFile<<"Putamen Right: "<<GetPutamenRight()<<std::endl;
  ParameterFile<<"Lateral Ventricle Left: "<<GetLateralVentricleLeft()<<std::endl;
  ParameterFile<<"Lateral Ventricle Right: "<<GetLateralVentricleRight()<<std::endl<<std::endl;  
  ParameterFile<<"// Generic ROI Maps"<<std::endl; 
  ParameterFile<<"Generic ROI Segmentation: "<<GetGenericROISegmentation()<<std::endl;
  ParameterFile<<"ROI File 1: "<<GetROIFile1()<<std::endl;
  ParameterFile<<"ROI File 2: "<<GetROIFile2()<<std::endl;
  ParameterFile<<"ROI File 3: "<<GetROIFile3()<<std::endl;
  ParameterFile<<"ROI File 4: "<<GetROIFile4()<<std::endl;
  ParameterFile<<"ROI File 5: "<<GetROIFile5()<<std::endl<<std::endl;
  ParameterFile<<"// Parcellation Maps"<<std::endl;  
  ParameterFile<<"Parcellation Map Segmentation: "<<GetParcellationMapSegmentation()<<std::endl;
  ParameterFile<<"Tissue Map: "<<GetSoftTissueMap()<<std::endl;
  ParameterFile<<"Parcellation File 1: "<<GetParcellationFile1()<<std::endl;
  ParameterFile<<"Parcellation File 2: "<<GetParcellationFile2()<<std::endl;
  ParameterFile<<"Parcellation File 3: "<<GetParcellationFile3()<<std::endl<<std::endl;
  ParameterFile<<"// Tissue Segmentation"<<std::endl;
  ParameterFile<<"EM Software: "<<GetEMSoftware()<<std::endl;
  ParameterFile<<"Filter Iterations: "<<GetFilterIterations()<<std::endl;
  ParameterFile<<"Filter TimeStep: "<<GetFilterTimeStep()<<std::endl;
  ParameterFile<<"Filter Method: "<<GetFilterMethod()<<std::endl;
  ParameterFile<<"Max Bias Degree: "<<GetMaxBiasDegree()<<std::endl;
  ParameterFile<<"Initial Distribution Estimator: "<<GetInitialDistributionEstimator()<<std::endl;
  ParameterFile<<"Prior 1: "<<GetPrior1()<<std::endl;
  ParameterFile<<"Prior 2: "<<GetPrior2()<<std::endl;
  ParameterFile<<"Prior 3: "<<GetPrior3()<<std::endl;
  ParameterFile<<"Prior 4: "<<GetPrior4()<<std::endl;
  ParameterFile<<"Prior 5: "<<GetPrior5()<<std::endl;
  ParameterFile<<"Prior 6: "<<GetPrior6()<<std::endl;
  ParameterFile<<"Prior 7: "<<GetPrior7()<<std::endl;
  ParameterFile<<"Prior 8: "<<GetPrior8()<<std::endl;
  ParameterFile<<"Prior 9: "<<GetPrior9()<<std::endl;
std::cout<<"EMS:" << GetEMSoftware()<<std::endl;
  if (std::strcmp(GetEMSoftware(), "ABC") == 0)
  {
    ParameterFile<<"Fluid Atlas Warp: "<<GetFluidAtlasWarp()<<std::endl;
    ParameterFile<<"Fluid Atlas Affine: "<<GetFluidAtlasAffine()<<std::endl;
    ParameterFile<<"Fluid Atlas FATW: "<<GetFluidAtlasFATW()<<std::endl;
    ParameterFile<<"ANTS Warp for ABC: "<<GetANTSAtlasABC()<<std::endl;
    ParameterFile<<"Fluid Atlas Warp Iterations: "<<GetFluidAtlasWarpIterations()<<std::endl;
    ParameterFile<<"Fluid Atlas Warp Max Step: "<<GetFluidAtlasWarpMaxStep()<<std::endl;
    ParameterFile<<"ANTS with brainmask: "<<GetANTSWithBrainmask()<<std::endl;
    ParameterFile<<"Use Initital Affine Transform: "<<GetUseInitialAffine()<<std::endl;
  }
  else if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
    {
      ParameterFile<<"BSpline Atlas Warp: "<<GetBSplineAtlasWarp()<<std::endl;
      ParameterFile<<"BSpline Atlas Warp Grid X: "<<GetBSplineAtlasWarpGridX()<<std::endl;
      ParameterFile<<"BSpline Atlas Warp Grid Y: "<<GetBSplineAtlasWarpGridY()<<std::endl;
      ParameterFile<<"BSpline Atlas Warp Grid Z: "<<GetBSplineAtlasWarpGridZ()<<std::endl;
    }

  ParameterFile<<"Atlas Linear Mapping: "<<GetAtlasLinearMapping()<<std::endl;
  ParameterFile<<"Image Linear Mapping: "<<GetImageLinearMapping()<<std::endl;
  if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
    {
      ParameterFile<<"Prior Threshold: "<<GetNeosegPriorThreshold()<<std::endl;
      ParameterFile<<"Parzen Kernel: "<<GetNeosegParzenKernel()<<std::endl;
      ParameterFile<<"Mahalanobis Threshold: "<<GetNeosegMahalanobisThreshold()<<std::endl;
    }
  ParameterFile<<"\n// Grid Template"<<std::endl;
  ParameterFile<<"Rigid Registration: "<<GetRigidRegistration()<<std::endl;
  ParameterFile<<"Is ROIAtlasGridTemplate: "<<GetROIAtlasGridTemplate()<<std::endl;
  ParameterFile<<"GridTemplate SizeX: "<<GetGridTemplateSizeX()<<std::endl;
  ParameterFile<<"GridTemplate SizeY: "<<GetGridTemplateSizeY()<<std::endl;
  ParameterFile<<"GridTemplate SizeZ: "<<GetGridTemplateSizeZ()<<std::endl;
  ParameterFile<<"GridTemplate SpacingX: "<<GetGridTemplateSpacingX()<<std::endl;
  ParameterFile<<"GridTemplate SpacingY: "<<GetGridTemplateSpacingY()<<std::endl;
  ParameterFile<<"GridTemplate SpacingZ: "<<GetGridTemplateSpacingZ()<<std::endl; 
  ParameterFile<<"Registration Initialization: "<<GetRegistrationInitialization()<<std::endl;
  ParameterFile<<"Use T1 initial transform: "<<GetInitRegUseT1InitTransform()<<std::endl;
  ParameterFile<<"\n// Atlas Warping"<<std::endl;
  if (GetClassicWarpingMethod())
    ParameterFile<<"Warping Method: Classic"<<std::endl;
  else if (GetBRAINSDemonWarpMethod())
    ParameterFile<<"Warping Method: BRAINSDemonWarp"<<std::endl;
  else if (GetCoarseToFineWarpingMethod())
    ParameterFile<<"Warping Method: Coarse-to-fine"<<std::endl;
  else if (GetANTSWarpingMethod())
    ParameterFile<<"Warping Method: ANTS"<<std::endl;
    
  ParameterFile<<"// - Fluid Warping parameters"<<std::endl;
  ParameterFile<<"Alpha: "<<GetAlpha()<<std::endl;
  ParameterFile<<"Beta: "<<GetBeta()<<std::endl;
  ParameterFile<<"Gamma: "<<GetGamma()<<std::endl;
  ParameterFile<<"Max Perturbation: "<<GetMaxPerturbation()<<std::endl;
  ParameterFile<<"NumBasis: "<<GetNumBasis()<<std::endl;
  ParameterFile<<"Scale 4 - Number Of Iterations: "<<GetScale4NbIterations()<<std::endl;
  ParameterFile<<"Scale 2 - Number Of Iterations: "<<GetScale2NbIterations()<<std::endl;
  ParameterFile<<"Scale 1 - Number Of Iterations: "<<GetScale1NbIterations()<<std::endl;
  ParameterFile<<"// - BRAINSDemonWarp parameters"<<std::endl;
  ParameterFile<<"Registration Filter Type: "<<GetRegistrationFilterType()<<std::endl;
  ParameterFile<<"Deformation Field Smoothing Sigma: "<<GetDeformationFieldSmoothingSigma()<<std::endl;
  ParameterFile<<"Pyramid Levels: "<<GetPyramidLevels()<<std::endl;
  ParameterFile<<"Moving Shrink Factors: "<<GetMovingShrinkFactors()<<std::endl;
  ParameterFile<<"Fixed Shrink Factors: "<<GetFixedShrinkFactors()<<std::endl;
  ParameterFile<<"Iteration Count Pyramid Levels: "<<GetIterationCountPyramidLevels()<<std::endl;
  ParameterFile<<"// - ANTS parameters"<<std::endl;
  ParameterFile<<"ANTS Iterations: "<<GetANTSIterations()<<std::endl;
  ParameterFile<<"ANTS CC weight: "<<GetANTSCCWeight()<<std::endl;
  ParameterFile<<"ANTS CC region radius: "<<GetANTSCCRegionRadius()<<std::endl;
  ParameterFile<<"ANTS MI weight: "<<GetANTSMIWeight()<<std::endl;
  ParameterFile<<"ANTS MI bins: "<<GetANTSMIBins()<<std::endl;
  ParameterFile<<"ANTS MSQ weight: "<<GetANTSMSQWeight()<<std::endl;
  ParameterFile<<"ANTS CC weight for 2nd modality: "<<GetANTSCCWeight2nd()<<std::endl;
  ParameterFile<<"ANTS CC region radius for 2nd modality: "<<GetANTSCCRegionRadius2nd()<<std::endl;
  ParameterFile<<"ANTS MI weight for 2nd modality: "<<GetANTSMIWeight2nd()<<std::endl;
  ParameterFile<<"ANTS MI bins for 2nd modality: "<<GetANTSMIBins2nd()<<std::endl;
  ParameterFile<<"ANTS MSQ weight for 2nd modality: "<<GetANTSMSQWeight2nd()<<std::endl;
  ParameterFile<<"ANTS Registration Type: "<<GetANTSRegistrationFilterType()<<std::endl;
  ParameterFile<<"ANTS Registration Step: "<<GetANTSTransformationStep()<<std::endl;
  ParameterFile<<"ANTS Gaussian Smoothing: "<<GetANTSGaussianSmoothing()<<std::endl;
  ParameterFile<<"ANTS Gaussian Sigma: "<<GetANTSGaussianSigma()<<std::endl;  
  ParameterFile<<"ANTS Number of Threads: "<<GetNbANTSThreads()<<std::endl;  
  ParameterFile<<"\n// Skull Stripping"<<std::endl;
  ParameterFile<<"Delete Vessels: "<<GetDeleteVessels()<<std::endl;
  ParameterFile<<"\n// Intensity Rescaling"<<std::endl;
  if (GetIntensityRescalingMethod() == 1)
    ParameterFile<<"Intensity Rescaling: Histogram quantile"<<std::endl;
  else
    ParameterFile<<"Intensity Rescaling: Tissue mean match"<<std::endl;

  ParameterFile<<"\n// Regional histogram"<<std::endl;
  ParameterFile<<"Quantiles: "<<GetQuantiles()<<std::endl; 
  ParameterFile<<"Point Spacing: "<<GetPointSpacing()<<std::endl; 

  ParameterFile<<"\n// N4 ITK Bias Field Correction"<<std::endl;
  ParameterFile<<"N4 ITK Bias Field Correction: "<<GetN4ITKBiasFieldCorrection()<<std::endl;
  ParameterFile<<"N4 Number of iterations: "<<GetNbOfIterations()<<std::endl;
  ParameterFile<<"N4 Spline distance: "<<GetSplineDistance()<<std::endl;
  ParameterFile<<"N4 Shrink factor: "<<GetShrinkFactor()<<std::endl;
  ParameterFile<<"N4 Convergence threshold: "<<GetConvergenceThreshold()<<std::endl;
  ParameterFile<<"N4 BSpline grid resolutions: "<<GetBSplineGridResolutions()<<std::endl;
  ParameterFile<<"N4 BSpline alpha: "<<GetBSplineAlpha()<<std::endl;
  ParameterFile<<"N4 BSpline beta: "<<GetBSplineBeta()<<std::endl;
  ParameterFile<<"N4 Histogram sharpening: "<<GetHistogramSharpening()<<std::endl;
  ParameterFile<<"N4 BSpline order: "<<GetBSplineOrder()<<std::endl<<std::endl;
  ParameterFile<<"Bias Correction stripped image: "<<GetStrippedN4ITKBiasFieldCorrection()<<std::endl;
  ParameterFile<<"The Version of Slicer Used: "<<GetSlicerVersion()<<std::endl;

  ParameterFile<<"\n// Reorientation"<<std::endl;
  ParameterFile<<"Reorientation: "<<GetReorientation()<<std::endl;
  ParameterFile<<"Input Orientation: "<<GetInputDataOrientation()<<std::endl;
  ParameterFile<<"Output Orientation: "<<GetOutputDataOrientation()<<std::endl<<std::endl;

  ParameterFile<<"// Multi-Atlas Segmentation"<<std::endl;
  ParameterFile<<"Label Fusion Algorithm: "<<GetLabelFusionAlgorithm()<<std::endl;
  ParameterFile<<"Intensity Energy Weight: "<<GetIntensityEnergyWeight()<<std::endl;
  ParameterFile<<"Harmonic Energy Weight: "<<GetHarmonicEnergyWeight()<<std::endl;
  ParameterFile<<"Shape Energy Weight: "<<GetShapeEnergyWeight()<<std::endl;
 
  ParameterFile<<"Multi-atlas directory: "<<m_MultiAtlasDirectory<<std::endl; 

  ParameterFile.close();
}

void AutoSegComputation::WriteBMSAutoSegFile()
{
  std::ofstream BMSAutoSegFile(GetBMSAutoSegFile());
  BMSAutoSegFile<<"set (ProcessingDir "<<GetProcessDataDirectory()<<")"<<std::endl;
  BMSAutoSegFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegFile<<"echo ('MAIN COMPUTATION...')"<<std::endl;
  BMSAutoSegFile<<"Include(${ProcessingDir}MainAutoSeg.bms)"<<std::endl;
	
  if (GetAux1Image())
  {
    BMSAutoSegFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegFile<<"echo ('AUXILIARY COMPUTATION...')"<<std::endl;
    BMSAutoSegFile<<"Include(${ProcessingDir}AuxAutoSeg.bms)"<<std::endl;
    BMSAutoSegFile<<"echo (*************************************************)"<<std::endl;

    /*std::string CaudateLeft=GetCaudateLeft();
    std::string CaudateRight=GetCaudateRight();
    std::string HippocampusLeft=GetHippocampusLeft();
    std::string HippocampusRight=GetHippocampusRight();
    std::string PutamenLeft=GetPutamenLeft();
    std::string PutamenRight=GetPutamenRight();
    if (CaudateLeft!="" || CaudateRight!="" || HippocampusLeft!="" || HippocampusRight!="" || PutamenLeft!="" ||PutamenRight!="")
    {
    BMSAutoSegFile<<"echo (*************************************************)"<<std::endl;
  }*/
    BMSAutoSegFile<<"set (ProcessDataDirectoryPath "<<GetProcessDataDirectory()<<")"<<std::endl;
    BMSAutoSegFile<<"set (WarpROIDir ${ProcessDataDirectoryPath}WarpROI/)"<<std::endl;
    BMSAutoSegFile<<"ListFileInDir(AllROICase ${WarpROIDir} *AllROI.nrrd)"<<std::endl;
    BMSAutoSegFile<<"If (${AllROICase} != '')"<<std::endl;
    BMSAutoSegFile<<"  echo (*************************************************)"<<std::endl;
    BMSAutoSegFile<<"EndIf (${AllROICase})"<<std::endl;
    BMSAutoSegFile<<"  echo ()"<<std::endl;
    BMSAutoSegFile<<"  echo (*************************************************)"<<std::endl;
    BMSAutoSegFile<<"  echo ('AUXILIARY COMPUTATION: DONE!')"<<std::endl;
  }
  BMSAutoSegFile.close();
}

//Write Batchmake script to compute Automatic Segmentation
void AutoSegComputation::WriteBMSAutoSegMainFile()
{
  int DataNumber;
  std::ofstream BMSAutoSegMainFile(GetBMSAutoSegMainFile());

  BMSAutoSegMainFile<<"#--------------------- Automatic Segmentation ------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;

  BMSAutoSegMainFile<<"set (OrigT1CasesList "<<m_T1List[0]<<")"<<std::endl;
  //BMSAutoSegMainFile<<"  echo (${OrigT1CasesList})"<<std::endl;
  for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++) {
        BMSAutoSegMainFile<<"set (OrigT1CasesList ${OrigT1CasesList} "<<m_T1List[DataNumber]<<")"<<std::endl;
        //BMSAutoSegMainFile<<"  echo (${OrigT1CasesList})"<<std::endl;
  }

  if (GetT2Image())
  {
        BMSAutoSegMainFile<<"\nset (OrigT2CasesList "<<m_T2List[0]<<")"<<std::endl;
        for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++) {
            BMSAutoSegMainFile<<"set (OrigT2CasesList ${OrigT2CasesList} "<<m_T2List[DataNumber]<<")"<<std::endl;
      }
  }  

  if (GetPDImage())
  {
    BMSAutoSegMainFile<<"\nset (OrigPDCasesList "<<m_PDList[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
      BMSAutoSegMainFile<<"set (OrigPDCasesList ${OrigPDCasesList} "<<m_PDList[DataNumber]<<")"<<std::endl;
  }  
	
  BMSAutoSegMainFile<<"\nset (NbData "<<GetNbData()<<")"<<std::endl;
  
  BMSAutoSegMainFile<<"\n# Data AutoSeg Directory"<<std::endl;
  BMSAutoSegMainFile<<"set (AutoSegDir "<<GetDataAutoSegDirectory()<<")"<<std::endl; 

  BMSAutoSegMainFile<<"# Processing Directory"<<std::endl;
  BMSAutoSegMainFile<<"set (ProcessingDir "<<GetProcessDataDirectory()<<")"<<std::endl;  
  
  BMSAutoSegMainFile<<"# Common Coordinate Image"<<std::endl;
  BMSAutoSegMainFile<<"set (AtlasIsoTemplate "<<GetCommonCoordinateImage()<<")"<<std::endl;  
  BMSAutoSegMainFile<<"GetFilename (AtlasIsoTemplate_name ${AtlasIsoTemplate} NAME)"<<std::endl;
  
  BMSAutoSegMainFile<<"# Segmentation Atlas Directory"<<std::endl;
  BMSAutoSegMainFile<<"set (atlasSegLoc "<<GetTissueSegmentationAtlasDirectory()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (regParamFile "<<GetTissueSegmentationAtlasDirectory()<<"areg_par.txt)"<<std::endl;
    
  BMSAutoSegMainFile<<"# ROI Atlas File"<<std::endl;
  if ( GetSingleAtlasSegmentation() ) {
    BMSAutoSegMainFile<<"set (atlasROIFile "<<GetROIAtlasFile()<<")"<<std::endl;
  }
  if ( GetSingleAtlasSegmentation() && GetANTSWarpingMethod() && GetMultiModalitySingleSegmentation()) {
      BMSAutoSegMainFile<<"# ROI T2 Atlas File"<<std::endl;
      BMSAutoSegMainFile<<"set (atlasROIT2File "<<GetROIT2AtlasFile()<<")"<<std::endl;
  }

  BMSAutoSegMainFile<<"# Stripped Atlas Directory"<<std::endl;
  
  if (strcmp(GetAtlasLoop(),"")) {  
      BMSAutoSegMainFile<<"set (atlasSegLocLoop "<<GetAtlasLoop()<<")"<<std::endl;
    } else {
      BMSAutoSegMainFile<<"set (atlasSegLocLoop None)"<<std::endl;
    }
    

  BMSAutoSegMainFile<<"# Parameters"<<std::endl;
  BMSAutoSegMainFile<<"set(EMSoftware "<<GetEMSoftware()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ComputeVolume "<<GetComputeVolume()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ComputeCorticalThickness "<<GetComputeCorticalThickness()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (RegistrationInitialization "<<GetRegistrationInitialization()<<")"<<std::endl;

  BMSAutoSegMainFile << "# Programs "<<std::endl;
  BMSAutoSegMainFile << "set (ABCCmd "<< m_RequiredToolsPaths.at("ABC") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (neosegCmd " << m_RequiredToolsPaths.at("neoseg") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (warpCmd "<< m_RequiredToolsPaths.at("WarpTool") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (ImageMathCmd " << m_RequiredToolsPaths.at("ImageMath") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (SegPostProcessCmd " << m_RequiredToolsPaths.at("SegPostProcessCLP") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (IntensityRescalerCmd " << m_RequiredToolsPaths.at("IntensityRescaler") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (convCmd " << m_RequiredToolsPaths.at("convertITKformats") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (imconvert3Cmd " << m_RequiredToolsPaths.at("imconvert3") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (ImageStatCmd " << m_RequiredToolsPaths.at("ImageStat") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (fWarpCmd " << m_RequiredToolsPaths.at("fWarp") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (txApplyCmd " << m_RequiredToolsPaths.at("txApply") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (CortThickCLPCmd " << m_RequiredToolsPaths.at("CortThickCLP") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (N4Cmd " << m_RequiredToolsPaths.at("N4ITKBiasFieldCorrection") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (ResampleVolume2Cmd " << m_RequiredToolsPaths.at("ResampleVolume2") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (BRAINSDemonWarpCmd " << m_RequiredToolsPaths.at("BRAINSDemonWarp") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (BRAINSFitCmd " << m_RequiredToolsPaths.at("BRAINSFit") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (BRAINSResampleCmd " << m_RequiredToolsPaths.at("BRAINSResample") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (ANTSCmd " << m_RequiredToolsPaths.at("ANTS") << ")" << std::endl ;
  BMSAutoSegMainFile << "set (WarpImageMultiTransformCmd " << m_RequiredToolsPaths.at("WarpImageMultiTransform") << ")" << std::endl << std::endl ;
  BMSAutoSegMainFile<<"set (ANTSIterations "<<GetANTSIterations()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSCCWeight "<<GetANTSCCWeight()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSCCRegionRadius "<<GetANTSCCRegionRadius()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSMIWeight "<<GetANTSMIWeight()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSMIBins "<<GetANTSMIBins()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSMSQWeight "<<GetANTSMSQWeight()<<")"<<std::endl;
  
  BMSAutoSegMainFile<<"set (ANTSCCWeight2nd "<<GetANTSCCWeight2nd()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSCCRegionRadius2nd "<<GetANTSCCRegionRadius2nd()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSMIWeight2nd "<<GetANTSMIWeight2nd()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSMIBins2nd "<<GetANTSMIBins2nd()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSMSQWeight2nd "<<GetANTSMSQWeight2nd()<<")"<<std::endl;
  
  BMSAutoSegMainFile<<"set (ANTSTransformationStep "<<GetANTSTransformationStep()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ANTSGaussianSigma "<<GetANTSGaussianSigma()<<")"<<std::endl;
  
  BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegMainFile<<"echo ('CHECKING FILES...')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
  BMSAutoSegMainFile<<"echo ('OrigT1CasesList: '${OrigT1CasesList})"<<std::endl;
  if (GetT2Image())
    BMSAutoSegMainFile<<"echo ('OrigT2CasesList: '${OrigT2CasesList})"<<std::endl;
  if (GetPDImage())
    BMSAutoSegMainFile<<"echo ('OrigPDCasesList: '${OrigPDCasesList})"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"echo ('CHECKING FILES: DONE!')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  if (GetRecompute())
  {
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('RECOMPUTE ALL...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
    BMSAutoSegMainFile<<"#  Delete files in the process data directory"<<std::endl;
    BMSAutoSegMainFile<<"DeleteFile(${ProcessingDir}GridTemplate.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"set(command_line rm -r ${ProcessingDir}AutoSeg_Volume/)"<<std::endl;
    BMSAutoSegMainFile<<"Run(output ${command_line})"<<std::endl;
    BMSAutoSegMainFile<<"set(command_line rm -r ${ProcessingDir}AutoSeg_CorticalThickness/)"<<std::endl;
    BMSAutoSegMainFile<<"Run(output ${command_line})"<<std::endl;
    BMSAutoSegMainFile<<"#  Delete files in the data directory"<<std::endl;
    BMSAutoSegMainFile<<"ForEach (OrigCase ${OrigT1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"   set(command_line rm -r ${Path}/${AutoSegDir}/)"<<std::endl;  
    BMSAutoSegMainFile<<"   Run(output ${command_line})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach(OrigCase)"<<std::endl<<std::endl; 
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;  
  }
  
  BMSAutoSegMainFile<<"# Pipeline "<<std::endl;
  BMSAutoSegMainFile<<"# 1. Initialization"<<std::endl;
  BMSAutoSegMainFile<<"# 2. Optional: image reorientation (if no rigid registration)"<<std::endl;
  BMSAutoSegMainFile<<"# 3. N4 ITK bias field correction"<<std::endl;
  BMSAutoSegMainFile<<"# 4. Registration to common coordinate image"<<std::endl;
  BMSAutoSegMainFile<<"# 5. Atlas-based expectation maximization tissue segmentation segmentation"<<std::endl;
  BMSAutoSegMainFile<<"# 6. Skull-stripping"<<std::endl;
  BMSAutoSegMainFile<<"# 7. Optional: Intensity calibration: histogram quantile matching or tissue mean matching - used with fWarp and BRAINSDemonWarp"<<std::endl;
  BMSAutoSegMainFile<<"# 8. Atlas to case affine registration"<<std::endl;
  BMSAutoSegMainFile<<"# 9. Atlas to case warping"<<std::endl;
  BMSAutoSegMainFile<<"# 10. Applying the transformations to the probabilistic maps (ROIs) and label maps (parcellation map and generic ROI map"<<std::endl;
  BMSAutoSegMainFile<<"# 11. Probabilistic volume thresholding"<<std::endl;
  BMSAutoSegMainFile<<"# 12. ROI gathering"<<std::endl<<std::endl;
  if (GetComputeCorticalThickness())
    BMSAutoSegMainFile<<"# 13. Option 1: Regional cortical thickness"<<std::endl;
  if (GetComputeVolume())
  {
    BMSAutoSegMainFile<<"# 13. Option 2: Volume information"<<std::endl;
    BMSAutoSegMainFile<<"#   - Volumes: Tissue segmentation, parcellation maps, subcortical structures"<<std::endl;
    BMSAutoSegMainFile<<"#   - Lobar cortical thickness analysis"<<std::endl<<std::endl;  
  }
  BMSAutoSegMainFile<<"# 14. Multi-Atlas Segmentationg"<<std::endl<<std::endl;
  BMSAutoSegMainFile<<"# 15. Volume information computation"<<std::endl<<std::endl;

  BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"# 1. Initialization"<<std::endl;
  BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    
  BMSAutoSegMainFile<<"# Checking Image Image"<<std::endl;
  BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegMainFile<<"echo ('CHECKING IMAGE TYPE...')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"GetParam(Case ${OrigT1CasesList} 0)"<<std::endl;
  BMSAutoSegMainFile<<"GetFilename (T1ImageExtension ${Case} EXTENSION)"<<std::endl;

  BMSAutoSegMainFile<<"set (Mode -1)"<<std::endl;
  BMSAutoSegMainFile<<"If (${T1ImageExtension} == '.gipl')"<<std::endl;
  BMSAutoSegMainFile<<"   set (Mode 1)"<<std::endl;
  BMSAutoSegMainFile<<"   echo ('Gipl Format')"<<std::endl;
  BMSAutoSegMainFile<<"EndIf (${T1ImageExtension})"<<std::endl;
  BMSAutoSegMainFile<<"If (${T1ImageExtension} == '.gipl.gz')"<<std::endl;
  BMSAutoSegMainFile<<"   set (Mode 1)"<<std::endl;
  BMSAutoSegMainFile<<"   echo ('Zipped Gipl Format')"<<std::endl;
  BMSAutoSegMainFile<<"EndIf (${T1ImageExtension})"<<std::endl;
  BMSAutoSegMainFile<<"If (${T1ImageExtension} == '.mhd')"<<std::endl;
  BMSAutoSegMainFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegMainFile<<"   echo ('Meta Format')"<<std::endl;
  BMSAutoSegMainFile<<"EndIf (${T1ImageExtension})"<<std::endl;
  BMSAutoSegMainFile<<"If (${T1ImageExtension} == '.hdr')"<<std::endl;
  BMSAutoSegMainFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegMainFile<<"   echo ('Analyse Format')"<<std::endl;
  BMSAutoSegMainFile<<"EndIf (${T1ImageExtension})"<<std::endl;  
  BMSAutoSegMainFile<<"If (${T1ImageExtension} == '.mha')"<<std::endl;
  BMSAutoSegMainFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegMainFile<<"   echo ('Meta Format')"<<std::endl;
  BMSAutoSegMainFile<<"EndIf (${T1ImageExtension})"<<std::endl;
  BMSAutoSegMainFile<<"If (${T1ImageExtension} == '.nhdr')"<<std::endl;
  BMSAutoSegMainFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegMainFile<<"   echo ('NRRD Format')"<<std::endl;  
  BMSAutoSegMainFile<<"EndIf (${T1ImageExtension})"<<std::endl;  
  BMSAutoSegMainFile<<"If (${T1ImageExtension} == '.nrrd')"<<std::endl;
  BMSAutoSegMainFile<<"   set (Mode 1)"<<std::endl;
  BMSAutoSegMainFile<<"   echo ('NRRD Format')"<<std::endl;  
  BMSAutoSegMainFile<<"EndIf (${T1ImageExtension})"<<std::endl;  
  BMSAutoSegMainFile<<"If ( ${Mode} == -1)"<<std::endl;
  BMSAutoSegMainFile<<"   echo(Unknown format ${T1ImageExtension})"<<std::endl;
  BMSAutoSegMainFile<<"EndIf (${T1ImageExtension})"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"echo ('CHECKING IMAGE TYPE: DONE!')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;

  BMSAutoSegMainFile<<"set (T1CasesList ${OrigT1CasesList})"<<std::endl;
  BMSAutoSegMainFile<<"echo ('T1 case List:: ' ${T1CasesList})"<<std::endl;

  
  BMSAutoSegMainFile<<"ForEach (T1CasesListShow ${T1CasesList})"<<std::endl;
  BMSAutoSegMainFile<<"      echo ('T1 show cases: '${T1CasesListShow})"<<std::endl;
  BMSAutoSegMainFile<<"EndForEach (T1CasesListShow)"<<std::endl;

  std::ostringstream ssNumberOfThreads ;
  ssNumberOfThreads << "ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS=" << GetNbANTSThreads() ;
  itksys::SystemTools::PutEnv( ssNumberOfThreads.str().c_str() ) ;

  if (GetT2Image())
  {
    BMSAutoSegMainFile<<"set (T2CasesList ${OrigT2CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"GetParam(Case ${OrigT2CasesList} 0)"<<std::endl;
    BMSAutoSegMainFile<<"GetFilename (T2ImageExtension ${Case} EXTENSION)"<<std::endl;

    BMSAutoSegMainFile<<"ForEach (T2CasesListShow ${T2CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      echo ('T2 show cases: '${T2CasesListShow})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (T2CasesListShow)"<<std::endl;
  }
  if (GetPDImage())
  {
    BMSAutoSegMainFile<<"set (PDCasesList ${OrigPDCasesList})"<<std::endl;
    BMSAutoSegMainFile<<"GetParam(Case ${OrigPDCasesList} 0)"<<std::endl;
    BMSAutoSegMainFile<<"GetFilename (PDImageExtension ${Case} EXTENSION)"<<std::endl;

    BMSAutoSegMainFile<<"ForEach (PDCasesListShow ${PDCasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      echo ('PD show cases: '${PDCasesListShow})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (PDCasesListShow)"<<std::endl;
  }

  BMSAutoSegMainFile<<"# Grid Template"<<std::endl;
  if (GetROIAtlasGridTemplate())
    BMSAutoSegMainFile<<"set (GridTemplate ${AtlasIsoTemplate})"<<std::endl<<std::endl;
  else
  {
    BMSAutoSegMainFile<<"set (GridTemplate ${ProcessingDir}GridTemplate.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"ListFileInDir(GridTemplateList ${ProcessingDir} GridTemplate.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"If (${GridTemplateList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"   set (SizeX "<<GetGridTemplateSizeX()<<")"<<std::endl;
    BMSAutoSegMainFile<<"   set (SizeY "<<GetGridTemplateSizeY()<<")"<<std::endl;
    BMSAutoSegMainFile<<"   set (SizeZ "<<GetGridTemplateSizeZ()<<")"<<std::endl;
    BMSAutoSegMainFile<<"   set (SpacingX "<<GetGridTemplateSpacingX()<<")"<<std::endl;
    BMSAutoSegMainFile<<"   set (SpacingY "<<GetGridTemplateSpacingY()<<")"<<std::endl;
    BMSAutoSegMainFile<<"   set (SpacingZ "<<GetGridTemplateSpacingZ()<<")"<<std::endl;
    BMSAutoSegMainFile<<"   set (command_line ${ImageMathCmd} ${atlasROIFile} -outfile ${GridTemplate} -createIm ${SizeX},${SizeY},${SizeZ},${SpacingX},${SpacingY},${SpacingZ})"<<std::endl;
    BMSAutoSegMainFile<<"   Run (output ${command_line})"<<std::endl;
    BMSAutoSegMainFile<<"EndIf (${GridTemplateList})"<<std::endl<<std::endl;
  }

  BMSAutoSegMainFile<<"#Creating AutoSeg Directory and writing parameter files"<<std::endl;
  BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegMainFile<<"echo ('WRITING PARAMETER FILES...')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"set (CaseNumber 0)"<<std::endl;
  BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
  BMSAutoSegMainFile<<"      GetFilename (Path ${T1Case} PATH)"<<std::endl;
  BMSAutoSegMainFile<<"      # Creating Directory if necessary"<<std::endl;
  BMSAutoSegMainFile<<"      set (AutoSegPath ${Path}/${AutoSegDir}/)"<<std::endl;
  BMSAutoSegMainFile<<"      ListDirInDir (AutoSegList ${Path} ${AutoSegDir})"<<std::endl;
  BMSAutoSegMainFile<<"      If (${AutoSegList} == '')"<<std::endl;  
  BMSAutoSegMainFile<<"          MakeDirectory (${AutoSegPath})"<<std::endl; 
  BMSAutoSegMainFile<<"      EndIf (${AutoSegList})"<<std::endl;  
  BMSAutoSegMainFile<<"      Set (copyCommand cp "<<GetParameterFile()<<" ${AutoSegPath})"<<std::endl;
  BMSAutoSegMainFile<<"      Run(output ${copyCommand})"<<std::endl;
  BMSAutoSegMainFile<<"      Set (copyCommand cp "<<GetComputationFile()<<" ${AutoSegPath})"<<std::endl;
  BMSAutoSegMainFile<<"      Run(output ${copyCommand})"<<std::endl; 
  BMSAutoSegMainFile<<"      Inc ({CaseNumber} 1)"<<std::endl;
  BMSAutoSegMainFile<<"      Int ({CaseNumber})"<<std::endl;
  BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"echo ('WRITING PARAMETER FILES: DONE!')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;  

  BMSAutoSegMainFile<<"echo (***********************************************)"<<std::endl;


  BMSAutoSegMainFile<<"Set(ProcessExtension '')"<<std::endl;
  if (GetReorientation())
  {
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 2. Image reorientation"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('Image reorientation...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;

    BMSAutoSegMainFile<<"set (InputOrientation "<<GetInputDataOrientation()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (OutputOrientation "<<GetOutputDataOrientation()<<")"<<std::endl;

    BMSAutoSegMainFile<<"set (ReorientationCasesList ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"Set(T1ImageExtension '.nrrd')"<<std::endl;
    if (GetT2Image())
    {
      BMSAutoSegMainFile<<"set (ReorientationCasesList ${ReorientationCasesList} ${T2CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"Set(T2ImageExtension '.nrrd')"<<std::endl;
    }
    if (GetPDImage())
    {
      BMSAutoSegMainFile<<"set (ReorientationCasesList ${ReorientationCasesList} ${PDCasesList})"<<std::endl;
      BMSAutoSegMainFile<<"Set(PDImageExtension '.nrrd')"<<std::endl;
    }

    BMSAutoSegMainFile<<"Set(ProcessExtension ${ProcessExtension}_${OutputOrientation})"<<std::endl;
    BMSAutoSegMainFile<<"ForEach (Case ${ReorientationCasesList})"<<std::endl;
    BMSAutoSegMainFile<<"  GetFilename (CaseHead ${Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"  GetFilename (Path ${Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"  # Creating reorientation directory"<<std::endl; 
    BMSAutoSegMainFile<<"  set (ReorientationPath ${Path}/${AutoSegDir}/${OutputOrientation}/)"<<std::endl;
    BMSAutoSegMainFile<<"  ListDirInDir (ReorientationList ${Path}/${AutoSegDir}/ ${OutputOrientation})"<<std::endl;
    BMSAutoSegMainFile<<"  If (${ReorientationList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"    MakeDirectory (${ReorientationPath})"<<std::endl;
    BMSAutoSegMainFile<<"  EndIf (${ReorientationList})"<<std::endl;
    BMSAutoSegMainFile<<"  set (ReorientedCase ${ReorientationPath}${CaseHead}${ProcessExtension}.nrrd)"<<std::endl;

    BMSAutoSegMainFile<<"  ListFileInDir(ReorientedCaseList ${ReorientationPath} ${CaseHead}${ProcessExtension}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"  If (${ReorientedCaseList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"    set (NRRDCase ${ReorientationPath}${CaseHead}.nrrd)"<<std::endl;

    BMSAutoSegMainFile<<"    set (command_line ${convCmd} ${Case} ${NRRDCase})"<<std::endl; 
    BMSAutoSegMainFile<<"    Run (output ${command_line} prog_error)"<<std::endl;

    BMSAutoSegMainFile<<"    set (command_line ${imconvert3Cmd} ${GiplCase} ${GiplReorientedCase} -setorient ${InputOrientation}-${OutputOrientation})"<<std::endl;
    BMSAutoSegMainFile<<"    Run (output ${command_line} prog_error)"<<std::endl;

    BMSAutoSegMainFile<<"    DeleteFile(${NRRDCase})"<<std::endl;

    BMSAutoSegMainFile<<"  Else ()"<<std::endl;
    BMSAutoSegMainFile<<"    echo ('Reoriented image already exists!')"<<std::endl;
    BMSAutoSegMainFile<<"  EndIf (${ReorientedCaseList})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (Case)"<<std::endl<<std::endl;

    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('Image reorientation: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }

  
  BMSAutoSegMainFile<<"Set (NbOfIterations "<<GetNbOfIterations()<<")"<<std::endl;
  BMSAutoSegMainFile<<"Set (SplineDistance "<<GetSplineDistance()<<")"<<std::endl;
  BMSAutoSegMainFile<<"Set (ShrinkFactor "<<GetShrinkFactor()<<")"<<std::endl;
  BMSAutoSegMainFile<<"Set (ConvergenceThreshold "<<GetConvergenceThreshold()<<")"<<std::endl;
  BMSAutoSegMainFile<<"Set (BSplineGridResolutions "<<GetBSplineGridResolutions()<<")"<<std::endl;
  BMSAutoSegMainFile<<"Set (BSplineAlpha "<<GetBSplineAlpha()<<")"<<std::endl;
  BMSAutoSegMainFile<<"Set (BSplineBeta "<<GetBSplineBeta()<<")"<<std::endl;
  BMSAutoSegMainFile<<"Set (BSplineOrder "<<GetBSplineOrder()<<")"<<std::endl;
  BMSAutoSegMainFile<<"Set (HistogramSharpening "<<GetHistogramSharpening()<<")"<<std::endl;  

  // set SlicerN4 options
    
  if(GetSlicerVersion() < 4.0) {
    BMSAutoSegMainFile<<"    Set (SlicerN4parameters --histogramsharpening ${HistogramSharpening} --bsplineorder ${BSplineOrder} --shrinkfactor ${ShrinkFactor} --splinedistance ${SplineDistance} --convergencethreshold ${ConvergenceThreshold} --iterations ${NbOfIterations} --meshresolution ${BSplineGridResolutions})"<<std::endl;
  }
  else if(GetSlicerVersion() < 4.3) {
    BMSAutoSegMainFile<<"    Set (SlicerN4parameters --bsplineorder ${BSplineOrder} --shrinkfactor ${ShrinkFactor} --splinedistance ${SplineDistance} --convergencethreshold ${ConvergenceThreshold} --iterations ${NbOfIterations} --meshresolution ${BSplineGridResolutions})"<<std::endl;
    }
  else { // >= 4.3
    BMSAutoSegMainFile<<"    Set (SlicerN4parameters --bsplineorder ${BSplineOrder} --shrinkfactor ${ShrinkFactor} --splinedistance ${SplineDistance} --convergencethreshold ${ConvergenceThreshold} --iterations ${NbOfIterations} --meshresolution ${BSplineGridResolutions})"<<std::endl;
  }

  if (GetN4ITKBiasFieldCorrection())   {
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 3. N4 ITK Bias Field Correction"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('N4 ITK Bias Field Correction...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;

    
    //BMSAutoSegMainFile<<"ForEach (CaseN4Show ${T1CasesList})"<<std::endl;
    //BMSAutoSegMainFile<<"echo ('T1 Cases List: '${CaseN4Show})"<<std::endl;
    //BMSAutoSegMainFile<<"EndForEach (CaseN4Show)"<<std::endl;

    BMSAutoSegMainFile<<"set (CasesListN4 ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"Set(T1ImageExtension '.nrrd')"<<std::endl;


    if (GetT2Image())
    {
      BMSAutoSegMainFile<<"set (CasesListN4 ${CasesListN4} ${T2CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"Set(T2ImageExtension '.nrrd')"<<std::endl;
    }

    if (GetPDImage())
    {
      BMSAutoSegMainFile<<"set (CasesListN4 ${CasesListN4} ${PDCasesList})"<<std::endl;
      BMSAutoSegMainFile<<"Set(PDImageExtension '.nrrd')"<<std::endl;
    }

    BMSAutoSegMainFile<<"Set(Bias 'Bias')"<<std::endl;
    BMSAutoSegMainFile<<"Set(NewProcessExtension ${ProcessExtension}_${Bias})"<<std::endl;
    BMSAutoSegMainFile<<"ForEach (CaseN4 ${CasesListN4})"<<std::endl;
    BMSAutoSegMainFile<<"  GetFilename (OrigCaseHead ${CaseN4} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"  GetFilename (OrigCasePath ${CaseN4} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"  # Creating Bias Directory if necessary"<<std::endl;
    BMSAutoSegMainFile<<"  set (BiasPath ${OrigCasePath}/${AutoSegDir}/Bias/)"<<std::endl;
    BMSAutoSegMainFile<<"  ListDirInDir (BiasList ${OrigCasePath}/${AutoSegDir}/ Bias)"<<std::endl;
    BMSAutoSegMainFile<<"  If (${BiasList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"    MakeDirectory (${BiasPath})"<<std::endl;
    BMSAutoSegMainFile<<"  EndIf (${BiasList})"<<std::endl;

    if (GetReorientation())
      BMSAutoSegMainFile<<"  set (CaseN4 ${OrigCasePath}/${AutoSegDir}/${OutputOrientation}/${OrigCaseHead}${ProcessExtension}.nrrd)"<<std::endl;    

    // is the input there??
    //    BMSAutoSegMainFile<<"  FileExists(N4InputExists ${CaseN4})"<<std::endl;
    //    BMSAutoSegMainFile<<"  If (${N4InputExists} == 1)"<<std::endl;
    //    BMSAutoSegMainFile<<"    echo ('N4 input exists:  ${CaseN4})"<<std::endl;
    //    BMSAutoSegMainFile<<"  EndIf (${N4InputExists})"<<std::endl;
    
    // is the output already there??
    BMSAutoSegMainFile<<"  ListFileInDir(OutputFileN4List ${BiasPath} ${OrigCaseHead}${NewProcessExtension}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"  If (${OutputFileN4List} == '')"<<std::endl;
    BMSAutoSegMainFile<<"    Set (my_output ${BiasPath}${OrigCaseHead}${NewProcessExtension}.nrrd)"<<std::endl;

    // set SlicerN4 options
    
    if(GetSlicerVersion() < 4.3) {
        BMSAutoSegMainFile<<"    Set (command_line ${N4Cmd} --outputimage ${my_output} --inputimage ${CaseN4} ${SlicerN4parameters})"<<std::endl;
    }
    else { // >= 4.3
        BMSAutoSegMainFile<<"    Set (command_line ${N4Cmd}  ${CaseN4} ${my_output} ${SlicerN4parameters})"<<std::endl;
    }


    BMSAutoSegMainFile<<"    Run (prog_output ${command_line} ${parameters} prog_error)"<<std::endl;


    BMSAutoSegMainFile<<"   Else ()"<<std::endl;
    BMSAutoSegMainFile<<"     echo ('Bias field corrected file already exists!')"<<std::endl;
    BMSAutoSegMainFile<<"   EndIf (${OutputFileN4List})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (CaseN4)"<<std::endl;

    BMSAutoSegMainFile<<"Set(ProcessExtension ${NewProcessExtension})"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('N4 ITK Bias Field Correction: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;

  }

  if (GetRigidRegistration())
  {
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 4. Rigid registration to an atlas                                    "<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('REGISTERING CASES TO A TEMPLATE...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;

    bool IsT1FirstFile;
    if ( (std::strcmp(GetCommonCoordinateImageType(),"T2") == 0) && (GetT2Image()))
    {
      BMSAutoSegMainFile<<"set(FirstFile T2)"<<std::endl;
      BMSAutoSegMainFile<<"set(FirstCasesList ${T2CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"set(T1RegistrationExtension _regT2_regAtlas)"<<std::endl;
      if (GetT2Image())
	BMSAutoSegMainFile<<"set(T2RegistrationExtension _regAtlas)"<<std::endl;
      if (GetPDImage())
	BMSAutoSegMainFile<<"set(PDRegistrationExtension _regT2_regAtlas)"<<std::endl;
      IsT1FirstFile = false;
    }
    else
    {
      BMSAutoSegMainFile<<"set(FirstFile T1)"<<std::endl;
      BMSAutoSegMainFile<<"set(FirstCasesList ${T1CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"set(T1RegistrationExtension _regAtlas)"<<std::endl;
      if (GetT2Image())
	BMSAutoSegMainFile<<"set(T2RegistrationExtension _regT1_regAtlas)"<<std::endl;
      if (GetPDImage())
	BMSAutoSegMainFile<<"set(PDRegistrationExtension _regT1_regAtlas)"<<std::endl;
      IsT1FirstFile = true;
    }

    BMSAutoSegMainFile<<"Set(T1ImageExtension '.nrrd')"<<std::endl;
    if (GetT2Image())
      BMSAutoSegMainFile<<"Set(T2ImageExtension '.nrrd')"<<std::endl;
    if (GetPDImage())
      BMSAutoSegMainFile<<"Set(PDImageExtension '.nrrd')"<<std::endl;
		
    BMSAutoSegMainFile<<"echo(${FirstFile}' registration...')"<<std::endl;
    BMSAutoSegMainFile<<"echo( )"<<std::endl;
    BMSAutoSegMainFile<<"ForEach (FirstCase ${FirstCasesList})  "<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (OrigFirstCasePath ${FirstCase} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      set (OrigFirstCasePathList ${OrigFirstCasePathList} ${OrigFirstCasePath})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (OrigFirstCaseHead ${FirstCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo ('Case Number: '${OrigFirstCaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;  
    BMSAutoSegMainFile<<"      set (AtlasIsoPath ${OrigFirstCasePath}/${AutoSegDir}/atlasIso/)"<<std::endl;
    BMSAutoSegMainFile<<"      # Creating atlasIso Directory if necessary"<<std::endl;
    BMSAutoSegMainFile<<"      ListDirInDir (AtlasIsoList ${OrigFirstCasePath}/${AutoSegDir}/ atlasIso)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${AtlasIsoList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory(${AtlasIsoPath})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${AtlasIsoList})"<<std::endl;

    if(GetN4ITKBiasFieldCorrection())
    {
      BMSAutoSegMainFile<<"      set (FirstCase ${OrigFirstCasePath}/${AutoSegDir}/${Bias}/${OrigFirstCaseHead}${ProcessExtension}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (FirstCaseHead ${FirstCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    }
    else
    {
      if (GetReorientation())
	{
	  BMSAutoSegMainFile<<"      set (FirstCase ${OrigFirstCasePath}/${AutoSegDir}/${OutputOrientation}/${OrigFirstCaseHead}${ProcessExtension}.nrrd)"<<std::endl;
	  BMSAutoSegMainFile<<"      GetFilename (FirstCaseHead ${FirstCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
	}
      else
	BMSAutoSegMainFile<<"      set (FirstCaseHead ${OrigFirstCaseHead})"<<std::endl;
    }

    BMSAutoSegMainFile<<"      ListFileInDir(OutputFileList ${AtlasIsoPath} ${FirstCaseHead}_regAtlas.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${OutputFileList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         # Creating new Files"<<std::endl;
    BMSAutoSegMainFile<<"          # Parameter File"<<std::endl;
    BMSAutoSegMainFile<<"         set (TxtInitFileTail ${FirstCaseHead}_init.txt)"<<std::endl;
    BMSAutoSegMainFile<<"         set (TxtInitFile ${AtlasIsoPath}/${TxtInitFileTail})"<<std::endl;  
    BMSAutoSegMainFile<<"          # Parameter File"<<std::endl;
    BMSAutoSegMainFile<<"         set (TxtOutFile ${AtlasIsoPath}${FirstCaseHead}.txt)"<<std::endl;
    BMSAutoSegMainFile<<"          # Report File (process)"<<std::endl;
    BMSAutoSegMainFile<<"         set (ReportFile ${AtlasIsoPath}${FirstCaseHead}_out.txt)"<<std::endl;
    BMSAutoSegMainFile<<"          # Output File"<<std::endl;
    BMSAutoSegMainFile<<"         set (OutputFileTail ${FirstCaseHead}_regAtlas.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"         set (OutputFile ${AtlasIsoPath}${OutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         ListFileInDir(TxtOutFileList ${AtlasIsoPath} ${FirstCaseHead}.txt)"<<std::endl;
    BMSAutoSegMainFile<<"         If (${TxtOutFileList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"            # Computing Transformation"<<std::endl;
    BMSAutoSegMainFile<<"            ListFileInDir(TxtInitFileList ${AtlasIsoPath} ${TxtInitFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"            If (${TxtInitFileList} == '')"<<std::endl;  
    BMSAutoSegMainFile<<"               echo ('Computing rigid transformation T1 to Atlas ...')"<<std::endl;
    BMSAutoSegMainFile<<"    	set (command_line ${BRAINSFitCmd} --fixedVolume ${AtlasIsoTemplate} --movingVolume ${FirstCase} --transformType Rigid --initializeTransformMode ${RegistrationInitialization} --outputTransform ${TxtOutFile} --outputVolume ${OutputFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;

    BMSAutoSegMainFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegMainFile<<"            Else ()"<<std::endl;
    BMSAutoSegMainFile<<"               echo ('Computing rigid transformation T1 to Atlas with initial transform...')"<<std::endl;
    BMSAutoSegMainFile<<"    	set (command_line ${BRAINSFitCmd} --fixedVolume ${AtlasIsoTemplate} --movingVolume ${FirstCase} --transformType Rigid --initialTransform ${TxtInitFile} --outputTransform ${TxtOutFile} --outputVolume ${OutputFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
    BMSAutoSegMainFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegMainFile<<"            EndIf (${TxtInitFileList})"<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${TxtOutFileList})"<<std::endl;
    BMSAutoSegMainFile<<"         # Applying Transformation"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Applying rigid transformation...')"<<std::endl;

    BMSAutoSegMainFile<<"       set (command_line ${ResampleVolume2Cmd} ${FirstCase} ${OutputFile} --transformationFile ${TxtOutFile} -i bs --Reference ${GridTemplate})"<<std::endl;
    BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"       set (command_line ${ImageMathCmd} ${OutputFile} -threshMask 0,1000000 -outfile ${OutputFile})"<<std::endl;
    BMSAutoSegMainFile<<"       Run (prog_output ${command_line} prog_error)"<<std::endl;

    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Registration already Done!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${OutputFileList})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (FirstCase)"<<std::endl<<std::endl;




    bool IsCasesListEmpty = true; 
    if ( (IsT1FirstFile) && (GetT2Image()))
    {
      BMSAutoSegMainFile<<"set(CasesList ${T2CasesList})"<<std::endl;
      IsCasesListEmpty = false;
    }
    else if (!IsT1FirstFile)
    {
      BMSAutoSegMainFile<<"set(CasesList ${T1CasesList})"<<std::endl;
      IsCasesListEmpty = false;
    }

    if (GetPDImage())
    {
      if (IsCasesListEmpty)
	BMSAutoSegMainFile<<"set(CasesList ${PDCasesList})"<<std::endl;
      else
	BMSAutoSegMainFile<<"set(CasesList ${CasesList} ${PDCasesList})"<<std::endl;
      IsCasesListEmpty = false;
    }
    if (!IsCasesListEmpty)
    {
      BMSAutoSegMainFile<<"If (${CasesList} != '')"<<std::endl;
      BMSAutoSegMainFile<<"   echo( )"<<std::endl;
      BMSAutoSegMainFile<<"   echo( )"<<std::endl;
      BMSAutoSegMainFile<<"   echo('Registration...')"<<std::endl;
      BMSAutoSegMainFile<<"   echo( )"<<std::endl;
      BMSAutoSegMainFile<<"   set (CaseNumber 0)"<<std::endl;
      BMSAutoSegMainFile<<"   set (CaseNumberLimit "<<GetNbData()-1<<")"<<std::endl;
      BMSAutoSegMainFile<<"   ForEach (Case ${CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (CaseHead ${Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      GetParam (FirstCase ${FirstCasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (FirstCaseHead ${FirstCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (Path ${FirstCase} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo ('Case Number ' ${CaseNumber} ' : '${CaseHead} ${FirstCase} ${FirstCasesList})"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      set (AtlasIsoPath ${Path}/${AutoSegDir}/atlasIso/)"<<std::endl;
      BMSAutoSegMainFile<<"      ListFileInDir(OutputFileList ${AtlasIsoPath} ${CaseHead}${ProcessExtension}_reg${FirstFile}_regAtlas.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${OutputFileList} == '')"<<std::endl;
      if(GetN4ITKBiasFieldCorrection())
      {
	BMSAutoSegMainFile<<"      set (Case ${Path}/${AutoSegDir}/${Bias}/${CaseHead}${ProcessExtension}.nrrd)"<<std::endl;
	BMSAutoSegMainFile<<"      set (FirstCase ${Path}/${AutoSegDir}/${Bias}/${FirstCaseHead}${ProcessExtension}.nrrd)"<<std::endl;
	BMSAutoSegMainFile<<"      GetFilename (FirstCaseHead ${FirstCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
      }
      else if (GetReorientation())
	{
	  BMSAutoSegMainFile<<"      set (Case ${Path}/${AutoSegDir}/${OutputOrientation}/${CaseHead}${ProcessExtension}.nrrd)"<<std::endl;
	  BMSAutoSegMainFile<<"      set (FirstCase ${Path}/${AutoSegDir}/${OutputOrientation}/${FirstCaseHead}${ProcessExtension}.nrrd)"<<std::endl;
	  BMSAutoSegMainFile<<"      GetFilename (FirstCaseHead ${FirstCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
	}

      BMSAutoSegMainFile<<"      GetFilename (Path ${Case} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (CaseHead ${Case} NAME_WITHOUT_EXTENSION)"<<std::endl;

      BMSAutoSegMainFile<<"         # Creating new Files"<<std::endl;
      BMSAutoSegMainFile<<"          # Parameter File"<<std::endl;
      BMSAutoSegMainFile<<"         set (TxtInitFileTail ${CaseHead}_init.txt)"<<std::endl;
      BMSAutoSegMainFile<<"         set (TxtInitFile ${AtlasIsoPath}${TxtInitFileTail})"<<std::endl;
      BMSAutoSegMainFile<<"          # Parameter File"<<std::endl;
      BMSAutoSegMainFile<<"         set (TxtOutFile ${AtlasIsoPath}${CaseHead}.txt)"<<std::endl;
      BMSAutoSegMainFile<<"          # Report File (process)"<<std::endl;
      BMSAutoSegMainFile<<"         set (ReportFile ${AtlasIsoPath}${CaseHead}_out.txt)"<<std::endl;
      BMSAutoSegMainFile<<"          # registered first Image"<<std::endl;
      BMSAutoSegMainFile<<"         set (FirstCaseregAtlas ${AtlasIsoPath}${FirstCaseHead}_regAtlas.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"          # Output File"<<std::endl;
      BMSAutoSegMainFile<<"         set (OutputFileTail ${CaseHead}_reg${FirstFile}_regAtlas.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"         set (OutputFile ${AtlasIsoPath}${OutputFileTail})"<<std::endl;
      BMSAutoSegMainFile<<"         ListFileInDir(TxtOutFileList ${AtlasIsoPath} ${CaseHead}.txt)"<<std::endl;
      BMSAutoSegMainFile<<"         If (${TxtOutFileList} == '')"<<std::endl;

    BMSAutoSegMainFile<<"             # Computing Transformation"<<std::endl;
    BMSAutoSegMainFile<<"             ListFileInDir(TxtInitFileList ${AtlasIsoPath} ${TxtInitFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"             If (${TxtInitFileList} == '')"<<std::endl; 

    if (!GetInitRegUseT1InitTransform())
      {
	BMSAutoSegMainFile<<"               echo ('Computing rigid transformation within case ...')"<<std::endl;
	BMSAutoSegMainFile<<"    	    set (command_line ${BRAINSFitCmd} --fixedVolume ${FirstCaseregAtlas} --movingVolume ${Case} --transformType Rigid --initializeTransformMode ${RegistrationInitialization} --outputTransform ${TxtOutFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
	BMSAutoSegMainFile<<"      	    Run (output ${command_line} prog_error)"<<std::endl;
	BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;    
      }
    else
      {	
	BMSAutoSegMainFile<<"               echo ('Computing rigid transformation within case using T1 transform file as initialization...')"<<std::endl;
	BMSAutoSegMainFile<<"               set (FirstCaseTransformFile ${AtlasIsoPath}${FirstCaseHead}.txt)"<<std::endl;
	BMSAutoSegMainFile<<"    	    set (command_line ${BRAINSFitCmd} --fixedVolume ${FirstCaseregAtlas} --movingVolume ${Case} --transformType Rigid --initializeTransformMode ${RegistrationInitialization} --outputTransform ${TxtOutFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
	BMSAutoSegMainFile<<"      	    Run (output ${command_line} prog_error)"<<std::endl;
	BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;    
      }
    BMSAutoSegMainFile<<"             Else ()"<<std::endl;
    BMSAutoSegMainFile<<"               echo ('Computing rigid transformation within case with initial transform...')"<<std::endl;
    BMSAutoSegMainFile<<"    	        set (command_line ${BRAINSFitCmd} --fixedVolume ${FirstCaseregAtlas} --movingVolume ${Case} --transformType Rigid --initializeTransformMode ${RegistrationInitialization} --outputTransform ${TxtOutFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
    BMSAutoSegMainFile<<"      	        Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegMainFile<<"             EndIf (${TxtInitFileList})"<<std::endl;
    
    BMSAutoSegMainFile<<"         EndIf (${TxtOutFileList})"<<std::endl;
      BMSAutoSegMainFile<<"         # Applying Transformation"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Applying rigid transformation...')"<<std::endl;
      BMSAutoSegMainFile<<"         set (command_line ${ResampleVolume2Cmd} ${Case} ${OutputFile} --transformationFile ${TxtOutFile} -i bs --Reference ${GridTemplate})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (prog_output ${command_line} prog_error)"<<std::endl;
      BMSAutoSegMainFile<<"         set (command_line ${ImageMathCmd} ${OutputFile} -threshMask 0,1000000 -outfile ${OutputFile})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (prog_output ${command_line} prog_error)"<<std::endl;
      BMSAutoSegMainFile<<"      Else ()"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Registration already Done!')"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${OutputFileList})"<<std::endl;
      BMSAutoSegMainFile<<"   If (${CaseNumber} != ${CaseNumberLimit})"<<std::endl;
      BMSAutoSegMainFile<<"     Inc (${CaseNumber} 1)"<<std::endl;
      BMSAutoSegMainFile<<"     Int (${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"   Else ()"<<std::endl;
      BMSAutoSegMainFile<<"     set(CaseNumber 0)"<<std::endl;
      BMSAutoSegMainFile<<"   EndIf (${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"   EndForEach (Case)  "<<std::endl;
      BMSAutoSegMainFile<<"EndIf(${CasesList})"<<std::endl<<std::endl; 
    }

    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('REGISTERING CASES TO A TEMPLATE: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }
  else
  {
    BMSAutoSegMainFile<<"set(T1RegistrationExtension '')"<<std::endl;
    if (GetT2Image())
      BMSAutoSegMainFile<<"set(T2RegistrationExtension '')"<<std::endl;
    if (GetPDImage())
      BMSAutoSegMainFile<<"set(PDRegistrationExtension '')"<<std::endl<<std::endl;
  }

  int iteration=0;
  int SuffixIteration=0;
  int SuffixIteration_2=0;
  int finalLoop=GetLoopIteration();
  bool IsT1LabelEMSFile=true;

  if ( (std::strcmp(GetTissueSegmentationAtlasType(),"T2") == 0) && (GetT2Image()) )
    IsT1LabelEMSFile = false;

  do
  {
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 5. Tissue Segmentation "<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# ---------------------------------------------------------------------"<<std::endl;
	
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('TISSUE SEGMENTATION...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
        
    BMSAutoSegMainFile<<"      set (stripEMS '')"<<std::endl;
    BMSAutoSegMainFile<<"      set (StrippedBias '')"<<std::endl;

    if (std::strcmp(GetEMSoftware(), "ABC") == 0)
      BMSAutoSegMainFile<<"set (NEOSEG_PREFIX '')"<<std::endl;
    else
      BMSAutoSegMainFile<<"set (NEOSEG_PREFIX '_EMonly')"<<std::endl;
	
    BMSAutoSegMainFile<<"set (EMSComputed 0)"<<std::endl;
    BMSAutoSegMainFile<<"set (CaseNumber 0)"<<std::endl;
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"      set (TissueSegAtlas ${T1Path}/${AutoSegDir}/TissueSegAtlas/)"<<std::endl;
    BMSAutoSegMainFile<<"      set (StrippedTissueSegAtlas ${T1Path}/${AutoSegDir}/StrippedTissueSegAtlas/)"<<std::endl;
    BMSAutoSegMainFile<<"      echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo ('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo ( )"<<std::endl;

    SuffixIteration=iteration+1;
    SuffixIteration_2=iteration+2;


    if (GetLoop() && iteration !=0)
    {
      BMSAutoSegMainFile<<"      Set(T1ImageExtension '.nrrd')"<<std::endl;
      if (GetT2Image())
	BMSAutoSegMainFile<<"      Set(T2ImageExtension '.nrrd')"<<std::endl;
      if (GetPDImage())
	BMSAutoSegMainFile<<"      Set(PDImageExtension '.nrrd')"<<std::endl;

      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;	
      BMSAutoSegMainFile<<"      ListDirInDir (EMSList ${T1Path}/${AutoSegDir}/ ems_"<<SuffixIteration<<")"<<std::endl;
      BMSAutoSegMainFile<<"      set (InputPath ${EMSPath})"<<std::endl;
      BMSAutoSegMainFile<<"      set (SUFFIX EMS_"<<SuffixIteration<<")"<<std::endl;
      if (GetANTSAtlasABC()) 
          BMSAutoSegMainFile<<"      set (Atlas ${StrippedTissueSegAtlas})"<<std::endl;
      else
          BMSAutoSegMainFile<<"      set (Atlas ${atlasSegLocLoop})"<<std::endl;

      BMSAutoSegMainFile<<"      echo ('The Atlas is '${Atlas})"<<std::endl;
      BMSAutoSegMainFile<<"      set (stripEMS _stripEMS${StrippedBias})"<<std::endl;
      if (GetStrippedN4ITKBiasFieldCorrection()) {
        BMSAutoSegMainFile<<"      set (StrippedBias '_Bias')"<<std::endl;
      }
      if( SetNbStrippedTissueClass(GetAtlasLoop()) )
      {
          BMSAutoSegMainFile.close();
          throw std::runtime_error( "Could not read tissue seg loop directory, or directory did not contain any tissue file.") ;
      }
    }
    else
    {
        BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
        BMSAutoSegMainFile<<"      ListDirInDir (EMSList ${T1Path}/${AutoSegDir}/ ems)"<<std::endl;
        if (GetRigidRegistration())
          BMSAutoSegMainFile<<"      set (InputPath ${T1Path}/${AutoSegDir}/atlasIso/)"<<std::endl;
        else if (GetN4ITKBiasFieldCorrection())
          BMSAutoSegMainFile<<"      set (InputPath ${T1Path}/${AutoSegDir}/${Bias}/)"<<std::endl;
        else if(GetReorientation())
          BMSAutoSegMainFile<<"      set (InputPath ${T1Path}/${AutoSegDir}/${OutputOrientation}/)"<<std::endl;
        else
          BMSAutoSegMainFile<<"      set (InputPath ${T1Path}/)"<<std::endl;
       
        BMSAutoSegMainFile<<"      set  (SUFFIX EMS)"<<std::endl;
        if (GetANTSAtlasABC()) 
            BMSAutoSegMainFile<<"      set (Atlas ${TissueSegAtlas})"<<std::endl;
        else
            BMSAutoSegMainFile<<"      set (Atlas ${atlasSegLoc})"<<std::endl;
        
        BMSAutoSegMainFile<<"      set (stripEMS '')"<<std::endl;
        BMSAutoSegMainFile<<"      set (StrippedBias '')"<<std::endl;
       
        std::cout << "Tissue Atlas: " << GetTissueSegmentationAtlasDirectory() << std::endl;
        std::cout << "Stripped Tissue Atlas: " << GetAtlasLoop() << std::endl;
        if( SetNbTissueClass(GetTissueSegmentationAtlasDirectory()) )
        {
          BMSAutoSegMainFile.close();
          throw std::runtime_error( "Could not read tissue segmentation atlas directory, or directory did not contain any tissue file.") ;
        }
        
    }

    BMSAutoSegMainFile<<"      set (SuffixCorrected _corrected_${SUFFIX})"<<std::endl;
    BMSAutoSegMainFile<<"      set  (SuffixLabel ${NEOSEG_PREFIX}_labels_${SUFFIX})"<<std::endl;
    BMSAutoSegMainFile<<"      set  (SuffixRegistered _registered_${SUFFIX})"<<std::endl;
    BMSAutoSegMainFile<<"      set  (SuffixTemplate _template_${SUFFIX})"<<std::endl;

    BMSAutoSegMainFile<<"      # Creating EMS Directory if necessary"<<std::endl;
    BMSAutoSegMainFile<<"      If (${EMSList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory(${EMSPath})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${EMSList})"<<std::endl;

    BMSAutoSegMainFile<<"      set (T1InputCase ${InputPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${StrippedBias}${T1ImageExtension})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1InputCaseHead ${T1InputCase} NAME_WITHOUT_EXTENSION)"<<std::endl; 
	
    if (GetT2Image())
    {
      BMSAutoSegMainFile<<"      If (${T2CasesList} != '')"<<std::endl;
      BMSAutoSegMainFile<<"         GetParam (T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"         GetFilename(T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"         set (T2InputCase ${InputPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}${StrippedBias}${T2ImageExtension})"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${T2CasesList})"<<std::endl;
    }
	
    if (GetPDImage())
    {
      BMSAutoSegMainFile<<"      If (${PDCasesList} != '')"<<std::endl;
      BMSAutoSegMainFile<<"         GetParam (PDCase ${PDCasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"         GetFilename(PDCaseHead ${PDCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"         set (PDInputCase ${InputPath}${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${stripEMS}${StrippedBias}${PDImageExtension})"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${PDCasesList})"<<std::endl;
    }
	
    BMSAutoSegMainFile<<"         # Setting parameters"<<std::endl;
    if (std::strcmp(GetEMSoftware(), "ABC") == 0)
      BMSAutoSegMainFile<<"         set (EMSfile ${EMSPath}ABCparam.xml)"<<std::endl;
    else
      BMSAutoSegMainFile<<"         set (EMSfile ${EMSPath}EMSparam.xml)"<<std::endl;

    // check if inputs exist
    //    BMSAutoSegMainFile<<"         FileExists(EMST1InputExists ${T1InputCase})"<<std::endl;
    //    BMSAutoSegMainFile<<"         If (${EMST1InputExists} == 1)"<<std::endl;
    //    BMSAutoSegMainFile<<"           echo ('EMS T1 input exists:  ${T1InputCase})"<<std::endl;
    //    BMSAutoSegMainFile<<"         EndIf (${EMST1InputExists})"<<std::endl;

    // check if T1 EMS corrected file already exists, if not compute
    BMSAutoSegMainFile<<"         ListFileInDir(EMSFileList ${EMSPath} ${T1InputCaseHead}${SuffixCorrected}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"         If (${EMSFileList} == '')"<<std::endl;  
    BMSAutoSegMainFile<<"           set (EMSComputed 1)"<<std::endl;
	
    BMSAutoSegMainFile<<"            # create EMS file"<<std::endl;
    BMSAutoSegMainFile<<"	       WriteFile (${EMSfile} '<?xml version=\"1.0\"?>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<!DOCTYPE SEGMENTATION-PARAMETERS>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<SEGMENTATION-PARAMETERS>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<SUFFIX>'${SUFFIX}'</SUFFIX>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-DIRECTORY>'${Atlas}'</ATLAS-DIRECTORY>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-ORIENTATION>file</ATLAS-ORIENTATION>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<OUTPUT-DIRECTORY>'${EMSPath}'</OUTPUT-DIRECTORY>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<OUTPUT-FORMAT>Nrrd</OUTPUT-FORMAT>\\n')"<<std::endl;
	
    if ( (std::strcmp(GetTissueSegmentationAtlasType(),"T2") == 0) && (GetT2Image()) )
    {
      BMSAutoSegMainFile<<"    	   AppendFile (${EMSfile} '<IMAGE>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	           AppendFile (${EMSfile} '  <FILE>'${T2InputCase}'</FILE>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	           AppendFile (${EMSfile} '  <ORIENTATION>file</ORIENTATION>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	           AppendFile (${EMSfile} '</IMAGE>\\n')"<<std::endl;
	
      BMSAutoSegMainFile<<"	           AppendFile(${EMSfile} '<IMAGE>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	           AppendFile(${EMSfile} '  <FILE>'${T1InputCase}'</FILE>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	           AppendFile(${EMSfile} '  <ORIENTATION>file</ORIENTATION>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	           AppendFile(${EMSfile} '</IMAGE>\\n')"<<std::endl;
    }
    else
    {
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<IMAGE>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '  <FILE>'${T1InputCase}'</FILE>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '  <ORIENTATION>file</ORIENTATION>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '</IMAGE>\\n')"<<std::endl;
		
      if (GetT2Image())
      {
	BMSAutoSegMainFile<<"	       AppendFile (${EMSfile} '<IMAGE>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile (${EMSfile} '  <FILE>'${T2InputCase}'</FILE>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile (${EMSfile} '  <ORIENTATION>file</ORIENTATION>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile (${EMSfile} '</IMAGE>\\n')"<<std::endl;
      }
    }
	
    if (GetPDImage())
    {
      BMSAutoSegMainFile<<"           AppendFile (${EMSfile} '<IMAGE>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	        AppendFile (${EMSfile} '  <FILE>'${PDInputCase}'</FILE>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	        AppendFile (${EMSfile} '  <ORIENTATION>file</ORIENTATION>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	        AppendFile (${EMSfile} '</IMAGE>\\n')"<<std::endl;
    }
	
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<FILTER-ITERATIONS>"<<GetFilterIterations()<<"</FILTER-ITERATIONS>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<FILTER-TIME-STEP>"<<GetFilterTimeStep()<<"</FILTER-TIME-STEP>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<FILTER-METHOD>"<<GetFilterMethod()<<"</FILTER-METHOD>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<MAX-BIAS-DEGREE>"<<GetMaxBiasDegree()<<"</MAX-BIAS-DEGREE>\\n')"<<std::endl;

    if (std::strcmp(GetEMSoftware(), "ABC") == 0)
    {
      if (iteration == 0) {
          if (GetNbTissueClass() > 0)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior1()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbTissueClass() > 1)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior2()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbTissueClass() > 2)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior3()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbTissueClass() > 3)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior4()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbTissueClass() > 4)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior5()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbTissueClass() > 5)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior6()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbTissueClass() > 6)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior7()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbTissueClass() > 7)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior8()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbTissueClass() > 8)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior9()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbTissueClass() > 9) {
              for (int i = 10; i <= GetNbTissueClass(); i++){
                  BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>1.0</PRIOR>\\n')"<<std::endl;
              }
          }
      }
      else {
          if (GetNbStrippedTissueClass() > 0)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior1()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbStrippedTissueClass() > 1)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior2()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbStrippedTissueClass() > 2)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior3()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbStrippedTissueClass() > 3)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior4()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbStrippedTissueClass() > 4)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior5()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbStrippedTissueClass() > 5)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior6()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbStrippedTissueClass() > 6)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior7()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbStrippedTissueClass() > 7)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior8()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbStrippedTissueClass() > 8)
              BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior9()<<"</PRIOR>\\n')"<<std::endl;
          if (GetNbStrippedTissueClass() > 9) {
              for (int i = 10; i <= GetNbStrippedTissueClass(); i++){
                  BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>1.0</PRIOR>\\n')"<<std::endl;
              }
          }
      }
    }
    else if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
      {
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior1()<<"</PRIOR>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior2()<<"</PRIOR>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior3()<<"</PRIOR>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior4()<<"</PRIOR>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior5()<<"</PRIOR>\\n')"<<std::endl;
      }
	
    if(std::strcmp(GetEMSoftware(), "ABC") == 0) {
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<INITIAL-DISTRIBUTION-ESTIMATOR>"<<GetInitialDistributionEstimator()<<"</INITIAL-DISTRIBUTION-ESTIMATOR>\\n')"<<std::endl;
        if (GetFluidAtlasFATW() && GetLoop() && iteration !=0)
        {
 	    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<DO-ATLAS-WARP>"<<GetFluidAtlasFATW()<<"</DO-ATLAS-WARP>\\n')"<<std::endl;
        }
        else {
		// only arrive in here if "affine then warp" is not set or in first/zeroth iteration of affine then warp 
            BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<DO-ATLAS-WARP>"<<GetFluidAtlasWarp()<<"</DO-ATLAS-WARP>\\n')"<<std::endl;
	}
	if (GetANTSAtlasABC())
	{
            BMSAutoSegMainFile<<"      ListFileInDir (TissueSegAtlasBrainMaskList ${atlasSegLoc} brainmask.nrrd)" << std::endl;                 // check whether brainmask in TissueSeg exist
            BMSAutoSegMainFile<<"      ListFileInDir (StrippedTissueSegAtlasBrainMaskList ${atlasSegLocLoop} brainmask.nrrd)" << std::endl;     // check whether brainmask in Stripped TissueSeg exist
            BMSAutoSegMainFile<<"      ListFileInDir (TissueSegAtlasImageList ${atlasSegLoc} *.mha)" << std::endl;                              // get tissue classes list
            BMSAutoSegMainFile<<"      ListFileInDir (TissueSegAtlasTemplateList ${TissueSegAtlas} template.mha)" << std::endl;                 // check whether template in TissueSeg exist 
            BMSAutoSegMainFile<<"      ListFileInDir (StrippedTissueSegAtlasTemplateList ${StrippedTissueSegAtlas} template.mha)" << std::endl; // check whether template in Stripped TissueSeg exist

            if (GetLoop() && iteration != 0) {
	      // not the first iteration
	      BMSAutoSegMainFile<<"      If (${StrippedTissueSegAtlasBrainMaskList} == '')" << std::endl; //if don't have brainmask in stripped tissue seg atlas
	      BMSAutoSegMainFile<<"        If (${StrippedTissueSegAtlasTemplateList} == '')" << std::endl;
	      //changed by Yundi Shi to use ${ANTSGaussianSigma} instead of default 3
	      BMSAutoSegMainFile<<"          set (ANTSRegStrippedTissueSegAtals ANTS 3 -m CC[${T1InputCase},${atlasSegLocLoop}template.mha,1,2] -i ${ANTSIterations} -o ${StrippedTissueSegAtlas}Atlas_T1Total.nii.gz -t SyN[0.25] -r Gauss[${ANTSGaussianSigma},0])"<<std::endl;
	      BMSAutoSegMainFile<<"          Run (output ${ANTSRegStrippedTissueSegAtals})"<<std::endl;
	      BMSAutoSegMainFile<<"          ForEach (TissueSegAtlasImage ${TissueSegAtlasImageList})"<<std::endl;
	      BMSAutoSegMainFile<<"            set (WarpStrippedTissueSegAtals WarpImageMultiTransform 3 ${atlasSegLocLoop}${TissueSegAtlasImage} ${StrippedTissueSegAtlas}${TissueSegAtlasImage} -R ${T1InputCase} ${StrippedTissueSegAtlas}Atlas_T1TotalWarp.nii.gz ${StrippedTissueSegAtlas}Atlas_T1TotalAffine.txt)"<<std::endl;
	      BMSAutoSegMainFile<<"            Run (output ${WarpStrippedTissueSegAtals})"<<std::endl;
	      BMSAutoSegMainFile<<"            set (command_line ${ImageMathCmd} ${StrippedTissueSegAtlas}${TissueSegAtlasImage} -constOper 2,1 -outfile ${StrippedTissueSegAtlas}${TissueSegAtlasImage})"<<std::endl;
	      BMSAutoSegMainFile<<"            Run (prog_output ${command_line} prog_error)"<<std::endl;
	      BMSAutoSegMainFile<<"          EndForEach(TissueSegAtlasImage)"<<std::endl;
	      BMSAutoSegMainFile<<"        EndIf (${StrippedTissueSegAtlasTemplateList})" << std::endl;
	      BMSAutoSegMainFile<<"      Else () " << std::endl;    // brainmask exist

	      BMSAutoSegMainFile<<"        If (   ${StrippedTissueSegAtlasTemplateList} == '')" << std::endl;
	      BMSAutoSegMainFile<<"          set (ANTSRegStrippedTissueSegAtals ANTS 3 -m CC[${T1InputCase},${atlasSegLocLoop}template.mha,1,2] -i ${ANTSIterations} -o ${StrippedTissueSegAtlas}Atlas_T1Total.nii.gz -t SyN[0.25] -r Gauss[${ANTSGaussianSigma},0] -a ${TissueSegAtlas}template_affine_transform.txt --continue-affine true)"<<std::endl;
	      BMSAutoSegMainFile<<"          Run (output ${ANTSRegStrippedTissueSegAtals})"<<std::endl;
	      BMSAutoSegMainFile<<"          ForEach (TissueSegAtlasImage ${TissueSegAtlasImageList})"<<std::endl;
	      BMSAutoSegMainFile<<"            set (WarpStrippedTissueSegAtals WarpImageMultiTransform 3 ${atlasSegLocLoop}${TissueSegAtlasImage} ${StrippedTissueSegAtlas}${TissueSegAtlasImage} -R ${T1InputCase} ${StrippedTissueSegAtlas}Atlas_T1TotalWarp.nii.gz ${StrippedTissueSegAtlas}Atlas_T1TotalAffine.txt)"<<std::endl;
	      BMSAutoSegMainFile<<"            Run (output ${WarpStrippedTissueSegAtals})"<<std::endl;
	      BMSAutoSegMainFile<<"           set (command_line ${ImageMathCmd} ${StrippedTissueSegAtlas}${TissueSegAtlasImage} -constOper 2,1 -outfile ${StrippedTissueSegAtlas}${TissueSegAtlasImage})"<<std::endl;
	      BMSAutoSegMainFile<<"           Run (prog_output ${command_line} prog_error)"<<std::endl;
	      BMSAutoSegMainFile<<"          EndForEach(TissueSegAtlasImage)"<<std::endl;
	      BMSAutoSegMainFile<<"        EndIf (${StrippedTissueSegAtlasTemplateList})" << std::endl;
	      BMSAutoSegMainFile<<"      EndIf (${TissueSegAtlasBrainMaskList})" << std::endl;
            }
            else 
            {
	      // first iteration
	      // prepare the Tissue Seg Atlas
	      BMSAutoSegMainFile<<"      ListDirInDir (TissueSegAtlasList ${T1Path}/${AutoSegDir}/ TissueSegAtlas)"<<std::endl;
	      BMSAutoSegMainFile<<"      If (${TissueSegAtlasList} == '')"<<std::endl;
	      BMSAutoSegMainFile<<"          MakeDirectory (${TissueSegAtlas})"<<std::endl;
	      BMSAutoSegMainFile<<"      EndIf (${TissueSegAtlasList})"<<std::endl;
	      
	      // prepare the Stripped Tissue Seg Atlas
	      BMSAutoSegMainFile<<"      ListDirInDir (StrippedTissueSegAtlasList ${T1Path}/${AutoSegDir}/ StrippedTissueSegAtlas)"<<std::endl;
	      BMSAutoSegMainFile<<"      If (${StrippedTissueSegAtlasList} == '')"<<std::endl;
	      BMSAutoSegMainFile<<"          MakeDirectory (${StrippedTissueSegAtlas})"<<std::endl;
	      BMSAutoSegMainFile<<"      EndIf (${StrippedTissueSegAtlasList})"<<std::endl;

	      BMSAutoSegMainFile<<"      If (${TissueSegAtlasBrainMaskList} == '')" << std::endl;   //if don't have brainmask in tissue seg atlas
	      BMSAutoSegMainFile<<"        If (${TissueSegAtlasTemplateList} == '')" << std::endl;  //if tissue seg atlas template doesn't exist -> do registration
	      
	      BMSAutoSegMainFile<<"          set (ANTSRegTissueSegAtals ANTS 3 -m CC[${T1InputCase},${atlasSegLoc}template.mha,1,2] -i ${ANTSIterations} -o ${TissueSegAtlas}Atlas_T1Total.nii.gz -t SyN[0.25] -r Gauss[${ANTSGaussianSigma},0])"<<std::endl;
	      BMSAutoSegMainFile<<"          Run (output ${ANTSRegTissueSegAtals})"<<std::endl;
	      
	      BMSAutoSegMainFile<<"          ForEach (TissueSegAtlasImage ${TissueSegAtlasImageList})"<<std::endl;
	      BMSAutoSegMainFile<<"            set (WarpTissueSegAtals WarpImageMultiTransform 3 ${atlasSegLoc}${TissueSegAtlasImage} ${TissueSegAtlas}${TissueSegAtlasImage} -R ${T1InputCase} ${TissueSegAtlas}Atlas_T1TotalWarp.nii.gz ${TissueSegAtlas}Atlas_T1TotalAffine.txt)"<<std::endl;
	      BMSAutoSegMainFile<<"            Run (output ${WarpTissueSegAtals})"<<std::endl;
	      BMSAutoSegMainFile<<"            set (command_line ${ImageMathCmd} ${TissueSegAtlas}${TissueSegAtlasImage} -constOper 2,1 -outfile ${TissueSegAtlas}${TissueSegAtlasImage})"<<std::endl;
	      BMSAutoSegMainFile<<"            Run (prog_output ${command_line} prog_error)"<<std::endl;
	      BMSAutoSegMainFile<<"          EndForEach(TissueSegAtlasImage)"<<std::endl;
	      BMSAutoSegMainFile<<"        EndIf (${TissueSegAtlasTemplateList})" << std::endl;
	      BMSAutoSegMainFile<<"      Else () " << std::endl;    // brainmask exist
	      BMSAutoSegMainFile<<"        ListFileInDir (AffineBrainMaskList ${TissueSegAtlas} brainmask_affine.nrrd)" << std::endl;  // check if warped brainmask already exist
	      BMSAutoSegMainFile<<"        If (   ${AffineBrainMaskList} == '')" << std::endl;
	      BMSAutoSegMainFile<<"          set (AffineTissueSegAtlasTemplate ${BRAINSFitCmd} --useAffine --outputTransform ${TissueSegAtlas}template_affine_transform.txt --outputVolume ${TissueSegAtlas}template_affine.nrrd --movingVolume ${atlasSegLoc}template.mha --fixedVolume ${T1InputCase} --initializeTransformMode ${RegistrationInitialization})"<<std::endl;
	      BMSAutoSegMainFile<<"          set (AffineTissueSegAtlasResample ${BRAINSResampleCmd} --warpTransform ${TissueSegAtlas}template_affine_transform.txt --outputVolume ${TissueSegAtlas}brainmask_affine.nrrd --inputVolume ${atlasSegLoc}brainmask.nrrd --referenceVolume ${T1InputCase})"<<std::endl;
	      BMSAutoSegMainFile<<"          set (AffineStrippedTissueSegAtlasResample ${BRAINSResampleCmd} --warpTransform ${TissueSegAtlas}template_affine_transform.txt --outputVolume ${StrippedTissueSegAtlas}brainmask_affine.nrrd --inputVolume ${atlasSegLocLoop}brainmask.nrrd --referenceVolume ${T1InputCase})"<<std::endl;
	      BMSAutoSegMainFile<<"          Run (output ${AffineTissueSegAtlasTemplate})"<<std::endl;
	      BMSAutoSegMainFile<<"          Run (output ${AffineTissueSegAtlasResample})"<<std::endl;
	      BMSAutoSegMainFile<<"          Run (output ${AffineStrippedTissueSegAtlasResample})"<<std::endl;
	      BMSAutoSegMainFile<<"        EndIf (${AffineBrainMaskList})" << std::endl;
 
	      BMSAutoSegMainFile<<"        If (   ${TissueSegAtlasTemplateList} == '')" << std::endl;
	      if (GetANTSWithBrainmask())  {
		if (GetUseInitialAffine())  {
		  BMSAutoSegMainFile<<"         set (ANTSRegTissueSegAtals ANTS 3 -m CC[${T1InputCase},${atlasSegLoc}template.mha,1,2] -i ${ANTSIterations} -o ${TissueSegAtlas}Atlas_T1Total.nii.gz -t SyN[0.25] -x ${TissueSegAtlas}brainmask_affine.nrrd -r Gauss[${ANTSGaussianSigma},0] -a ${TissueSegAtlas}template_affine_transform.txt --continue-affine false)"<<std::endl;
		} else {
		  BMSAutoSegMainFile<<"         set (ANTSRegTissueSegAtals ANTS 3 -m CC[${T1InputCase},${atlasSegLoc}template.mha,1,2] -i ${ANTSIterations} -o ${TissueSegAtlas}Atlas_T1Total.nii.gz -t SyN[0.25] -x ${TissueSegAtlas}brainmask_affine.nrrd -r Gauss[${ANTSGaussianSigma},0] -a ${TissueSegAtlas}template_affine_transform.txt --continue-affine true)"<<std::endl;
		}
	      } else  {
		if (GetUseInitialAffine())  {
		  BMSAutoSegMainFile<<"         set (ANTSRegTissueSegAtals ANTS 3 -m CC[${T1InputCase},${atlasSegLoc}template.mha,1,2] -i ${ANTSIterations} -o ${TissueSegAtlas}Atlas_T1Total.nii.gz -t SyN[0.25] -r Gauss[${ANTSGaussianSigma},0] -a ${TissueSegAtlas}template_affine_transform.txt --continue-affine false)"<<std::endl;
		} else {
		  BMSAutoSegMainFile<<"         set (ANTSRegTissueSegAtals ANTS 3 -m CC[${T1InputCase},${atlasSegLoc}template.mha,1,2] -i ${ANTSIterations} -o ${TissueSegAtlas}Atlas_T1Total.nii.gz -t SyN[0.25] -r Gauss[${ANTSGaussianSigma},0] -a ${TissueSegAtlas}template_affine_transform.txt --continue-affine true)"<<std::endl;
		}
	      }
	      BMSAutoSegMainFile<<"         Run (output ${ANTSRegTissueSegAtals})"<<std::endl;
	      BMSAutoSegMainFile<<"         ForEach (TissueSegAtlasImage ${TissueSegAtlasImageList})"<<std::endl;
	      BMSAutoSegMainFile<<"           set (WarpTissueSegAtals WarpImageMultiTransform 3 ${atlasSegLoc}${TissueSegAtlasImage} ${TissueSegAtlas}${TissueSegAtlasImage} -R ${T1InputCase} ${TissueSegAtlas}Atlas_T1TotalWarp.nii.gz ${TissueSegAtlas}Atlas_T1TotalAffine.txt)"<<std::endl;
	      BMSAutoSegMainFile<<"           Run (output ${WarpTissueSegAtals})"<<std::endl;
	      BMSAutoSegMainFile<<"           set (command_line ${ImageMathCmd} ${TissueSegAtlas}${TissueSegAtlasImage} -constOper 2,1 -outfile ${TissueSegAtlas}${TissueSegAtlasImage})"<<std::endl;
	      BMSAutoSegMainFile<<"           Run (prog_output ${command_line} prog_error)"<<std::endl;
	      BMSAutoSegMainFile<<"         EndForEach(TissueSegAtlasImage)"<<std::endl;
	      BMSAutoSegMainFile<<"        EndIf (${TissueSegAtlasTemplateList})" << std::endl;
	      BMSAutoSegMainFile<<"      EndIf (${TissueSegAtlasBrainMaskList})" << std::endl;
            }
        }
        BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-WARP-FLUID-ITERATIONS>"<<GetFluidAtlasWarpIterations()<<"</ATLAS-WARP-FLUID-ITERATIONS>\\n')"<<std::endl;
        BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-WARP-FLUID-MAX-STEP>"<<GetFluidAtlasWarpMaxStep()<<"</ATLAS-WARP-FLUID-MAX-STEP>\\n')"<<std::endl;
    }
    else if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
      {
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<DO-ATLAS-WARP>"<<GetBSplineAtlasWarp()<<"</DO-ATLAS-WARP>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-WARP-GRID-X>"<<GetBSplineAtlasWarpGridX()<<"</ATLAS-WARP-GRID-X>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-WARP-GRID-Y>"<<GetBSplineAtlasWarpGridY()<<"</ATLAS-WARP-GRID-Y>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-WARP-GRID-Z>"<<GetBSplineAtlasWarpGridZ()<<"</ATLAS-WARP-GRID-Z>\\n')"<<std::endl;
      }
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-LINEAR-MAP-TYPE>"<<GetAtlasLinearMapping()<<"</ATLAS-LINEAR-MAP-TYPE>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<IMAGE-LINEAR-MAP-TYPE>"<<GetImageLinearMapping()<<"</IMAGE-LINEAR-MAP-TYPE>\\n')"<<std::endl;
      
    if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
      {
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<MAHALANOBIS-THRESHOLD>"<<GetNeosegMahalanobisThreshold()<<"</MAHALANOBIS-THRESHOLD>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PARZEN-KERNEL-WIDTH>"<<GetNeosegParzenKernel()<<"</PARZEN-KERNEL-WIDTH>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR-THRESHOLD>"<<GetNeosegPriorThreshold()<<"</PRIOR-THRESHOLD>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<REFERENCE-IMAGE-INDEX>1</REFERENCE-IMAGE-INDEX>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<REFERENCE-MODALITY>"<<GetTissueSegmentationAtlasType()<<"</REFERENCE-MODALITY>\\n')"<<std::endl;
      }
      
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '</SEGMENTATION-PARAMETERS>\\n')"<<std::endl;
	
    if (std::strcmp(GetEMSoftware(), "ABC") == 0)
    {	
      
      BMSAutoSegMainFile<<"               set ( command_line ${ABCCmd} ${EMSfile})"<<std::endl;
      BMSAutoSegMainFile<<"               Run (output ${command_line})"<<std::endl;	  
    }
    else if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
    {
      BMSAutoSegMainFile<<"               set (command_line ${neosegCmd} ${EMSfile})"<<std::endl;
      BMSAutoSegMainFile<<"               Run (output ${command_line})"<<std::endl;	 
    }
    else
    {
      std::string errorMessage = "Tissue segmentation software: " ;
      if( std::string( GetEMSoftware() ) == "" )
      {
        errorMessage += "None specified\n" ;
      }
      else
      {
        errorMessage += std::string( GetEMSoftware() ) + std::string( "\n" ) ;
      }
      errorMessage += "Error EM Software (itkEMS is no longer supported)!" ;
      throw std::runtime_error( errorMessage ) ;
    }   
    // next lines are kept for historic reasons MRML settings 
    BMSAutoSegMainFile<<"	       set (OutputFileTail ${T1InputCaseHead}${SuffixCorrected}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"	       set (LabelFile ${EMSPath}${T1InputCaseHead}${SuffixLabel}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"	       set (LabelTail ${T1InputCaseHead}${SuffixLabel}.nrrd)"<<std::endl;

    BMSAutoSegMainFile<<"         Else ()"<<std::endl;
    BMSAutoSegMainFile<<"            echo ('EMS Segmentation already Done!')"<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${EMSFileList})"<<std::endl;
    BMSAutoSegMainFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMainFile<<"   Int (${CaseNumber})"<<std::endl;

    BMSAutoSegMainFile<<"EndForEach(T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('EMS SEGMENTATION: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;

    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 6. Skull Stripping"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl<<std::endl;

    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('SKULL STRIPPING...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"set (CaseNumber 0)"<<std::endl;

    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;  
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;  
	
    BMSAutoSegMainFile<<"      set (stripEMS _corrected_EMS-stripped)"<<std::endl;

    if (GetRigidRegistration())
      BMSAutoSegMainFile<<"      set (InputPath ${T1Path}/${AutoSegDir}/atlasIso/)"<<std::endl;
    else if (GetN4ITKBiasFieldCorrection())
      BMSAutoSegMainFile<<"      set (InputPath ${T1Path}/${AutoSegDir}/${Bias}/)"<<std::endl;
    else if(GetReorientation())
      BMSAutoSegMainFile<<"      set (InputPath ${T1Path}/${AutoSegDir}/${OutputOrientation}/)"<<std::endl;
    else
      BMSAutoSegMainFile<<"      set (InputPath ${T1Path}/)"<<std::endl;

    if (GetLoop() && iteration!=0)
    {
      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
      BMSAutoSegMainFile<<"         set  (SUFFIX EMS_"<<SuffixIteration<<")"<<std::endl;
      BMSAutoSegMainFile<<"         set (SuffixCorrected _stripEMS${StrippedBias}_corrected_${SUFFIX})"<<std::endl;
      BMSAutoSegMainFile<<"         set (SuffixLabel _stripEMS${StrippedBias}${NEOSEG_PREFIX}_labels_${SUFFIX})"<<std::endl;
      if(iteration==finalLoop)
      {
	BMSAutoSegMainFile<<"      set (InputPath ${EMSPath}/)"<<std::endl;
	BMSAutoSegMainFile<<"      ListDirInDir (StrippedList ${T1Path}/${AutoSegDir}/ Stripped)"<<std::endl;
	BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/Stripped/)"<<std::endl;

      }
      else 
      {
	BMSAutoSegMainFile<<"      set (stripEMS _stripEMS${StrippedBias})"<<std::endl;
	BMSAutoSegMainFile<<"      ListDirInDir (StrippedList ${T1Path}/${AutoSegDir}/ ems_"<<SuffixIteration_2<<")"<<std::endl;
	BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration_2<<"/)"<<std::endl;
      }
    }
    else
    {
      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
      BMSAutoSegMainFile<<"         set  (SUFFIX EMS)"<<std::endl;
      BMSAutoSegMainFile<<"         set (SuffixCorrected _corrected_${SUFFIX})"<<std::endl;
      BMSAutoSegMainFile<<"         set (SuffixLabel ${NEOSEG_PREFIX}_labels_${SUFFIX})"<<std::endl;
      if (GetLoop())
      {
	BMSAutoSegMainFile<<"      set (stripEMS _stripEMS)"<<std::endl;
	BMSAutoSegMainFile<<"      ListDirInDir (StrippedList ${T1Path}/${AutoSegDir}/ ems_"<<SuffixIteration_2<<")"<<std::endl;
	BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration_2<<"/)"<<std::endl;
      }
      else	
      {
	BMSAutoSegMainFile<<"      set (InputPath ${EMSPath}/)"<<std::endl;
	BMSAutoSegMainFile<<"      ListDirInDir (StrippedList ${T1Path}/${AutoSegDir}/ Stripped)"<<std::endl;
	BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/Stripped/)"<<std::endl;
      }
    }

    BMSAutoSegMainFile<<"      # Creating directory if necessary"<<std::endl;
    BMSAutoSegMainFile<<"      If (${StrippedList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory(${StrippedPath})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${StrippedList})"<<std::endl;


    BMSAutoSegMainFile<<"      ListFileInDir(FinalTargetList ${StrippedPath} ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;

    BMSAutoSegMainFile<<"      If (${FinalTargetList} == '')"<<std::endl;
	
    if (IsT1LabelEMSFile)
      BMSAutoSegMainFile<<"        set (SegmentedCase ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl; 	
    else
    {
      if (GetT2Image()) {
	BMSAutoSegMainFile<<"          GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	BMSAutoSegMainFile<<"          GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegMainFile<<"          set (SegmentedCase ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
      }
    }
    BMSAutoSegMainFile<<"        GetFilename (SegmentedCaseHead ${SegmentedCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
	

    BMSAutoSegMainFile<<"            set (TmpMaskTail ${SegmentedCaseHead}tmp1.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"            set (FinalMaskTail ${SegmentedCaseHead}_mask.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"            set (FinalTargetTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;

    if ((GetLoop() && iteration==finalLoop) || (!GetLoop()))
      BMSAutoSegMainFile<<"            set (CurrentCaseTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    else
       BMSAutoSegMainFile<<"            set (CurrentCaseTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}.nrrd)"<<std::endl;  

    BMSAutoSegMainFile<<"            set (CurrentCase ${InputPath}${CurrentCaseTail})"<<std::endl;
    BMSAutoSegMainFile<<"            set (FinalTarget ${StrippedPath}${FinalTargetTail})"<<std::endl;
    BMSAutoSegMainFile<<"            set (TmpMask ${StrippedPath}${TmpMaskTail})"<<std::endl;
    BMSAutoSegMainFile<<"            set (FinalMask ${StrippedPath}${FinalMaskTail})"<<std::endl;

    BMSAutoSegMainFile<<"            ListFileInDir (FinalMaskList ${FinalMask})"<<std::endl;
    BMSAutoSegMainFile<<"            If (${FinalMaskList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"              # Creating the binary mask"<<std::endl;  
    if (std::strcmp(GetEMSoftware(), "ABC") == 0) {
      BMSAutoSegMainFile<<"              set (command_line ${ImageMathCmd} ${SegmentedCase} -threshold 1,10000 -outfile ${TmpMask})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl; 
    } else {
      BMSAutoSegMainFile<<"              set (command_line ${ImageMathCmd} ${SegmentedCase} -threshold 1,4 -outfile ${TmpMask})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl; 
    }
    BMSAutoSegMainFile<<"	         #In order to make sur that the binary mask is fine (without holes and smoothed)"<<std::endl;
	
    if (GetDeleteVessels())
    {
      BMSAutoSegMainFile<<"              set (command_line ${SegPostProcessCmd} ${TmpMask} ${FinalTarget} --skullstripping ${CurrentCase} --mask ${FinalMask} --deleteVessels)"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl; 

      BMSAutoSegMainFile<<"              set (command_line ${ImageMathCmd} ${FinalTarget} -constOper 2,1 -outfile ${FinalTarget})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl; 
    }
    else
    {
      BMSAutoSegMainFile<<"              set (command_line ${SegPostProcessCmd} ${TmpMask} ${FinalTarget} --skullstripping ${CurrentCase} --mask ${FinalMask})"<<std::endl;	
      BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl; 
      BMSAutoSegMainFile<<"              set (command_line ${ImageMathCmd} ${FinalTarget} -constOper 2,1 -outfile ${FinalTarget})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl; 
    }


    BMSAutoSegMainFile<<"            Else ()"<<std::endl;
    BMSAutoSegMainFile<<"              echo ('Binary mask already exists!')"<<std::endl;
    BMSAutoSegMainFile<<"            EndIf (${FinalMaskList})"<<std::endl;

    if (GetT2Image())
    {
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Skull stripping T2w image...')"<<std::endl;
      BMSAutoSegMainFile<<"      GetParam (T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      ListFileInDir(T2FinalTargetList ${StrippedPath} ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${T2FinalTargetList} == '')"<<std::endl;

      if ((GetLoop() && iteration==finalLoop) || (!GetLoop()))
        BMSAutoSegMainFile<<"        set (T2CurrentCaseTail ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
      else
	BMSAutoSegMainFile<<"        set (T2CurrentCaseTail ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"        set (T2CurrentCase ${InputPath}${T2CurrentCaseTail})"<<std::endl;
      BMSAutoSegMainFile<<"        set (T2FinalTargetTail ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"        set (T2FinalTarget ${StrippedPath}${T2FinalTargetTail})"<<std::endl;

      BMSAutoSegMainFile<<"        set (command_line ${ImageMathCmd} ${T2CurrentCase} -mask ${FinalMask} -outfile ${T2FinalTarget})"<<std::endl; 
      BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
      BMSAutoSegMainFile<<"      Else ()"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Stripped Image already exists!')"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${T2FinalTargetList})"<<std::endl;
    }
    if (GetPDImage())
    {
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Skull stripping PDw image...')"<<std::endl;
      BMSAutoSegMainFile<<"      GetParam (PDCase ${PDCasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (PDCaseHead ${PDCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      ListFileInDir(PDFinalTargetList ${StrippedPath} ${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${PDFinalTargetList} == '')"<<std::endl;
      if ((GetLoop() && iteration==finalLoop) || (!GetLoop()))
        BMSAutoSegMainFile<<"        set (PDCurrentCaseTail ${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
      else
	BMSAutoSegMainFile<<"        set (PDCurrentCaseTail ${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"        set (PDCurrentCase ${InputPath}${PDCurrentCaseTail})"<<std::endl;
      BMSAutoSegMainFile<<"        set (PDFinalTargetTail ${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"        set (PDFinalTarget ${StrippedPath}${PDFinalTargetTail})"<<std::endl;
      BMSAutoSegMainFile<<"        set (command_line ${ImageMathCmd} ${PDCurrentCase} -mask ${FinalMask} -outfile ${PDFinalTarget})"<<std::endl; 
      BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
      BMSAutoSegMainFile<<"      Else ()"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Stripped Image already exists!')"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${PDFinalTargetList})"<<std::endl;
    }
    BMSAutoSegMainFile<<"            # Deleting temporary file"<<std::endl;
    BMSAutoSegMainFile<<"	    DeleteFile (${TmpMask})"<<std::endl<<std::endl;

    // optional Bias Field correction on stripped data
    if (GetStrippedN4ITKBiasFieldCorrection() && GetLoop() && iteration !=finalLoop)
    {
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Bias field correction...')"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      set(StrippedBias _${Bias})"<<std::endl;

      BMSAutoSegMainFile<<"	 Set (my_output ${StrippedPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${StrippedBias}.nrrd)"<<std::endl;
     
      if(GetSlicerVersion() < 4.3) {
        BMSAutoSegMainFile<<"    Set (command_line ${N4Cmd} --outputimage ${my_output} --inputimage ${FinalTarget} ${SlicerN4parameters})"<<std::endl;
      }
      else { // >= 4.3
        BMSAutoSegMainFile<<"    Set (command_line ${N4Cmd}  ${FinalTarget} ${my_output} ${SlicerN4parameters})"<<std::endl;
      }

      BMSAutoSegMainFile<<"      Run (prog_output ${command_line} prog_error)"<<std::endl;
      BMSAutoSegMainFile<<"      Set (FinalTarget ${my_output})"<<std::endl;
	
      if (GetT2Image())
      {
	BMSAutoSegMainFile<<"	 Set (my_output ${StrippedPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}${StrippedBias}.nrrd)"<<std::endl;

	if(GetSlicerVersion() < 4.3) {
	  BMSAutoSegMainFile<<"    Set (command_line ${N4Cmd} --outputimage ${my_output} --inputimage ${T2FinalTarget} ${SlicerN4parameters})"<<std::endl;
	}
	else { // >= 4.3
	  BMSAutoSegMainFile<<"    Set (command_line ${N4Cmd}  ${T2FinalTarget} ${my_output} ${SlicerN4parameters})"<<std::endl;
	}
	BMSAutoSegMainFile<<"    Run (prog_output ${command_line} prog_error)"<<std::endl;	
      }
      if (GetPDImage())
      {
	BMSAutoSegMainFile<<"	 Set (my_output ${StrippedPath}${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${stripEMS}${StrippedBias}.nrrd)"<<std::endl;
	if(GetSlicerVersion() < 4.3) {
	  BMSAutoSegMainFile<<"    Set (command_line ${N4Cmd} --outputimage ${my_output} --inputimage ${PDFinalTarget} ${SlicerN4parameters})"<<std::endl;
	}
	else { // >= 4.3
	  BMSAutoSegMainFile<<"    Set (command_line ${N4Cmd}  ${PDFinalTarget} ${my_output} ${SlicerN4parameters})"<<std::endl;
	}
	BMSAutoSegMainFile<<"    Run (prog_output ${command_line} prog_error)"<<std::endl;
      }		
    }

    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Stripped Image already exists!')"<<std::endl;
    if (GetT2Image())
    {
      BMSAutoSegMainFile<<"        GetParam (T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      ListFileInDir(T2FinalTargetList ${StrippedPath} ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${T2FinalTargetList} != '')"<<std::endl;
      BMSAutoSegMainFile<<"         echo( )"<<std::endl;
      BMSAutoSegMainFile<<"         echo('T2 Case Number: '${T2CaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"         echo( )"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Stripped Image already exists!')"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${T2FinalTargetList})"<<std::endl;
    }
    if (GetPDImage())
    {
      BMSAutoSegMainFile<<"        GetParam (PDCase ${PDCasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (PDCaseHead ${PDCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      ListFileInDir(PDFinalTargetList ${StrippedPath} ${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${PDFinalTargetList} != '')"<<std::endl;
      BMSAutoSegMainFile<<"         echo( )"<<std::endl;
      BMSAutoSegMainFile<<"         echo('Case Number: '${PDCaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"         echo( )"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Stripped Image already exists!')"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${PDFinalTargetList})"<<std::endl;
    }
    BMSAutoSegMainFile<<"      EndIf (${FinalTargetList})"<<std::endl;
    BMSAutoSegMainFile<<"Inc(${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMainFile<<"Int(${CaseNumber})"<<std::endl; 

    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;

    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('SKULL STRIPPING: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
    if (GetLoop())
      iteration++;

  }while (GetLoop() && iteration<=GetLoopIteration());



  // Warping performed via ANTS: no intensity rescaling needed.
  if (GetANTSWarpingMethod() && GetSingleAtlasSegmentation())
  {    

    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 8. 9. Atlas warping via ANTS to the skull stripped image"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  
    BMSAutoSegMainFile<<"set (IRescaled '')"<<std::endl;

    //BMSAutoSegMainFile<<"set (ANTSIterations "<<GetANTSIterations()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSCCWeight "<<GetANTSCCWeight()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSCCRegionRadius "<<GetANTSCCRegionRadius()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSMIWeight "<<GetANTSMIWeight()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSMIBins "<<GetANTSMIBins()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSMSQWeight "<<GetANTSMSQWeight()<<")"<<std::endl;

    BMSAutoSegMainFile<<"set (ANTSCCWeight2nd "<<GetANTSCCWeight2nd()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSCCRegionRadius2nd "<<GetANTSCCRegionRadius2nd()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSMIWeight2nd "<<GetANTSMIWeight2nd()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSMIBins2nd "<<GetANTSMIBins2nd()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSMSQWeight2nd "<<GetANTSMSQWeight2nd()<<")"<<std::endl;

    BMSAutoSegMainFile<<"set (ANTSTransformationStep "<<GetANTSTransformationStep()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (ANTSGaussianSigma "<<GetANTSGaussianSigma()<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (CaseNumber 0)"<<std::endl;

    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('ATLAS WARPING...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;    
    BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/Stripped/)"<<std::endl;
    BMSAutoSegMainFile<<"      set (StrippedCaseHead ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS})"<<std::endl;
    BMSAutoSegMainFile<<"      set (StrippedCaseTail ${StrippedCaseHead}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      set (StrippedCase ${StrippedPath}${StrippedCaseTail})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (StrippedCaseHead ${StrippedCase} NAME_WITHOUT_EXTENSION)"<<std::endl;

    BMSAutoSegMainFile<<"      # Creating WarpROI Directory if necessary"<<std::endl;
    BMSAutoSegMainFile<<"      set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
    BMSAutoSegMainFile<<"      ListDirInDir (WarpROIList ${T1Path}/${AutoSegDir}/ WarpROI)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${WarpROIList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory( ${WarpROIPath})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${WarpROIList})"<<std::endl;
    
    // for T2
    if (GetT2Image()) {
      BMSAutoSegMainFile<<"      GetParam (T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T2Path ${T2Case} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Case Number: '${T2CaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;    
      BMSAutoSegMainFile<<"      set (T2StrippedPath ${T2Path}/${AutoSegDir}/Stripped/)"<<std::endl;
      BMSAutoSegMainFile<<"      set (T2StrippedCaseHead ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS})"<<std::endl;
      BMSAutoSegMainFile<<"      set (T2StrippedCaseTail ${T2StrippedCaseHead}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      set (T2StrippedCase ${T2StrippedPath}${T2StrippedCaseTail})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T2StrippedCaseHead ${T2StrippedCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    }

    BMSAutoSegMainFile<<"Inc(${CaseNumber} 1)"<<std::endl;

    BMSAutoSegMainFile<<"      ListFileInDir(DeformationFieldList ${WarpROIPath} AtlasWarpReg-${StrippedCaseHead}_Warp.nii.gz)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${DeformationFieldList} == '')"<<std::endl;        
    BMSAutoSegMainFile<<"         echo ('Computing deformation field...')"<<std::endl;  
    //if (GetROIT2Atlas()) {
    if (GetMultiModalitySingleSegmentation()) {
        BMSAutoSegMainFile<<"    set (command_line ${ANTSCmd} 3)"<<std::endl;
        if (GetANTSCCWeight() > 0.01)
            BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m CC[${StrippedCase},${atlasROIFile},${ANTSCCWeight},${ANTSCCRegionRadius}])"<<std::endl;
        if (GetANTSCCWeight2nd() > 0.01)
            BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m CC[${T2StrippedCase},${atlasROIT2File},${ANTSCCWeight2nd},${ANTSCCRegionRadius2nd}])"<<std::endl;
        if (GetANTSMIWeight() > 0.01)
            BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MI[${StrippedCase},${atlasROIFile},${ANTSMIWeight},${ANTSMIBins}])"<<std::endl;
        if (GetANTSMIWeight2nd() > 0.01)
            BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MI[${T2StrippedCase},${atlasROIT2File},${ANTSMIWeight2nd},${ANTSMIBins2nd}])"<<std::endl;
        if (GetANTSMSQWeight() > 0.01)
            BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MSQ[${StrippedCase},${atlasROIFile},${ANTSMSQWeight},0.01])"<<std::endl;
        if (GetANTSMSQWeight2nd() > 0.01)
            BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MSQ[${T2StrippedCase},${atlasROIT2File},${ANTSMSQWeight2nd},0.01])"<<std::endl;
    }
    else {
        BMSAutoSegMainFile<<"    set (command_line ${ANTSCmd} 3)"<<std::endl;
        if (GetANTSCCWeight() > 0.01)
            BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m CC[${StrippedCase},${atlasROIFile},${ANTSCCWeight},${ANTSCCRegionRadius}])"<<std::endl;
        if (GetANTSMIWeight() > 0.01)
            BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MI[${StrippedCase},${atlasROIFile},${ANTSMIWeight},${ANTSMIBins}])"<<std::endl;
        if (GetANTSMSQWeight() > 0.01)
            BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MSQ[${StrippedCase},${atlasROIFile},${ANTSMSQWeight},0.01])"<<std::endl;
    }
    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -i ${ANTSIterations} -o ${WarpROIPath}AtlasWarpReg-${StrippedCaseHead}_)"<<std::endl;
    if (std::strcmp(GetANTSRegistrationFilterType(), "GreedyDiffeomorphism") == 0)
      {
	BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t SyN[${ANTSTransformationStep}])"<<std::endl;
	if (GetANTSGaussianSmoothing())
	  BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[${ANTSGaussianSigma},0])"<<std::endl;      
      }
    else if (std::strcmp(GetANTSRegistrationFilterType(), "SpatiotemporalDiffeomorphism") == 0)
      {
	BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t SyN[${ANTSTransformationStep}])"<<std::endl;
	if (GetANTSGaussianSmoothing())
	  BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[${ANTSGaussianSigma},0.5])"<<std::endl;      
      }
    else if (std::strcmp(GetANTSRegistrationFilterType(), "Elastic") == 0)
      {
	BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t Elast[${ANTSTransformationStep}])"<<std::endl;
	if (GetANTSGaussianSmoothing())
	  BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[0,${ANTSGaussianSigma}])"<<std::endl;
      }
    else if (std::strcmp(GetANTSRegistrationFilterType(), "Exponential") == 0)
      {
	BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t Exp[${ANTSTransformationStep}])"<<std::endl;
	if (GetANTSGaussianSmoothing())
	  BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[0,${ANTSGaussianSigma}])"<<std::endl;
      }
    BMSAutoSegMainFile<<"    	set (command_line ${command_line} --use-all-metrics-for-convergence)"<<std::endl;
    BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"      	set(ANTSLogFile ${WarpROIPath}AtlasWarpReg-${WarpROIPath}${StrippedCaseHead}_ANTS.log)"<<std::endl;
    BMSAutoSegMainFile<<"      	WriteFile(${ANTSLogFile} ${prog_output})"<<std::endl;
    BMSAutoSegMainFile<<"      	If(${prog_error} !=  '')"<<std::endl;
    BMSAutoSegMainFile<<"      	  echo('ANTS Error: '${prog_error})"<<std::endl;
    BMSAutoSegMainFile<<"      	EndIf(${prog_error})"<<std::endl;
    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Registration already Done!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${DeformationFieldList})"<<std::endl;

    BMSAutoSegMainFile<<"      ListFileInDir(OutputFileList ${WarpROIPath} AtlasWarpReg-${StrippedCaseHead}_warp.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${OutputFileList} == '')"<<std::endl;        
    BMSAutoSegMainFile<<"        echo ('Applying deformation to grayscale image...')"<<std::endl;

    BMSAutoSegMainFile<<"        set (WarpedAtlas ${WarpROIPath}AtlasWarpReg-${StrippedCaseHead}_warp.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"        set (DeformationField ${WarpROIPath}AtlasWarpReg-${StrippedCaseHead}_Warp.nii.gz)"<<std::endl;
    BMSAutoSegMainFile<<"        set (AffineTransform ${WarpROIPath}AtlasWarpReg-${StrippedCaseHead}_Affine.txt)"<<std::endl;
    BMSAutoSegMainFile<<"        set (command_line ${WarpImageMultiTransformCmd} 3 ${atlasROIFile} ${WarpedAtlas} -R ${StrippedCase} --use-BSpline ${DeformationField} ${AffineTransform})"<<std::endl;

    BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"      	If(${prog_error} !=  '')"<<std::endl;
    BMSAutoSegMainFile<<"      	  echo('WarpImageMultiTransform Error: '${prog_error})"<<std::endl;
    BMSAutoSegMainFile<<"      	EndIf(${prog_error})"<<std::endl;

  // Extra step to compress the nrrd images (not done automatically with WarpImageMultiTransform...)
	BMSAutoSegMainFile<<"       set (command_line ${ImageMathCmd} ${WarpedAtlas} -constOper 2,1 -outfile ${WarpedAtlas})"<<std::endl;
 	BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;	
	BMSAutoSegMainFile<<"      	If(${prog_error} !=  '')"<<std::endl;
	BMSAutoSegMainFile<<"      	  echo('ImageMath Error: '${prog_error})"<<std::endl;
	BMSAutoSegMainFile<<"      	EndIf(${prog_error})"<<std::endl; 

    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Warping already Done!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${OutputFileList})"<<std::endl;

    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WARPING COMPUTATION: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }
  // Registration performed via fWarp or BRAINSDemonWarp -> intensity rescaling, affine registration via BRAINSFit, then warping.
  // only if single atlas segmentation is requested
  else if (GetSingleAtlasSegmentation()) {

    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 7. Intensity Calibration"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  
    BMSAutoSegMainFile<<"set (IRescaled '-irescaled')"<<std::endl;

    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo (INTENSITY CALIBRATION)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList}) "<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;    
    BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/Stripped/)"<<std::endl;
    BMSAutoSegMainFile<<"      set (StrippedCase ${StrippedPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (StrippedCaseHead ${StrippedCase} NAME_WITHOUT_EXTENSION)"<<std::endl;  
    BMSAutoSegMainFile<<"      # Output File"<<std::endl;
    BMSAutoSegMainFile<<"      set (OutputFile ${StrippedPath}${StrippedCaseHead}${IRescaled}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      set (OutputFileTail ${StrippedCaseHead}${IRescaled}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      ListFileInDir(OutputFileList ${StrippedPath} ${StrippedCaseHead}${IRescaled}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${OutputFileList} == '')"<<std::endl;

    // Intensity rescaling based on histogram quantile matching (ImageMath)
    if (GetIntensityRescalingMethod() == 1)
      {
	
	BMSAutoSegMainFile<<"          set (command_line ${ImageMathCmd} ${StrippedCase} -matchHistogram ${atlasROIFile} -outfile ${OutputFile})"<<std::endl;
	BMSAutoSegMainFile<<"          Run (output ${command_line})"<<std::endl;
      }
    // Intensity rescaling based on tissue mean matching (Intensity Rescaler)
    else
      {
	BMSAutoSegMainFile<<"         GetFilename (atlasROIFileHead ${atlasROIFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegMainFile<<"         GetFilename (atlasROIFilePath ${atlasROIFile} PATH)"<<std::endl;
	BMSAutoSegMainFile<<"         set (atlasROIEMSFile ${atlasROIFilePath}/${atlasROIFileHead}${SuffixLabel}.nrrd)"<<std::endl;
	
	if(GetLoop())
	  BMSAutoSegMainFile<<"         set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
	else
	  BMSAutoSegMainFile<<"         set (EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
	
	if (IsT1LabelEMSFile)
	  BMSAutoSegMainFile<<"        set (SegmentedCase ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
	else
	  {
	    BMSAutoSegMainFile<<"          GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	    BMSAutoSegMainFile<<"          GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	    BMSAutoSegMainFile<<"        set (SegmentedCase ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
	  }

	BMSAutoSegMainFile<<"         set (IntRescalerParameterFile ${StrippedPath}${StrippedCaseHead}_intRescale.txt)"<<std::endl;
	BMSAutoSegMainFile<<"         ListFileInDir(IntRescalerParameterFileList ${StrippedPath} ${StrippedCaseHead}_intRescale.txt)"<<std::endl;
	BMSAutoSegMainFile<<"         If (${IntRescalerParameterFileList} == '')"<<std::endl;
	
	BMSAutoSegMainFile<<"            # create Intensity Rescaler parameter file"<<std::endl;
	BMSAutoSegMainFile<<"	           WriteFile (${IntRescalerParameterFile} '# Target image: One image which is the reference\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'Target='${atlasROIFile}'\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# Target segmentation image: EM Segmentation of the target image\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'TargetEMS='${atlasROIEMSFile}'\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# Source image: Image to be intensity rescaled base on reference image\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# Source segmentation image: EM Segmentation image\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'Source='${StrippedCase}'\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'SourceEMS='${SegmentedCase}'\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# Label List: Usually 1,2 and 3 are the labels for White, Gray and CSF pattern\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'Label=1\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'Label=2\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'Label=3\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# Target intensity windowing: Do you want to adjust min/max intensity of the target ? [ON/OFF]\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'TargetWindowing=ON\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# Source intensity windowing: Do you want to adjust min/max intensity of source image ? [ON/OFF]\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'SourceWindowing=ON\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# Class matching: Do you want to iteratively adjust classes ? [ON/OFF]\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'ClassMatching=ON\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# Sigma for class matching: Standard deviation for min/max adjustment\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'Sigma=3\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# OutputSuffix: Suffix for output filename\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'OutputSuffix=${IRescaled}\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# OutputDir: Output Directory\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'OutputDir='${StrippedPath}'\\n')"<<std::endl;
	BMSAutoSegMainFile<<"         EndIf (${IntRescalerParameterFileList})"<<std::endl;
	
	
	BMSAutoSegMainFile<<"              set (command_line ${IntensityRescalerCmd} -i ${IntRescalerParameterFile} -v)"<<std::endl;
	BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl;
      }
    BMSAutoSegMainFile<<"          If(${IsAtlasROIFileZipped} == 1)"<<std::endl;
    BMSAutoSegMainFile<<"              set (command_line gzip ${atlasROIFile})"<<std::endl;
    BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl;
    BMSAutoSegMainFile<<"             set (atlasROIFile ${atlasROIFile}.gz)"<<std::endl;
    BMSAutoSegMainFile<<"          Endif(${IsAtlasROIFileZipped})"<<std::endl;


    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"          echo ('Intensity Calibration already done!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${OutputFileList})"<<std::endl;
    
    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('INTENSITY CALIBRATION DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;

    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 8. Affine Registration : register the atlas to the skull stripped image"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('AFFINE REGISTRATION...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;
    BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/Stripped/)"<<std::endl;
    BMSAutoSegMainFile<<"      set (StrippedCaseTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      set (StrippedCase ${StrippedPath}${StrippedCaseTail})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (StrippedCaseHead ${StrippedCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    
    BMSAutoSegMainFile<<"      # Creating WarpROI Directory if necessary"<<std::endl;
    BMSAutoSegMainFile<<"      set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
    BMSAutoSegMainFile<<"      ListDirInDir (WarpROIList ${T1Path}/${AutoSegDir}/ WarpROI)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${WarpROIList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory( ${WarpROIPath})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${WarpROIList})"<<std::endl;
    BMSAutoSegMainFile<<"      ListFileInDir(OutputFileList ${WarpROIPath} AtlasAffReg-${StrippedCaseTail})"<<std::endl;
    BMSAutoSegMainFile<<"      If (${OutputFileList} == '')"<<std::endl;
    
    BMSAutoSegMainFile<<"         # Creating new Files"<<std::endl;
    BMSAutoSegMainFile<<"          # Output File"<<std::endl;
    BMSAutoSegMainFile<<"         set (OutputFileHead AtlasAffReg-${StrippedCaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"         set (OutputFileTail ${OutputFileHead}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"         set (OutputFile ${WarpROIPath}${OutputFileTail})"<<std::endl;

    BMSAutoSegMainFile<<"            set (TransformInputFile ${WarpROIPath}${OutputFileHead}_initializetransform.txt)"<<std::endl;
    BMSAutoSegMainFile<<"    	set (command_line ${BRAINSFitCmd} --fixedVolume ${StrippedCase} --movingVolume ${atlasROIFile} --transformType Rigid --initializeTransformMode ${RegistrationInitialization} --useAffine --outputVolume ${OutputFile} --outputTransform ${TransformInputFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
    BMSAutoSegMainFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Registration already Done!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${OutputFileList})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('AFFINE REGISTRATION: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;


    BMSAutoSegMainFile<<"#-------------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#-------------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#-------------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 9. Warping Procedure:  Warping the Atlas to the skull stripping T1 image"<<std::endl;
    BMSAutoSegMainFile<<"#-------------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#-------------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#-------------------------------------------------------------------------"<<std::endl;
    
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WARPING COMPUTATION...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;      
    BMSAutoSegMainFile<<"      set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
    BMSAutoSegMainFile<<"      set (AtlasAffRegCase ${WarpROIPath}AtlasAffReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}.nrrd)" <<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (AtlasAffRegCaseHead ${AtlasAffRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    
    BMSAutoSegMainFile<<"      ListFileInDir(OutputFileList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}.nrrd)" <<std::endl;
    BMSAutoSegMainFile<<"      If (${OutputFileList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         # Creating new Files"<<std::endl;
    BMSAutoSegMainFile<<"          # Output File"<<std::endl;
    BMSAutoSegMainFile<<"         set (OutputFileHead AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled})"<<std::endl;
    BMSAutoSegMainFile<<"         set (OutputFileTail ${OutputFileHead}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"         set (OutputFile ${WarpROIPath}${OutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"          # Report File (process)"<<std::endl;
    BMSAutoSegMainFile<<"         set (ReportFile ${WarpROIPath}${OutputFileHead}_out.txt)"<<std::endl;
    
    if (GetClassicWarpingMethod())
      {
	BMSAutoSegMainFile<<"         ListFileInDir (SkullStrippedList ${T1Path}/${AutoSegDir}/Stripped/ ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}.nrrd)"<<std::endl;
	BMSAutoSegMainFile<<"         If (${SkullStrippedList} != '')"<<std::endl;
	BMSAutoSegMainFile<<"            set (SkullStrippedFileTail ${SkullStrippedList})"<<std::endl;
	BMSAutoSegMainFile<<"         Else ()"<<std::endl;
	BMSAutoSegMainFile<<"            echo ('Error:No Skull Stripped File!')"<<std::endl;		
	BMSAutoSegMainFile<<"         EndIf (${SkullStrippedList})"<<std::endl;
	BMSAutoSegMainFile<<"         set (SkullStrippedFile ${T1Path}/${AutoSegDir}/Stripped/${SkullStrippedFileTail})"<<std::endl;
	
	BMSAutoSegMainFile<<"         set (DofOutWarpFileTail ${OutputFileHead}_warp.hfield)"<<std::endl;
	BMSAutoSegMainFile<<"         set (DofOutWarpFile ${WarpROIPath}${DofOutWarpFileTail})"<<std::endl;
	BMSAutoSegMainFile<<"         ListFileInDir(DofOutWarpFileList ${WarpROIPath} ${DofOutWarpFileTail}.gz)"<<std::endl;
	BMSAutoSegMainFile<<"         If (${DofOutWarpFileList} == '')"<<std::endl;
	
	BMSAutoSegMainFile<<"            echo ('Computing transformation...')"<<std::endl;
	BMSAutoSegMainFile<<"            set (Alpha "<<GetAlpha()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (Beta "<<GetBeta()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (Gamma "<<GetGamma()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (MaxPerturbation "<<GetMaxPerturbation()<<")"<<std::endl;  
	BMSAutoSegMainFile<<"            set (NumBasis "<<GetNumBasis()<<")"<<std::endl;
	
	BMSAutoSegMainFile<<"            set (NumIteration "<<GetScale1NbIterations()<<")"<<std::endl;
	
	BMSAutoSegMainFile<<"            set (command_line ${warpCmd} -c ${AtlasAffRegCase} ${SkullStrippedFile} ${DofOutWarpFile} ${Alpha} ${Beta} ${Gamma} ${MaxPerturbation} ${NumIteration} ${NumBasis})"<<std::endl;
	BMSAutoSegMainFile<<"            Run (output ${command_line})"<<std::endl;
	BMSAutoSegMainFile<<"            WriteFile(${ReportFile} ${output})"<<std::endl<<std::endl;
	BMSAutoSegMainFile<<"         EndIf (${DofOutWarpFileList})"<<std::endl;
	
	BMSAutoSegMainFile<<"         # Applying Transformation"<<std::endl;
	BMSAutoSegMainFile<<"         # Transforming all volumes, by default uses cspline interpolation"<<std::endl;
	BMSAutoSegMainFile<<"            echo ('Applying transformation...')"<<std::endl;
	
	BMSAutoSegMainFile<<"         set (command_line ${warpCmd} -a ${AtlasAffRegCase} ${DofOutWarpFile} ${OutputFile} -linear)"<<std::endl;
	BMSAutoSegMainFile<<"         Run (output ${command_line})"<<std::endl;
	BMSAutoSegMainFile<<"         AppendFile(${ReportFile} ${output})"<<std::endl<<std::endl;
	
	BMSAutoSegMainFile<<"         set (command_line gzip ${DofOutWarpFile})"<<std::endl;
	BMSAutoSegMainFile<<"         Run (output ${command_line})"<<std::endl;
      }
    else if (GetBRAINSDemonWarpMethod())
      {
	BMSAutoSegMainFile<<"            ListFileInDir (SkullStrippedList ${T1Path}/${AutoSegDir}/Stripped/ ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}.nrrd)"<<std::endl;
	BMSAutoSegMainFile<<"            If (${SkullStrippedList} != '')"<<std::endl;
	BMSAutoSegMainFile<<"               set (SkullStrippedFileTail ${SkullStrippedList})"<<std::endl;
	BMSAutoSegMainFile<<"            Else ()"<<std::endl;
	BMSAutoSegMainFile<<"               echo ('Error:No Skull Stripped File!')"<<std::endl;
	BMSAutoSegMainFile<<"            EndIf (${SkullStrippedList})"<<std::endl;
	BMSAutoSegMainFile<<"            set (SkullStrippedFile ${T1Path}/${AutoSegDir}/Stripped/${SkullStrippedFileTail})"<<std::endl;
	
	BMSAutoSegMainFile<<"            echo ('Computing transformation...')"<<std::endl;
	BMSAutoSegMainFile<<"            set (RegistrationFilterType "<<GetRegistrationFilterType()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (DeformationFieldSmoothingSigma "<<GetDeformationFieldSmoothingSigma()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (PyramidLevels "<<GetPyramidLevels()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (MovingShrinkFactors "<<GetMovingShrinkFactors()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (FixedShrinkFactors "<<GetFixedShrinkFactors()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (IterationCountPyramidLevels "<<GetIterationCountPyramidLevels()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (InterpolationMode BSpline)"<<std::endl;
	BMSAutoSegMainFile<<"            set (TransformOutputFile ${WarpROIPath}${OutputFileHead}_transform.nrrd)"<<std::endl;
	BMSAutoSegMainFile<<"            set (TransformInputFile ${WarpROIPath}${AtlasAffRegCaseHead}_initializetransform.txt)"<<std::endl;
	BMSAutoSegMainFile<<"            set (parameters --registrationFilterType ${RegistrationFilterType} -s ${DeformationFieldSmoothingSigma} -n ${PyramidLevels} --minimumMovingPyramid ${MovingShrinkFactors} --minimumFixedPyramid ${FixedShrinkFactors} -i ${IterationCountPyramidLevels} --interpolationMode ${InterpolationMode})"<<std::endl;
	BMSAutoSegMainFile<<"            set (Input -f ${SkullStrippedFile} -m ${atlasROIFile} --inputPixelType short --initializeWithTransform ${TransformInputFile})"<<std::endl;
	BMSAutoSegMainFile<<"            set (Output -o ${OutputFile} -O ${TransformOutputFile} --outputPixelType short)"<<std::endl;
	BMSAutoSegMainFile<<"    	set (command_line ${BRAINSDemonWarpCmd} ${Input} ${Output} ${parameters})"<<std::endl;
	BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;
	BMSAutoSegMainFile<<"    	set (command_line ${ResampleVolume2Cmd} ${atlasROIFile} ${OutputFile} --hfieldtype displacement -i bs -H ${TransformOutputFile} --Reference ${SkullStrippedFile})"<<std::endl;
	BMSAutoSegMainFile<<"      	Run (prog_output_2 ${command_line} prog_error)"<<std::endl;
      }
    else if (GetCoarseToFineWarpingMethod())
      {
	
	BMSAutoSegMainFile<<"            ListFileInDir (SkullStrippedList ${T1Path}/${AutoSegDir}/Stripped/ ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}.nrrd)"<<std::endl;
	BMSAutoSegMainFile<<"            If (${SkullStrippedList} != '')"<<std::endl;
	BMSAutoSegMainFile<<"               set (SkullStrippedFileTail ${SkullStrippedList})"<<std::endl;
	BMSAutoSegMainFile<<"            Else ()"<<std::endl;
	BMSAutoSegMainFile<<"               echo ('Error:No Skull Stripped File!')"<<std::endl;
	BMSAutoSegMainFile<<"         	 EndIf (${SkullStrippedList})"<<std::endl;
	BMSAutoSegMainFile<<"            set (SkullStrippedFile ${T1Path}/${AutoSegDir}/Stripped/${SkullStrippedFileTail})"<<std::endl;
	
	BMSAutoSegMainFile<<"            set (DofOutWarpFileTail ${OutputFileHead}_deformationfield)"<<std::endl;
	BMSAutoSegMainFile<<"            set (DofOutWarpFile ${WarpROIPath}${DofOutWarpFileTail})"<<std::endl;
	
	BMSAutoSegMainFile<<"            echo ('Computing transformation...')"<<std::endl;
	BMSAutoSegMainFile<<"            set (Alpha "<<GetAlpha()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (Beta "<<GetBeta()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (Gamma "<<GetGamma()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (MaxPerturbation "<<GetMaxPerturbation()<<")"<<std::endl;  
	BMSAutoSegMainFile<<"            set (NumBasis "<<GetNumBasis()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (Scale4NbIterations "<<GetScale4NbIterations()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (Scale2NbIterations "<<GetScale2NbIterations()<<")"<<std::endl;
	BMSAutoSegMainFile<<"            set (Scale1NbIterations "<<GetScale1NbIterations()<<")"<<std::endl;
	BMSAutoSegMainFile<<"    	     set (command_line ${fWarpCmd} --outputImageFilenamePrefix=${WarpROIPath}${OutputFileHead} --outputHFieldFilenamePrefix=${DofOutWarpFile} --scaleLevel=4 --numberOfIterations=${Scale4NbIterations} --alpha=${Alpha} --beta=${Beta} --gamma=${Gamma} --maxPerturbation=${MaxPerturbation} --scaleLevel=2 --numberOfIterations=${Scale2NbIterations} --alpha=${Alpha} --beta=${Beta} --gamma=${Gamma} --maxPerturbation=${MaxPerturbation} --scaleLevel=1 --numberOfIterations=${Scale1NbIterations} --alpha=${Alpha} --beta=${Beta} --gamma=${Gamma} --maxPerturbation=${MaxPerturbation} ${SkullStrippedFile} ${AtlasAffRegCase})"<<std::endl;
	BMSAutoSegMainFile<<"            Run (output ${command_line})"<<std::endl;
	BMSAutoSegMainFile<<"            WriteFile(${ReportFile} ${output})"<<std::endl<<std::endl;
	BMSAutoSegMainFile<<"           set (command_line ${ImageMathCmd} ${WarpROIPath}${OutputFileHead}2.mhd -constOper 2,1000 -outfile ${OutputFile})"<<std::endl;
	BMSAutoSegMainFile<<"           Run (output ${command_line})"<<std::endl;
	
	BMSAutoSegMainFile<<"           DeleteFile(${WarpROIPath}${OutputFileHead}2.mhd)"<<std::endl;
	BMSAutoSegMainFile<<"           DeleteFile(${WarpROIPath}${OutputFileHead}2.raw)"<<std::endl;
	BMSAutoSegMainFile<<"           DeleteFile(${WarpROIPath}${OutputFileHead}1.mhd)"<<std::endl;
	BMSAutoSegMainFile<<"           DeleteFile(${WarpROIPath}${OutputFileHead}1.raw)"<<std::endl;
	BMSAutoSegMainFile<<"           DeleteFile(${WarpROIPath}${OutputFileHead}0.mhd)"<<std::endl;
	BMSAutoSegMainFile<<"           DeleteFile(${WarpROIPath}${OutputFileHead}0.raw)"<<std::endl;
      }


    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Warping already Done!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${OutputFileList})"<<std::endl;
    
    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WARPING COMPUTATION: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }

  bool IsStructureListEmpty = true;
  bool IsVentricleListEmpty = true;
  bool IsGenericROIMapListEmpty = true;
  bool IsParcellationMapListEmpty = true;
  bool IsLabelListEmpty = true;

  int StructureListLength = 0;
  int VentricleListLength = 0;
  int GenericROIMapListLength = 0;
  int ParcellationMapListLength = 0;
  if (GetSingleAtlasSegmentation()) 
    {
 
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"# 10. Applying the 2 transformations to the ROIs"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  
  BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegMainFile<<"echo ('ROI TRANSFORMATION...')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    
  BMSAutoSegMainFile<<"set (VentricleListLength 0)"<<std::endl;

  if (GetSubcorticalStructureSegmentation())
  {
    if (std::strlen(GetAmygdalaLeft()) != 0)
    {
      BMSAutoSegMainFile<<"set (StructureList "<<GetAmygdalaLeft()<<")"<<std::endl;
      BMSAutoSegMainFile<<"set (StructureLabelList 1)"<<std::endl;
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetAmygdalaRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set (StructureList "<<GetAmygdalaRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList 2)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (StructureList ${StructureList} "<<GetAmygdalaRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList ${StructureLabelList} 2)"<<std::endl;
      }
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetCaudateLeft()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set (StructureList "<<GetCaudateLeft()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList 3)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (StructureList ${StructureList} "<<GetCaudateLeft()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList ${StructureLabelList} 3)"<<std::endl;
      }
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetCaudateRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set (StructureList "<<GetCaudateRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList 4)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (StructureList ${StructureList} "<<GetCaudateRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList ${StructureLabelList} 4)"<<std::endl;
      }
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetHippocampusLeft()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set (StructureList "<<GetHippocampusLeft()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList 5)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (StructureList ${StructureList} "<<GetHippocampusLeft()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList ${StructureLabelList} 5)"<<std::endl;
      }
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetHippocampusRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set (StructureList "<<GetHippocampusRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList 6)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (StructureList ${StructureList} "<<GetHippocampusRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList ${StructureLabelList} 6)"<<std::endl;
      }
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetPallidusLeft()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set (StructureList "<<GetPallidusLeft()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList 9)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (StructureList ${StructureList} "<<GetPallidusLeft()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList ${StructureLabelList} 9)"<<std::endl;
      }
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetPallidusRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set (StructureList "<<GetPallidusRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList 10)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (StructureList ${StructureList} "<<GetPallidusRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList ${StructureLabelList} 10)"<<std::endl;
      }
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetPutamenLeft()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set (StructureList "<<GetPutamenLeft()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList 11)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (StructureList ${StructureList} "<<GetPutamenLeft()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList ${StructureLabelList} 11)"<<std::endl;
      }
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetPutamenRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set (StructureList "<<GetPutamenRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList 12)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (StructureList ${StructureList} "<<GetPutamenRight()<<")"<<std::endl;
	BMSAutoSegMainFile<<"set (StructureLabelList ${StructureLabelList} 12)"<<std::endl;
      }
      IsStructureListEmpty = false;
      StructureListLength ++;
    }
    if (std::strlen(GetLateralVentricleLeft()) != 0)
    {
      BMSAutoSegMainFile<<"set (LeftVentricle "<<GetLateralVentricleLeft()<<")"<<std::endl;
      BMSAutoSegMainFile<<"set (VentricleList ${LeftVentricle})"<<std::endl;
      BMSAutoSegMainFile<<"set (VentricleLabelList 7)"<<std::endl;
      BMSAutoSegMainFile<<"GetFilename(LeftVentricleName ${LeftVentricle} NAME_WITHOUT_EXTENSION)"<<std::endl; 	  
      IsVentricleListEmpty = false;
      VentricleListLength ++;
    }
    if (std::strlen(GetLateralVentricleRight()) != 0)
    {
      BMSAutoSegMainFile<<"set (RightVentricle "<<GetLateralVentricleRight()<<")"<<std::endl;
      BMSAutoSegMainFile<<"GetFilename(RightVentricleName ${RightVentricle} NAME_WITHOUT_EXTENSION)"<<std::endl;   
      if (IsVentricleListEmpty)
      {
	BMSAutoSegMainFile<<"set (VentricleList ${RightVentricle})"<<std::endl;
	BMSAutoSegMainFile<<"set (VentricleLabelList 8)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (VentricleList ${VentricleList} ${RightVentricle})"<<std::endl;
	BMSAutoSegMainFile<<"set (VentricleLabelList ${VentricleLabelList} 8)"<<std::endl;
      }
      IsVentricleListEmpty = false;
      VentricleListLength ++;
    }   

    BMSAutoSegMainFile<<"set (StructureListLength "<<StructureListLength<<")"<<std::endl;
    BMSAutoSegMainFile<<"set (VentricleListLength "<<VentricleListLength<<")"<<std::endl;
  }

  if (GetGenericROISegmentation())
  {
    if (std::strlen(GetROIFile1()) != 0)
    {
      BMSAutoSegMainFile<<"set (GenericROIMapList "<<GetROIFile1()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
      GenericROIMapListLength ++;
    }
    if (std::strlen(GetROIFile2()) != 0)
    {
      if (IsGenericROIMapListEmpty)
	BMSAutoSegMainFile<<"set (GenericROIMapList "<<GetROIFile2()<<")"<<std::endl;
      else
	BMSAutoSegMainFile<<"set (GenericROIMapList ${GenericROIMapList} "<<GetROIFile2()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
      GenericROIMapListLength ++;
    }
    if (std::strlen(GetROIFile3()) != 0)
    {
      if (IsGenericROIMapListEmpty)
	BMSAutoSegMainFile<<"set (GenericROIMapList "<<GetROIFile3()<<")"<<std::endl;
      else
	BMSAutoSegMainFile<<"set (GenericROIMapList ${GenericROIMapList} "<<GetROIFile3()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
      GenericROIMapListLength ++;
    } 
    if (std::strlen(GetROIFile4()) != 0)
    {
      if (IsGenericROIMapListEmpty)
	BMSAutoSegMainFile<<"set (GenericROIMapList "<<GetROIFile4()<<")"<<std::endl;
      else
	BMSAutoSegMainFile<<"set (GenericROIMapList ${GenericROIMapList} "<<GetROIFile4()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
      GenericROIMapListLength ++;
    } 
    if (std::strlen(GetROIFile5()) != 0)
    {
      if (IsGenericROIMapListEmpty)
	BMSAutoSegMainFile<<"set (GenericROIMapList "<<GetROIFile5()<<")"<<std::endl;
      else
	BMSAutoSegMainFile<<"set (GenericROIMapList ${GenericROIMapList} "<<GetROIFile5()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
      GenericROIMapListLength ++;
    } 
    BMSAutoSegMainFile<<"set (GenericROIMapListLength "<<GenericROIMapListLength<<")"<<std::endl;
  } 


  if (GetParcellationMapSegmentation())
  {
    if (std::strlen(GetParcellationFile1()) != 0)
    {
      BMSAutoSegMainFile<<"set (ParcellationMapList "<<GetParcellationFile1()<<")"<<std::endl;
      IsParcellationMapListEmpty = false;
      ParcellationMapListLength ++;
    }
    if (std::strlen(GetParcellationFile2()) != 0)
    {
      if (IsParcellationMapListEmpty)
	BMSAutoSegMainFile<<"set (ParcellationMapList "<<GetParcellationFile2()<<")"<<std::endl;
      else
	BMSAutoSegMainFile<<"set (ParcellationMapList ${ParcellationMapList} "<<GetParcellationFile2()<<")"<<std::endl;
      IsParcellationMapListEmpty = false;
      ParcellationMapListLength ++;
    }
    if (std::strlen(GetParcellationFile3()) != 0)
    {
      if (IsParcellationMapListEmpty)
	BMSAutoSegMainFile<<"set (ParcellationMapList "<<GetParcellationFile3()<<")"<<std::endl;
      else
	BMSAutoSegMainFile<<"set (ParcellationMapList ${ParcellationMapList} "<<GetParcellationFile3()<<")"<<std::endl;
      IsParcellationMapListEmpty = false;
      ParcellationMapListLength ++;
    }
    BMSAutoSegMainFile<<"set (ParcellationMapListLength "<<ParcellationMapListLength<<")"<<std::endl;
  }  

  // Fill LabelList: contains all label files (Ventricles + Generic ROI maps + Parcellation maps)
  if ( !IsVentricleListEmpty || !IsGenericROIMapListEmpty || !IsParcellationMapListEmpty )
  {
    BMSAutoSegMainFile<<"# Fill LabelList, containing all existing label files (Ventricles + Generic ROI maps + Parcellation maps)"<<std::endl;
    if (!IsVentricleListEmpty)
    {
      BMSAutoSegMainFile<<"set (LabelList ${VentricleList})"<<std::endl;
      IsLabelListEmpty = false;
    }
    if (!IsGenericROIMapListEmpty)
    {
      if (IsLabelListEmpty)
	BMSAutoSegMainFile<<"set (LabelList ${GenericROIMapList})"<<std::endl;
      else
	BMSAutoSegMainFile<<"set (LabelList ${LabelList} ${GenericROIMapList})"<<std::endl;
      IsLabelListEmpty = false;
    }
    if (!IsParcellationMapListEmpty)
    {
      if (IsLabelListEmpty)
	BMSAutoSegMainFile<<"set (LabelList ${ParcellationMapList})"<<std::endl;
      else
	BMSAutoSegMainFile<<"set (LabelList ${LabelList} ${ParcellationMapList})"<<std::endl; 
      IsLabelListEmpty = false;
    }
  }

  BMSAutoSegMainFile<<"set(StructureComputed 0)"<<std::endl;      
  BMSAutoSegMainFile<<"set (GenericROIMapComputed 0)"<<std::endl;
  BMSAutoSegMainFile<<"set (ParcellationMapComputed 0)"<<std::endl;   
          
  BMSAutoSegMainFile<<"set (SuffixCorrected _corrected_EMS)"<<std::endl;

  BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
  BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
  BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegMainFile<<"      echo( )"<<std::endl;
  BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
  BMSAutoSegMainFile<<"      echo( )"<<std::endl;
  BMSAutoSegMainFile<<"      set (Case: ${T1CaseHead})"<<std::endl;  
  BMSAutoSegMainFile<<"      set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;

  if (GetClassicWarpingMethod())
  {
    BMSAutoSegMainFile<<"      ListFileInDir(ZippedHFieldList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_warp.hfield.gz)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${ZippedHFieldList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"         set (ZippedHFieldFileTail ${ZippedHFieldList})"<<std::endl;
    BMSAutoSegMainFile<<"         set (ZippedHFieldFile ${WarpROIPath}${ZippedHFieldFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         ExtractString (HFieldFile ${ZippedHFieldFile} 3 FROMEND)"<<std::endl;
    BMSAutoSegMainFile<<"         set (IsHFieldFileZipped 1)"<<std::endl;
  }
  else if (GetBRAINSDemonWarpMethod())
  {
    BMSAutoSegMainFile<<"      ListFileInDir(NrrdList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_transform.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${NrrdList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"         set (NrrdFileTail ${NrrdList})"<<std::endl;
    BMSAutoSegMainFile<<"         set (NrrdFile ${WarpROIPath}${NrrdFileTail})"<<std::endl;
  }
  else if (GetCoarseToFineWarpingMethod())
  {
    BMSAutoSegMainFile<<"      ListFileInDir(MHDFieldList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_deformationfield.mhd)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${MHDFieldList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"         set (MHDFieldFileTail ${MHDFieldList})"<<std::endl;
    BMSAutoSegMainFile<<"         set (MHDFieldFile ${WarpROIPath}${MHDFieldFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         ListFileInDir(ZippedRAWFieldList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_deformationfield.raw.gz)"<<std::endl;
    BMSAutoSegMainFile<<"         If (${ZippedRAWFieldList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"            set (IsRAWFieldFileZipped 1)"<<std::endl; 
    BMSAutoSegMainFile<<"            set (ZippedRAWFieldFile ${WarpROIPath}${ZippedRAWFieldList})"<<std::endl;
    BMSAutoSegMainFile<<"            ExtractString (RAWFieldFile ${ZippedRAWFieldFile} 3 FROMEND)"<<std::endl;
    BMSAutoSegMainFile<<"         Else (${ZippedRAWFieldList})"<<std::endl;
    BMSAutoSegMainFile<<"            set (RAWFieldFile ${WarpROIPath}AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_deformationfield.raw)"<<std::endl;
    BMSAutoSegMainFile<<"            set (IsRAWFieldFileZipped 0)"<<std::endl; 
    BMSAutoSegMainFile<<"         EndIf (${ZippedRAWFieldList})"<<std::endl;
  }
  else if (GetANTSWarpingMethod())
    {
      BMSAutoSegMainFile<<"      ListFileInDir(DeformationFieldList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_Warp.nii.gz)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${DeformationFieldList} != '')"<<std::endl;
      BMSAutoSegMainFile<<"         set (DeformationFieldTail ${DeformationFieldList})"<<std::endl;
      BMSAutoSegMainFile<<"         set (DeformationField ${WarpROIPath}${DeformationFieldTail})"<<std::endl;
    }

  BMSAutoSegMainFile<<"         ListFileInDir(SkullStrippedList ${T1Path}/${AutoSegDir}/Stripped/ ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}.nrrd)"<<std::endl;
  BMSAutoSegMainFile<<"         If (${SkullStrippedList} != '')"<<std::endl;
  BMSAutoSegMainFile<<"            set (SkullStrippedFileTail ${SkullStrippedList})"<<std::endl;
  BMSAutoSegMainFile<<"         EndIf (${SkullStrippedList})"<<std::endl;
  BMSAutoSegMainFile<<"         set (SkullStrippedFile ${T1Path}/${AutoSegDir}/Stripped/${SkullStrippedFileTail})"<<std::endl;
  BMSAutoSegMainFile<<"         set (Case ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected})"<<std::endl<<std::endl;


  if (!IsStructureListEmpty)
  {
    BMSAutoSegMainFile<<"         # Subcortical Structure Segmentation (except Lateral Ventricles)"<<std::endl;
    BMSAutoSegMainFile<<"         ForEach (Structure ${StructureList})"<<std::endl;  
    BMSAutoSegMainFile<<"           GetFilename(StructureName ${Structure} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"           echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"	          echo ('Registering '${StructureName}...)"<<std::endl;
    BMSAutoSegMainFile<<"           set (StructureAffTail ${Case}--${StructureName}-AffReg.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"           set (StructureWarpTail ${Case}--${StructureName}-WarpReg.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"           set (StructureWarpHead ${Case}--${StructureName}-WarpReg)"<<std::endl;
    BMSAutoSegMainFile<<"	    set (StructureAff ${WarpROIPath}${StructureAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"	    set (StructureWarp ${WarpROIPath}${StructureWarpTail})"<<std::endl;
    BMSAutoSegMainFile<<"           # Applying Affine transformation"<<std::endl;
    BMSAutoSegMainFile<<"           ListFileInDir(AffFileList ${WarpROIPath} ${StructureAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"                echo (${AffFileList})"<<std::endl;
    BMSAutoSegMainFile<<"           If (${AffFileList} == '')"<<std::endl;

    //if (!GetBRAINSDemonWarpMethod())
    if (!GetANTSWarpingMethod())
      {
	BMSAutoSegMainFile<<"              set (TransformInputFile ${WarpROIPath}AtlasAffReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_initializetransform.txt)"<<std::endl; 
	BMSAutoSegMainFile<<"              set (command_line ${ResampleVolume2Cmd} ${Structure} ${StructureAff} --transformationFile ${TransformInputFile} -i bs --Reference ${SkullStrippedFile})"<<std::endl;
	BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;
      }
    BMSAutoSegMainFile<<"	          Else ()"<<std::endl;
    BMSAutoSegMainFile<<"              echo ('File already exists: '${StructureAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"           EndIf (${AffFileList})"<<std::endl;
    BMSAutoSegMainFile<<"	    # Applying Warping transformation"<<std::endl;
    BMSAutoSegMainFile<<"           ListFileInDir(WarpFileList ${WarpROIPath} ${StructureWarpTail})"<<std::endl;
    BMSAutoSegMainFile<<"           If (${WarpFileList} == '')"<<std::endl;

    if (GetClassicWarpingMethod())
    {
      BMSAutoSegMainFile<<"              If (${IsHFieldFileZipped} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"                 # Unzipping HField File "<<std::endl;
      BMSAutoSegMainFile<<"                 set (command_line gunzip -f ${ZippedHFieldFile})"<<std::endl;
      BMSAutoSegMainFile<<"                 Run (output ${command_line})"<<std::endl;
      BMSAutoSegMainFile<<"                 set(IsHFieldFileZipped 0)"<<std::endl;
      BMSAutoSegMainFile<<"              EndIf (${IsHFieldFileZipped})"<<std::endl;

      BMSAutoSegMainFile<<"                 set (command_line ${warpCmd} -a ${StructureAff} ${HFieldFile} ${StructureWarp} -linear)"<<std::endl;
      BMSAutoSegMainFile<<"                 Run (output ${command_line})"<<std::endl;
    }
    else if (GetBRAINSDemonWarpMethod())
    {
      BMSAutoSegMainFile<<"              set (command_line ${ResampleVolume2Cmd} ${Structure} ${StructureWarp} -H ${NrrdFile} -i linear --hfieldtype displacement --Reference ${SkullStrippedFile})"<<std::endl;
      BMSAutoSegMainFile<<"      	 Run (prog_output ${command_line} prog_error)"<<std::endl;
    }
    else if (GetCoarseToFineWarpingMethod())
    {
      BMSAutoSegMainFile<<"              If (${IsRAWFieldFileZipped} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"	             # Unzipping deformation field"<<std::endl;
      BMSAutoSegMainFile<<"                  set (command_line gunzip ${ZippedRAWFieldFile})"<<std::endl;
      BMSAutoSegMainFile<<"                  Run (output ${command_line})"<<std::endl;
      BMSAutoSegMainFile<<"                  set(IsRAWFieldFileZipped 0)"<<std::endl;
      BMSAutoSegMainFile<<"	         EndIf (${IsRAWFieldFileZipped})"<<std::endl;
      BMSAutoSegMainFile<<"              set (command_line ${txApplyCmd} -h ${MHDFieldFile} -b -i ${StructureAff} -o ${WarpROIPath}${StructureWarpHead})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl;
      BMSAutoSegMainFile<<"              set (command_line ${convCmd} ${WarpROIPath}${StructureWarpHead}.mhd ${StructureWarp})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (prog_output ${command_line} prog_error)"<<std::endl;
      BMSAutoSegMainFile<<"              DeleteFile (${WarpROIPath}${StructureWarpHead}.mhd)"<<std::endl;
      BMSAutoSegMainFile<<"              DeleteFile (${WarpROIPath}${StructureWarpHead}.raw)"<<std::endl;
    }
    else if (GetANTSWarpingMethod())
      {
	BMSAutoSegMainFile<<"           set (AffineTransformFile ${WarpROIPath}AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_Affine.txt)"<<std::endl; 
	BMSAutoSegMainFile<<"           set (command_line ${WarpImageMultiTransformCmd} 3 ${Structure} ${StructureWarp} -R ${SkullStrippedFile} ${DeformationField} ${AffineTransformFile} --use-BSpline)"<<std::endl;
	BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;	
	BMSAutoSegMainFile<<"      	If(${prog_error} !=  '')"<<std::endl;
	BMSAutoSegMainFile<<"      	  echo('ANTS Error: '${prog_error})"<<std::endl;
	BMSAutoSegMainFile<<"      	EndIf(${prog_error})"<<std::endl;

  // Extra step to compress the nrrd images (not done automatically with WarpImageMultiTransform...)
	BMSAutoSegMainFile<<"       set (command_line ${ImageMathCmd} ${StructureWarp} -constOper 2,1 -outfile ${StructureWarp})"<<std::endl;
 	BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;	
	BMSAutoSegMainFile<<"      	If(${prog_error} !=  '')"<<std::endl;
	BMSAutoSegMainFile<<"      	  echo('ImageMath Error: '${prog_error})"<<std::endl;
	BMSAutoSegMainFile<<"      	EndIf(${prog_error})"<<std::endl; 
      }

    BMSAutoSegMainFile<<"              set(StructureComputed 1)"<<std::endl;
    BMSAutoSegMainFile<<"            Else ()"<<std::endl;
    BMSAutoSegMainFile<<"              echo ('File already exists: '${StructureWarpTail})"<<std::endl;
    BMSAutoSegMainFile<<"            EndIf (${WarpFileList})"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Registering '${StructureName}': DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"         EndForEach(Structure)"<<std::endl<<std::endl;  
  }  
    
    
  if (!IsLabelListEmpty)
  {
    BMSAutoSegMainFile<<"         # Labels Segmentation "<<std::endl;
    BMSAutoSegMainFile<<"         set (GenericROIMapLabelLimit "<<VentricleListLength+GenericROIMapListLength<<")"<<std::endl; 
    BMSAutoSegMainFile<<"         set (LabelNumber 0)"<<std::endl;   
    BMSAutoSegMainFile<<"         ForEach (Label ${LabelList})"<<std::endl;  
    BMSAutoSegMainFile<<"           GetFilename(LabelName ${Label} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"           echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"	    echo ('Registering '${LabelName}...)"<<std::endl;
    BMSAutoSegMainFile<<"           Inc (${LabelNumber} 1)"<<std::endl;
    BMSAutoSegMainFile<<"	    Int(${LabelNumber})"<<std::endl;
  
    if (!IsVentricleListEmpty)
    {
      BMSAutoSegMainFile<<"	          If ( ${Label} == ${LeftVentricle} || ${Label} == ${RightVentricle} )"<<std::endl;
      BMSAutoSegMainFile<<"                 set (LabelAffTail ${Case}--${LabelName}-AffReg-BinMask.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"                 set (LabelWarpTail ${Case}--${LabelName}-WarpReg-BinMask.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"                 set (LabelWarpHead ${Case}--${LabelName}-WarpReg-BinMask)"<<std::endl;
      BMSAutoSegMainFile<<"               Else ()"<<std::endl;
      BMSAutoSegMainFile<<"                 set (LabelAffTail ${Case}--${LabelName}-AffReg.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"                 set (LabelWarpTail ${Case}--${LabelName}-WarpReg.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"                 set (LabelWarpHead ${Case}--${LabelName}-WarpReg)"<<std::endl;
      BMSAutoSegMainFile<<"	          EndIf()"<<std::endl;
    }
    else
    {
      BMSAutoSegMainFile<<"           set (LabelAffTail ${Case}--${LabelName}-AffReg.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"           set (LabelWarpTail ${Case}--${LabelName}-WarpReg.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"           set (LabelWarpHead ${Case}--${LabelName}-WarpReg)"<<std::endl;
    }
  
    BMSAutoSegMainFile<<"	    set (LabelAff ${WarpROIPath}${LabelAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"	    set (LabelWarp ${WarpROIPath}${LabelWarpTail})"<<std::endl;
  
    BMSAutoSegMainFile<<"                 # Applying Affine transformation"<<std::endl;
    BMSAutoSegMainFile<<"           ListFileInDir(AffFileList ${WarpROIPath} ${LabelAffTail})"<<std::endl;
  
    BMSAutoSegMainFile<<"           If (${AffFileList} == '')"<<std::endl;
  
    if (!GetANTSWarpingMethod())
    {
      BMSAutoSegMainFile<<"             set (TransformInputFile ${WarpROIPath}AtlasAffReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_initializetransform.txt)"<<std::endl; 
      BMSAutoSegMainFile<<"             set (command_line ${ResampleVolume2Cmd} ${Label} ${LabelAff} --transformationFile ${TransformInputFile} -i nn --Reference ${SkullStrippedFile})"<<std::endl;
      BMSAutoSegMainFile<<"             Run (prog_output ${command_line} prog_error)"<<std::endl;
    }
  
    BMSAutoSegMainFile<<"	    Else ()"<<std::endl;
    BMSAutoSegMainFile<<"             echo ('File already exists: '${LabelAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"           EndIf (${AffFileList})"<<std::endl;

    BMSAutoSegMainFile<<"           set (flag 0)"<<std::endl;	  
    BMSAutoSegMainFile<<"	    # Applying Warping transformation"<<std::endl;
    BMSAutoSegMainFile<<"           ListFileInDir(WarpFileList ${WarpROIPath} ${LabelWarpTail})"<<std::endl;
    BMSAutoSegMainFile<<"           If (${WarpFileList} == '')"<<std::endl;	  
    BMSAutoSegMainFile<<"               set (flag 1)"<<std::endl;	  

    if (GetClassicWarpingMethod())
    {
      BMSAutoSegMainFile<<"              If (${IsHFieldFileZipped} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"                 # Unzipping HField File "<<std::endl;
      BMSAutoSegMainFile<<"                 set (command_line gunzip -f ${ZippedHFieldFile})"<<std::endl;
      BMSAutoSegMainFile<<"                 Run (output $command_line)"<<std::endl;

      BMSAutoSegMainFile<<"                 set(IsHFieldFileZipped 0)"<<std::endl;
      BMSAutoSegMainFile<<"              EndIf (${IsHFieldFileZipped})"<<std::endl;
      BMSAutoSegMainFile<<"              set (command_line ${warpCmd} -a ${LabelAff} ${HFieldFile} ${LabelWarp} -nearest)"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output $command_line)"<<std::endl;
    }
    else if (GetBRAINSDemonWarpMethod())
    {
      BMSAutoSegMainFile<<"              set (command_line ${ResampleVolume2Cmd} ${Label} ${LabelWarp} -H ${NrrdFile} -i nn --hfieldtype displacement --Reference ${SkullStrippedFile})"<<std::endl;
      BMSAutoSegMainFile<<"      	   Run (prog_output ${command_line} prog_error)"<<std::endl;
    }
    else if (GetCoarseToFineWarpingMethod())
    {
      BMSAutoSegMainFile<<"              If (${IsRAWFieldFileZipped} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"	             # Unzipping deformation field"<<std::endl;
      BMSAutoSegMainFile<<"                  set (command_line gunzip ${ZippedRAWFieldFile})"<<std::endl;
      BMSAutoSegMainFile<<"                  Run (output $command_line)"<<std::endl;
      BMSAutoSegMainFile<<"                  set(IsRAWFieldFileZipped 0)"<<std::endl;
      BMSAutoSegMainFile<<"	         EndIf (${IsRAWFieldFileZipped})"<<std::endl;
      BMSAutoSegMainFile<<"              set (command_line ${txApplyCmd} -h ${MHDFieldFile} -b -i ${LabelAff} -o ${WarpROIPath}${LabelWarpHead} -n)"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output $command_line)"<<std::endl;
      BMSAutoSegMainFile<<"           set (command_line ${convCmd} ${WarpROIPath}${LabelWarpHead}.mhd ${LabelWarp})"<<std::endl;
      BMSAutoSegMainFile<<"           Run (prog_output ${command_line} prog_error)"<<std::endl;
      BMSAutoSegMainFile<<"           DeleteFile(${WarpROIPath}${LabelWarpHead}.mhd)"<<std::endl;
      BMSAutoSegMainFile<<"           DeleteFile(${WarpROIPath}${LabelWarpHead}.raw)"<<std::endl;
    }
    else if (GetANTSWarpingMethod())
      {
	BMSAutoSegMainFile<<"           set (AffineTransformFile ${WarpROIPath}AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}_Affine.txt)"<<std::endl; 
	BMSAutoSegMainFile<<"           set (command_line ${WarpImageMultiTransformCmd} 3 ${Label} ${LabelWarp} -R ${SkullStrippedFile} ${DeformationField} ${AffineTransformFile} --use-NN)"<<std::endl;
	BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;	
	BMSAutoSegMainFile<<"      	If(${prog_error} !=  '')"<<std::endl;
	BMSAutoSegMainFile<<"      	  echo('ANTS Error: '${prog_error})"<<std::endl;
	BMSAutoSegMainFile<<"      	EndIf(${prog_error})"<<std::endl;
	
	// Extra step to compress the nrrd images (not done automatically with WarpImageMultiTransform...)
	BMSAutoSegMainFile<<"       set (command_line ${ImageMathCmd} ${LabelWarp} -constOper 2,1 -outfile ${LabelWarp})"<<std::endl;
	BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;	
	BMSAutoSegMainFile<<"      	If(${prog_error} !=  '')"<<std::endl;
	BMSAutoSegMainFile<<"      	  echo('ImageMath Error: '${prog_error})"<<std::endl;
	BMSAutoSegMainFile<<"      	EndIf(${prog_error})"<<std::endl; 
      }
  
   
    BMSAutoSegMainFile<<"              If (${LabelNumber} > ${VentricleListLength} && ${LabelNumber} <= ${GenericROIMapLabelLimit})"<<std::endl;
    BMSAutoSegMainFile<<"                set (GenericROIMapComputed 1)"<<std::endl; 
    BMSAutoSegMainFile<<"              Else ()"<<std::endl;
    BMSAutoSegMainFile<<"                If (${LabelNumber} > ${GenericROIMapLabelLimit})"<<std::endl; 
    BMSAutoSegMainFile<<"                  set (ParcellationMapComputed 1)"<<std::endl; 
    BMSAutoSegMainFile<<"                EndIf (${LabelNumber})"<<std::endl; 
    BMSAutoSegMainFile<<"              EndIf (${LabelNumber})"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Registering '${LabelName}': DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"           Else ()"<<std::endl;
    BMSAutoSegMainFile<<"              echo ('File already exists: '${LabelWarpTail})"<<std::endl;
    BMSAutoSegMainFile<<"           EndIf (${WarpFileList})"<<std::endl;
    BMSAutoSegMainFile<<"         EndForEach(Label)"<<std::endl<<std::endl;


    BMSAutoSegMainFile<<"   GetFilename(LabelName ${ParcellationMapList} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"   set (ParcellationTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--${LabelName}-WarpReg.nrrd )"<<std::endl;	

    BMSAutoSegMainFile<<"   ListFileInDir(parcellationList ${WarpROIPath} ${ParcellationTail})"<<std::endl;
    BMSAutoSegMainFile<<"   If (${parcellationList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"   If (${flag} == 1)"<<std::endl;

    BMSAutoSegMainFile<<"   set (StripIrescaledTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"   set (StripIrescaled ${T1Path}/${AutoSegDir}/Stripped/${StripIrescaledTail})"<<std::endl;
    BMSAutoSegMainFile<<"   set (Parcellation ${WarpROIPath}${ParcellationTail} )"<<std::endl;	

  }
       
  if (!IsLabelListEmpty)
  {
    BMSAutoSegMainFile<<"           EndIf (${flag})"<<std::endl;
    BMSAutoSegMainFile<<"           EndIf (${parcellationList})"<<std::endl;
  }
  if (GetClassicWarpingMethod())
  {
    BMSAutoSegMainFile<<"           If (${IsHFieldFileZipped} == 0)"<<std::endl;  
    BMSAutoSegMainFile<<"              # Zipping HField File "<<std::endl;
    BMSAutoSegMainFile<<"              set (command_line gzip -f ${HFieldFile})"<<std::endl;
    BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl;
    BMSAutoSegMainFile<<"           EndIf (${IsHFieldFileZipped})"<<std::endl;
    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Error:No Hfield File!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${ZippedHFieldList})"<<std::endl;
  }
  else if (GetBRAINSDemonWarpMethod())
  {
    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Error:No Nrrd File!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${NrrdList})"<<std::endl;
  }
  else if (GetCoarseToFineWarpingMethod())
  {
    BMSAutoSegMainFile<<"           If (${IsRAWFieldFileZipped} == 0)"<<std::endl;  
    BMSAutoSegMainFile<<"               # Zipping deformationfield File "<<std::endl;
    BMSAutoSegMainFile<<"               set (command_line gzip -f ${RAWFieldFile})"<<std::endl;
    BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl;
    BMSAutoSegMainFile<<"           EndIf (${IsRAWFieldFileZipped})"<<std::endl;
    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Error:No deformationfield file!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${MHDFieldList})"<<std::endl;
  }
  else if (GetANTSWarpingMethod())
    {
      BMSAutoSegMainFile<<"      Else ()"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Error:No Deformation file!')"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${DeformationFileList})"<<std::endl;
    }
    
  BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"echo ('ROI TRANSFORMATION DONE!')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  
  if (!IsVentricleListEmpty)
  {
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 10bis. Lateral Ventricles Computation"<<std::endl;
    BMSAutoSegMainFile<<"# "<<std::endl;
    BMSAutoSegMainFile<<"# This step is just applied to the lateral ventricles."<<std::endl;
    BMSAutoSegMainFile<<"# As their segmentation is not so good, we need to take the intersection between "<<std::endl;
    BMSAutoSegMainFile<<"# the lat vent from the previous step and the CSF ems segmentation (label 3)"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('LATERAL VENTRICULES COMPUTATION...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
  
    if (std::strlen(GetLateralVentricleRight()) != 0)
    {
      BMSAutoSegMainFile<<"echo (Right Lateral Ventricle:)"<<std::endl;
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"set (CaseNumber 0)"<<std::endl; 
      BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList}) "<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;  
      BMSAutoSegMainFile<<"      set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
	
      BMSAutoSegMainFile<<"      set (LatVentRightOutputTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--${RightVentricleName}-WarpReg.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      set (LatVentRightOutput ${WarpROIPath}/${LatVentRightOutputTail}) "<<std::endl;
  
      BMSAutoSegMainFile<<"      # The EMS segmentation is scaled up to 1024, so we have to rescale the output image to 256"<<std::endl;
      BMSAutoSegMainFile<<"      # If the EMS Scaling is 32767 (2^15) we have to divide by 128"<<std::endl;
      BMSAutoSegMainFile<<"      set (coeff 128)"<<std::endl;
      BMSAutoSegMainFile<<"      ListFileInDir(LatVentRightOutputList ${WarpROIPath} ${LatVentRightOutputTail})"<<std::endl;
      BMSAutoSegMainFile<<"      If (${LatVentRightOutputList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Masking CSF probabilistic map with the right ventricle segmentation...')"<<std::endl;

      BMSAutoSegMainFile<<"         set (TmpFile ${WarpROIPath}tmp.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"         set (LatVentRight ${WarpROIPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--${RightVentricleName}-WarpReg-BinMask.nrrd)"<<std::endl;
	  
      if(GetLoop())
      {
	BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
	if (std::strcmp(GetEMSoftware(), "ABC") == 0)
	  BMSAutoSegMainFile<<"           set(SuffixPosterior _stripEMS${StrippedBias}${NEOSEG_PREFIX}_posterior2_${SUFFIX})"<<std::endl;
	else
	  BMSAutoSegMainFile<<"           set(SuffixPosterior _stripEMS${StrippedBias}${NEOSEG_PREFIX}_posterior3_${SUFFIX})"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
	if (std::strcmp(GetEMSoftware(), "ABC") == 0)
	  BMSAutoSegMainFile<<"           set(SuffixPosterior ${NEOSEG_PREFIX}_posterior2_${SUFFIX})"<<std::endl;
	else
	  BMSAutoSegMainFile<<"           set(SuffixPosterior ${NEOSEG_PREFIX}_posterior3_${SUFFIX})"<<std::endl;
      }
      if (IsT1LabelEMSFile)
	BMSAutoSegMainFile<<"           set (CSFProbMap ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
      else
      {
	BMSAutoSegMainFile<<"             GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	BMSAutoSegMainFile<<"             GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegMainFile<<"           set (CSFProbMap ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
      }  
      
      BMSAutoSegMainFile<<"         set (command_line ${ImageMathCmd} ${CSFProbMap} -mask ${LatVentRight} -outfile ${TmpFile})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (output ${command_line})"<<std::endl;
      BMSAutoSegMainFile<<"         #And then rescale it to 256"<<std::endl;

      BMSAutoSegMainFile<<"         set (command_line ${ImageMathCmd} ${TmpFile} -constOper 3,${coeff} -outfile ${LatVentRightOutput})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (output ${command_line})"<<std::endl;
      BMSAutoSegMainFile<<"         # Deleting temporary files"<<std::endl;
      BMSAutoSegMainFile<<"         DeleteFile (${TmpFile})"<<std::endl;  
      BMSAutoSegMainFile<<"      Else ()"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('File already exists: '${LatVentRightOutputTail})"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${LatVentRightOutputList})"<<std::endl;	 
      BMSAutoSegMainFile<<"      Inc (${CaseNumber} 1)"<<std::endl;	
      BMSAutoSegMainFile<<"      Int (${CaseNumber})"<<std::endl;	
      BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl<<std::endl;
    }       

    if (std::strlen(GetLateralVentricleLeft()) != 0)
    { 
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"echo ('Left Lateral Ventricle:')"<<std::endl;
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"set (CaseNumber 0)"<<std::endl; 
      BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList}) "<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl; 
	  
      BMSAutoSegMainFile<<"      set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
      BMSAutoSegMainFile<<"      set (LatVentLeftOutputTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--${LeftVentricleName}-WarpReg.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      set (LatVentLeftOutput ${WarpROIPath}/${LatVentLeftOutputTail})"<<std::endl;  
      
      BMSAutoSegMainFile<<"      # The EMS segmentation is scaled up to 1024, so we have to rescale the output image to 256"<<std::endl;
      BMSAutoSegMainFile<<"      # If the EMS Scaling is 32767 (2^15) we have to divide by 128"<<std::endl;
      BMSAutoSegMainFile<<"      set (coeff 128)"<<std::endl;  
      BMSAutoSegMainFile<<"      ListFileInDir(LatVentLeftOutputList ${WarpROIPath} ${LatVentLeftOutputTail})"<<std::endl;
      BMSAutoSegMainFile<<"      If (${LatVentLeftOutputList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Masking CSF probabilistic map with the left ventricle segmentation...')"<<std::endl;
      BMSAutoSegMainFile<<"         set (LatVentLeft ${WarpROIPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--${LeftVentricleName}-WarpReg-BinMask.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"         set (TmpFile ${WarpROIPath}tmp.nrrd)"<<std::endl;  
  			
      if(GetLoop())
      {
	BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
	if (std::strcmp(GetEMSoftware(), "ABC") == 0)
	  BMSAutoSegMainFile<<"           set(SuffixPosterior _stripEMS${StrippedBias}${NEOSEG_PREFIX}_posterior2_${SUFFIX})"<<std::endl;
	else
	  BMSAutoSegMainFile<<"           set(SuffixPosterior _stripEMS${StrippedBias}${NEOSEG_PREFIX}_posterior3_${SUFFIX})"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
	if (std::strcmp(GetEMSoftware(), "ABC") == 0)
	  BMSAutoSegMainFile<<"           set(SuffixPosterior ${NEOSEG_PREFIX}_posterior2_${SUFFIX})"<<std::endl;
	else
	  BMSAutoSegMainFile<<"           set(SuffixPosterior ${NEOSEG_PREFIX}_posterior3_${SUFFIX})"<<std::endl;
      }
      if (IsT1LabelEMSFile)
	BMSAutoSegMainFile<<"           set (CSFProbMap ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
      else
      {
	BMSAutoSegMainFile<<"             GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	BMSAutoSegMainFile<<"             GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegMainFile<<"           set (CSFProbMap ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
      }	  

      BMSAutoSegMainFile<<"         set (command_line ${ImageMathCmd} ${CSFProbMap} -mask ${LatVentLeft} -outfile ${TmpFile})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (output ${command_line})"<<std::endl;
      BMSAutoSegMainFile<<"         #And then rescale it to 256"<<std::endl;

      BMSAutoSegMainFile<<"         set (command_line ${ImageMathCmd} ${TmpFile} -constOper 3,${coeff} -outfile ${LatVentLeftOutput})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (output ${command_line})"<<std::endl;
      BMSAutoSegMainFile<<"         DeleteFile (${TmpFile})"<<std::endl;  
      BMSAutoSegMainFile<<"      Else ()"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('File already exists: '${LatVentLeftOutputTail})"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${LatVentLeftOutputList})"<<std::endl;  
      BMSAutoSegMainFile<<"      Inc (${CaseNumber} 1)"<<std::endl;	
      BMSAutoSegMainFile<<"      Int (${CaseNumber})"<<std::endl;	
      BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl; 
    }      
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('LATERAL VENTRICULES COMPUTATION: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }  
  
  if ( (!IsStructureListEmpty) || (!IsVentricleListEmpty) )
  {

    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 11. Probabilistic volume thresholding"<<std::endl;
    BMSAutoSegMainFile<<"# "<<std::endl;
    BMSAutoSegMainFile<<"# This step is to threshold all the probabilistic volume at 127 "<<std::endl;
    BMSAutoSegMainFile<<"# to get the hard segmentation"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('PROBABILISTIC VOLUME THRESHOLDING...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;

    bool IsProbaListEmpty = true;
    if (!IsStructureListEmpty)
    {
      BMSAutoSegMainFile<<"set (ProbaList ${StructureList})"<<std::endl;
      BMSAutoSegMainFile<<"set (ProbaLabelList ${StructureLabelList})"<<std::endl;
      IsProbaListEmpty = false;
    }
    if (!IsVentricleListEmpty)
    {
      if (IsProbaListEmpty)
      {
	BMSAutoSegMainFile<<"set (ProbaList ${VentricleList})"<<std::endl;
	BMSAutoSegMainFile<<"set (ProbaLabelList ${VentricleLabelList})"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"set (ProbaList ${ProbaList} ${VentricleList})"<<std::endl;
	BMSAutoSegMainFile<<"set (ProbaLabelList ${ProbaLabelList} ${VentricleLabelList})"<<std::endl;
      }
      IsProbaListEmpty = false;
    }

    BMSAutoSegMainFile<<"set (VentricleComputed 0)"<<std::endl;
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"   GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"   GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"   echo( )"<<std::endl;
    BMSAutoSegMainFile<<"   echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"   echo( )"<<std::endl;
    BMSAutoSegMainFile<<"   set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
    BMSAutoSegMainFile<<"   set (ProbaFileNumber 0)"<<std::endl;
    BMSAutoSegMainFile<<"   ForEach (ProbaFile ${ProbaList})"<<std::endl;
    BMSAutoSegMainFile<<"     GetFilename(ProbaFileName ${ProbaFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"     set(FileTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--${ProbaFileName}-WarpReg.nrrd)"<<std::endl;

    BMSAutoSegMainFile<<"     ListFileInDir(FileList ${WarpROIPath} ${FileTail})"<<std::endl;
    BMSAutoSegMainFile<<"     If (${FileList} != '') "<<std::endl;     
    BMSAutoSegMainFile<<"         GetFilename(FileHead ${FileTail} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"         set (File ${WarpROIPath}${FileTail})"<<std::endl;

    BMSAutoSegMainFile<<"         set (OutputFileTail ${FileHead}-HardSeg.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"         set (OutputFile ${WarpROIPath}${OutputFileTail})"<<std::endl;
  
    BMSAutoSegMainFile<<"         ListFileInDir(OutputList ${WarpROIPath} ${OutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         If (${OutputList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"            #Thresholding ${FileTail})"<<std::endl;
    BMSAutoSegMainFile<<"            echo ('Thresholding '${FileTail}...)"<<std::endl;
    
    BMSAutoSegMainFile<<"            set (command_line ${ImageMathCmd} ${File} -threshold 127,256 -outfile ${OutputFile})"<<std::endl;
    BMSAutoSegMainFile<<"            Run (output ${command_line})"<<std::endl;
    BMSAutoSegMainFile<<"            #Labeling ${OutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"            echo ('Labeling '${OutputFileTail}...)"<<std::endl;
    BMSAutoSegMainFile<<"            GetParam(LabelValue ${ProbaLabelList} ${ProbaFileNumber})"<<std::endl;

    BMSAutoSegMainFile<<"            set (command_line ${ImageMathCmd} ${OutputFile} -constOper 2,${LabelValue} -outfile ${OutputFile})"<<std::endl;
    BMSAutoSegMainFile<<"            Run (output ${command_line})"<<std::endl;
      
    if (std::strlen(GetLateralVentricleLeft()) != 0)
    {
      BMSAutoSegMainFile<<"            If (${ProbaFileName} == ${LeftVentricleName})"<<std::endl;
      BMSAutoSegMainFile<<"              set (VentricleComputed 1)"<<std::endl;
      BMSAutoSegMainFile<<"            EndIf()"<<std::endl;
    }
    if (std::strlen(GetLateralVentricleRight()) != 0)
    {
      BMSAutoSegMainFile<<"            If (${ProbaFileName} == ${RightVentricleName})"<<std::endl;
      BMSAutoSegMainFile<<"              set (VentricleComputed 1)"<<std::endl;
      BMSAutoSegMainFile<<"            EndIf()"<<std::endl;
    }

    BMSAutoSegMainFile<<"         Else ()"<<std::endl;
    BMSAutoSegMainFile<<"            echo ('File already exists: '${OutputFileTail})"<<std::endl;
	
    BMSAutoSegMainFile<<"         EndIf (${OutputList})"<<std::endl;
    BMSAutoSegMainFile<<"     Else ()"<<std::endl;
    BMSAutoSegMainFile<<"        echo ('Error:No Probabilistic File!')"<<std::endl;
    BMSAutoSegMainFile<<"     EndIf (${FileList})"<<std::endl;
    BMSAutoSegMainFile<<"     Inc (${ProbaFileNumber} 1)"<<std::endl;	
    BMSAutoSegMainFile<<"     Int (${ProbaFileNumber})"<<std::endl;	  
    BMSAutoSegMainFile<<"   EndForEach(ProbaFile)"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('PROBABILISTIC VOLUME THRESHOLDING: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl; 
   
  
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 12. AllRoi file creation"<<std::endl;
    BMSAutoSegMainFile<<"# "<<std::endl;
    BMSAutoSegMainFile<<"# This step is to create one file with all the ROIs"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('ROI GATHERING...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList}) "<<std::endl;
    BMSAutoSegMainFile<<"   GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"   GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"   echo( )"<<std::endl;
    BMSAutoSegMainFile<<"   echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"   echo( )"<<std::endl; 	  
    BMSAutoSegMainFile<<"   set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
    BMSAutoSegMainFile<<"   set (AllROIFileTail AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}-AllROI.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"   set (AllROIFile ${WarpROIPath}${AllROIFileTail})"<<std::endl;
  
    BMSAutoSegMainFile<<"      ListFileInDir(AllROIList ${WarpROIPath} ${AllROIFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"      If (${AllROIList} == '' || ${StructureComputed} == 1 || ${VentricleComputed} == 1)"<<std::endl;
    BMSAutoSegMainFile<<"         set (FileNumber 1)"<<std::endl;
    BMSAutoSegMainFile<<"         ListFileInDir(ROIList ${WarpROIPath} ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--*-WarpReg-HardSeg.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"         If (${ROIList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"            ForEach(ROIFileTail ${ROIList})"<<std::endl;
    BMSAutoSegMainFile<<"               set (ROIFile ${WarpROIPath}${ROIFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"               If (${FileNumber} == 1)"<<std::endl;
    BMSAutoSegMainFile<<"                  set (command_line cp ${ROIFile} ${AllROIFile})"<<std::endl;
    BMSAutoSegMainFile<<"                  Run (output ${command_line})"<<std::endl;
    BMSAutoSegMainFile<<"               Else ()"<<std::endl;
    BMSAutoSegMainFile<<"                  echo ('Adding '${ROIFileTail}...)"<<std::endl;
    BMSAutoSegMainFile<<"                  set (command_line ${ImageMathCmd} ${AllROIFile} -combine ${ROIFile} -outfile ${AllROIFile})"<<std::endl;
    BMSAutoSegMainFile<<"                  Run (output ${command_line})"<<std::endl;; 
    BMSAutoSegMainFile<<"               EndIf (${FileNumber})"<<std::endl;
    BMSAutoSegMainFile<<"               Inc (${FileNumber} 1)"<<std::endl;
    BMSAutoSegMainFile<<"               Int (${FileNumber})"<<std::endl;
    BMSAutoSegMainFile<<"            EndForEach (ROIFile)"<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${ROIList})"<<std::endl;

    BMSAutoSegMainFile<<"   set (StripIrescaledTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"   set (StripIrescaled ${T1Path}/${AutoSegDir}/Stripped/${StripIrescaledTail})"<<std::endl;

    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('File already exists: '${AllROIFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${AllROIList})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('ROI GATHERING: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }
  }

  if (GetComputeCorticalThickness())
  {
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 13. Cortical thickness computation"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;  

    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('CORTICAL THICKNESS COMPUTATION...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList}) "<<std::endl;
    BMSAutoSegMainFile<<"   GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"   GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"   echo( )"<<std::endl;
    BMSAutoSegMainFile<<"   echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"   echo( )"<<std::endl; 	  
      
    BMSAutoSegMainFile<<"      # Creating CortThick Directory if necessary"<<std::endl;
    BMSAutoSegMainFile<<"      set (CortThickPath ${T1Path}/${AutoSegDir}/CortThick/)"<<std::endl;
    BMSAutoSegMainFile<<"      ListDirInDir (CortThickList ${T1Path}/${AutoSegDir}/ CortThick)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${CortThickList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory( ${CortThickPath})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${CortThickList})"<<std::endl;
    if(GetLoop())
      BMSAutoSegMainFile<<"         set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
    else
      BMSAutoSegMainFile<<"         set (EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
    if (IsT1LabelEMSFile)
      BMSAutoSegMainFile<<"          set (SegmentedCase ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
    else
    {
      BMSAutoSegMainFile<<"          GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"          GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"          set (SegmentedCase ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
    }
    BMSAutoSegMainFile<<"      GetFilename (SegmentedCaseHead ${SegmentedCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"      set(WMCortThickFileHead ${SegmentedCaseHead}-DistanceMapOnWhiteAvg.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      set(GMCortThickFileHead ${SegmentedCaseHead}-DistanceMapOnGreyAvg.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      set(WMCortThickFile ${CortThickPath}${WMCortThickFileHead})"<<std::endl;
    BMSAutoSegMainFile<<"      set(GMCortThickFile ${CortThickPath}${GMCortThickFileHead})"<<std::endl;
      
    BMSAutoSegMainFile<<"      ListFileInDir(OutputFileList ${CortThickPath} ${WMCortThickFileHead})"<<std::endl;
    BMSAutoSegMainFile<<"      If (${OutputFileList} == '')"<<std::endl;  

    if (GetParcellationMapSegmentation())
    {
      BMSAutoSegMainFile<<"         set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
      BMSAutoSegMainFile<<"         ForEach(ParcellationMap ${ParcellationMapList})"<<std::endl;
      BMSAutoSegMainFile<<"            GetFilename (ParcellationMapName ${ParcellationMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"            set (ParcellationFile ${WarpROIPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--${ParcellationMapName}-WarpReg.nrrd )"<<std::endl;
	  

      BMSAutoSegMainFile<<"            set (command_line ${CortThickCLPCmd} ${CortThickPath} --inputSeg ${SegmentedCase} --par ${ParcellationFile} --SaveWM ${WMCortThickFile} --SaveGM ${GMCortThickFile})"<<std::endl;
      BMSAutoSegMainFile<<"            Run (output ${command_line})"<<std::endl;
	  
      BMSAutoSegMainFile<<"            set (OutputCSVFile ${CortThickPath}${SegmentedCaseHead}-WhiteMatDistanceMap_par.csv)"<<std::endl;
      BMSAutoSegMainFile<<"            set (OutputArrayCSVFile ${CortThickPath}${SegmentedCaseHead}-WhiteMatDistanceMap_par_array.csv)"<<std::endl;
      BMSAutoSegMainFile<<"            set (NewOutputCSVFile ${CortThickPath}${SegmentedCaseHead}--${ParcellationMapName}-WhiteMatDistanceMap_par.csv)"<<std::endl;
      BMSAutoSegMainFile<<"            set (NewOutputArrayCSVFile ${CortThickPath}${SegmentedCaseHead}--${ParcellationMapName}-WhiteMatDistanceMap_par_array.csv)"<<std::endl;
      BMSAutoSegMainFile<<"            set (command_line mv ${OutputCSVFile} ${NewOutputCSVFile})"<<std::endl;
      BMSAutoSegMainFile<<"            Run (output ${command_line})"<<std::endl;
      BMSAutoSegMainFile<<"            set (command_line mv ${OutputArrayCSVFile} ${NewOutputArrayCSVFile})"<<std::endl;
      BMSAutoSegMainFile<<"            Run (output ${command_line})"<<std::endl;

      BMSAutoSegMainFile<<"         EndForEach(ParcellationMap)"<<std::endl;
    }
    else
    {
      BMSAutoSegMainFile<<"            set (command_line ${CortThickCLPCmd} ${CortThickPath} --inputSeg ${SegmentedCase} --SaveWM ${WMCortThickFile} --SaveGM ${GMCortThickFile})"<<std::endl;  
      BMSAutoSegMainFile<<"            Run (output ${command_line})"<<std::endl;
    }

    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Cortical Thickness already Done!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${OutputFileList})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('CORTICAL THICKNESS COMPUTATION: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }
    
  if (GetMultiAtlasSegmentation()) {
        BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
        BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
        BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
        BMSAutoSegMainFile<<"# 14. Multi-Atlas Segmentation"<<std::endl;
        BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
        BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
        BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;  

        BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
        BMSAutoSegMainFile<<"echo ('MULTI-ATLAS SEGMENTATION...')"<<std::endl;
        BMSAutoSegMainFile<<"echo ( )"<<std::endl;
 
        BMSAutoSegMainFile<<"set (ANTSCCWeight "<<GetANTSCCWeight()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (ANTSCCRegionRadius "<<GetANTSCCRegionRadius()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (ANTSMIWeight "<<GetANTSMIWeight()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (ANTSMIBins "<<GetANTSMIBins()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (ANTSMSQWeight "<<GetANTSMSQWeight()<<")"<<std::endl;
 
        BMSAutoSegMainFile<<"set (ANTSCCWeight2nd "<<GetANTSCCWeight2nd()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (ANTSCCRegionRadius2nd "<<GetANTSCCRegionRadius2nd()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (ANTSMIWeight2nd "<<GetANTSMIWeight2nd()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (ANTSMIBins2nd "<<GetANTSMIBins2nd()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (ANTSMSQWeight2nd "<<GetANTSMSQWeight2nd()<<")"<<std::endl;
 
        BMSAutoSegMainFile<<"set (ANTSTransformationStep "<<GetANTSTransformationStep()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (ANTSGaussianSigma "<<GetANTSGaussianSigma()<<")"<<std::endl;

        int NUMBER_OF_CASE = 0;

        for (int i = 0; i < GetNbData(); i++){
            std::cout << "data list  : " << m_MultiAtlasT1List[i] << " " << m_MultiAtlasT2List[i] << std::endl;
        }

        while (NUMBER_OF_CASE < GetNbData()) { 
          std::string targetDirectory;// targetT2Directory; 
          targetDirectory = m_MultiAtlasT1List[NUMBER_OF_CASE];
          NUMBER_OF_CASE++;

	  targetDirectory = itksys::SystemTools::GetParentDirectory(targetDirectory.c_str()) +"/";

	    BMSAutoSegMainFile<<"set (MultiAtlasDir "<<GetMultiAtlasDirectory()<<")"<<std::endl;
	    BMSAutoSegMainFile<<"set (MultiAtlasSegCmd 'MultiAtlasSeg')"<<std::endl;
	    if (GetMultiModalityMultiSegmentation()) { 
	      BMSAutoSegMainFile<<"set (TargetCaseFile "<< m_MultiAtlasT1List[NUMBER_OF_CASE - 1] <<")"<<std::endl;
	      BMSAutoSegMainFile<<"set (TargetT2CaseFile "<< m_MultiAtlasT2List[NUMBER_OF_CASE - 1] <<")"<<std::endl;
	    }
	    else {
	      BMSAutoSegMainFile<<"set (TargetCaseFile "<< m_MultiAtlasT1List[NUMBER_OF_CASE - 1] <<")"<<std::endl;
	    }
	    
	    if (GetMultiModalityMultiSegmentation()) { 
	      std::cout << "target file of first modality: " << m_MultiAtlasT1List[NUMBER_OF_CASE - 1] << std::endl;
	      std::cout << "target file of second modality: " << m_MultiAtlasT2List[NUMBER_OF_CASE - 1] << std::endl; }
	    else {
	      std::cout << "target file: " << m_MultiAtlasT1List[NUMBER_OF_CASE - 1] << std::endl;
	    }

        BMSAutoSegMainFile<<"set (WeightIntensityEnergy "<<GetIntensityEnergyWeight()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (WeightHarmonicEnergy "<<GetHarmonicEnergyWeight()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (WeightShapeEnergy "<<GetShapeEnergyWeight()<<")"<<std::endl;

        BMSAutoSegMainFile<<"GetFilename (TargetCasePath ${TargetCaseFile} PATH)"<<std::endl;
        BMSAutoSegMainFile<<"set (TargetPath "<< targetDirectory.c_str()<<"/${AutoSegDir}/Stripped/)"<<std::endl;
        BMSAutoSegMainFile<<"echo('Target Case Path: '${TargetCasePath} )"<<std::endl;
        BMSAutoSegMainFile<<"set (SuffixStrippedNRRD -stripped.nrrd)"<<std::endl;
        BMSAutoSegMainFile<<"set (PrefixMultiAtlas Rescaled_)"<<std::endl;

        if (GetMultiModalityMultiSegmentation()) { 
            BMSAutoSegMainFile<<"GetFilename (T1CaseHead ${TargetCaseFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
            BMSAutoSegMainFile<<"GetFilename (T2CaseHead ${TargetT2CaseFile} NAME_WITHOUT_EXTENSION)"<<std::endl;

            BMSAutoSegMainFile<<"ListFileInDir(MultiAtlasTargetList ${TargetPath} ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}${SuffixStrippedNRRD})"<<std::endl;
            BMSAutoSegMainFile<<"set (MultiAtlasTargetFile ${MultiAtlasTargetList})"<<std::endl;
            BMSAutoSegMainFile<<"ListFileInDir(MultiAtlasT2TargetList ${TargetPath} ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixCorrected}${SuffixStrippedNRRD})"<<std::endl;
            BMSAutoSegMainFile<<"set (MultiAtlasT2TargetFile ${MultiAtlasT2TargetList})"<<std::endl;
            BMSAutoSegMainFile<<"set (MultiAtlasTargetHead ${TargetPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected})"<<std::endl;
	    
            BMSAutoSegMainFile<<"echo('MULTIATLAS TARGET FILES:  '${MultiAtlasTargetFile} ' ' ${MultiAtlasT2TargetList})"<<std::endl;

            BMSAutoSegMainFile<<"ListFileInDir(RescaleOutputFileList ${TargetPath} ${PrefixMultiAtlas}${MultiAtlasTargetFile})"<<std::endl;
            BMSAutoSegMainFile<<"If (${RescaleOutputFileList} == '')"<<std::endl;
            BMSAutoSegMainFile<<"  set (command_line ${ImageMathCmd} ${TargetPath}${MultiAtlasTargetFile} -rescale 0,8192 -outfile ${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile})"<<std::endl; 
	    BMSAutoSegMainFile<<"  Run (output ${command_line})"<<std::endl;
            BMSAutoSegMainFile<<"Else ()"<<std::endl;
            BMSAutoSegMainFile<<"  echo ('Rescaling T1 already Done!')"<<std::endl;
            BMSAutoSegMainFile<<"EndIf (${RescaleOutputFileList})"<<std::endl;

            BMSAutoSegMainFile<<"ListFileInDir(RescaleOutputFileList ${TargetPath} ${PrefixMultiAtlas}${MultiAtlasT2TargetFile})"<<std::endl;
            BMSAutoSegMainFile<<"If (${RescaleOutputFileList} == '')"<<std::endl;
            BMSAutoSegMainFile<<"  set (command_line ${ImageMathCmd} ${TargetPath}${MultiAtlasT2TargetFile} -rescale 0,8192 -outfile ${TargetPath}${PrefixMultiAtlas}${MultiAtlasT2TargetFile})"<<std::endl;
	    BMSAutoSegMainFile<<"  Run (output ${command_line})"<<std::endl; 
            BMSAutoSegMainFile<<"Else ()"<<std::endl;
            BMSAutoSegMainFile<<"  echo ('Rescaling T2 already Done!')"<<std::endl;
            BMSAutoSegMainFile<<"EndIf (${RescaleOutputFileList})"<<std::endl;
            
        }
        else {
            BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${TargetCaseFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
            BMSAutoSegMainFile<<"      ListFileInDir(MultiAtlasTargetList ${TargetPath} ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}${SuffixStrippedNRRD})"<<std::endl;
            BMSAutoSegMainFile<<"      set (MultiAtlasTargetFile ${MultiAtlasTargetList})"<<std::endl;
            BMSAutoSegMainFile<<"set (MultiAtlasTargetHead ${TargetPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected})"<<std::endl;
            BMSAutoSegMainFile<<"      echo('MULTIATLAS TARGET FILE:  '${MultiAtlasTargetFile} )"<<std::endl;
            BMSAutoSegMainFile<<"      ListFileInDir(RescaleOutputFileList ${TargetPath} ${PrefixMultiAtlas}${MultiAtlasTargetFile})"<<std::endl;
            BMSAutoSegMainFile<<"      If (${RescaleOutputFileList} == '')"<<std::endl;  
            BMSAutoSegMainFile<<"        set (command_line ${ImageMathCmd} ${TargetPath}${MultiAtlasTargetFile} -rescale 0,8192 -outfile ${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile})"<<std::endl; 
	    BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
            BMSAutoSegMainFile<<"      Else ()"<<std::endl;
            BMSAutoSegMainFile<<"         echo ('Rescaling already Done!')"<<std::endl;
            BMSAutoSegMainFile<<"      EndIf (${RescaleOutputFileList})"<<std::endl;
        }


        BMSAutoSegMainFile<<"      ListDirInDir (FolderList "<<targetDirectory.c_str()<<"${AutoSegDir} displacement_field)"<<std::endl;
        BMSAutoSegMainFile<<"      If (${FolderList} == '')"<<std::endl;
        BMSAutoSegMainFile<<"         MakeDirectory ("<<targetDirectory.c_str()<<"${AutoSegDir}/displacement_field)"<<std::endl;
        BMSAutoSegMainFile<<"      EndIf (${FolderList})"<<std::endl;

        BMSAutoSegMainFile<<"      set (DeformationFieldDir "<<targetDirectory.c_str()<<"${AutoSegDir}/displacement_field/)"<<std::endl;
        BMSAutoSegMainFile<<"      echo('Deformation Case Path: '${DeformationFieldDir} )"<<std::endl;

        BMSAutoSegMainFile<<"      ListDirInDir (FolderList "<<targetDirectory.c_str()<<"${AutoSegDir} warped-atlas-images)"<<std::endl;
        BMSAutoSegMainFile<<"      If (${FolderList} == '')"<<std::endl;
        BMSAutoSegMainFile<<"         MakeDirectory ("<<targetDirectory.c_str()<<"${AutoSegDir}/warped-atlas-images)"<<std::endl;
        BMSAutoSegMainFile<<"      EndIf (${FolderList})"<<std::endl;
        BMSAutoSegMainFile<<"      set (WarpedAtlasDir "<<targetDirectory.c_str()<<"${AutoSegDir}/warped-atlas-images/)"<<std::endl;

        BMSAutoSegMainFile<<"      ListDirInDir (FolderList "<<targetDirectory.c_str()<<"${AutoSegDir} warped-atlas-labels)"<<std::endl;
        BMSAutoSegMainFile<<"      If (${FolderList} == '')"<<std::endl;
        BMSAutoSegMainFile<<"         MakeDirectory ("<<targetDirectory.c_str()<<"${AutoSegDir}/warped-atlas-labels)"<<std::endl;
        BMSAutoSegMainFile<<"      EndIf (${FolderList})"<<std::endl;
        BMSAutoSegMainFile<<"      set (WarpedLabelDir "<<targetDirectory.c_str()<<"${AutoSegDir}/warped-atlas-labels/)"<<std::endl;

        BMSAutoSegMainFile<<"      ListDirInDir (FolderList "<<targetDirectory.c_str()<<"${AutoSegDir} warped-atlas-parcellations)"<<std::endl;
        BMSAutoSegMainFile<<"      If (${FolderList} == '')"<<std::endl;
        BMSAutoSegMainFile<<"         MakeDirectory ("<<targetDirectory.c_str()<<"${AutoSegDir}/warped-atlas-parcellations)"<<std::endl;
        BMSAutoSegMainFile<<"      EndIf (${FolderList})"<<std::endl;
        BMSAutoSegMainFile<<"      set (WarpedParcellationDir "<<targetDirectory.c_str()<<"${AutoSegDir}/warped-atlas-parcellations/)"<<std::endl;

        BMSAutoSegMainFile<<"      ListDirInDir (FolderList "<<targetDirectory.c_str()<<"${AutoSegDir} intensity_energy)"<<std::endl;
        BMSAutoSegMainFile<<"      If (${FolderList} == '')"<<std::endl;
        BMSAutoSegMainFile<<"         MakeDirectory ("<<targetDirectory.c_str()<<"${AutoSegDir}/intensity_energy)"<<std::endl;
        BMSAutoSegMainFile<<"      EndIf (${FolderList})"<<std::endl;
        BMSAutoSegMainFile<<"      set (IntEnergyDir "<<targetDirectory.c_str()<<"${AutoSegDir}/intensity_energy/)"<<std::endl;

        BMSAutoSegMainFile<<"      ListDirInDir (FolderList "<<targetDirectory.c_str()<<"${AutoSegDir} harmonic_energy)"<<std::endl;
        BMSAutoSegMainFile<<"      If (${FolderList} == '')"<<std::endl;
        BMSAutoSegMainFile<<"         MakeDirectory ("<<targetDirectory.c_str()<<"${AutoSegDir}/harmonic_energy)"<<std::endl;
        BMSAutoSegMainFile<<"      EndIf (${FolderList})"<<std::endl;
        BMSAutoSegMainFile<<"      set (HarmonicEnergyDir "<<targetDirectory.c_str()<<"${AutoSegDir}/harmonic_energy/)"<<std::endl;

        BMSAutoSegMainFile<<"      ListDirInDir (FolderList "<<targetDirectory.c_str()<<"${AutoSegDir} template)"<<std::endl;
        BMSAutoSegMainFile<<"      If (${FolderList} == '')"<<std::endl;
        BMSAutoSegMainFile<<"         MakeDirectory ("<<targetDirectory.c_str()<<"${AutoSegDir}/template)"<<std::endl;
        BMSAutoSegMainFile<<"      EndIf (${FolderList})"<<std::endl;
        BMSAutoSegMainFile<<"      set (TemplateDir " <<targetDirectory.c_str()<<"${AutoSegDir}/template/)"<<std::endl;

        BMSAutoSegMainFile<<"      ListDirInDir (FolderList "<<targetDirectory.c_str()<<"${AutoSegDir} fused-labels)"<<std::endl;
        BMSAutoSegMainFile<<"      If (${FolderList} == '')"<<std::endl;
        BMSAutoSegMainFile<<"         MakeDirectory ("<<targetDirectory.c_str()<<"${AutoSegDir}/fused-labels)"<<std::endl;
        BMSAutoSegMainFile<<"      EndIf (${FolderList})"<<std::endl;
        BMSAutoSegMainFile<<"      set (FusedLabelDir " <<targetDirectory.c_str()<<"${AutoSegDir}/fused-labels/)"<<std::endl;

        BMSAutoSegMainFile<<"      set (DeformationFieldTrainToTrainDir "<<GetMultiAtlasDirectory()<<"displacement_field_atlas_to_atlas/)"<<std::endl;
        BMSAutoSegMainFile<<"      set (WarpedAtlasTrainToTrainDir "<<GetMultiAtlasDirectory()<<"warped-atlas-atlas-images/)"<<std::endl;

        if (GetLoop()) {
           BMSAutoSegMainFile<<"      set (EMSDIR "<<targetDirectory.c_str()<<"${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
        } else {
           BMSAutoSegMainFile<<"      set (EMSDir "<<targetDirectory.c_str()<<"${AutoSegDir}/ems/)"<<std::endl;
        }

        BMSAutoSegMainFile<<"      set (NumberAtlas "<<(GetNbAtlas())<<")"<<std::endl;
        BMSAutoSegMainFile<<"      set (NumberCase "<<(GetNbAtlas())+1<<")"<<std::endl;

        // sort the name of atlas
        SortStringList(m_AtlasList, GetNbAtlas());
        if (GetMultiModalityMultiSegmentation()) { 
            SortStringList(m_2ndAtlasList, GetNb2ndAtlas());
        }
        SortStringList(m_AtlasLabelList, GetNbAtlasLabel());
	if (GetNbAtlasParcellation() > 0 ) {
	  SortStringList(m_AtlasParcellationList, GetNbAtlasParcellation());
	}

        //set up atlas case ID list 
        BMSAutoSegMainFile<<"Set(AtlasHead 'atlas_')"<<std::endl;
        BMSAutoSegMainFile<<"echo('atlas head: ' ${AtlasHead})"<<std::endl; 

	std::string tmpAtlasCaseIDFilename;
	std::string tmpAtlasCaseExtension = itksys::SystemTools::GetFilenameExtension(m_AtlasList[0]);

	std::string tmpAtlasName = itksys::SystemTools::GetFilenameWithoutExtension(m_AtlasList[0]);
	int sizeOfFilename = tmpAtlasName.length();

        if (GetMultiModalityMultiSegmentation()) { 
            tmpAtlasCaseIDFilename = tmpAtlasName.substr(6, sizeOfFilename - 4 - 6); // remove atlas_, dot,  _t1w or _t2w
        }
        else {
            tmpAtlasCaseIDFilename = tmpAtlasName.substr(6, sizeOfFilename - 6); // remove dot 
        }
        BMSAutoSegMainFile<<"set (AtlasCaseIDList "<<tmpAtlasCaseIDFilename.c_str()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (AtlasList "<< "atlas_" <<tmpAtlasCaseIDFilename.c_str()<<")"<<std::endl;
        BMSAutoSegMainFile<<"set (AtlasCaseExtension "<< tmpAtlasCaseExtension.c_str()<<")"<<std::endl;

        for (int DataNumber = 1; DataNumber < GetNbAtlas(); DataNumber++){       // set up warped atlas list
	  tmpAtlasName = itksys::SystemTools::GetFilenameWithoutExtension(m_AtlasList[DataNumber]);
	  sizeOfFilename = tmpAtlasName.length(); 
         
	  if (GetMultiModalityMultiSegmentation()) { 
            tmpAtlasCaseIDFilename = tmpAtlasName.substr(6, sizeOfFilename - 4 - 6); // remove atlas_, dot,  _t1w or _t2w
	  }
	  else {
            tmpAtlasCaseIDFilename = tmpAtlasName.substr(6, sizeOfFilename - 6); // remove dot 
	  }
	  BMSAutoSegMainFile<<"set (AtlasCaseIDList ${AtlasCaseIDList} "<<tmpAtlasCaseIDFilename.c_str()<<")"<<std::endl;
	  BMSAutoSegMainFile<<"set (AtlasList ${AtlasList} "<< "atlas_"<<tmpAtlasCaseIDFilename.c_str()<<")"<<std::endl;
        }

        //set up gray-level atlas image list
        BMSAutoSegMainFile<<"   echo('atlas directory: ' ${MultiAtlasDir})"<<std::endl; 	  
        BMSAutoSegMainFile<<"   echo('atlas list: ' ${AtlasList})"<<std::endl; 	  
        BMSAutoSegMainFile<<"   echo('atlas extension: ' ${AtlasCaseExtension})"<<std::endl; 	  
        BMSAutoSegMainFile<<"   echo('atlas case ID list: ' ${AtlasCaseIDList})"<<std::endl; 	  

        //set up atlas label list
	std::string tmpAtlasLabelExt = itksys::SystemTools::GetFilenameExtension(m_AtlasLabelList[0]);
        BMSAutoSegMainFile<<"   set (AtlasLabelExtension "<< tmpAtlasLabelExt.c_str() <<")"<<std::endl;

	//std::string tmpAtlasLabelName = itksys::SystemTools::GetFilenameWithoutExtension(m_AtlasLabelList[0]);
	//sizeOfFilename = tmpAtlasLabelName.length();
	//	std::string tmpAtlasLabelFilename = tmpAtlasLabelName.substr(0, sizeOfFilename - 1); // remove dot
        
	//BMSAutoSegMainFile<<"   set (AtlasLabelList "<<tmpAtlasLabelFilename.c_str() <<")"<<std::endl;

        //for (int DataNumber = 1; DataNumber < GetNbAtlasLabel(); DataNumber++){       // set up warped atlas list
	//    tmpAtlasLabelName = itksys::SystemTools::GetFilenameWithoutExtension(m_AtlasLabelList[DataNumber]);
	//    sizeOfFilename = tmpAtlasLabelName.length();
	//    tmpAtlasLabelFilename = tmpAtlasLabelName.substr(0, sizeOfFilename - 1); // remove dot
        //    BMSAutoSegMainFile<<"   set (AtlasLabelList ${AtlasLabelList} "<<tmpAtlasLabelFilename.c_str()<<")"<<std::endl;
	//}
        //BMSAutoSegMainFile<<"   echo('label list: ' ${AtlasLabelList})"<<std::endl; 	  

        //set up target list 

	std::string tmpTargetName = itksys::SystemTools::GetFilenameWithoutExtension(m_MultiAtlasT1List[NUMBER_OF_CASE - 1]);
	sizeOfFilename = tmpTargetName.length();
						
        std::string tmpTargetFilename = tmpTargetName;
        //if (GetMultiModalityMultiSegmentation()) 
	//    tmpTargetFilename = tmpTargetName.substr(0, sizeOfFilename - 4 - 1);  // remove dot and _t1w or _t2w
        //else 
	//    tmpTargetFilename = tmpTargetName.substr(0, sizeOfFilename - 1); // remove dot
        BMSAutoSegMainFile<<"   set (TargetList "<<tmpTargetFilename.c_str()<<")"<<std::endl;

        //set up target case ID list
        BMSAutoSegMainFile<<"   set (TargetCaseIDList "<<tmpTargetFilename.c_str()<<")"<<std::endl;

        std::string WarpedAtlasDirectory = GetDataDirectory();
        WarpedAtlasDirectory = targetDirectory;
        WarpedAtlasDirectory += GetDataAutoSegDirectory();
        WarpedAtlasDirectory += "/warped-atlas-images/";

        std::string WarpedLabelDirectory = GetDataDirectory();
        WarpedLabelDirectory = targetDirectory;
        WarpedLabelDirectory += GetDataAutoSegDirectory();
        WarpedLabelDirectory += "/warped-atlas-labels/";

        std::string WarpedParcellationDirectory = GetDataDirectory();
        WarpedParcellationDirectory = targetDirectory;
        WarpedParcellationDirectory += GetDataAutoSegDirectory();
        WarpedParcellationDirectory += "/warped-atlas-parcellations/";

        std::string DeformationFieldDirectory = GetDataDirectory();
        DeformationFieldDirectory = targetDirectory;
        DeformationFieldDirectory += GetDataAutoSegDirectory();
        DeformationFieldDirectory += "/displacement_field/";

        std::string IntEnergyDirectory = GetDataDirectory();
        IntEnergyDirectory = targetDirectory;
        IntEnergyDirectory += GetDataAutoSegDirectory();
        IntEnergyDirectory += "/intensity_energy/";

        std::string HarmonicEnergyDirectory = GetDataDirectory();
        HarmonicEnergyDirectory = targetDirectory;
        HarmonicEnergyDirectory += GetDataAutoSegDirectory();
        HarmonicEnergyDirectory += "/harmonic_energy/";

        std::string WarpedAtlasTrainToTrainDirectory = GetMultiAtlasDirectory();
        WarpedAtlasTrainToTrainDirectory += "warped-atlas-atlas-images/";
	std::string DeformationFieldTrainToTrainDirectory = GetMultiAtlasDirectory();
	DeformationFieldTrainToTrainDirectory += "displacement_field_atlas_to_atlas/";

        BMSAutoSegMainFile<<"set (DisplacementFieldDirectory "<< DeformationFieldDirectory.c_str() << ")" << std::endl;
 
        BMSAutoSegMainFile<<"set (Path ${DeformationFieldTrainToTrainDir})"<<std::endl;	  
        BMSAutoSegMainFile<<"set (WarpCmd WarpImageMultiTransform)"<<std::endl;
        BMSAutoSegMainFile<<"ListFileInDir(ANTSOutputDisplacementFieldList ${Path})"<<std::endl;
        BMSAutoSegMainFile<<"ForEach (WarpedCaseForTarget ${AtlasList}) "<<std::endl;
        BMSAutoSegMainFile<<"ForEach (WarpedCase ${AtlasList}) "<<std::endl;
        BMSAutoSegMainFile<<"If (${WarpedCase} != ${WarpedCaseForTarget})"<<std::endl;
        BMSAutoSegMainFile<<"   set (FileExist 1)"<<std::endl;
        BMSAutoSegMainFile<<"   set (FileExistFlag 0)"<<std::endl;
        BMSAutoSegMainFile<<"   set (outputfilenameForCheck ${WarpedCase}x${WarpedCaseForTarget}TotalWarp.nii.gz)"<<std::endl;
        BMSAutoSegMainFile<<"   ListFileInDir(DisplacementExistList ${Path} ${outputfilenameForCheck})"<<std::endl;
        BMSAutoSegMainFile<<"   If (${DisplacementExistList} == '')"<<std::endl;
	if (GetMultiModalityMultiSegmentation()) {
	  BMSAutoSegMainFile<<"    set (command_line ${ANTSCmd} 3)"<<std::endl;
	  if (GetANTSCCWeight() > 0.01)
	    BMSAutoSegMainFile<<"    set (command_line ${command_line} -m CC[${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}_t1w${AtlasCaseExtension},${MultiAtlasDir}atlas_image/${WarpedCase}_t1w${AtlasCaseExtension},${ANTSCCWeight},${ANTSCCRegionRadius}])"<<std::endl;
	  if (GetANTSCCWeight2nd() > 0.01)
	    BMSAutoSegMainFile<<"    set (command_line ${command_line} -m CC[${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}_t2w${AtlasCaseExtension},${MultiAtlasDir}atlas_image/${WarpedCase}_t2w${AtlasCaseExtension},${ANTSCCWeight2nd},${ANTSCCRegionRadius2nd}])"<<std::endl;
	  if (GetANTSMIWeight() > 0.01)
	    BMSAutoSegMainFile<<"    set (command_line ${command_line} -m MI[${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}_t1w${AtlasCaseExtension},${MultiAtlasDir}atlas_image/${WarpedCase}_t1w${AtlasCaseExtension},${ANTSMIWeight},${ANTSMIBins}])"<<std::endl;
	  if (GetANTSMIWeight2nd() > 0.01)
	    BMSAutoSegMainFile<<"    set (command_line ${command_line} -m MI[${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}_t2w${AtlasCaseExtension},${MultiAtlasDir}atlas_image/${WarpedCase}_t2w${AtlasCaseExtension},${ANTSMIWeight2nd},${ANTSMIBins2nd}])"<<std::endl;
	  if (GetANTSMSQWeight() > 0.01)
	    BMSAutoSegMainFile<<"    set (command_line ${command_line} -m MSQ[${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}_t1w${AtlasCaseExtension},${MultiAtlasDir}atlas_image/${WarpedCase}_t1w${AtlasCaseExtension},${ANTSMSQWeight},0.01])"<<std::endl;
	  if (GetANTSMSQWeight2nd() > 0.01)
	    BMSAutoSegMainFile<<"    set (command_line ${command_line} -m MSQ[${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}_t2w${AtlasCaseExtension},${MultiAtlasDir}atlas_image/${WarpedCase}_t2w${AtlasCaseExtension},${ANTSMSQWeight2nd},0.01])"<<std::endl;
	}
	else{
	  BMSAutoSegMainFile<<"    set (command_line ${ANTSCmd} 3)"<<std::endl;
	  if (GetANTSCCWeight() > 0.01)
	    BMSAutoSegMainFile<<"    set (command_line ${command_line} -m CC[${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}${AtlasCaseExtension},${MultiAtlasDir}atlas_image/${WarpedCase}${AtlasCaseExtension},${ANTSCCWeight},${ANTSCCRegionRadius}])"<<std::endl;
	  if (GetANTSMIWeight() > 0.01)
	    BMSAutoSegMainFile<<"    set (command_line ${command_line} -m MI[${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}${AtlasCaseExtension},${MultiAtlasDir}atlas_image/${WarpedCase}${AtlasCaseExtension},${ANTSMIWeight},${ANTSMIBins}])"<<std::endl;
	  if (GetANTSMSQWeight() > 0.01)
	    BMSAutoSegMainFile<<"    set (command_line ${command_line} -m MSQ[${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}${AtlasCaseExtension},${MultiAtlasDir}atlas_image/${WarpedCase}${AtlasCaseExtension},${ANTSMSQWeight},0.01])"<<std::endl;
	}
	BMSAutoSegMainFile<<"    	set (command_line ${command_line} -i ${ANTSIterations} -o ${DeformationFieldTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}Total.nii.gz)"<<std::endl;
	if (std::strcmp(GetANTSRegistrationFilterType(), "GreedyDiffeomorphism") == 0)
	  {
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t SyN[${ANTSTransformationStep}])"<<std::endl;
	    if (GetANTSGaussianSmoothing())
	      BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[${ANTSGaussianSigma},0])"<<std::endl;      
	  }
	else if (std::strcmp(GetANTSRegistrationFilterType(), "SpatiotemporalDiffeomorphism") == 0)
	  {
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t SyN[${ANTSTransformationStep}])"<<std::endl;
	    if (GetANTSGaussianSmoothing())
	      BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[${ANTSGaussianSigma},0.5])"<<std::endl;      
	  }
	else if (std::strcmp(GetANTSRegistrationFilterType(), "Elastic") == 0)
	  {
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t Elast[${ANTSTransformationStep}])"<<std::endl;
	    if (GetANTSGaussianSmoothing())
	      BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[0,${ANTSGaussianSigma}])"<<std::endl;
	  }
	else if (std::strcmp(GetANTSRegistrationFilterType(), "Exponential") == 0)
	  {
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t Exp[${ANTSTransformationStep}])"<<std::endl;
	    if (GetANTSGaussianSmoothing())
	      BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[0,${ANTSGaussianSigma}])"<<std::endl;
	  }
	BMSAutoSegMainFile<<"    	set (command_line ${command_line} --use-all-metrics-for-convergence)"<<std::endl;
	BMSAutoSegMainFile<<"           Run (output ${command_line})"<<std::endl; // calculate intensity energy
	BMSAutoSegMainFile<<"      EndIf (${DisplacementExistList})"<<std::endl;
	//BMSAutoSegMainFile<<"echo('atlas warp: ' ${WarpedCase}x${WarpedCaseForTarget}Warped.nii.gz)"<<std::endl;
	BMSAutoSegMainFile<<"ListFileInDir(WarpedAtlasExistList ${WarpedAtlasTrainToTrainDir} ${WarpedCase}x${WarpedCaseForTarget}Warped.nii.gz)"<<std::endl;
	BMSAutoSegMainFile<<"If (${WarpedAtlasExistList} == '')"<<std::endl;
	if (GetMultiModalityMultiSegmentation()){ 
	  BMSAutoSegMainFile<<"set (command_line WarpImageMultiTransform 3 ${MultiAtlasDir}atlas_image/${WarpedCase}_t1w${AtlasCaseExtension} ${WarpedAtlasTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}Warped.nii.gz -R ${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}_t1w${AtlasCaseExtension} ${DeformationFieldTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}TotalWarp.nii.gz ${DeformationFieldTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}TotalAffine.txt --use-BSpline)"<<std::endl; 
	  BMSAutoSegMainFile<<"Run (output ${command_line})"<<std::endl;
	  BMSAutoSegMainFile<<"set (command_line ${ImageMathCmd} ${WarpedAtlasTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}Warped.nii.gz -rescale 0,8192 -outfile ${WarpedAtlasTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}Warped.nii.gz)"<<std::endl; 
	  BMSAutoSegMainFile<<"Run (output ${command_line})"<<std::endl;
	}
	else{
	  BMSAutoSegMainFile<<"set (command_line WarpImageMultiTransform 3 ${MultiAtlasDir}atlas_image/${WarpedCase}${AtlasCaseExtension} ${WarpedAtlasTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}Warped.nii.gz -R ${MultiAtlasDir}atlas_image/${WarpedCaseForTarget}${AtlasCaseExtension} ${DeformationFieldTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}TotalWarp.nii.gz ${DeformationFieldTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}TotalAffine.txt --use-BSpline)"<<std::endl;  
	  BMSAutoSegMainFile<<"Run (output ${command_line})"<<std::endl;
	  BMSAutoSegMainFile<<"set (command_line ${ImageMathCmd} ${WarpedAtlasTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}Warped.nii.gz -rescale 0,8192 -outfile ${WarpedAtlasTrainToTrainDir}${WarpedCase}x${WarpedCaseForTarget}Warped.nii.gz)"<<std::endl;  
	  BMSAutoSegMainFile<<"Run (output ${command_line})"<<std::endl;
	}
	BMSAutoSegMainFile<<"      EndIf (${WarpedAtlasExistList})"<<std::endl;
	BMSAutoSegMainFile<<"      EndIf (${WarpedCase})"<<std::endl;
	BMSAutoSegMainFile<<"EndForEach (WarpedCase)"<<std::endl;
        BMSAutoSegMainFile<<"EndForEach (WarpedCaseForTarget)"<<std::endl;

        // ANTS registration target-atlas
        BMSAutoSegMainFile<<"set (Path ${DisplacementFieldDirectory})"<<std::endl;
        BMSAutoSegMainFile<<"ListFileInDir(ANTSOutputDisplacementFieldList ${Path})"<<std::endl;
        BMSAutoSegMainFile<<"   echo('atlas-atlas displacement directory; ' ${Path})"<<std::endl; 	  

        BMSAutoSegMainFile<<"set (WarpCmd WarpImageMultiTransform)"<<std::endl;
        BMSAutoSegMainFile<<"ForEach (AtlasCase ${AtlasCaseIDList}) "<<std::endl;
        //check the exist of ANTS registration
        BMSAutoSegMainFile<<"    	set (outputfilename atlas_${AtlasCase}x${TargetList}Total.nii.gz)"<<std::endl;
        BMSAutoSegMainFile<<"    	set (outputfilenameForCheck atlas_${AtlasCase}x${TargetList}TotalWarp.nii.gz)"<<std::endl;
        BMSAutoSegMainFile<<"    	set (FileExist 1)"<<std::endl;
        BMSAutoSegMainFile<<"    	set (FileExistFlag 0)"<<std::endl;
        BMSAutoSegMainFile<<"   ListFileInDir(WarpedFileExistList ${DisplacementFieldDirectory} ${outputfilenameForCheck})"<<std::endl;
        //BMSAutoSegMainFile<<"   echo(${DisplacementFieldDirectory} ${outputfilenameForCheck})"<<std::endl;
        BMSAutoSegMainFile<<"   If (${WarpedFileExistList} == '')"<<std::endl;
	if (GetMultiModalityMultiSegmentation()) {
	  BMSAutoSegMainFile<<"    set (command_line ${ANTSCmd} 3)"<<std::endl;
	  if (GetANTSCCWeight() > 0.01)
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m CC[${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile},${MultiAtlasDir}atlas_image/atlas_${AtlasCase}_t1w${AtlasCaseExtension},${ANTSCCWeight},${ANTSCCRegionRadius}])"<<std::endl;
	  if (GetANTSCCWeight2nd() > 0.01)
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m CC[${TargetPath}${PrefixMultiAtlas}${MultiAtlasT2TargetFile},${MultiAtlasDir}atlas_image/atlas_${AtlasCase}_t2w${AtlasCaseExtension},${ANTSCCWeight2nd},${ANTSCCRegionRadius2nd}])"<<std::endl;
	  if (GetANTSMIWeight() > 0.01)
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MI[${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile},${MultiAtlasDir}atlas_image/atlas_${AtlasCase}_t1w${AtlasCaseExtension},${ANTSMIWeight},${ANTSMIBins}])"<<std::endl;
	  if (GetANTSMIWeight2nd() > 0.01)
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MI[${TargetPath}${PrefixMultiAtlas}${MultiAtlasT2TargetFile},${MultiAtlasDir}atlas_image/atlas_${AtlasCase}_t2w${AtlasCaseExtension},${ANTSMIWeight2nd},${ANTSMIBins2nd}])"<<std::endl;
	  if (GetANTSMSQWeight() > 0.01)
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MSQ[${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile},${MultiAtlasDir}atlas_image/atlas_${AtlasCase}_t1w${AtlasCaseExtension}],${ANTSMSQWeight},0.01)"<<std::endl;
	  if (GetANTSMSQWeight2nd() > 0.01)
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MSQ[${TargetPath}${PrefixMultiAtlas}${MultiAtlasT2TargetFile},${MultiAtlasDir}atlas_image/atlas_${AtlasCase}_t2w${AtlasCaseExtension},${ANTSMSQWeight2nd},0.01])"<<std::endl;
	}
	else {
	  BMSAutoSegMainFile<<"    set (command_line ${ANTSCmd} 3)"<<std::endl;
	  if (GetANTSCCWeight() > 0.01)
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m CC[${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile},${MultiAtlasDir}atlas_image/atlas_${AtlasCase}${AtlasCaseExtension},${ANTSCCWeight},${ANTSCCRegionRadius}])"<<std::endl;
	  else if (GetANTSMIWeight() > 0.01)
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MI[${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile},${MultiAtlasDir}atlas_image/atlas_${AtlasCase}${AtlasCaseExtension},${ANTSMIWeight},${ANTSMIBins}])"<<std::endl;
	  else if (GetANTSMSQWeight() > 0.01)
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -m MSQ[${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile},${MultiAtlasDir}atlas_image/atlas_${AtlasCase}${AtlasCaseExtension}],${ANTSMSQWeight},0.01)"<<std::endl;
	  // ADDED by Yundi Shi to handle exception
	  else{
	    std::cerr<<"Error:   Please specify the metric used in ANTS by ssigning a non-zero weighting "<<itksysProcess_GetExceptionString(m_Process)<<"\n"<<std::endl;
	    //LogFile<<"Error:  Please specify the metric used in ANTS by ssigning a non-zero weighting "<<itksysProcess_GetExceptionString(m_Process)<<"\n"<<std::endl;
	  }
	    
	}
	BMSAutoSegMainFile<<"    	set (command_line ${command_line} -i ${ANTSIterations} -o ${DisplacementFieldDirectory}${outputfilename})"<<std::endl;
	if (std::strcmp(GetANTSRegistrationFilterType(), "GreedyDiffeomorphism") == 0)
	  {
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t SyN[${ANTSTransformationStep}])"<<std::endl;
	    if (GetANTSGaussianSmoothing())
	      BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[${ANTSGaussianSigma},0])"<<std::endl;      
	  }
	else if (std::strcmp(GetANTSRegistrationFilterType(), "SpatiotemporalDiffeomorphism") == 0)
	  {
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t SyN[${ANTSTransformationStep}])"<<std::endl;
	    if (GetANTSGaussianSmoothing())
	      BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[${ANTSGaussianSigma},0.5])"<<std::endl;      
	  }
	else if (std::strcmp(GetANTSRegistrationFilterType(), "Elastic") == 0)
	  {
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t Elast[${ANTSTransformationStep}])"<<std::endl;
	    if (GetANTSGaussianSmoothing())
	      BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[0,${ANTSGaussianSigma}])"<<std::endl;
	  }
	else if (std::strcmp(GetANTSRegistrationFilterType(), "Exponential") == 0)
	  {
	    BMSAutoSegMainFile<<"    	set (command_line ${command_line} -t Exp[${ANTSTransformationStep}])"<<std::endl;
	    if (GetANTSGaussianSmoothing())
	      BMSAutoSegMainFile<<"    	set (command_line ${command_line} -r Gauss[0,${ANTSGaussianSigma}])"<<std::endl;
	  }
	BMSAutoSegMainFile<<"    	set (command_line ${command_line} --use-all-metrics-for-convergence)"<<std::endl;
	BMSAutoSegMainFile<<"           Run (output ${command_line})"<<std::endl; // calculate intensity energy
        BMSAutoSegMainFile<<"      EndIf (${WarpedFileExistList})"<<std::endl;
        BMSAutoSegMainFile<<"EndForEach (AtlasCase)"<<std::endl;
        
        BMSAutoSegMainFile<<"set (OriginalAtlasImagePath ${MultiAtlasDir}atlas_image/)"<<std::endl;
        BMSAutoSegMainFile<<"ListFileInDir(AtlasImageLabelParcellationList ${OriginalAtlasImagePath})"<<std::endl;
        
	// Atlas image Warping
        BMSAutoSegMainFile<<"set (Path ${WarpedAtlasDir})"<<std::endl;
        BMSAutoSegMainFile<<"ListFileInDir(WarpedAtlasImageList ${Path})"<<std::endl;
        BMSAutoSegMainFile<<"ForEach (AtlasCase ${AtlasCaseIDList}) "<<std::endl;
	BMSAutoSegMainFile<<"   set (outputfilename atlas_${AtlasCase}_warped.nii.gz)"<<std::endl;
       
	BMSAutoSegMainFile<<"   set (FileExist 1)"<<std::endl;
	BMSAutoSegMainFile<<"   set (FileExistFlag 0)"<<std::endl;
        BMSAutoSegMainFile<<"   ListFileInDir(WarpedFileExistList ${WarpedAtlasDir} ${outputfilename})"<<std::endl;
        BMSAutoSegMainFile<<"   If (${WarpedFileExistList} == '')"<<std::endl;

	if (GetMultiModalityMultiSegmentation()) {
	  
	  BMSAutoSegMainFile<<"    	set (command_line ${WarpCmd} 3 ${MultiAtlasDir}atlas_image/atlas_${AtlasCase}_t1w${AtlasCaseExtension} ${WarpedAtlasDir}atlas_${AtlasCase}_warped.nii.gz -R ${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile} ${DeformationFieldDir}atlas_${AtlasCase}x${TargetList}TotalWarp.nii.gz ${DeformationFieldDir}atlas_${AtlasCase}x${TargetList}TotalAffine.txt)"<<std::endl;
	  BMSAutoSegMainFile<<"    	Run (output ${command_line})"<<std::endl; 
	  
	  BMSAutoSegMainFile<<"    	set (command_line ${ImageMathCmd} ${WarpedAtlasDir}atlas_${AtlasCase}_warped.nii.gz -rescale 0,8192 -outfile ${WarpedAtlasDir}atlas_${AtlasCase}_warped.nii.gz)"<<std::endl;  
	  BMSAutoSegMainFile<<"    	Run (output ${command_line})"<<std::endl;
	}
	else {
	  BMSAutoSegMainFile<<"      set (command_line ${WarpCmd} 3 ${MultiAtlasDir}atlas_image/atlas_${AtlasCase}${AtlasCaseExtension} ${WarpedAtlasDir}atlas_${AtlasCase}_warped.nii.gz -R ${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile} ${DeformationFieldDir}atlas_${AtlasCase}x${TargetList}TotalWarp.nii.gz ${DeformationFieldDir}atlas_${AtlasCase}x${TargetList}TotalAffine.txt)"<<std::endl;
	  BMSAutoSegMainFile<<"      Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"      set (command_line ${ImageMathCmd} ${WarpedAtlasDir}atlas_${AtlasCase}_warped.nii.gz -rescale 0,8192 -outfile ${WarpedAtlasDir}atlas_${AtlasCase}_warped.nii.gz)"<<std::endl; 
	  BMSAutoSegMainFile<<"      Run (output ${command_line})"<<std::endl; 
	}
	BMSAutoSegMainFile<<"      EndIf (${WarpedFileExistList})"<<std::endl;
        BMSAutoSegMainFile<<"EndForEach (AtlasCase)"<<std::endl;

        // Atlas label and parcellation Warping
        BMSAutoSegMainFile<<"set (Path ${WarpedLabelDir})"<<std::endl;
        BMSAutoSegMainFile<<"ListFileInDir(WarpedAtlasLabelList ${Path})"<<std::endl;
        BMSAutoSegMainFile<<"ForEach (AtlasCase ${AtlasCaseIDList})"<<std::endl;
	BMSAutoSegMainFile<<"   set (outputfilename label_${AtlasCase}_warped.nii.gz)"<<std::endl;
        BMSAutoSegMainFile<<"   ListFileInDir(WarpedFileExistList ${WarpedLabelDir} ${outputfilename})"<<std::endl;
        BMSAutoSegMainFile<<"   If (${WarpedFileExistList} == '')"<<std::endl;

	// does the label actually exist in the atlas folder, TODO: this should be checked before, so it does not need to be checked here
	BMSAutoSegMainFile<<"     set (OriginalAtlasImageExistFlag 0)"<<std::endl;
	BMSAutoSegMainFile<<"     set (OriginalAtlasImage label_${AtlasCase}${AtlasCaseExtension})"<<std::endl;
	BMSAutoSegMainFile<<"     ForEach (WarpedImageFilenameInList ${AtlasImageLabelParcellationList}) "<<std::endl;
	BMSAutoSegMainFile<<"       If (${WarpedImageFilenameInList} == ${OriginalAtlasImage})"<<std::endl;
	BMSAutoSegMainFile<<"         set (OriginalAtlasImageExistFlag 1)"<<std::endl;
	BMSAutoSegMainFile<<"       EndIf (${WarpedImageFilenameInList})"<<std::endl;
	BMSAutoSegMainFile<<"     EndForEach (WarpedImageFilenameInList)"<<std::endl;
	
	BMSAutoSegMainFile<<"     If (${OriginalAtlasImageExistFlag} == ${FileExist})"<<std::endl;
	BMSAutoSegMainFile<<"       set (command_line ${WarpCmd} 3 ${MultiAtlasDir}atlas_image/label_${AtlasCase}${AtlasLabelExtension} ${WarpedLabelDir}label_${AtlasCase}_warped.nii.gz -R ${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile} ${DeformationFieldDir}atlas_${AtlasCase}x${TargetList}TotalWarp.nii.gz ${DeformationFieldDir}atlas_${AtlasCase}x${TargetList}TotalAffine.txt --use-NN)"<<std::endl;
	BMSAutoSegMainFile<<"       Run (output ${command_line})"<<std::endl;
	BMSAutoSegMainFile<<"     EndIf (${OriginalAtlasImageExistFlag})"<<std::endl;
	BMSAutoSegMainFile<<"   EndIf (${WarpedFileExistList})"<<std::endl;
	
	BMSAutoSegMainFile<<"   set (outputfilename parcellation_${AtlasCase}_warped.nii.gz)"<<std::endl;
        BMSAutoSegMainFile<<"   ListFileInDir(WarpedFileExistList ${WarpedParcellationDir} ${outputfilename})"<<std::endl;
        BMSAutoSegMainFile<<"   If (${WarpedFileExistList} == '')"<<std::endl;

	BMSAutoSegMainFile<<"     set (OriginalAtlasImageExistFlag 0)"<<std::endl;
	BMSAutoSegMainFile<<"     set (OriginalAtlasImage parcellation_${AtlasCase}${AtlasCaseExtension})"<<std::endl;
	BMSAutoSegMainFile<<"     ForEach (WarpedImageFilenameInList ${AtlasImageLabelParcellationList}) "<<std::endl;
	BMSAutoSegMainFile<<"       If (${WarpedImageFilenameInList} == ${OriginalAtlasImage})"<<std::endl;
	BMSAutoSegMainFile<<"    	set (OriginalAtlasImageExistFlag 1)"<<std::endl;
	BMSAutoSegMainFile<<"       EndIf (${WarpedImageFilenameInList})"<<std::endl;
	BMSAutoSegMainFile<<"     EndForEach (WarpedImageFilenameInList)"<<std::endl;
	
	BMSAutoSegMainFile<<"     If (${OriginalAtlasImageExistFlag} == ${FileExist})"<<std::endl;
	BMSAutoSegMainFile<<"       set (command_line ${WarpCmd} 3 ${MultiAtlasDir}atlas_image/parcellation_${AtlasCase}${AtlasCaseExtension} ${WarpedParcellationDir}parcellation_${AtlasCase}_warped.nii.gz -R ${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile} ${DeformationFieldDir}atlas_${AtlasCase}x${TargetList}TotalWarp.nii.gz ${DeformationFieldDir}atlas_${AtlasCase}x${TargetList}TotalAffine.txt --use-NN)"<<std::endl;
	BMSAutoSegMainFile<<"       Run (output ${command_line})"<<std::endl;
	BMSAutoSegMainFile<<"     EndIf (${OriginalAtlasImageExistFlag})"<<std::endl;
	BMSAutoSegMainFile<<"   EndIf (${WarpedFileExistList})"<<std::endl;

        BMSAutoSegMainFile<<"EndForEach (AtlasCase)"<<std::endl;

        SetWarpedAtlasList(WarpedAtlasDirectory.c_str());
        SetWarpedAtlasTrainToTrainList(WarpedAtlasTrainToTrainDirectory.c_str());
        SetWarpedLabelList(WarpedLabelDirectory.c_str());
        SetDeformationFieldList(DeformationFieldDirectory.c_str());
        SetDeformationFieldTrainToTrainList(DeformationFieldTrainToTrainDirectory.c_str());

	if (!strcmp(GetLabelFusionAlgorithm(), "Weighted Majority Voting")) {
	  if (GetRecalculateAtlasTargetMultiAtlasEnergy()){
            BMSAutoSegMainFile<<"      ForEach (AtlasCase ${AtlasList}) "<<std::endl;
	    BMSAutoSegMainFile<<"    	 set (command_line rm ${IntEnergyDir}${AtlasCase}IntensityEnergy.txt)"<<std::endl;
	    BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
            BMSAutoSegMainFile<<"      EndForEach (AtlasCase)"<<std::endl;
            BMSAutoSegMainFile<<"      ForEach (AtlasCase ${AtlasList}) "<<std::endl;
	    BMSAutoSegMainFile<<"    	 set (command_line rm ${HarmonicEnergyDir}${AtlasCase}HarmonicEnergy.txt)"<<std::endl;
	    BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
            BMSAutoSegMainFile<<"      EndForEach (AtlasCase)"<<std::endl;
            BMSAutoSegMainFile<<"      set (command_line rm ${HarmonicEnergyDir}HarmonicEnergyNormalized.txt)"<<std::endl;
	    BMSAutoSegMainFile<<"      Run (output ${command_line})"<<std::endl; 
            BMSAutoSegMainFile<<"      set (command_line rm ${IntEnergyDir}IntensityEnergyNormalized.txt)"<<std::endl;
            BMSAutoSegMainFile<<"      Run (output ${command_line})"<<std::endl;
	  }

	  // Calculate energy atlas-atlas intensity
	  BMSAutoSegMainFile<<"    ForEach (AtlasCase ${AtlasList}) "<<std::endl;
	  BMSAutoSegMainFile<<"      ListFileInDir(EnergyList ${IntEnergyDir} ${AtlasCase}IntensityEnergy.txt)"<<std::endl;
	  BMSAutoSegMainFile<<"      If (${EnergyList} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"        ForEach (IntEnergyCaseTrainToTrain ${AtlasList}) "<<std::endl;
	  BMSAutoSegMainFile<<"          If (${IntEnergyCaseTrainToTrain} != ${AtlasCase})"<<std::endl;
	  if (GetMultiModalityMultiSegmentation()) {
	    BMSAutoSegMainFile<<"          set (command_line ${MultiAtlasSegCmd} -p ${WarpedAtlasTrainToTrainDir}${IntEnergyCaseTrainToTrain}x${AtlasCase}Warped.nii.gz ${MultiAtlasDir}atlas_image/${AtlasCase}_t1w${AtlasCaseExtension} ${IntEnergyDir}${AtlasCase}IntensityEnergy.txt ${NumberAtlas} ${NumberCase})"<<std::endl; 
	    BMSAutoSegMainFile<<"          Run (output ${command_line})"<<std::endl; 
	  } else {
	    BMSAutoSegMainFile<<"          set (command_line ${MultiAtlasSegCmd} -p ${WarpedAtlasTrainToTrainDir}${IntEnergyCaseTrainToTrain}x${AtlasCase}Warped.nii.gz ${MultiAtlasDir}atlas_image/${AtlasCase}${AtlasCaseExtension} ${IntEnergyDir}${AtlasCase}IntensityEnergy.txt ${NumberAtlas} ${NumberCase})"<<std::endl; 
	    BMSAutoSegMainFile<<"          Run (output ${command_line})"<<std::endl; 
	  }
	  BMSAutoSegMainFile<<"          EndIf (${IntEnergyCaseTrainToTrain})"<<std::endl;
	  BMSAutoSegMainFile<<"        EndForEach (IntEnergyCaseTrainToTrain)"<<std::endl;
	  BMSAutoSegMainFile<<"      EndIf (${EnergyList})"<<std::endl;
	  
	  BMSAutoSegMainFile<<"    EndForEach (AtlasCase)"<<std::endl;
	  
	  
	  // Calculate energy atlas-atlas harmonic
	  BMSAutoSegMainFile<<"   ForEach (TargetCase ${AtlasList}) "<<std::endl;
	  BMSAutoSegMainFile<<"      ListFileInDir(EnergyList ${HarmonicEnergyDir} ${TargetCase}HarmonicEnergy.txt)"<<std::endl;
	  BMSAutoSegMainFile<<"      If (${EnergyList} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"        ForEach (AtlasCase ${AtlasList}) "<<std::endl;
	  BMSAutoSegMainFile<<"          If (${AtlasCase} != ${TargetCase})"<<std::endl;
	  if (GetMultiModalityMultiSegmentation()) {
	    BMSAutoSegMainFile<<"          set (command_line ${MultiAtlasSegCmd} -e ${DeformationFieldTrainToTrainDir}${TargetCase}x${AtlasCase}TotalWarp.nii.gz ${HarmonicEnergyDir}${TargetCase}HarmonicEnergy.txt ${NumberAtlas} ${NumberCase})"<<std::endl; 
	    BMSAutoSegMainFile<<"          Run (output ${command_line})"<<std::endl; 
	  } else {
	    BMSAutoSegMainFile<<"          set (command_line ${MultiAtlasSegCmd} -e ${DeformationFieldTrainToTrainDir}${TargetCase}x${AtlasCase}TotalWarp.nii.gz ${HarmonicEnergyDir}${TargetCase}HarmonicEnergy.txt ${NumberAtlas} ${NumberCase})"<<std::endl; 
	    BMSAutoSegMainFile<<"          Run (output ${command_line})"<<std::endl; 
	  }
	  BMSAutoSegMainFile<<"          EndIf (${AtlaseCase})"<<std::endl;
	  BMSAutoSegMainFile<<"        EndForEach (AtlasCase)"<<std::endl;
	  BMSAutoSegMainFile<<"      EndIf (${EnergyList})"<<std::endl;
	  BMSAutoSegMainFile<<"    EndForEach (TargetCase)"<<std::endl;
	  
	  if (GetRecalculateAtlasTargetMultiAtlasEnergy()){
	    BMSAutoSegMainFile<<"    set (command_line rm ${IntEnergyDir}intensityEnergy.txt)"<<std::endl;
	    BMSAutoSegMainFile<<"    Run (output ${command_line})"<<std::endl; 
	    
	    BMSAutoSegMainFile<<"    set (command_line rm ${HarmonicEnergyDir}harmonicEnergy.txt)"<<std::endl;
	    BMSAutoSegMainFile<<"    Run (output ${command_line})"<<std::endl; 
	    BMSAutoSegMainFile<<"    set (command_line rm ${TemplateDir}${TargetList}template.txt)"<<std::endl;
	    BMSAutoSegMainFile<<"    Run (output ${command_line})"<<std::endl; // calculate intensity energy
	  }
	  
	  // Calculate energy atlas-target intensity
	  BMSAutoSegMainFile<<"      ListFileInDir(EnergyList ${IntEnergyDir} intensityEnergy.txt)"<<std::endl;
	  BMSAutoSegMainFile<<"      If (${EnergyList} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"        ForEach (IntEnergyCase ${AtlasCaseIDList}) "<<std::endl;
	  BMSAutoSegMainFile<<"          set (command_line ${MultiAtlasSegCmd} -p ${TargetPath}${PrefixMultiAtlas}${MultiAtlasTargetFile} ${WarpedAtlasDir}atlas_${IntEnergyCase}_warped.nii.gz ${IntEnergyDir}intensityEnergy.txt ${NumberAtlas} ${NumberCase})"<<std::endl; 
	  BMSAutoSegMainFile<<"          Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"        EndForEach (IntEnergyCase)"<<std::endl;
	  BMSAutoSegMainFile<<"      EndIf (${EnergyList})"<<std::endl;
	  
	  // normalize energy atlas-atlas intensity; 
	  BMSAutoSegMainFile<<"    ListFileInDir(EnergyList ${IntEnergyDir} IntensityEnergyNormalized.txt)"<<std::endl;
	  BMSAutoSegMainFile<<"    If (${EnergyList} == '')"<<std::endl; 
	  BMSAutoSegMainFile<<"        ForEach (AtlasCase ${AtlasList}) "<<std::endl;
	  
	  BMSAutoSegMainFile<<"          set (command_line ${MultiAtlasSegCmd} -n ${IntEnergyDir}${AtlasCase}IntensityEnergy.txt  ${IntEnergyDir}IntensityEnergyNormalized.txt ${NumberAtlas} ${NumberCase})"<<std::endl;
 
	  BMSAutoSegMainFile<<"          Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"        EndForEach (AtlasCase)"<<std::endl;
	  BMSAutoSegMainFile<<"        set (command_line ${MultiAtlasSegCmd} -n ${IntEnergyDir}intensityEnergy.txt  ${IntEnergyDir}IntensityEnergyNormalized.txt ${NumberAtlas} ${NumberCase})"<<std::endl;
 
	  BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl;
	  BMSAutoSegMainFile<<"    EndIf (${EnergyList})"<<std::endl;
	  
	  // Calculate energy  harmonic
	  BMSAutoSegMainFile<<"      ListFileInDir(EnergyList ${HarmonicEnergyDir} harmonicEnergy.txt)"<<std::endl;
	  BMSAutoSegMainFile<<"      If (${EnergyList} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"        ForEach (HarmonicEnergyCase ${AtlasCaseIDList}) "<<std::endl;
	  BMSAutoSegMainFile<<"          set (command_line ${MultiAtlasSegCmd} -e ${DeformationFieldDir}atlas_${HarmonicEnergyCase}x${TargetCaseIDList}TotalWarp.nii.gz ${HarmonicEnergyDir}harmonicEnergy.txt ${NumberAtlas} ${NumberCase})"<<std::endl; 
	  BMSAutoSegMainFile<<"          Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"        EndForEach (HarmonicEnergyCase)"<<std::endl;
	  BMSAutoSegMainFile<<"      EndIf (${EnergyList})"<<std::endl;
	  
	  // normalize energy  harmonic; 
	  BMSAutoSegMainFile<<"    ListFileInDir(EnergyList ${HarmonicEnergyDir} HarmonicEnergyNormalized.txt)"<<std::endl;
	  BMSAutoSegMainFile<<"    If (${EnergyList} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"      ForEach (TargetCase ${AtlasList}) "<<std::endl;
	  BMSAutoSegMainFile<<"        set (command_line ${MultiAtlasSegCmd} -n ${HarmonicEnergyDir}${TargetCase}HarmonicEnergy.txt ${HarmonicEnergyDir}HarmonicEnergyNormalized.txt ${NumberAtlas} ${NumberCase})"<<std::endl; 
	  BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"      EndForEach (TargetCase)"<<std::endl;
	  BMSAutoSegMainFile<<"      set (command_line ${MultiAtlasSegCmd} -n ${HarmonicEnergyDir}harmonicEnergy.txt ${HarmonicEnergyDir}HarmonicEnergyNormalized.txt ${NumberAtlas} ${NumberCase})"<<std::endl; 
	  BMSAutoSegMainFile<<"      Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"    EndIf (${EnergyList})"<<std::endl;
	  
	  // Template selection
	  BMSAutoSegMainFile<<"      ListFileInDir(TemplateList ${TemplateDir} ${TargetList}template.txt)"<<std::endl;
	  BMSAutoSegMainFile<<"      If (${TemplateList} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"        set (command_line ${MultiAtlasSegCmd} -g ${IntEnergyDir}IntensityEnergyNormalized.txt ${HarmonicEnergyDir}HarmonicEnergyNormalized.txt ${TemplateDir}${TargetList}template.txt ${WeightIntensityEnergy} ${WeightHarmonicEnergy} ${WeightShapeEnergy} ${NumberAtlas} ${NumberCase})"<<std::endl; 
	  BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"      EndIf (${TemplateList})"<<std::endl;
	}
          
	// Label Fusion
	if (!strcmp(GetLabelFusionAlgorithm(), "Weighted Majority Voting")) {
	  BMSAutoSegMainFile<<"      ListFileInDir(FusionList ${FusedLabelDir} ${TargetList}_label.nrrd)"<<std::endl;
	  BMSAutoSegMainFile<<"      If (${FusionList} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"        set (command_line ${MultiAtlasSegCmd} -v ${IntEnergyDir}IntensityEnergyNormalized.txt ${HarmonicEnergyDir}HarmonicEnergyNormalized.txt ${TemplateDir}${TargetList}template.txt ${WarpedLabelDir} ${FusedLabelDir} ${TargetList}_label.nrrd label ${WeightIntensityEnergy} ${WeightHarmonicEnergy} ${WeightShapeEnergy} ${NumberAtlas} ${NumberCase})"<<std::endl; 
	  BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl;
	  BMSAutoSegMainFile<<"      Else ()"<<std::endl;
	  BMSAutoSegMainFile<<"        echo ('File already exists: '${TargetList}_label.nrrd)"<<std::endl; 
	  BMSAutoSegMainFile<<"      EndIf (${FusionList})"<<std::endl;
	  
	  if (GetNbAtlasParcellation() > 0 ) {
	    BMSAutoSegMainFile<<"      ListFileInDir(FusionList ${FusedLabelDir} ${TargetList}_parcellation.nrrd)"<<std::endl;
	    BMSAutoSegMainFile<<"      If (${FusionList} == '')"<<std::endl;
	    BMSAutoSegMainFile<<"        set (command_line ${MultiAtlasSegCmd} -v ${IntEnergyDir}IntensityEnergyNormalized.txt ${HarmonicEnergyDir}HarmonicEnergyNormalized.txt ${TemplateDir}${TargetList}template.txt ${WarpedParcellationDir} ${FusedLabelDir} ${TargetList}_parcellation.nrrd parce ${WeightIntensityEnergy} ${WeightHarmonicEnergy} ${WeightShapeEnergy} ${NumberAtlas} ${NumberCase})"<<std::endl; 
	    BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
	    BMSAutoSegMainFile<<"      EndIf (${FusionList})"<<std::endl;
	  }
	}

	if (!strcmp(GetLabelFusionAlgorithm(), "Majority Voting")) {
	  BMSAutoSegMainFile<<"      ListFileInDir(FusionList ${FusedLabelDir} ${TargetList}_label.nrrd)"<<std::endl;
	  BMSAutoSegMainFile<<"      If (${FusionList} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"        set (command_line ${MultiAtlasSegCmd} -m ${IntEnergyDir}IntensityEnergyNormalized.txt ${HarmonicEnergyDir}HarmonicEnergyNormalized.txt ${TemplateDir}${TargetList}template.txt ${WarpedLabelDir} ${FusedLabelDir} ${TargetList}_label.nrrd label ${WeightIntensityEnergy} ${WeightHarmonicEnergy} ${WeightShapeEnergy} ${NumberAtlas} ${NumberCase})"<<std::endl; 
	  BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"      Else ()"<<std::endl;
	  BMSAutoSegMainFile<<"        echo ('File already exists: '${TargetList}_label.nrrd)"<<std::endl;
	  BMSAutoSegMainFile<<"      EndIf (${FusionList})"<<std::endl;

	  if (GetNbAtlasParcellation() > 0 ) {
	    BMSAutoSegMainFile<<"      ListFileInDir(FusionList ${FusedLabelDir} ${TargetList}_parcellation.nrrd)"<<std::endl;
	    BMSAutoSegMainFile<<"      If (${FusionList} == '')"<<std::endl;
	    BMSAutoSegMainFile<<"        set (command_line ${MultiAtlasSegCmd} -m ${IntEnergyDir}IntensityEnergyNormalized.txt ${HarmonicEnergyDir}HarmonicEnergyNormalized.txt ${TemplateDir}${TargetList}template.txt ${WarpedParcellationDir} ${FusedLabelDir} ${TargetList}_parcellation.nrrd parce ${WeightIntensityEnergy} ${WeightHarmonicEnergy} ${WeightShapeEnergy} ${NumberAtlas} ${NumberCase})"<<std::endl;
	    BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
	    BMSAutoSegMainFile<<"      EndIf (${FusionList})"<<std::endl; 
	  }
	}
     }
  }
 
  if (GetComputeVolume())
  {
    bool IsListEmpty = true;

    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 15. Volume information computation"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;  
    BMSAutoSegMainFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMainFile<<"echo ('VOLUME COMPUTATION...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
      
    BMSAutoSegMainFile<<"set (VolumeDir ${ProcessingDir}AutoSeg_Volume/)"<<std::endl;
    BMSAutoSegMainFile<<"ListDirInDir(VolumeDirList ${ProcessingDir} AutoSeg_Volume)"<<std::endl;
    BMSAutoSegMainFile<<"If (${VolumeDirList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"  MakeDirectory(${VolumeDir})"<<std::endl;
    BMSAutoSegMainFile<<"EndIf (${VolumeDirList})"<<std::endl;    

    BMSAutoSegMainFile<<"echo ('Tissue segmentation volume analysis:')"<<std::endl;  
    BMSAutoSegMainFile<<"set (VolumeFile ${VolumeDir}AutoSeg_TissueSegmentationVolume.csv)"<<std::endl;  
    BMSAutoSegMainFile<<"GetFileName(Path ${VolumeFile} PATH)"<<std::endl; 
    BMSAutoSegMainFile<<"GetFileName(VolumeFileTail ${VolumeFile} NAME)"<<std::endl;

    BMSAutoSegMainFile<<"WriteFile(${VolumeFile} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
    BMSAutoSegMainFile<<"AppendFile(${VolumeFile} '##################################\\n')"<<std::endl;
    BMSAutoSegMainFile<<"AppendFile(${VolumeFile} '# Tissue segmentation volume analysis:\\n')"<<std::endl;
    BMSAutoSegMainFile<<"AppendFile(${VolumeFile} '##################################\\n\\n')"<<std::endl;
    if (std::strcmp(GetEMSoftware(), "neoseg") == 0)  {
      BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} 'Case,Volume,MWM,WM,GM,CSF\\n')"<<std::endl;
    } else if (std::strcmp(GetEMSoftware(), "ABC") == 0) {
      // check for number of priors
      if (m_NbTissueClass == 4) {
	BMSAutoSegMainFile<<"AppendFile(${VolumeFile} 'Case,Volume,WM,GM,CSF\\n')"<<std::endl;
      } else {
	std::string tissueClassString = std::string("'Case,Volume");
	for (int classID = 1; classID < m_NbTissueClass; classID++) {
	  char classIDstring[100];
	  sprintf(classIDstring,"%d",classID);

	  tissueClassString = tissueClassString + std::string(",") + std::string(classIDstring);
	}
	tissueClassString = tissueClassString + std::string("\\n')");
	BMSAutoSegMainFile<<"AppendFile(${VolumeFile} " << tissueClassString <<std::endl;
      }

    } else {
      BMSAutoSegMainFile<<"AppendFile(${VolumeFile} 'Case,Volume,WM,GM,CSF\\n')"<<std::endl;
    }
    BMSAutoSegMainFile<<"set (files_to_cat '')"<<std::endl;
    BMSAutoSegMainFile<<"set (CaseNumber 0)"<<std::endl;
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;

    BMSAutoSegMainFile<<"      set (TargetPath ${T1Path}/${AutoSegDir}/fused-labels/)"<<std::endl;
    
    if (GetMultiModalityMultiSegmentation()) { 
      
      BMSAutoSegMainFile<<"      set (MultiAtlasTarget ${TargetPath}${T1CaseHead}_label.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      set (MultiAtlasTargetHead ${TargetPath}${T1CaseHead}_label)"<<std::endl;
    }

    BMSAutoSegMainFile<<"      echo( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;	  
    BMSAutoSegMainFile<<"      set (SkullStrippedImage ${T1Path}/${AutoSegDir}/Stripped/${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;

    if(GetLoop() && std::strcmp(GetEMSoftware(), "neoseg") != 0)
      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
    else
      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;

    

    if (IsT1LabelEMSFile)
      BMSAutoSegMainFile<<"      set (SegmentedFile ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
    else
    {
      BMSAutoSegMainFile<<"      GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      set (SegmentedFile ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;      
    }
    BMSAutoSegMainFile<<"      GetFilename (SegmentedFileHead ${SegmentedFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"      #Computing Statistics"<<std::endl;
    BMSAutoSegMainFile<<"      echo ('Computing Volume information...')"<<std::endl;
    BMSAutoSegMainFile<<"      set (CSVFile ${EMSPath}${SegmentedFileHead}_volumeSummary.csv)"<<std::endl;
    BMSAutoSegMainFile<<"      ListFileInDir(FileList ${EMSPath} ${SegmentedFileHead}_volumeSummary.csv)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${FileList} == '')"<<std::endl;
    
    BMSAutoSegMainFile<<"        set (command_line ${ImageStatCmd} ${SkullStrippedImage} -label ${SegmentedFile} -volumeSummary  -outbase ${EMSPath}${SegmentedFileHead})"<<std::endl;
    BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
    BMSAutoSegMainFile<<"      EndIf(${FileList})"<<std::endl<<std::endl<<std::endl;
    BMSAutoSegMainFile<<"      If (${files_to_cat} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         set (files_to_cat ${CSVFile})"<<std::endl;
    BMSAutoSegMainFile<<"      Else (${files_to_cat})"<<std::endl;
    BMSAutoSegMainFile<<"         set (files_to_cat ${files_to_cat} ${CSVFile})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf(${files_to_cat})"<<std::endl<<std::endl<<std::endl;
    
    BMSAutoSegMainFile<<"      Inc(${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMainFile<<"      Int(${CaseNumber})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('Gathering tissue volume information for the whole dataset...')"<<std::endl;
    BMSAutoSegMainFile<<"If (${files_to_cat} == '')"<<std::endl;
    BMSAutoSegMainFile<<"     echo ('no volume files!!')"<<std::endl;
    BMSAutoSegMainFile<<"Else (${files_to_cat})"<<std::endl;
    BMSAutoSegMainFile<<"     set (command_line cat ${files_to_cat})"<<std::endl;
    BMSAutoSegMainFile<<"     Run (output ${command_line})"<<std::endl; 
    BMSAutoSegMainFile<<"     AppendFile(${VolumeFile} ${output})"<<std::endl;
    BMSAutoSegMainFile<<"EndIf(${files_to_cat})"<<std::endl<<std::endl<<std::endl;	  
    
      
    if (GetSubcorticalStructureSegmentation())
    {
      if (!IsStructureListEmpty)
      {
	BMSAutoSegMainFile<<"set(List ${StructureList})"<<std::endl;
	IsListEmpty = false;
      }
      if (!IsVentricleListEmpty)
      {
	if (IsListEmpty)
	  BMSAutoSegMainFile<<"set(List ${VentricleList})"<<std::endl;
	else
	  BMSAutoSegMainFile<<"set(List ${List} ${VentricleList})"<<std::endl;
	IsListEmpty = false;	
      }
	  
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"echo ('Subcortical Structures volume analysis:')"<<std::endl;  
      BMSAutoSegMainFile<<"set (VolumeFile ${VolumeDir}AutoSeg_SubcorticalStructureVolume.csv)"<<std::endl;  
      BMSAutoSegMainFile<<"GetFileName(Path ${VolumeFile} PATH)"<<std::endl; 
      BMSAutoSegMainFile<<"GetFileName(VolumeFileTail ${VolumeFile} NAME)"<<std::endl;
      BMSAutoSegMainFile<<"ListFileInDir(FileList ${Path} ${VolumeFileTail})"<<std::endl;
      BMSAutoSegMainFile<<"If (${FileList} == '' || ${StructureComputed} == 1 || ${VentricleComputed} == 1)"<<std::endl;

      BMSAutoSegMainFile<<"   WriteFile(${VolumeFile} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
      BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} '##################################\\n')"<<std::endl;
      BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} '# Subcortical structures volume analysis:\\n')"<<std::endl;
      BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} '##################################\\n\\n')"<<std::endl;
      BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} 'Case,Volume,amygdalaLeft,amygdalaRight,caudateLeft,caudateRight,hippoLeft,hippoRight,latVentLeft,latVentRight,pallidusLeft,pallidusRight,putamenLeft,putamenRight\\n')"<<std::endl;

      BMSAutoSegMainFile<<"   set (files_to_cat '')"<<std::endl;
      BMSAutoSegMainFile<<"   ForEach (T1Case ${T1CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
      BMSAutoSegMainFile<<"      set (AllROIFileTail AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${IRescaled}-AllROI.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      set (AllROIFile ${WarpROIPath}${AllROIFileTail})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (AllROIFileHead ${AllROIFile} NAME_WITHOUT_EXTENSION)"<<std::endl;

      BMSAutoSegMainFile<<"      #Computing Statistics"<<std::endl;
      BMSAutoSegMainFile<<"      echo ('Computing Volume information...')"<<std::endl;
      BMSAutoSegMainFile<<"      set (SkullStrippedImage ${T1Path}/${AutoSegDir}/Stripped/${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      
      BMSAutoSegMainFile<<"      ListFileInDir(FileList ${WarpROIPath} ${AllROIFileHead}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${FileList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"        set (command_line ${ImageStatCmd} ${SkullStrippedImage} -label ${AllROIFile} -volumeSummary -outbase ${WarpROIPath}${AllROIFileHead})"<<std::endl;	
      BMSAutoSegMainFile<<"        Run (output ${command_line})"<<std::endl; 
      BMSAutoSegMainFile<<"      EndIf(${FileList})"<<std::endl<<std::endl<<std::endl;
      BMSAutoSegMainFile<<"      set (CSVFile ${WarpROIPath}${AllROIFileHead}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${files_to_cat} == '')"<<std::endl;
      BMSAutoSegMainFile<<"         set (files_to_cat ${CSVFile})"<<std::endl;
      BMSAutoSegMainFile<<"      Else (${files_to_cat})"<<std::endl;
      BMSAutoSegMainFile<<"         set (files_to_cat ${files_to_cat} ${CSVFile})"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf(${files_to_cat})"<<std::endl<<std::endl<<std::endl;
      BMSAutoSegMainFile<<"   EndForEach (T1Case)"<<std::endl;
      BMSAutoSegMainFile<<"   echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"   echo ('Gathering subcortical volume information for the whole dataset...')"<<std::endl;
      BMSAutoSegMainFile<<"   If (${files_to_cat} != '')"<<std::endl;
      BMSAutoSegMainFile<<"     set (command_line cat ${files_to_cat})"<<std::endl;
      BMSAutoSegMainFile<<"     Run (output ${command_line})"<<std::endl; 
      BMSAutoSegMainFile<<"     AppendFile(${VolumeFile} ${output})"<<std::endl;
      BMSAutoSegMainFile<<"   EndIf(${files_to_cat})"<<std::endl<<std::endl<<std::endl;

      //BMSAutoSegMainFile<<"Else ()"<<std::endl;
      //     BMSAutoSegMainFile<<"   echo ('Volume File already exists!')"<<std::endl;
      BMSAutoSegMainFile<<"EndIf()"<<std::endl<<std::endl<<std::endl;
    }

    if (GetGenericROISegmentation())
    {
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
      BMSAutoSegMainFile<<"echo ('Generic ROI map volume analysis:')"<<std::endl;  
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;  

      BMSAutoSegMainFile<<"ForEach(GenericROIMap ${GenericROIMapList})"<<std::endl;
      BMSAutoSegMainFile<<"  GetFilename (GenericROIMapName ${GenericROIMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"  set (VolumeFile ${VolumeDir}AutoSeg_${GenericROIMapName}_Volume.csv)"<<std::endl;  
      BMSAutoSegMainFile<<"  GetFileName(Path ${VolumeFile} PATH)"<<std::endl; 
      BMSAutoSegMainFile<<"  GetFileName(VolumeFileTail ${VolumeFile} NAME)"<<std::endl;
      BMSAutoSegMainFile<<"  echo (Generic ROI File: ${GenericROIMapName}:)"<<std::endl;
      BMSAutoSegMainFile<<"  ListFileInDir(FileList ${Path} ${VolumeFileTail})"<<std::endl;
      BMSAutoSegMainFile<<"  If (${FileList} == '' || ${GenericROIMapComputed} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"     WriteFile(${VolumeFile} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
      BMSAutoSegMainFile<<"     AppendFile(${VolumeFile} '##################################\\n')"<<std::endl;
      BMSAutoSegMainFile<<"     AppendFile(${VolumeFile} '# Generic ROI map volume analysis:\\n')"<<std::endl;
      BMSAutoSegMainFile<<"     AppendFile(${VolumeFile} '##################################\\n\\n')"<<std::endl;
      BMSAutoSegMainFile<<"     set (files_to_cat '')"<<std::endl;
      BMSAutoSegMainFile<<"     ForEach (T1Case ${T1CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"       GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"       GetFilename (T1Dir ${T1Path} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"       GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"       echo( )"<<std::endl;
      BMSAutoSegMainFile<<"       echo('Case Number: '${T1CaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"       echo( )"<<std::endl;	  
      BMSAutoSegMainFile<<"       set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
      BMSAutoSegMainFile<<"       set (File ${WarpROIPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}_corrected_EMS--${GenericROIMapName}-WarpReg.nrrd )"<<std::endl;

      BMSAutoSegMainFile<<"       GetFilename (FileHead ${File} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"       #Computing Statistics"<<std::endl;
      BMSAutoSegMainFile<<"       echo ('Computing Volume information...')"<<std::endl;
      BMSAutoSegMainFile<<"       set (SkullStrippedImage ${T1Path}/${AutoSegDir}/Stripped/${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"       ListFileInDir(VolList ${WarpROIPath} ${FileHead}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"       If (${VolList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"         set (command_line ${ImageStatCmd} ${SkullStrippedImage} -label ${File} -volumeSummary  -outbase ${WarpROIPath}${FileHead})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (output ${command_line})"<<std::endl; 
      BMSAutoSegMainFile<<"       EndIf(${VolList})"<<std::endl<<std::endl<<std::endl;
      BMSAutoSegMainFile<<"       set (CSVFile ${WarpROIPath}${FileHead}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"       If (${files_to_cat} == '')"<<std::endl;
      BMSAutoSegMainFile<<"         set (files_to_cat ${CSVFile})"<<std::endl;
      BMSAutoSegMainFile<<"       Else (${files_to_cat})"<<std::endl;
      BMSAutoSegMainFile<<"         set (files_to_cat ${files_to_cat} ${CSVFile})"<<std::endl;
      BMSAutoSegMainFile<<"       EndIf(${files_to_cat})"<<std::endl<<std::endl<<std::endl;

      BMSAutoSegMainFile<<"     EndForEach (T1Case)"<<std::endl;
      BMSAutoSegMainFile<<"    echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"    echo ('Gathering volume information for the whole dataset...')"<<std::endl;
      BMSAutoSegMainFile<<"    If (${files_to_cat} != '')"<<std::endl;
      BMSAutoSegMainFile<<"      set (command_line cat ${files_to_cat})"<<std::endl;
      BMSAutoSegMainFile<<"      Run (output ${command_line})"<<std::endl; 
      BMSAutoSegMainFile<<"      AppendFile(${VolumeFile} ${output})"<<std::endl;
      BMSAutoSegMainFile<<"    EndIf(${files_to_cat})"<<std::endl<<std::endl<<std::endl;
      //     BMSAutoSegMainFile<<"  Else ()"<<std::endl;
      //     BMSAutoSegMainFile<<"    echo (${VolumeFileTail} already exists!)"<<std::endl;
      BMSAutoSegMainFile<<"  EndIf(${FileList})"<<std::endl<<std::endl<<std::endl;
      BMSAutoSegMainFile<<"EndForEach(GenericROIMap)"<<std::endl;
    }


    if (GetParcellationMapSegmentation())
    {
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;  
      BMSAutoSegMainFile<<"echo ('Parcellation map volume analysis:')"<<std::endl;
      BMSAutoSegMainFile<<"echo ( )"<<std::endl;  

      BMSAutoSegMainFile<<"ForEach(ParcellationMap ${ParcellationMapList})"<<std::endl;
      BMSAutoSegMainFile<<" GetFilename (ParcellationMapName ${ParcellationMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<" echo ('Parcellation File: '${ParcellationMapName})"<<std::endl;
      if (std::strcmp(GetEMSoftware(), "neoseg") == 0) 
	{
	  BMSAutoSegMainFile<<" set (VolumeFileMWM ${VolumeDir}AutoSeg_${ParcellationMapName}_Volume_MWM.csv)"<<std::endl;
	}
      BMSAutoSegMainFile<<" set (VolumeFileWM ${VolumeDir}AutoSeg_${ParcellationMapName}_Volume_WM.csv)"<<std::endl;
      BMSAutoSegMainFile<<" set (VolumeFileGM ${VolumeDir}AutoSeg_${ParcellationMapName}_Volume_GM.csv)"<<std::endl;
      BMSAutoSegMainFile<<" set (VolumeFileCSF ${VolumeDir}AutoSeg_${ParcellationMapName}_Volume_CSF.csv)"<<std::endl;
      if (m_NbTissueClass > 4) 
	{
	  for (int classID = 1; classID < m_NbTissueClass; classID++) 
	    {  
	      char classIDstring[100];
	      sprintf(classIDstring,"label_%d",classID);
	      BMSAutoSegMainFile<<" set (VolumeFile" << classIDstring <<" ${VolumeDir}AutoSeg_${ParcellationMapName}_Volume_" << classIDstring << ".csv)"<<std::endl;
	    }
	}

      //check file existance
      if (std::strcmp(GetEMSoftware(), "neoseg") == 0)	{
	BMSAutoSegMainFile<<" GetFileName(Path ${VolumeFileMWM} PATH)"<<std::endl;
	BMSAutoSegMainFile<<" GetFileName(VolumeFileTail ${VolumeFileMWM} NAME)"<<std::endl;
      }
      BMSAutoSegMainFile<<" GetFileName(Path ${VolumeFileWM} PATH)"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(VolumeFileTail ${VolumeFileWM} NAME)"<<std::endl;
      BMSAutoSegMainFile<<" ListFileInDir(FileListWM ${Path} ${VolumeFileTail})"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(Path ${VolumeFileGM} PATH)"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(VolumeFileTail ${VolumeFileGM} NAME)"<<std::endl;
      BMSAutoSegMainFile<<" ListFileInDir(FileListGM ${Path} ${VolumeFileTail})"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(Path ${VolumeFileCSF} PATH)"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(VolumeFileTail ${VolumeFileCSF} NAME)"<<std::endl;
      BMSAutoSegMainFile<<" ListFileInDir(FileListCSF ${Path} ${VolumeFileTail})"<<std::endl;
      if (m_NbTissueClass > 4) 	{
	for (int classID = 1; classID < m_NbTissueClass; classID++)  {  
	  char classIDstring[100];
	  sprintf(classIDstring,"label_%d",classID);
	  BMSAutoSegMainFile<<" GetFileName(Path ${VolumeFile" << classIDstring <<"} PATH)"<<std::endl;
	  BMSAutoSegMainFile<<" GetFileName(VolumeFileTail ${VolumeFile" << classIDstring <<"} NAME)"<<std::endl;
	  BMSAutoSegMainFile<<" ListFileInDir(FileList" << classIDstring <<" ${Path} ${VolumeFileTail})"<<std::endl;
	}
      }

      // write headers and set flags
      BMSAutoSegMainFile<<"  set (flagMWM 0)"<<std::endl;
      BMSAutoSegMainFile<<"  set (flagWM 0)"<<std::endl;
      BMSAutoSegMainFile<<"  set (flagGM 0)"<<std::endl;
      BMSAutoSegMainFile<<"  set (flagCSF 0)"<<std::endl;
      if (m_NbTissueClass > 4) 	{
	for (int classID = 1; classID < m_NbTissueClass; classID++) {
	  char classIDstring[100];
	  sprintf(classIDstring,"label_%d",classID);
	  BMSAutoSegMainFile<<"  set (flag" << classIDstring <<" 0)"<<std::endl;
	}
      }

      if (std::strcmp(GetEMSoftware(), "neoseg") == 0)	{
	BMSAutoSegMainFile<<" If (${ParcellationMapComputed} == 1 || ${FileListMWM} == '' || ${FileListWM} == '' || ${FileListGM} == '' || ${FileListCSF} == '')"<<std::endl;	
	
	BMSAutoSegMainFile<<"  If (${FileListMWM} == '')"<<std::endl;
	BMSAutoSegMainFile<<"   set (flagMWM 1)"<<std::endl;
	BMSAutoSegMainFile<<"   WriteFile(${VolumeFileMWM} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileMWM} '################################################\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileMWM} '# Parcellation map volume analysis: Myelinated White Matter\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileMWM} '################################################\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   set (files_to_cat_MWM '')"<<std::endl;
	BMSAutoSegMainFile<<"  EndIf (${FileListMWM})"<<std::endl;  
      } else {
	BMSAutoSegMainFile<<" If (${ParcellationMapComputed} == 1 || ${FileListWM} == '' || ${FileListGM} == '' || ${FileListCSF} == '')"<<std::endl;
      }

      if (m_NbTissueClass == 4 || (std::strcmp(GetEMSoftware(), "neoseg") == 0)) {
	BMSAutoSegMainFile<<"  If (${FileListWM} == '')"<<std::endl;
	BMSAutoSegMainFile<<"   set (flagWM 1)"<<std::endl;
	BMSAutoSegMainFile<<"   WriteFile(${VolumeFileWM} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileWM} '################################################\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileWM} '# Parcellation map volume analysis: White Matter\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileWM} '################################################\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   set (files_to_cat_WM '')"<<std::endl;
	BMSAutoSegMainFile<<"  EndIf (${FileListWM})"<<std::endl;
	
	BMSAutoSegMainFile<<"  If (${FileListGM} == '')"<<std::endl;
	BMSAutoSegMainFile<<"   set (flagGM 1)"<<std::endl;
	BMSAutoSegMainFile<<"   WriteFile(${VolumeFileGM} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileGM} '################################################\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileGM} '# Parcellation map volume analysis: Grey Matter\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileGM} '################################################\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   set (files_to_cat_GM '')"<<std::endl;
	BMSAutoSegMainFile<<"  EndIf (${FileListGM})"<<std::endl;
	
	BMSAutoSegMainFile<<"  If (${FileListCSF} == '')"<<std::endl;
	BMSAutoSegMainFile<<"   set (flagCSF 1)"<<std::endl;
	BMSAutoSegMainFile<<"   WriteFile(${VolumeFileCSF} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileCSF} '################################################\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileCSF} '# Parcellation map volume analysis: CSF\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   AppendFile(${VolumeFileCSF} '################################################\\n\\n')"<<std::endl;
	BMSAutoSegMainFile<<"   set (files_to_cat_CSF '')"<<std::endl;
	BMSAutoSegMainFile<<"  EndIf (${FileListCSF})"<<std::endl;
      } 
      if (m_NbTissueClass > 4) 	{
	for (int classID = 1; classID < m_NbTissueClass; classID++) {
	  char classIDstring[100];
	  sprintf(classIDstring,"label_%d",classID);
	  BMSAutoSegMainFile<<"  If (${FileList" << classIDstring <<"} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"   set (flag" << classIDstring <<" 1)"<<std::endl;
	  BMSAutoSegMainFile<<"   WriteFile(${VolumeFile" << classIDstring <<"} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
	  BMSAutoSegMainFile<<"   AppendFile(${VolumeFile" << classIDstring <<"} '################################################\\n')"<<std::endl;
	  BMSAutoSegMainFile<<"   AppendFile(${VolumeFile" << classIDstring <<"} '# Parcellation map volume analysis: " << classIDstring << "\\n')"<<std::endl;
	  BMSAutoSegMainFile<<"   AppendFile(${VolumeFile" << classIDstring <<"} '################################################\\n\\n')"<<std::endl;
	  BMSAutoSegMainFile<<"   set (files_to_cat_" << classIDstring <<" '')"<<std::endl;
	  BMSAutoSegMainFile<<"  EndIf (${FileList" << classIDstring <<"})"<<std::endl;
	}
      }

      // loop over all cases to compute and gather results
      BMSAutoSegMainFile<<"   set (CaseNumber 0)"<<std::endl;
      BMSAutoSegMainFile<<"   ForEach (T1Case ${T1CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1Dir ${T1Path} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
	
      if(GetLoop() && std::strcmp(GetEMSoftware(), "neoseg") != 0)
	BMSAutoSegMainFile<<"         set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
      else
	BMSAutoSegMainFile<<"         set (EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
      if (IsT1LabelEMSFile)
	BMSAutoSegMainFile<<"      set (SegmentedFile ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
      else
      {
	BMSAutoSegMainFile<<"      GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	BMSAutoSegMainFile<<"      GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegMainFile<<"      set (SegmentedFile ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
      }

      BMSAutoSegMainFile<<"      GetFilename (SegmentedFileTail ${SegmentedFile} NAME_WITHOUT_EXTENSION)"<<std::endl; 
      if(GetLoop() && std::strcmp(GetEMSoftware(), "neoseg") != 0)
	BMSAutoSegMainFile<<"      ExtractString(SegmentedFileHead ${SegmentedFileTail} 13 FROMEND)"<<std::endl;
      else
	BMSAutoSegMainFile<<"      ExtractString(SegmentedFileHead ${SegmentedFileTail} 11 FROMEND)"<<std::endl;

      // set label list
      if (std::strcmp(GetEMSoftware(), "neoseg") == 0) {
	BMSAutoSegMainFile<<"      set(LabelList 'MWM' 'WM' 'GM' 'CSF')"<<std::endl;
      } else {
	if (m_NbTissueClass == 4 ) {
	  BMSAutoSegMainFile<<"      set(LabelList 'WM' 'GM' 'CSF')"<<std::endl;
	} else {
	  std::string tissueClassString = std::string("      set (LabelList ");
	  for (int classID = 1; classID < m_NbTissueClass; classID++) {
	    char classIDstring[100];
	    sprintf(classIDstring,"'label_%d'",classID);
	    
	    tissueClassString = tissueClassString + std::string(" ") + std::string(classIDstring);
	  }
	  tissueClassString = tissueClassString + std::string(")");
	  BMSAutoSegMainFile<< tissueClassString <<std::endl;
	}
      }

      BMSAutoSegMainFile<<"      set (SkullStrippedImage ${T1Path}/${AutoSegDir}/Stripped/${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;

      BMSAutoSegMainFile<<"      set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
      BMSAutoSegMainFile<<"      set(TissueNumber 1)"<<std::endl;

      // iterate over all labels
      BMSAutoSegMainFile<<"      ForEach(Label ${LabelList})"<<std::endl;
      BMSAutoSegMainFile<<"            set (LabelFileTail ${SegmentedFileHead}_${Label}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"            set (LabelFile ${EMSPath}${LabelFileTail})"<<std::endl;
      //BMSAutoSegMainFile<<"            echo (${LabelFile})"<<std::endl;
      BMSAutoSegMainFile<<"            set (File ${WarpROIPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--${ParcellationMapName}-WarpReg.nrrd )"<<std::endl;
      BMSAutoSegMainFile<<"            GetFilename (FileName ${File} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"            set(FileMaskedRoot ${FileName}_${Label})"<<std::endl;
      BMSAutoSegMainFile<<"            set(FileMaskedTail ${FileMaskedRoot}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"            set(FileMasked ${WarpROIPath}${FileMaskedTail})"<<std::endl;
      std::string SoftTissueMap =GetSoftTissueMap();
      if (SoftTissueMap=="Soft") {
	if (std::strcmp(GetEMSoftware(), "neoseg") == 0) {
	    BMSAutoSegMainFile<<"           If (${Label} == 'MWM' && ${flagMWM} == 1)"<<std::endl;
	    BMSAutoSegMainFile<<"              set(Posterior ${NEOSEG_PREFIX}_posterior0)"<<std::endl;
	    BMSAutoSegMainFile<<"           EndIf (${Label})"<<std::endl;
	    BMSAutoSegMainFile<<"           If (${Label} == 'WM' && ${flagWM} == 1)"<<std::endl;
	    BMSAutoSegMainFile<<"              set(Posterior ${NEOSEG_PREFIX}_posterior1)"<<std::endl;
	    BMSAutoSegMainFile<<"           EndIf (${Label})"<<std::endl;
	    BMSAutoSegMainFile<<"           If (${Label} == 'GM' && ${flagGM} == 1)"<<std::endl;
	    BMSAutoSegMainFile<<"              set(Posterior ${NEOSEG_PREFIX}_posterior2)"<<std::endl;
	    BMSAutoSegMainFile<<"           EndIf (${Label})"<<std::endl;
	    BMSAutoSegMainFile<<"           If (${Label} == 'CSF' && ${flagCSF} == 1)"<<std::endl;
	    BMSAutoSegMainFile<<"              set(Posterior ${NEOSEG_PREFIX}_posterior3)"<<std::endl;
	    BMSAutoSegMainFile<<"           EndIf (${Label})"<<std::endl;
	} else {
	    if (m_NbTissueClass == 4 ) {
	      BMSAutoSegMainFile<<"           If (${Label} == 'WM' && ${flagWM} == 1)"<<std::endl;
	      BMSAutoSegMainFile<<"              set(Posterior _posterior0)"<<std::endl;
	      BMSAutoSegMainFile<<"           EndIf (${Label})"<<std::endl;
	      BMSAutoSegMainFile<<"           If (${Label} == 'GM' && ${flagGM} == 1)"<<std::endl;
	      BMSAutoSegMainFile<<"              set(Posterior _posterior1)"<<std::endl;
	      BMSAutoSegMainFile<<"           EndIf (${Label})"<<std::endl;
	      BMSAutoSegMainFile<<"           If (${Label} == 'CSF' && ${flagCSF} == 1)"<<std::endl;
	      BMSAutoSegMainFile<<"              set(Posterior _posterior2)"<<std::endl;
	      BMSAutoSegMainFile<<"           EndIf (${Label})"<<std::endl;
	    } else {
	      BMSAutoSegMainFile<<"              set(Posterior _posterior${Label})"<<std::endl;
	    }
	}
	if(GetLoop() && std::strcmp(GetEMSoftware(), "neoseg") != 0) {
	  BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
	  BMSAutoSegMainFile<<"           set(SuffixPosterior _stripEMS${StrippedBias}${Posterior}_${SUFFIX})"<<std::endl;
	} else {
	  BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
	  BMSAutoSegMainFile<<"           set(SuffixPosterior ${Posterior}_${SUFFIX})"<<std::endl;
	}
	if (IsT1LabelEMSFile) {
	  BMSAutoSegMainFile<<"             set (ProbMap ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
	} else {
	  BMSAutoSegMainFile<<"             GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	  BMSAutoSegMainFile<<"             GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	  BMSAutoSegMainFile<<"             set (ProbMap ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
	}	
	BMSAutoSegMainFile<<"             ListFileInDir(List ${WarpROIPath} ${FileMaskedRoot}_volumeSummary.csv)"<<std::endl;
	BMSAutoSegMainFile<<"             If (${List} == '')"<<std::endl;
	BMSAutoSegMainFile<<"               set (command_line ${ImageStatCmd} ${SkullStrippedImage} -label ${File} -probabilityMap ${ProbMap} -volumeSummary  -outbase ${WarpROIPath}${FileMaskedRoot})"<<std::endl;
	BMSAutoSegMainFile<<"               Run (output ${command_line})"<<std::endl; 
	//BMSAutoSegMainFile<<"            Else ()"<<std::endl;
	//BMSAutoSegMainFile<<"              echo('File already exists: '${WarpROIPath}${FileMaskedRoot}_volumeSummary.csv)"<<std::endl; 
	BMSAutoSegMainFile<<"             Endif(${List})"<<std::endl;		
      } else {
	BMSAutoSegMainFile<<"            ListFileInDir(List ${EMSPath} ${LabelFileTail})"<<std::endl;
	BMSAutoSegMainFile<<"            If (${List} == '')"<<std::endl;
	BMSAutoSegMainFile<<"              set (command_line ${ImageMathCmd} ${SegmentedFile} -extractLabel ${TissueNumber} -outfile ${LabelFile})"<<std::endl;
	BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl; 
	//BMSAutoSegMainFile<<"            Else ()"<<std::endl;
	//BMSAutoSegMainFile<<"              echo('File already exists: '${EMSPath}${LabelFileTail})"<<std::endl;
	BMSAutoSegMainFile<<"            Endif(${List})"<<std::endl;	

	BMSAutoSegMainFile<<"            ListFileInDir(List ${WarpROIPath} ${FileMaskedTail})"<<std::endl;
	BMSAutoSegMainFile<<"            If (${List} == '')"<<std::endl;
	BMSAutoSegMainFile<<"              set (command_line ${ImageMathCmd} ${File} -mask ${LabelFile} -outfile ${FileMasked})"<<std::endl;
	BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl; 
	//BMSAutoSegMainFile<<"            Else ()"<<std::endl;
	//BMSAutoSegMainFile<<"              echo('File already exists: '${FileMaskedTail})"<<std::endl;
	BMSAutoSegMainFile<<"            Endif(${List})"<<std::endl;

	BMSAutoSegMainFile<<"            ListFileInDir(List ${WarpROIPath} ${FileMaskedRoot}_volumeSummary.csv)"<<std::endl;
	BMSAutoSegMainFile<<"            If (${List} == '')"<<std::endl;
	BMSAutoSegMainFile<<"              set (command_line ${ImageStatCmd} ${SkullStrippedImage} -label ${FileMasked} -volumeSummary -outbase ${WarpROIPath}${FileMaskedRoot})"<<std::endl;
	BMSAutoSegMainFile<<"              Run (output ${command_line})"<<std::endl; 
	//BMSAutoSegMainFile<<"            Else ()"<<std::endl;
	//BMSAutoSegMainFile<<"              echo('File already exists: '${WarpROIPath}${FileMaskedRoot}_volumeSummary.csv)"<<std::endl;
	BMSAutoSegMainFile<<"            Endif(${List})"<<std::endl;
      }

      BMSAutoSegMainFile<<"             set (CSVFile ${WarpROIPath}${FileMaskedRoot}_volumeSummary.csv)"<<std::endl;
      if (m_NbTissueClass == 4 || (std::strcmp(GetEMSoftware(), "neoseg") == 0)) {
	if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
	  {
	    BMSAutoSegMainFile<<"              If (${Label} == 'MWM' && ${flagMWM} == 1)"<<std::endl;
	    BMSAutoSegMainFile<<"                  If (${files_to_cat_MWM} == '')"<<std::endl;
	    BMSAutoSegMainFile<<"                     set (files_to_cat_MWM ${CSVFile})"<<std::endl;
	    BMSAutoSegMainFile<<"                  Else ()"<<std::endl;
	    BMSAutoSegMainFile<<"                     set (files_to_cat_MWM ${files_to_cat_MWM} ${CSVFile})"<<std::endl;
	    BMSAutoSegMainFile<<"                  EndIf(${files_to_cat_MWM})"<<std::endl<<std::endl<<std::endl;
	    BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;	  
	  }
	BMSAutoSegMainFile<<"              If (${Label} == 'WM' && ${flagWM} == 1)"<<std::endl;
	BMSAutoSegMainFile<<"                  If (${files_to_cat_WM} == '')"<<std::endl;
	BMSAutoSegMainFile<<"                     set (files_to_cat_WM ${CSVFile})"<<std::endl;
	BMSAutoSegMainFile<<"                  Else ()"<<std::endl;
	BMSAutoSegMainFile<<"                     set (files_to_cat_WM ${files_to_cat_WM} ${CSVFile})"<<std::endl;
	BMSAutoSegMainFile<<"                  EndIf(${files_to_cat_WM})"<<std::endl<<std::endl<<std::endl;
	BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
	BMSAutoSegMainFile<<"              If (${Label} == 'GM' && ${flagGM} == 1)"<<std::endl;
	BMSAutoSegMainFile<<"                  If (${files_to_cat_GM} == '')"<<std::endl;
	BMSAutoSegMainFile<<"                     set (files_to_cat_GM ${CSVFile})"<<std::endl;
	BMSAutoSegMainFile<<"                  Else ()"<<std::endl;
	BMSAutoSegMainFile<<"                     set (files_to_cat_GM ${files_to_cat_GM} ${CSVFile})"<<std::endl;
	BMSAutoSegMainFile<<"                  EndIf(${files_to_cat_GM})"<<std::endl<<std::endl<<std::endl;
	BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
	BMSAutoSegMainFile<<"              If (${Label} == 'CSF' && ${flagCSF} == 1)"<<std::endl;
	BMSAutoSegMainFile<<"                  If (${files_to_cat_CSF} == '')"<<std::endl;
	BMSAutoSegMainFile<<"                     set (files_to_cat_CSF ${CSVFile})"<<std::endl;
	BMSAutoSegMainFile<<"                  Else ()"<<std::endl;
	BMSAutoSegMainFile<<"                     set (files_to_cat_CSF ${files_to_cat_CSF} ${CSVFile})"<<std::endl;
	BMSAutoSegMainFile<<"                  EndIf(${files_to_cat_CSF})"<<std::endl<<std::endl<<std::endl;
	BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
      } else {
	for (int classID = 1; classID < m_NbTissueClass; classID++) {
	  char classIDstring[100];
	  sprintf(classIDstring,"label_%d",classID);
	  BMSAutoSegMainFile<<"              If (${Label} == '"<< classIDstring <<"' && ${flag"<< classIDstring <<"} == 1)"<<std::endl;
	  BMSAutoSegMainFile<<"                  If (${files_to_cat_"<< classIDstring <<"} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"                     set (files_to_cat_"<< classIDstring <<" ${CSVFile})"<<std::endl;
	  BMSAutoSegMainFile<<"                  Else ()"<<std::endl;
	  BMSAutoSegMainFile<<"                     set (files_to_cat_"<< classIDstring <<" ${files_to_cat_"<< classIDstring <<"} ${CSVFile})"<<std::endl;
	  BMSAutoSegMainFile<<"                  EndIf(${files_to_cat_"<< classIDstring <<"})"<<std::endl<<std::endl<<std::endl;
	  BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
	}
      }

      //BMSAutoSegMainFile<<"           echo ( )  "<<std::endl;
      BMSAutoSegMainFile<<"           Inc(${TissueNumber} 1)"<<std::endl;
      BMSAutoSegMainFile<<"           Int(${TissueNumber})"<<std::endl;
      BMSAutoSegMainFile<<"         EndForEach(Label)"<<std::endl;
      BMSAutoSegMainFile<<"      Inc(${CaseNumber} 1)"<<std::endl;
      BMSAutoSegMainFile<<"      Int(${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"   EndForEach (T1Case)"<<std::endl;
      BMSAutoSegMainFile<<"   echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"   echo ('Gathering volume information for the whole dataset...')"<<std::endl;
      if (m_NbTissueClass == 4 || (std::strcmp(GetEMSoftware(), "neoseg") == 0)) {
	if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
	  {
	    BMSAutoSegMainFile<<"   If (${flagMWM} == 1)"<<std::endl;
	    BMSAutoSegMainFile<<"     If (${files_to_cat_MWM} != '')"<<std::endl;
	    BMSAutoSegMainFile<<"       set (command_line cat ${files_to_cat_MWM})"<<std::endl;
	    BMSAutoSegMainFile<<"       Run (output ${command_line})"<<std::endl; 	
	    BMSAutoSegMainFile<<"       AppendFile(${VolumeFileMWM} ${output})"<<std::endl;
	    BMSAutoSegMainFile<<"     EndIf (${files_to_cat_MWM})"<<std::endl;
	    BMSAutoSegMainFile<<"   EndIf (${flagMWM})"<<std::endl;
	  }
	BMSAutoSegMainFile<<"   If (${flagWM} == 1)"<<std::endl;
	BMSAutoSegMainFile<<"     If (${files_to_cat_WM} != '')"<<std::endl;
	BMSAutoSegMainFile<<"       set (command_line cat ${files_to_cat_WM})"<<std::endl;
	BMSAutoSegMainFile<<"       Run (output ${command_line})"<<std::endl; 	
	BMSAutoSegMainFile<<"       AppendFile(${VolumeFileWM} ${output})"<<std::endl;
	BMSAutoSegMainFile<<"     EndIf (${files_to_cat_WM})"<<std::endl;
	BMSAutoSegMainFile<<"   EndIf (${flagWM})"<<std::endl;
	BMSAutoSegMainFile<<"   If (${flagGM} == 1)"<<std::endl;
	BMSAutoSegMainFile<<"     If (${files_to_cat_GM} != '')"<<std::endl;
	BMSAutoSegMainFile<<"       set (command_line cat ${files_to_cat_GM})"<<std::endl;
	BMSAutoSegMainFile<<"       Run (output ${command_line})"<<std::endl; 		
	BMSAutoSegMainFile<<"       AppendFile(${VolumeFileGM} ${output})"<<std::endl;
	BMSAutoSegMainFile<<"     EndIf (${files_to_cat_GM})"<<std::endl;
	BMSAutoSegMainFile<<"   EndIf (${flagGM})"<<std::endl;
	BMSAutoSegMainFile<<"   If (${flagCSF} == 1)"<<std::endl;
	BMSAutoSegMainFile<<"     If (${files_to_cat_CSF} != '')"<<std::endl;
	BMSAutoSegMainFile<<"       set (command_line cat ${files_to_cat_CSF})"<<std::endl;
	BMSAutoSegMainFile<<"       Run (output ${command_line})"<<std::endl; 		
	BMSAutoSegMainFile<<"       AppendFile(${VolumeFileCSF} ${output})"<<std::endl;
	BMSAutoSegMainFile<<"     EndIf (${files_to_cat_CSF})"<<std::endl; 
	BMSAutoSegMainFile<<"   EndIf (${flagCSF})"<<std::endl;
      } else {
	for (int classID = 1; classID < m_NbTissueClass; classID++) {
	  char classIDstring[100];
	  sprintf(classIDstring,"label_%d",classID);
	  BMSAutoSegMainFile<<"   If (${flag"<< classIDstring <<"} == 1)"<<std::endl;
	  BMSAutoSegMainFile<<"     If (${files_to_cat_"<< classIDstring <<"} != '')"<<std::endl;
	  BMSAutoSegMainFile<<"       set (command_line cat ${files_to_cat_"<< classIDstring <<"})"<<std::endl;
	  BMSAutoSegMainFile<<"       Run (output ${command_line})"<<std::endl; 		
	  BMSAutoSegMainFile<<"       AppendFile(${VolumeFile"<< classIDstring <<"} ${output})"<<std::endl;
	  BMSAutoSegMainFile<<"     EndIf (${files_to_cat_"<< classIDstring <<"})"<<std::endl; 
	  BMSAutoSegMainFile<<"   EndIf (${flag"<< classIDstring <<"})"<<std::endl;
	}
	
      }

      BMSAutoSegMainFile<<"  Else ()"<<std::endl; // else (${ParcellationMapComputed})
      BMSAutoSegMainFile<<"   echo ('Volume file already exists!')"<<std::endl;
      BMSAutoSegMainFile<<"  EndIf(${ParcellationMapComputed})"<<std::endl;
	
      BMSAutoSegMainFile<<"EndForEach(ParcellationMap)"<<std::endl;
      
    }
  
    //multi-atlas
    
    if (GetMultiAtlasSegmentation()) {
      BMSAutoSegMainFile<<"set (files_to_cat '')"<<std::endl;
      BMSAutoSegMainFile<<"set (CaseNumber 0)"<<std::endl;
      BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1Dir ${T1Path} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Case Number - multi atlas: '${T1CaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      set (SkullStrippedImage ${T1Path}/${AutoSegDir}/Stripped/${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      if(GetLoop() && std::strcmp(GetEMSoftware(), "neoseg") != 0)
	BMSAutoSegMainFile<<"         set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
      else
	BMSAutoSegMainFile<<"         set (EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
      if (IsT1LabelEMSFile)
	BMSAutoSegMainFile<<"      set (SegmentedFile ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
      else
      {
	BMSAutoSegMainFile<<"      GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	BMSAutoSegMainFile<<"      GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegMainFile<<"      set (SegmentedFile ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
      }

      BMSAutoSegMainFile<<"      GetFilename (SegmentedFileTail ${SegmentedFile} NAME_WITHOUT_EXTENSION)"<<std::endl; 
      if(GetLoop() && std::strcmp(GetEMSoftware(), "neoseg") != 0)
	BMSAutoSegMainFile<<"      ExtractString(SegmentedFileHead ${SegmentedFileTail} 13 FROMEND)"<<std::endl;
      else
	BMSAutoSegMainFile<<"      ExtractString(SegmentedFileHead ${SegmentedFileTail} 11 FROMEND)"<<std::endl;
      
      BMSAutoSegMainFile<<"         set (FusedLabelDir ${T1Path}/${AutoSegDir}/fused-labels/)"<<std::endl;
      BMSAutoSegMainFile<<"         set (MultiAtlasTarget ${FusedLabelDir}${T1CaseHead}_label.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"         set (MultiAtlasTargetHead ${T1CaseHead}_label)"<<std::endl;
      BMSAutoSegMainFile<<"         ListFileInDir(List ${FusedLabelDir} ${MultiAtlasTargetHead}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"         If (${List} == '')"<<std::endl;
      BMSAutoSegMainFile<<"           set (command_line ${ImageStatCmd} ${SkullStrippedImage}  -label ${MultiAtlasTarget} -volumeSummary -outbase ${FusedLabelDir}${MultiAtlasTargetHead})"<<std::endl;
      BMSAutoSegMainFile<<"           Run (output ${command_line})"<<std::endl; 
      //BMSAutoSegMainFile<<"         Else ()"<<std::endl;
      //BMSAutoSegMainFile<<"           echo('File already exists: ' ${FusedLabelDir}${MultiAtlasTargetHead}_volumeSummary.csv)"<<std::endl; 
      BMSAutoSegMainFile<<"         EndIf (${List})"<<std::endl;

      // adding cat file stuff
      BMSAutoSegMainFile<<"       set (CSVFile ${FusedLabelDir}${MultiAtlasTargetHead}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"       If (${files_to_cat} == '')"<<std::endl;
      BMSAutoSegMainFile<<"         set (files_to_cat ${CSVFile})"<<std::endl;
      BMSAutoSegMainFile<<"       Else (${files_to_cat})"<<std::endl;
      BMSAutoSegMainFile<<"         set (files_to_cat ${files_to_cat} ${CSVFile})"<<std::endl;
      BMSAutoSegMainFile<<"       EndIf(${files_to_cat})"<<std::endl<<std::endl<<std::endl;
      
      if (GetNbAtlasParcellation() > 0 ) {
	BMSAutoSegMainFile<<"         set (MultiAtlasTargetHead ${T1CaseHead}_parcellation)"<<std::endl;

	if (m_NbTissueClass == 4 || (std::strcmp(GetEMSoftware(), "neoseg") == 0)) {
	  BMSAutoSegMainFile<<"         ListFileInDir(List ${FusedLabelDir} ${MultiAtlasTargetHead}_GM.nrrd)"<<std::endl;
	  BMSAutoSegMainFile<<"         If (${List} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"           set (command_line ${ImageMathCmd} ${FusedLabelDir}${MultiAtlasTargetHead}.nrrd -mask ${EMSPath}${SegmentedFileHead}_GM.nrrd  -outfile ${FusedLabelDir}${MultiAtlasTargetHead}_GM.nrrd)"<<std::endl; 
	  BMSAutoSegMainFile<<"           Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"         Else ()"<<std::endl;
	  BMSAutoSegMainFile<<"           echo('File already exists: ' ${FusedLabelDir}${MultiAtlasTargetHead}_GM.nrrd )"<<std::endl; 
	  BMSAutoSegMainFile<<"         EndIf (${List})"<<std::endl;
	  BMSAutoSegMainFile<<"         ListFileInDir(List ${FusedLabelDir} ${MultiAtlasTargetHead}_WM.nrrd)"<<std::endl;
	  BMSAutoSegMainFile<<"         If (${List} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"           set (command_line ${ImageMathCmd} ${FusedLabelDir}${MultiAtlasTargetHead}.nrrd -mask ${EMSPath}${SegmentedFileHead}_WM -outfile ${FusedLabelDir}${MultiAtlasTargetHead}_WM.nrrd)"<<std::endl; 
	  BMSAutoSegMainFile<<"           Run (output ${command_line})"<<std::endl; 
	  BMSAutoSegMainFile<<"         Else ()"<<std::endl;
	  BMSAutoSegMainFile<<"           echo('File already exists: ' ${FusedLabelDir}${MultiAtlasTargetHead}_WM.nrrd )"<<std::endl; 
	  BMSAutoSegMainFile<<"         EndIf (${List})"<<std::endl;
	  BMSAutoSegMainFile<<"         ListFileInDir(List ${FusedLabelDir} ${MultiAtlasTargetHead}_CSF.nrrd)"<<std::endl;
	  BMSAutoSegMainFile<<"         If (${List} == '')"<<std::endl;
	  BMSAutoSegMainFile<<"           set (command_line ${ImageMathCmd} ${FusedLabelDir}${MultiAtlasTargetHead}.nrrd -mask ${EMSPath}${SegmentedFileHead}_CSG -outfile ${FusedLabelDir}${MultiAtlasTargetHead}_CSF.nrrd)"<<std::endl; 
	  BMSAutoSegMainFile<<"           Run (output ${command_line})"<<std::endl;
	  BMSAutoSegMainFile<<"         Else ()"<<std::endl;
	  BMSAutoSegMainFile<<"           echo('File already exists: ' ${FusedLabelDir}${MultiAtlasTargetHead}_CSF.nrrd )"<<std::endl; 
	  BMSAutoSegMainFile<<"         EndIf (${List})"<<std::endl; 
	  if (std::strcmp(GetEMSoftware(), "neoseg") == 0)  {
	    BMSAutoSegMainFile<<"         ListFileInDir(List ${FusedLabelDir} ${MultiAtlasTargetHead}_MWM.nrrd)"<<std::endl;
	    BMSAutoSegMainFile<<"         If (${List} == '')"<<std::endl;
	    BMSAutoSegMainFile<<"           set (command_line ${ImageMathCmd} ${FusedLabelDir}${MultiAtlasTargetHead}.nrrd -mask ${EMSPath}${SegmentedFileHead}_MWM -outfile ${FusedLabelDir}${MultiAtlasTargetHead}_MWM.nrrd)"<<std::endl; 
	    BMSAutoSegMainFile<<"           Run (output ${command_line})"<<std::endl; 
	    BMSAutoSegMainFile<<"         Else ()"<<std::endl;
	    BMSAutoSegMainFile<<"           echo('File already exists: ' ${FusedLabelDir}${MultiAtlasTargetHead}_MWM.nrrd )"<<std::endl; 
	    BMSAutoSegMainFile<<"         EndIf (${List})"<<std::endl;
	  }
	} else {
	  for (int classID = 1; classID < m_NbTissueClass; classID++) {
	    char parcellationSuffix[200];
	    sprintf(parcellationSuffix,"_label_%d.nrrd",classID);
	    
	    
	    //BMSAutoSegMainFile<<"         echo('ext: '${FusedLabelDir}${MultiAtlasTargetHead}.nrrd' -mask '${EMSPath}${SegmentedFileHead}" << parcellationSuffix << ")" << std::endl;
	    BMSAutoSegMainFile<<"         set (MultiAtlasParcFile ${MultiAtlasTargetHead}"<< parcellationSuffix << ")" << std::endl;
	    BMSAutoSegMainFile<<"         GetFilename (MultiAtlasParcHead ${MultiAtlasParcFile} NAME_WITHOUT_EXTENSION)" << std::endl;

	    BMSAutoSegMainFile<<"         ListFileInDir(List ${FusedLabelDir} ${MultiAtlasParcFile})" << std::endl;
	    BMSAutoSegMainFile<<"         If (${List} == '')"<<std::endl;
	    BMSAutoSegMainFile<<"           set (command_line ${ImageMathCmd} ${FusedLabelDir}${MultiAtlasTargetHead}.nrrd -mask ${EMSPath}${SegmentedFileHead}" << parcellationSuffix << " -outfile ${FusedLabelDir}${MultiAtlasParcFile})"<<std::endl; 
	    BMSAutoSegMainFile<<"           Run (output ${command_line})"<<std::endl;
	    BMSAutoSegMainFile<<"         Else ()"<<std::endl;
	    BMSAutoSegMainFile<<"           echo('File already exists: ' ${FusedLabelDir}${MultiAtlasParcFile})"<<std::endl; 
	    BMSAutoSegMainFile<<"         Endif(${List})"<<std::endl;


	    BMSAutoSegMainFile<<"         ListFileInDir(List ${FusedLabelDir} ${MultiAtlasParcHead}_volumeSummary.csv)"<<std::endl;
	    BMSAutoSegMainFile<<"         If (${List} == '')"<<std::endl;
	    BMSAutoSegMainFile<<"             set (command_line ${ImageStatCmd} ${SkullStrippedImage} -label ${FusedLabelDir}${MultiAtlasParcFile} -volumeSummary -outbase ${FusedLabelDir}${MultiAtlasParcHead})"<<std::endl;
	    BMSAutoSegMainFile<<"             Run (output ${command_line})"<<std::endl; 
	    //BMSAutoSegMainFile<<"         Else ()"<<std::endl;
	    //BMSAutoSegMainFile<<"            echo('File already exists: '${FusedLabelDir}${MultiAtlasParcHead}_volumeSummary.csv)"<<std::endl;
	    BMSAutoSegMainFile<<"         Endif(${List})"<<std::endl;
	  }
	}
      }
      BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;

      BMSAutoSegMainFile<<"set (VolumeFile ${VolumeDir}AutoSeg_MultiAtlasSegmentationVolume.csv)"<<std::endl;  
      BMSAutoSegMainFile<<"GetFileName(Path ${VolumeFile} PATH)"<<std::endl; 
      BMSAutoSegMainFile<<"GetFileName(VolumeFileTail ${VolumeFile} NAME)"<<std::endl;
      
      BMSAutoSegMainFile<<"WriteFile(${VolumeFile} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
      BMSAutoSegMainFile<<"AppendFile(${VolumeFile} '##################################\\n')"<<std::endl;
      BMSAutoSegMainFile<<"AppendFile(${VolumeFile} '# Multiatlas segmentation volume analysis:\\n')"<<std::endl;
      BMSAutoSegMainFile<<"AppendFile(${VolumeFile} '##################################\\n\\n')"<<std::endl;
      BMSAutoSegMainFile<<"echo ('Gathering multiatlas volume information for the whole dataset...')"<<std::endl;
      BMSAutoSegMainFile<<"If (${files_to_cat} != '')"<<std::endl;
      BMSAutoSegMainFile<<"      set (command_line cat ${files_to_cat})"<<std::endl;
      BMSAutoSegMainFile<<"      Run (output ${command_line})"<<std::endl; 
      BMSAutoSegMainFile<<"      AppendFile(${VolumeFile} ${output})"<<std::endl;
      BMSAutoSegMainFile<<"EndIf(${files_to_cat})"<<std::endl<<std::endl<<std::endl;
    }
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('VOLUME COMPUTATION: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }
  // TODO: add cat-ing of multi-atlas files when more than 3 clases

  BMSAutoSegMainFile<<"   Glob (UnzippedFileList ${T1Path}/${AutoSegDir}/*warp.hfield RECURSIVE)"<<std::endl;
  BMSAutoSegMainFile<<"   If (${UnzippedFileList} != '')"<<std::endl;
  BMSAutoSegMainFile<<"	   # -------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"	   # -------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"	   #Sanity check for zipped images"<<std::endl;
  BMSAutoSegMainFile<<"      # -------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"      # -------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"	   echo (*************************************************)"<<std::endl;
  BMSAutoSegMainFile<<"	   echo ('ZIP SANITY CHECK...')"<<std::endl;
  BMSAutoSegMainFile<<"	   echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"      ForEach(UnzippedFile ${UnzippedFileList})"<<std::endl;
  BMSAutoSegMainFile<<"         set (ZippedFile ${UnzippedFile}.gz)"<<std::endl;
  BMSAutoSegMainFile<<"         GetFilename(Path ${UnzippedFile} PATH)"<<std::endl;
  BMSAutoSegMainFile<<"         ListFileInDir(ZippedFileList ${Path} ${ZippedFile})"<<std::endl;
  BMSAutoSegMainFile<<"         If (${ZippedFileList} != '')"<<std::endl;
  BMSAutoSegMainFile<<"            DeleteFile(${UnzippedFile})"<<std::endl;
  BMSAutoSegMainFile<<"         Else ()"<<std::endl;
  BMSAutoSegMainFile<<"            set (command_line gzip -f ${UnzippedFile})"<<std::endl;
  BMSAutoSegMainFile<<"            Run (output ${command_line})"<<std::endl; 	
  BMSAutoSegMainFile<<"         EndIf (${ZippedFileList})"<<std::endl;
  BMSAutoSegMainFile<<"      EndForEach (UnzippedFile)"<<std::endl;
  BMSAutoSegMainFile<<"      echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"	   echo ('ZIP SANITY CHECK: DONE!')"<<std::endl;
  BMSAutoSegMainFile<<"	   echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"   EndIf (${UnzippedFileList}) "<<std::endl;
  BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"echo ('End of Automatic Segmentation!')"<<std::endl;

  BMSAutoSegMainFile.close();
}

void AutoSegComputation::WriteBMSAutoSegAuxFile()
{
  int DataNumber;
  std::ofstream BMSAutoSegAuxFile(GetBMSAutoSegAuxFile());

  BMSAutoSegAuxFile<<"#---------------------   Regional histogram   ------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
	
  BMSAutoSegAuxFile<<"\nset (NbAuxData "<<GetNbAuxData()<<")"<<std::endl;
  BMSAutoSegAuxFile<<"\n# Data AutoSeg Directory"<<std::endl;
  BMSAutoSegAuxFile<<"set (AutoSegDir "<<GetDataAutoSegDirectory()<<")"<<std::endl; 
  BMSAutoSegAuxFile<<"set (DataDir "<<GetDataDirectory()<<")"<<std::endl; 
  BMSAutoSegAuxFile<<"# Processing Directory"<<std::endl;
  BMSAutoSegAuxFile<<"set (ProcessingDir "<<GetProcessDataDirectory()<<")"<<std::endl;
  BMSAutoSegAuxFile<<"# Programs "<<std::endl;
  BMSAutoSegAuxFile<<"set (ImageMathCmd ImageMath)"<<std::endl;
  BMSAutoSegAuxFile<<"set (convCmd convertITKformats)"<<std::endl;
  BMSAutoSegAuxFile<<"set (ImageStatCmd ImageStat)"<<std::endl;
  std::string pathResampleVolume = itksys::SystemTools::FindProgram("ResampleScalarVectorDWIVolume");
  if( !pathResampleVolume.empty() )
  {
    BMSAutoSegAuxFile<<"set (ResampleVolume2Cmd "<< pathResampleVolume << ")"<<std::endl<<std::endl;
  }
  else
  {
    pathResampleVolume = itksys::SystemTools::FindProgram("ResampleVolume2");
    BMSAutoSegAuxFile<<"set (ResampleVolume2Cmd "<< pathResampleVolume << ")"<<std::endl<<std::endl;
  }
  BMSAutoSegAuxFile<<"set (BRAINSFitCmd BRAINSFit)"<<std::endl<<std::endl;

  if (GetAuxT1Image())
  {
    if(!m_Manually)
    {
      BMSAutoSegAuxFile<<"set (OrigSourceCasesList "<<m_T1List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegAuxFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_T1List[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegAuxFile<<"set (OrigSourceCasesList "<<m_AuxT1List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegAuxFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxT1List[DataNumber]<<")"<<std::endl;
    }
  }
  else if (GetAuxT2Image())
  {
    if(!m_Manually)
    {
      BMSAutoSegAuxFile<<"set (OrigSourceCasesList "<<m_T2List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegAuxFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_T2List[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegAuxFile<<"set (OrigSourceCasesList "<<m_AuxT2List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegAuxFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxT2List[DataNumber]<<")"<<std::endl;
    }
  }
  else
  {
    if(!m_Manually)
    {
      BMSAutoSegAuxFile<<"set (OrigSourceCasesList "<<m_PDList[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegAuxFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_PDList[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegAuxFile<<"set (OrigSourceCasesList "<<m_AuxPDList[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegAuxFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxPDList[DataNumber]<<")"<<std::endl;
    }
  }
	
  if ( (std::strcmp(GetCommonCoordinateImageType(),"T2") == 0) && (GetT2Image()))
  {
    if (GetAuxT1Image())
      BMSAutoSegAuxFile<<"set(T1RegistrationExtension _regT2_regAtlas)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegAuxFile<<"set(T2RegistrationExtension _regAtlas)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegAuxFile<<"set(PDRegistrationExtension _regT2_regAtlas)"<<std::endl;
  }
  else
  {
    if (GetAuxT1Image())
      BMSAutoSegAuxFile<<"set(T1RegistrationExtension _regAtlas)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegAuxFile<<"set(T2RegistrationExtension _regT1_regAtlas)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegAuxFile<<"set(PDRegistrationExtension _regT1_regAtlas)"<<std::endl;
  }
	
  if(GetN4ITKBiasFieldCorrection())
    BMSAutoSegAuxFile<<"set (Bias _Bias)"<<std::endl;
  else
    BMSAutoSegAuxFile<<"set (Bias '')"<<std::endl;

  if (std::strcmp(GetEMSoftware(), "ABC") == 0)
    BMSAutoSegAuxFile<<"set (NEOSEG_PREFIX '')"<<std::endl;
  else
    BMSAutoSegAuxFile<<"set (NEOSEG_PREFIX '_EMonly')"<<std::endl;

  if (GetStrippedN4ITKBiasFieldCorrection())
    BMSAutoSegAuxFile<<"set (StrippedBias _Bias)"<<std::endl;
  else
    BMSAutoSegAuxFile<<"set (StrippedBias '')"<<std::endl;

  if (GetLoop())
  {
    int SuffixIteration=GetLoopIteration()+1;
    BMSAutoSegAuxFile<<"set (SUFFIX EMS_"<<SuffixIteration<<")"<<std::endl;
    BMSAutoSegAuxFile<<"set (SuffixCorrected _stripEMS${StrippedBias}_corrected_${SUFFIX})"<<std::endl;
  }
  else
  {
    BMSAutoSegAuxFile<<"set (SUFFIX EMS)"<<std::endl;
    BMSAutoSegAuxFile<<"set (SuffixCorrected _corrected_${SUFFIX})"<<std::endl;
  }

  if(GetAtlasSpaceImage())
  {
    BMSAutoSegAuxFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegAuxFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegAuxFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegAuxFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegAuxFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"   set (AtlasSpaceImagePath ${Path}/${AutoSegDir}/atlasIso)"<<std::endl;
    BMSAutoSegAuxFile<<"   set (AtlasSpaceImage ${AtlasSpaceImagePath}/${AtlasSpaceImageHead})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AtlasSpaceCasesList ${AtlasSpaceImage})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AtlasSpaceCasesList ${AtlasSpaceCasesList} ${AtlasSpaceImage})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegAuxFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegAuxFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegAuxFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegAuxFile<<"set (SourceCasesList ${AtlasSpaceCasesList})"<<std::endl;
  }
  // corrected image from ABC
  else if(GetBiasCorrectedImage())
  {
    BMSAutoSegAuxFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegAuxFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegAuxFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegAuxFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegAuxFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetLoop())
    {
      int SuffixIteration=GetLoopIteration()+1;
      BMSAutoSegAuxFile<<"   set (BiasCorrectedImagePath ${Path}/${AutoSegDir}/ems_"<<SuffixIteration<<")"<<std::endl;
    }
    else
      BMSAutoSegAuxFile<<"   set (BiasCorrectedImagePath ${Path}/${AutoSegDir}/ems)"<<std::endl;		
    BMSAutoSegAuxFile<<"   set (BiasCorrectedImage ${BiasCorrectedImagePath}/${BiasCorrectedImageHead})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (BiasCorrectedCasesList ${BiasCorrectedImage})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      set (BiasCorrectedCasesList ${BiasCorrectedCasesList} ${BiasCorrectedImage})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegAuxFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegAuxFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegAuxFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegAuxFile<<"set (SourceCasesList ${BiasCorrectedCasesList})"<<std::endl;
  }
  else
  {
    BMSAutoSegAuxFile<<"set (stripEMS _corrected_EMS-stripped)"<<std::endl;
    BMSAutoSegAuxFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegAuxFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegAuxFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegAuxFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegAuxFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"   set (SkullStrippedImagePath ${Path}/${AutoSegDir}/Stripped)"<<std::endl;
    BMSAutoSegAuxFile<<"   set (SkullStrippedImage ${SkullStrippedImagePath}/${SkullStrippedImageHead})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (SkullStrippedCasesList ${SkullStrippedImage})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      set (SkullStrippedCasesList ${SkullStrippedCasesList} ${SkullStrippedImage})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegAuxFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegAuxFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegAuxFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegAuxFile<<"set (SourceCasesList ${SkullStrippedCasesList})"<<std::endl;
  }
  if (GetAux1Image())
  {
    BMSAutoSegAuxFile<<"\nset (OrigAux1CasesList "<<m_Aux1List[0]<<")"<<std::endl;
    BMSAutoSegAuxFile<<"\nset (OrigAuxCasesList "<<m_Aux1List[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
    {
      BMSAutoSegAuxFile<<"set (OrigAux1CasesList ${OrigAux1CasesList} "<<m_Aux1List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux1List[DataNumber]<<")"<<std::endl;
    }
  }
  if (GetAux2Image())
  {
    BMSAutoSegAuxFile<<"\nset (OrigAux2CasesList "<<m_Aux2List[0]<<")"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux2List[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
    {
      BMSAutoSegAuxFile<<"set (OrigAux2CasesList ${OrigAux2CasesList} "<<m_Aux2List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux2List[DataNumber]<<")"<<std::endl;
    }
  }
  if (GetAux3Image())
  {
    BMSAutoSegAuxFile<<"\nset (OrigAux3CasesList "<<m_Aux3List[0]<<")"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux3List[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
    {
      BMSAutoSegAuxFile<<"set (OrigAux3CasesList ${OrigAux3CasesList} "<<m_Aux3List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux3List[DataNumber]<<")"<<std::endl;
    }
  }
  if (GetAux4Image())
  {
    BMSAutoSegAuxFile<<"\nset (OrigAux4CasesList "<<m_Aux4List[0]<<")"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux4List[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
    {
      BMSAutoSegAuxFile<<"set (OrigAux4CasesList ${OrigAux4CasesList} "<<m_Aux4List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux4List[DataNumber]<<")"<<std::endl;
    }
  }
  if (GetAux5Image())
  {
    BMSAutoSegAuxFile<<"\nset (OrigAux5CasesList "<<m_Aux5List[0]<<")"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux5List[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
    {
      BMSAutoSegAuxFile<<"set (OrigAux5CasesList ${OrigAux5CasesList} "<<m_Aux5List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux5List[DataNumber]<<")"<<std::endl;
    }
  }
  if (GetAux6Image())
  {
    BMSAutoSegAuxFile<<"\nset (OrigAux6CasesList "<<m_Aux6List[0]<<")"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux6List[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
    {
      BMSAutoSegAuxFile<<"set (OrigAux6CasesList ${OrigAux6CasesList} "<<m_Aux6List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux6List[DataNumber]<<")"<<std::endl;
    }
  }
  if (GetAux7Image())
  {
    BMSAutoSegAuxFile<<"\nset (OrigAux7CasesList "<<m_Aux7List[0]<<")"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux7List[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
    {
      BMSAutoSegAuxFile<<"set (OrigAux7CasesList ${OrigAux7CasesList} "<<m_Aux7List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux7List[DataNumber]<<")"<<std::endl;
    }
  }
  if (GetAux8Image())
  {
    BMSAutoSegAuxFile<<"\nset (OrigAux8CasesList "<<m_Aux8List[0]<<")"<<std::endl;
    BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux8List[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbAuxData(); DataNumber++)
    {
      BMSAutoSegAuxFile<<"set (OrigAux8CasesList ${OrigAux8CasesList} "<<m_Aux8List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"set (OrigAuxCasesList ${OrigAuxCasesList} "<<m_Aux8List[DataNumber]<<")"<<std::endl;
    }
  }
  
  BMSAutoSegAuxFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegAuxFile<<"echo ('CHECKING FILES...')"<<std::endl;
  BMSAutoSegAuxFile<<"echo ( )"<<std::endl;  

  BMSAutoSegAuxFile<<"echo ('OrigSourceCasesList: '${OrigSourceCasesList})"<<std::endl;
  BMSAutoSegAuxFile<<"echo ('SourceCasesList: '${SourceCasesList})"<<std::endl;
  if (GetAux1Image())
    BMSAutoSegAuxFile<<"echo ('OrigAux1CasesList: '${OrigAux1CasesList})"<<std::endl;
  if (GetAux2Image())
    BMSAutoSegAuxFile<<"echo ('OrigAux2CasesList: '${OrigAux2CasesList})"<<std::endl;
  if (GetAux3Image())
    BMSAutoSegAuxFile<<"echo ('OrigAux3CasesList: '${OrigAux3CasesList})"<<std::endl;
  if (GetAux4Image())
    BMSAutoSegAuxFile<<"echo ('OrigAux4CasesList: '${OrigAux4CasesList})"<<std::endl;
  if (GetAux5Image())
    BMSAutoSegAuxFile<<"echo ('OrigAux5CasesList: '${OrigAux5CasesList})"<<std::endl;
  if (GetAux6Image())
    BMSAutoSegAuxFile<<"echo ('OrigAux6CasesList: '${OrigAux6CasesList})"<<std::endl;
  if (GetAux7Image())
    BMSAutoSegAuxFile<<"echo ('OrigAux7CasesList: '${OrigAux7CasesList})"<<std::endl;
  if (GetAux8Image())
    BMSAutoSegAuxFile<<"echo ('OrigAux8CasesList: '${OrigAux8CasesList})"<<std::endl;
  BMSAutoSegAuxFile<<"echo ( )"<<std::endl;
  BMSAutoSegAuxFile<<"echo ('CHECKING FILES: DONE!')"<<std::endl;
  BMSAutoSegAuxFile<<"echo ( )"<<std::endl<<std::endl;  
	
  BMSAutoSegAuxFile<<"# Pipeline "<<std::endl;
  BMSAutoSegAuxFile<<"# 1. Checking format"<<std::endl;
  BMSAutoSegAuxFile<<"# 2. Source to Aux1 registration"<<std::endl;
  BMSAutoSegAuxFile<<"# 3. Applying the transformation to the others auxiliary images"<<std::endl;
  BMSAutoSegAuxFile<<"# 4. Statistic files"<<std::endl;

  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# 1. Converting images if necessary"<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl<<std::endl;

  BMSAutoSegAuxFile<<"# Checking Image"<<std::endl;
  BMSAutoSegAuxFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegAuxFile<<"echo ('CHECKING IMAGE TYPE...')"<<std::endl;
  BMSAutoSegAuxFile<<"echo ( )"<<std::endl;
  BMSAutoSegAuxFile<<"GetParam(Case ${OrigAuxCasesList} 0)"<<std::endl;
  BMSAutoSegAuxFile<<"GetFilename (Extension ${Case} EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"set (Mode -1)"<<std::endl;
  BMSAutoSegAuxFile<<"If (${Extension} == '.gipl.gz')"<<std::endl;
  BMSAutoSegAuxFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ('Zipped Gipl Format')"<<std::endl;
  BMSAutoSegAuxFile<<"EndIf (${Extension})"<<std::endl;
  BMSAutoSegAuxFile<<"If (${Extension} == '.mhd')"<<std::endl;
  BMSAutoSegAuxFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ('Meta Format')"<<std::endl;
  BMSAutoSegAuxFile<<"EndIf (${Extension})"<<std::endl;
  BMSAutoSegAuxFile<<"If (${Extension} == '.hdr')"<<std::endl;
  BMSAutoSegAuxFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ('Analyse Format')"<<std::endl;
  BMSAutoSegAuxFile<<"EndIf (${Extension})"<<std::endl;  
  BMSAutoSegAuxFile<<"If (${Extension} == '.mha')"<<std::endl;
  BMSAutoSegAuxFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ('Meta Format')"<<std::endl;
  BMSAutoSegAuxFile<<"EndIf (${Extension})"<<std::endl;
  BMSAutoSegAuxFile<<"If (${Extension} == '.nhdr')"<<std::endl;
  BMSAutoSegAuxFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ('NHDR Format')"<<std::endl;  
  BMSAutoSegAuxFile<<"EndIf (${Extension})"<<std::endl;  
  BMSAutoSegAuxFile<<"If (${Extension} == '.nrrd')"<<std::endl;
  BMSAutoSegAuxFile<<"   set (Mode 1)"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ('NRRD Format')"<<std::endl;  
  BMSAutoSegAuxFile<<"EndIf (${Extension})"<<std::endl;
  BMSAutoSegAuxFile<<"If (${Extension} == '.gipl')"<<std::endl;
  BMSAutoSegAuxFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ('Gipl Format')"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ('mode : '${Mode})"<<std::endl;
  BMSAutoSegAuxFile<<"EndIf (${Extension})"<<std::endl; 
  BMSAutoSegAuxFile<<"If ( ${Mode} == -1)"<<std::endl;
  BMSAutoSegAuxFile<<"   set (Mode 2)"<<std::endl;
  BMSAutoSegAuxFile<<"EndIf (${Extension})"<<std::endl;
		
  BMSAutoSegAuxFile<<"echo ( )"<<std::endl;
  BMSAutoSegAuxFile<<"echo ('CHECKING IMAGE TYPE: DONE!')"<<std::endl;
  BMSAutoSegAuxFile<<"echo ( )"<<std::endl<<std::endl;

  BMSAutoSegAuxFile<<"#Creating Auxiliary Directories"<<std::endl;
  BMSAutoSegAuxFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegAuxFile<<"echo ('CREATING AUXILIARY DIRECTORIES...')"<<std::endl;
  BMSAutoSegAuxFile<<"echo ( )"<<std::endl;
  BMSAutoSegAuxFile<<"set (CaseNumber 0)"<<std::endl;
  BMSAutoSegAuxFile<<"ForEach (SourceCase ${SourceCasesList})"<<std::endl;
  BMSAutoSegAuxFile<<"   echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"   echo('Source Case : '${SourceCase})"<<std::endl;
  BMSAutoSegAuxFile<<"   echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"   GetFilename (SourceCasePath ${SourceCase} PATH)"<<std::endl;
  BMSAutoSegAuxFile<<"   GetFilename (SourceCaseParentPath ${SourceCase} PARENT_PATH)"<<std::endl;
  if (GetAux1Image())
  {
    BMSAutoSegAuxFile<<"   # Creating Auxiliary 1 Directory if necessary"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux1Label "<<GetAux1Label()<<")"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux1Dir ${SourceCaseParentPath}/${Aux1Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   ListDirInDir (AutoSegList ${SourceCaseParentPath} ${Aux1Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${AutoSegList} == '')"<<std::endl;  
    BMSAutoSegAuxFile<<"      MakeDirectory (${Aux1Dir})"<<std::endl; 
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Aux1 Directory already exists!)"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${AutoSegList})"<<std::endl;  
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 0)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AuxDirList ${Aux1Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl; 
  }	
  if (GetAux2Image())
  {
    BMSAutoSegAuxFile<<"   # Creating Auxiliary 2 Directory if necessary"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux2Label "<<GetAux2Label()<<")"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux2Dir ${SourceCaseParentPath}/${Aux2Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   ListDirInDir (AutoSegList ${SourceCaseParentPath} ${Aux2Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${AutoSegList} == '')"<<std::endl;  
    BMSAutoSegAuxFile<<"      MakeDirectory (${Aux2Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Aux2 Directory already exists!)"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${AutoSegList})"<<std::endl; 
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 0)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AuxDirList ${AuxDirList} ${Aux2Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
  }
  if (GetAux3Image())
  {
    BMSAutoSegAuxFile<<"   # Creating Auxiliary 3 Directory if necessary"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux3Label "<<GetAux3Label()<<")"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux3Dir ${SourceCaseParentPath}/${Aux3Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   ListDirInDir (AutoSegList ${SourceCaseParentPath} ${Aux3Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${AutoSegList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"      MakeDirectory (${Aux3Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Aux3 Directory already exists!)"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${AutoSegList})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 0)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AuxDirList ${AuxDirList} ${Aux3Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
  }
  if (GetAux4Image())
  {
    BMSAutoSegAuxFile<<"   # Creating Auxiliary 4 Directory if necessary"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux4Label "<<GetAux4Label()<<")"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux4Dir ${SourceCaseParentPath}/${Aux4Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   ListDirInDir (AutoSegList ${SourceCaseParentPath} ${Aux4Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${AutoSegList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"      MakeDirectory (${Aux4Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Aux4 Directory already exists!)"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${AutoSegList})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 0)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AuxDirList ${AuxDirList} ${Aux4Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
  }
  if (GetAux5Image())
  {
    BMSAutoSegAuxFile<<"   # Creating Auxiliary 5 Directory if necessary"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux5Label "<<GetAux5Label()<<")"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux5Dir ${SourceCaseParentPath}/${Aux5Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   ListDirInDir (AutoSegList ${SourceCaseParentPath} ${Aux5Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${AutoSegList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"      MakeDirectory (${Aux5Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Aux5 Directory already exists!)"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${AutoSegList})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 0)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AuxDirList ${AuxDirList} ${Aux5Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
  }
  if (GetAux6Image())
  {
    BMSAutoSegAuxFile<<"   # Creating Auxiliary 6 Directory if necessary"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux6Label "<<GetAux6Label()<<")"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux6Dir ${SourceCaseParentPath}/${Aux6Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   ListDirInDir (AutoSegList ${SourceCaseParentPath} ${Aux6Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${AutoSegList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"      MakeDirectory (${Aux6Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Aux6 Directory already exists!)"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${AutoSegList})"<<std::endl;  
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 0)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AuxDirList ${AuxDirList} ${Aux6Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
  }
  if (GetAux7Image())
  {
    BMSAutoSegAuxFile<<"   # Creating Auxiliary 7 Directory if necessary"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux7Label "<<GetAux7Label()<<")"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux7Dir ${SourceCaseParentPath}/${Aux7Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   ListDirInDir (AutoSegList ${SourceCaseParentPath} ${Aux7Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${AutoSegList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"      MakeDirectory (${Aux7Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Aux7 Directory already exists!)"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${AutoSegList})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 0)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AuxDirList ${AuxDirList} ${Aux7Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
  }
  if (GetAux8Image())
  {
    BMSAutoSegAuxFile<<"   # Creating Auxiliary 8 Directory if necessary"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux8Label "<<GetAux8Label()<<")"<<std::endl;
    BMSAutoSegAuxFile<<"   set (Aux8Dir ${SourceCaseParentPath}/${Aux8Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   ListDirInDir (AutoSegList ${SourceCaseParentPath} ${Aux8Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${AutoSegList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"      MakeDirectory (${Aux8Dir})"<<std::endl;
    BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Aux8 Directory already exists!)"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${AutoSegList})"<<std::endl;
    BMSAutoSegAuxFile<<"   If (${CaseNumber} == 0)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (AuxDirList ${AuxDirList} ${Aux8Label})"<<std::endl;
    BMSAutoSegAuxFile<<"   EndIf (${CaseNumber})"<<std::endl;
  }
  BMSAutoSegAuxFile<<"   Inc ({CaseNumber} 1)"<<std::endl;
  BMSAutoSegAuxFile<<"   Int ({CaseNumber})"<<std::endl;
  BMSAutoSegAuxFile<<"EndForEach (SourceCase)"<<std::endl;

  BMSAutoSegAuxFile<<"echo ( )"<<std::endl;
  BMSAutoSegAuxFile<<"echo ('CREATING AUXILIARY DIRECTORIES: DONE!')"<<std::endl;
  BMSAutoSegAuxFile<<"echo ( )"<<std::endl<<std::endl;

  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# 2. Source to Aux1 registration"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
	
  BMSAutoSegAuxFile<<"set (CaseNumber 0)"<<std::endl;
  BMSAutoSegAuxFile<<"ForEach (SourceCase ${SourceCasesList})"<<std::endl;
  BMSAutoSegAuxFile<<"   GetFilename (SourcePath ${SourceCase} PATH)"<<std::endl;
  BMSAutoSegAuxFile<<"   GetFilename (SourceParentPath ${SourceCase} PARENT_PATH)"<<std::endl;
  BMSAutoSegAuxFile<<"   GetFilename (SourceCaseHead ${SourceCase} NAME_WITHOUT_EXTENSION)"<<std::endl;

  BMSAutoSegAuxFile<<"   echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"   echo('Source Case : '${SourceCaseHead})"<<std::endl;
  BMSAutoSegAuxFile<<"   echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"   set (Aux1Dir "<<GetAux1Label()<<")"<<std::endl;
  BMSAutoSegAuxFile<<"   GetParam(Aux1Case ${OrigAux1CasesList} ${CaseNumber})"<<std::endl;
  BMSAutoSegAuxFile<<"   # Registration Repertory"<<std::endl;
  BMSAutoSegAuxFile<<"   set (RegPath ${SourceParentPath}/${Aux1Dir})"<<std::endl;
  BMSAutoSegAuxFile<<"   # Parameter File"<<std::endl;
  BMSAutoSegAuxFile<<"   set (TxtRegFileTail ${SourceCaseHead}_reg.txt)"<<std::endl;
  BMSAutoSegAuxFile<<"   set (TxtRegFile ${RegPath}/${TxtRegFileTail})"<<std::endl;
  BMSAutoSegAuxFile<<"   set (parFile "<<GetTissueSegmentationAtlasDirectory()<<"areg_par.txt)"<<std::endl;
  BMSAutoSegAuxFile<<"   # Report File (process)"<<std::endl;
  BMSAutoSegAuxFile<<"   set (ReportFile ${RegPath}/${SourceCaseHead}_out.txt)"<<std::endl;
  BMSAutoSegAuxFile<<"   ListFileInDir(OutputList ${RegPath} ${TxtRegFileTail})"<<std::endl;
  BMSAutoSegAuxFile<<"   If (${OutputList} == '')"<<std::endl;
  BMSAutoSegAuxFile<<"     set (TxtinitFileTail ${SourceCaseHead}_init.txt)"<<std::endl;
  BMSAutoSegAuxFile<<"     ListFileInDir(TxtinitFileList ${RegPath} ${TxtinitFileTail})"<<std::endl;
  if(GetRigidTransformation())
  {
    BMSAutoSegAuxFile<<"      echo (*************************************************)"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('RIGID REGISTRATION...')"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ( )"<<std::endl;
		
    BMSAutoSegAuxFile<<"      # Computing Transformation"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Computing rigid registration...')"<<std::endl;
    BMSAutoSegAuxFile<<"     If (${TxtinitFileList} == '')"<<std::endl;
    //BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --useRigid --initializeTransformMode useCenterOfHeadAlign --outputTransform ${TxtRegFile})"<<std::endl;
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --transformType Rigid --initializeTransformMode ${RegistrationInitialization} --outputTransform ${TxtRegFile})"<<std::endl;
    BMSAutoSegAuxFile<<"     Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --transformType Rigid --outputTransform ${TxtRegFile} --initialTransform ${RegPath}/${TxtinitFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"     EndIf (${TxtinitFileList})"<<std::endl;
    BMSAutoSegAuxFile<<"      Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"      WriteFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ( )"<<std::endl;
		
    BMSAutoSegAuxFile<<"      echo ('RIGID REGISTRATION: DONE!')"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ( )"<<std::endl<<std::endl;	
  }
  else if(GetAffineTransformation())
  {
    BMSAutoSegAuxFile<<"      echo (*************************************************)"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('AFFINE REGISTRATION...')"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ( )"<<std::endl;
		
    BMSAutoSegAuxFile<<"      # Computing Transformation"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Computing affine registration...')"<<std::endl;
    BMSAutoSegAuxFile<<"     If (${TxtinitFileList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --transformType Affine  --initializeTransformMode ${RegistrationInitialization} --outputTransform ${TxtRegFile})"<<std::endl;
    BMSAutoSegAuxFile<<"     Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --transformType Affine --outputTransform ${TxtRegFile} --initialTransform ${RegPath}/${TxtinitFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"     EndIf (${TxtinitFileList})"<<std::endl;
    BMSAutoSegAuxFile<<"      Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"      WriteFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ( )"<<std::endl;
		
    BMSAutoSegAuxFile<<"      echo ('AFFINE REGISTRATION: DONE!')"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ( )"<<std::endl<<std::endl;
  }
  else
  {
    BMSAutoSegAuxFile<<"      echo (*************************************************)"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('BSPLINE REGISTRATION...')"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ( )"<<std::endl;
		
    BMSAutoSegAuxFile<<"      # Computing Transformations"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('Computing affine registration...')"<<std::endl;
    BMSAutoSegAuxFile<<"      # Parameter File"<<std::endl;
    BMSAutoSegAuxFile<<"     If (${TxtinitFileList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --useRigid --useAffine --initializeTransformMode ${RegistrationInitialization} --outputTransform ${TxtRegFile} --interpolationMode BSpline)"<<std::endl;
    BMSAutoSegAuxFile<<"     Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --useRigid --useAffine --outputTransform ${TxtRegFile} --interpolationMode BSpline --initialTransform ${RegPath}/${TxtinitFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"     EndIf (${TxtinitFileList})"<<std::endl;
    BMSAutoSegAuxFile<<"      Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"      WriteFile(${ReportFile} ${output})"<<std::endl;
		
    BMSAutoSegAuxFile<<"      echo ( )"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ('BSPLINE REGISTRATION: DONE!')"<<std::endl;
    BMSAutoSegAuxFile<<"      echo ( )"<<std::endl<<std::endl;
  }
  BMSAutoSegAuxFile<<"   Else ()"<<std::endl;
  BMSAutoSegAuxFile<<"      echo ('File already exists: '${TxtRegFileTail})"<<std::endl;
  BMSAutoSegAuxFile<<"   EndIf (${OutputList})"<<std::endl;
  BMSAutoSegAuxFile<<"   Inc(${CaseNumber} 1)"<<std::endl;
  BMSAutoSegAuxFile<<"   Int(${CaseNumber})"<<std::endl;
  BMSAutoSegAuxFile<<"EndForEach (SourceCase)"<<std::endl;

  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# 3. Applying the transformation to the source images and their label maps"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"#---------------------------------------------------------------------"<<std::endl;

  bool IsParcellationMapListEmpty = true;
  if (GetParcellationMapSegmentation())
  {
    if (std::strlen(GetParcellationFile1()) != 0)
    {
      BMSAutoSegAuxFile<<"set (ParcellationMapList "<<GetParcellationFile1()<<")"<<std::endl;
      IsParcellationMapListEmpty = false;
    }
    if (std::strlen(GetParcellationFile2()) != 0)
    {
      if (IsParcellationMapListEmpty)
	BMSAutoSegAuxFile<<"set (ParcellationMapList "<<GetParcellationFile2()<<")"<<std::endl;
      else
	BMSAutoSegAuxFile<<"set (ParcellationMapList ${ParcellationMapList} "<<GetParcellationFile2()<<")"<<std::endl;
      IsParcellationMapListEmpty = false;
    }
    if (std::strlen(GetParcellationFile3()) != 0)
    {
      if (IsParcellationMapListEmpty)
	BMSAutoSegAuxFile<<"set (ParcellationMapList "<<GetParcellationFile3()<<")"<<std::endl;
      else
	BMSAutoSegAuxFile<<"set (ParcellationMapList ${ParcellationMapList} "<<GetParcellationFile3()<<")"<<std::endl;
      IsParcellationMapListEmpty = false;
    }
  }
  bool IsGenericROIMapListEmpty = true;
  BMSAutoSegAuxFile<<"set (GenericROIMapList empty)"<<std::endl;
  if (GetGenericROISegmentation())
  {
    if (std::strlen(GetROIFile1()) != 0)
    {
      BMSAutoSegAuxFile<<"set (GenericROIMapList "<<GetROIFile1()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
    }
    if (std::strlen(GetROIFile2()) != 0)
    {
      if (IsGenericROIMapListEmpty)
	BMSAutoSegAuxFile<<"set (GenericROIMapList "<<GetROIFile2()<<")"<<std::endl;
      else
	BMSAutoSegAuxFile<<"set (GenericROIMapList ${GenericROIMapList} "<<GetROIFile2()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
    }
    if (std::strlen(GetROIFile3()) != 0)
    {
      if (IsGenericROIMapListEmpty)
	BMSAutoSegAuxFile<<"set (GenericROIMapList "<<GetROIFile3()<<")"<<std::endl;
      else
	BMSAutoSegAuxFile<<"set (GenericROIMapList ${GenericROIMapList} "<<GetROIFile3()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
    }
    if (std::strlen(GetROIFile4()) != 0)
    {
      if (IsGenericROIMapListEmpty)
	BMSAutoSegAuxFile<<"set (GenericROIMapList "<<GetROIFile4()<<")"<<std::endl;
      else
	BMSAutoSegAuxFile<<"set (GenericROIMapList ${GenericROIMapList} "<<GetROIFile4()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
    }
    if (std::strlen(GetROIFile5()) != 0)
    {
      if (IsGenericROIMapListEmpty)
	BMSAutoSegAuxFile<<"set (GenericROIMapList "<<GetROIFile5()<<")"<<std::endl;
      else
	BMSAutoSegAuxFile<<"set (GenericROIMapList ${GenericROIMapList} "<<GetROIFile5()<<")"<<std::endl;
      IsGenericROIMapListEmpty = false;
    }
  }
  bool IsStructureListEmpty = true;

  BMSAutoSegAuxFile<<"set (VentricleListLength 0)"<<std::endl;

  if (GetSubcorticalStructureSegmentation())
  {
    if (std::strlen(GetAmygdalaLeft()) != 0)
    {
      BMSAutoSegAuxFile<<"set (StructureList "<<GetAmygdalaLeft()<<")"<<std::endl;
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetAmygdalaRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetAmygdalaRight()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetAmygdalaRight()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetCaudateLeft()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetCaudateLeft()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetCaudateLeft()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetCaudateRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetCaudateRight()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetCaudateRight()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetHippocampusLeft()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetHippocampusLeft()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetHippocampusLeft()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetHippocampusRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetHippocampusRight()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetHippocampusRight()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetPallidusLeft()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetPallidusLeft()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetPallidusLeft()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetPallidusRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetPallidusRight()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetPallidusRight()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetPutamenLeft()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetPutamenLeft()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetPutamenLeft()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetPutamenRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetPutamenRight()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetPutamenRight()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetLateralVentricleLeft()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetLateralVentricleLeft()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetLateralVentricleLeft()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
    if (std::strlen(GetLateralVentricleRight()) != 0)
    {
      if (IsStructureListEmpty)
      {
	BMSAutoSegAuxFile<<"set (StructureList "<<GetLateralVentricleRight()<<")"<<std::endl;
      }
      else
      {
	BMSAutoSegAuxFile<<"set (StructureList ${StructureList} "<<GetLateralVentricleRight()<<")"<<std::endl;
      }
      IsStructureListEmpty = false;
    }
  }
  BMSAutoSegAuxFile<<"# Applying Transformation"<<std::endl;
  BMSAutoSegAuxFile<<"set (CaseNumber 0)"<<std::endl;
  BMSAutoSegAuxFile<<"set (Quantiles "<<GetQuantiles()<<")"<<std::endl;
  BMSAutoSegAuxFile<<"ForEach (SourceCase ${SourceCasesList})"<<std::endl;
	
  BMSAutoSegAuxFile<<"   GetFilename (SourcePath ${SourceCase} PATH)"<<std::endl;
  BMSAutoSegAuxFile<<"   GetFilename (SourceParentPath ${SourceCase} PARENT_PATH)"<<std::endl;
  BMSAutoSegAuxFile<<"   GetFilename (SourceCaseHead ${SourceCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
	
  BMSAutoSegAuxFile<<"   echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"   echo('Source Case: '${SourceCaseHead})"<<std::endl;
  BMSAutoSegAuxFile<<"   echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"   set (DirNumber 0)"<<std::endl;
  BMSAutoSegAuxFile<<"   set (SourceImageFlag 0)"<<std::endl;
  BMSAutoSegAuxFile<<"   ForEach (AuxDir ${AuxDirList})"<<std::endl;
  BMSAutoSegAuxFile<<"      echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"      echo('Auxiliary Directory: '${AuxDir})"<<std::endl;
  BMSAutoSegAuxFile<<"      echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"   echo (*************************************************)"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ('APPLYING TRANSFORMATION...')"<<std::endl;
  BMSAutoSegAuxFile<<"   echo ( )"<<std::endl;
  BMSAutoSegAuxFile<<"      set (AuxPath ${SourceParentPath}/${AuxDir}/)"<<std::endl;
  BMSAutoSegAuxFile<<"      Math( NbAuxData2 ${NbAuxData} * ${DirNumber})"<<std::endl;
  BMSAutoSegAuxFile<<"      Math( NbAuxData3 ${NbAuxData2} + ${CaseNumber})"<<std::endl;
  BMSAutoSegAuxFile<<"      GetParam(AuxCase ${OrigAuxCasesList} ${NbAuxData3})"<<std::endl;
  BMSAutoSegAuxFile<<"      echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"      echo('Auxiliary Case: '${AuxCase})"<<std::endl;
  BMSAutoSegAuxFile<<"      echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"      set (Aux1Dir "<<GetAux1Label()<<")"<<std::endl;

  BMSAutoSegAuxFile<<"      set (Aux1Path ${SourceParentPath}/${Aux1Dir}/)"<<std::endl;
  BMSAutoSegAuxFile<<"      # Parameter File"<<std::endl;
  BMSAutoSegAuxFile<<"      set (TxtRegFileTail ${SourceCaseHead}_reg.txt)"<<std::endl;
  BMSAutoSegAuxFile<<"      set (TxtRegFile ${Aux1Path}${TxtRegFileTail})"<<std::endl;

  BMSAutoSegAuxFile<<"      # Report File (process)"<<std::endl; 
  BMSAutoSegAuxFile<<"      set (ReportFile ${Aux1Path}${SourceCaseHead}_out.txt)"<<std::endl;
  BMSAutoSegAuxFile<<"      # WarpROI Repertory"<<std::endl;
  BMSAutoSegAuxFile<<"      set (WarpROIDir ${SourceParentPath}/WarpROI/)"<<std::endl;
  BMSAutoSegAuxFile<<"      set (StrippedDir ${SourceParentPath}/Stripped/)"<<std::endl;
  BMSAutoSegAuxFile<<"      set (OutputFile2 ${AuxPath}${SourceCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;

  BMSAutoSegAuxFile<<"      GetFilename (OutputFile2Tail ${OutputFile2} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${OutputFile2Tail}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
  BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${SourceCase} ${OutputFile2} --transformationFile ${TxtRegFile} -i bs --Reference ${AuxCase})"<<std::endl;
  BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
  BMSAutoSegAuxFile<<"         AppendFile(${ReportFile} ${output})"<<std::endl;
  BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
  BMSAutoSegAuxFile<<"         echo ('File already exists: '${OutputFile2Tail})"<<std::endl;
  BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;
  if(!IsParcellationMapListEmpty)
  {
    BMSAutoSegAuxFile<<"      ForEach (ParcellationMapName ${ParcellationMapList})"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (ParcellationMapNameHead ${ParcellationMapName} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"      ListFileInDir(WarpRegCasesList ${WarpROIDir} *${ParcellationMapNameHead}-WarpReg.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"      ForEach (WarpRegCase ${WarpRegCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         echo('WarpReg Case: '${WarpRegCase})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (WarpRegCaseHead ${WarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         set (OutputFile ${AuxPath}${WarpRegCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (OutputFileTail ${OutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         ListFileInDir(OutputList ${AuxPath} ${OutputFileTail}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         If (${OutputList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${WarpROIDir}${WarpRegCase} ${OutputFile} --transformationFile ${TxtRegFile} -i nn --Reference ${AuxCase})"<<std::endl;
    BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"            AppendFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"            echo ('File already exists: '${OutputFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"         EndIf (${OutputList})"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (WarpRegCase)"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (ParcellationMapName)"<<std::endl;
  }
  if (!IsGenericROIMapListEmpty)
  {
    BMSAutoSegAuxFile<<"      ForEach (GenericROIMap ${GenericROIMapList})"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename(GenericROIMapHead ${GenericROIMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"      ListFileInDir(WarpRegCasesList ${WarpROIDir} *${GenericROIMapHead}-WarpReg.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"      ForEach (WarpRegCase ${WarpRegCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         echo('WarpReg Case: '${WarpRegCase})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (WarpRegCaseHead ${WarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         set (OutputFile ${AuxPath}${WarpRegCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (OutputFileTail ${OutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         ListFileInDir(OutputList ${AuxPath} ${OutputFileTail}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         If (${OutputList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${WarpROIDir}${WarpRegCase} ${OutputFile} --transformationFile ${TxtRegFile} -i nn --Reference ${AuxCase})"<<std::endl;
    BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"            AppendFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"            echo ('File already exists: '${OutputFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"         EndIf (${OutputList})"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (WarpRegCase)"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (GenericROIMap)"<<std::endl;
  }
  if (!IsStructureListEmpty)
  {
    BMSAutoSegAuxFile<<"      ForEach (Structure ${StructureList})"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename(StructureHead ${Structure} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"      ListFileInDir(WarpRegCasesList ${WarpROIDir} *${StructureHead}-WarpReg.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"      ForEach (WarpRegCase ${WarpRegCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         echo('WarpReg Case: '${WarpRegCase})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (WarpRegCaseHead ${WarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         set (OutputFile ${AuxPath}${WarpRegCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (OutputFileTail ${OutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         ListFileInDir(OutputList ${AuxPath} ${OutputFileTail}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         If (${OutputList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${WarpROIDir}${WarpRegCase} ${OutputFile} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
    BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"            AppendFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"            echo ('File already exists: '${OutputFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"         EndIf (${OutputList})"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (WarpRegCase)"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (Structure)"<<std::endl;

    BMSAutoSegAuxFile<<"         ListFileInDir(MaskList ${StrippedDir} *mask.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         GetParam(Mask ${MaskList} ${CaseNumber})"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename(MaskHead ${Mask} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         set (MaskOutputFile ${AuxPath}${MaskHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (MaskOutputFileTail ${MaskOutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         ListFileInDir(MaskOutputList ${AuxPath} ${MaskOutputFileTail}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         If (${MaskOutputList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${StrippedDir}${Mask} ${MaskOutputFile} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
    BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"            AppendFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"            echo ('File already exists: '${MaskOutputFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"         EndIf (${MaskOutputList})"<<std::endl;
  }
  std::string SoftTissueMap =GetSoftTissueMap();
  if (SoftTissueMap == "Hard")
  {
    
    if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
      {
	BMSAutoSegAuxFile<<"      ListFileInDir(MWMWarpRegCasesList ${WarpROIDir} *WarpReg_MWM.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      ForEach (MWMWarpRegCase ${MWMWarpRegCasesList})"<<std::endl;
	BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
	BMSAutoSegAuxFile<<"         echo('MWM WarpReg Case: '${MWMWarpRegCase})"<<std::endl;
	BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
	BMSAutoSegAuxFile<<"         GetFilename (MWMWarpRegCaseHead ${MWMWarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegAuxFile<<"         set (OutputFile ${AuxPath}${MWMWarpRegCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"         GetFilename (OutputFileTail ${OutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegAuxFile<<"         ListFileInDir(OutputList ${AuxPath} ${OutputFileTail}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"         If (${OutputList} == '')"<<std::endl;
	BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${WarpROIDir}${MWMWarpRegCase} ${OutputFile} --transformationFile ${TxtRegFile} -i nn --Reference ${AuxCase})"<<std::endl;
	BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
	BMSAutoSegAuxFile<<"            AppendFile(${ReportFile} ${output})"<<std::endl;
	BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
	BMSAutoSegAuxFile<<"            echo ('File already exists: '${OutputFileTail})"<<std::endl;
	BMSAutoSegAuxFile<<"         EndIf (${OutputList})"<<std::endl;
	BMSAutoSegAuxFile<<"      EndForEach (MWMWarpRegCase)"<<std::endl;
      }

    BMSAutoSegAuxFile<<"      ListFileInDir(WMWarpRegCasesList ${WarpROIDir} *WarpReg_WM.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"      ForEach (WMWarpRegCase ${WMWarpRegCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         echo('WM WarpReg Case: '${WMWarpRegCase})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (WMWarpRegCaseHead ${WMWarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         set (OutputFile ${AuxPath}${WMWarpRegCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (OutputFileTail ${OutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         ListFileInDir(OutputList ${AuxPath} ${OutputFileTail}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         If (${OutputList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${WarpROIDir}${WMWarpRegCase} ${OutputFile} --transformationFile ${TxtRegFile} -i nn --Reference ${AuxCase})"<<std::endl;
    BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"            AppendFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"            echo ('File already exists: '${OutputFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"         EndIf (${OutputList})"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (WMWarpRegCase)"<<std::endl;
    BMSAutoSegAuxFile<<"      ListFileInDir(GMWarpRegCasesList ${WarpROIDir} *WarpReg_GM.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"      ForEach (GMWarpRegCase ${GMWarpRegCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         echo('GM WarpReg Case: '${GMWarpRegCase})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (GMWarpRegCaseHead ${GMWarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         set (OutputFile ${AuxPath}${GMWarpRegCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (OutputFileTail ${OutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         ListFileInDir(OutputList ${AuxPath} ${OutputFileTail}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         If (${OutputList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${WarpROIDir}${GMWarpRegCase} ${OutputFile} --transformationFile ${TxtRegFile} -i nn --Reference ${AuxCase})"<<std::endl;
    BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"            AppendFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"            echo ('File already exists: '${OutputFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"         EndIf (${OutputList})"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (GMWarpRegCase)"<<std::endl;
    BMSAutoSegAuxFile<<"      ListFileInDir(CSFWarpRegCasesList ${WarpROIDir} *WarpReg_CSF.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"      ForEach (CSFWarpRegCase ${CSFWarpRegCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         echo('CSF WarpReg Case: '${CSFWarpRegCase})"<<std::endl;
    BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (CSFWarpRegCaseHead ${CSFWarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         set (OutputFile ${AuxPath}${CSFWarpRegCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (OutputFileTail ${OutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         ListFileInDir(OutputList ${AuxPath} ${OutputFileTail}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         If (${OutputList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${WarpROIDir}${CSFWarpRegCase} ${OutputFile} --transformationFile ${TxtRegFile} -i nn --Reference ${AuxCase})"<<std::endl;
    BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegAuxFile<<"            AppendFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"            echo ('File already exists: '${OutputFileTail})"<<std::endl;
    BMSAutoSegAuxFile<<"         EndIf (${OutputList})"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (CSFWarpRegCase)"<<std::endl;
  }
  BMSAutoSegAuxFile<<"      ListFileInDir(HardSegCasesList ${WarpROIDir} *HardSeg.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      ForEach (HardSegCase ${HardSegCasesList})"<<std::endl;
  BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"         echo('HardSeg Case: '${HardSegCase})"<<std::endl;
  BMSAutoSegAuxFile<<"         echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"         GetFilename (HardSegCaseHead ${HardSegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"         set (HardSegCase ${WarpROIDir}${HardSegCaseHead}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"         set (OutputFile ${AuxPath}${HardSegCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"         GetFilename (OutputFileTail ${OutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"         ListFileInDir(OutputList ${AuxPath} ${OutputFileTail}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"         If (${OutputList} == '')"<<std::endl;
  BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${HardSegCase} ${OutputFile} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
  BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
  BMSAutoSegAuxFile<<"            AppendFile(${ReportFile} ${output})"<<std::endl;
  BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
  BMSAutoSegAuxFile<<"            echo ('File already exists: '${OutputFileTail})"<<std::endl;
  BMSAutoSegAuxFile<<"         EndIf (${OutputList})"<<std::endl;
  BMSAutoSegAuxFile<<"      EndForEach (HardSegCase)"<<std::endl;
  BMSAutoSegAuxFile<<"      ListFileInDir(AllROICase ${WarpROIDir} *AllROI.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      If (${AllROICase} != '')"<<std::endl;
  BMSAutoSegAuxFile<<"        echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"        echo('AllROI case: '${AllROICase})"<<std::endl;
  BMSAutoSegAuxFile<<"        echo( )"<<std::endl;
  BMSAutoSegAuxFile<<"        GetFilename (AllROICaseHead ${AllROICase} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"        set (AllROICase ${WarpROIDir}${AllROICaseHead}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"        set (AllROIOutputFile ${AuxPath}${AllROICaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"        GetFilename (AllROIOutputFileTail ${AllROIOutputFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"        ListFileInDir(OutputList ${AuxPath} ${AllROIOutputFileTail}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"        If (${OutputList} == '')"<<std::endl;
  BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${AllROICase} ${AllROIOutputFile} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
  BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
  BMSAutoSegAuxFile<<"          AppendFile(${ReportFile} ${output})"<<std::endl;
  BMSAutoSegAuxFile<<"        Else ()"<<std::endl;
  BMSAutoSegAuxFile<<"         echo ('File already exists: '${AllROIOutputFileTail})"<<std::endl;
  BMSAutoSegAuxFile<<"        EndIf (${OutputList})"<<std::endl;
  BMSAutoSegAuxFile<<"      EndIf (${AllROICase})"<<std::endl;
  BMSAutoSegAuxFile<<"      GetParam(OrigSourceCase ${OrigSourceCasesList} ${CaseNumber})"<<std::endl;
  BMSAutoSegAuxFile<<"      GetFilename (OrigSourceCaseHead ${OrigSourceCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      GetFilename (OrigSourceCasePath ${OrigSourceCase} PATH)"<<std::endl;	
  if (GetLoop())
  {
    int SuffixIteration=GetLoopIteration()+1;
    BMSAutoSegAuxFile<<"      set (EMSPath ${OrigSourceCasePath}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (stripEMS _stripEMS${StrippedBias})"<<std::endl;
  }
  else
  {
    BMSAutoSegAuxFile<<"      set (EMSPath ${OrigSourceCasePath}/${AutoSegDir}/ems/)"<<std::endl;
    BMSAutoSegAuxFile<<"      set (stripEMS '')"<<std::endl;
  }
  if ( (std::strcmp(GetTissueSegmentationAtlasType(),"T2") == 0) && (GetT2Image()) )
  {
    if (GetAuxT2Image())
    {
      BMSAutoSegAuxFile<<"	  set (PosteriorOrigSourceCasesHead ${OrigSourceCaseHead})"<<std::endl;
    }
    else
    {
      BMSAutoSegAuxFile<<"set (PosteriorOrigSourceCasesList "<<m_T2List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegAuxFile<<"set (PosteriorOrigSourceCasesList ${PosteriorOrigSourceCasesList} "<<m_T2List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"	  set (flagPosterior 0)"<<std::endl;
      BMSAutoSegAuxFile<<"	  ForEach (PosteriorOrigSourceCases ${PosteriorOrigSourceCasesList})"<<std::endl;
      BMSAutoSegAuxFile<<"        If (${flagPosterior} == ${CaseNumber})"<<std::endl;
      BMSAutoSegAuxFile<<"          GetFilename (PosteriorOrigSourceCasesHead ${PosteriorOrigSourceCases} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegAuxFile<<"        EndIf (${flagPosterior})"<<std::endl;
      BMSAutoSegAuxFile<<"       Inc (${flagPosterior} 1)"<<std::endl;
      BMSAutoSegAuxFile<<"       Int (${flagPosterior})"<<std::endl;
      BMSAutoSegAuxFile<<"      EndForEach (${PosteriorOrigSourceCasesList})"<<std::endl;
    }
    BMSAutoSegAuxFile<<"         echo (${PosteriorOrigSourceCasesHead})"<<std::endl;
    
    if (std::strcmp(GetEMSoftware(), "ABC") == 0)
      {
	BMSAutoSegAuxFile<<"      set(WMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T2RegistrationExtension}${stripEMS}_posterior0_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(GMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T2RegistrationExtension}${stripEMS}_posterior1_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(CSFProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T2RegistrationExtension}${stripEMS}_posterior2_${SUFFIX}.nrrd)"<<std::endl;
      }
    else
      {
	BMSAutoSegAuxFile<<"      set(MWMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T2RegistrationExtension}${stripEMS}${NEOSEG_PREFIX}_posterior0_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(WMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T2RegistrationExtension}${stripEMS}${NEOSEG_PREFIX}_posterior1_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(GMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T2RegistrationExtension}${stripEMS}${NEOSEG_PREFIX}_posterior2_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(CSFProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T2RegistrationExtension}${stripEMS}${NEOSEG_PREFIX}_posterior3_${SUFFIX}.nrrd)"<<std::endl;
      }
  }
  else
  {
    if (GetAuxT1Image())
    {
      BMSAutoSegAuxFile<<"	  set (PosteriorOrigSourceCasesHead ${OrigSourceCaseHead})"<<std::endl;
    }
    else
    {
      BMSAutoSegAuxFile<<"set (PosteriorOrigSourceCasesList "<<m_T1List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegAuxFile<<"set (PosteriorOrigSourceCasesList ${PosteriorOrigSourceCasesList} "<<m_T1List[DataNumber]<<")"<<std::endl;
      BMSAutoSegAuxFile<<"	  set (flagPosterior 0)"<<std::endl;
      BMSAutoSegAuxFile<<"	  ForEach (PosteriorOrigSourceCases ${PosteriorOrigSourceCasesList})"<<std::endl;
      BMSAutoSegAuxFile<<"        If (${flagPosterior} == ${CaseNumber})"<<std::endl;
      BMSAutoSegAuxFile<<"          GetFilename (PosteriorOrigSourceCasesHead ${PosteriorOrigSourceCases} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegAuxFile<<"        EndIf (${flagPosterior})"<<std::endl;
      BMSAutoSegAuxFile<<"       Inc (${flagPosterior} 1)"<<std::endl;
      BMSAutoSegAuxFile<<"       Int (${flagPosterior})"<<std::endl;
      BMSAutoSegAuxFile<<"      EndForEach (${PosteriorOrigSourceCasesList})"<<std::endl;
    }
    
    if (std::strcmp(GetEMSoftware(), "ABC") == 0)
      {
	BMSAutoSegAuxFile<<"      set(WMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T1RegistrationExtension}${stripEMS}_posterior0_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(GMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T1RegistrationExtension}${stripEMS}_posterior1_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(CSFProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T1RegistrationExtension}${stripEMS}_posterior2_${SUFFIX}.nrrd)"<<std::endl;
      }
    else
      {
	BMSAutoSegAuxFile<<"      set(MWMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T1RegistrationExtension}${stripEMS}${NEOSEG_PREFIX}_posterior0_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(WMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T1RegistrationExtension}${stripEMS}${NEOSEG_PREFIX}_posterior1_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(GMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T1RegistrationExtension}${stripEMS}${NEOSEG_PREFIX}_posterior2_${SUFFIX}.nrrd)"<<std::endl;
	BMSAutoSegAuxFile<<"      set(CSFProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T1RegistrationExtension}${stripEMS}${NEOSEG_PREFIX}_posterior3_${SUFFIX}.nrrd)"<<std::endl;
      }
  }

  if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
    {
      BMSAutoSegAuxFile<<"      GetFilename (MWMProbMapHead ${MWMProbMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegAuxFile<<"      set (OutputFileMWM ${AuxPath}${MWMProbMapHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
      BMSAutoSegAuxFile<<"      GetFilename (OutputFileMWMTail ${OutputFileMWM} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${OutputFileMWMTail}.nrrd)"<<std::endl;
      BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
      BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${MWMProbMap} ${OutputFileMWM} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
      BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
      BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
      BMSAutoSegAuxFile<<"         echo ('File already exists: '${OutputFileMWMTail})"<<std::endl;
      BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;      
    }

  BMSAutoSegAuxFile<<"      GetFilename (WMProbMapHead ${WMProbMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      set (OutputFileWM ${AuxPath}${WMProbMapHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      GetFilename (OutputFileWMTail ${OutputFileWM} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${OutputFileWMTail}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
  BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${WMProbMap} ${OutputFileWM} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
  BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
  BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
  BMSAutoSegAuxFile<<"         echo ('File already exists: '${OutputFileWMTail})"<<std::endl;
  BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;

  BMSAutoSegAuxFile<<"      GetFilename (GMProbMapHead ${GMProbMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      set (OutputFileGM ${AuxPath}${GMProbMapHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      GetFilename (OutputFileGMTail ${OutputFileGM} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${OutputFileGMTail}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
  BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${GMProbMap} ${OutputFileGM} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
  BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
  BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
  BMSAutoSegAuxFile<<"         echo ('File already exists: '${OutputFileGMTail})"<<std::endl;
  BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;
	
  BMSAutoSegAuxFile<<"      GetFilename (CSFProbMapHead ${CSFProbMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      set (OutputFileCSF ${AuxPath}${CSFProbMapHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      GetFilename (OutputFileCSFTail ${OutputFileCSF} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${OutputFileCSFTail}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
  BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${CSFProbMap} ${OutputFileCSF} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
  BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
  BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
  BMSAutoSegAuxFile<<"         echo ('File already exists: '${OutputFileCSFTail})"<<std::endl;
  BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;
	
  BMSAutoSegAuxFile<<"      echo ( )"<<std::endl;
  BMSAutoSegAuxFile<<"      echo ('APPLYING TRANSFORMATION: DONE!')"<<std::endl;
  BMSAutoSegAuxFile<<"      echo ( )"<<std::endl<<std::endl;
	
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# 4. Statistic files                                                   "<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegAuxFile<<"# ---------------------------------------------------------------------"<<std::endl;	
	
  BMSAutoSegAuxFile<<"      echo (*************************************************)"<<std::endl;
  BMSAutoSegAuxFile<<"      echo ('WRITING STATISTICS FILES...')"<<std::endl;
  BMSAutoSegAuxFile<<"      echo ( )"<<std::endl;
  if(!IsParcellationMapListEmpty)
  {
    BMSAutoSegAuxFile<<"      ForEach (ParcellationMapName ${ParcellationMapList})"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (ParcellationMapNameHead ${ParcellationMapName} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (SoftTissueMap == "Soft")
    {
      BMSAutoSegAuxFile<<"      ListFileInDir(WarpRegCasesList ${AuxPath} *${ParcellationMapNameHead}-WarpReg_Reg_${AuxDir}.nrrd)"<<std::endl;
      BMSAutoSegAuxFile<<"      ForEach (WarpRegCase ${WarpRegCasesList})"<<std::endl;
      BMSAutoSegAuxFile<<"         GetFilename (WarpRegCaseHead ${WarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;

      if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
	{
	  BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${WarpRegCaseHead}_MWM_intensitySummary.csv)"<<std::endl;
	  BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
	  BMSAutoSegAuxFile<<"         set(MWMProbMapTr ${OutputFileMWM})"<<std::endl;
	  BMSAutoSegAuxFile<<"         set(command_line ${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -probabilityMap ${MWMProbMapTr} -outbase ${AuxPath}${WarpRegCaseHead}_MWM -quantile ${Quantiles})"<<std::endl;
	  BMSAutoSegAuxFile<<"        Run (output ${command_line})"<<std::endl; 	
	  BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
	  BMSAutoSegAuxFile<<"         echo ('File already exists: '${AuxPath}${WarpRegCaseHead}_MWM_intensitySummary.csv)"<<std::endl;
	  BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;	  
	}

      BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${WarpRegCaseHead}_WM_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
      BMSAutoSegAuxFile<<"         set(WMProbMapTr ${OutputFileWM})"<<std::endl;
      BMSAutoSegAuxFile<<"         set(command_line ${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -probabilityMap ${WMProbMapTr} -outbase ${AuxPath}${WarpRegCaseHead}_WM -quantile ${Quantiles})"<<std::endl;
      BMSAutoSegAuxFile<<"        Run (output ${command_line})"<<std::endl; 	
      BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
      BMSAutoSegAuxFile<<"         echo ('File already exists: '${AuxPath}${WarpRegCaseHead}_WM_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;

      BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath}  ${WarpRegCaseHead}_GM_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
      BMSAutoSegAuxFile<<"         set(GMProbMapTr ${OutputFileGM})"<<std::endl;
      BMSAutoSegAuxFile<<"         set (command_line ${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -probabilityMap ${GMProbMapTr} -outbase ${AuxPath}${WarpRegCaseHead}_GM -quantile ${Quantiles})"<<std::endl;
      BMSAutoSegAuxFile<<"        Run (output ${command_line})"<<std::endl; 	
      BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
      BMSAutoSegAuxFile<<"         echo ('File already exists: '${WarpRegCaseHead}_GM_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;

      BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath}  ${WarpRegCaseHead}_CSF_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
      BMSAutoSegAuxFile<<"         set(CSFProbMapTr ${OutputFileCSF})"<<std::endl;
      BMSAutoSegAuxFile<<"         set(command_line ${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -probabilityMap ${CSFProbMapTr} -outbase ${AuxPath}${WarpRegCaseHead}_CSF -quantile ${Quantiles})"<<std::endl;
      BMSAutoSegAuxFile<<"        Run (output ${command_line})"<<std::endl; 	
      BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
      BMSAutoSegAuxFile<<"         echo ('File already exists: '${WarpRegCaseHead}_CSF_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;

      BMSAutoSegAuxFile<<"      EndForEach (WarpRegCase)"<<std::endl;
    }
    else
    {
      if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
	BMSAutoSegAuxFile<<"      set(LabelList MWM WM GM CSF)"<<std::endl;
      else
	BMSAutoSegAuxFile<<"      set(LabelList WM GM CSF)"<<std::endl;

      BMSAutoSegAuxFile<<"      ForEach (Label ${LabelList})"<<std::endl;
      BMSAutoSegAuxFile<<"      ListFileInDir(WarpRegCasesList ${AuxPath} *${ParcellationMapNameHead}-WarpReg_${Label}_Reg_${AuxDir}.nrrd)"<<std::endl;
      BMSAutoSegAuxFile<<"      ForEach (WarpRegCase ${WarpRegCasesList})"<<std::endl;
      BMSAutoSegAuxFile<<"         GetFilename (WarpRegCaseHead ${WarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${WarpRegCaseHead}_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
      BMSAutoSegAuxFile<<"         set (command_line ${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -outbase ${AuxPath}${WarpRegCaseHead} -quantile ${Quantiles})"<<std::endl;
      BMSAutoSegAuxFile<<"        Run (output ${command_line})"<<std::endl; 	
      BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
      BMSAutoSegAuxFile<<"         echo ('File already exists: '${WarpRegCaseHead}_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;
      BMSAutoSegAuxFile<<"      EndForEach (WarpRegCase)"<<std::endl;
      BMSAutoSegAuxFile<<"      EndForEach (Label)"<<std::endl;
    }
    BMSAutoSegAuxFile<<"      EndForEach (ParcellationMapName)"<<std::endl;
  }
  if (!IsStructureListEmpty)
  {
    BMSAutoSegAuxFile<<"      ForEach (Structure ${StructureList})"<<std::endl;
    BMSAutoSegAuxFile<<"      GetFilename (StructureHead ${Structure} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"      ListFileInDir(WarpRegCasesList ${AuxPath} *${StructureHead}-WarpReg_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"      ForEach (WarpRegCase ${WarpRegCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename (WarpRegCaseHead ${WarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         ListFileInDir(OutputList ${AuxPath} ${WarpRegCaseHead}_intensitySummary.csv)"<<std::endl;
    BMSAutoSegAuxFile<<"         If (${OutputList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"           set (command_line ${ImageStatCmd} ${AuxCase} -label ${MaskOutputFile} -intensitySummary -probabilityMap ${AuxPath}${WarpRegCase} -outbase ${AuxPath}${WarpRegCaseHead} -quantile ${Quantiles})"<<std::endl;
      BMSAutoSegAuxFile<<"        Run (output ${command_line})"<<std::endl; 
    BMSAutoSegAuxFile<<"         Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"            echo ('File already exists: '${StructureHead}_intensitySummary.csv)"<<std::endl;
    BMSAutoSegAuxFile<<"         EndIf (${OutputList})"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (WarpRegCase)"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (Structure)"<<std::endl;
  }
  if (!IsGenericROIMapListEmpty)
  {
    BMSAutoSegAuxFile<<"      ForEach (GenericROIMap ${GenericROIMapList})"<<std::endl;
    BMSAutoSegAuxFile<<"         GetFilename(GenericROIMapHead ${GenericROIMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"         ListFileInDir(WarpRegCasesList ${AuxPath} *${GenericROIMapHead}-WarpReg_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"         ForEach (WarpRegCase ${WarpRegCasesList})"<<std::endl;
    BMSAutoSegAuxFile<<"            GetFilename (WarpRegCaseHead ${WarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegAuxFile<<"            ListFileInDir(OutputList ${AuxPath} ${WarpRegCaseHead}_intensitySummary.csv)"<<std::endl;
    BMSAutoSegAuxFile<<"            If (${OutputList} == '')"<<std::endl;
    BMSAutoSegAuxFile<<"               set (command_line ${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -outbase ${AuxPath}${WarpRegCaseHead} -quantile ${Quantiles})"<<std::endl;
    BMSAutoSegAuxFile<<"              Run (output ${command_line})"<<std::endl; 
    BMSAutoSegAuxFile<<"            Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"                echo ('File already exists: '${WarpRegCaseHead}_intensitySummary.csv)"<<std::endl;
    BMSAutoSegAuxFile<<"            EndIf (${OutputList})"<<std::endl;
    BMSAutoSegAuxFile<<"         EndForEach (WarpRegCase)"<<std::endl;
    BMSAutoSegAuxFile<<"      EndForEach (GenericROIMap)"<<std::endl;
  }

  BMSAutoSegAuxFile<<"      echo ( )"<<std::endl;
  BMSAutoSegAuxFile<<"      echo ('WRITING STATISTICS FILES: DONE!')"<<std::endl;
  BMSAutoSegAuxFile<<"      echo ( )"<<std::endl<<std::endl;
  BMSAutoSegAuxFile<<"      Inc(${DirNumber} 1)"<<std::endl;
  BMSAutoSegAuxFile<<"      Int(${DirNumber})"<<std::endl;
  BMSAutoSegAuxFile<<"   EndForEach (AuxDir)"<<std::endl;
  BMSAutoSegAuxFile<<"   Inc(${CaseNumber} 1)"<<std::endl;
  BMSAutoSegAuxFile<<"   Int(${CaseNumber})"<<std::endl;
  BMSAutoSegAuxFile<<"EndForEach (SourceCase)"<<std::endl;
  BMSAutoSegAuxFile.close();
}


void AutoSegComputation::WriteBMSAutoSegMRMLSourceFile()
{
}

void AutoSegComputation::WriteBMSAutoSegMRMLAllROIFile()
{
}

void AutoSegComputation::ExecuteSlicer3withScene(std::string pathSlicer)
{
  itksys::Glob globMRML;
  std::string pathMRML =GetProcessDataDirectory();
  std::string path="AutoSeg_MRML/*.mrml";
  pathMRML=pathMRML+path;
  globMRML.FindFiles(pathMRML);
  std::vector<std::string> MRML=globMRML.GetFiles();
  if (!MRML.empty())
  {

    std::ofstream LogFile(GetLogFile());
	
    std::vector<const char*> args;
    char* data = NULL;
    int length;
    int Value;
    args.push_back(pathSlicer.c_str());
    args.push_back((MRML.front()).c_str());
	// don't forget this line at the end of the argument list
    args.push_back(0);
	
    m_Process = itksysProcess_New();
    itksysProcess_SetCommand(m_Process, &*args.begin());
    itksysProcess_SetOption(m_Process,itksysProcess_Option_HideWindow,1);
    itksysProcess_Execute(m_Process);
    while((Value = itksysProcess_WaitForData(m_Process,&data,&length,NULL)))
    {
      if (Value == itksysProcess_Pipe_STDERR)
      {
	for(int i=0;i<length;i++)
	  std::cout<<data[i];
      }
    }
    itksysProcess_WaitForExit(m_Process, 0);
	
    LogFile<<m_output.c_str()<<std::endl;
	
    switch(itksysProcess_GetState(m_Process))
    {
      case itksysProcess_State_Exited:
      {
	itksysProcess_GetExitValue(m_Process);
      } break;
      case itksysProcess_State_Error:
      {
	std::cerr<<"Error: Could not run " << args[0]<<":\n"<<itksysProcess_GetErrorString(m_Process)<<"\n";
	LogFile<<"Error: Could not run " << args[0]<<":\n"<<itksysProcess_GetErrorString(m_Process)<<"\n";
      } break;
      case itksysProcess_State_Exception:
      {
	std::cerr<<"Error: "<<args[0]<<" terminated with an exception: "<<itksysProcess_GetExceptionString(m_Process)<<"\n";
	LogFile<<"Error: "<<args[0]<<" terminated with an exception: "<<itksysProcess_GetExceptionString(m_Process)<<"\n";
      } break;
      case itksysProcess_State_Starting:
      case itksysProcess_State_Executing:
      case itksysProcess_State_Expired:  
      case itksysProcess_State_Killed:  
      {
		// Should not get here.
	std::cerr<<"Unexpected ending state after running "<<args[0]<<std::endl;
	LogFile<<"Unexpected ending state after running "<<args[0]<<std::endl;
      } break;
    }
    LogFile.close();
    itksysProcess_Delete(m_Process);  
    SetIsAutoSegInProcess(false);   
  }
  else
  {
    std::cerr<<"No MRML Scene found"<<std::endl;
  }
}

void AutoSegComputation::LoadAuxComputationFile(const char *_FileName)
{
  FILE* AuxComputationFile;
  char Line[1536];
  int Length;
	
  // Computation Options
  int IsAux1Image, IsAux2Image, IsAux3Image, IsAux4Image, IsAux5Image, IsAux6Image, IsAux7Image, IsAux8Image;
  int IsAuxT1Image, IsAuxT2Image, IsAuxPDImage;
  int RigidTransformation, AffineTransformation, BsplineTransformation, AtlasSpaceImage, BiasCorrectedImage, SkullStrippedImage;
  int nbData=0;
  std::vector<const char *> DataList;
	
  if ((AuxComputationFile = fopen(_FileName,"r")) != NULL) 
  {
    while ( (fgets(Line,1536,AuxComputationFile)) != NULL)
    {
      Length = std::strlen(Line);
      Line[Length-1] = '\0';
      if ( (std::strncmp("Is AuxT1 Image: ", Line, 16)) == 0)
      {
	IsAuxT1Image = atoi(Line+16);
	SetAuxT1Image(IsAuxT1Image);
      }
      else if ( (std::strncmp("Is AuxT2 Image: ", Line, 16)) == 0)
      {
	IsAuxT2Image = atoi(Line+16);
	SetAuxT2Image(IsAuxT2Image);
      }
      else
      {
	IsAuxPDImage = atoi(Line+16);
	SetAuxPDImage(IsAuxPDImage);
      }
      if ( (std::strncmp("Is Aux1 Image: ", Line, 15)) == 0)
      {
	IsAux1Image = atoi(Line+15);
	SetAux1Image(IsAux1Image);
      }
      else if ( (std::strncmp("Is Aux2 Image: ", Line, 15)) == 0)
      {
	IsAux2Image = atoi(Line+15);
	SetAux2Image(IsAux2Image);
      }
      else if ( (std::strncmp("Is Aux3 Image: ", Line, 15)) == 0)
      {
	IsAux3Image = atoi(Line+15);
	SetAux3Image(IsAux3Image);
      }
      else if ( (std::strncmp("Is Aux4 Image: ", Line, 15)) == 0)
      {
	IsAux4Image = atoi(Line+15);
	SetAux4Image(IsAux4Image);
      }
      else if ( (std::strncmp("Is Aux5 Image: ", Line, 15)) == 0)
      {
	IsAux5Image = atoi(Line+15);
	SetAux5Image(IsAux5Image);
      }
      else if ( (std::strncmp("Is Aux6 Image: ", Line, 15)) == 0)
      {
	IsAux6Image = atoi(Line+15);
	SetAux6Image(IsAux6Image);
      }
      else if ( (std::strncmp("Is Aux7 Image: ", Line, 15)) == 0)
      {
	IsAux7Image = atoi(Line+15);
	SetAux7Image(IsAux7Image);
      }
      else if ( (std::strncmp("Is Aux8 Image: ", Line, 15)) == 0)
      {
	IsAux8Image = atoi(Line+15);
	SetAux8Image(IsAux8Image);
      }
      else if ( (std::strncmp("Data Directory: ", Line, 16)) == 0)
      {
	if (std::strlen(Line+16) != 0)
	  SetDataDirectory(Line+16);
	else
	  SetDataDirectory("");
      }
      else if ( (std::strncmp("AuxT1 Files: ", Line, 13)) == 0)
      {
	if (std::strlen(Line+13) != 0)
	  SetT1(Line+13);
	else
	  SetT1("");
      }
      else if ( (std::strncmp("AuxT2 Files: ", Line, 13)) == 0)
      {
	if (std::strlen(Line+13) != 0)
	  SetT2(Line+13);
	else
	  SetT2("");
      }
      else if ( (std::strncmp("AuxPD Files: ", Line, 13)) == 0)
      {
	if (std::strlen(Line+13) != 0)
	  SetPD(Line+13);
	else
	  SetPD("");
      }
      else if ( (std::strncmp("Aux1 Files: ", Line, 12)) == 0)
      {
	if (std::strlen(Line+12) != 0)
	  SetAux1(Line+12);
	else
	  SetAux1("");
      }
      else if ( (std::strncmp("Aux2 Files: ", Line, 12)) == 0)
      {
	if (std::strlen(Line+12) != 0)
	  SetAux2(Line+12);
	else
	  SetAux2("");
      }
      else if ( (std::strncmp("Aux3 Files: ", Line, 12)) == 0)
      {
	if (std::strlen(Line+12) != 0)
	  SetAux3(Line+12);
	else
	  SetAux3("");
      }
      else if ( (std::strncmp("Aux4 Files: ", Line, 12)) == 0)
      {
	if (std::strlen(Line+12) != 0)
	  SetAux4(Line+12);
	else
	  SetAux4("");
      }
      else if ( (std::strncmp("Aux5 Files: ", Line, 12)) == 0)
      {
	if (std::strlen(Line+12) != 0)
	  SetAux5(Line+12);
	else
	  SetAux5("");
      }
      else if ( (std::strncmp("Aux6 Files: ", Line, 12)) == 0)
      {
	if (std::strlen(Line+12) != 0)
	  SetAux6(Line+12);
	else
	  SetAux6("");
      }
      else if ( (std::strncmp("Aux7 Files: ", Line, 12)) == 0)
      {
	if (std::strlen(Line+12) != 0)
	  SetAux7(Line+12);
	else
	  SetAux7("");
      }
      else if ( (std::strncmp("Aux8 Files: ", Line, 12)) == 0)
      {
	if (std::strlen(Line+12) != 0)
	  SetAux8(Line+12);
	else
	  SetAux8("");
      }
      else if ( (std::strncmp("Aux1 Label: ", Line, 11)) == 0)
      {
	if (std::strlen(Line+11) != 0)
	  SetAux1Label(Line+11);
	else
	  SetAux1Label("");
      }
      else if ( (std::strncmp("Aux2 Label: ", Line, 11)) == 0)
      {
	if (std::strlen(Line+11) != 0)
	  SetAux2Label(Line+11);
	else
	  SetAux2Label("");
      }
      else if ( (std::strncmp("Aux3 Label: ", Line, 11)) == 0)
      {
	if (std::strlen(Line+11) != 0)
	  SetAux3Label(Line+11);
	else
	  SetAux3Label("");
      }
      else if ( (std::strncmp("Aux4 Label: ", Line, 11)) == 0)
      {
	if (std::strlen(Line+11) != 0)
	  SetAux4Label(Line+11);
	else
	  SetAux4Label("");
      }
      else if ( (std::strncmp("Aux5 Label: ", Line, 11)) == 0)
      {
	if (std::strlen(Line+11) != 0)
	  SetAux5Label(Line+11);
	else
	  SetAux5Label("");
      }
      else if ( (std::strncmp("Aux6 Label: ", Line, 11)) == 0)
      {
	if (std::strlen(Line+11) != 0)
	  SetAux6Label(Line+11);
	else
	  SetAux6Label("");
      }
      else if ( (std::strncmp("Aux7 Label: ", Line, 11)) == 0)
      {
	if (std::strlen(Line+11) != 0)
	  SetAux7Label(Line+11);
	else
	  SetAux7Label("");
      }
      else if ( (std::strncmp("Aux8 Label: ", Line, 11)) == 0)
      {
	if (std::strlen(Line+11) != 0)
	  SetAux8Label(Line+11);
	else
	  SetAux8Label("");
      }
      else if ( (std::strncmp("Atlas Space Image: ", Line, 19)) == 0)
      {
	AtlasSpaceImage = atoi(Line+19);
	SetAtlasSpaceImage(AtlasSpaceImage);
	if (AtlasSpaceImage == 1)
	{
	  BiasCorrectedImage = 0;
	  SkullStrippedImage = 0;
	}
      }
      else if ( (std::strncmp("Bias Corrected Image: ", Line, 22)) == 0)
      {
	BiasCorrectedImage = atoi(Line+22);
	SetBiasCorrectedImage(BiasCorrectedImage);
	if (BiasCorrectedImage == 1)
	{
	  AtlasSpaceImage = 0;
	  SkullStrippedImage = 0;
	}
      }
      else if ( (std::strncmp("Skull Stripped Image: ", Line, 22)) == 0)
      {
	SkullStrippedImage = atoi(Line+22);
	SetSkullStrippedImage(SkullStrippedImage);
	if (SkullStrippedImage == 1)
	{
	  AtlasSpaceImage = 0;
	  BiasCorrectedImage = 0;
	}
      }
      else if ( (std::strncmp("Rigid Transformation: ", Line, 22)) == 0)
      {
	RigidTransformation = atoi(Line+22);
	SetRigidTransformation(RigidTransformation);
	if (RigidTransformation == 1)
	{
	  AffineTransformation = 0;
	  BsplineTransformation = 0;
	}
      }
      else if ( (std::strncmp("Affine Transformation: ", Line, 23)) == 0)
      {
	AffineTransformation = atoi(Line+23);
	SetAffineTransformation(AffineTransformation);
	if (AffineTransformation == 1)
	{
	  BsplineTransformation = 0;
	  RigidTransformation = 0;
	}
      }
      else if ( (std::strncmp("Bspline Transformation: ", Line, 24)) == 0)
      {
	BsplineTransformation = atoi(Line+24);
	SetBsplineTransformation(BsplineTransformation);
	if (BsplineTransformation == 1)
	{
	  AffineTransformation = 0;
	  RigidTransformation = 0;
	}
      }
      else if ( (std::strncmp("AuxData: ", Line, 9)) == 0)
      {
	DataList.push_back(Line+9);
	nbData++;				
      }
      else if (Line[0] != '/' && Line[0] != '#' && (std::strcmp("",Line) != 0) && (std::strcmp(" ",Line) != 0)) 
      {
	std::cout << "unknown line:" << Line << std::endl;
      }
    }
    fclose(AuxComputationFile);
    if (nbData!=0)
    {
      SetNbData(nbData);
      AllocateAuxDataList();
      for(int i=0;i<nbData;i++)
	SetAuxDataList(DataList.at(i),i);
    }
  }
  else
  {
    std::cout<<"Error Opening File: "<<_FileName<<std::endl;
  }
}

// Load computation file
void AutoSegComputation::LoadComputationFile(const char *_FileName)
{
  FILE* ComputationFile;
  char Line[1536];
  int Length;
  // Computation Options
  int IsT2Image, IsPDImage;
  int nbData=0;
  std::vector<std::string> DataList;
  int ComputeVolume, ComputeCorticalThickness, Recompute, UseCondor;

  if ((ComputationFile = fopen(_FileName,"r")) != NULL) 
  {
    while ( (fgets(Line,1536,ComputationFile)) != NULL)
    {
      Length = std::strlen(Line);
      Line[Length-1] = '\0';

      if ( (std::strncmp("Process Data Directory: ", Line, 24)) == 0)
      {
	if ( std::strlen(Line+24) != 0) {
	  std::string directory (Line+24);
	  if (directory[directory.size() - 1] != '/') {
	    directory = directory + "/";
	  }
	  SetProcessDataDirectory(Line+24);
	} else {
	  SetProcessDataDirectory("");
	}
      }
      else if ( (std::strncmp("Is T1 Image: ", Line, strlen("Is T1 Image: "))) == 0)
      {
	if (atoi(Line+strlen("Is T1 Image: ")) == 0)
	{
	  std::cout << "Computation File issue: T1 image is not set, setting back to 1 (default)" << std::endl;
	} 
      }
      else if ( (std::strncmp("Is T2 Image: ", Line, 13)) == 0)
      {
	IsT2Image = atoi(Line+13);
	SetT2Image(IsT2Image);
      }
      else if ( (std::strncmp("Is PD Image: ", Line, 13)) == 0)
      {
	IsPDImage = atoi(Line+13);
	SetPDImage(IsPDImage);
      }
      else if( (std::strncmp("Data AutoSeg Directory: ", Line, 24)) == 0)
      {
	if (std::strlen(Line+24))
	  SetDataAutoSegDirectory(Line+24);
	else
	  SetDataAutoSegDirectory("");
      }
      else if ( (std::strncmp("Data Directory: ", Line, 16)) == 0)
      {
	if (std::strlen(Line+16) != 0)
	  SetDataDirectory(Line+16);
	else
	  SetDataDirectory("");
      }
      else if ( (std::strncmp("T1 Files: ", Line, 10)) == 0)
      {
	if (std::strlen(Line+10) != 0)
	  SetT1(Line+10);
	else
	  SetT1("");
      }
      else if ( (std::strncmp("T2 Files: ", Line, 10)) == 0)
      {
	if (std::strlen(Line+10) != 0)
	  SetT2(Line+10);
	else
	  SetT2("");
      }
      else if ( (std::strncmp("PD Files: ", Line, 10)) == 0)
      {
	if (std::strlen(Line+10) != 0)        
	  SetPD(Line+10);
	else
	  SetPD("");
      }
      else if ( (std::strncmp("Compute Volume: ", Line, 16)) == 0)
      {
	ComputeVolume = atoi(Line+16);
	SetComputeVolume(ComputeVolume);
      }
      else if ( (std::strncmp("Compute cortical thickness: ", Line, 28)) == 0)
      {
	ComputeCorticalThickness = atoi(Line+28);
	SetComputeCorticalThickness(ComputeCorticalThickness);
      }
      else if ( (std::strncmp("Recompute: ", Line, 11)) == 0)
      {
	Recompute = atoi(Line+11);
	SetRecompute(Recompute);
      }
      else if ( (std::strncmp("Use Condor: ", Line, 12)) == 0)
      {
	UseCondor = atoi(Line+12);
	SetUseCondor(UseCondor);
      }
      else if ( (std::strncmp("Data: ", Line, 6)) == 0)
      {
	DataList.push_back(Line+6);
	nbData++;
        SetMultiAtlasTargetFile(Line+6);
      }
      else if ( (std::strncmp("Compute Multi-modality Single-atlas Segmentation: ", Line, strlen("Compute Multi-modality Single-atlas Segmentation: "))) == 0)
      {
          SetMultiModalitySingleSegmentation(atoi(Line + strlen("Compute Multi-modality Single-atlas Segmentation: ")));
      }
      else if ( (std::strncmp("Compute Multi-modality Multi-atlas Segmentation: ", Line, strlen("Compute Multi-modality Multi-atlas Segmentation: "))) == 0)
      {
          SetMultiModalityMultiSegmentation(atoi(Line + strlen("Compute Multi-modality Multi-atlas Segmentation: ")));
      }
      else if ( (std::strncmp("Conduct Atlas-Atlas registration: ", Line, 34)) == 0)
      {
          SetMultiAtlasAtlasRegistration(atoi(Line + 34));
      }
      else if ( (std::strncmp("Recalculate Atlas-Target Energy: ", Line, 33)) == 0)
      {
          SetRecalculateAtlasTargetMultiAtlasEnergy(atoi(Line + 33));
      }
      else if ( (std::strncmp("Recalculate Atlas-Atlas Energy: ", Line, 32)) == 0)
      {
          SetRecalculateAtlasAtlasMultiAtlasEnergy(atoi(Line + 32));
      }
      else if ( (std::strncmp("Conduct Atlas-Atlas Registration: ", Line, 34)) == 0)
      {
          SetMultiAtlasAtlasRegistration(atoi(Line + 34));
      }
      else if ( (std::strncmp("Compute Multi-atlas Segmentation: ", Line, 34)) == 0)
      {
	  SetMultiAtlasSegmentation(atoi(Line+34)); 
      }
      else if ( (std::strncmp("Compute Single-atlas Segmentation: ", Line, 35)) == 0)
      {
	  SetSingleAtlasSegmentation(atoi(Line+35)); 
      }
      else if ( (std::strncmp("Randomize Subject Order: ", Line, 25)) == 0)
      {
	  SetRandomizeSubjects(atoi(Line+25)); 
      }
      else if ( (std::strncmp("Multi-atlas directory: ", Line, 23)) == 0)
      { // for sake of backward compatibility
	if (std::strlen(Line+23) != 0)
	  SetMultiAtlasDirectory(Line+23); 
	else
	  SetMultiAtlasDirectory("");
      }
      else if (Line[0] != '/' && Line[0] != '#' && (std::strcmp("",Line) != 0) && (std::strcmp(" ",Line) != 0)) 
      {
	std::cout << "unknown line:" << Line << std::endl;
      }
    }
    fclose(ComputationFile);
    std::cout << "number of data: " << nbData << std::endl;
    if (nbData!=0)
    {
      SetNbData(nbData);
      AllocateDataList();
      for(int i=0;i<nbData;i++) {
	SetDataList(DataList[i].c_str(),i,0);
        //std::cout << " data file: " << DataList[i] << std::endl;
      }
    }
  }
  else
    std::cout<<"Error Opening File: "<<_FileName<<std::endl;
}

bool AutoSegComputation::SetMultiAtlasDirectory(const char *_MultiAtlasDirectory)
// ToDo: THIS Procedure is NOT Cross-platform, likely won't work on windows
{

  std::cout << "setting multiAtlas Directory " << _MultiAtlasDirectory << std::endl;

    int c ='/';
    const char * firstSlash = std::strrchr(_MultiAtlasDirectory, c);
    if (!firstSlash) {
      return false;
    }
    int l = std::strlen(firstSlash);
    if(l == 1) {
        std::strcpy(m_MultiAtlasDirectory,_MultiAtlasDirectory);
    }
    else {
        std::strcpy(m_MultiAtlasDirectory,_MultiAtlasDirectory);
        std::strcat(m_MultiAtlasDirectory,"/");
    }

    std::string tmp;
    //tmp = fl_dir_chooser("Set the Multi Atlas Directory", NULL);
    tmp = _MultiAtlasDirectory;
    char MultiAtlasDirectory[256], MultiAtlasDirectoryDisp[256];
    char WarpedMultiAtlasAtlasImageDirectory[256];
    char MultiAtlasAtlasDisplacementDirectory[256];
    bool needWarpAtlasAtlas = 0, needAtlasAtlasDisplacement = 0;
  
  //  MultiAtlasDirectory = fl_dir_chooser("Set the Multi Atlas Directory", NULL);
//    tmp = MultiAtlasDirectory;
    strcpy(MultiAtlasDirectoryDisp, tmp.c_str());
    strcpy(MultiAtlasDirectory, tmp.c_str());
    strcat(MultiAtlasDirectory, "atlas_image/");
    strncpy(WarpedMultiAtlasAtlasImageDirectory, MultiAtlasDirectory, tmp.length() - 12);
    strcat(WarpedMultiAtlasAtlasImageDirectory, "warped-atlas-atlas-images/");
    strncpy(MultiAtlasAtlasDisplacementDirectory, MultiAtlasDirectory, tmp.length() - 12) ;
    strcat(MultiAtlasAtlasDisplacementDirectory, "displacement_field_atlas_to_atlas/");
    
    DIR *dir;
    struct dirent *ent;

    int t1num = 0, labelnum = 0, t2num = 0, parcelnum = 0;
    if ((dir = opendir (MultiAtlasDirectory)) != NULL) {
        std::string filename;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                if (!filename.find("atlas")) {
                    if (filename.find("t1w") != std::string::npos) {
                        t1num++;
                    }
                    else if (filename.find("t2w") != std::string::npos) {
                        t2num++;
                    }
                    else {
                        t1num++;
                    }
                }
                if (!filename.find("label"))
                    labelnum++;
                if (!filename.find("parcellation"))
                    parcelnum++;
            }
        }
        SetNbAtlas(t1num);
        SetNb2ndAtlas(t2num);
        SetNbAtlasLabel(labelnum);
        SetNbAtlasParcellation(parcelnum);
    }
    closedir (dir);

    std::cout << "Multiatlas numbers: " << t1num << ", " << t2num << " ," << labelnum << ", " << parcelnum << std::endl;

    t1num = 0; labelnum = 0; t2num = 0; parcelnum = 0;
    if ((dir = opendir (MultiAtlasDirectory)) != NULL) {
        std::string filename;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                if (!filename.find("atlas")) {
                    if (filename.find("t1w") != std::string::npos) {
                        SetAtlasList(filename.c_str(), t1num);
                        t1num++;
                    }
                    else if (filename.find("t2w") != std::string::npos) {
                        Set2ndAtlasList(filename.c_str(), t2num);
                        t2num++;
                    }
                    else {
                        SetAtlasList(filename.c_str(), t1num);
                        t1num++;
                    }
                }
                if (!filename.find("label")) {
                    SetAtlasLabelList(filename.c_str(), labelnum);
                    labelnum++;
                }
                if (!filename.find("parcellation")) {
                    SetAtlasParcellationList(filename.c_str(), parcelnum);
                    parcelnum++;
                }
            }
        }
    }
    closedir (dir);
    //std::cout << "Multiatlas numbers: " << t1num << ", " << t2num << " ," << labelnum << ", " << parcelnum << std::endl;
   
    int numberOfWarpedAtlasAtlasImage = 0;
    if ((dir = opendir (WarpedMultiAtlasAtlasImageDirectory)) != NULL) {
        std::string filename;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                numberOfWarpedAtlasAtlasImage++;
            }
        }
    }
    closedir (dir);

    if( numberOfWarpedAtlasAtlasImage < (GetNbAtlas() * (GetNbAtlas() - 1)) ) {
        needWarpAtlasAtlas = 1;
    }

    int numberOfAtlasAtlasDisplacement = 0;
    if ((dir = opendir (MultiAtlasAtlasDisplacementDirectory)) != NULL) {
        std::string filename;
        while ((ent = readdir (dir)) != NULL) {
            filename = ent->d_name;
            if(filename.at(0) == '.')    // skip . and ..
                continue;
            else{
                numberOfAtlasAtlasDisplacement++;
            }
        }
    }
    closedir (dir);
    if( numberOfAtlasAtlasDisplacement < (GetNbAtlas() * (GetNbAtlas() - 1)) ) {
        needAtlasAtlasDisplacement = 1;
    }

    if(needAtlasAtlasDisplacement || needWarpAtlasAtlas)
        return 1;
    else
        return 0;
}


// Load parameter file
// Mode = file: Read the total file
// Mode = advancedParameters: Read only the advanced parameters (tissue segmentation, warping parameters and N4 ITK Bias Field Correction parameters)
// Mode = tissueSeg: Read only the tissue segmentation parameters
// Mode = warping: Read only the warping parameters
// Mode = N4biasFieldCorrection: Read only the N4 ITK Bias Field Correction parameters
bool AutoSegComputation::LoadParameterFile(const char *_FileName, enum Mode mode)
{
  FILE* ParameterFile;
  char Line[2048]; 
  int Length;
    // Tissue Segmentation
  int FilterIterations, MaxBiasDegree, Loop;
  float FilterTimeStep, Prior1, Prior2, Prior3, Prior4, Prior5, Prior6, Prior7, Prior8, Prior9,FluidAtlasWarpMaxStep;
  int BSplineAtlasWarp;
  float BSplineAtlasWarpGridX, BSplineAtlasWarpGridY, BSplineAtlasWarpGridZ;
  int FluidAtlasWarp, FluidAtlasFATW, FluidAtlasAffine, FluidAtlasWarpIterations, LoopIteration;
  float NeosegPriorThreshold, NeosegParzenKernel, NeosegMahalanobisThreshold;
  

    // Rigid Registration
  int RigidRegistration, IsROIAtlasGridTemplate;
  int GridTemplateSizeX, GridTemplateSizeY, GridTemplateSizeZ;
  float GridTemplateSpacingX, GridTemplateSpacingY, GridTemplateSpacingZ;
  int InitRegUseT1InitTransform;
    // Atlas Warping
  float Alpha, Beta, Gamma, MaxPerturbation, NumBasis,DeformationFieldSmoothingSigma;
  int Scale4NbIterations, Scale2NbIterations, Scale1NbIterations,PyramidLevels;
  std::string RegistrationFilterType,MovingShrinkFactors,FixedShrinkFactors,IterationCountPyramidLevels;
  // - ANTS
  std::string ANTSIterations, ANTSRegistrationFilterType, ANTSTransformationStep;
  float ANTSCCWeight, ANTSCCRegionRadius, ANTSMIWeight, ANTSMIBins, ANTSMSQWeight, ANTSGaussianSigma;
  bool ANTSGaussianSmoothing;    
    // Skull Stripping
  int DeleteVessels;
    // Regional histogram
  float PointSpacing;
    // N4 ITK Bias Field Correction
  std::string NbOfIterations,BSplineGridResolutions,HistogramSharpening;
  int N4ITKBiasFieldCorrection,ShrinkFactor,BSplineOrder;
  float ConvergenceThreshold,BSplineBeta,BSplineAlpha,SplineDistance,SlicerVersion;
    // Reorientation
  int Reorientation;
  std::string InputDataOrientation, OutputDataOrientation;
  int value;

  bool IsParameterFileLoaded=false;
  bool unknownLineFile= false;
  bool unknownLineWarping = false;
  bool unknownLineTissueSeg = false;

  if (mode == file)
  {
    // INIT / default setting for backward compatibility
    
    // Init for EMS loop
    SetLoop(0);
    // Init for N4
    SetN4ITKBiasFieldCorrection(0);
    // Init for reorientation
    SetReorientation(0);
    // Init for rigid registration
    SetRegistrationInitialization("useCenterOfHeadAlign");
    SetInitRegUseT1InitTransform(0);
    // Init for atlas warping
    SetANTSWarpingMethod(1);//We initialize to ANTS by default
    SetBRAINSDemonWarpMethod(0);
    SetCoarseToFineWarpingMethod(0);
    SetClassicWarpingMethod(0);
  }

  if ((ParameterFile = fopen(_FileName,"r")) != NULL) 
  {
    IsParameterFileLoaded = true;
    while ( (fgets(Line,512,ParameterFile)) != NULL)
    {
      Length = std::strlen(Line);
      Line[Length-1] = '\0';
      
      if (mode == file)
      {
	if ( (std::strncmp("Common Coordinate Image: ", Line, 25)) == 0)
	{
	  if (std::strlen(Line+25) != 0)
	    SetCommonCoordinateImage(Line+25);
	  else
	    SetCommonCoordinateImage("");
	}
	else if ( (std::strncmp("Common Coordinate Image Type: ", Line, 30)) == 0)
	{
	  SetCommonCoordinateImageType(Line+30);
	  if (std::strcmp(Line+30, "T1") == 0)
	    SetCommonCoordinateImageType("T1");
	  else
	    SetCommonCoordinateImageType("T2");
	}
	else if ( (std::strncmp("Tissue Segmentation Atlas Directory: ", Line, 37)) == 0)
	{
	  if (std::strlen(Line+37) != 0)
	    SetTissueSegmentationAtlasDirectory(Line+37); 
	  else
	    SetTissueSegmentationAtlasDirectory("");
	}
	else if ( (std::strncmp("Tissue Segmentation Atlas Type: ", Line, 32)) == 0)
	{
	  SetTissueSegmentationAtlasType(Line+32);
	  if (std::strcmp(Line+32, "T1") == 0)
	    SetTissueSegmentationAtlasType("T1");
	  else
	    SetTissueSegmentationAtlasType("T2");
	}
	else if ( (std::strncmp("ROI Atlas File: ", Line, 16)) == 0)
	{
	  if (std::strlen(Line+16) != 0)
	    SetROIAtlasFile(Line+16);
	  else
	    SetROIAtlasFile("");
	}
        else if ( (std::strncmp("ROI Second Atlas File: ", Line, 23)) == 0)
	{
	  if (std::strlen(Line+23) != 0) {
	    SetROIT2AtlasFile(Line+23);
            SetROIT2Atlas(1);
          }
	  else{
	    SetROIT2AtlasFile("");
            SetROIT2Atlas(0);
          }
	}
	else if ( (std::strncmp("Subcortical Structure Segmentation: ", Line, 36)) == 0)
	{
	  if (atoi(Line+36) == 1)
	    SetSubcorticalStructureSegmentation(1);
	  else
	    SetSubcorticalStructureSegmentation(0);
	}
	else if ( (std::strncmp("Amygdala Left: ", Line, 15)) == 0)
	{
	  if (std::strlen(Line+15) != 0)
	    SetAmygdalaLeft(Line+15);
	  else
	    SetAmygdalaLeft("");
	}
	else if ( (std::strncmp("Amygdala Right: ", Line, 16)) == 0)
	{
	  if (std::strlen(Line+16) != 0)
	    SetAmygdalaRight(Line+16);
	  else
	    SetAmygdalaRight("");
	}     
	else if ( (std::strncmp("Caudate Left: ", Line, 14)) == 0)
	{
	  if (std::strlen(Line+14) != 0)
	    SetCaudateLeft(Line+14);
	  else
	    SetCaudateLeft("");
	}     
	else if ( (std::strncmp("Caudate Right: ", Line, 15)) == 0)
	{
	  if (std::strlen(Line+15) != 0)
	    SetCaudateRight(Line+15);
	  else
	    SetCaudateRight("");
	}     
	else if ( (std::strncmp("Hippocampus Left: ", Line, 18)) == 0)
	{
	  if (std::strlen(Line+18) != 0)
	    SetHippocampusLeft(Line+18);
	  else
	    SetHippocampusLeft("");
	}     
	else if ( (std::strncmp("Hippocampus Right: ", Line, 19)) == 0)
	{
	  if (std::strlen(Line+19) != 0)
	    SetHippocampusRight(Line+19);
	  else
	    SetHippocampusRight(""); 
	}     
	else if ( (std::strncmp("Pallidus Left: ", Line, 15)) == 0)
	{
	  if (std::strlen(Line+15) != 0)
	    SetPallidusLeft(Line+15);
	  else
	    SetPallidusLeft("");
	}     
	else if ( (std::strncmp("Pallidus Right: ", Line, 16)) == 0)
	{
	  if (std::strlen(Line+16) != 0)
	    SetPallidusRight(Line+16);
	  else
	    SetPallidusRight("");
	}     
	else if ( (std::strncmp("Putamen Left: ", Line, 14)) == 0)
	{
	  if (std::strlen(Line+14) != 0)
	    SetPutamenLeft(Line+14);
	  else
	    SetPutamenLeft("");
	}     
	else if ( (std::strncmp("Putamen Right: ", Line, 15)) == 0)
	{
	  if (std::strlen(Line+15) != 0)
	    SetPutamenRight(Line+15);
	  else
	    SetPutamenRight("");
	}     
	else if ( (std::strncmp("Lateral Ventricle Left: ", Line, 24)) == 0)
	{
	  if (std::strlen(Line+24) != 0)
	    SetLateralVentricleLeft(Line+24);
	  else
	    SetLateralVentricleLeft("");
	}
	else if ( (std::strncmp("Lateral Ventricle Right: ", Line, 25)) == 0)
	{
	  if (std::strlen(Line+25) != 0)
	    SetLateralVentricleRight(Line+25);
	  else
	    SetLateralVentricleRight("");
	}
	else if ( (std::strncmp("Generic ROI Segmentation: ", Line, 26)) == 0)
	{
	  if (atoi(Line+26) == 1)
	    SetGenericROISegmentation(1); 
	  else
	    SetGenericROISegmentation(0); 
	}
	else if ( (std::strncmp("ROI File 1: ", Line, 12)) == 0)
	{
	  if (std::strlen(Line+12) != 0)
	    SetROIFile1(Line+12); 
	  else
	    SetROIFile1(""); 
	}
	else if ( (std::strncmp("ROI File 2: ", Line, 12)) == 0)
	{
	  if (std::strlen(Line+12) != 0)
	    SetROIFile2(Line+12);
	  else
	    SetROIFile2("");
	}
	else if ( (std::strncmp("ROI File 3: ", Line, 12)) == 0)
	{
	  if (std::strlen(Line+12) != 0)
	    SetROIFile3(Line+12);
	  else
	    SetROIFile3("");
	}
	else if ( (std::strncmp("ROI File 4: ", Line, 12)) == 0)
	{
	  if (std::strlen(Line+12) != 0)
	    SetROIFile4(Line+12);
	  else
	    SetROIFile4("");
	}
	else if ( (std::strncmp("ROI File 5: ", Line, 12)) == 0)
	{
	  if (std::strlen(Line+12) != 0)
	    SetROIFile5(Line+12);
	  else
	    SetROIFile5("");
	}
	else if ( (std::strncmp("Parcellation Map Segmentation: ", Line, 31)) == 0)
	{
	  if (atoi(Line+31) == 1)
	    SetParcellationMapSegmentation(1);
	  else
	    SetParcellationMapSegmentation(0);
	}
	else if ( (std::strncmp("Tissue Map: ", Line, 12)) == 0)
	{
	  SetSoftTissueMap(Line+12);
	  if (std::strcmp(Line+12, "Soft") == 0)
	    SetSoftTissueMap("Soft");
	  else
	    SetSoftTissueMap("Hard");
	}
	else if ( (std::strncmp("Parcellation File 1: ", Line, 21)) == 0)
	{
	  if (std::strlen(Line+21) != 0)
	    SetParcellationFile1(Line+21);
	  else
	    SetParcellationFile1("");
	}
	else if ( (std::strncmp("Parcellation File 2: ", Line, 21)) == 0)
	{
	  if (std::strlen(Line+21) != 0)
	    SetParcellationFile2(Line+21);
	  else
	    SetParcellationFile2("");
	}
	else if ( (std::strncmp("Parcellation File 3: ", Line, 21)) == 0)
	{
	  if (std::strlen(Line+21) != 0)
	    SetParcellationFile3(Line+21);
	  else
	    SetParcellationFile3("");
	}
	else if ((std::strncmp("Reorientation: ", Line, 15)) == 0)
	{
	  Reorientation = (atoi(Line+15));
	  SetReorientation(Reorientation);
	}
	else if ( (std::strncmp("Input Orientation: ", Line, 19)) == 0)
	{
	  if (std::strlen(Line+19) != 0)
	    SetInputDataOrientation(Line+19);
	  else
	    SetInputDataOrientation("");
	}
	else if ( (std::strncmp("Output Orientation: ", Line, 20)) == 0)
	{
	  if (std::strlen(Line+20) != 0)
	    SetOutputDataOrientation(Line+20);
	  else
	    SetOutputDataOrientation("");
	}
	else if ((std::strncmp("Rigid Registration: ", Line, 20)) == 0)
	{
	  RigidRegistration = (atoi(Line+20));
	  SetRigidRegistration(RigidRegistration);
	}
	else if ((std::strncmp("Is ROIAtlasGridTemplate: ", Line, 25)) == 0)
	{
	  IsROIAtlasGridTemplate = atoi(Line+25);
	  SetROIAtlasGridTemplate(IsROIAtlasGridTemplate);
	}
	else if ((std::strncmp("GridTemplate SizeX: ", Line, 20)) == 0)
	{
	  GridTemplateSizeX = atoi(Line+20);
	  SetGridTemplateSizeX(GridTemplateSizeX);
	}
	else if ((std::strncmp("GridTemplate SizeY: ", Line, 20)) == 0)
	{
	  GridTemplateSizeY = atoi(Line+20);
	  SetGridTemplateSizeY(GridTemplateSizeY);
	}
	else if ((std::strncmp("GridTemplate SizeZ: ", Line, 20)) == 0)
	{
	  GridTemplateSizeZ = atoi(Line+20);
	  SetGridTemplateSizeZ(GridTemplateSizeZ);
	}
	else if ((std::strncmp("GridTemplate SpacingX: ", Line, 23)) == 0)
	{
	  GridTemplateSpacingX = atof(Line+23);
	  SetGridTemplateSpacingX(GridTemplateSpacingX);
	}
	else if ((std::strncmp("GridTemplate SpacingY: ", Line, 23)) == 0)
	{
	  GridTemplateSpacingY = atof(Line+23);
	  SetGridTemplateSpacingY(GridTemplateSpacingY);
	}
	else if ((std::strncmp("GridTemplate SpacingZ: ", Line, 23)) == 0)
	{
	  GridTemplateSpacingZ = atof(Line+23);
	  SetGridTemplateSpacingZ(GridTemplateSpacingZ);
	}
	else if ((std::strncmp("Registration Initialization: ", Line, 29)) == 0)
	  {
	    if (std::strlen(Line+29) != 0)
	      SetRegistrationInitialization(Line+29);
	    else
	      SetRegistrationInitialization("");
	  }
	else if ((std::strncmp("Use T1 initial transform: ", Line, 26)) == 0)
	  {
	    InitRegUseT1InitTransform = atoi(Line+26);
	    SetInitRegUseT1InitTransform(InitRegUseT1InitTransform);
	  }	
	else if ((std::strncmp("Delete Vessels: ", Line, 16)) == 0)
	{
	  DeleteVessels = (atoi(Line+16));
	  SetDeleteVessels(DeleteVessels);
	}
	else if ((std::strncmp("Intensity Rescaling: ", Line, 21)) == 0)
	{
	  if (std::strcmp(Line+21,"Histogram quantile") == 0)
	    SetIntensityRescalingMethod(1);
	  else
	    SetIntensityRescalingMethod(2);
	}
	else if ( (std::strncmp("Quantiles: ", Line, 11)) == 0)
	  SetQuantiles(Line+11);	
	else if ( (std::strncmp("Point Spacing: ", Line, 15)) == 0)
	{
	  PointSpacing = atof(Line+15);
	  SetPointSpacing(PointSpacing);
	}
	else if (Line[0] != '/' && Line[0] != '#' && (std::strcmp("",Line) != 0) && (std::strcmp(" ",Line) != 0)) 
	{
	  unknownLineFile = true;
	}
      }

      /// tissue seg params *****************************************

      if (mode == tissueSeg ||mode == advancedParameters||mode == file)
      {
	if ((std::strncmp("EM Software: ", Line, 13)) == 0)
	  SetEMSoftware(Line+13);
	else if ( (std::strncmp("Filter Iterations: ", Line, 19)) == 0)
	{
	  FilterIterations = atoi(Line+19);
	  SetFilterIterations(FilterIterations);
	}
	else if ( (std::strncmp("Filter TimeStep: ", Line, 17)) == 0)
	{
	  FilterTimeStep = atof(Line+17);
	  SetFilterTimeStep(FilterTimeStep);	
	}
	else if ( (std::strncmp("Filter Method: ", Line, 15)) == 0)
	  SetFilterMethod(Line+15);
	else if ( (std::strncmp("Max Bias Degree: ", Line, 17)) == 0)
	{
	  MaxBiasDegree = atoi(Line+17);
	  SetMaxBiasDegree(MaxBiasDegree);
	}
	else if ( (std::strncmp("Initial Distribution Estimator: ", Line, 32)) == 0)
	  SetInitialDistributionEstimator(Line+32);
	else if ( (std::strncmp("Prior 1: ", Line, 9)) == 0)
	{
	  Prior1 = atof(Line+9);
	  SetPrior1(Prior1);	
	}
	else if ( (std::strncmp("Prior 2: ", Line, 9)) == 0)
	{
	  Prior2 = atof(Line+9);
	  SetPrior2(Prior2);
	}
	else if ( (std::strncmp("Prior 3: ", Line, 9)) == 0)
	{
	  Prior3 = atof(Line+9);
	  SetPrior3(Prior3);
	}
	else if ( (std::strncmp("Prior 4: ", Line, 9)) == 0)
	{
	  Prior4 = atof(Line+9);
	  SetPrior4(Prior4);
	}
	else if ( (std::strncmp("Prior 5: ", Line, 9)) == 0)
	{
	  Prior5 = atof(Line+9);
	  SetPrior5(Prior5);
	}
        else if ( (std::strncmp("Prior 6: ", Line, 9)) == 0)
	{
	  Prior6 = atof(Line+9);
	  SetPrior6(Prior6);
	}
        else if ( (std::strncmp("Prior 7: ", Line, 9)) == 0)
	{
	  Prior7 = atof(Line+9);
	  SetPrior7(Prior7);
	}
        else if ( (std::strncmp("Prior 8: ", Line, 9)) == 0)
	{
	  Prior8 = atof(Line+9);
	  SetPrior8(Prior8);
	}
        else if ( (std::strncmp("Prior 9: ", Line, 9)) == 0)
	{
	  Prior9 = atof(Line+9);
	  SetPrior9(Prior9);
	}
	else if ( (std::strncmp("BSpline Atlas Warp: ", Line, 20)) == 0)
	{
	  BSplineAtlasWarp = atoi(Line+20);
	  SetBSplineAtlasWarp(BSplineAtlasWarp);
	}
	else if ( (std::strncmp("BSpline Atlas Warp Grid X: ", Line, 27)) == 0)
	{
	  BSplineAtlasWarpGridX = atof(Line+27);
	  SetBSplineAtlasWarpGridX(BSplineAtlasWarpGridX);	
	}	
	else if ( (std::strncmp("BSpline Atlas Warp Grid Y: ", Line, 27)) == 0)
	{
	  BSplineAtlasWarpGridY = atof(Line+27);
	  SetBSplineAtlasWarpGridY(BSplineAtlasWarpGridY);	
	}	
	else if ( (std::strncmp("BSpline Atlas Warp Grid Z: ", Line, 27)) == 0)
	{
	  BSplineAtlasWarpGridZ = atof(Line+27);
	  SetBSplineAtlasWarpGridZ(BSplineAtlasWarpGridZ);	
	}	
	else if ( (std::strncmp("Fluid Atlas Warp: ", Line, 18)) == 0)
	{
	  FluidAtlasWarp = atoi(Line+18);
	  if (FluidAtlasWarp == 1)
	  {
	    SetFluidAtlasWarp(1);
	    SetFluidAtlasAffine(0);
	    SetFluidAtlasFATW(0);
	    SetANTSAtlasABC(0);
	  }
	  else
	    SetFluidAtlasWarp(0);
	}
	else if ( (std::strncmp("Fluid Atlas Affine: ", Line, 20)) == 0)
	{
	  FluidAtlasAffine = atoi(Line+20);
	  if (FluidAtlasAffine == 1)
	  {
	    SetFluidAtlasAffine(1);
	    SetFluidAtlasWarp(0);
	    SetFluidAtlasFATW(0);
	    SetANTSAtlasABC(0);
	  }
	  else
	    SetFluidAtlasAffine(0);
	}
	else if ( (std::strncmp("Fluid Atlas FATW: ", Line, 18)) == 0)
	{
	  FluidAtlasFATW = atoi(Line+18);
	  if (FluidAtlasFATW == 1)
	  {
	    SetFluidAtlasFATW(1);
	    SetFluidAtlasWarp(0);
	    SetFluidAtlasAffine(0);
	    SetANTSAtlasABC(0);
	  }
	  else
	    SetFluidAtlasFATW(0);
	}
        else if ( (std::strncmp("ANTS Warp for ABC: ", Line, 19)) == 0)
	{
	  if (atoi(Line+19) == 1)
	  {
	    SetFluidAtlasFATW(0);
	    SetFluidAtlasWarp(0);
	    SetFluidAtlasAffine(0);
	    SetANTSAtlasABC(1);
	  }
	  else
	    SetANTSAtlasABC(0);
	}
	else if ( (std::strncmp("Fluid Atlas Warp Iterations: ", Line, 29)) == 0)
	{
	  FluidAtlasWarpIterations = atoi(Line+29);
	  SetFluidAtlasWarpIterations(FluidAtlasWarpIterations);	
	}
	else if ( (std::strncmp("Fluid Atlas Warp Max Step: ", Line, 27)) == 0)
	{
	  FluidAtlasWarpMaxStep = atof(Line+27);
	  SetFluidAtlasWarpMaxStep(FluidAtlasWarpMaxStep);
	}
	else if ( (std::strncmp("Atlas Linear Mapping: ", Line, 22)) == 0)
	  SetAtlasLinearMapping(Line+22);
	else if ( (std::strncmp("Image Linear Mapping: ", Line, 22)) == 0)
	  SetImageLinearMapping(Line+22);
	else if ( (std::strncmp("Prior Threshold: ", Line, 17)) == 0)
	{
	  NeosegPriorThreshold = atof(Line+17);
	  SetNeosegPriorThreshold(NeosegPriorThreshold);	
	}	
	else if ( (std::strncmp("Parzen Kernel: ", Line, 15)) == 0)
	{
	  NeosegParzenKernel = atof(Line+15);
	  SetNeosegParzenKernel(NeosegParzenKernel);	
	}	
	else if ( (std::strncmp("Mahalanobis Threshold: ", Line, 23)) == 0)
	{
	  NeosegMahalanobisThreshold = atof(Line+23);
	  SetNeosegMahalanobisThreshold(NeosegMahalanobisThreshold);	
	}	
	else if ((std::strncmp("Loop: ", Line, 6)) == 0)
	{
	  Loop= atoi(Line+6);
	  if (mode==tissueSeg)
	    SetLoop(1);
	  else
	  {
	    if (Loop == 1)
	      SetLoop(1);
	    else
	      SetLoop(0);
	  }
	}
	else if ( (std::strncmp("Atlas Loop: ", Line, 12)) == 0)
	{
	  if (std::strlen(Line+12) != 0)
	    SetAtlasLoop(Line+12);
	  else
	    SetAtlasLoop("");
	}
	else if ( (std::strncmp("Loop - Number of iterations: ", Line, 29)) == 0)
	{
	  LoopIteration = atoi(Line+29);
	  SetLoopIteration(LoopIteration);
	}
	else if (Line[0] != '/' && Line[0] != '#' && (std::strcmp("",Line) != 0) && (std::strcmp(" ",Line) != 0)) 
	{
	  unknownLineTissueSeg = true;
	}
      }

      /// warping params *****************************************

      if (mode == warping||mode == advancedParameters||mode == file)
      {
	if ( (std::strncmp("Warping Method: ", Line, 16)) == 0)
	{	
	  if (std::strcmp(Line+16, "Classic") == 0)
	  {
	    SetClassicWarpingMethod(1);
	    SetCoarseToFineWarpingMethod(0);
	    SetBRAINSDemonWarpMethod(0);
	    SetANTSWarpingMethod(0);
	  }
	  else if (std::strcmp(Line+16, "Coarse-to-fine") == 0)
	  {
	    SetClassicWarpingMethod(0);
	    SetCoarseToFineWarpingMethod(1);
	    SetBRAINSDemonWarpMethod(0);
	    SetANTSWarpingMethod(0);
	  }
	  else if (std::strcmp(Line+16, "BRAINSDemonWarp") == 0)
	  {
	    SetClassicWarpingMethod(0);
	    SetCoarseToFineWarpingMethod(0);
	    SetBRAINSDemonWarpMethod(1);
	    SetANTSWarpingMethod(0);
	  }
	  else if (std::strcmp(Line+16, "ANTS") == 0)
	  {
	    SetClassicWarpingMethod(0);
	    SetCoarseToFineWarpingMethod(0);
	    SetBRAINSDemonWarpMethod(0);
	    SetANTSWarpingMethod(1);
	  }	  
	  else
	    std::cerr<<"Error while reading parameter file: warping method incorrect!"<<std::endl;
	}
	else if ( (std::strncmp("Alpha: ", Line, 7)) == 0)
	{
	  Alpha = atof(Line+7);
	  SetAlpha(Alpha);
	}
	else if ( (std::strncmp("Beta: ", Line, 6)) == 0)
	{
	  Beta = atof(Line+6);
	  SetBeta(Beta);	
	}
	else if ( (std::strncmp("Gamma: ", Line, 7)) == 0)
	{
	  Gamma = atof(Line+7);
	  SetGamma(Gamma);
	}
	else if ( (std::strncmp("Max Perturbation: ", Line, 18)) == 0)
	{
	  MaxPerturbation = atof(Line+18);
	  SetMaxPerturbation(MaxPerturbation);	
	}
	else if ( (std::strncmp("Scale 4 - Number Of Iterations: ", Line, 32)) == 0)
	{
	  Scale4NbIterations = atoi(Line+32);
	  SetScale4NbIterations(Scale4NbIterations);	
	}
	else if ( (std::strncmp("Scale 2 - Number Of Iterations: ", Line, 32)) == 0)
	{
	  Scale2NbIterations = atoi(Line+32);
	  SetScale2NbIterations(Scale2NbIterations);	
	}
	else if ( (std::strncmp("Scale 1 - Number Of Iterations: ", Line, 32)) == 0)
	{
	  Scale1NbIterations = atoi(Line+32);
	  SetScale1NbIterations(Scale1NbIterations);
	}
	else if ( (std::strncmp("Registration Filter Type: ", Line, 26)) == 0)
	{
	  RegistrationFilterType =Line+26;	
	  if (RegistrationFilterType=="Demons"){
	    SetRegistrationFilterType("Demons");
	  }
	  else if(RegistrationFilterType=="FastSymmetricForces"){
	    SetRegistrationFilterType("FastSymmetricForces");
	  }
	  else if(RegistrationFilterType=="Diffeomorphic"){
	    SetRegistrationFilterType("Diffeomorphic");
	  }
	  else if(RegistrationFilterType=="LogDemons"){
	    SetRegistrationFilterType("LogDemons");
	  }
	  else if(RegistrationFilterType=="SymmetricLogDemons"){
	    SetRegistrationFilterType("SymmetricLogDemons");
	  }
	}
	else if ( (std::strncmp("Deformation Field Smoothing Sigma: ", Line, 35)) == 0)
	{
	  DeformationFieldSmoothingSigma = atof(Line+35);
	  SetDeformationFieldSmoothingSigma(DeformationFieldSmoothingSigma);
	}
	else if ( (std::strncmp("Pyramid Levels: ", Line, 16)) == 0)
	{
	  PyramidLevels = atoi(Line+16);
	  SetPyramidLevels(PyramidLevels);
	}
	else if ( (std::strncmp("Moving Shrink Factors: ", Line, 23)) == 0)
	{
	  MovingShrinkFactors =Line+23;
	  SetMovingShrinkFactors(MovingShrinkFactors.c_str());
	}
	else if ( (std::strncmp("Fixed Shrink Factors: ", Line, 22)) == 0)
	{
	  FixedShrinkFactors =Line+22;
	  SetFixedShrinkFactors(FixedShrinkFactors.c_str());
	}
	else if ( (std::strncmp("Iteration Count Pyramid Levels: ", Line, 32)) == 0)
	{
	  IterationCountPyramidLevels =Line+32;
	  SetIterationCountPyramidLevels(IterationCountPyramidLevels.c_str());
	}
	  // consistency with the first version of the tool
	else if ( (std::strncmp("Number Of Iterations: ", Line, 22)) == 0)
	{
	  Scale1NbIterations = atoi(Line+22);
	  SetScale1NbIterations(Scale1NbIterations);
	}
	else if ( (std::strncmp("NumBasis: ", Line, 10)) == 0)
	{
	  NumBasis = atof(Line+10);
	  SetNumBasis(NumBasis);
	}
	else if ( (std::strncmp("ANTS Iterations: ", Line, 17)) == 0)
	{
	  ANTSIterations = Line+17;
	  SetANTSIterations(ANTSIterations.c_str());	
	}
	else if ( (std::strncmp("ANTS CC weight: ", Line, 16)) == 0)
	{
	  ANTSCCWeight = atof(Line+16);
	  SetANTSCCWeight(ANTSCCWeight);	
	}
	else if ( (std::strncmp("ANTS CC region radius: ", Line, 23)) == 0)
	{
	  ANTSCCRegionRadius = atof(Line+23);
	  SetANTSCCRegionRadius(ANTSCCRegionRadius);	
	}
	else if ( (std::strncmp("ANTS MI weight: ", Line, 16)) == 0)
	{
	  ANTSMIWeight = atof(Line+16);
	  SetANTSMIWeight(ANTSMIWeight);	
	}
	else if ( (std::strncmp("ANTS MI bins: ", Line, 14)) == 0)
	{
	  ANTSMIBins = atoi(Line+14);
	  SetANTSMIBins(ANTSMIBins);	
	}
	else if ( (std::strncmp("ANTS MSQ weight: ", Line, 17)) == 0)
	{
	  ANTSMSQWeight = atof(Line+17);
	  SetANTSMSQWeight(ANTSMSQWeight);	
	}
        else if ( (std::strncmp("ANTS CC weight for 2nd modality: ", Line, 33)) == 0)
	{
	  ANTSCCWeight = atof(Line+33);
	  SetANTSCCWeight2nd(ANTSCCWeight);	
	} else if ( (std::strncmp("ANTS CC region radius for 2nd modality: ", Line, 40)) == 0) {
	  ANTSCCRegionRadius = atof(Line+40);
	  SetANTSCCRegionRadius2nd(ANTSCCRegionRadius);	
	}
	else if ( (std::strncmp("ANTS MI weight for 2nd modality: ", Line, 33)) == 0)
	{
	  ANTSMIWeight = atof(Line+33);
	  SetANTSMIWeight2nd(ANTSMIWeight);	
	}
	else if ( (std::strncmp("ANTS MI bins for 2nd modality: ", Line, 31)) == 0)
	{
	  ANTSMIBins = atoi(Line+31);
	  SetANTSMIBins2nd(ANTSMIBins);	
	}
	else if ( (std::strncmp("ANTS MSQ weight for 2nd modality: ", Line, 34)) == 0)
	{
	  ANTSMSQWeight = atof(Line+34);
	  SetANTSMSQWeight2nd(ANTSMSQWeight);	
	}
	else if ( (std::strncmp("ANTS Registration Type: ", Line, 24)) == 0)
	{
	  ANTSRegistrationFilterType = Line+24;	
	  if (ANTSRegistrationFilterType=="GreedyDiffeomorphism"){
	    SetANTSRegistrationFilterType("GreedyDiffeomorphism");
	  }
	  else if(ANTSRegistrationFilterType=="SpatiotemporalDiffeomorphism"){
	    SetANTSRegistrationFilterType("SpatiotemporalDiffeomorphism");
	  }
	  else if(ANTSRegistrationFilterType=="Elastic"){
	    SetANTSRegistrationFilterType("Elastic");
	  }
	  else if(ANTSRegistrationFilterType=="Exponential"){
	    SetANTSRegistrationFilterType("Exponential");
	  }
	}
	else if ( (std::strncmp("ANTS Registration Step: ", Line, 24)) == 0)
	{
	  ANTSTransformationStep = Line+24;
	  SetANTSTransformationStep(ANTSTransformationStep.c_str());	
	}
	else if ( (std::strncmp("ANTS Gaussian Smoothing: ", Line, 25)) == 0)
	{
	  ANTSGaussianSmoothing = atoi(Line+25);
	  SetANTSGaussianSmoothing(ANTSGaussianSmoothing);	
	}
	else if ( (std::strncmp("ANTS Gaussian Sigma: ", Line, 21)) == 0)
	{
	  ANTSGaussianSigma = atof(Line+21);
	  SetANTSGaussianSigma(ANTSGaussianSigma);	
	}
        else if ( (std::strncmp("ANTS Number of Threads: ", Line, 24)) == 0)
	{
	  SetNbANTSThreads(atoi(Line+24));	
	}
      }
      if(mode == N4biasFieldCorrection||mode == advancedParameters||mode == file)
      {	
	if ((std::strncmp("Bias Correction stripped image: ", Line, 32)) == 0)
	{
	  value = atoi(Line+32);
	  if (value == 1)
	    SetStrippedN4ITKBiasFieldCorrection(1);
	  else
	    SetStrippedN4ITKBiasFieldCorrection(0);
	}
	if ((std::strncmp("N4 ITK Bias Field Correction: ", Line, 30)) == 0)
	{
	  N4ITKBiasFieldCorrection= atoi(Line+30);
	  if(mode == N4biasFieldCorrection)
	    SetN4ITKBiasFieldCorrection(1);
	  else
	  {
	    if (N4ITKBiasFieldCorrection == 1)
	      SetN4ITKBiasFieldCorrection(1);
	    else
	      SetN4ITKBiasFieldCorrection(0);
	  }
					
	}
	else if ( (std::strncmp("N4 Number of iterations: ", Line, 25)) == 0)
	{
	  NbOfIterations = Line+25;
	  SetNbOfIterations(NbOfIterations.c_str());
	}
	else if ( (std::strncmp("N4 Spline distance: ", Line, 20)) == 0)
	{
	  SplineDistance = atof(Line+20);
	  SetSplineDistance(SplineDistance);
	}
	else if ( (std::strncmp("N4 Shrink factor: ", Line, 18)) == 0)
	{
	  ShrinkFactor = atoi(Line+18);
	  SetShrinkFactor(ShrinkFactor);
	}
	else if ( (std::strncmp("N4 Convergence threshold: ", Line, 26)) == 0)
	{
	  ConvergenceThreshold = atof(Line+26);
	  SetConvergenceThreshold(ConvergenceThreshold);
	}
	else if ( (std::strncmp("N4 BSpline grid resolutions: ", Line, 29)) == 0)
	{
	  BSplineGridResolutions = Line+29;
	  SetBSplineGridResolutions(BSplineGridResolutions.c_str());
	}
	else if ( (std::strncmp("N4 BSpline alpha: ", Line, 18)) == 0)
	{
	  BSplineAlpha = atof(Line+18);
	  SetBSplineAlpha(BSplineAlpha);
	}
	else if ( (std::strncmp("N4 BSpline beta: ", Line, 17)) == 0)
	{
	  BSplineBeta = atof(Line+17);
	  SetBSplineBeta(BSplineBeta);
	}
	else if ( (std::strncmp("N4 Histogram sharpening: ", Line, 25)) == 0)
	{
	  HistogramSharpening = Line+25;
	  SetHistogramSharpening(HistogramSharpening.c_str());
	}
	else if ( (std::strncmp("N4 BSpline order: ", Line, 18)) == 0)
	{
	  BSplineOrder =atoi(Line+18);
	  SetBSplineOrder(BSplineOrder);
	}
        else if ( (std::strncmp("The Version of Slicer Used: ", Line, 27)) == 0)
	{
	  SlicerVersion = atof(Line+27);
	  SetSlicerVersion(SlicerVersion);
	}
        else if ( (std::strncmp("Label Fusion Algorithm: ", Line, 24)) == 0)
        {
          if (std::strcmp(Line+24,"Majority Voting") == 0)
          {
            SetLabelFusionAlgorithm("Majority Voting");
          }
          if (std::strcmp(Line+24,"Weighted Majority Voting") == 0)
          {
            SetLabelFusionAlgorithm("Weighted Majority Voting");
          }
          if (std::strcmp(Line+24,"STAPLE") == 0)
          {
            SetLabelFusionAlgorithm("STAPLE");
          }
        }
        else if ( (std::strncmp("Intensity Energy Weight: ", Line, 25)) == 0)
        {
            SetWeightIntensityEnergy(atof(Line + 25));
        }
        else if ( (std::strncmp("Harmonic Energy Weight: ", Line, 24)) == 0)
        {
            SetWeightHarmonicEnergy(atof(Line + 24));
        }
        else if ( (std::strncmp("Shape Energy Weight: ", Line, 21)) == 0)
        {
            SetWeightShapeEnergy(atof(Line + 21));
        }
	else if ( (std::strncmp("Multi-atlas directory: ", Line, 23)) == 0)
	{
	  if (std::strlen(Line+23) != 0)
	    SetMultiAtlasDirectory(Line+23); 
	  else
	    SetMultiAtlasDirectory("");
	}
        else if ( (std::strncmp("ANTS with brainmask: ", Line, 21)) == 0)
        {
            SetANTSWithBrainmask(atof(Line + 21));
        }
        else if ( (std::strncmp("Use Initital Affine Transform: ", Line, 31)) == 0)
        {
            SetUseInitialAffine(atof(Line + 31));
        } 
	else if (Line[0] != '/' && Line[0] != '#' && (std::strcmp("",Line) != 0) && (std::strcmp(" ",Line) != 0)) 
	{
	  unknownLineWarping = true;
	}

	if (unknownLineWarping && unknownLineTissueSeg && unknownLineFile && (mode == file)) 
	{
	  //std::cout << "Unknown Line: " << Line << std::endl;
	}
      }
    }
    fclose(ParameterFile);
  }
  else
    std::cout<<"Error Opening File: "<<_FileName<<std::endl;

  return IsParameterFileLoaded;
}

