/*=========================================================================
  Program:   AutoSeg
  Module:    $RCSfile: AutoSegGUIControls.h,v $
  Language:  C++
  Date:      $Date: 2011/04/19 08:04:40 $
  Version:   $Revision: 1.19 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef AUTOSEGGUICONTROLS_H
#define AUTOSEGGUICONTROLS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "AutoSegGUI.h"
#include "AddDataGUIControls.h"
#include "AddAuxDataGUIControls.h"
#include "AboutGUIControls.h"
#include "AutoSegComputation.h"

// FLTK Libraries
#include <FL/Fl_Group.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_File_Browser.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/fl_ask.H>

//ITK Libraries
#include <itksys/Process.h>

class AutoSegGUIControls : public AutoSegGUI
{
  public:
  
    AutoSegGUIControls(char *_AutoSegPath);
    virtual ~AutoSegGUIControls();

  // Menu
    // SubMenu File
    void LoadParameterFileGUI();
    void LoadComputationFileGUI();

    void SaveParameterFileGUI();
    void SaveComputationFileGUI();

    void UseDefaultParametersGUI();
    void SetDefaultParametersGUI();
    void ResetDefaultParametersGUI();
    void ExitAutoSeg();
    //SubMenu Help
    void AboutButtonPressed();  
  
    // Tab Computation
    void SetProcessDataDirectoryGUI();
    // Data Selection
    void T2ButtonChecked();
    void PDButtonChecked();  
    void Aux1ButtonChecked();
    void Aux2ButtonChecked();
    void Aux3ButtonChecked();
    void Aux4ButtonChecked();
    void Aux5ButtonChecked();
    void Aux6ButtonChecked();
    void Aux7ButtonChecked();
    void Aux8ButtonChecked();
    void AuxT1ButtonChecked();
    void AuxT2ButtonChecked();
    void AuxPDButtonChecked();
		
    // Automatic Data Selection  
    void SetDataDirectoryGUI();
    void ComputeDataGUI();
    void ComputeAuxDataGUI();
    // Browser Interaction
    void AddDataGUI();
    void AddAuxDataGUI();
		
    void RemoveDataGUI();
    void RemoveAuxDataGUI();
		
    void ClearDataGUI();
    void ClearAuxDataGUI();
    // Automatic Segmentation Computation
    void ComputeGUI();
    // Computation Parameters
    void ComputeVolumeButtonChecked();
    void ComputeCorticalThicknessButtonChecked();
    void RecomputeButtonChecked();
    void UseCondorButtonChecked();
		
    //Regional Histogram Parameters
    void AtlasSpaceButtonChecked();
    void BiasCorrectedButtonChecked();
    void StrippedButtonChecked();
    void RigidTransformationButtonChecked();
    void AffineTransformationButtonChecked();
    void BsplineTransformationButtonChecked();

    // Display
    void ShowDisplayButtonPressed();
    void ShowMRMLSceneButtonPressed();
    void StopAutoSeg();

  // Tab Parameters
     // Age Group
    void SharpAtlasButtonToggled();
    void FuzzyAtlasButtonToggled();
     // Atlas Selection
    void SetCommonCoordinateImageGUI();
    void SetTissueSegmentationAtlasDirectoryGUI();
    void SetROIAtlasFileGUI();
    void TissueSegmentationAtlasT1ButtonToggled();
    void TissueSegmentationAtlasT2ButtonToggled();
    void CommonCoordinateImageT1ButtonToggled();
    void CommonCoordinateImageT2ButtonToggled();
    // Subcortical Structure Selection
    void AllStructuresButtonChecked();
    void AmygdalaLeftButtonChecked();
    void AmygdalaRightButtonChecked();
    void CaudateLeftButtonChecked();
    void CaudateRightButtonChecked();
    void HippocampusLeftButtonChecked();
    void HippocampusRightButtonChecked();
    void PallidusLeftButtonChecked();
    void PallidusRightButtonChecked();
    void PutamenLeftButtonChecked();
    void PutamenRightButtonChecked();
    void LateralVentricleLeftButtonChecked();
    void LateralVentricleRightButtonChecked();
    void SetAmygdalaLeftGUI();
    void SetAmygdalaRightGUI();
    void SetCaudateLeftGUI();
    void SetCaudateRightGUI();
    void SetHippocampusLeftGUI();
    void SetHippocampusRightGUI();
    void SetPallidusLeftGUI();
    void SetPallidusRightGUI();
    void SetPutamenLeftGUI();
    void SetPutamenRightGUI();
    void SetLateralVentricleLeftGUI();
    void SetLateralVentricleRightGUI();
    // Generic ROI Maps
    void ROIFile1ButtonChecked();
    void ROIFile2ButtonChecked();
    void ROIFile3ButtonChecked();
    void ROIFile4ButtonChecked();
    void ROIFile5ButtonChecked();  
    void SetROIFile1GUI();
    void SetROIFile2GUI();
    void SetROIFile3GUI();
    void SetROIFile4GUI();
    void SetROIFile5GUI();
  // Parcellation File
    void ParcellationFile1ButtonChecked();
    void ParcellationFile2ButtonChecked();
    void ParcellationFile3ButtonChecked();
    void SetParcellationFile1GUI();
    void SetParcellationFile2GUI();
    void SetParcellationFile3GUI();
    void SoftTissueMapButtonToggled();
    void HardTissueMapButtonToggled();
  
  // Tab AdvancedParameters
    // Tissue Segmentation
    void ABCButtonToggled();
    void SetFilterIterationsGUI();
    void SetFilterTimeStepGUI();
    void SetFilterMethodChoiceGUI();
    void SetMaxBiasDegreeGUI();
    void SetInitialDistributionEstimatorChoiceGUI();
    void SetPrior1GUI();
    void SetPrior2GUI();
    void SetPrior3GUI();
    void SetPrior4GUI();
    void FluidAtlasWarpButtonChecked();
    void FluidAtlasFATWButtonChecked();
    void FluidAtlasAffineButtonChecked();
    void SetFluidAtlasWarpIterationsGUI();
    void SetFluidAtlasWarpMaxStepGUI();
    void SetAtlasLinearMappingChoiceGUI();
    void SetImageLinearMappingChoiceGUI();
    void UseDefaultEMSAdvancedParametersGUI();
    void SetLoopGUI();
    void LoopButtonChecked();
    void SetLoopIterationGUI();
    // Reorientation
    void ReorientationButtonChecked();
    // N4ITKBiasFieldCorrection 
    void N4ITKBiasFieldCorrectionButtonChecked();
    void UseDefaultN4AdvancedParametersGUI();
    void SetNbOfIterationsGUI();
    void SetBSplineGridResolutionsGUI();
    void SetConvergenceThresholdGUI();
    void SetSplineDistanceGUI();
    void SetShrinkFactorGUI();
    void SetBSplineOrderGUI();
    void SetBSplineAlphaGUI();
    void SetBSplineBetaGUI();
    void SetHistogramSharpeningGUI();
    // Rigid Registration
    void RigidRegistrationButtonChecked();
    void GridTemplateAtlasButtonToggled();
    void GridTemplateManualButtonToggled();
    void InitRegCenterOfHeadButtonToggled();
    void InitRegMomentsButtonToggled();
    void InitRegGeometryButtonToggled();
    void InitRegOffButtonToggled();
    void InitRegUseT1InitTransformButtonChecked();
    // Atlas Warping
    void ClassicWarpingButtonToggled();
    void CoarseToFineWarpingButtonToggled();
    void BRAINSDemonWarpToggled();
    void SetAlphaGUI();
    void SetBetaGUI();
    void SetGammaGUI();
    void SetMaxPerturbationGUI();
    void SetNumBasisGUI();
    void SetScale4NbIterationsGUI();
    void SetScale2NbIterationsGUI();
    void SetScale1NbIterationsGUI();
    void SetPyramidLevelsGUI();
    void SetMovingShrinkFactorsGUI();
    void SetFixedShrinkFactorsGUI();
    void SetIterationCountPyramidLevelsGUI();
    void SetDeformationFieldSmoothingSigmaGUI();
    void SetRegistrationFilterTypeGUI();
    void UseDefaultWarpingAdvancedParametersGUI();
    // Skull Stripping
    void DeleteVesselsButtonChecked();
    // Intensity Rescaling
    void HistogramQuantileButtonToggled();
    void TissueMeanMatchButtonToggled();
    // Regional histogram
    void SetPointSpacingGUI();
    
  private:

  // Load Files
    bool UpdateParameterGUI(const char *_FileName, enum Mode mode=file);
    void UpdateComputationGUI(const char *_FileName);
    void UpdateAuxComputationGUI(const char *_FileName);
  // Default Parameters Files
    void SetDefaultParameterFile(const char *_DefaultParameterFile);
  
    char *GetDefaultParameterFile();

  // Check inputs to compute data automatically
    bool CheckInputAutoDataSelection();
    bool CheckInputAutoAuxDataSelection();
		
  // Check inputs to compute Automatic Segmentation
    bool CheckInputAutoSeg();
    void CheckDirectoryName(char *_Directory);

  //Automatic Data Selection
    void ComputeData();
    void AddBrowserAutoData();
    void AddAuxBrowserAutoData();
		
    void CheckData(char *_Line);
    void CheckData2(char *_Line);
  
  // Browser
    void InitBrowser();
    void InitAuxBrowser();

    void RightJustifyData(const char *_Input, char *_Output);
    void RightJustifyAuxData(const char *_Input, char *_Output);

    void SetSubcorticalStructures();
    void SetGenericROIMaps();
    void SetParcellationMap();
    void SetGridTemplateParameters();
    void SetReorientationParameters();
    void SetN4Parameters();
    void SetIntensityRescalingMethod();
    void SetRegionalHistogramParameters();
    void InitializeData();
    void InitializeAuxData();
    void InitializeParameters();
    void UpdateParameters();
 
  // Computation
    int CheckStudy();

    int *m_BrowserWidths;
    int *m_AuxBrowserWidths;
		
    AutoSegComputation m_Computation;
		//Mode m_Mode;

    bool m_IsSubcorticalStructureSegmentation;
    bool m_IsGenericROISegmentation;
    bool m_IsParcellationMapSegmentation;
  
  // Default Parameter Files
    char m_DefaultParameterFile[512];
};

#endif
