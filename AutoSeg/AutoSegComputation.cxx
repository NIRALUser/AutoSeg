/*=========================================================================

  Program:   AutoSeg
  Module:    $RCSfile: AutoSegComputation.cxx,v $
  Language:  C++
  Date:      $Date: 2011/04/19 08:04:40 $
  Version:   $Revision: 1.45 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "AutoSegComputation.h"

AutoSegComputation::AutoSegComputation()
{
  m_AllocationData=0;
  m_AllocationAuxData=0;
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

  m_T1List = new char *[GetNbData()];
  for (DataNumber = 0; DataNumber < GetNbData(); DataNumber++)
  {
    m_T1List[DataNumber] = new char[512];
    std::strcpy(m_T1List[DataNumber], "");
  }
  if (GetT2Image())
  {
    m_T2List = new char *[GetNbData()];
    for (DataNumber = 0; DataNumber < GetNbData(); DataNumber++)
    {
      m_T2List[DataNumber] = new char[512];  
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
      m_PDList[DataNumber] = new char[512];
      std::strcpy(m_PDList[DataNumber], "");
    }
  }
  else
    m_PDList=NULL;
  m_AllocationData=1;
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
	m_AuxT1List[DataNumber] = new char[512];
	std::strcpy(m_AuxT1List[DataNumber], "");
      }
    }
    if (GetAuxT2Image())
    {
      m_AuxT2List = new char *[GetNbAuxData()];
      for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
      {
	m_AuxT2List[DataNumber] = new char[512];
	std::strcpy(m_AuxT2List[DataNumber], "");
      }
    } 
    if (GetAuxPDImage())
    {
      m_AuxPDList = new char *[GetNbAuxData()];
      for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
      {
	m_AuxPDList[DataNumber] = new char[512];
	std::strcpy(m_AuxPDList[DataNumber], "");
      }
    }
  }
  if (GetAux1Image())
  {
    m_Aux1List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux1List[DataNumber] = new char[512];
      std::strcpy(m_Aux1List[DataNumber], "");
    }
  }
  if (GetAux2Image())
  {
    m_Aux2List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux2List[DataNumber] = new char[512];
      std::strcpy(m_Aux2List[DataNumber], "");
    }
  }
  if (GetAux3Image())
  {
    m_Aux3List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux3List[DataNumber] = new char[512];
      std::strcpy(m_Aux3List[DataNumber], "");
    }
  }
  if (GetAux4Image())
  {
    m_Aux4List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux4List[DataNumber] = new char[512];
      std::strcpy(m_Aux4List[DataNumber], "");
    }
  }
  if (GetAux5Image())
  {
    m_Aux5List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux5List[DataNumber] = new char[512];
      std::strcpy(m_Aux5List[DataNumber], "");
    }
  }
  if (GetAux6Image())
  {
    m_Aux6List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux6List[DataNumber] = new char[512];
      std::strcpy(m_Aux6List[DataNumber], "");
    }
  }
  if (GetAux7Image())
  {
    m_Aux7List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux7List[DataNumber] = new char[512];
      std::strcpy(m_Aux7List[DataNumber], "");
    }
  }
  if (GetAux8Image())
  {
    m_Aux8List = new char *[GetNbAuxData()];
    for (DataNumber = 0; DataNumber < GetNbAuxData(); DataNumber++)
    {
      m_Aux8List[DataNumber] = new char[512];
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
    if ( !GetT2Image() && !GetPDImage()) 
      SetData(_Data, m_T1List[_DataNumber]);
    else if (GetT2Image() && GetPDImage()) 
      SetDatawoGUI(_Data, m_T1List[_DataNumber], m_T2List[_DataNumber], m_PDList[_DataNumber]);
    else if (GetT2Image())
      SetDatawoGUI(_Data, m_T1List[_DataNumber], m_T2List[_DataNumber]);
    else
      SetDatawoGUI(_Data, m_T1List[_DataNumber], m_PDList[_DataNumber]);
  }
  else
  {
    if ( !GetT2Image() && !GetPDImage()) 
      SetData(_Data, m_T1List[_DataNumber]);
    else if (GetT2Image() && GetPDImage()) 
      SetData(_Data, m_T1List[_DataNumber], m_T2List[_DataNumber], m_PDList[_DataNumber]);
    else if (GetT2Image())
      SetData(_Data, m_T1List[_DataNumber], m_T2List[_DataNumber]);
    else
      SetData(_Data, m_T1List[_DataNumber], m_PDList[_DataNumber]);
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

void AutoSegComputation::SetData(const char *_Data, char *_T1)
{
  std::strcpy(_T1, _Data);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1)
{
  std::strcpy(_T1, _Data);
}

void AutoSegComputation::SetData(const char *_Data, char *_T1, char *_SecondImage)
{
  int Char1 = 0;
  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;    
  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strcpy(_SecondImage,_Data+Char1+1);
}

void AutoSegComputation::SetDatawoGUI(const char *_Data, char *_T1, char *_SecondImage)
{
  int Char1 = 0;
  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;    
  std::strncpy(_T1, _Data, Char1);
  _T1[Char1] = '\0';
  std::strcpy(_SecondImage,_Data+Char1+1);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_SecondImage)
{
  int Char1 = 0;

  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;

  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strcpy(_SecondImage,_Data+Char1+1);
}

//Set Data: T1, T2 and PD files
void AutoSegComputation::SetData(const char *_Data, char *_T1, char *_T2, char *_PD)
{
  int Char1 = 0;
  int Char2 = 0;

  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;
  while (std::strncmp(" ", _Data+Char1+1+Char2, 1) != 0)
    Char2++;

  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strncpy(_T2,_Data+Char1+1+2, Char2-2);
  _T2[Char2-2] = '\0';
  std::strcpy(_PD, _Data+Char1+Char2+2);
}

void AutoSegComputation::SetDatawoGUI(const char *_Data, char *_T1, char *_T2, char *_PD)
{
  int Char1 = 0;
  int Char2 = 0;

  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;
  while (std::strncmp(" ", _Data+Char1+1+Char2, 1) != 0)
    Char2++;

  std::strncpy(_T1, _Data, Char1);
  _T1[Char1] = '\0';
  std::strncpy(_T2,_Data+Char1+1, Char2);
  _T2[Char2] = '\0';
  std::strcpy(_PD, _Data+Char1+Char2);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2)
{
  int Char1 = 0;
  int Char2 = 0;

  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;
  while (std::strncmp(" ", _Data+Char1+1+Char2, 1) != 0)
    Char2++;

  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strncpy(_Aux1,_Data+Char1+1+2, Char2-2);
  _Aux1[Char2-2] = '\0';
  std::strcpy(_Aux2, _Data+Char1+Char2+2);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3)
{
  int Char1 = 0;
  int Char2 = 0;
  int Char3 = 0;
  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;
  while (std::strncmp(" ", _Data+Char1+1+Char2, 1) != 0)
    Char2++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3, 1) != 0)
    Char3++;

  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strncpy(_Aux1,_Data+Char1+1+2, Char2-2);
  _Aux1[Char2-2] = '\0';
  std::strncpy(_Aux2, _Data+Char1+Char2+1+1+2, Char2-2);
  _Aux2[Char2-2] = '\0';
  std::strcpy(_Aux3, _Data+Char1+Char2+Char3+3);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4)
{
  int Char1 = 0;
  int Char2 = 0;
  int Char3 = 0;
  int Char4 = 0;
  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;
  while (std::strncmp(" ", _Data+Char1+1+Char2, 1) != 0)
    Char2++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3, 1) != 0)
    Char3++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4, 1) != 0)
    Char4++;

  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strncpy(_Aux1,_Data+Char1+1+2, Char2-2);
  _Aux1[Char2-2] = '\0';
  std::strncpy(_Aux2, _Data+Char1+Char2+2+2, Char2-2);
  _Aux2[Char2-2] = '\0';
  std::strncpy(_Aux3, _Data+Char1+Char2+Char3+3+2, Char2-2); 
  _Aux3[Char2-2] = '\0';
  std::strcpy(_Aux4, _Data+Char1+Char2+Char3+Char4+4);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5)
{
  int Char1 = 0;
  int Char2 = 0;
  int Char3 = 0;
  int Char4 = 0;
  int Char5 = 0;
  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;
  while (std::strncmp(" ", _Data+Char1+1+Char2, 1) != 0)
    Char2++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3, 1) != 0)
    Char3++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4, 1) != 0)
    Char4++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5, 1) != 0)
    Char5++;

  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strncpy(_Aux1,_Data+Char1+1+2, Char2-2);
  _Aux1[Char2-2] = '\0';
  std::strncpy(_Aux2, _Data+Char1+Char2+2+2, Char2-2);
  _Aux2[Char2-2] = '\0';
  std::strncpy(_Aux3, _Data+Char1+Char2+Char3+3+2, Char2-2); 
  _Aux3[Char2-2] = '\0';
  std::strncpy(_Aux4, _Data+Char1+Char2+Char3+Char4+4+2, Char2-2);
  _Aux4[Char2-2] = '\0';
  std::strcpy(_Aux5, _Data+Char1+Char2+Char3+Char4+Char5+5);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5, char *_Aux6)
{
  int Char1 = 0;
  int Char2 = 0;
  int Char3 = 0;
  int Char4 = 0;
  int Char5 = 0;
  int Char6 = 0;
  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;
  while (std::strncmp(" ", _Data+Char1+1+Char2, 1) != 0)
    Char2++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3, 1) != 0)
    Char3++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4, 1) != 0)
    Char4++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5, 1) != 0)
    Char5++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5+1+Char6, 1) != 0)
    Char6++;

  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strncpy(_Aux1,_Data+Char1+1+2, Char2-2);
  _Aux1[Char2-2] = '\0';
  std::strncpy(_Aux2, _Data+Char1+Char2+2+2, Char2-2);
  _Aux2[Char2-2] = '\0';
  std::strncpy(_Aux3, _Data+Char1+Char2+Char3+3+2, Char2-2);
  _Aux3[Char2-2] = '\0';
  std::strncpy(_Aux4, _Data+Char1+Char2+Char3+Char4+4+2, Char2-2);
  _Aux4[Char2-2] = '\0';
  std::strncpy(_Aux5, _Data+Char1+Char2+Char3+Char4+Char5+5+2, Char2-2);
  _Aux5[Char2-2] = '\0';
  std::strcpy(_Aux6, _Data+Char1+Char2+Char3+Char4+Char5+Char6+6);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5, char *_Aux6, char *_Aux7)
{
  int Char1 = 0;
  int Char2 = 0;
  int Char3 = 0;
  int Char4 = 0;
  int Char5 = 0;
  int Char6 = 0;
  int Char7 = 0;
  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;
  while (std::strncmp(" ", _Data+Char1+1+Char2, 1) != 0)
    Char2++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3, 1) != 0)
    Char3++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4, 1) != 0)
    Char4++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5, 1) != 0)
    Char5++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5+1+Char6, 1) != 0)
    Char6++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5+1+Char6+1+Char7, 1) != 0)
    Char7++;

  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strncpy(_Aux1,_Data+Char1+1+2, Char2-2);
  _Aux1[Char2-2] = '\0';
  std::strncpy(_Aux2, _Data+Char1+Char2+2+2, Char2-2);
  _Aux2[Char2-2] = '\0';
  std::strncpy(_Aux3, _Data+Char1+Char2+Char3+3+2, Char2-2);
  _Aux3[Char2-2] = '\0';
  std::strncpy(_Aux4, _Data+Char1+Char2+Char3+Char4+4+2, Char2-2);
  _Aux4[Char2-2] = '\0';
  std::strncpy(_Aux5, _Data+Char1+Char2+Char3+Char4+Char5+5+2, Char2-2);
  _Aux5[Char2-2] = '\0';
  std::strncpy(_Aux6, _Data+Char1+Char2+Char3+Char4+Char5+Char6+6+2, Char2-2);
  _Aux6[Char2-2] = '\0';
  std::strcpy(_Aux7, _Data+Char1+Char2+Char3+Char4+Char5+Char6+Char7+7);
}

void AutoSegComputation::SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5, char *_Aux6, char *_Aux7, char *_Aux8)
{
  int Char1 = 0;
  int Char2 = 0;
  int Char3 = 0;
  int Char4 = 0;
  int Char5 = 0;
  int Char6 = 0;
  int Char7 = 0;
  int Char8 = 0;
  while (std::strncmp(" ", _Data+Char1, 1) != 0)
    Char1++;  
  while (std::strncmp(" ", _Data+Char1+1+Char2, 1) != 0)
    Char2++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3, 1) != 0)
    Char3++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4, 1) != 0)
    Char4++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5, 1) != 0)
    Char5++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5+1+Char6, 1) != 0)
    Char6++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5+1+Char6+1+Char7, 1) != 0)
    Char7++;
  while (std::strncmp(" ", _Data+Char1+1+Char2+1+Char3+1+Char4+1+Char5+1+Char6+1+Char7+1+Char8, 1) != 0)
    Char8++;

  std::strncpy(_T1, _Data+2, Char1-2);
  _T1[Char1-2] = '\0';
  std::strncpy(_Aux1,_Data+Char1+1+2, Char2-2);
  _Aux1[Char2-2] = '\0';
  std::strncpy(_Aux2, _Data+Char1+Char2+2+2, Char2-2);
  _Aux2[Char2-2] = '\0';
  std::strncpy(_Aux3, _Data+Char1+Char2+Char3+3+2, Char2-2);
  _Aux3[Char2-2] = '\0';
  std::strncpy(_Aux4, _Data+Char1+Char2+Char3+Char4+4+2, Char2-2);
  _Aux4[Char2-2] = '\0';
  std::strncpy(_Aux5, _Data+Char1+Char2+Char3+Char4+Char5+5+2, Char2-2);
  _Aux5[Char2-2] = '\0';
  std::strncpy(_Aux6, _Data+Char1+Char2+Char3+Char4+Char5+Char6+6+2, Char2-2);
  _Aux6[Char2-2] = '\0';
  std::strncpy(_Aux7, _Data+Char1+Char2+Char3+Char4+Char5+Char6+Char7+7+2, Char2-2);
  _Aux7[Char2-2] = '\0';
  std::strcpy(_Aux8, _Data+Char1+Char2+Char3+Char4+Char5+Char6+Char7+Char8+8);
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


// Compute Automatic Segmentation
void AutoSegComputation::Computation()
{
  SetParameterFile();
  SetComputationFile();
  SetBMSAutoSegFile();
  SetBMSAutoSegMainFile();
  if (GetAux1Image())
  {
    SetBMSAutoSegAuxFile();
    SetBMSAutoSegMRMLSourceFile();
    SetBMSAutoSegMRMLParcelFile();
    SetBMSAutoSegMRMLAllROIFile();
  }
  SetLogFile();

  WriteParameterFile(GetParameterFile());
  WriteComputationFile(GetComputationFile());
  WriteBMSAutoSegFile();
  WriteBMSAutoSegMainFile();
  if (GetAux1Image())
  {
    WriteBMSAutoSegAuxFile();
    WriteBMSAutoSegMRMLSourceFile();
    WriteBMSAutoSegMRMLParcelFile();
    WriteBMSAutoSegMRMLAllROIFile();
  }
  m_KillProcess = false;
  m_output.clear();
  ExecuteBatchMake(GetBMSAutoSegFile(),1);
}

// Compute Automatic Segmentation
void AutoSegComputation::ComputationWithoutGUI(const char *_computationFile, const char *_parameterFile)
{
  LoadComputationFile(_computationFile);
  LoadParameterFile(_parameterFile);
  SetParameterFile(_computationFile);
  SetComputationFile(_parameterFile);
  SetBMSAutoSegFile();
  SetBMSAutoSegMainFile();
  SetLogFile();
  SetAux1Image(0);
  WriteBMSAutoSegFile();
  WriteBMSAutoSegMainFile();
  m_KillProcess = false;
  m_output.clear();
  ExecuteBatchMake(GetBMSAutoSegFile(),0);
}

void AutoSegComputation::ShowDisplay()
{
  TextDisplay.g_MainWindow->show();  
  Fl::check();
}

// Execute Batchmake script
//_GUIMode == 1 with GUI
//_GUIMode == 0 without GUI
void AutoSegComputation::ExecuteBatchMake(char *_Input, int _GUIMode)
{
  //Text Display
  //std::string m_output;
  std::ofstream LogFile(GetLogFile());
  
  std::vector<const char*> args;  
  char* data = NULL;
  int length;
  double timeout = 0.05;
  int result, Value;

  //char BatchMakeApplicationDir[512];

  //std::strcpy(BatchMakeApplicationDir, GetAutoSegPath());
  //std::strcat(BatchMakeApplicationDir, "BatchMakeApplications");

  if (_GUIMode == 1)
  {                
    TextDisplay.g_TextDisp->buffer(m_TextBuf); 
    TextDisplay.g_MainWindow->show(); 
    Fl::check();
  }  
  
  args.push_back("BatchMake");
  //if (_Mode == 1)
  //  {
  //    args.push_back("-p");
  //    args.push_back(BatchMakeApplicationDir);
  //  }
  args.push_back("-e");
  args.push_back(_Input);
  // don't forget this line at the end of the argument list
  args.push_back(0);    

  // Run the application
  m_Process = itksysProcess_New();
  itksysProcess_SetCommand(m_Process, &*args.begin());
  itksysProcess_SetOption(m_Process,itksysProcess_Option_HideWindow,1);
  itksysProcess_Execute(m_Process);   
  
  if (_GUIMode == 1)
  {
    while(Value = itksysProcess_WaitForData(m_Process,&data,&length,&timeout))
    {
      if ( (Value == itksysProcess_Pipe_STDOUT) || (Value == itksysProcess_Pipe_STDERR) )
      {
	for(int i=0;i<length;i++)
	  {
	    m_output +=data[i]; 
	    LogFile<<data[i];
	  }
	m_TextBuf.text(m_output.c_str());
	TextDisplay.g_TextDisp->scroll(1000,0);
      }
      timeout = 0.05;
      Fl::check();
      if(m_KillProcess)
      {
	itksysProcess_Kill(m_Process);
	break;
      }
    }
  }
  else
    {
      while(Value = itksysProcess_WaitForData(m_Process,&data,&length,NULL))
	{
	  if ( (Value == itksysProcess_Pipe_STDOUT) || (Value == itksysProcess_Pipe_STDERR) )
	    {
	      for(int i=0;i<length;i++)
		{
		  std::cout<<data[i];
		  m_output +=data[i];
		  LogFile<<data[i];
		}
	    }
	  if(m_KillProcess)
	    {
	      itksysProcess_Kill(m_Process);
	      break;
	    }
	}
    }
  
  itksysProcess_WaitForExit(m_Process, 0);

  result = 1;
  switch(itksysProcess_GetState(m_Process))
  {
    case itksysProcess_State_Exited:
    {
      result = itksysProcess_GetExitValue(m_Process);
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
      ComputationFile<<"Data: "<<m_T1List[i]<<std::endl;
  }
  else if (GetT2Image() && GetPDImage()) 
  {
    for (i = 0; i < GetNbData(); i++)
      ComputationFile<<"Data: "<<m_T1List[i]<<" "<<m_T2List[i]<<" "<<m_PDList[i]<<std::endl;  
  }
  else if (GetT2Image())
  {
    for (i = 0; i < GetNbData(); i++)
      ComputationFile<<"Data: "<<m_T1List[i]<<" "<<m_T2List[i]<<std::endl;  
  }
  else 
  {
    for (i = 0; i < GetNbData(); i++)
      ComputationFile<<"Data: "<<m_T1List[i]<<" "<<m_PDList[i]<<std::endl;  
  }
  if (GetAux1Image())
  {
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
  if (std::strcmp(GetEMSoftware(), "ABC") == 0)
  {
    ParameterFile<<"Fluid Atlas Warp: "<<GetFluidAtlasWarp()<<std::endl;
    ParameterFile<<"Fluid Atlas Affine: "<<GetFluidAtlasAffine()<<std::endl;
    ParameterFile<<"Fluid Atlas FATW: "<<GetFluidAtlasFATW()<<std::endl;
    ParameterFile<<"Fluid Atlas Warp Iterations: "<<GetFluidAtlasWarpIterations()<<std::endl;
    ParameterFile<<"Fluid Atlas Warp Max Step: "<<GetFluidAtlasWarpMaxStep()<<std::endl;
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
  else
    ParameterFile<<"Warping Method: Coarse-to-fine"<<std::endl;
  ParameterFile<<"Alpha: "<<GetAlpha()<<std::endl;
  ParameterFile<<"Beta: "<<GetBeta()<<std::endl;
  ParameterFile<<"Gamma: "<<GetGamma()<<std::endl;
  ParameterFile<<"Max Perturbation: "<<GetMaxPerturbation()<<std::endl;
  ParameterFile<<"NumBasis: "<<GetNumBasis()<<std::endl;
  ParameterFile<<"Scale 4 - Number Of Iterations: "<<GetScale4NbIterations()<<std::endl;
  ParameterFile<<"Scale 2 - Number Of Iterations: "<<GetScale2NbIterations()<<std::endl;
  ParameterFile<<"Scale 1 - Number Of Iterations: "<<GetScale1NbIterations()<<std::endl;
  ParameterFile<<"Registration Filter Type: "<<GetRegistrationFilterType()<<std::endl;
  ParameterFile<<"Deformation Field Smoothing Sigma: "<<GetDeformationFieldSmoothingSigma()<<std::endl;
  ParameterFile<<"Pyramid Levels: "<<GetPyramidLevels()<<std::endl;
  ParameterFile<<"Moving Shrink Factors: "<<GetMovingShrinkFactors()<<std::endl;
  ParameterFile<<"Fixed Shrink Factors: "<<GetFixedShrinkFactors()<<std::endl;
  ParameterFile<<"Iteration Count Pyramid Levels: "<<GetIterationCountPyramidLevels()<<std::endl;
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

  ParameterFile<<"\n// Reorientation"<<std::endl;
  ParameterFile<<"Reorientation: "<<GetReorientation()<<std::endl;
  ParameterFile<<"Input Orientation: "<<GetInputDataOrientation()<<std::endl;
  ParameterFile<<"Output Orientation: "<<GetOutputDataOrientation()<<std::endl;

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
    BMSAutoSegFile<<"echo ('SOURCE MRML SCENES CREATION...')"<<std::endl;
    BMSAutoSegFile<<"Include(${ProcessingDir}SourceMRMLAutoSeg.bms)"<<std::endl;
    if (GetParcellationMapSegmentation())
    {
      BMSAutoSegFile<<"echo (*************************************************)"<<std::endl;
      BMSAutoSegFile<<"echo ('PARCELLATION MRML SCENES CREATION...')"<<std::endl;
      BMSAutoSegFile<<"Include(${ProcessingDir}ParcelMRMLAutoSeg.bms)"<<std::endl;
    }
		/*std::string CaudateLeft=GetCaudateLeft();
    std::string CaudateRight=GetCaudateRight();
    std::string HippocampusLeft=GetHippocampusLeft();
    std::string HippocampusRight=GetHippocampusRight();
    std::string PutamenLeft=GetPutamenLeft();
    std::string PutamenRight=GetPutamenRight();
    if (CaudateLeft!="" || CaudateRight!="" || HippocampusLeft!="" || HippocampusRight!="" || PutamenLeft!="" ||PutamenRight!="")
    {
    BMSAutoSegFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegFile<<"echo ('STRUCTURES MRML SCENES CREATION...')"<<std::endl;
    BMSAutoSegFile<<"Include(${ProcessingDir}StructMRMLAutoSeg.bms)"<<std::endl;
  }*/
    BMSAutoSegFile<<"set (ProcessDataDirectoryPath "<<GetProcessDataDirectory()<<")"<<std::endl;
    BMSAutoSegFile<<"set (WarpROIDir ${ProcessDataDirectoryPath}WarpROI/)"<<std::endl;
    BMSAutoSegFile<<"ListFileInDir(AllROICase ${WarpROIDir} *AllROI.nrrd)"<<std::endl;
    BMSAutoSegFile<<"If (${AllROICase} != '')"<<std::endl;
    BMSAutoSegFile<<"  echo (*************************************************)"<<std::endl;
    BMSAutoSegFile<<"  echo ('ALL ROI MRML SCENES CREATION...')"<<std::endl;
    BMSAutoSegFile<<"  Include(${ProcessingDir}AllROIMRMLAutoSeg.bms)"<<std::endl;
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
  for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
    BMSAutoSegMainFile<<"set (OrigT1CasesList ${OrigT1CasesList} "<<m_T1List[DataNumber]<<")"<<std::endl;

  if (GetT2Image())
  {
    BMSAutoSegMainFile<<"\nset (OrigT2CasesList "<<m_T2List[0]<<")"<<std::endl;
    for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
      BMSAutoSegMainFile<<"set (OrigT2CasesList ${OrigT2CasesList} "<<m_T2List[DataNumber]<<")"<<std::endl;
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
  BMSAutoSegMainFile<<"set (atlasROIFile "<<GetROIAtlasFile()<<")"<<std::endl;

  BMSAutoSegMainFile<<"# Stripped Atlas Directory"<<std::endl;
  BMSAutoSegMainFile<<"set (atlasSegLocLoop "<<GetAtlasLoop()<<")"<<std::endl;
  
  BMSAutoSegMainFile<<"# Parameters"<<std::endl;
  BMSAutoSegMainFile<<"set(EMSoftware "<<GetEMSoftware()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ComputeVolume "<<GetComputeVolume()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (ComputeCorticalThickness "<<GetComputeCorticalThickness()<<")"<<std::endl;
  BMSAutoSegMainFile<<"set (RegistrationInitialization "<<GetRegistrationInitialization()<<")"<<std::endl;

  BMSAutoSegMainFile<<"# Programs "<<std::endl;
  BMSAutoSegMainFile<<"set (ABCCmd ABC)"<<std::endl;
  BMSAutoSegMainFile<<"set (neosegCmd neoseg)"<<std::endl;
  BMSAutoSegMainFile<<"set (warpCmd WarpTool)"<<std::endl;
  BMSAutoSegMainFile<<"set (ImageMathCmd ImageMath)"<<std::endl;
  BMSAutoSegMainFile<<"set (SegPostProcessCmd SegPostProcessCLP)"<<std::endl;
  BMSAutoSegMainFile<<"set (IntensityRescalerCmd IntensityRescaler)"<<std::endl;
  BMSAutoSegMainFile<<"set (convCmd convertITKformats)"<<std::endl;
  BMSAutoSegMainFile<<"set (imconvert3Cmd imconvert3)"<<std::endl;
  BMSAutoSegMainFile<<"set (ImageStatCmd ImageStat)"<<std::endl;
  BMSAutoSegMainFile<<"set (fWarpCmd fWarp)"<<std::endl;
  BMSAutoSegMainFile<<"set (txApplyCmd txApply)"<<std::endl;
  BMSAutoSegMainFile<<"set (CortThickCLPCmd CortThickCLP)"<<std::endl<<std::endl;
  BMSAutoSegMainFile<<"set (N4Cmd N4ITKBiasFieldCorrection)"<<std::endl<<std::endl;
  BMSAutoSegMainFile<<"set (ResampleVolume2Cmd ResampleVolume2)"<<std::endl<<std::endl;
  BMSAutoSegMainFile<<"set (BRAINSDemonWarpCmd BRAINSDemonWarp)"<<std::endl<<std::endl;
  BMSAutoSegMainFile<<"set (BRAINSFitCmd BRAINSFit)"<<std::endl<<std::endl;
  BMSAutoSegMainFile<<"set (ModelMakerCmd ModelMaker)"<<std::endl<<std::endl;
  
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
    BMSAutoSegMainFile<<"Run(output 'rm -r ${ProcessingDir}AutoSeg_Volume/')"<<std::endl;
    BMSAutoSegMainFile<<"Run(output 'rm -r ${ProcessingDir}AutoSeg_CorticalThickness/')"<<std::endl;
    BMSAutoSegMainFile<<"Run(output 'rm -r ${ProcessingDir}AutoSeg_MRML/')"<<std::endl;
    BMSAutoSegMainFile<<"#  Delete files in the data directory"<<std::endl;
    BMSAutoSegMainFile<<"ForEach (OrigCase ${OrigT1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"   Run(output 'rm -r ${Path}/${AutoSegDir}/')"<<std::endl;  
    BMSAutoSegMainFile<<"EndForEach(OrigCase)"<<std::endl<<std::endl; 
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;  
  }
  
  BMSAutoSegMainFile<<"# Pipeline "<<std::endl;
  BMSAutoSegMainFile<<"# 1. If necessary, image reorientation "<<std::endl;
  BMSAutoSegMainFile<<"# 2. N4 ITK bias field correction"<<std::endl;
  BMSAutoSegMainFile<<"# 3. Registration to commom coordinate image"<<std::endl;
  BMSAutoSegMainFile<<"# 4. Atlas-based expectation maximization tissue segmentation segmentation"<<std::endl;
  BMSAutoSegMainFile<<"# 5. Skull-stripping"<<std::endl;
  BMSAutoSegMainFile<<"# 6. Intensity calibration: histogram quantile matching or tissue mean matching"<<std::endl;
  BMSAutoSegMainFile<<"# 7. Atlas to case affine registration"<<std::endl;
  BMSAutoSegMainFile<<"# 8. Atlas to case warping"<<std::endl;
  BMSAutoSegMainFile<<"# 9. Applying the transformations to the probabilistic maps (ROIs) and label maps (parcellation map and generic ROI map"<<std::endl;
  BMSAutoSegMainFile<<"# 10. Probabilistic volume thresholding"<<std::endl;
  BMSAutoSegMainFile<<"# 11. ROI gathering"<<std::endl<<std::endl;
  if (GetComputeCorticalThickness())
    BMSAutoSegMainFile<<"# 12. Option 1: Regional cortical thickness"<<std::endl;
  if (GetComputeVolume())
  {
    BMSAutoSegMainFile<<"# 13. Option 2: Volume information"<<std::endl;
    BMSAutoSegMainFile<<"#   - Volumes: Tissue segmentation, parcellation maps, subcortical structures"<<std::endl;
    BMSAutoSegMainFile<<"#   - Lobar cortical thickness analysis"<<std::endl<<std::endl;  
  }

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
  if (GetT2Image())
  {
    BMSAutoSegMainFile<<"GetParam(Case ${OrigT2CasesList} 0)"<<std::endl;
    BMSAutoSegMainFile<<"GetFilename (T2ImageExtension ${Case} EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"set (T2CasesList ${OrigT2CasesList})"<<std::endl;
  }
  if (GetPDImage())
  {
    BMSAutoSegMainFile<<"GetParam(Case ${OrigPDCasesList} 0)"<<std::endl;
    BMSAutoSegMainFile<<"GetFilename (PDImageExtension ${Case} EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"set (PDCasesList ${OrigPDCasesList})"<<std::endl;
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
      //BMSAutoSegMainFile<<"   SetApp(ImageMathCmd @ImageMath)"<<std::endl;
      //BMSAutoSegMainFile<<"   SetAppOption(ImageMathCmd.Input ${atlasROIFile})"<<std::endl;
      //BMSAutoSegMainFile<<"   SetAppOption(ImageMathCmd.OutputFileName ${GridTemplate})"<<std::endl;
      //BMSAutoSegMainFile<<"   SetAppOption(ImageMathCmd.CreateImageValue ${SizeX}','${SizeY}','${SizeZ}','${SpacingX}','${SpacingY}','${SpacingZ})"<<std::endl;
      //BMSAutoSegMainFile<<"   Run (output ${ImageMathCmd})"<<std::endl;
    BMSAutoSegMainFile<<"   Run (output '${ImageMathCmd} ${atlasROIFile} -outfile ${GridTemplate} -createIm ${SizeX},${SizeY},${SizeZ},${SpacingX},${SpacingY},${SpacingZ}')"<<std::endl;
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
  BMSAutoSegMainFile<<"      Run(output 'cp "<<GetParameterFile()<<" ${AutoSegPath}')"<<std::endl;
  BMSAutoSegMainFile<<"      Run(output 'cp "<<GetComputationFile()<<" ${AutoSegPath}')"<<std::endl;  
  BMSAutoSegMainFile<<"      Inc ({CaseNumber} 1)"<<std::endl;
  BMSAutoSegMainFile<<"      Int ({CaseNumber})"<<std::endl;
  BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"echo ('WRITING PARAMETER FILES: DONE!')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;  

  BMSAutoSegMainFile<<"echo (***********************************************)"<<std::endl;
  BMSAutoSegMainFile<<"echo ('WRITING MRML FILE...')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;

  BMSAutoSegMainFile<<"set (CasesList ${T1CasesList})"<<std::endl;
  BMSAutoSegMainFile<<"ForEach (Case ${CasesList})"<<std::endl;
  BMSAutoSegMainFile<<"      GetFilename (Path ${Case} PATH)"<<std::endl;
  BMSAutoSegMainFile<<"      GetFilename(CaseTail ${Case} NAME)"<<std::endl;
  BMSAutoSegMainFile<<"      GetFilename(CaseHead ${Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegMainFile<<"# Creating MRMLScene Directory if necessary"<<std::endl;
  BMSAutoSegMainFile<<"set (MRMLPath ${Path}/${AutoSegDir}/MRMLScene/)"<<std::endl;
  BMSAutoSegMainFile<<"ListDirInDir (MRMLList ${Path}/${AutoSegDir}/ MRMLScene)"<<std::endl;
  BMSAutoSegMainFile<<"If (${MRMLList} == '')"<<std::endl;
  BMSAutoSegMainFile<<"     MakeDirectory (${MRMLPath})"<<std::endl;
  BMSAutoSegMainFile<<"EndIf (${MRMLList})"<<std::endl;
  BMSAutoSegMainFile<<"# Creating MRMLScene Directory if necessary"<<std::endl;
  BMSAutoSegMainFile<<"ListDirInDir (MRMLSceneList ${MRMLPath} ${CaseHead}_MRMLScene)"<<std::endl;
  BMSAutoSegMainFile<<"If (${MRMLSceneList} == '')"<<std::endl;
  BMSAutoSegMainFile<<"     MakeDirectory (${MRMLPath}${CaseHead}_MRMLScene/)"<<std::endl;
  BMSAutoSegMainFile<<"EndIf (${MRMLSceneList})"<<std::endl;
  BMSAutoSegMainFile<<"set (MRMLScene ${MRMLPath}${CaseHead}_MRMLScene/${CaseHead}_MRMLScene.mrml)"<<std::endl;
  BMSAutoSegMainFile<<"ListFileInDir(OutputList ${MRMLPath}${CaseHead}_MRMLScene/ ${CaseHead}_MRMLScene.mrml)"<<std::endl;
  BMSAutoSegMainFile<<"If (${OutputList} == '')"<<std::endl;

  BMSAutoSegMainFile<<"   WriteFile(${MRMLScene} '<MRML  version=\"14658\" userTags=\"\">\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '<Selection\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSelectionNode1\"  name=\"vtkMRMLSelectionNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  activeVolumeID=\"vtkMRMLScalarVolumeNode1\"  secondaryVolumeID=\"NULL\"  activeLabelVolumeID=\"NULL\"  activeFiducialListID=\"NULL\"  activeROIListID=\"NULL\"  activeCameraID=\"NULL\"  activeViewID=\"NULL\"  activeLayoutID=\"vtkMRMLLayoutNode1\"></Selection>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <Interaction\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLInteractionNode1\"  name=\"vtkMRMLInteractionNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  currentInteractionMode=\"ViewTransform\"  lastInteractionMode=\"ViewTransform\" ></Interaction>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <Layout\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLLayoutNode1\"  name=\"vtkMRMLLayoutNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  currentViewArrangement=\"2\"  guiPanelVisibility=\"1\"  bottomPanelVisibility =\"1\"  guiPanelLR=\"0\"  collapseSliceControllers=\"0\"\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  numberOfCompareViewRows=\"1\"  numberOfCompareViewColumns=\"1\"  numberOfLightboxRows=\"1\"  numberOfLightboxColumns=\"1\"  mainPanelSize=\"400\"  secondaryPanelSize=\"400\" ></Layout>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <TGParameters\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLChangeTrackerNode1\"  name=\"vtkMRMLChangeTrackerNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  ROIMin=\"-1 -1 -1\"  ROIMax=\"-1 -1 -1\"  SegmentThresholdMin=\"-1\"  SegmentThresholdMax=\"-1\"  Analysis_Intensity_Flag=\"0\"  Analysis_Deformable_Flag=\"0\"  UseITK=\"1\"  RegistrationChoice=\"3\"  ROIRegistration=\"1\"  ResampleChoice=\"3\"  ResampleConst=\"0.5\" ></TGParameters>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <Crosshair\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLCrosshairNode1\"  name=\"vtkMRMLCrosshairNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  crosshairMode=\"NoCrosshair\"  navigation=\"true\"  crosshairBehavior=\"Normal\"  crosshairThickness=\"Fine\"  crosshairRAS=\"0 0 0\" ></Crosshair>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <Slice\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSliceNode1\"  name=\"Green\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  fieldOfView=\"175 179.487 1\"  dimensions=\"312 320 1\"  activeSlice=\"0\"  layoutGridRows=\"1\"  layoutGridColumns=\"1\"  sliceToRAS=\"-1 0 0 0 0 0 1 0 0 1 0 0 0 0 0 1\"  layoutName=\"Green\"  orientation=\"Coronal\"  jumpMode=\"1\"  sliceVisibility=\"true\"  widgetVisibility=\"false\"  useLabelOutline=\"false\"  sliceSpacingMode=\"0\"  prescribedSliceSpacing=\"1 1 1\" ></Slice>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSliceCompositeNode1\"  name=\"vtkMRMLSliceCompositeNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"  foregroundVolumeID=\"\"  labelVolumeID=\"\"  compositing=\"0\"  labelOpacity=\"1\"  linkedControl=\"1\"  foregroundGrid=\"0\"  backgroundGrid=\"0\"  labelGrid=\"1\"  fiducialVisibility=\"1\"  fiducialLabelVisibility=\"1\"  sliceIntersectionVisibility=\"0\"  layoutName=\"Green\"  annotationMode=\"All\"  doPropagateVolumeSelection=\"1\" ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <Slice\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSliceNode2\"  name=\"Red\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  fieldOfView=\"248.62 254.995 1\"  dimensions=\"312 320 1\"  activeSlice=\"0\"  layoutGridRows=\"1\"  layoutGridColumns=\"1\"  sliceToRAS=\"-1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1\"  layoutName=\"Red\"  orientation=\"Axial\"  jumpMode=\"1\"  sliceVisibility=\"true\"  widgetVisibility=\"false\"  useLabelOutline=\"false\"  sliceSpacingMode=\"0\"  prescribedSliceSpacing=\"1 1 1\" ></Slice>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSliceCompositeNode2\"  name=\"vtkMRMLSliceCompositeNode2\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"  foregroundVolumeID=\"\"  labelVolumeID=\"\"  compositing=\"0\"  labelOpacity=\"1\"  linkedControl=\"1\"  foregroundGrid=\"0\"  backgroundGrid=\"0\"  labelGrid=\"1\"  fiducialVisibility=\"1\"  fiducialLabelVisibility=\"1\"  sliceIntersectionVisibility=\"0\"  layoutName=\"Red\"  annotationMode=\"All\"  doPropagateVolumeSelection=\"1\" ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <Slice\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSliceNode3\"  name=\"Yellow\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  fieldOfView=\"255 261.538 1\"  dimensions=\"312 320 1\"  activeSlice=\"0\"  layoutGridRows=\"1\"  layoutGridColumns=\"1\"  sliceToRAS=\"0 0 1 0 -1 0 0 0 0 1 0 0 0 0 0 1\"  layoutName=\"Yellow\"  orientation=\"Sagittal\"  jumpMode=\"1\"  sliceVisibility=\"true\"  widgetVisibility=\"false\"  useLabelOutline=\"false\"  sliceSpacingMode=\"0\"  prescribedSliceSpacing=\"1 1 1\" ></Slice>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSliceCompositeNode3\"  name=\"vtkMRMLSliceCompositeNode3\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"  foregroundVolumeID=\"\"  labelVolumeID=\"\"  compositing=\"0\"  labelOpacity=\"1\"  linkedControl=\"1\"  foregroundGrid=\"0\"  backgroundGrid=\"0\"  labelGrid=\"1\"  fiducialVisibility=\"1\"  fiducialLabelVisibility=\"1\"  sliceIntersectionVisibility=\"0\"  layoutName=\"Yellow\"  annotationMode=\"All\"  doPropagateVolumeSelection=\"1\" ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <ScriptedModule\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLScriptedModuleNode1\"  name=\"vtkMRMLScriptedModuleNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" ModuleName =\"Editor\" parameter0= \"label 1\" ></ScriptedModule>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <View\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLViewNode1\"  name=\"View1\"  hideFromEditors=\"false\"  selectable=\"true\"  selected=\"false\"  active=\"true\"  visibility=\"true\"  fieldOfView=\"200\"  letterSize=\"0.05\"  boxVisible=\"true\"  fiducialsVisible=\"true\"  fiducialLabelsVisible=\"true\"  axisLabelsVisible=\"true\"  backgroundColor=\"0.70196 0.70196 0.90588\"  animationMode=\"Off\"  viewAxisMode=\"LookFrom\"  spinDegrees=\"2\"  spinMs=\"5\"  spinDirection=\"YawLeft\"  rotateDegrees=\"5\"  rockLength=\"200\"  rockCount=\"0\"  stereoType=\"NoStereo\"  renderMode=\"Perspective\" ></View>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <Camera\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLCameraNode1\"  name=\"Default Scene Camera\"  hideFromEditors=\"false\"  selectable=\"true\"  selected=\"false\"  position=\"0 500 0\"  focalPoint=\"0 0 0\"  viewUp=\"0 0 1\"  parallelProjection=\"false\"  parallelScale=\"1\"  activetag=\"vtkMRMLViewNode1\" ></Camera>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <VolumeArchetypeStorage\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLVolumeArchetypeStorageNode1\"  name=\"vtkMRMLVolumeArchetypeStorageNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  fileName=\"${Case}\"  useCompression=\"1\"  readState=\"0\"  writeState=\"0\"  centerImage=\"1\"  singleFile=\"0\"  UseOrientationFromFile=\"1\" ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <Volume\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLScalarVolumeNode1\"  name=\"${CaseTail}\"  hideFromEditors=\"false\"  selectable=\"true\"  selected=\"false\"  storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode1\"  userTags=\"\"  displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode1\"  ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"  spacing=\"1 1 1\"  origin=\"87.5 127.5 -87.5\"  labelMap=\"0\" ></Volume>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLScalarVolumeDisplayNode1\"  name=\"vtkMRMLScalarVolumeDisplayNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  color=\"0.5 0.5 0.5\"  selectedColor=\"1 0 0\"  selectedAmbient=\"0.4\"  ambient=\"0\"  diffuse=\"1\"  selectedSpecular=\"0.5\"  specular=\"0\"  power=\"1\"  opacity=\"1\"  visibility=\"true\"  clipping=\"false\"  sliceIntersectionVisibility=\"false\"  backfaceCulling=\"true\"  scalarVisibility=\"false\"  vectorVisibility=\"false\"  tensorVisibility=\"false\"  autoScalarRange=\"true\"  scalarRange=\"0 100\"  colorNodeRef=\"vtkMRMLColorTableNodeGrey\"   window=\"118\"  level=\"63\"  upperThreshold=\"32767\"  lowerThreshold=\"-32768\"  interpolate=\"1\"  autoWindowLevel=\"1\"  applyThreshold=\"0\"  autoThreshold=\"0\" ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"      Else ()"<<std::endl;
  BMSAutoSegMainFile<<"         echo ('${CaseHead}_MRMLScene.mrml already exists!')"<<std::endl;
  BMSAutoSegMainFile<<"EndIf (${OutputList})"<<std::endl;
  BMSAutoSegMainFile<<"EndForEach (Case)"<<std::endl;

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

    BMSAutoSegMainFile<<"    set (GiplCase ${ReorientationPath}${CaseHead}.gipl)"<<std::endl;
    BMSAutoSegMainFile<<"    set (GiplReorientedCase ${ReorientationPath}${CaseHead}${ProcessExtension}.gipl)"<<std::endl;
    BMSAutoSegMainFile<<"    set (command_line ${convCmd} ${Case} ${GiplCase})"<<std::endl;    
    BMSAutoSegMainFile<<"    Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"    set (command_line ${imconvert3Cmd} ${GiplCase} ${GiplReorientedCase} -setorient ${InputOrientation}-${OutputOrientation})"<<std::endl;    
    BMSAutoSegMainFile<<"    Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"    set (command_line ${convCmd} ${GiplReorientedCase} ${ReorientedCase})"<<std::endl;    
    BMSAutoSegMainFile<<"    Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"    DeleteFile(${GiplCase})"<<std::endl;
    BMSAutoSegMainFile<<"    DeleteFile(${GiplReorientedCase})"<<std::endl;  

    BMSAutoSegMainFile<<"   echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"   echo ('WRITING MRML FILE...')"<<std::endl;
    BMSAutoSegMainFile<<"   echo ( )"<<std::endl;

    BMSAutoSegMainFile<<"   set (MRMLPath ${Path}/${AutoSegDir}/MRMLScene/${CaseHead}_MRMLScene/)"<<std::endl;
    BMSAutoSegMainFile<<"   set (MRMLScene ${MRMLPath}${CaseHead}_MRMLScene.mrml)"<<std::endl;
    BMSAutoSegMainFile<<"   Set (my_output ${ReorientedCase})"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode1\"  name=\"${OutputOrientation}\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode1\"    secondaryVolumeID=\"NULL\"    activeLabelVolumeID=\"NULL\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"2\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"1\"    guiPanelLR=\"0\"    collapseSliceControllers=\"0\"\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    numberOfCompareViewRows=\"1\"    numberOfCompareViewColumns=\"1\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"    selectedModule=\"Volumes\"  ></Layout>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"-87.5 -127.5 87.5\"  ></Crosshair>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"175 175.591 1\"    dimensions=\"296 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -87.5 0 0 1 -127.5 0 1 0 87.5 0 0 0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"0\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"253.28 254.997 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -87.5 0 1 0 -127.5 0 0 1 87.5 0 0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"0\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"255 256.729 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"0 0 1 -87.5 -1 0 0 -127.5 0 1 0 87.5 0 0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"0\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View1\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    visibility=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Default Scene Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"0 500 0\"    focalPoint=\"0 0 0\"    viewUp=\"0 0 1\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode1\"  name=\"vtkMRMLVolumeArchetypeStorageNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  fileName=\"${my_output}\"  useCompression=\"1\"  readState=\"0\"  writeState=\"0\"  centerImage=\"1\"  singleFile=\"0\"  UseOrientationFromFile=\"1\" ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode1\"  name=\"${CaseHead}${ProcessExtension}\"  hideFromEditors=\"false\"  selectable=\"true\"  selected=\"false\"  storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode1\"  userTags=\"\"  displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode1\"  ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"  spacing=\"1 1 1\"  origin=\"87.5 127.5 -87.5\"  labelMap=\"0\" ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode1\"  name=\"vtkMRMLScalarVolumeDisplayNode1\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\"  color=\"0.5 0.5 0.5\"  selectedColor=\"1 0 0\"  selectedAmbient=\"0.4\"  ambient=\"0\"  diffuse=\"1\"  selectedSpecular=\"0.5\"  specular=\"0\"  power=\"1\"  opacity=\"1\"  visibility=\"true\"  clipping=\"false\"  sliceIntersectionVisibility=\"false\"  backfaceCulling=\"true\"  scalarVisibility=\"false\"  vectorVisibility=\"false\"  tensorVisibility=\"false\"  autoScalarRange=\"true\"  scalarRange=\"0 100\"  colorNodeRef=\"vtkMRMLColorTableNodeGrey\"   window=\"118\"  level=\"63\"  upperThreshold=\"32767\"  lowerThreshold=\"-32768\"  interpolate=\"1\"  autoWindowLevel=\"1\"  applyThreshold=\"0\"  autoThreshold=\"0\" ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"  Else ()"<<std::endl;
    BMSAutoSegMainFile<<"    echo ('Reoriented image already exists!')"<<std::endl;
    BMSAutoSegMainFile<<"  EndIf (${ReorientedCaseList})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (CaseN4)"<<std::endl<<std::endl;

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
  if (GetN4ITKBiasFieldCorrection())
  {
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

    BMSAutoSegMainFile<<"Set(Bias Bias)"<<std::endl;
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

    BMSAutoSegMainFile<<"  ListFileInDir(OutputFileN4List ${BiasPath} ${OrigCaseHead}${NewProcessExtension}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"  If (${OutputFileN4List} == '')"<<std::endl;
    BMSAutoSegMainFile<<"    Set (my_output ${BiasPath}${OrigCaseHead}${NewProcessExtension}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"    Set (parameters --histogramsharpening ${HistogramSharpening} --bsplinebeta ${BSplineBeta} --bsplinealpha ${BSplineAlpha} --bsplineorder ${BSplineOrder} --shrinkfactor ${ShrinkFactor} --splinedistance ${SplineDistance} --convergencethreshold ${ConvergenceThreshold} --iterations ${NbOfIterations} --meshresolution ${BSplineGridResolutions})"<<std::endl;
    BMSAutoSegMainFile<<"    Set (command_line ${N4Cmd} --outputimage ${my_output} --inputimage ${CaseN4} ${parameters})"<<std::endl;
    BMSAutoSegMainFile<<"    Run (prog_output ${command_line} ${parameters} prog_error)"<<std::endl;

    BMSAutoSegMainFile<<"  echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"  echo ('WRITING MRML FILE...')"<<std::endl;
    BMSAutoSegMainFile<<"  echo ( )"<<std::endl;

    BMSAutoSegMainFile<<"  set (MRMLPath ${OrigCasePath}/${AutoSegDir}/MRMLScene/${OrigCaseHead}_MRMLScene/)"<<std::endl;
    BMSAutoSegMainFile<<"  set (MRMLScene ${MRMLPath}${OrigCaseHead}_MRMLScene.mrml)"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode2\"  name=\"Bias\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode3\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode3\"    activeLabelVolumeID=\"NULL\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"2\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"1\"    guiPanelLR=\"0\"    collapseSliceControllers=\"0\"\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    numberOfCompareViewRows=\"1\"    numberOfCompareViewColumns=\"1\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"    selectedModule=\"Volumes\"  ></Layout>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"-87.5 -127.5 87.5\"  ></Crosshair>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"175 175.591 1\"    dimensions=\"296 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -87.5 0 0 1 -127.5 0 1 0 87.5 0 0 0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode3\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"0\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"253.28 254.997 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -87.5 0 1 0 -127.5 0 0 1 87.5 0 0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode3\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"0\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"255 256.729 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"0 0 1 -87.5 -1 0 0 -127.5 0 1 0 87.5 0 0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode3\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"0\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View1\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    visibility=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Default Scene Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"0 500 0\"    focalPoint=\"0 0 0\"    viewUp=\"0 0 1\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode1\"    name=\"vtkMRMLScalarVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode2\"    name=\"vtkMRMLVolumeArchetypeStorageNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${my_output}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"0\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode2\"    name=\"vtkMRMLScalarVolumeDisplayNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"118\"    level=\"63\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode3\"    name=\"vtkMRMLVolumeArchetypeStorageNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${my_output}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"0\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode3\"    name=\"${OrigCaseHead}${NewProcessExtension}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode3\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode3\"    ijkToRASDirections=\"-1   0   0 -0   -1   0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"-0 -0 -0\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode3\"    name=\"vtkMRMLScalarVolumeDisplayNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"766.13\"    level=\"383.065\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   Else ()"<<std::endl;
    BMSAutoSegMainFile<<"     echo ('Bias field corrected file already exists!')"<<std::endl;
    BMSAutoSegMainFile<<"   EndIf (${OutputFileN4List})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (CaseN4)"<<std::endl<<std::endl;

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
    BMSAutoSegMainFile<<"# 4. Rigid registration to an atlas"<<std::endl;
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
    BMSAutoSegMainFile<<"               echo ('Computing rigid transformation...')"<<std::endl;
    BMSAutoSegMainFile<<"    	set (command_line ${BRAINSFitCmd} --fixedVolume ${AtlasIsoTemplate} --movingVolume ${FirstCase} --transformType Rigid --initializeTransformMode ${RegistrationInitialization} --outputTransform ${TxtOutFile} --outputVolume ${OutputFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
    BMSAutoSegMainFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegMainFile<<"            Else ()"<<std::endl;
    BMSAutoSegMainFile<<"               echo ('Computing rigid transformation with initial transform...')"<<std::endl;
    BMSAutoSegMainFile<<"    	set (command_line ${BRAINSFitCmd} --fixedVolume ${AtlasIsoTemplate} --movingVolume ${FirstCase} --transformType Rigid --initialTransform ${TxtInitFile} --outputTransform ${TxtOutFile} --outputVolume ${OutputFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
    BMSAutoSegMainFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegMainFile<<"            EndIf (${TxtInitFileList})"<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${TxtOutFileList})"<<std::endl;
    BMSAutoSegMainFile<<"         # Applying Transformation"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Applying rigid transformation...')"<<std::endl;

    BMSAutoSegMainFile<<"       set (command_line ${ResampleVolume2Cmd} ${FirstCase} ${OutputFile} --transformationFile ${TxtOutFile} -i bs --Reference ${GridTemplate})"<<std::endl;
    BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;

    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WRITING MRML FILE...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;

    BMSAutoSegMainFile<<"set (MRMLPath ${OrigFirstCasePath}/${AutoSegDir}/MRMLScene/${OrigFirstCaseHead}_MRMLScene/)"<<std::endl;
    BMSAutoSegMainFile<<"set (MRMLScene ${MRMLPath}${OrigFirstCaseHead}_MRMLScene.mrml)"<<std::endl;

    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode3\"  name=\"atlasISO\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode2\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode2\"    activeLabelVolumeID=\"NULL\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"2\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"1\"    guiPanelLR=\"0\"    collapseSliceControllers=\"0\"\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    numberOfCompareViewRows=\"1\"    numberOfCompareViewColumns=\"1\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"  ></Layout>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"0 0 0\"  ></Crosshair>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"199 204.103 1\"    dimensions=\"312 320 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 0 0 0 1 0 0 1 0 0 0 0 0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode2\"    foregroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"218.4 224 1\"    dimensions=\"312 320 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode2\"    foregroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"224 229.744 1\"    dimensions=\"312 320 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"0 0 1 0 -1 0 0 0 0 1 0 0 0 0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode2\"    foregroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View2\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    visibility=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Default Scene Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"0 500 0\"    focalPoint=\"0 0 0\"    viewUp=\"0 0 1\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode1\"    name=\"vtkMRMLVolumeArchetypeStorageNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${AtlasIsoTemplate}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode1\"    name=\"${AtlasIsoTemplate_name}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode1\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode1\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"99.5 112 -99.5\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode1\"    name=\"vtkMRMLScalarVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeCoolShade1\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode2\"    name=\"vtkMRMLVolumeArchetypeStorageNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${OutputFile}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode2\"    name=\"${OutputFileTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode2\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode2\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"99.5 112 -99.5\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode2\"    name=\"vtkMRMLScalarVolumeDisplayNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeWarmShade1\"     window=\"794.457\"    level=\"379.982\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;

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
      BMSAutoSegMainFile<<"      echo ('Case Number: '${CaseHead})"<<std::endl;
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
	BMSAutoSegMainFile<<"               echo ('Computing rigid transformation...')"<<std::endl;
	BMSAutoSegMainFile<<"    	    set (command_line ${BRAINSFitCmd} --fixedVolume ${FirstCaseregAtlas} --movingVolume ${Case} --useRigid --initializeTransformMode ${RegistrationInitialization} --outputTransform ${TxtOutFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
	BMSAutoSegMainFile<<"      	    Run (output ${command_line} prog_error)"<<std::endl;
	BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;    
      }
    else
      {	
	BMSAutoSegMainFile<<"               echo ('Computing rigid transformation using T1 transform file as initialization...')"<<std::endl;
	BMSAutoSegMainFile<<"               set (FirstCaseTransformFile ${AtlasIsoPath}${FirstCaseHead}.txt)"<<std::endl;
	BMSAutoSegMainFile<<"    	    set (command_line ${BRAINSFitCmd} --fixedVolume ${FirstCaseregAtlas} --movingVolume ${Case} --useRigid --initialTransform ${FirstCaseTransformFile} --outputTransform ${TxtOutFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
	BMSAutoSegMainFile<<"      	    Run (output ${command_line} prog_error)"<<std::endl;
	BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;    
      }
    BMSAutoSegMainFile<<"             Else ()"<<std::endl;
    BMSAutoSegMainFile<<"               echo ('Computing rigid transformation with initial transform...')"<<std::endl;
    BMSAutoSegMainFile<<"    	        set (command_line ${BRAINSFitCmd} --fixedVolume ${FirstCaseregAtlas} --movingVolume ${Case} --useRigid --initialTransform ${TxtInitFile} --outputTransform ${TxtOutFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
    BMSAutoSegMainFile<<"      	        Run (output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"               WriteFile(${ReportFile} ${output})"<<std::endl;
    BMSAutoSegMainFile<<"             EndIf (${TxtInitFileList})"<<std::endl;
    
    BMSAutoSegMainFile<<"         EndIf (${TxtOutFileList})"<<std::endl;
      BMSAutoSegMainFile<<"         # Applying Transformation"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Applying rigid transformation...')"<<std::endl;
      BMSAutoSegMainFile<<"         set (command_line ${ResampleVolume2Cmd} ${Case} ${OutputFile} --transformationFile ${TxtOutFile} -i bs --Reference ${GridTemplate})"<<std::endl;
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
  int flag=GetLoopIteration();
  int nbCorrected=0;
  int nbLabel=0;
  int nbRegistered=0;
  int nbTemplate=0;
  int nbtoTemplate=0;
  int nbPosterior=0;
  bool IsT1LabelEMSFile=true;

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
	
    BMSAutoSegMainFile<<"set (EMSComputed 0)"<<std::endl;
    BMSAutoSegMainFile<<"set (CaseNumber 0)"<<std::endl;
    BMSAutoSegMainFile<<"ForEach (T1Case ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;  
    BMSAutoSegMainFile<<"      echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo ('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo ( )"<<std::endl;

    SuffixIteration=iteration+1;
    SuffixIteration_2=iteration+2;

    if (GetLoop() && iteration !=0)
    {
      BMSAutoSegMainFile<<"Set(T1ImageExtension '.nrrd')"<<std::endl;
      if (GetT2Image())
	BMSAutoSegMainFile<<"Set(T2ImageExtension '.nrrd')"<<std::endl;
      if (GetPDImage())
	BMSAutoSegMainFile<<"Set(PDImageExtension '.nrrd')"<<std::endl;

      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;	
      BMSAutoSegMainFile<<"      ListDirInDir (EMSList ${T1Path}/${AutoSegDir}/ ems_"<<SuffixIteration<<")"<<std::endl;
      BMSAutoSegMainFile<<"      set (InputPath ${EMSPath})"<<std::endl;
      BMSAutoSegMainFile<<"      set  (SUFFIX EMS_"<<SuffixIteration<<")"<<std::endl;
      BMSAutoSegMainFile<<"      set (Atlas ${atlasSegLocLoop})"<<std::endl;
      BMSAutoSegMainFile<<"      set (stripEMS _stripEMS_Bias)"<<std::endl;
      if (GetLoopIteration()<9)
      {
	nbCorrected=29;
	nbLabel=26;
	nbRegistered=30;
	nbTemplate=28;
	nbtoTemplate=29;
	nbPosterior=30;
      }
      else
      {
	nbCorrected=30;
	nbLabel=27;
	nbRegistered=31;
	nbTemplate=29;
	nbtoTemplate=30;
	nbPosterior=31;
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

      BMSAutoSegMainFile<<"         set  (SUFFIX EMS)"<<std::endl;
      BMSAutoSegMainFile<<"         set (Atlas ${atlasSegLoc})"<<std::endl;
      BMSAutoSegMainFile<<"      set (stripEMS '')"<<std::endl;
      nbCorrected=27;
      nbLabel=24;
      nbRegistered=28;
      nbTemplate=26;
      nbtoTemplate=27;
      nbPosterior=28;
    }

    BMSAutoSegMainFile<<"      set (SuffixCorrected _corrected_${SUFFIX})"<<std::endl;
    BMSAutoSegMainFile<<"      set  (SuffixLabel _labels_${SUFFIX})"<<std::endl;
    BMSAutoSegMainFile<<"      set  (SuffixRegistered _registered_${SUFFIX})"<<std::endl;
    BMSAutoSegMainFile<<"      set  (SuffixTemplate _template_${SUFFIX})"<<std::endl;

    BMSAutoSegMainFile<<"      # Creating EMS Directory if necessary"<<std::endl;
    BMSAutoSegMainFile<<"      If (${EMSList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory(${EMSPath})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${EMSList})"<<std::endl;

    BMSAutoSegMainFile<<"      set (T1InputCase ${InputPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}${T1ImageExtension})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1InputCaseHead ${T1InputCase} NAME_WITHOUT_EXTENSION)"<<std::endl; 
	
    if (GetT2Image())
    {
      BMSAutoSegMainFile<<"      If (${T2CasesList} != '')"<<std::endl;
      BMSAutoSegMainFile<<"         GetParam (T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"         GetFilename(T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"         set (T2InputCase ${InputPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}${T2ImageExtension})"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${T2CasesList})"<<std::endl;
    }
	
    if (GetPDImage())
    {
      BMSAutoSegMainFile<<"      If (${PDCasesList} != '')"<<std::endl;
      BMSAutoSegMainFile<<"         GetParam (PDCase ${PDCasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"         GetFilename(PDCaseHead ${PDCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"         set (PDInputCase ${InputPath}${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${stripEMS}${PDImageExtension})"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${PDCasesList})"<<std::endl;
    }
	
    BMSAutoSegMainFile<<"         # Setting parameters"<<std::endl;
    if (std::strcmp(GetEMSoftware(), "ABC") == 0)
      BMSAutoSegMainFile<<"         set (EMSfile ${EMSPath}ABCparam.xml)"<<std::endl;
    else
      BMSAutoSegMainFile<<"         set (EMSfile ${EMSPath}EMSparam.xml)"<<std::endl;

    BMSAutoSegMainFile<<"         ListFileInDir(EMSFileList ${EMSPath} ${T1InputCaseHead}${SuffixCorrected}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"         If (${EMSFileList} == '')"<<std::endl;  
    BMSAutoSegMainFile<<"           set (EMSComputed 1)"<<std::endl;
	
    BMSAutoSegMainFile<<"            # create EMS file"<<std::endl;
    BMSAutoSegMainFile<<"	       WriteFile (${EMSfile} '<?xml version=\"1.0\"?>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<!DOCTYPE SEGMENTATION-PARAMETERS>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<SEGMENTATION-PARAMETERS>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<SUFFIX>'${SUFFIX}'</SUFFIX>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-DIRECTORY>'${Atlas}'</ATLAS-DIRECTORY>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<OUTPUT-DIRECTORY>'${EMSPath}'</OUTPUT-DIRECTORY>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<OUTPUT-FORMAT>Nrrd</OUTPUT-FORMAT>\\n')"<<std::endl;
	
    bool IsT1LabelEMSFile;
    if ( (std::strcmp(GetTissueSegmentationAtlasType(),"T2") == 0) && (GetT2Image()) )
    {
      IsT1LabelEMSFile = false;
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
      IsT1LabelEMSFile = true;
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
	
    if (std::strcmp(GetEMSoftware(), "ABC") == 0)
    {
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior1()<<"</PRIOR>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior2()<<"</PRIOR>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior3()<<"</PRIOR>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior4()<<"</PRIOR>\\n')"<<std::endl;
    }
    else if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
      {
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior1()<<"</PRIOR>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior2()<<"</PRIOR>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior3()<<"</PRIOR>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior4()<<"</PRIOR>\\n')"<<std::endl;
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR>"<<GetPrior5()<<"</PRIOR>\\n')"<<std::endl;
      }
    else
    {
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR-1>"<<GetPrior1()<<"</PRIOR-1>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR-2>"<<GetPrior2()<<"</PRIOR-2>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR-3>"<<GetPrior3()<<"</PRIOR-3>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<PRIOR-4>"<<GetPrior4()<<"</PRIOR-4>\\n')"<<std::endl;
    }
	
    if(std::strcmp(GetEMSoftware(), "ABC") == 0)
    {
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<INITIAL-DISTRIBUTION-ESTIMATOR>"<<GetInitialDistributionEstimator()<<"</INITIAL-DISTRIBUTION-ESTIMATOR>\\n')"<<std::endl;
      if (GetFluidAtlasFATW() && GetLoop() && iteration !=0)
      {
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<DO-ATLAS-WARP>"<<GetFluidAtlasFATW()<<"</DO-ATLAS-WARP>\\n')"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<DO-ATLAS-WARP>"<<GetFluidAtlasWarp()<<"</DO-ATLAS-WARP>\\n')"<<std::endl;
      }
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-WARP-FLUID-ITERATIONS>"<<GetFluidAtlasWarpIterations()<<"</ATLAS-WARP-FLUID-ITERATIONS>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-WARP-FLUID-MAX-STEP>"<<GetFluidAtlasWarpMaxStep()<<"</ATLAS-WARP-FLUID-MAX-STEP>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<ATLAS-LINEAR-MAP-TYPE>"<<GetAtlasLinearMapping()<<"</ATLAS-LINEAR-MAP-TYPE>\\n')"<<std::endl;
      BMSAutoSegMainFile<<"	       AppendFile(${EMSfile} '<IMAGE-LINEAR-MAP-TYPE>"<<GetImageLinearMapping()<<"</IMAGE-LINEAR-MAP-TYPE>\\n')"<<std::endl;
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
	
	// BMSAutoSegMainFile<<"               SetApp(ABCCmd @ABC)"<<std::endl;
	// 	  BMSAutoSegMainFile<<"               SetAppOption(ABCCmd.ParameterFile ${EMSfile})"<<std::endl;
	// 	  BMSAutoSegMainFile<<"               Run (output ${ABCCmd})"<<std::endl;
      BMSAutoSegMainFile<<"               Run (output '${ABCCmd} ${EMSfile}')"<<std::endl;	  
    }
    else if (std::strcmp(GetEMSoftware(), "neoseg") == 0)
    {
      BMSAutoSegMainFile<<"               Run (output '${neosegCmd} ${EMSfile}')"<<std::endl;
    }
    else
    {
      std::cout<<"Error EMSoftware"<<std::endl;
      exit(-2);
    }   

    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WRITING MRML FILE...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;

    BMSAutoSegMainFile<<"set (OutputFile ${EMSPath}${T1InputCaseHead}${SuffixCorrected}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"set (OutputFileTail ${T1InputCaseHead}${SuffixCorrected}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"set (LabelFile ${EMSPath}${T1InputCaseHead}${SuffixLabel}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"set (LabelTail ${T1InputCaseHead}${SuffixLabel}.nrrd)"<<std::endl;

    BMSAutoSegMainFile<<"set (MRMLPath ${T1Path}/${AutoSegDir}/MRMLScene/${T1CaseHead}_MRMLScene/)"<<std::endl;
    BMSAutoSegMainFile<<"set (MRMLScene ${MRMLPath}${T1CaseHead}_MRMLScene.mrml)"<<std::endl;

    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
    if (GetLoop() && iteration !=0)
    {
      BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode6\"  name=\"ems_"<<SuffixIteration<<"\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    }
    else
    {
      BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode4\"  name=\"ems\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    }
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode7\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode7\"    activeLabelVolumeID=\"vtkMRMLScalarVolumeNode8\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"2\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"1\"    guiPanelLR=\"0\"    collapseSliceControllers=\"0\"\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    numberOfCompareViewRows=\"1\"    numberOfCompareViewColumns=\"1\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"    selectedModule=\"Volumes\"  ></Layout>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"-99.5 -112 99.5\"  ></Crosshair>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"199 199.672 1\"    dimensions=\"296 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -99.5 0 0 1 -112 0 1 0 99.5 0 0 0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode7\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode8\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"0\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"222.49 223.998 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -99.5 0 1 0 -112 0 0 1 99.5 0 0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode7\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode8\"    compositing=\"0\"    labelOpacity=\"0.5\"    linkedControl=\"0\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"224 225.519 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"0 0 1 -99.5 -1 0 0 -112 0 1 0 99.5 0 0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode7\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode8\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"0\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View1\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    visibility=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Default Scene Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"0 500 0\"    focalPoint=\"0 0 0\"    viewUp=\"0 0 1\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode1\"    name=\"vtkMRMLScalarVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode2\"    name=\"vtkMRMLScalarVolumeDisplayNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"118\"    level=\"63\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode3\"    name=\"vtkMRMLScalarVolumeDisplayNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"766.13\"    level=\"383.065\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode4\"    name=\"vtkMRMLScalarVolumeDisplayNode4\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"794.457\"    level=\"379.982\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode5\"    name=\"vtkMRMLScalarVolumeDisplayNode5\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeWarmShade1\"     window=\"794.457\"    level=\"379.982\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode6\"    name=\"vtkMRMLScalarVolumeDisplayNode6\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeCoolShade1\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode7\"    name=\"vtkMRMLVolumeArchetypeStorageNode7\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${OutputFile}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"0\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode7\"    name=\"${OutputFileTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode7\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode7\"    ijkToRASDirections=\"-1   0   0 -0   -1   0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"-0 -0 -0\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode7\"    name=\"vtkMRMLScalarVolumeDisplayNode7\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"26952\"    level=\"13476\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode8\"    name=\"vtkMRMLVolumeArchetypeStorageNode8\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${LabelFile}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"0\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode8\"    name=\"${LabelTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode8\"    userTags=\"\"    displayNodeRef=\"vtkMRMLLabelMapVolumeDisplayNode1\"    ijkToRASDirections=\"-1   0   0 -0   -1   0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"-0 -0 -0\"    labelMap=\"1\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <LabelMapVolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLabelMapVolumeDisplayNode1\"    name=\"vtkMRMLLabelMapVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeFileGenericColors.txt\"   ></LabelMapVolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;
	
    BMSAutoSegMainFile<<"         Else ()"<<std::endl;
    BMSAutoSegMainFile<<"            echo ('EMS Segmentation already Done!')"<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${EMSfileList})"<<std::endl;
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

    if (GetLoop() && iteration!=0)
    {
      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
      BMSAutoSegMainFile<<"         set  (SUFFIX EMS_"<<SuffixIteration<<")"<<std::endl;
      BMSAutoSegMainFile<<"         set (SuffixCorrected _stripEMS_Bias_corrected_${SUFFIX})"<<std::endl;
      BMSAutoSegMainFile<<"         set (SuffixLabel _stripEMS_Bias_labels_${SUFFIX})"<<std::endl;
      if(iteration==flag)
      {
	BMSAutoSegMainFile<<"      ListDirInDir (StrippedList ${T1Path}/${AutoSegDir}/ Stripped)"<<std::endl;
	BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/Stripped/)"<<std::endl;

      }
      else if (iteration>=1)
      {
	BMSAutoSegMainFile<<"      set (stripEMS _stripEMS_Bias)"<<std::endl;
	BMSAutoSegMainFile<<"      ListDirInDir (StrippedList ${T1Path}/${AutoSegDir}/ ems_"<<SuffixIteration_2<<")"<<std::endl;
	BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration_2<<"/)"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"      set (stripEMS _stripEMS)"<<std::endl;
	BMSAutoSegMainFile<<"      ListDirInDir (StrippedList ${T1Path}/${AutoSegDir}/ ems_"<<SuffixIteration_2<<")"<<std::endl;
	BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration_2<<"/)"<<std::endl;
      }
    }
    else
    {
      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
      BMSAutoSegMainFile<<"         set  (SUFFIX EMS)"<<std::endl;
      BMSAutoSegMainFile<<"         set (SuffixCorrected _corrected_${SUFFIX})"<<std::endl;
      BMSAutoSegMainFile<<"         set (SuffixLabel _labels_${SUFFIX})"<<std::endl;
      if (GetLoop())
      {
	BMSAutoSegMainFile<<"      set (stripEMS _stripEMS)"<<std::endl;
	BMSAutoSegMainFile<<"      ListDirInDir (StrippedList ${T1Path}/${AutoSegDir}/ ems_"<<SuffixIteration_2<<")"<<std::endl;
	BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration_2<<"/)"<<std::endl;
      }
      else	
      {
	BMSAutoSegMainFile<<"      ListDirInDir (StrippedList ${T1Path}/${AutoSegDir}/ Stripped)"<<std::endl;
	BMSAutoSegMainFile<<"      set (StrippedPath ${T1Path}/${AutoSegDir}/Stripped/)"<<std::endl;	

      }
    }

    BMSAutoSegMainFile<<"      # Creating EMS Directory if necessary"<<std::endl;
    BMSAutoSegMainFile<<"      If (${StrippedList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory(${StrippedPath})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${StrippedList})"<<std::endl;


    BMSAutoSegMainFile<<"      ListFileInDir(FinalTargetList ${StrippedPath} ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;

    BMSAutoSegMainFile<<"      If (${FinalTargetList} == '')"<<std::endl;
	
    if (IsT1LabelEMSFile)
      BMSAutoSegMainFile<<"        set (SegmentedCase ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl; 

	
    else
    {
      BMSAutoSegMainFile<<"          GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"          GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"          set (SegmentedCase ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
    }
    BMSAutoSegMainFile<<"        GetFilename (SegmentedCaseHead ${SegmentedCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
	

    BMSAutoSegMainFile<<"            set (TmpMaskTail ${SegmentedCaseHead}tmp1.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"            set (FinalMaskTail ${SegmentedCaseHead}_mask.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"            set (CurrentCaseTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"            set (FinalTargetTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;

    BMSAutoSegMainFile<<"            set (CurrentCase ${EMSPath}${CurrentCaseTail})"<<std::endl;
    BMSAutoSegMainFile<<"            set (FinalTarget ${StrippedPath}${FinalTargetTail})"<<std::endl;
    BMSAutoSegMainFile<<"            set (TmpMask ${StrippedPath}${TmpMaskTail})"<<std::endl;
    BMSAutoSegMainFile<<"            set (FinalMask ${StrippedPath}${FinalMaskTail})"<<std::endl;
    BMSAutoSegMainFile<<"            set (FinalMaskList ${FinalMaskList}${FinalMask})"<<std::endl;

    BMSAutoSegMainFile<<"            # Creating the binary mask"<<std::endl;
	// BMSAutoSegMainFile<<"            SetApp(ImageMathCmd @ImageMath)"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.Input ${SegmentedCase})"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.ThresholdValue '1,3')"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.OutputFileName ${TmpMask})"<<std::endl;
	//   BMSAutoSegMainFile<<"            Run (output ${ImageMathCmd})"<<std::endl<<std::endl;  
    BMSAutoSegMainFile<<"            Run (output '${ImageMathCmd} ${SegmentedCase} -threshold 1,3 -outfile ${TmpMask}')"<<std::endl; 
    BMSAutoSegMainFile<<"	       #In order to make sur that the binary mask is fine (without holes and smoothed)"<<std::endl;
	
    if (GetDeleteVessels())
    {
	// BMSAutoSegMainFile<<"	       SetApp(SegPostProcessCmd @SegPostProcess)"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(SegPostProcessCmd.filename ${TmpMask})"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(SegPostProcessCmd.outfile.outfileName ${FinalMask})"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(SegPostProcessCmd.DeleteVessels 1"<<std::endl;
	//   BMSAutoSegMainFile<<"            Run (output ${SegPostProcessCmd})"<<std::endl<<std::endl;
      BMSAutoSegMainFile<<"            Run (output '${SegPostProcessCmd} ${TmpMask} ${TmpMask} ${FinalTarget} --skullstripping ${CurrentCase} --mask ${FinalMask} --deleteVessels')"<<std::endl;      
    }
    else
    {
	// BMSAutoSegMainFile<<"	       SetApp(SegPostProcessCmd @SegPostProcess)"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(SegPostProcessCmd.filename ${TmpMask})"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(SegPostProcessCmd.outfile.outfileName ${FinalMask})"<<std::endl;
	//   BMSAutoSegMainFile<<"            Run (output ${SegPostProcessCmd})"<<std::endl<<std::endl;
      BMSAutoSegMainFile<<"            Run (output '${SegPostProcessCmd} ${TmpMask} ${FinalTarget} --skullstripping ${CurrentCase} --mask ${FinalMask}')"<<std::endl;
	
    }
	
	/*	BMSAutoSegMainFile<<"	       # Masking the original case with the binary image"<<std::endl;
	// BMSAutoSegMainFile<<"	       SetApp(ImageMathCmd @ImageMath)"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.Input ${CurrentCase})"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.MaskFile ${FinalMask})"<<std::endl;
	//   BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.OutputFileName ${FinalTarget})"<<std::endl;
	//   BMSAutoSegMainFile<<"            Run (output ${ImageMathCmd})"<<std::endl<<std::endl;
    BMSAutoSegMainFile<<"            Run (output '${ImageMathCmd} ${CurrentCase} -mask ${FinalMask} -outfile ${FinalTarget}')"<<std::endl; 	*/

    if (GetT2Image())
    {
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Skull stripping T2w image...')"<<std::endl;
      BMSAutoSegMainFile<<"        GetParam (T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      ListFileInDir(T2FinalTargetList ${StrippedPath} ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${T2FinalTargetList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"            set (T2CurrentCaseTail ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"            set (T2CurrentCase ${EMSPath}${T2CurrentCaseTail})"<<std::endl;
      BMSAutoSegMainFile<<"            set (T2FinalTargetTail ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"            set (T2FinalTarget ${StrippedPath}${T2FinalTargetTail})"<<std::endl;
      BMSAutoSegMainFile<<"            Run (output '${ImageMathCmd} ${T2CurrentCase} -mask ${FinalMask} -outfile ${T2FinalTarget}')"<<std::endl; 
      BMSAutoSegMainFile<<"      Else ()"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Stripped Image already exists!')"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${T2FinalTargetList})"<<std::endl;
    }
    if (GetPDImage())
    {
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Skull stripping PDw image...')"<<std::endl;
      BMSAutoSegMainFile<<"        GetParam (PDCase ${PDCasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (PDCaseHead ${PDCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      ListFileInDir(PDFinalTargetList ${StrippedPath} ${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${PDFinalTargetList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"            set (PDCurrentCaseTail ${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"            set (PDCurrentCase ${EMSPath}${PDCurrentCaseTail})"<<std::endl;
      BMSAutoSegMainFile<<"            set (PDFinalTargetTail ${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"            set (PDFinalTarget ${StrippedPath}${PDFinalTargetTail})"<<std::endl;
      BMSAutoSegMainFile<<"            Run (output '${ImageMathCmd} ${PDCurrentCase} -mask ${FinalMask} -outfile ${PDFinalTarget}')"<<std::endl; 
      BMSAutoSegMainFile<<"      Else ()"<<std::endl;
      BMSAutoSegMainFile<<"         echo ('Stripped Image already exists!')"<<std::endl;
      BMSAutoSegMainFile<<"      EndIf (${PDFinalTargetList})"<<std::endl;
    }
    BMSAutoSegMainFile<<"            # Deleting temporary file"<<std::endl;
    BMSAutoSegMainFile<<"	    DeleteFile (${TmpMask})"<<std::endl;

    if (GetLoop() && iteration==0)
    {
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Bias field correction...')"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;

      BMSAutoSegMainFile<<"	   	Set (my_output ${StrippedPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}_Bias.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"             Set (parameters --histogramsharpening ${HistogramSharpening} --bsplinebeta ${BSplineBeta} --bsplinealpha ${BSplineAlpha} --bsplineorder ${BSplineOrder} --shrinkfactor ${ShrinkFactor} --splinedistance ${SplineDistance} --convergencethreshold ${ConvergenceThreshold} --iterations ${NbOfIterations} --meshresolution ${BSplineGridResolutions})"<<std::endl;
      BMSAutoSegMainFile<<"      	Set (command_line ${N4Cmd} --outputimage ${my_output} --inputimage ${FinalTarget} ${parameters})"<<std::endl;
      BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;	
	
      if (GetT2Image())
      {
	BMSAutoSegMainFile<<"	   	Set (my_output ${StrippedPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}_Bias.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"             Set (parameters --histogramsharpening ${HistogramSharpening} --bsplinebeta ${BSplineBeta} --bsplinealpha ${BSplineAlpha} --bsplineorder ${BSplineOrder} --shrinkfactor ${ShrinkFactor} --splinedistance ${SplineDistance} --convergencethreshold ${ConvergenceThreshold} --iterations ${NbOfIterations} --meshresolution ${BSplineGridResolutions})"<<std::endl;
	BMSAutoSegMainFile<<"      	Set (command_line ${N4Cmd} --outputimage ${my_output} --inputimage ${T2FinalTarget} ${parameters})"<<std::endl;
	BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;	
      }
      if (GetPDImage())
      {
	BMSAutoSegMainFile<<"	   	Set (my_output ${StrippedPath}${PDCaseHead}${ProcessExtension}${PDRegistrationExtension}${stripEMS}_Bias.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"             Set (parameters --histogramsharpening ${HistogramSharpening} --bsplinebeta ${BSplineBeta} --bsplinealpha ${BSplineAlpha} --bsplineorder ${BSplineOrder} --shrinkfactor ${ShrinkFactor} --splinedistance ${SplineDistance} --convergencethreshold ${ConvergenceThreshold} --iterations ${NbOfIterations} --meshresolution ${BSplineGridResolutions})"<<std::endl;
	BMSAutoSegMainFile<<"      	Set (command_line ${N4Cmd} --outputimage ${my_output} --inputimage ${PDFinalTarget} ${parameters})"<<std::endl;
	BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;
      }		
    }

    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WRITING MRML FILE...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;

    BMSAutoSegMainFile<<"set (MRMLPath ${T1Path}/${AutoSegDir}/MRMLScene/${T1CaseHead}_MRMLScene/)"<<std::endl;
    BMSAutoSegMainFile<<"set (MRMLScene ${MRMLPath}${T1CaseHead}_MRMLScene.mrml)"<<std::endl;

    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
    if (GetLoop() && iteration !=0)
    {
      BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode5\"  name=\"stripped_"<<SuffixIteration<<"\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    }
    else
    {
      BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode7\"  name=\"stripped\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    }
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode10\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode10\"    activeLabelVolumeID=\"vtkMRMLScalarVolumeNode8\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"2\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"1\"    guiPanelLR=\"0\"    collapseSliceControllers=\"0\"\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    numberOfCompareViewRows=\"1\"    numberOfCompareViewColumns=\"1\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"    selectedModule=\"Volumes\"  ></Layout>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"-99.5 -112 99.5\"  ></Crosshair>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"199 199.672 1\"    dimensions=\"296 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -99.5 0 0 1 -112 0 1 0 99.5 0 0 0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode10\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"222.49 223.998 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -99.5 0 1 0 -112 0 0 1 99.5 0 0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode10\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"0.5\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"224 225.519 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"0 0 1 -99.5 -1 0 0 -112 0 1 0 99.5 0 0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode10\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View1\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    visibility=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Default Scene Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"0 500 0\"    focalPoint=\"0 0 0\"    viewUp=\"0 0 1\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode1\"    name=\"vtkMRMLScalarVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode2\"    name=\"vtkMRMLScalarVolumeDisplayNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"118\"    level=\"63\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode3\"    name=\"vtkMRMLScalarVolumeDisplayNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"766.13\"    level=\"383.065\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode4\"    name=\"vtkMRMLScalarVolumeDisplayNode4\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"794.457\"    level=\"379.982\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode5\"    name=\"vtkMRMLScalarVolumeDisplayNode5\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeWarmShade1\"     window=\"794.457\"    level=\"379.982\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode6\"    name=\"vtkMRMLScalarVolumeDisplayNode6\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeCoolShade1\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode7\"    name=\"vtkMRMLScalarVolumeDisplayNode7\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"26952\"    level=\"13476\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <LabelMapVolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLabelMapVolumeDisplayNode1\"    name=\"vtkMRMLLabelMapVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeFileGenericColors.txt\"   ></LabelMapVolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode8\"    name=\"vtkMRMLScalarVolumeDisplayNode8\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"106\"    level=\"146\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode10\"    name=\"vtkMRMLVolumeArchetypeStorageNode10\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${FinalTarget}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"0\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode10\"    name=\"${FinalTargetTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode10\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode9\"    ijkToRASDirections=\"-1   0   0 -0   -1   0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"-0 -0 -0\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode9\"    name=\"vtkMRMLScalarVolumeDisplayNode9\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"24196\"    level=\"20593\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Stripped Image already exists!')"<<std::endl;
    if (GetT2Image())
    {
      BMSAutoSegMainFile<<"        GetParam (T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      ListFileInDir(T2FinalTargetList ${StrippedPath} ${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      If (${T2FinalTargetList} != '')"<<std::endl;
      BMSAutoSegMainFile<<"         echo( )"<<std::endl;
      BMSAutoSegMainFile<<"         echo('Case Number: '${T2CaseHead})"<<std::endl;
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


  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"# 7. Intensity Calibration"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
  
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
  BMSAutoSegMainFile<<"      set (OutputFile ${StrippedPath}${StrippedCaseHead}-irescaled.nrrd)"<<std::endl;
  BMSAutoSegMainFile<<"      set (OutputFileTail ${StrippedCaseHead}-irescaled.nrrd)"<<std::endl;
  BMSAutoSegMainFile<<"      ListFileInDir(OutputFileList ${StrippedPath} ${StrippedCaseHead}-irescaled.nrrd)"<<std::endl;
  BMSAutoSegMainFile<<"      If (${OutputFileList} == '')"<<std::endl;

  // Intensity rescaling based on histogram quantile matching (ImageMath)
  if (GetIntensityRescalingMethod() == 1)
  {
    
      // BMSAutoSegMainFile<<"          SetApp(ImageMathCmd @ImageMath)"<<std::endl;
    //   BMSAutoSegMainFile<<"          SetAppOption(ImageMathCmd.Input ${StrippedCase})"<<std::endl;
    //   BMSAutoSegMainFile<<"          SetAppOption(ImageMathCmd.MatchHistogramFile ${atlasROIFile})"<<std::endl;
    //   BMSAutoSegMainFile<<"          SetAppOption(ImageMathCmd.OutputFileName ${OutputFile})"<<std::endl;
    //   BMSAutoSegMainFile<<"          Run (output ${ImageMathCmd})"<<std::endl;
    BMSAutoSegMainFile<<"          Run (output '${ImageMathCmd} ${StrippedCase} -matchHistogram ${atlasROIFile} -outfile ${OutputFile}')"<<std::endl;
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
    BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'OutputSuffix=-irescaled\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} '# OutputDir: Output Directory\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	           AppendFile(${IntRescalerParameterFile} 'OutputDir='${StrippedPath}'\\n')"<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${IntRescalerParameterFileList})"<<std::endl;
      
      // BMSAutoSegMainFile<<"	        SetApp(IntensityRescalerCmd @IntensityRescaler)"<<std::endl;
      // BMSAutoSegMainFile<<"              SetAppOption(IntensityRescalerCmd.ParameterFile ${IntRescalerParameterFile})"<<std::endl;
      // BMSAutoSegMainFile<<"              SetAppOption(IntensityRescalerCmd.Verbose 1)"<<std::endl;               
      // BMSAutoSegMainFile<<"              Run (output ${IntensityRescalerCmd})"<<std::endl;
    BMSAutoSegMainFile<<"              Run (output '${IntensityRescalerCmd} -i ${IntRescalerParameterFile} -v')"<<std::endl;
  }
  BMSAutoSegMainFile<<"          If(${IsAtlasROIFileZipped} == 1)"<<std::endl;
  BMSAutoSegMainFile<<"             Run (output 'gzip ${atlasROIFile}')"<<std::endl;
  BMSAutoSegMainFile<<"             set (atlasROIFile ${atlasROIFile}.gz)"<<std::endl;
  BMSAutoSegMainFile<<"          Endif(${IsAtlasROIFileZipped})"<<std::endl;

  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"echo ('WRITING MRML FILE...')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;

  BMSAutoSegMainFile<<"set (MRMLPath ${T1Path}/${AutoSegDir}/MRMLScene/${T1CaseHead}_MRMLScene/)"<<std::endl;
  BMSAutoSegMainFile<<"set (MRMLScene ${MRMLPath}${T1CaseHead}_MRMLScene.mrml)"<<std::endl;

  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode8\"  name=\"stripped_irescaled\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode10\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode10\"    activeLabelVolumeID=\"vtkMRMLScalarVolumeNode8\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"2\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"1\"    guiPanelLR=\"0\"    collapseSliceControllers=\"0\"\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    numberOfCompareViewRows=\"1\"    numberOfCompareViewColumns=\"1\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"    selectedModule=\"Volumes\"  ></Layout>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"-99.5 -112 99.5\"  ></Crosshair>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"199 199.672 1\"    dimensions=\"296 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -99.5 0 0 1 -112 0 1 0 99.5 0 0 0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode9\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"222.49 223.998 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -99.5 0 1 0 -112 0 0 1 99.5 0 0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode9\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"0.5\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"224 225.519 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"0 0 1 -99.5 -1 0 0 -112 0 1 0 99.5 0 0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode9\"    foregroundVolumeID=\"\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View1\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    visibility=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Default Scene Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"0 500 0\"    focalPoint=\"0 0 0\"    viewUp=\"0 0 1\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode1\"    name=\"vtkMRMLScalarVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode2\"    name=\"vtkMRMLScalarVolumeDisplayNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"118\"    level=\"63\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode3\"    name=\"vtkMRMLScalarVolumeDisplayNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"766.13\"    level=\"383.065\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode4\"    name=\"vtkMRMLScalarVolumeDisplayNode4\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"794.457\"    level=\"379.982\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode5\"    name=\"vtkMRMLScalarVolumeDisplayNode5\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeWarmShade1\"     window=\"794.457\"    level=\"379.982\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode6\"    name=\"vtkMRMLScalarVolumeDisplayNode6\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeCoolShade1\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode7\"    name=\"vtkMRMLScalarVolumeDisplayNode7\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"26952\"    level=\"13476\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <LabelMapVolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLabelMapVolumeDisplayNode1\"    name=\"vtkMRMLLabelMapVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeFileGenericColors.txt\"   ></LabelMapVolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode9\"    name=\"vtkMRMLVolumeArchetypeStorageNode9\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${OutputFile}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"0\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode9\"    name=\"${OutputFileTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode9\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode8\"    ijkToRASDirections=\"-1   0   0 -0   -1   0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"-0 -0 -0\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode8\"    name=\"vtkMRMLScalarVolumeDisplayNode8\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"106\"    level=\"146\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode9\"    name=\"vtkMRMLScalarVolumeDisplayNode9\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"24196\"    level=\"20593\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;

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
  BMSAutoSegMainFile<<"# 8. Affine Registration : warping the atlas to the skull stripped image"<<std::endl;
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
  BMSAutoSegMainFile<<"      set (StrippedCaseTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled.nrrd)"<<std::endl;
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
  BMSAutoSegMainFile<<"    	set (command_line ${BRAINSFitCmd} --fixedVolume ${StrippedCase} --movingVolume ${atlasROIFile} --useRigid --useAffine --initializeTransformMode useCenterOfHeadAlign --outputVolume ${OutputFile} --outputTransform ${TransformInputFile} --interpolationMode BSpline --outputVolumePixelType short)"<<std::endl;
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
  BMSAutoSegMainFile<<"      set (AtlasAffRegCase ${WarpROIPath}AtlasAffReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled.nrrd)"<<std::endl;
  BMSAutoSegMainFile<<"      GetFilename (AtlasAffRegCaseHead ${AtlasAffRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
  
  BMSAutoSegMainFile<<"      ListFileInDir(OutputFileList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled.nrrd)"<<std::endl;
  BMSAutoSegMainFile<<"      If (${OutputFileList} == '')"<<std::endl;
  BMSAutoSegMainFile<<"         # Creating new Files"<<std::endl;
  BMSAutoSegMainFile<<"          # Output File"<<std::endl;
  BMSAutoSegMainFile<<"         set (OutputFileHead AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled)"<<std::endl;
  BMSAutoSegMainFile<<"         set (OutputFileTail ${OutputFileHead}.nrrd)"<<std::endl;
  BMSAutoSegMainFile<<"         set (OutputFile ${WarpROIPath}${OutputFileTail})"<<std::endl;
  BMSAutoSegMainFile<<"          # Report File (process)"<<std::endl;
  BMSAutoSegMainFile<<"         set (ReportFile ${WarpROIPath}${OutputFileHead}_out.txt)"<<std::endl;

  if (GetClassicWarpingMethod())
  {
    BMSAutoSegMainFile<<"         ListFileInDir (SkullStrippedList ${T1Path}/${AutoSegDir}/Stripped/ ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled.nrrd)"<<std::endl;
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

      // BMSAutoSegMainFile<<"            SetApp(warptoolCmd @WarpTool)"<<std::endl;
      //   BMSAutoSegMainFile<<"            SetAppOption(warptoolCmd.Source ${AtlasAffRegCase})"<<std::endl;
      //   BMSAutoSegMainFile<<"            SetAppOption(warptoolCmd.Target ${SkullStrippedFile})"<<std::endl;
      //   BMSAutoSegMainFile<<"            SetAppOption(warptoolCmd.DofFile ${DofOutWarpFile})"<<std::endl;
      //   BMSAutoSegMainFile<<"            SetAppOption(warptoolCmd.Alpha ${Alpha})"<<std::endl;
      //   BMSAutoSegMainFile<<"            SetAppOption(warptoolCmd.Beta ${Beta})"<<std::endl;
      //   BMSAutoSegMainFile<<"            SetAppOption(warptoolCmd.Gamma ${Gamma})"<<std::endl;
      //   BMSAutoSegMainFile<<"            SetAppOption(warptoolCmd.MaxPerturbation ${MaxPerturbation})"<<std::endl;
      //   BMSAutoSegMainFile<<"            SetAppOption(warptoolCmd.NumIteration ${NumIteration})"<<std::endl;
      //   BMSAutoSegMainFile<<"            SetAppOption(warptoolCmd.NumBasis ${NumBasis})"<<std::endl;
      //   BMSAutoSegMainFile<<"            Run (output ${warptoolCmd})"<<std::endl;
    BMSAutoSegMainFile<<"            Run (output '${warpCmd} -c ${AtlasAffRegCase} ${SkullStrippedFile} ${DofOutWarpFile} ${Alpha} ${Beta} ${Gamma} ${MaxPerturbation} ${NumIteration} ${NumBasis}')"<<std::endl;
    BMSAutoSegMainFile<<"            WriteFile(${ReportFile} ${output})"<<std::endl<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${DofOutWarpFileList})"<<std::endl;

    BMSAutoSegMainFile<<"         # Applying Transformation"<<std::endl;
    BMSAutoSegMainFile<<"         # Transforming all volumes, by default uses cspline interpolation"<<std::endl;
    BMSAutoSegMainFile<<"            echo ('Applying transformation...')"<<std::endl;

      // BMSAutoSegMainFile<<"         SetApp(warptoolCmd @WarpTool)"<<std::endl;
      //   BMSAutoSegMainFile<<"         SetAppOption(warptoolCmd.SourceImage ${AtlasAffRegCase})"<<std::endl;         
      //   BMSAutoSegMainFile<<"         SetAppOption(warptoolCmd.HFieldFile ${DofOutWarpFile})"<<std::endl; 
      //   BMSAutoSegMainFile<<"         SetAppOption(warptoolCmd.WarpedFile ${OutputFile})"<<std::endl;
      //   BMSAutoSegMainFile<<"         SetAppOption(warptoolCmd.Trilinear 1)"<<std::endl;
      //   BMSAutoSegMainFile<<"         Run (output ${warptoolCmd})"<<std::endl;
    BMSAutoSegMainFile<<"         Run (output '${warpCmd} -a ${AtlasAffRegCase} ${DofOutWarpFile} ${OutputFile} -linear')"<<std::endl;
    BMSAutoSegMainFile<<"         AppendFile(${ReportFile} ${output})"<<std::endl<<std::endl;

    BMSAutoSegMainFile<<"         Run (output 'gzip ${DofOutWarpFile}')"<<std::endl;
  }
  else if (GetBRAINSDemonWarpMethod())
  {
    BMSAutoSegMainFile<<"            ListFileInDir (SkullStrippedList ${T1Path}/${AutoSegDir}/Stripped/ ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled.nrrd)"<<std::endl;
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
  else
  {
    BMSAutoSegMainFile<<"         set (MHDOutputFileTail ${OutputFileHead}2.mhd)"<<std::endl;
    BMSAutoSegMainFile<<"         set (RAWOutputFileTail ${OutputFileHead}2.raw)"<<std::endl;
    BMSAutoSegMainFile<<"         set (RAW0OutputFileTail ${OutputFileHead}0.raw)"<<std::endl;
    BMSAutoSegMainFile<<"         set (RAW1OutputFileTail ${OutputFileHead}1.raw)"<<std::endl;
    BMSAutoSegMainFile<<"         set (ZippedRAWOutputFileTail ${RAWOutputFileTail}.gz)"<<std::endl;
    BMSAutoSegMainFile<<"         set (MHDOutputFile ${WarpROIPath}${MHDOutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         set (RAWOutputFile ${WarpROIPath}${RAWOutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         set (RAW0OutputFile ${WarpROIPath}${RAW0OutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         set (RAW1OutputFile ${WarpROIPath}${RAW1OutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         set (ZippedRAWOutputFile ${WarpROIPath}${ZippedRAWOutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         ListFileInDir(MHDOutputFileList ${WarpROIPath} ${MHDOutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         If (${MHDOutputFileList} == '')"<<std::endl;

    BMSAutoSegMainFile<<"            ListFileInDir (SkullStrippedList ${T1Path}/${AutoSegDir}/Stripped/ ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled.nrrd)"<<std::endl;
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
    BMSAutoSegMainFile<<"            Run (output '${fWarpCmd} --outputImageFilenamePrefix=${WarpROIPath}${OutputFileHead} --outputHFieldFilenamePrefix=${DofOutWarpFile} --scaleLevel=4 --numberOfIterations=${Scale4NbIterations} --scaleLevel=2 --numberOfIterations=${Scale2NbIterations} --scaleLevel=1 --numberOfIterations=${Scale1NbIterations} --alpha=${Alpha} --beta=${Beta} --gamma=${Gamma} --maxPerturbation=${MaxPerturbation} ${SkullStrippedFile} ${AtlasAffRegCase}')"<<std::endl;
    BMSAutoSegMainFile<<"            WriteFile(${ReportFile} ${output})"<<std::endl<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${MHDOutputFileList})"<<std::endl;

    BMSAutoSegMainFile<<"         Run (output '${ImageMathCmd} ${MHDOutputFile} -constOper 2,1000 -outfile ${OutputFile}')"<<std::endl;
  }


  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"echo ('WRITING MRML FILE...')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;

  BMSAutoSegMainFile<<"set (MRMLPath ${T1Path}/${AutoSegDir}/MRMLScene/${T1CaseHead}_MRMLScene/)"<<std::endl;
  BMSAutoSegMainFile<<"set (MRMLScene ${MRMLPath}${T1CaseHead}_MRMLScene.mrml)"<<std::endl;

  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode9\"  name=\"warp\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode12\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode12\"    activeLabelVolumeID=\"vtkMRMLScalarVolumeNode8\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"2\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"1\"    guiPanelLR=\"0\"    collapseSliceControllers=\"0\"\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    numberOfCompareViewRows=\"1\"    numberOfCompareViewColumns=\"1\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"    selectedModule=\"Volumes\"  ></Layout>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"0 0 1.42109e-14\"  ></Crosshair>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"199 199.672 1\"    dimensions=\"296 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 0 0 0 1 0 0 1 0 0 0 0 0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode12\"    foregroundVolumeID=\"vtkMRMLScalarVolumeNode6\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"222.49 223.998 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode12\"    foregroundVolumeID=\"vtkMRMLScalarVolumeNode6\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"0.5\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"224 225.519 1\"    dimensions=\"295 297 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"0 0 1 0 -1 0 0 0 0 1 0 0 0 0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode12\"    foregroundVolumeID=\"vtkMRMLScalarVolumeNode6\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View1\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    visibility=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Default Scene Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"0 500 0\"    focalPoint=\"0 0 0\"    viewUp=\"0 0 1\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode1\"    name=\"vtkMRMLVolumeArchetypeStorageNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${AtlasIsoTemplate}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode1\"    name=\"vtkMRMLScalarVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode2\"    name=\"vtkMRMLScalarVolumeDisplayNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"118\"    level=\"63\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode3\"    name=\"vtkMRMLScalarVolumeDisplayNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"766.13\"    level=\"383.065\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode4\"    name=\"vtkMRMLScalarVolumeDisplayNode4\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"794.457\"    level=\"379.982\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode5\"    name=\"vtkMRMLScalarVolumeDisplayNode5\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeWarmShade1\"     window=\"794.457\"    level=\"379.982\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode6\"    name=\"vtkMRMLVolumeArchetypeStorageNode6\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${AtlasIsoTemplate}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode6\"    name=\"${AtlasIsoTemplate_name}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode6\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode6\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"99.5 112 -99.5\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode6\"    name=\"vtkMRMLScalarVolumeDisplayNode6\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeCoolShade1\"     window=\"904\"    level=\"532\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode7\"    name=\"vtkMRMLScalarVolumeDisplayNode7\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"26952\"    level=\"13476\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <LabelMapVolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLabelMapVolumeDisplayNode1\"    name=\"vtkMRMLLabelMapVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeFileGenericColors.txt\"   ></LabelMapVolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode8\"    name=\"vtkMRMLScalarVolumeDisplayNode8\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"106\"    level=\"146\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode9\"    name=\"vtkMRMLScalarVolumeDisplayNode9\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"24196\"    level=\"20593\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode11\"    name=\"vtkMRMLVolumeArchetypeStorageNode11\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${OutputFile}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode10\"    name=\"vtkMRMLScalarVolumeDisplayNode10\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"110\"    level=\"144\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode12\"    name=\"vtkMRMLVolumeArchetypeStorageNode12\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${OutputFile}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode12\"    name=\"${OutputFileTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode12\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode11\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"99.5 112 -99.5\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode11\"    name=\"vtkMRMLScalarVolumeDisplayNode11\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeWarmShade1\"     window=\"110\"    level=\"144\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;

  BMSAutoSegMainFile<<"      Else ()"<<std::endl;
  BMSAutoSegMainFile<<"         echo ('Warping already Done!)'"<<std::endl;
  BMSAutoSegMainFile<<"      EndIf (${OutputFileList})"<<std::endl;

  BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl;
  BMSAutoSegMainFile<<"echo ('WARPING COMPUTATION: DONE!')"<<std::endl;
  BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;


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

  bool IsStructureListEmpty = true;
  bool IsVentricleListEmpty = true;
  bool IsGenericROIMapListEmpty = true;
  bool IsParcellationMapListEmpty = true;
  bool IsLabelListEmpty = true;

  int StructureListLength = 0;
  int VentricleListLength = 0;
  int GenericROIMapListLength = 0;
  int ParcellationMapListLength = 0;

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
    BMSAutoSegMainFile<<"      ListFileInDir(ZippedHFieldList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled_warp.hfield.gz)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${ZippedHFieldList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"         set (ZippedHFieldFileTail ${ZippedHFieldList})"<<std::endl;
    BMSAutoSegMainFile<<"         set (ZippedHFieldFile ${WarpROIPath}${ZippedHFieldFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         ExtractString (HFieldFile ${ZippedHFieldFile} 3 FROMEND)"<<std::endl;
    BMSAutoSegMainFile<<"         set (IsHFieldFileZipped 1)"<<std::endl;
  }
  else if (GetBRAINSDemonWarpMethod())
  {
    BMSAutoSegMainFile<<"      ListFileInDir(NrrdList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled_transform.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${NrrdList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"         set (NrrdFileTail ${NrrdList})"<<std::endl;
    BMSAutoSegMainFile<<"         set (NrrdFile ${WarpROIPath}${NrrdFileTail})"<<std::endl;
  }
  else
  {
    BMSAutoSegMainFile<<"      ListFileInDir(MHDFieldList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled_deformationfield.mhd)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${MHDFieldList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"         set (MHDFieldFileTail ${MHDFieldList})"<<std::endl;
    BMSAutoSegMainFile<<"         set (MHDFieldFile ${WarpROIPath}${MHDFieldFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"         ListFileInDir(ZippedRAWFieldList ${WarpROIPath} AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled_deformationfield.raw.gz)"<<std::endl;
    BMSAutoSegMainFile<<"         If (${ZippedRAWFieldList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"            set (IsRAWFieldFileZipped 1)"<<std::endl; 
    BMSAutoSegMainFile<<"            set (ZippedRAWFieldFile ${WarpROIPath}${ZippedRAWFieldList})"<<std::endl;
    BMSAutoSegMainFile<<"            ExtractString (RAWFieldFile ${ZippedRAWFieldFile} 3 FROMEND)"<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${ZippedRAWFieldList})"<<std::endl;
  }

  BMSAutoSegMainFile<<"         ListFileInDir(SkullStrippedList ${T1Path}/${AutoSegDir}/Stripped/ ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled.nrrd)"<<std::endl;
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
    BMSAutoSegMainFile<<"	          set (StructureAff ${WarpROIPath}${StructureAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"	          set (StructureWarp ${WarpROIPath}${StructureWarpTail})"<<std::endl;
    BMSAutoSegMainFile<<"           # Applying Affine transformation"<<std::endl;
    BMSAutoSegMainFile<<"           ListFileInDir(AffFileList ${WarpROIPath} ${StructureAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"                echo (${AffFileList})"<<std::endl;
    BMSAutoSegMainFile<<"           If (${AffFileList} == '')"<<std::endl;

    if (!GetBRAINSDemonWarpMethod())
    {
      BMSAutoSegMainFile<<"              set (TransformInputFile ${WarpROIPath}AtlasAffReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled_initializetransform.txt)"<<std::endl; 
      BMSAutoSegMainFile<<"              set (command_line ${ResampleVolume2Cmd} ${Structure} ${StructureAff} --transformationFile ${TransformInputFile} -i bs --Reference ${SkullStrippedFile})"<<std::endl;
      BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;
    }
    BMSAutoSegMainFile<<"	          Else ()"<<std::endl;
    BMSAutoSegMainFile<<"              echo ('File already exists: '${StructureAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"           EndIf (${AffFileList})"<<std::endl;
    BMSAutoSegMainFile<<"	          # Applying Warping transformation"<<std::endl;
    BMSAutoSegMainFile<<"           ListFileInDir(WarpFileList ${WarpROIPath} ${StructureWarpTail})"<<std::endl;
    BMSAutoSegMainFile<<"           If (${WarpFileList} == '')"<<std::endl;

    if (GetClassicWarpingMethod())
    {
      BMSAutoSegMainFile<<"              If (${IsHFieldFileZipped} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"                 # Unzipping HField File "<<std::endl;
      BMSAutoSegMainFile<<"                 Run (output 'gunzip -f ${ZippedHFieldFile}')"<<std::endl;
      BMSAutoSegMainFile<<"                 set(IsHFieldFileZipped 0)"<<std::endl;
      BMSAutoSegMainFile<<"              EndIf (${IsHFieldFileZipped})"<<std::endl;
	  //      BMSAutoSegMainFile<<"           SetApp(warptoolCmd @WarpTool)"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"           SetAppOption(warptoolCmd.SourceImage ${StructureAff}) "<<std::endl;
	  // 	  BMSAutoSegMainFile<<"           SetAppOption(warptoolCmd.HFieldFile ${HFieldFile}) "<<std::endl;
	  // 	  BMSAutoSegMainFile<<"           SetAppOption(warptoolCmd.WarpedFile ${StructureWarp})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"           SetAppOption(warptoolCmd.Trilinear 1)"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"           Run (output ${warptoolCmd})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output '${warpCmd} -a ${StructureAff} ${HFieldFile} ${StructureWarp} -linear')"<<std::endl;
    }
    else if (GetBRAINSDemonWarpMethod())
    {
      BMSAutoSegMainFile<<"              set (command_line ${ResampleVolume2Cmd} ${Structure} ${StructureWarp} -H ${NrrdFile} -i linear --hfieldtype displacement --Reference ${SkullStrippedFile})"<<std::endl;
      BMSAutoSegMainFile<<"      	   Run (prog_output ${command_line} prog_error)"<<std::endl;
    }
    else
    {
      BMSAutoSegMainFile<<"	      ListFileInDir(MHDWarpFileList ${WarpROIPath} ${StructureWarpHead}.mhd)"<<std::endl;
      BMSAutoSegMainFile<<"           If (${MHDWarpFileList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"              If (${IsRAWFieldFileZipped} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"	             # Unzipping deformation field"<<std::endl;
      BMSAutoSegMainFile<<"                  Run (output 'gunzip ${ZippedRAWFieldFile}')"<<std::endl;
      BMSAutoSegMainFile<<"                  set(IsRAWFieldFileZipped 0)"<<std::endl;
      BMSAutoSegMainFile<<"	         EndIf (${IsRAWFieldFileZipped})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output '${txApplyCmd} -h ${MHDFieldFile} -b -i ${StructureAff} -o ${WarpROIPath}${StructureWarpHead}')"<<std::endl;
      BMSAutoSegMainFile<<"           Else()"<<std::endl;
      BMSAutoSegMainFile<<"	         ListFileInDir(ZippedRAWFileList ${WarpROIPath} ${StructureWarpHead}.raw.gz)"<<std::endl;
      BMSAutoSegMainFile<<"              If (${ZippedRAWFileList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"                  Run (output 'gunzip ${WarpROIPath}${StructureWarpHead}.raw.gz')"<<std::endl;
      BMSAutoSegMainFile<<"	         EndIf (${ZippedRAWFileList})"<<std::endl;
      BMSAutoSegMainFile<<"           EndIf (${MHDWarpFileList)"<<std::endl;
      BMSAutoSegMainFile<<"           Run (output '${convCmd} ${WarpROIPath}${StructureWarpHead}.mhd ${StructureWarp}')"<<std::endl;
      BMSAutoSegMainFile<<"           Run (output 'gzip ${WarpROIPath}${StructureWarpHead}.raw')"<<std::endl;
	  	  
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
    BMSAutoSegMainFile<<"	          echo ('Registering '${LabelName}...)"<<std::endl;
    BMSAutoSegMainFile<<"           Inc (${LabelNumber} 1)"<<std::endl;
    BMSAutoSegMainFile<<"	          Int(${LabelNumber})"<<std::endl;
  
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

    BMSAutoSegMainFile<<"	          set (LabelAff ${WarpROIPath}${LabelAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"	          set (LabelWarp ${WarpROIPath}${LabelWarpTail})"<<std::endl;
    BMSAutoSegMainFile<<"           # Applying Affine transformation"<<std::endl;
    BMSAutoSegMainFile<<"           ListFileInDir(AffFileList ${WarpROIPath} ${LabelAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"           If (${AffFileList} == '')"<<std::endl;

    if (!GetBRAINSDemonWarpMethod())
    {
      BMSAutoSegMainFile<<"              set (TransformInputFile ${WarpROIPath}AtlasAffReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled_initializetransform.txt)"<<std::endl; 
      BMSAutoSegMainFile<<"              set (command_line ${ResampleVolume2Cmd} ${Label} ${LabelAff} --transformationFile ${TransformInputFile} -i nn --Reference ${SkullStrippedFile})"<<std::endl;
      BMSAutoSegMainFile<<"      	Run (prog_output ${command_line} prog_error)"<<std::endl;
    }

    BMSAutoSegMainFile<<"	          Else ()"<<std::endl;
    BMSAutoSegMainFile<<"              echo ('File already exists: '${LabelAffTail})"<<std::endl;
    BMSAutoSegMainFile<<"           EndIf (${AffFileList})"<<std::endl;

    BMSAutoSegMainFile<<"               set (flag 0)"<<std::endl;	  
    BMSAutoSegMainFile<<"	          # Applying Warping transformation"<<std::endl;
    BMSAutoSegMainFile<<"           ListFileInDir(WarpFileList ${WarpROIPath} ${LabelWarpTail})"<<std::endl;
    BMSAutoSegMainFile<<"           If (${WarpFileList} == '')"<<std::endl;	  
    BMSAutoSegMainFile<<"               set (flag 1)"<<std::endl;	  

    if (GetClassicWarpingMethod())
    {
      BMSAutoSegMainFile<<"              If (${IsHFieldFileZipped} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"                 # Unzipping HField File "<<std::endl;
      BMSAutoSegMainFile<<"                 Run (output 'gunzip -f ${ZippedHFieldFile}')"<<std::endl;
      BMSAutoSegMainFile<<"                 set(IsHFieldFileZipped 0)"<<std::endl;
      BMSAutoSegMainFile<<"              EndIf (${IsHFieldFileZipped})"<<std::endl;
	  //      BMSAutoSegMainFile<<"           SetApp(warptoolCmd @WarpTool)"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"           SetAppOption(warptoolCmd.SourceImage ${LabelAff}) "<<std::endl;        
	  // 	  BMSAutoSegMainFile<<"           SetAppOption(warptoolCmd.HFieldFile ${HFieldFile}) "<<std::endl;
	  // 	  BMSAutoSegMainFile<<"           SetAppOption(warptoolCmd.WarpedFile ${LabelWarp})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"           SetAppOption(warptoolCmd.Nearest 1)"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"           Run (output ${warptoolCmd})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output '${warpCmd} -a ${LabelAff} ${HFieldFile} ${LabelWarp} -nearest')"<<std::endl;
    }
    else if (GetBRAINSDemonWarpMethod())
    {
      BMSAutoSegMainFile<<"              set (command_line ${ResampleVolume2Cmd} ${Label} ${LabelWarp} -H ${NrrdFile} -i nn --hfieldtype displacement --Reference ${SkullStrippedFile})"<<std::endl;
      BMSAutoSegMainFile<<"      	   Run (prog_output ${command_line} prog_error)"<<std::endl;
    }
    else
    {
      BMSAutoSegMainFile<<"	      ListFileInDir(MHDWarpFileList ${WarpROIPath} ${LabelWarpHead}.mhd)"<<std::endl;
      BMSAutoSegMainFile<<"           If (${MHDWarpFileList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"              If (${IsRAWFieldFileZipped} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"	             # Unzipping deformation field"<<std::endl;
      BMSAutoSegMainFile<<"                  Run (output 'gunzip ${ZippedRAWFieldFile}')"<<std::endl;
      BMSAutoSegMainFile<<"                  set(IsRAWFieldFileZipped 0)"<<std::endl;
      BMSAutoSegMainFile<<"	         EndIf (${IsRAWFieldFileZipped})"<<std::endl;
      BMSAutoSegMainFile<<"              Run (output '${txApplyCmd} -h ${MHDFieldFile} -b -i ${LabelAff} -o ${WarpROIPath}${LabelWarpHead} -n')"<<std::endl;
      BMSAutoSegMainFile<<"           Else()"<<std::endl;
      BMSAutoSegMainFile<<"	         ListFileInDir(ZippedRAWFileList ${WarpROIPath} ${LabelWarpHead}.raw.gz)"<<std::endl;
      BMSAutoSegMainFile<<"              If (${ZippedRAWFileList} == '')"<<std::endl;
      BMSAutoSegMainFile<<"                  Run (output 'gunzip ${WarpROIPath}${LabelWarpHead}.raw.gz')"<<std::endl;
      BMSAutoSegMainFile<<"	         EndIf (${ZippedRAWFileList})"<<std::endl;
      BMSAutoSegMainFile<<"           EndIf (${MHDWarpFileList)"<<std::endl;
      BMSAutoSegMainFile<<"           Run (output '${convCmd} ${WarpROIPath}${LabelWarpHead}.mhd ${LabelWarp}')"<<std::endl;
      BMSAutoSegMainFile<<"           Run (output 'gzip ${WarpROIPath}${LabelWarpHead}.raw')"<<std::endl;
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
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WRITING MRML FILE...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;

    BMSAutoSegMainFile<<"   set (StripIrescaledTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"   set (StripIrescaled ${T1Path}/${AutoSegDir}/Stripped/${StripIrescaledTail})"<<std::endl;
    BMSAutoSegMainFile<<"   set (Parcellation ${WarpROIPath}${ParcellationTail} )"<<std::endl;	
    BMSAutoSegMainFile<<"set (MRMLPath ${T1Path}/${AutoSegDir}/MRMLScene/${T1CaseHead}_MRMLScene/)"<<std::endl;
    BMSAutoSegMainFile<<"set (MRMLScene ${MRMLPath}${T1CaseHead}_MRMLScene.mrml)"<<std::endl;

    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode10\"  name=\"warp_parcellation\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode1\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode1\"    activeLabelVolumeID=\"vtkMRMLScalarVolumeNode3\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <Layout\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"2\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"1\"    guiPanelLR=\"0\"    collapseSliceControllers=\"0\"\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    numberOfCompareViewRows=\"1\"    numberOfCompareViewColumns=\"1\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"  ></Layout>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"0 0 0\"  ></Crosshair>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"199 204.103 1\"    dimensions=\"312 320 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 0 0 0 1 0 0 1 0 0 0 0 0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode3\"    compositing=\"0\"    labelOpacity=\"0.44\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"218.4 224 1\"    dimensions=\"312 320 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode3\"    compositing=\"0\"    labelOpacity=\"0.44\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"224 229.744 1\"    dimensions=\"312 320 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"0 0 1 0 -1 0 0 0 0 1 0 0 0 0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode3\"    compositing=\"0\"    labelOpacity=\"0.44\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View2\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    visibility=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Default Scene Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"0 500 0\"    focalPoint=\"0 0 0\"    viewUp=\"0 0 1\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode1\"    name=\"vtkMRMLVolumeArchetypeStorageNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${StripIrescaled}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode1\"    name=\"${StripIrescaledTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode1\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode1\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"99.5 112 -99.5\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode1\"    name=\"vtkMRMLScalarVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"108\"    level=\"145\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <LabelMapVolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLabelMapVolumeDisplayNode1\"    name=\"vtkMRMLLabelMapVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeFileGenericColors.txt\"   ></LabelMapVolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode3\"    name=\"vtkMRMLVolumeArchetypeStorageNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${Parcellation}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode3\"    name=\"${ParcellationTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode3\"    userTags=\"\"    displayNodeRef=\"vtkMRMLLabelMapVolumeDisplayNode2\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"99.5 112 -99.5\"    labelMap=\"1\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <LabelMapVolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLabelMapVolumeDisplayNode2\"    name=\"vtkMRMLLabelMapVolumeDisplayNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeFileGenericColors.txt\"   ></LabelMapVolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;
  }
  if ( IsStructureListEmpty && IsVentricleListEmpty )
  {
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</MRML>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"      ListDirInDir (MRMLList ${ProcessingDir} AutoSeg_MRML)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${MRMLList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory( ${ProcessingDir}AutoSeg_MRML)"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${MRMLList})"<<std::endl;
    BMSAutoSegMainFile<<"   Set (command_line cp ${MRMLPath}${T1CaseHead}_MRMLScene.mrml ${ProcessingDir}AutoSeg_MRML/)"<<std::endl;
    BMSAutoSegMainFile<<"   Run (prog_output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WRITING STRUCTURE MRML FILE: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;	
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
    BMSAutoSegMainFile<<"              Run (output 'gzip -f ${HFieldFile}')"<<std::endl;
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
  else
  {
    BMSAutoSegMainFile<<"           If (${IsRAWFieldFileZipped} == 0)"<<std::endl;  
    BMSAutoSegMainFile<<"               # Zipping deformationfield File "<<std::endl;
    BMSAutoSegMainFile<<"               Run (output 'gzip -f ${RAWFieldFile}')"<<std::endl;
    BMSAutoSegMainFile<<"           EndIf (${IsHFieldFileZipped})"<<std::endl;
    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Error:No deformationfield file!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${MHDFieldList})"<<std::endl;
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
	BMSAutoSegMainFile<<"           set(SuffixPosterior _stripEMS_Bias_posterior2_${SUFFIX})"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
	BMSAutoSegMainFile<<"           set(SuffixPosterior _posterior2_${SUFFIX})"<<std::endl;
      }
      if (IsT1LabelEMSFile)
	BMSAutoSegMainFile<<"           set (CSFProbMap ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
      else
      {
	BMSAutoSegMainFile<<"             GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	BMSAutoSegMainFile<<"             GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegMainFile<<"           set (CSFProbMap ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
      }  

	  // BMSAutoSegMainFile<<"         SetApp(ImageMathCmd @ImageMath)"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.Input ${CSFProbMap})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.MaskFile ${LatVentRight})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.OutputFileName ${TmpFile})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         Run (output ${ImageMathCmd})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (output '${ImageMathCmd} ${CSFProbMap} -mask ${LatVentRight} -outfile ${TmpFile}')"<<std::endl;
      BMSAutoSegMainFile<<"         #And then rescale it to 256"<<std::endl;
	  // BMSAutoSegMainFile<<"	    SetApp(ImageMathCmd @ImageMath)"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.Input ${TmpFile})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.ConstOperValue '3,'${coeff})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.OutputFileName ${LatVentRightOutput})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         Run (output ${ImageMathCmd})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (output '${ImageMathCmd} ${TmpFile} -constOper 3,${coeff} -outfile ${LatVentRightOutput}')"<<std::endl;
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
	BMSAutoSegMainFile<<"           set(SuffixPosterior _stripEMS_Bias_posterior2_${SUFFIX})"<<std::endl;
      }
      else
      {
	BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
	BMSAutoSegMainFile<<"           set(SuffixPosterior _posterior2_${SUFFIX})"<<std::endl;
      }
      if (IsT1LabelEMSFile)
	BMSAutoSegMainFile<<"           set (CSFProbMap ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
      else
      {
	BMSAutoSegMainFile<<"             GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	BMSAutoSegMainFile<<"             GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	BMSAutoSegMainFile<<"           set (CSFProbMap ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
      }	  

	  // BMSAutoSegMainFile<<"         SetApp(ImageMathCmd @ImageMath)"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.Input ${CSFProbMap})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.MaskFile ${LatVentLeft})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.OutputFileName ${TmpFile})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         Run (output ${ImageMathCmd})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (output '${ImageMathCmd} ${CSFProbMap} -mask ${LatVentLeft} -outfile ${TmpFile}')"<<std::endl;
      BMSAutoSegMainFile<<"         #And then rescale it to 256"<<std::endl;
	  // BMSAutoSegMainFile<<"	    SetApp(ImageMathCmd @ImageMath)"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.Input ${TmpFile})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.ConstOperValue '3,${coeff})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         SetAppOption(ImageMathCmd.OutputFileName ${LatVentLeftOutput})"<<std::endl;
// 	  BMSAutoSegMainFile<<"         Run (output ${ImageMathCmd})"<<std::endl;
      BMSAutoSegMainFile<<"         Run (output '${ImageMathCmd} ${TmpFile} -constOper 3,${coeff} -outfile ${LatVentLeftOutput}')"<<std::endl;
      BMSAutoSegMainFile<<"         # Deleting temporary files "<<std::endl;
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
      // BMSAutoSegMainFile<<"	      SetApp(ImageMathCmd @ImageMath)"<<std::endl;
      //       BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.Input ${File})"<<std::endl;
      //       BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.ThresholdValue '127,256')"<<std::endl;
      //       BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.OutputFileName ${OutputFile})"<<std::endl;
      //       BMSAutoSegMainFile<<"            Run (output ${ImageMathCmd})"<<std::endl;
    BMSAutoSegMainFile<<"            Run (output '${ImageMathCmd} ${File} -threshold 127,256 -outfile ${OutputFile}')"<<std::endl;
    BMSAutoSegMainFile<<"            #Labeling ${OutputFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"            echo ('Labeling '${OutputFileTail}...)"<<std::endl;
    BMSAutoSegMainFile<<"            GetParam(LabelValue ${ProbaLabelList} ${ProbaFileNumber})"<<std::endl;
      // BMSAutoSegMainFile<<"	      SetApp(ImageMathCmd @ImageMath)"<<std::endl;
      //       BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.Input ${OutputFile})"<<std::endl;
      //       BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.ConstOperValue '2,'${LabelValue})"<<std::endl;
      //       BMSAutoSegMainFile<<"            SetAppOption(ImageMathCmd.OutputFileName ${OutputFile})"<<std::endl;
      //       BMSAutoSegMainFile<<"            Run (output ${ImageMathCmd})"<<std::endl;
    BMSAutoSegMainFile<<"            Run (output '${ImageMathCmd} ${OutputFile} -constOper 2,${LabelValue} -outfile ${OutputFile}')"<<std::endl;
      
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
    BMSAutoSegMainFile<<"   set (AllROIFileTail AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled-AllROI.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"   set (AllROIFile ${WarpROIPath}${AllROIFileTail})"<<std::endl;
  
    BMSAutoSegMainFile<<"      ListFileInDir(AllROIList ${WarpROIPath} ${AllROIFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"      If (${AllROIList} == '' || ${StructureComputed} == 1 || ${VentricleComputed} == 1)"<<std::endl;
    BMSAutoSegMainFile<<"         set (FileNumber 1)"<<std::endl;
    BMSAutoSegMainFile<<"         ListFileInDir(ROIList ${WarpROIPath} ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--*-WarpReg-HardSeg.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"         If (${ROIList} != '')"<<std::endl;
    BMSAutoSegMainFile<<"            ForEach(ROIFileTail ${ROIList})"<<std::endl;
    BMSAutoSegMainFile<<"               set (ROIFile ${WarpROIPath}${ROIFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"               If (${FileNumber} == 1)"<<std::endl;
    BMSAutoSegMainFile<<"                  Run (output 'cp ${ROIFile} ${AllROIFile}')"<<std::endl;
    BMSAutoSegMainFile<<"               Else ()"<<std::endl;
    BMSAutoSegMainFile<<"                  echo ('Adding '${ROIFileTail}...)"<<std::endl;
      // BMSAutoSegMainFile<<"                  SetApp(ImageMathCmd @ImageMath)"<<std::endl;
      //       BMSAutoSegMainFile<<"                  SetAppOption(ImageMathCmd.Input ${AllROIFile})"<<std::endl;
      //       BMSAutoSegMainFile<<"                  SetAppOption(ImageMathCmd.CombineFile ${ROIFile})"<<std::endl;
      //       BMSAutoSegMainFile<<"                  SetAppOption(ImageMathCmd.OutputFileName ${AllROIFile})"<<std::endl;
      //       BMSAutoSegMainFile<<"                  Run (output ${ImageMathCmd})"<<std::endl;
    BMSAutoSegMainFile<<"                  Run (output '${ImageMathCmd} ${AllROIFile} -combine ${ROIFile} -outfile ${AllROIFile}')"<<std::endl; 
    BMSAutoSegMainFile<<"               EndIf (${FileNumber})"<<std::endl;
    BMSAutoSegMainFile<<"               Inc (${FileNumber} 1)"<<std::endl;
    BMSAutoSegMainFile<<"               Int (${FileNumber})"<<std::endl;
    BMSAutoSegMainFile<<"            EndForEach (ROIFile)"<<std::endl;
    BMSAutoSegMainFile<<"         EndIf (${ROIList})"<<std::endl;

    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WRITING MRML FILE...')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;

    BMSAutoSegMainFile<<"   set (StripIrescaledTail ${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled.nrrd)"<<std::endl;
    BMSAutoSegMainFile<<"   set (StripIrescaled ${T1Path}/${AutoSegDir}/Stripped/${StripIrescaledTail})"<<std::endl;
    BMSAutoSegMainFile<<"   set (MRMLPath ${T1Path}/${AutoSegDir}/MRMLScene/${T1CaseHead}_MRMLScene/)"<<std::endl;
    BMSAutoSegMainFile<<"   set (MRMLScene ${MRMLPath}${T1CaseHead}_MRMLScene.mrml)"<<std::endl;

    BMSAutoSegMainFile<<"	Set(tmpMRMLFile ${MRMLPath}/tmp.mrml)"<<std::endl;
    BMSAutoSegMainFile<<"	WriteFile(${tmpMRMLFile} '<MRML >\\n')"<<std::endl;
    BMSAutoSegMainFile<<"	AppendFile(${tmpMRMLFile} '</MRML>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   Set (command_line ${ModelMakerCmd} ${AllROIFile} --generateAll --modelSceneFile ${tmpMRMLFile})"<<std::endl;
    BMSAutoSegMainFile<<"   Run (prog_output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"	DeleteFile(${tmpMRMLFile})"<<std::endl;

    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode11\"  name=\"warp_ROI\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode1\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode1\"    activeLabelVolumeID=\"vtkMRMLScalarVolumeNode2\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"2\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"1\"    guiPanelLR=\"0\"    collapseSliceControllers=\"0\"\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    numberOfCompareViewRows=\"1\"    numberOfCompareViewColumns=\"1\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"    selectedModule=\"Models\"  ></Layout>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"-99.5 -112 99.5\"  ></Crosshair>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"199 204.103 1\"    dimensions=\"312 320 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -99.5 0 0 1 -112 0 1 0 99.5 0 0 0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode2\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"218.4 224 1\"    dimensions=\"312 320 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 0 -99.5 0 1 0 -112 0 0 1 99.5 0 0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode2\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"224 229.744 1\"    dimensions=\"312 320 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"0 0 1 -99.5 -1 0 0 -112 0 1 0 99.5 0 0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode1\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode2\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ClipModels\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLClipModelsNode1\"    name=\"vtkMRMLClipModelsNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    clipType=\"0\"    redSliceClipState=\"0\"    yellowSliceClipState=\"0\"    greenSliceClipState=\"0\"  ></ClipModels>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode1\"    name=\"vtkMRMLModelHierarchyNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    displayNodeRef=\"vtkMRMLModelDisplayNode1\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode1\"    name=\"vtkMRMLModelDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.392157 0.392157 0.509804\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeLabels\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode1\"    name=\"vtkMRMLModelStorageNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_1.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode2\"    name=\"vtkMRMLModelDisplayNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.501961 0.682353 0.501961\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode1\"    name=\"Model_1\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode1\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode2\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode2\"    name=\"vtkMRMLModelHierarchyNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode1\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode2\"    name=\"vtkMRMLModelStorageNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_2.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode3\"    name=\"vtkMRMLModelDisplayNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.945098 0.839216 0.568627\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode2\"    name=\"Model_2\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode2\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode3\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode3\"    name=\"vtkMRMLModelHierarchyNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode2\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode3\"    name=\"vtkMRMLModelStorageNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_3.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode4\"    name=\"vtkMRMLModelDisplayNode4\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.694118 0.478431 0.396078\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode3\"    name=\"Model_3\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode3\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode4\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode4\"    name=\"vtkMRMLModelHierarchyNode4\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode3\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode4\"    name=\"vtkMRMLModelStorageNode4\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_4.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode5\"    name=\"vtkMRMLModelDisplayNode5\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.435294 0.721569 0.823529\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode4\"    name=\"Model_4\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode4\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode5\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode5\"    name=\"vtkMRMLModelHierarchyNode5\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode4\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode5\"    name=\"vtkMRMLModelStorageNode5\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_5.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode6\"    name=\"vtkMRMLModelDisplayNode6\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.847059 0.396078 0.309804\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode5\"    name=\"Model_5\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode5\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode6\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode6\"    name=\"vtkMRMLModelHierarchyNode6\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode5\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode6\"    name=\"vtkMRMLModelStorageNode6\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_6.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode7\"    name=\"vtkMRMLModelDisplayNode7\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.866667 0.509804 0.396078\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode6\"    name=\"Model_6\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode6\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode7\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode7\"    name=\"vtkMRMLModelHierarchyNode7\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode6\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode7\"    name=\"vtkMRMLModelStorageNode7\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_7.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode8\"    name=\"vtkMRMLModelDisplayNode8\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.564706 0.933333 0.564706\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode7\"    name=\"Model_7\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode7\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode8\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode8\"    name=\"vtkMRMLModelHierarchyNode8\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode7\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode8\"    name=\"vtkMRMLModelStorageNode8\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_8.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode9\"    name=\"vtkMRMLModelDisplayNode9\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.752941 0.407843 0.345098\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode8\"    name=\"Model_8\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode8\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode9\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode9\"    name=\"vtkMRMLModelHierarchyNode9\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode8\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode9\"    name=\"vtkMRMLModelStorageNode9\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_9.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode10\"    name=\"vtkMRMLModelDisplayNode10\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.862745 0.960784 0.0784314\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode9\"    name=\"Model_9\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode9\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode10\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode10\"    name=\"vtkMRMLModelHierarchyNode10\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode9\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode10\"    name=\"vtkMRMLModelStorageNode10\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_10.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode11\"    name=\"vtkMRMLModelDisplayNode11\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.305882 0.247059 0\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode10\"    name=\"Model_10\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode10\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode11\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode11\"    name=\"vtkMRMLModelHierarchyNode11\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode10\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode11\"    name=\"vtkMRMLModelStorageNode11\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_11.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode12\"    name=\"vtkMRMLModelDisplayNode12\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"1 0.980392 0.862745\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode11\"    name=\"Model_11\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode11\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode12\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode12\"    name=\"vtkMRMLModelHierarchyNode12\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode11\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelStorageNode12\"    name=\"vtkMRMLModelStorageNode12\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"Model_12.vtk\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"  ></ModelStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelDisplayNode13\"    name=\"vtkMRMLModelDisplayNode13\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.392157 0.392157 0.509804\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    activeScalarName=\"\"   ></ModelDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Model\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelNode12\"    name=\"Model_12\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLModelStorageNode12\"    userTags=\"\"    displayNodeRef=\"vtkMRMLModelDisplayNode13\"  ></Model>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <ModelHierarchy\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLModelHierarchyNode13\"    name=\"vtkMRMLModelHierarchyNode13\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parentNodeRef=\"vtkMRMLModelHierarchyNode1\"    modelNodeRef=\"vtkMRMLModelNode12\"    expanded=\"true\"  ></ModelHierarchy>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View2\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    visibility=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Default Scene Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"0 500 0\"    focalPoint=\"0 0 0\"    viewUp=\"0 0 1\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode1\"    name=\"vtkMRMLVolumeArchetypeStorageNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${StripIrescaled}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"0\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode1\"    name=\"${StripIrescaledTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode1\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode1\"    ijkToRASDirections=\"-1   0   0 -0   -1   0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"-0 -0 -0\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode1\"    name=\"vtkMRMLScalarVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"108\"    level=\"145\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode2\"    name=\"vtkMRMLVolumeArchetypeStorageNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${AllROIFile}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"0\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode2\"    name=\"${AllROIFileTail}\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode2\"    userTags=\"\"    displayNodeRef=\"vtkMRMLLabelMapVolumeDisplayNode1\"    ijkToRASDirections=\"-1   0   0 -0   -1   0 0 -0 1 \"    spacing=\"1 1 1\"    origin=\"-0 -0 -0\"    labelMap=\"1\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '  <LabelMapVolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '    id=\"vtkMRMLLabelMapVolumeDisplayNode1\"    name=\"vtkMRMLLabelMapVolumeDisplayNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeFileGenericColors.txt\"   ></LabelMapVolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${MRMLScene} '</MRML>\\n')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('WRITING STRUCTURE MRML FILE: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;	
    BMSAutoSegMainFile<<"      ListDirInDir (MRMLList ${ProcessingDir} AutoSeg_MRML)"<<std::endl;
    BMSAutoSegMainFile<<"      If (${MRMLList} == '')"<<std::endl;
    BMSAutoSegMainFile<<"         MakeDirectory( ${ProcessingDir}AutoSeg_MRML)"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${MRMLList})"<<std::endl;
    BMSAutoSegMainFile<<"   Set (command_line cp ${MRMLPath}${T1CaseHead}_MRMLScene.mrml ${ProcessingDir}AutoSeg_MRML/)"<<std::endl;
    BMSAutoSegMainFile<<"   Run (prog_output ${command_line} prog_error)"<<std::endl;
    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('File already exists: '${AllROIFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${AllROIList})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('ROI GATHERING: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
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
	  

	  // BMSAutoSegMainFile<<"            SetApp(CortThickCLPCmd @CortThickCLP)"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            SetAppOption(CortThickCLPCmd.OutputDir ${CortThickPath})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            SetAppOption(CortThickCLPCmd.InputSeg ${SegmentedCase})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            SetAppOption(CortThickCLPCmd.Parcellation ${ParcellationFile})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            SetAppOption(CortThickCLPCmd.SaveWM ${WMCortThickFile})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            SetAppOption(CortThickCLPCmd.SaveGM ${GMCortThickFile})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            Run (output ${CortThickCLPCmd})"<<std::endl;
      BMSAutoSegMainFile<<"            Run (output '${CortThickCLPCmd} ${CortThickPath} --inputSeg ${SegmentedCase} --par ${ParcellationFile} --SaveWM ${WMCortThickFile} --SaveGM ${GMCortThickFile}')"<<std::endl;
	  
      BMSAutoSegMainFile<<"            set (OutputCSVFile ${CortThickPath}${SegmentedCaseHead}-WhiteMatDistanceMap_par.csv)"<<std::endl;
      BMSAutoSegMainFile<<"            set (OutputArrayCSVFile ${CortThickPath}${SegmentedCaseHead}-WhiteMatDistanceMap_par_array.csv)"<<std::endl;
      BMSAutoSegMainFile<<"            set (NewOutputCSVFile ${CortThickPath}${SegmentedCaseHead}--${ParcellationMapName}-WhiteMatDistanceMap_par.csv)"<<std::endl;
      BMSAutoSegMainFile<<"            set (NewOutputArrayCSVFile ${CortThickPath}${SegmentedCaseHead}--${ParcellationMapName}-WhiteMatDistanceMap_par_array.csv)"<<std::endl;
      BMSAutoSegMainFile<<"            Run (output 'mv ${OutputCSVFile} ${NewOutputCSVFile}')"<<std::endl;
      BMSAutoSegMainFile<<"            Run (output 'mv ${OutputArrayCSVFile} ${NewOutputArrayCSVFile}')"<<std::endl;
      BMSAutoSegMainFile<<"         EndForEach(ParcellationMap)"<<std::endl;
    }
    else
    {
	  // BMSAutoSegMainFile<<"            SetApp(CortThickCLPCmd @CortThickCLP)"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            SetAppOption(CortThickCLPCmd.OutputDir ${CortThickPath})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            SetAppOption(CortThickCLPCmd.InputSeg ${SegmentedCase})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            SetAppOption(CortThickCLPCmd.SaveWM ${WMCortThickFile})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            SetAppOption(CortThickCLPCmd.SaveGM ${GMCortThickFile})"<<std::endl;
	  // 	  BMSAutoSegMainFile<<"            Run (output ${CortThickCLPCmd})"<<std::endl;
      BMSAutoSegMainFile<<"            Run (output '${CortThickCLPCmd} ${CortThickPath} --inputSeg ${SegmentedCase} --SaveWM ${WMCortThickFile} --SaveGM ${GMCortThickFile}')"<<std::endl;  
    }

    BMSAutoSegMainFile<<"      Else ()"<<std::endl;
    BMSAutoSegMainFile<<"         echo ('Cortical Thickness already Done!')"<<std::endl;
    BMSAutoSegMainFile<<"      EndIf (${OutputFileList})"<<std::endl;
    BMSAutoSegMainFile<<"EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('CORTICAL THICKNESS COMPUTATION: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }

  if (GetComputeVolume())
  {
    bool IsListEmpty = true;

    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"#---------------------------------------------------------------------"<<std::endl;
    BMSAutoSegMainFile<<"# 14. Volume information computation"<<std::endl;
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
    BMSAutoSegMainFile<<"ListFileInDir(FileList ${Path} ${VolumeFileTail})"<<std::endl;
    BMSAutoSegMainFile<<"If (${FileList} == '' || ${EMSComputed} == 1)"<<std::endl;

    BMSAutoSegMainFile<<"   WriteFile(${VolumeFile} 'Volume analysis in cubic mm\\n\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} '##################################\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} '# Tissue segmentation volume analysis:\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} '##################################\\n\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} 'Case,Volume,WM,GM,CSF\\n')"<<std::endl;
    BMSAutoSegMainFile<<"   set (files_to_cat '')"<<std::endl;
    BMSAutoSegMainFile<<"   set (CaseNumber 0)"<<std::endl;
    BMSAutoSegMainFile<<"   ForEach (T1Case ${T1CasesList})"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
    BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;
    BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
    BMSAutoSegMainFile<<"      echo( )"<<std::endl;	  
    BMSAutoSegMainFile<<"      set (SkullStrippedImage ${T1Path}/${AutoSegDir}/Stripped/${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;

    if(GetLoop())
      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
    else
      BMSAutoSegMainFile<<"      set (EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;

    if (IsT1LabelEMSFile)
      {
	BMSAutoSegMainFile<<"      set (SegmentedFile ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
	BMSAutoSegMainFile<<"      GetFilename (SegmentedFileHead ${SegmentedFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
      }
    else
    {
      BMSAutoSegMainFile<<"      GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      set (SegmentedFile ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixLabel}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (SegmentedFileHead ${SegmentedFile} NAME_WITHOUT_EXTENSION)"<<std::endl;
    }
    BMSAutoSegMainFile<<"      #Computing Statistics"<<std::endl;
    BMSAutoSegMainFile<<"      echo ('Computing Volume information...')"<<std::endl;
    BMSAutoSegMainFile<<"      Run (output '${ImageStatCmd} ${SkullStrippedImage} -label ${SegmentedFile} -volumeSummary -outbase ${EMSPath}${SegmentedFileHead}')"<<std::endl;	
    BMSAutoSegMainFile<<"      set (files_to_cat ${files_to_cat} ${EMSPath}${SegmentedFileHead}_volumeSummary.csv)"<<std::endl;
    
    BMSAutoSegMainFile<<"      Inc(${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMainFile<<"      Int(${CaseNumber})"<<std::endl;
    BMSAutoSegMainFile<<"   EndForEach (T1Case)"<<std::endl;
    BMSAutoSegMainFile<<"   echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"   echo ('Gathering volume information for the whole dataset...')"<<std::endl;
    BMSAutoSegMainFile<<"   Run (output 'cat ${files_to_cat}')"<<std::endl;
    BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} ${output})"<<std::endl;
    BMSAutoSegMainFile<<"Else ()"<<std::endl;
    BMSAutoSegMainFile<<"   echo ('Volume File already exists!')"<<std::endl;
    BMSAutoSegMainFile<<"EndIf(${FileList})"<<std::endl<<std::endl<<std::endl;	  
    
      
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
      BMSAutoSegMainFile<<"      set (AllROIFileTail AtlasWarpReg-${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}-irescaled-AllROI.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      set (AllROIFile ${WarpROIPath}${AllROIFileTail})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (AllROIFileHead ${AllROIFile} NAME_WITHOUT_EXTENSION)"<<std::endl;

      BMSAutoSegMainFile<<"      #Computing Statistics"<<std::endl;
      BMSAutoSegMainFile<<"      echo ('Computing Volume information...')"<<std::endl;
      BMSAutoSegMainFile<<"      set (SkullStrippedImage ${T1Path}/${AutoSegDir}/Stripped/${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"      Run (output '${ImageStatCmd} ${SkullStrippedImage} -label ${AllROIFile} -volumeSummary -outbase ${WarpROIPath}${AllROIFileHead}')"<<std::endl;	
      BMSAutoSegMainFile<<"      set (files_to_cat ${files_to_cat} ${WarpROIPath}${AllROIFileHead}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"   EndForEach (T1Case)"<<std::endl;
      BMSAutoSegMainFile<<"   echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"   echo ('Gathering volume information for the whole dataset...')"<<std::endl;
      BMSAutoSegMainFile<<"   Run (output 'cat ${files_to_cat}')"<<std::endl;
      BMSAutoSegMainFile<<"   AppendFile(${VolumeFile} ${output})"<<std::endl;
      BMSAutoSegMainFile<<"Else ()"<<std::endl;
      BMSAutoSegMainFile<<"   echo ('Volume File already exists!')"<<std::endl;
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
	  //	BMSAutoSegMainFile<<"            SetApp(ImageStatCmd @ImageStat)"<<std::endl;
	  // 	BMSAutoSegMainFile<<"            SetAppOption(ImageStatCmd.Input ${SkullStrippedImage})"<<std::endl;
	  //    BMSAutoSegMainFile<<"            SetAppOption(ImageStatCmd.LabelFile ${File})"<<std::endl;
	  // 	BMSAutoSegMainFile<<"            SetAppOption(ImageStatCmd.OutBase ${WarpROIPath}${FileHead})"<<std::endl;
	  // 	BMSAutoSegMainFile<<"            Run (output ${ImageStatCmd})"<<std::endl;
      BMSAutoSegMainFile<<"       Run (output '${ImageStatCmd} ${SkullStrippedImage} -label ${File} -volumeSummary -outbase ${WarpROIPath}${FileHead}')"<<std::endl;
      BMSAutoSegMainFile<<"       set (files_to_cat ${files_to_cat} ${WarpROIPath}${FileHead}_volumeSummary.csv)"<<std::endl;

      BMSAutoSegMainFile<<"     EndForEach (T1Case)"<<std::endl;
      BMSAutoSegMainFile<<"  echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"  echo ('Gathering volume information for the whole dataset...')"<<std::endl;
      BMSAutoSegMainFile<<"  Run (output 'cat ${files_to_cat}')"<<std::endl;
      BMSAutoSegMainFile<<"  AppendFile(${VolumeFile} ${output})"<<std::endl;
      BMSAutoSegMainFile<<"  Else ()"<<std::endl;
      BMSAutoSegMainFile<<"    echo (${VolumeFileTail} already exists!)"<<std::endl;
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
      BMSAutoSegMainFile<<" echo (Parcellation File: ${ParcellationMapName}:)"<<std::endl;
      BMSAutoSegMainFile<<" set (VolumeFileWM ${VolumeDir}AutoSeg_${ParcellationMapName}_Volume_WM.csv)"<<std::endl;
      BMSAutoSegMainFile<<" set (VolumeFileGM ${VolumeDir}AutoSeg_${ParcellationMapName}_Volume_GM.csv)"<<std::endl;
      BMSAutoSegMainFile<<" set (VolumeFileCSF ${VolumeDir}AutoSeg_${ParcellationMapName}_Volume_CSF.csv)"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(Path ${VolumeFileWM} PATH)"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(VolumeFileTail ${VolumeFileWM} NAME)"<<std::endl;
      BMSAutoSegMainFile<<" ListFileInDir(FileListWM ${Path} ${VolumeFileTail})"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(Path ${VolumeFileGM} PATH)"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(VolumeFileTail ${VolumeFileGM} NAME)"<<std::endl;
      BMSAutoSegMainFile<<" ListFileInDir(FileListGM ${Path} ${VolumeFileTail})"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(Path ${VolumeFileCSF} PATH)"<<std::endl;
      BMSAutoSegMainFile<<" GetFileName(VolumeFileTail ${VolumeFileCSF} NAME)"<<std::endl;
      BMSAutoSegMainFile<<" ListFileInDir(FileListCSF ${Path} ${VolumeFileTail})"<<std::endl;
      BMSAutoSegMainFile<<" If (${ParcellationMapComputed} == 1 || ${FileListWM} == '' || ${FileListGM} == '' || ${FileListCSF} == '')"<<std::endl;
      BMSAutoSegMainFile<<"  set (flagWM 0)"<<std::endl;
      BMSAutoSegMainFile<<"  set (flagGM 0)"<<std::endl;
      BMSAutoSegMainFile<<"  set (flagCSF 0)"<<std::endl;

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
      BMSAutoSegMainFile<<"   AppendFile(${VolumeFileCSF} '#########################################################\\n')"<<std::endl;
      BMSAutoSegMainFile<<"   AppendFile(${VolumeFileCSF} '# Parcellation map volume analysis: Cerebral Spinal Fluid\\n')"<<std::endl;
      BMSAutoSegMainFile<<"   AppendFile(${VolumeFileCSF} '#########################################################\\n\\n')"<<std::endl;
      BMSAutoSegMainFile<<"   set (files_to_cat_CSF '')"<<std::endl;
      BMSAutoSegMainFile<<"  EndIf (${FileListCSF})"<<std::endl;

      BMSAutoSegMainFile<<"   ForEach (T1Case ${T1CasesList})"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1Path ${T1Case} PATH)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1Dir ${T1Path} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      GetFilename (T1CaseHead ${T1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
      BMSAutoSegMainFile<<"      echo('Case Number: '${T1CaseHead})"<<std::endl;
      BMSAutoSegMainFile<<"      echo( )"<<std::endl;
	
      if(GetLoop())
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
      if(GetLoop())
	BMSAutoSegMainFile<<"      ExtractString(SegmentedFileHead ${SegmentedFileTail} 13 FROMEND)"<<std::endl;
      else
	BMSAutoSegMainFile<<"      ExtractString(SegmentedFileHead ${SegmentedFileTail} 11 FROMEND)"<<std::endl;
      BMSAutoSegMainFile<<"      set(LabelList 'WM' 'GM' 'CSF')"<<std::endl;
      BMSAutoSegMainFile<<"      set (SkullStrippedImage ${T1Path}/${AutoSegDir}/Stripped/${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;

      BMSAutoSegMainFile<<"      set (WarpROIPath ${T1Path}/${AutoSegDir}/WarpROI/)"<<std::endl;
      BMSAutoSegMainFile<<"         set(TissueNumber 1)"<<std::endl;		
      BMSAutoSegMainFile<<"         ForEach(Label ${LabelList})"<<std::endl;
      BMSAutoSegMainFile<<"            set (LabelFileTail ${SegmentedFileHead}_${Label}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"            set (LabelFile ${EMSPath}${LabelFileTail})"<<std::endl;
	
      BMSAutoSegMainFile<<"            set (File ${WarpROIPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixCorrected}--${ParcellationMapName}-WarpReg.nrrd )"<<std::endl;
      BMSAutoSegMainFile<<"            GetFilename (FileName ${File} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegMainFile<<"            set(FileMaskedRoot ${FileName}_${Label})"<<std::endl;
      BMSAutoSegMainFile<<"            set(FileMaskedTail ${FileMaskedRoot}.nrrd)"<<std::endl;
      BMSAutoSegMainFile<<"            set(FileMasked ${WarpROIPath}${FileMaskedTail})"<<std::endl;
      BMSAutoSegMainFile<<"            set (OutputTail ${FileMaskedRoot}_volumeSummary.csv)"<<std::endl;
      std::string SoftTissueMap =GetSoftTissueMap();
      if (SoftTissueMap=="Soft")
      {
	BMSAutoSegMainFile<<"              If (${Label} == 'WM' && ${flagWM} == 1)"<<std::endl;
	BMSAutoSegMainFile<<"           set(Posterior _posterior0)"<<std::endl;
	BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
	BMSAutoSegMainFile<<"              If (${Label} == 'GM' && ${flagGM} == 1)"<<std::endl;
	BMSAutoSegMainFile<<"           set(Posterior _posterior1)"<<std::endl;
	BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
	BMSAutoSegMainFile<<"              If (${Label} == 'CSF' && ${flagCSF} == 1)"<<std::endl;
	BMSAutoSegMainFile<<"           set(Posterior _posterior2)"<<std::endl;
	BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
	if(GetLoop())
	{
	  BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems_"<<SuffixIteration<<"/)"<<std::endl;
	  BMSAutoSegMainFile<<"           set(SuffixPosterior _stripEMS_Bias${Posterior}_${SUFFIX})"<<std::endl;
	}
	else
	{
	  BMSAutoSegMainFile<<"           set(EMSPath ${T1Path}/${AutoSegDir}/ems/)"<<std::endl;
	  BMSAutoSegMainFile<<"           set(SuffixPosterior ${Posterior}_${SUFFIX})"<<std::endl;
	}
	if (IsT1LabelEMSFile)
	  BMSAutoSegMainFile<<"           set (ProbMap ${EMSPath}${T1CaseHead}${ProcessExtension}${T1RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
	else
	{
	  BMSAutoSegMainFile<<"             GetParam(T2Case ${T2CasesList} ${CaseNumber})"<<std::endl;
	  BMSAutoSegMainFile<<"             GetFilename (T2CaseHead ${T2Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	  BMSAutoSegMainFile<<"           set (ProbMap ${EMSPath}${T2CaseHead}${ProcessExtension}${T2RegistrationExtension}${SuffixPosterior}.nrrd)"<<std::endl;
	}	
	BMSAutoSegMainFile<<"              Run(output '${ImageStatCmd} ${SkullStrippedImage} -label ${File} -probabilityMap ${ProbMap} -volumeSummary -outbase ${WarpROIPath}${FileMaskedRoot}')"<<std::endl;	
      }
      else  
      {
	BMSAutoSegMainFile<<"            ListFileInDir(List ${EMSPath} ${LabelFileTail})"<<std::endl;
	BMSAutoSegMainFile<<"            If (${List} == '')"<<std::endl;
	BMSAutoSegMainFile<<"              Run (output '${ImageMathCmd} ${SegmentedFile} -extractLabel ${TissueNumber} -outfile ${LabelFile}')"<<std::endl;
	BMSAutoSegMainFile<<"            Endif(${List})"<<std::endl;	

	BMSAutoSegMainFile<<"            ListFileInDir(List ${WarpROIPath} ${FileMaskedTail})"<<std::endl;
	BMSAutoSegMainFile<<"            If (${List} == '')"<<std::endl;
	BMSAutoSegMainFile<<"              Run(output '${ImageMathCmd} ${File} -mask ${LabelFile} -outfile ${FileMasked}')"<<std::endl;
	BMSAutoSegMainFile<<"            Else ()"<<std::endl;
	BMSAutoSegMainFile<<"              echo('File already exists: '${FileMaskedTail})"<<std::endl;
	BMSAutoSegMainFile<<"            Endif(${List})"<<std::endl;
	BMSAutoSegMainFile<<"            Run(output '${ImageStatCmd} ${SkullStrippedImage} -label ${FileMasked} -volumeSummary -outbase ${WarpROIPath}${FileMaskedRoot}')"<<std::endl;
      }
      BMSAutoSegMainFile<<"              If (${Label} == 'WM' && ${flagWM} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"                  set (files_to_cat_WM ${files_to_cat_WM} ${WarpROIPath}${FileMaskedRoot}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
      BMSAutoSegMainFile<<"              If (${Label} == 'GM' && ${flagGM} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"                  set (files_to_cat_GM ${files_to_cat_GM} ${WarpROIPath}${FileMaskedRoot}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
      BMSAutoSegMainFile<<"              If (${Label} == 'CSF' && ${flagCSF} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"                  set (files_to_cat_CSF ${files_to_cat_CSF} ${WarpROIPath}${FileMaskedRoot}_volumeSummary.csv)"<<std::endl;
      BMSAutoSegMainFile<<"              EndIf (${Label})"<<std::endl;
      BMSAutoSegMainFile<<"            echo ( )  "<<std::endl;
      BMSAutoSegMainFile<<"           Inc(${TissueNumber} 1)"<<std::endl;
      BMSAutoSegMainFile<<"           Int(${TissueNumber})"<<std::endl;      
      BMSAutoSegMainFile<<"         EndForEach(Label)"<<std::endl;
      BMSAutoSegMainFile<<"   EndForEach (T1Case)"<<std::endl;
      BMSAutoSegMainFile<<"   echo ( )"<<std::endl;
      BMSAutoSegMainFile<<"   echo ('Gathering volume information for the whole dataset...')"<<std::endl;
      BMSAutoSegMainFile<<"   If (${flagWM} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"     Run (output 'cat ${files_to_cat_WM}')"<<std::endl;	
      BMSAutoSegMainFile<<"     AppendFile(${VolumeFileWM} ${output})"<<std::endl;
      BMSAutoSegMainFile<<"   EndIf (${flagWM})"<<std::endl;
      BMSAutoSegMainFile<<"   If (${flagGM} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"     Run (output 'cat ${files_to_cat_GM}')"<<std::endl;	
      BMSAutoSegMainFile<<"     AppendFile(${VolumeFileGM} ${output})"<<std::endl;
      BMSAutoSegMainFile<<"   EndIf (${flagGM})"<<std::endl;
      BMSAutoSegMainFile<<"   If (${flagCSF} == 1)"<<std::endl;
      BMSAutoSegMainFile<<"     Run (output 'cat ${files_to_cat_CSF}')"<<std::endl;	
      BMSAutoSegMainFile<<"     AppendFile(${VolumeFileCSF} ${output})"<<std::endl;
      BMSAutoSegMainFile<<"   EndIf (${flagCSF})"<<std::endl;
      BMSAutoSegMainFile<<"  Else ()"<<std::endl;
      BMSAutoSegMainFile<<"   echo ('Volume file already exists!')"<<std::endl;
      BMSAutoSegMainFile<<"  EndIf(${ParcellationMapComputed})"<<std::endl;	  
      BMSAutoSegMainFile<<"EndForEach(ParcellationMap)"<<std::endl;
    }

    BMSAutoSegMainFile<<"echo ( )"<<std::endl;
    BMSAutoSegMainFile<<"echo ('VOLUME COMPUTATION: DONE!')"<<std::endl;
    BMSAutoSegMainFile<<"echo ( )"<<std::endl<<std::endl;
  }

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
  BMSAutoSegMainFile<<"            Run (output 'gzip -f ${UnzippedFile}')"<<std::endl;
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
  BMSAutoSegAuxFile<<"set (ResampleVolume2Cmd ResampleVolume2)"<<std::endl<<std::endl;
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

  if (GetLoop())
  {
    int SuffixIteration=GetLoopIteration()+1;
    BMSAutoSegAuxFile<<"set  (SUFFIX EMS_"<<SuffixIteration<<")"<<std::endl;
    BMSAutoSegAuxFile<<"set (SuffixCorrected _stripEMS_Bias_corrected_${SUFFIX})"<<std::endl;
  }
  else
  {
    BMSAutoSegAuxFile<<"set  (SUFFIX EMS)"<<std::endl;
    BMSAutoSegAuxFile<<"set (SuffixCorrected _stripEMS_corrected_${SUFFIX})"<<std::endl;
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
    BMSAutoSegAuxFile<<"   # Creating Auxiliairy 1 Directory if necessary"<<std::endl;
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
    BMSAutoSegAuxFile<<"   # Creating Auxiliairy 2 Directory if necessary"<<std::endl;
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
    BMSAutoSegAuxFile<<"   # Creating Auxiliairy 3 Directory if necessary"<<std::endl;
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
    BMSAutoSegAuxFile<<"   # Creating Auxiliairy 4 Directory if necessary"<<std::endl;
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
    BMSAutoSegAuxFile<<"   # Creating Auxiliairy 5 Directory if necessary"<<std::endl;
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
    BMSAutoSegAuxFile<<"   # Creating Auxiliairy 6 Directory if necessary"<<std::endl;
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
    BMSAutoSegAuxFile<<"   # Creating Auxiliairy 7 Directory if necessary"<<std::endl;
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
    BMSAutoSegAuxFile<<"   # Creating Auxiliairy 8 Directory if necessary"<<std::endl;
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
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --useRigid --initializeTransformMode useCenterOfHeadAlign --outputTransform ${TxtRegFile})"<<std::endl;
    BMSAutoSegAuxFile<<"     Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --useRigid --outputTransform ${TxtRegFile} --initialTransform ${RegPath}/${TxtinitFileTail})"<<std::endl;
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
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --useAffine --initializeTransformMode useCenterOfHeadAlign --outputTransform ${TxtRegFile})"<<std::endl;
    BMSAutoSegAuxFile<<"     Else ()"<<std::endl;
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --useAffine --outputTransform ${TxtRegFile} --initialTransform ${RegPath}/${TxtinitFileTail})"<<std::endl;
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
    BMSAutoSegAuxFile<<"      set (command_line ${BRAINSFitCmd} --fixedVolume ${Aux1Case} --movingVolume ${SourceCase} --useRigid --useAffine --initializeTransformMode useCenterOfHeadAlign --outputTransform ${TxtRegFile} --interpolationMode BSpline)"<<std::endl;
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
  BMSAutoSegAuxFile<<"# 3. Applying the transformation to the others auxiliary images"<<std::endl;
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
    BMSAutoSegAuxFile<<"      set (stripEMS _stripEMS_Bias)"<<std::endl;
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
    }	BMSAutoSegAuxFile<<"         echo (${PosteriorOrigSourceCasesHead})"<<std::endl;
    BMSAutoSegAuxFile<<"      set(WMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T2RegistrationExtension}${stripEMS}_posterior0_${SUFFIX}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"      set(GMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T2RegistrationExtension}${stripEMS}_posterior1_${SUFFIX}.nrrd)"<<std::endl;
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
    BMSAutoSegAuxFile<<"      set(WMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T1RegistrationExtension}${stripEMS}_posterior0_${SUFFIX}.nrrd)"<<std::endl;
    BMSAutoSegAuxFile<<"      set(GMProbMap ${EMSPath}${PosteriorOrigSourceCasesHead}${Bias}${T1RegistrationExtension}${stripEMS}_posterior1_${SUFFIX}.nrrd)"<<std::endl;
  }
  BMSAutoSegAuxFile<<"      GetFilename (WMProbMapHead ${WMProbMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      GetFilename (GMProbMapHead ${GMProbMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      set (OutputFileWM ${AuxPath}${WMProbMapHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      GetFilename (OutputFileWMTail ${OutputFileWM} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${OutputFileWMTail}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
  BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${WMProbMap} ${OutputFileWM} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
  BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
  BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
  BMSAutoSegAuxFile<<"         echo ('File already exists: '${OutputFileWMTail})"<<std::endl;
  BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;
  BMSAutoSegAuxFile<<"      set (OutputFileGM ${AuxPath}${GMProbMapHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      GetFilename (OutputFileGMTail ${OutputFileGM} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${OutputFileGMTail}.nrrd)"<<std::endl;
  BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
  BMSAutoSegAuxFile<<"           set (command_line ${ResampleVolume2Cmd} ${GMProbMap} ${OutputFileGM} --transformationFile ${TxtRegFile} --Reference ${AuxCase})"<<std::endl;
  BMSAutoSegAuxFile<<"      	Run (output ${command_line} prog_error)"<<std::endl;
  BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
  BMSAutoSegAuxFile<<"         echo ('File already exists: '${OutputFileGMTail})"<<std::endl;
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
      BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${WarpRegCaseHead}_WM_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
      BMSAutoSegAuxFile<<"         set(WMProbMapTr ${OutputFileWM})"<<std::endl;
      BMSAutoSegAuxFile<<"         Run (output '${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -probabilityMap ${WMProbMapTr} -outbase ${AuxPath}${WarpRegCaseHead}_WM -quantile ${Quantiles}')"<<std::endl;
      BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
      BMSAutoSegAuxFile<<"         echo ('File already exists: '${AuxPath}${WarpRegCaseHead}_WM_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;
      BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath}  ${WarpRegCaseHead}_GM_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
      BMSAutoSegAuxFile<<"         set(GMProbMapTr ${OutputFileGM})"<<std::endl;
      BMSAutoSegAuxFile<<"         Run (output '${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -probabilityMap ${GMProbMapTr} -outbase ${AuxPath}${WarpRegCaseHead}_GM -quantile ${Quantiles}')"<<std::endl;
      BMSAutoSegAuxFile<<"      Else ()"<<std::endl;
      BMSAutoSegAuxFile<<"         echo ('File already exists: '${WarpRegCaseHead}_GM_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      EndIf (${OutputList})"<<std::endl;
      BMSAutoSegAuxFile<<"      EndForEach (WarpRegCase)"<<std::endl;
    }
    else
    {
      BMSAutoSegAuxFile<<"      set(LabelList WM GM)"<<std::endl;
      BMSAutoSegAuxFile<<"      ForEach (Label ${LabelList})"<<std::endl;
      BMSAutoSegAuxFile<<"      ListFileInDir(WarpRegCasesList ${AuxPath} *${ParcellationMapNameHead}-WarpReg_${Label}_Reg_${AuxDir}.nrrd)"<<std::endl;
      BMSAutoSegAuxFile<<"      ForEach (WarpRegCase ${WarpRegCasesList})"<<std::endl;
      BMSAutoSegAuxFile<<"         GetFilename (WarpRegCaseHead ${WarpRegCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
      BMSAutoSegAuxFile<<"      ListFileInDir(OutputList ${AuxPath} ${WarpRegCaseHead}_intensitySummary.csv)"<<std::endl;
      BMSAutoSegAuxFile<<"      If (${OutputList} == '')"<<std::endl;
      BMSAutoSegAuxFile<<"         Run (output '${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -outbase ${AuxPath}${WarpRegCaseHead} -quantile ${Quantiles}')"<<std::endl;
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
    BMSAutoSegAuxFile<<"            Run (output '${ImageStatCmd} ${AuxCase} -label ${MaskOutputFile} -intensitySummary -probabilityMap ${AuxPath}${WarpRegCase} -outbase ${AuxPath}${WarpRegCaseHead} -quantile ${Quantiles}')"<<std::endl;
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
    BMSAutoSegAuxFile<<"               Run (output '${ImageStatCmd} ${AuxCase} -label ${AuxPath}${WarpRegCase} -intensitySummary -outbase ${AuxPath}${WarpRegCaseHead} -quantile ${Quantiles}')"<<std::endl;
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
  int DataNumber;
  std::ofstream BMSAutoSegMRMLSourceFile(GetBMSAutoSegMRMLSourceFile());

  BMSAutoSegMRMLSourceFile<<"#---------------------    MRML scene script   ------------------------"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"set (AutoSegDir "<<GetDataAutoSegDirectory()<<")"<<std::endl; 

  if (GetAuxT1Image())
  {
    if(!m_Manually)
    {
      BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList "<<m_T1List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_T1List[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList "<<m_AuxT1List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxT1List[DataNumber]<<")"<<std::endl;
    }
  }
  else if (GetAuxT2Image())
  {
    if(!m_Manually)
    {
      BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList "<<m_T2List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_T2List[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList "<<m_AuxT2List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxT2List[DataNumber]<<")"<<std::endl;
    }
  }
  else
  {
    if(!m_Manually)
    {
      BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList "<<m_PDList[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_PDList[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList "<<m_AuxPDList[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLSourceFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxPDList[DataNumber]<<")"<<std::endl;
    }
  }
	
  BMSAutoSegMRMLSourceFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo ('CREATING MRML DIRECTORY...')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo ( )"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"set (ProcessDataDirectoryPath "<<GetProcessDataDirectory()<<")"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"set (MRMLPath ${ProcessDataDirectoryPath}MRML)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"ListDirInDir (MRMLList ${ProcessDataDirectoryPath} ${MRMLPath})"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"If (${MRMLList} == '')"<<std::endl;  
  BMSAutoSegMRMLSourceFile<<"   MakeDirectory (${MRMLPath})"<<std::endl; 
  BMSAutoSegMRMLSourceFile<<"EndIf (${MRMLList})"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"set (MRMLScene ${MRMLPath}/Source_MRMLScene.mrml)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo ( )"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo ('CREATING MRML DIRECTORY: DONE!')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo ( )"<<std::endl<<std::endl;
	
  if ( (std::strcmp(GetCommonCoordinateImageType(),"T2") == 0) && (GetT2Image()))
  {
    if (GetAuxT1Image())
      BMSAutoSegMRMLSourceFile<<"set(T1RegistrationExtension _regT2_regAtlas)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLSourceFile<<"set(T2RegistrationExtension _regAtlas)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLSourceFile<<"set(PDRegistrationExtension _regT2_regAtlas)"<<std::endl;
  }
  else
  {
    if (GetAuxT1Image())
      BMSAutoSegMRMLSourceFile<<"set(T1RegistrationExtension _regAtlas)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLSourceFile<<"set(T2RegistrationExtension _regT1_regAtlas)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLSourceFile<<"set(PDRegistrationExtension _regT1_regAtlas)"<<std::endl;
  }
	
  if(GetN4ITKBiasFieldCorrection())
    BMSAutoSegMRMLSourceFile<<"set (Bias _Bias)"<<std::endl;
  else
    BMSAutoSegMRMLSourceFile<<"set (Bias '')"<<std::endl;
  if (GetLoop())
  {
    int SuffixIteration=GetLoopIteration()+1;
    BMSAutoSegMRMLSourceFile<<"      set  (SUFFIX EMS_"<<SuffixIteration<<")"<<std::endl;
  }
  else
    BMSAutoSegMRMLSourceFile<<"         set  (SUFFIX EMS)"<<std::endl;

  BMSAutoSegMRMLSourceFile<<"      set (SuffixCorrected _stripEMS_corrected_${SUFFIX})"<<std::endl;

  if(GetAtlasSpaceImage())
  {
    BMSAutoSegMRMLSourceFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegMRMLSourceFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLSourceFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLSourceFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}.nrrd)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   set (AtlasSpaceImagePath ${Path}/${AutoSegDir}/atlasIso)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   set (AtlasSpaceImage ${AtlasSpaceImagePath}/${AtlasSpaceImageHead})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"      set (AtlasSpaceCasesList ${AtlasSpaceImage})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   Else ()"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"      set (AtlasSpaceCasesList ${AtlasSpaceCasesList} ${AtlasSpaceImage})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"set (SourceCasesList ${AtlasSpaceCasesList})"<<std::endl;
  }
  else if(GetBiasCorrectedImage())
  {
    BMSAutoSegMRMLSourceFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegMRMLSourceFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLSourceFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLSourceFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetLoop())
    {
      int SuffixIteration=GetLoopIteration()+1;
      BMSAutoSegMRMLSourceFile<<"   set (BiasCorrectedImagePath ${Path}/${AutoSegDir}/ems_"<<SuffixIteration<<")"<<std::endl;
    }
    else
      BMSAutoSegMRMLSourceFile<<"   set (BiasCorrectedImagePath ${Path}/${AutoSegDir}/ems)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   set (BiasCorrectedImage ${BiasCorrectedImagePath}/${BiasCorrectedImageHead})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"      set (BiasCorrectedCasesList ${BiasCorrectedImage})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   Else ()"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"      set (BiasCorrectedCasesList ${BiasCorrectedCasesList} ${BiasCorrectedImage})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"set (SourceCasesList ${BiasCorrectedCasesList})"<<std::endl;
  }
  else
  {
    BMSAutoSegMRMLSourceFile<<"set (stripEMS _corrected_EMS-stripped)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegMRMLSourceFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLSourceFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLSourceFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   set (SkullStrippedImagePath ${Path}/${AutoSegDir}/Stripped)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   set (SkullStrippedImage ${SkullStrippedImagePath}/${SkullStrippedImageHead})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"      set (SkullStrippedCasesList ${SkullStrippedImage})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   Else ()"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"      set (SkullStrippedCasesList ${SkullStrippedCasesList} ${SkullStrippedImage})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegMRMLSourceFile<<"set (SourceCasesList ${SkullStrippedCasesList})"<<std::endl;
  }
	
  BMSAutoSegMRMLSourceFile<<"set (Aux1Label "<<GetAux1Label()<<")"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"set (AuxDirList ${Aux1Label})"<<std::endl;
  if (GetAux2Image())
  {
    BMSAutoSegMRMLSourceFile<<"set (AuxDirList ${AuxDirList} "<<GetAux2Label()<<")"<<std::endl;
  }
  if (GetAux3Image())
  {
    BMSAutoSegMRMLSourceFile<<"set (AuxDirList ${AuxDirList} "<<GetAux3Label()<<")"<<std::endl;
  }
  if (GetAux4Image())
  {
    BMSAutoSegMRMLSourceFile<<"set (AuxDirList ${AuxDirList} "<<GetAux4Label()<<")"<<std::endl;
  }
  if (GetAux5Image())
  {
    BMSAutoSegMRMLSourceFile<<"set (AuxDirList ${AuxDirList} "<<GetAux5Label()<<")"<<std::endl;
  }
  if (GetAux6Image())
  {
    BMSAutoSegMRMLSourceFile<<"set (AuxDirList ${AuxDirList} "<<GetAux6Label()<<")"<<std::endl;
  }
  if (GetAux7Image())
  {
    BMSAutoSegMRMLSourceFile<<"set (AuxDirList ${AuxDirList} "<<GetAux7Label()<<")"<<std::endl;
  }
  if (GetAux8Image())
  {
    BMSAutoSegMRMLSourceFile<<"set (AuxDirList ${AuxDirList} "<<GetAux8Label()<<")"<<std::endl;
  }
	
  BMSAutoSegMRMLSourceFile<<"ListFileInDir(OutputList ${MRMLPath} Source_MRMLScene.mrml)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"If (${OutputList} == '')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo (*************************************************)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo ('WRITING MRML FILE...')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo ( )"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"   WriteFile(${MRMLScene} '<MRML  version=\"11150\" userTags=\"\">\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"   set (CaseNumber 0)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"   ForEach (SourceCase ${SourceCasesList})"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      GetFilename (SourceCaseHead ${SourceCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      GetFilename (SourceCasePath ${SourceCase} PATH)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      GetFilename (AutoSegPath ${SourceCase} PARENT_PATH)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      Set (Aux1Path ${AutoSegPath}/${Aux1Label})"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      GetFilename (DataPath ${AutoSegPath} PATH)"<<std::endl;	
	
	
  BMSAutoSegMRMLSourceFile<<"      echo ()"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      echo ('Source Case: '${SourceCase})"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      echo ()"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      Math( SnapshotNb ${CaseNumber} + 1)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      Math( foregroundVolumeNb ${SnapshotNb} * 2)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      Math( backgroundVolumeNb ${foregroundVolumeNb} - 1)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      Int(${SnapshotNb})"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      Int(${foregroundVolumeNb})"<<std::endl;
	
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;	
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode${SnapshotNb}\"  name=\"${SourceCaseHead}\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode${foregroundVolumeNb}\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode${foregroundVolumeNb}\"    activeLabelVolumeID=\"NULL\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"3\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"0\"    guiPanelLR=\"0\"    numberOfCompareViewRows=\"0\"    numberOfCompareViewColumns=\"0\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"  ></Layout>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLViewNode1\"    name=\"View\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLCameraNode1\"    name=\"Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"-485.615 -95.5951 70.9923\"    focalPoint=\"0 0 0\"    viewUp=\"0.155036 -0.960178 -0.232429\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <XYPlot\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLXYPlotManagerNode1\"    name=\"vtkMRMLXYPlotManagerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"  ></XYPlot>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <ProstateNavManager\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLProstateNavManagerNode1\"    name=\"vtkMRMLProstateNavManagerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"  ></ProstateNavManager>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <VolumeRenderingScenario\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLVolumeRenderingScenarioNode1\"    name=\"vtkMRMLVolumeRenderingScenarioNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parametersNodeID=\"NULL\"  ></VolumeRenderingScenario>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"296.783 238.125 4.2\"    dimensions=\"425 341 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 -0 0 -1.42109e-14 -0 -0 1 0 0 1 0 1.42109e-14 -0 0 -0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode${backgroundVolumeNb}\"    foregroundVolumeID=\"vtkMRMLScalarVolumeNode${foregroundVolumeNb}\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"238.125 191.06 1.875\"    dimensions=\"425 341 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 -0 -1.42109e-14 0 1 0 -0 -0 0 1 1.4211e-14 0 -0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode${backgroundVolumeNb}\"    foregroundVolumeID=\"vtkMRMLScalarVolumeNode${foregroundVolumeNb}\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"296.783 238.125 1.875\"    dimensions=\"425 341 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-0 0 1 -1.4211e-14 -1 -0 0 -0 -0 1 -0 1.42109e-14 0 -0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode${backgroundVolumeNb}\"    foregroundVolumeID=\"vtkMRMLScalarVolumeNode${foregroundVolumeNb}\"    labelVolumeID=\"\"    compositing=\"0\"    labelOpacity=\"1\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"-1.4211e-14 0 1.42109e-14\"  ></Crosshair>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
	
  BMSAutoSegMRMLSourceFile<<"   Set (fileName ${Aux1Path}/${SourceCaseHead}_Reg_${Aux1Label}.nrrd)"<<std::endl;
	
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLVolumeArchetypeStorageNode${backgroundVolumeNb}\"    name=\"vtkMRMLVolumeArchetypeStorageNode${backgroundVolumeNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${fileName}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLScalarVolumeNode${backgroundVolumeNb}\"    name=\"${SourceCaseHead}_Reg_${Aux1Label}.nrrd\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode${backgroundVolumeNb}\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode${backgroundVolumeNb}\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1.875 4.2 1.875\"    origin=\"119.062 60.9 -119.062\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLScalarVolumeDisplayNode${backgroundVolumeNb}\"    name=\"vtkMRMLScalarVolumeDisplayNode${backgroundVolumeNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeOcean\"     window=\"27282\"    level=\"17849\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
	
  BMSAutoSegMRMLSourceFile<<"      GetParam(OrigAux1Case ${OrigAux1CasesList} ${CaseNumber})"<<std::endl;
	
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLVolumeArchetypeStorageNode${foregroundVolumeNb}\"    name=\"vtkMRMLVolumeArchetypeStorageNode${foregroundVolumeNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${OrigAux1Case}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
	
  BMSAutoSegMRMLSourceFile<<"      GetFilename (OrigAux1CaseHead ${OrigAux1Case} NAME_WITHOUT_EXTENSION)"<<std::endl;
	
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLScalarVolumeNode${foregroundVolumeNb}\"    name=\"${OrigAux1CaseHead}.nrrd\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode${foregroundVolumeNb}\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode${foregroundVolumeNb}\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1.875 4.2 1.875\"    origin=\"119.062 60.9 -119.062\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} '   id=\"vtkMRMLScalarVolumeDisplayNode${foregroundVolumeNb}\"    name=\"vtkMRMLScalarVolumeDisplayNode${foregroundVolumeNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeDesert\"     window=\"4986\"    level=\"2493\"    upperThreshold=\"11160\"    lowerThreshold=\"310\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      AppendFile(${MRMLScene} ' </SceneSnapshot>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      Inc(${CaseNumber} 1)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"      Int(${CaseNumber})"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"   EndForEach (SourceCase)"<<std::endl;
	
  BMSAutoSegMRMLSourceFile<<"echo ( )"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo ('WRITING MRML FILE: DONE!')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"echo ( )"<<std::endl<<std::endl;
	
  BMSAutoSegMRMLSourceFile<<"AppendFile(${MRMLScene} '</MRML>\\n')"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"Else ()"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"   echo ('File already exists: 'Source_MRMLScene.mrml)"<<std::endl;
  BMSAutoSegMRMLSourceFile<<"EndIf (${OutputList})"<<std::endl;
  BMSAutoSegMRMLSourceFile.close();
}

void AutoSegComputation::WriteBMSAutoSegMRMLParcelFile()
{
  int DataNumber;
  std::ofstream BMSAutoSegMRMLParcelFile(GetBMSAutoSegMRMLParcelFile());

  BMSAutoSegMRMLParcelFile<<"#--------------  Parcellation MRML scene script  ---------------------"<<std::endl;
  BMSAutoSegMRMLParcelFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLParcelFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLParcelFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLParcelFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLParcelFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLParcelFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLParcelFile<<"set (AutoSegDir "<<GetDataAutoSegDirectory()<<")"<<std::endl; 

  if (GetAuxT1Image())
  {
    if(!m_Manually)
    {
      BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList "<<m_T1List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_T1List[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList "<<m_AuxT1List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxT1List[DataNumber]<<")"<<std::endl;
    }
  }
  else if (GetAuxT2Image())
  {
    if(!m_Manually)
    {
      BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList "<<m_T2List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_T2List[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList "<<m_AuxT2List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxT2List[DataNumber]<<")"<<std::endl;
    }
  }
  else
  {
    if(!m_Manually)
    {
      BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList "<<m_PDList[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_PDList[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList "<<m_AuxPDList[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLParcelFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxPDList[DataNumber]<<")"<<std::endl;
    }
  }
	
  BMSAutoSegMRMLParcelFile<<"set (ProcessDataDirectoryPath "<<GetProcessDataDirectory()<<")"<<std::endl;
  BMSAutoSegMRMLParcelFile<<"set (MRMLPath ${ProcessDataDirectoryPath}MRML)"<<std::endl;
	
  if ( (std::strcmp(GetCommonCoordinateImageType(),"T2") == 0) && (GetT2Image()))
  {
    if (GetAuxT1Image())
      BMSAutoSegMRMLParcelFile<<"set(T1RegistrationExtension _regT2_regAtlas)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLParcelFile<<"set(T2RegistrationExtension _regAtlas)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLParcelFile<<"set(PDRegistrationExtension _regT2_regAtlas)"<<std::endl;
  }
  else
  {
    if (GetAuxT1Image())
      BMSAutoSegMRMLParcelFile<<"set(T1RegistrationExtension _regAtlas)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLParcelFile<<"set(T2RegistrationExtension _regT1_regAtlas)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLParcelFile<<"set(PDRegistrationExtension _regT1_regAtlas)"<<std::endl;
  }
	
  if(GetN4ITKBiasFieldCorrection())
    BMSAutoSegMRMLParcelFile<<"set (Bias _Bias)"<<std::endl;
  else
    BMSAutoSegMRMLParcelFile<<"set (Bias '')"<<std::endl;
  if (GetLoop())
  {
    int SuffixIteration=GetLoopIteration()+1;
    BMSAutoSegMRMLParcelFile<<"      set  (SUFFIX EMS_"<<SuffixIteration<<")"<<std::endl;
  }
  else
    BMSAutoSegMRMLParcelFile<<"         set  (SUFFIX EMS)"<<std::endl;

  BMSAutoSegMRMLParcelFile<<"      set (SuffixCorrected _stripEMS_corrected_${SUFFIX})"<<std::endl;

  if(GetAtlasSpaceImage())
  {
    BMSAutoSegMRMLParcelFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegMRMLParcelFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLParcelFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLParcelFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}.nrrd)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   set (AtlasSpaceImagePath ${Path}/${AutoSegDir}/atlasIso)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   set (AtlasSpaceImage ${AtlasSpaceImagePath}/${AtlasSpaceImageHead})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      set (AtlasSpaceCasesList ${AtlasSpaceImage})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   Else ()"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      set (AtlasSpaceCasesList ${AtlasSpaceCasesList} ${AtlasSpaceImage})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"set (SourceCasesList ${AtlasSpaceCasesList})"<<std::endl;
  }
  else if(GetBiasCorrectedImage())
  {
    BMSAutoSegMRMLParcelFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegMRMLParcelFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLParcelFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLParcelFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetLoop())
    {
      int SuffixIteration=GetLoopIteration()+1;
      BMSAutoSegMRMLParcelFile<<"   set (BiasCorrectedImagePath ${Path}/${AutoSegDir}/ems_"<<SuffixIteration<<")"<<std::endl;
    }
    else
      BMSAutoSegMRMLParcelFile<<"   set (BiasCorrectedImagePath ${Path}/${AutoSegDir}/ems)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   set (BiasCorrectedImage ${BiasCorrectedImagePath}/${BiasCorrectedImageHead})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      set (BiasCorrectedCasesList ${BiasCorrectedImage})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   Else ()"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      set (BiasCorrectedCasesList ${BiasCorrectedCasesList} ${BiasCorrectedImage})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"set (SourceCasesList ${BiasCorrectedCasesList})"<<std::endl;
  }
  else
  {
    BMSAutoSegMRMLParcelFile<<"set (stripEMS _corrected_EMS-stripped)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegMRMLParcelFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLParcelFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLParcelFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   set (SkullStrippedImagePath ${Path}/${AutoSegDir}/Stripped)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   set (SkullStrippedImage ${SkullStrippedImagePath}/${SkullStrippedImageHead})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      set (SkullStrippedCasesList ${SkullStrippedImage})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   Else ()"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      set (SkullStrippedCasesList ${SkullStrippedCasesList} ${SkullStrippedImage})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"set (SourceCasesList ${SkullStrippedCasesList})"<<std::endl;
  }
	
  BMSAutoSegMRMLParcelFile<<"set (Aux1Label "<<GetAux1Label()<<")"<<std::endl;
  BMSAutoSegMRMLParcelFile<<"set (AuxDirList ${Aux1Label})"<<std::endl;
  if (GetAux2Image())
  {
    BMSAutoSegMRMLParcelFile<<"set (AuxDirList ${AuxDirList} "<<GetAux2Label()<<")"<<std::endl;
  }
  if (GetAux3Image())
  {
    BMSAutoSegMRMLParcelFile<<"set (AuxDirList ${AuxDirList} "<<GetAux3Label()<<")"<<std::endl;
  }
  if (GetAux4Image())
  {
    BMSAutoSegMRMLParcelFile<<"set (AuxDirList ${AuxDirList} "<<GetAux4Label()<<")"<<std::endl;
  }
  if (GetAux5Image())
  {
    BMSAutoSegMRMLParcelFile<<"set (AuxDirList ${AuxDirList} "<<GetAux5Label()<<")"<<std::endl;
  }
  if (GetAux6Image())
  {
    BMSAutoSegMRMLParcelFile<<"set (AuxDirList ${AuxDirList} "<<GetAux6Label()<<")"<<std::endl;
  }
  if (GetAux7Image())
  {
    BMSAutoSegMRMLParcelFile<<"set (AuxDirList ${AuxDirList} "<<GetAux7Label()<<")"<<std::endl;
  }
  if (GetAux8Image())
  {
    BMSAutoSegMRMLParcelFile<<"set (AuxDirList ${AuxDirList} "<<GetAux8Label()<<")"<<std::endl;
  }


  bool IsParcellationMapListEmpty = true;
  if (GetParcellationMapSegmentation())
  {
    if (std::strlen(GetParcellationFile1()) != 0)
    {
      BMSAutoSegMRMLParcelFile<<"set (ParcellationMapList "<<GetParcellationFile1()<<")"<<std::endl;
      IsParcellationMapListEmpty = false;
    }
    if (std::strlen(GetParcellationFile2()) != 0)
    {
      if (IsParcellationMapListEmpty)
	BMSAutoSegMRMLParcelFile<<"set (ParcellationMapList "<<GetParcellationFile2()<<")"<<std::endl;
      else
	BMSAutoSegMRMLParcelFile<<"set (ParcellationMapList ${ParcellationMapList} "<<GetParcellationFile2()<<")"<<std::endl;
      IsParcellationMapListEmpty = false;
    }
    if (std::strlen(GetParcellationFile3()) != 0)
    {
      if (IsParcellationMapListEmpty)
	BMSAutoSegMRMLParcelFile<<"set (ParcellationMapList "<<GetParcellationFile3()<<")"<<std::endl;
      else
	BMSAutoSegMRMLParcelFile<<"set (ParcellationMapList ${ParcellationMapList} "<<GetParcellationFile3()<<")"<<std::endl;
      IsParcellationMapListEmpty = false;
    }
  }  
  if (!IsParcellationMapListEmpty)
  {
	
    BMSAutoSegMRMLParcelFile<<"      ForEach(ParcellationMap ${ParcellationMapList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         GetFilename (ParcellationMapName ${ParcellationMap} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"     set (MRMLScene ${MRMLPath}/${ParcellationMapName}_MRMLScene.mrml)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"     ListFileInDir(OutputList ${MRMLPath} ${ParcellationMapName}_MRMLScene.mrml)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"If (${OutputList} == '')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"echo (*************************************************)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"echo ('WRITING PARCELLATION MRML FILE...')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"echo ( )"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   WriteFile(${MRMLScene} '<MRML  version=\"11150\" userTags=\"\">\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   set (SnapshotNb 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   ForEach (SourceCase ${SourceCasesList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      GetFilename (SourceCaseHead ${SourceCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      GetFilename (SourceCasePath ${SourceCase} PATH)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      GetFilename (AutoSegPath ${SourceCase} PARENT_PATH)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      Set (Aux1Path ${AutoSegPath}/${Aux1Label})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      GetFilename (DataPath ${AutoSegPath} PATH)"<<std::endl;
		
		
    BMSAutoSegMRMLParcelFile<<"      echo ()"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      echo ('Source Case: '${SourceCase})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      echo ()"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"      ForEach (AuxDir ${AuxDirList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         set (AuxPath ${AutoSegPath}/${AuxDir}/)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         Math( LabelVolumeNb ${SnapshotNb} * 2)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         Math( VolumeNb ${LabelVolumeNb} - 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"	    Int(${LabelVolumeNb})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"	    Int(${VolumeNb})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         Set (fileName ${AuxPath}${SourceCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         GetFilename(fileNameHead ${fileName} NAME_WITHOUT_EXTENSION)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         ListFileInDir(Parcellation ${AuxPath} *${ParcellationMapName}-WarpReg_Reg_${AuxDir}.nrrd)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         Set (ParcellationCase ${AuxPath}${Parcellation})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         GetFilename(ParcellationCaseHead ${ParcellationCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
	
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode${SnapshotNb}\"  name=\"${fileNameHead}\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    activeLabelVolumeID=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"3\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"0\"    guiPanelLR=\"0\"    numberOfCompareViewRows=\"0\"    numberOfCompareViewColumns=\"0\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"  ></Layout>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"-273.613 409.401 -86.7554\"    focalPoint=\"0 0 0\"    viewUp=\"-0.0559526 0.171053 0.983672\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <XYPlot\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLXYPlotManagerNode1\"    name=\"vtkMRMLXYPlotManagerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"  ></XYPlot>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <ProstateNavManager\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLProstateNavManagerNode1\"    name=\"vtkMRMLProstateNavManagerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"  ></ProstateNavManager>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <VolumeRenderingScenario\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeRenderingScenarioNode1\"    name=\"vtkMRMLVolumeRenderingScenarioNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parametersNodeID=\"NULL\"  ></VolumeRenderingScenario>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"296.783 238.125 4.2\"    dimensions=\"425 341 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 -0 0 -1.42109e-14 -0 -0 1 0 0 1 0 1.42109e-14 -0 0 -0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    compositing=\"0\"    labelOpacity=\"0.3\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"238.125 191.06 1.875\"    dimensions=\"425 341 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 -0 -1.42109e-14 0 1 0 -0 -0 0 1 1.4211e-14 0 -0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    compositing=\"0\"    labelOpacity=\"0.3\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"296.783 238.125 1.875\"    dimensions=\"425 341 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-0 0 1 -1.4211e-14 -1 -0 0 -0 -0 1 -0 1.42109e-14 0 -0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    compositing=\"0\"    labelOpacity=\"0.3\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"-1.4211e-14 0 1.42109e-14\"  ></Crosshair>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode${VolumeNb}\"    name=\"vtkMRMLVolumeArchetypeStorageNode${VolumeNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${fileName}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    name=\"${fileNameHead}.nrrd\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode${VolumeNb}\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode${SnapshotNb}\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1.875 4.2 1.875\"    origin=\"119.062 60.9 -119.062\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode${SnapshotNb}\"    name=\"vtkMRMLScalarVolumeDisplayNode${SnapshotNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"27282\"    level=\"17849\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode${LabelVolumeNb}\"    name=\"vtkMRMLVolumeArchetypeStorageNode${LabelVolumeNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${ParcellationCase}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    name=\"${ParcellationCaseHead}.nrrd\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode${LabelVolumeNb}\"    userTags=\"\"    displayNodeRef=\"vtkMRMLLabelMapVolumeDisplayNode${SnapshotNb}\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1.875 4.2 1.875\"    origin=\"119.062 60.9 -119.062\"    labelMap=\"1\"  ></Volume>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '  <LabelMapVolumeDisplay\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLLabelMapVolumeDisplayNode${SnapshotNb}\"    name=\"vtkMRMLLabelMapVolumeDisplayNode${SnapshotNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeLabels\"   ></LabelMapVolumeDisplay>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"         AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"	    Inc(${SnapshotNb} 1)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"	    Int(${SnapshotNb})"<<std::endl;	
    BMSAutoSegMRMLParcelFile<<"      EndForEach (AuxDir)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   EndForEach (SourceCase)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"AppendFile(${MRMLScene} '</MRML>\\n')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"echo ( )"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"echo ('WRITING PARCELLATION MRML FILE: DONE!')"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"echo ( )"<<std::endl<<std::endl;
    BMSAutoSegMRMLParcelFile<<"Else ()"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   echo ('File already exists: 'Parcellation_MRMLScene.mrml)"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"EndIf (${OutputList})"<<std::endl;
    BMSAutoSegMRMLParcelFile<<"   EndForEach (ParcellationMap)"<<std::endl;
  }
  BMSAutoSegMRMLParcelFile.close();
}

void AutoSegComputation::WriteBMSAutoSegMRMLAllROIFile()
{
  int DataNumber;
  std::ofstream BMSAutoSegMRMLAllROIFile(GetBMSAutoSegMRMLAllROIFile());

  BMSAutoSegMRMLAllROIFile<<"#--------------     AllROI MRML scene script     ---------------------"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"#---------------------------------------------------------------------"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"set (AutoSegDir "<<GetDataAutoSegDirectory()<<")"<<std::endl; 

  if (GetAuxT1Image())
  {
    if(!m_Manually)
    {
      BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList "<<m_T1List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_T1List[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList "<<m_AuxT1List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxT1List[DataNumber]<<")"<<std::endl;
    }
  }
  else if (GetAuxT2Image())
  {
    if(!m_Manually)
    {
      BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList "<<m_T2List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_T2List[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList "<<m_AuxT2List[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxT2List[DataNumber]<<")"<<std::endl;
    }
  }
  else
  {
    if(!m_Manually)
    {
      BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList "<<m_PDList[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_PDList[DataNumber]<<")"<<std::endl;
    }
    else
    {
      BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList "<<m_AuxPDList[0]<<")"<<std::endl;
      for (DataNumber = 1; DataNumber < GetNbData(); DataNumber++)
	BMSAutoSegMRMLAllROIFile<<"set (OrigSourceCasesList ${OrigSourceCasesList} "<<m_AuxPDList[DataNumber]<<")"<<std::endl;
    }
  }
  BMSAutoSegMRMLAllROIFile<<"set (MRMLPath ${ProcessDataDirectoryPath}MRML)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"set (MRMLScene ${MRMLPath}/AllROI_MRMLScene.mrml)"<<std::endl;
	
  if ( (std::strcmp(GetCommonCoordinateImageType(),"T2") == 0) && (GetT2Image()))
  {
    if (GetAuxT1Image())
      BMSAutoSegMRMLAllROIFile<<"set(T1RegistrationExtension _regT2_regAtlas)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLAllROIFile<<"set(T2RegistrationExtension _regAtlas)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLAllROIFile<<"set(PDRegistrationExtension _regT2_regAtlas)"<<std::endl;
  }
  else
  {
    if (GetAuxT1Image())
      BMSAutoSegMRMLAllROIFile<<"set(T1RegistrationExtension _regAtlas)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLAllROIFile<<"set(T2RegistrationExtension _regT1_regAtlas)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLAllROIFile<<"set(PDRegistrationExtension _regT1_regAtlas)"<<std::endl;
  }

  if(GetN4ITKBiasFieldCorrection())
    BMSAutoSegMRMLAllROIFile<<"set (Bias _Bias)"<<std::endl;
  else
    BMSAutoSegMRMLAllROIFile<<"set (Bias '')"<<std::endl;
  if (GetLoop())
  {
    int SuffixIteration=GetLoopIteration()+1;
    BMSAutoSegMRMLAllROIFile<<"      set  (SUFFIX EMS_"<<SuffixIteration<<")"<<std::endl;
  }
  else
    BMSAutoSegMRMLAllROIFile<<"         set  (SUFFIX EMS)"<<std::endl;

  BMSAutoSegMRMLAllROIFile<<"      set (SuffixCorrected _stripEMS_corrected_${SUFFIX})"<<std::endl;
	
  if(GetAtlasSpaceImage())
  {
    BMSAutoSegMRMLAllROIFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegMRMLAllROIFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLAllROIFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLAllROIFile<<"   set (AtlasSpaceImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}.nrrd)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   set (AtlasSpaceImagePath ${Path}/${AutoSegDir}/atlasIso)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   set (AtlasSpaceImage ${AtlasSpaceImagePath}/${AtlasSpaceImageHead})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"      set (AtlasSpaceCasesList ${AtlasSpaceImage})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   Else ()"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"      set (AtlasSpaceCasesList ${AtlasSpaceCasesList} ${AtlasSpaceImage})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"set (SourceCasesList ${AtlasSpaceCasesList})"<<std::endl;
  }
  else if(GetBiasCorrectedImage())
  {
    BMSAutoSegMRMLAllROIFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegMRMLAllROIFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLAllROIFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLAllROIFile<<"   set (BiasCorrectedImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}${SuffixCorrected}.nrrd)"<<std::endl;
    if (GetLoop())
    {
      int SuffixIteration=GetLoopIteration()+1;
      BMSAutoSegMRMLAllROIFile<<"   set (BiasCorrectedImagePath ${Path}/${AutoSegDir}/ems_"<<SuffixIteration<<")"<<std::endl;
    }
    else
      BMSAutoSegMRMLAllROIFile<<"   set (BiasCorrectedImagePath ${Path}/${AutoSegDir}/ems)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   set (BiasCorrectedImage ${BiasCorrectedImagePath}/${BiasCorrectedImageHead})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"      set (BiasCorrectedCasesList ${BiasCorrectedImage})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   Else ()"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"      set (BiasCorrectedCasesList ${BiasCorrectedCasesList} ${BiasCorrectedImage})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"set (SourceCasesList ${BiasCorrectedCasesList})"<<std::endl;
  }
  else
  {
    BMSAutoSegMRMLAllROIFile<<"set (stripEMS _corrected_EMS-stripped)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"set (CaseNumber 1)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"set (OrigCasesList ${OrigSourceCasesList})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"ForEach (OrigCase ${OrigCasesList})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   GetFilename (Path ${OrigCase} PATH)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   GetFilename(OrigCaseHead ${OrigCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
    if (GetAuxT1Image())
      BMSAutoSegMRMLAllROIFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${T1RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    if (GetAuxT2Image())
      BMSAutoSegMRMLAllROIFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${T2RegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    if (GetAuxPDImage())
      BMSAutoSegMRMLAllROIFile<<"   set (SkullStrippedImageHead ${OrigCaseHead}${Bias}${PDRegistrationExtension}${stripEMS}.nrrd)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   set (SkullStrippedImagePath ${Path}/${AutoSegDir}/Stripped)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   set (SkullStrippedImage ${SkullStrippedImagePath}/${SkullStrippedImageHead})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   If (${CaseNumber} == 1)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"      set (SkullStrippedCasesList ${SkullStrippedImage})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   Else ()"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"      set (SkullStrippedCasesList ${SkullStrippedCasesList} ${SkullStrippedImage})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   EndIf (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   Inc (${CaseNumber} 1)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"   Int (${CaseNumber})"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"EndForEach(OrigCase)"<<std::endl;
    BMSAutoSegMRMLAllROIFile<<"set (SourceCasesList ${SkullStrippedCasesList})"<<std::endl;
  }
		
  BMSAutoSegMRMLAllROIFile<<"set (Aux1Label "<<GetAux1Label()<<")"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"set (AuxDirList ${Aux1Label})"<<std::endl;
  if (GetAux2Image())
  {
    BMSAutoSegMRMLAllROIFile<<"set (AuxDirList ${AuxDirList} "<<GetAux2Label()<<")"<<std::endl;
  }
  if (GetAux3Image())
  {
    BMSAutoSegMRMLAllROIFile<<"set (AuxDirList ${AuxDirList} "<<GetAux3Label()<<")"<<std::endl;
  }
  if (GetAux4Image())
  {
    BMSAutoSegMRMLAllROIFile<<"set (AuxDirList ${AuxDirList} "<<GetAux4Label()<<")"<<std::endl;
  }
  if (GetAux5Image())
  {
    BMSAutoSegMRMLAllROIFile<<"set (AuxDirList ${AuxDirList} "<<GetAux5Label()<<")"<<std::endl;
  }
  if (GetAux6Image())
  {
    BMSAutoSegMRMLAllROIFile<<"set (AuxDirList ${AuxDirList} "<<GetAux6Label()<<")"<<std::endl;
  }
  if (GetAux7Image())
  {
    BMSAutoSegMRMLAllROIFile<<"set (AuxDirList ${AuxDirList} "<<GetAux7Label()<<")"<<std::endl;
  }
  if (GetAux8Image())
  {
    BMSAutoSegMRMLAllROIFile<<"set (AuxDirList ${AuxDirList} "<<GetAux8Label()<<")"<<std::endl;
  }
	
  BMSAutoSegMRMLAllROIFile<<"ListFileInDir(OutputList ${MRMLPath} AllROI_MRMLScene.mrml)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"If (${OutputList} == '')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"   echo (*************************************************)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"   echo ('WRITING AllROI MRML FILE...')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"   echo ( )"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"   WriteFile(${MRMLScene} '<MRML  version=\"11150\" userTags=\"\">\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"   set (SnapshotNb 1)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"   ForEach (SourceCase ${SourceCasesList})"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"      GetFilename (SourceCaseHead ${SourceCase} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"      GetFilename (SourceCasePath ${SourceCase} PATH)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"      GetFilename (AutoSegPath ${SourceCase} PARENT_PATH)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"      Set (Aux1Path ${AutoSegPath}/${Aux1Label})"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"      GetFilename (DataPath ${AutoSegPath} PATH)"<<std::endl;

  BMSAutoSegMRMLAllROIFile<<"      echo ( )"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"      echo ('Source Case: '${SourceCase})"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"      echo ( )"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"      ForEach (AuxDir ${AuxDirList})"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         set (AuxPath ${AutoSegPath}/${AuxDir}/)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         Math( LabelVolumeNb ${SnapshotNb} * 2)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         Math( VolumeNb ${LabelVolumeNb} - 1)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"	    Int(${LabelVolumeNb})"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"	    Int(${VolumeNb})"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         Set (fileName ${AuxPath}${SourceCaseHead}_Reg_${AuxDir}.nrrd)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         GetFilename(fileNameHead ${fileName} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         ListFileInDir(AllROI ${AuxPath} *AllROI_Reg_${AuxDir}.nrrd)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         Set (AllROICase ${AuxPath}/${AllROI})"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         GetFilename(AllROICaseHead ${AllROICase} NAME_WITHOUT_EXTENSION)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} ' <SceneSnapshot\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  id=\"vtkMRMLSceneSnapshotNode${SnapshotNb}\"  name=\"${AuxDir}_${AllROICaseHead}\"  hideFromEditors=\"true\"  selectable=\"true\"  selected=\"false\" >  <Selection\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSelectionNode1\"    name=\"vtkMRMLSelectionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    activeVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    secondaryVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    activeLabelVolumeID=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    activeFiducialListID=\"NULL\"    activeROIListID=\"NULL\"    activeCameraID=\"NULL\"    activeViewID=\"NULL\"    activeLayoutID=\"vtkMRMLLayoutNode1\"  ></Selection>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <Interaction\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLInteractionNode1\"    name=\"vtkMRMLInteractionNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentInteractionMode=\"ViewTransform\"    lastInteractionMode=\"ViewTransform\"  ></Interaction>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <Layout\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLLayoutNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    currentViewArrangement=\"3\"    guiPanelVisibility=\"1\"    bottomPanelVisibility =\"0\"    guiPanelLR=\"0\"    numberOfCompareViewRows=\"0\"    numberOfCompareViewColumns=\"0\"    numberOfLightboxRows=\"1\"    numberOfLightboxColumns=\"1\"    mainPanelSize=\"400\"    secondaryPanelSize=\"400\"  ></Layout>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <View\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLViewNode1\"    name=\"View\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    active=\"true\"    fieldOfView=\"200\"    letterSize=\"0.05\"    boxVisible=\"true\"    fiducialsVisible=\"true\"    fiducialLabelsVisible=\"true\"    axisLabelsVisible=\"true\"    backgroundColor=\"0.70196 0.70196 0.90588\"    animationMode=\"Off\"    viewAxisMode=\"LookFrom\"    spinDegrees=\"2\"    spinMs=\"5\"    spinDirection=\"YawLeft\"    rotateDegrees=\"5\"    rockLength=\"200\"    rockCount=\"0\"    stereoType=\"NoStereo\"    renderMode=\"Perspective\"  ></View>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <Camera\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLCameraNode1\"    name=\"Camera\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    position=\"-420.748 -96.5859 252.275\"    focalPoint=\"0 0 0\"    viewUp=\"0.415013 -0.829049 0.374755\"    parallelProjection=\"false\"    parallelScale=\"1\"    activetag=\"vtkMRMLViewNode1\"  ></Camera>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <TGParameters\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLChangeTrackerNode1\"    name=\"vtkMRMLChangeTrackerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    ROIMin=\"-1 -1 -1\"    ROIMax=\"-1 -1 -1\"    SegmentThresholdMin=\"-1\"    SegmentThresholdMax=\"-1\"    Analysis_Intensity_Flag=\"0\"    Analysis_Deformable_Flag=\"0\"    UseITK=\"1\"    RegistrationChoice=\"3\"    ROIRegistration=\"1\"    ResampleChoice=\"3\"    ResampleConst=\"0.5\"  ></TGParameters>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <XYPlot\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLXYPlotManagerNode1\"    name=\"vtkMRMLXYPlotManagerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"  ></XYPlot>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <ProstateNavManager\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLProstateNavManagerNode1\"    name=\"vtkMRMLProstateNavManagerNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"  ></ProstateNavManager>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <VolumeRenderingScenario\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeRenderingScenarioNode1\"    name=\"vtkMRMLVolumeRenderingScenarioNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    parametersNodeID=\"NULL\"  ></VolumeRenderingScenario>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode1\"    name=\"Green\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"296.783 238.125 4.2\"    dimensions=\"425 341 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 -0 0 -1.42109e-14 -0 -0 1 0 0 1 0 1.42109e-14 -0 0 -0 1\"    layoutName=\"Green\"    orientation=\"Coronal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode1\"    name=\"vtkMRMLSliceCompositeNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    compositing=\"0\"    labelOpacity=\"0.58\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Green\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode2\"    name=\"Red\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"238.125 191.06 1.875\"    dimensions=\"425 341 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-1 0 -0 -1.42109e-14 0 1 0 -0 -0 0 1 1.4211e-14 0 -0 0 1\"    layoutName=\"Red\"    orientation=\"Axial\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode2\"    name=\"vtkMRMLSliceCompositeNode2\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    compositing=\"0\"    labelOpacity=\"0.58\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Red\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <Slice\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceNode3\"    name=\"Yellow\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fieldOfView=\"296.783 238.125 1.875\"    dimensions=\"425 341 1\"    activeSlice=\"0\"    layoutGridRows=\"1\"    layoutGridColumns=\"1\"    sliceToRAS=\"-0 0 1 -1.4211e-14 -1 -0 0 -0 -0 1 -0 1.42109e-14 0 -0 0 1\"    layoutName=\"Yellow\"    orientation=\"Sagittal\"    jumpMode=\"1\"    sliceVisibility=\"true\"    widgetVisibility=\"false\"    useLabelOutline=\"false\"    sliceSpacingMode=\"0\"    prescribedSliceSpacing=\"1 1 1\"  ></Slice>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <SliceComposite\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLSliceCompositeNode3\"    name=\"vtkMRMLSliceCompositeNode3\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    backgroundVolumeID=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    foregroundVolumeID=\"\"    labelVolumeID=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    compositing=\"0\"    labelOpacity=\"0.58\"    linkedControl=\"1\"    foregroundGrid=\"0\"    backgroundGrid=\"0\"    labelGrid=\"1\"    fiducialVisibility=\"1\"    fiducialLabelVisibility=\"1\"    sliceIntersectionVisibility=\"0\"    layoutName=\"Yellow\"    annotationMode=\"All\"    doPropagateVolumeSelection=\"1\"  ></SliceComposite>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <Crosshair\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLCrosshairNode1\"    name=\"vtkMRMLCrosshairNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    crosshairMode=\"NoCrosshair\"    navigation=\"true\"    crosshairBehavior=\"Normal\"    crosshairThickness=\"Fine\"    crosshairRAS=\"-1.4211e-14 0 1.42109e-14\"  ></Crosshair>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <ScriptedModule\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLScriptedModuleNode1\"    name=\"vtkMRMLScriptedModuleNode1\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\" parameter0= \"label 1\"  ></ScriptedModule>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode${VolumeNb}\"    name=\"vtkMRMLVolumeArchetypeStorageNode${VolumeNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${fileName}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode${VolumeNb}\"    name=\"${fileNameHead}.nrrd\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode${VolumeNb}\"    userTags=\"\"    displayNodeRef=\"vtkMRMLScalarVolumeDisplayNode${SnapshotNb}\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1.875 4.2 1.875\"    origin=\"119.062 60.9 -119.062\"    labelMap=\"0\"  ></Volume>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <VolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeDisplayNode${SnapshotNb}\"    name=\"vtkMRMLScalarVolumeDisplayNode${SnapshotNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeGrey\"     window=\"27282\"    level=\"17849\"    upperThreshold=\"32767\"    lowerThreshold=\"-32768\"    interpolate=\"1\"    autoWindowLevel=\"1\"    applyThreshold=\"0\"    autoThreshold=\"0\"  ></VolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <VolumeArchetypeStorage\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLVolumeArchetypeStorageNode${LabelVolumeNb}\"    name=\"vtkMRMLVolumeArchetypeStorageNode${LabelVolumeNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    fileName=\"${AllROICase}\"    useCompression=\"1\"    readState=\"0\"    writeState=\"0\"    centerImage=\"1\"    singleFile=\"0\"    UseOrientationFromFile=\"1\"  ></VolumeArchetypeStorage>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <Volume\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLScalarVolumeNode${LabelVolumeNb}\"    name=\"${AllROICaseHead}.nrrd\"    hideFromEditors=\"false\"    selectable=\"true\"    selected=\"false\"    storageNodeRef=\"vtkMRMLVolumeArchetypeStorageNode${LabelVolumeNb}\"    userTags=\"\"    displayNodeRef=\"vtkMRMLLabelMapVolumeDisplayNode${SnapshotNb}\"    ijkToRASDirections=\"-1   -0   0 -0   -1   -0 0 -0 1 \"    spacing=\"1.875 4.2 1.875\"    origin=\"119.062 60.9 -119.062\"    labelMap=\"1\"  ></Volume>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '  <LabelMapVolumeDisplay\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '    id=\"vtkMRMLLabelMapVolumeDisplayNode${SnapshotNb}\"    name=\"vtkMRMLLabelMapVolumeDisplayNode${SnapshotNb}\"    hideFromEditors=\"true\"    selectable=\"true\"    selected=\"false\"    color=\"0.5 0.5 0.5\"    selectedColor=\"1 0 0\"    selectedAmbient=\"0.4\"    ambient=\"0\"    diffuse=\"1\"    selectedSpecular=\"0.5\"    specular=\"0\"    power=\"1\"    opacity=\"1\"    visibility=\"true\"    clipping=\"false\"    sliceIntersectionVisibility=\"false\"    backfaceCulling=\"true\"    scalarVisibility=\"false\"    vectorVisibility=\"false\"    tensorVisibility=\"false\"    autoScalarRange=\"true\"    scalarRange=\"0 100\"    colorNodeRef=\"vtkMRMLColorTableNodeLabels\"   ></LabelMapVolumeDisplay>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         AppendFile(${MRMLScene} '</SceneSnapshot>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"	    Inc(${SnapshotNb} 1)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"         Int(${SnapshotNb})"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"      EndForEach (AuxDir)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"   EndForEach (SourceCase)"<<std::endl;
	
  BMSAutoSegMRMLAllROIFile<<"echo ( )"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"echo ('WRITING AllROI MRML FILE: DONE!')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"echo ( )"<<std::endl<<std::endl;
	
  BMSAutoSegMRMLAllROIFile<<"AppendFile(${MRMLScene} '</MRML>\\n')"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"Else ()"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"   echo ('File already exists: 'AllROI_MRMLScene.mrml)"<<std::endl;
  BMSAutoSegMRMLAllROIFile<<"EndIf (${OutputList})"<<std::endl;
  BMSAutoSegMRMLAllROIFile.close();
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
    int result, Value;
    args.push_back(pathSlicer.c_str());
    args.push_back((MRML.front()).c_str());
	// don't forget this line at the end of the argument list
    args.push_back(0);
	
    m_Process = itksysProcess_New();
    itksysProcess_SetCommand(m_Process, &*args.begin());
    itksysProcess_SetOption(m_Process,itksysProcess_Option_HideWindow,1);
    itksysProcess_Execute(m_Process);
    while(Value = itksysProcess_WaitForData(m_Process,&data,&length,NULL))
    {
      if (Value == itksysProcess_Pipe_STDERR)
      {
	for(int i=0;i<length;i++)
	  std::cout<<data[i];
      }
    }
    itksysProcess_WaitForExit(m_Process, 0);
	
    LogFile<<m_output.c_str()<<std::endl;
	
    result = 1;
    switch(itksysProcess_GetState(m_Process))
    {
      case itksysProcess_State_Exited:
      {
	result = itksysProcess_GetExitValue(m_Process);
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
	if (std::strlen(Line+24) != 0)
	  SetProcessDataDirectory(Line+24);
	else
	  SetProcessDataDirectory("");
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
      }
    }
    fclose(ComputationFile);
    if (nbData!=0)
    {
      SetNbData(nbData);
      AllocateDataList();
      for(int i=0;i<nbData;i++)
	SetDataList(DataList[i].c_str(),i,0);
    }
  }
  else
    std::cout<<"Error Opening File: "<<_FileName<<std::endl;
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
  char Line[512]; 
  int Length;
    // Tissue Segmentation
  int FilterIterations, MaxBiasDegree, Loop;
  float FilterTimeStep, Prior1, Prior2, Prior3, Prior4, Prior5, FluidAtlasWarpMaxStep;
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
    // Skull Stripping
  int DeleteVessels;
    // Regional histogram
  float PointSpacing;
    // N4 ITK Bias Field Correction
  std::string NbOfIterations,BSplineGridResolutions,HistogramSharpening;
  int N4ITKBiasFieldCorrection,ShrinkFactor,BSplineOrder;
  float ConvergenceThreshold,BSplineBeta,BSplineAlpha,SplineDistance;
    // Reorientation
  int Reorientation;
  std::string InputDataOrientation, OutputDataOrientation;

  bool IsParameterFileLoaded=false;

  if (mode ==file)
  {
    // INIT / default setting for backward comaptibility
    
    // Init for EMS loop
    SetLoop(0);
    // Init for N4
    SetN4ITKBiasFieldCorrection(0);
    // Init for reorientation
    SetReorientation(0);
    // Init for rigid registration
    SetRegistrationInitialization("useCenterOfHeadAlign");
    SetInitRegUseT1InitTransform(0);
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
      }
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
	  }
	  else
	    SetFluidAtlasFATW(0);
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
      }
      if (mode == warping||mode == advancedParameters||mode == file)
      {
	if ( (std::strncmp("Warping Method: ", Line, 16)) == 0)
	{	
	  if (std::strcmp(Line+16, "Classic") == 0)
	  {
	    SetClassicWarpingMethod(1);
	    SetBRAINSDemonWarpMethod(0);
	  }
	  else if (std::strcmp(Line+16, "Coarse-to-fine") == 0)
	  {
	    SetClassicWarpingMethod(0);
	    SetBRAINSDemonWarpMethod(0);
	  }
	  else if (std::strcmp(Line+16, "BRAINSDemonWarp") == 0)
	  {
	    SetClassicWarpingMethod(0);
	    SetBRAINSDemonWarpMethod(1);
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
      }
      if(mode == N4biasFieldCorrection||mode == advancedParameters||mode == file)
      {	
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
      }
    }
    fclose(ParameterFile);
  }
  else
    std::cout<<"Error Opening File: "<<_FileName<<std::endl;

  return IsParameterFileLoaded;
}
