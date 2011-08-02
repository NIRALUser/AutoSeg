/*=========================================================================
  Program:   AutoSeg
  Module:    $RCSfile: AutoSegComputation.h,v $
  Language:  C++
  Date:      $Date: 2011/04/19 08:04:40 $
  Version:   $Revision: 1.17 $
  Author:    Clement Vachet

  Copyright (c) 2004 NeuroImaging Lab @ UNC. All rights reserved.
  See NeuroLibCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

# ifndef AUTOSEGCOMPUTATION_H
# define AUTOSEGCOMPUTATION_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <itksys/Process.h>
#include <vector>
#include <itksys/Glob.hxx>
#include <cstdlib>

#include "TextDisplayGUIControls.h"

#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/fl_ask.H>

enum Mode
{
  file, advancedParameters,tissueSeg, warping,N4biasFieldCorrection
};

class AutoSegComputation
{
  public:
    AutoSegComputation();
    ~AutoSegComputation();

  //Get AutoSeg Environment
    void SetAutoSegPath(const char* _AutoSegPath){std::strcpy(m_AutoSegPath, _AutoSegPath);};
    char *GetAutoSegPath(){return m_AutoSegPath;};

  // Get Automatic Data File
    char *GetDataFile(){return m_DataFile;};
    char *GetAuxDataFile(){return m_AuxDataFile;};
  // Write Files
    void WriteParameterFile(const char *_FileName);
    void WriteComputationFile(const char *_FileName);

  // Automatic Segmentation computation
    void SetIsAutoSegInProcess(bool _IsAutoSegInProcess){m_IsAutoSegInProcess = _IsAutoSegInProcess;};
    bool GetIsAutoSegInProcess(){return m_IsAutoSegInProcess;};  
  // Tab Computation
    void SetProcessDataDirectory(const char *_ProcessDataDirectory){std::strcpy(m_ProcessDataDirectory,_ProcessDataDirectory);};

		
    void SetT2Image(bool _IsT2Image){m_IsT2Image = _IsT2Image;};
    void SetPDImage(bool _IsPDImage){m_IsPDImage = _IsPDImage;};
		
    void SetAuxT1Image(bool _IsAuxT1Image){m_IsAuxT1Image = _IsAuxT1Image;};
    void SetAuxT2Image(bool _IsAuxT2Image){m_IsAuxT2Image = _IsAuxT2Image;};
    void SetAuxPDImage(bool _IsAuxPDImage){m_IsAuxPDImage = _IsAuxPDImage;};
		
    void SetAux1Image(bool _IsAux1Image){m_IsAux1Image = _IsAux1Image;};
    void SetAux2Image(bool _IsAux2Image){m_IsAux2Image = _IsAux2Image;};
    void SetAux3Image(bool _IsAux3Image){m_IsAux3Image = _IsAux3Image;};
    void SetAux4Image(bool _IsAux4Image){m_IsAux4Image = _IsAux4Image;};
    void SetAux5Image(bool _IsAux5Image){m_IsAux5Image = _IsAux5Image;};
    void SetAux6Image(bool _IsAux6Image){m_IsAux6Image = _IsAux6Image;};
    void SetAux7Image(bool _IsAux7Image){m_IsAux7Image = _IsAux7Image;};
    void SetAux8Image(bool _IsAux8Image){m_IsAux8Image = _IsAux8Image;};
		
    void SetManually(bool _Manually){m_Manually = _Manually;};

  // Dat AutoSeg Directory
    void SetDataAutoSegDirectory(const char *_DataAutoSegDirectory){std::strcpy(m_DataAutoSegDirectory,_DataAutoSegDirectory);};
  // Automatic Data Selection 
    void SetDataDirectory(const char *_DataDirectory){std::strcpy(m_DataDirectory,_DataDirectory);};
    void SetAuxDataDirectory(const char *_AuxDataDirectory){std::strcpy(m_AuxDataDirectory,_AuxDataDirectory);};
    void SetT1(const char *_T1){std::strcpy(m_T1,_T1);};
    void SetT2(const char *_T2){std::strcpy(m_T2,_T2);};
    void SetPD(const char *_PD){std::strcpy(m_PD,_PD);}; 
    void SetAux1(const char *_Aux1){std::strcpy(m_Aux1,_Aux1);};
    void SetAux2(const char *_Aux2){std::strcpy(m_Aux2,_Aux2);};
    void SetAux3(const char *_Aux3){std::strcpy(m_Aux3,_Aux3);};
    void SetAux4(const char *_Aux4){std::strcpy(m_Aux4,_Aux4);};
    void SetAux5(const char *_Aux5){std::strcpy(m_Aux5,_Aux5);};
    void SetAux6(const char *_Aux6){std::strcpy(m_Aux6,_Aux6);};
    void SetAux7(const char *_Aux7){std::strcpy(m_Aux7,_Aux7);};
    void SetAux8(const char *_Aux8){std::strcpy(m_Aux8,_Aux8);};
		
    void SetAux1Label(const char *_Aux1Label){std::strcpy(m_Aux1Label,_Aux1Label);};
    void SetAux2Label(const char *_Aux2Label){std::strcpy(m_Aux2Label,_Aux2Label);};
    void SetAux3Label(const char *_Aux3Label){std::strcpy(m_Aux3Label,_Aux3Label);};
    void SetAux4Label(const char *_Aux4Label){std::strcpy(m_Aux4Label,_Aux4Label);};
    void SetAux5Label(const char *_Aux5Label){std::strcpy(m_Aux5Label,_Aux5Label);};
    void SetAux6Label(const char *_Aux6Label){std::strcpy(m_Aux6Label,_Aux6Label);};
    void SetAux7Label(const char *_Aux7Label){std::strcpy(m_Aux7Label,_Aux7Label);};
    void SetAux8Label(const char *_Aux8Label){std::strcpy(m_Aux8Label,_Aux8Label);};
		
  // Computation Options
    void SetComputeVolume(bool _ComputeVolume){m_ComputeVolume = _ComputeVolume;};
    void SetComputeCorticalThickness(bool _ComputeCorticalThickness){m_ComputeCorticalThickness = _ComputeCorticalThickness;};
    void SetRecompute(bool _Recompute){m_Recompute = _Recompute;};
    void SetUseCondor(bool _UseCondor){m_UseCondor = _UseCondor;};
		
  // Regional Histogram Options
    void SetAtlasSpaceImage(bool _AtlasSpaceImage){m_AtlasSpaceImage = _AtlasSpaceImage;};
    void SetBiasCorrectedImage(bool _BiasCorrectedImage){m_BiasCorrectedImage = _BiasCorrectedImage;};
    void SetSkullStrippedImage(bool _SkullStrippedImage){m_SkullStrippedImage = _SkullStrippedImage;};
    void SetRigidTransformation(bool _RigidTransformation){m_RigidTransformation = _RigidTransformation;};
    void SetAffineTransformation(bool _AffineTransformation){m_AffineTransformation = _AffineTransformation;};
    void SetBsplineTransformation(bool _BsplineTransformation){m_BsplineTransformation = _BsplineTransformation;};
	
  // Display
    void ShowDisplay();
  
  // Tab Parameters
    // Atlases
    void SetCommonCoordinateImage(const char *_CommonCoordinateImage){std::strcpy(m_CommonCoordinateImage,_CommonCoordinateImage);};
    void SetTissueSegmentationAtlasDirectory(const char *_TissueSegmentationAtlasDirectory){std::strcpy(m_TissueSegmentationAtlasDirectory,_TissueSegmentationAtlasDirectory);};
    void SetROIAtlasFile(const char *_ROIAtlasFile){std::strcpy(m_ROIAtlasFile,_ROIAtlasFile);};
    void SetTissueSegmentationAtlasType(const char *_TissueSegmentationAtlasType){std::strcpy(m_TissueSegmentationAtlasType, _TissueSegmentationAtlasType);};
    void SetCommonCoordinateImageType(const char *_CommonCoordinateImageType){std::strcpy(m_CommonCoordinateImageType, _CommonCoordinateImageType);};
    // Probabilistic Subcortical Structures Parameters
    void SetAmygdalaLeft(const char *_AmygdalaLeft){std::strcpy(m_AmygdalaLeft, _AmygdalaLeft);};
    void SetAmygdalaRight(const char *_AmygdalaRight){std::strcpy(m_AmygdalaRight, _AmygdalaRight);};
    void SetCaudateLeft(const char *_CaudateLeft){std::strcpy(m_CaudateLeft, _CaudateLeft);};
    void SetCaudateRight(const char *_CaudateRight){std::strcpy(m_CaudateRight, _CaudateRight);};
    void SetHippocampusLeft(const char *_HippocampusLeft){std::strcpy(m_HippocampusLeft, _HippocampusLeft);};
    void SetHippocampusRight(const char *_HippocampusRight){std::strcpy(m_HippocampusRight, _HippocampusRight);};
    void SetPallidusLeft(const char *_PallidusLeft){std::strcpy(m_PallidusLeft, _PallidusLeft);};
    void SetPallidusRight(const char *_PallidusRight){std::strcpy(m_PallidusRight, _PallidusRight);};
    void SetPutamenLeft(const char *_PutamenLeft){std::strcpy(m_PutamenLeft, _PutamenLeft);};
    void SetPutamenRight(const char *_PutamenRight){std::strcpy(m_PutamenRight, _PutamenRight);};
    void SetLateralVentricleLeft(const char *_LateralVentricleLeft){std::strcpy(m_LateralVentricleLeft, _LateralVentricleLeft);};
    void SetLateralVentricleRight(const char *_LateralVentricleRight){std::strcpy(m_LateralVentricleRight, _LateralVentricleRight);};
    void SetSubcorticalStructureSegmentation(bool _IsSubcorticalStructureSegmentation){m_IsSubcorticalStructureSegmentation = _IsSubcorticalStructureSegmentation;};
    // Generic ROI Map
    void SetROIFile1(const char *_ROIFile1){std::strcpy(m_ROIFile1, _ROIFile1);};
    void SetROIFile2(const char *_ROIFile2){std::strcpy(m_ROIFile2, _ROIFile2);};
    void SetROIFile3(const char *_ROIFile3){std::strcpy(m_ROIFile3, _ROIFile3);};
    void SetROIFile4(const char *_ROIFile4){std::strcpy(m_ROIFile4, _ROIFile4);};
    void SetROIFile5(const char *_ROIFile5){std::strcpy(m_ROIFile5, _ROIFile5);};
    void SetGenericROISegmentation(bool _IsGenericROISegmentation){m_IsGenericROISegmentation = _IsGenericROISegmentation;};
  // ParcellationMap
    void SetParcellationFile1(const char *_ParcellationFile1){std::strcpy(m_ParcellationFile1, _ParcellationFile1);};
    void SetParcellationFile2(const char *_ParcellationFile2){std::strcpy(m_ParcellationFile2, _ParcellationFile2);};
    void SetParcellationFile3(const char *_ParcellationFile3){std::strcpy(m_ParcellationFile3, _ParcellationFile3);};
    void SetParcellationMapSegmentation(bool _IsParcellationMapSegmentation){m_IsParcellationMapSegmentation = _IsParcellationMapSegmentation;};
    void SetSoftTissueMap(const char *_SoftTissueMap){std::strcpy(m_SoftTissueMap, _SoftTissueMap);};

  // Tab Advanced Parameters
    // Tissue Segmentation Parameters
    void SetEMSoftware(const char *_EMSoftware){std::strcpy(m_EMSoftware, _EMSoftware);};
    void SetFilterIterations(int _FilterIterations){m_FilterIterations = _FilterIterations;};
    void SetFilterTimeStep(float _FilterTimeStep){m_FilterTimeStep = _FilterTimeStep;};
    void SetFilterMethod(const char *_FilterMethod){std::strcpy(m_FilterMethod , _FilterMethod);};
    void SetMaxBiasDegree(int _MaxBiasDegree){m_MaxBiasDegree = _MaxBiasDegree;};
    void SetInitialDistributionEstimator(const char *_InitialDistributionEstimator){std::strcpy(m_InitialDistributionEstimator, _InitialDistributionEstimator);};
    void SetPrior1(float _Prior1){m_Prior1 = _Prior1;};
    void SetPrior2(float _Prior2){m_Prior2 = _Prior2;};
    void SetPrior3(float _Prior3){m_Prior3 = _Prior3;};
    void SetPrior4(float _Prior4){m_Prior4 = _Prior4;};
    void SetFluidAtlasWarp(bool _FluidAtlasWarp){m_FluidAtlasWarp = _FluidAtlasWarp;};
    void SetFluidAtlasAffine(bool _FluidAtlasAffine){m_FluidAtlasAffine = _FluidAtlasAffine;};
    void SetFluidAtlasFATW(bool _FluidAtlasFATW){m_FluidAtlasFATW = _FluidAtlasFATW;};
    void SetFluidAtlasWarpIterations(int _FluidAtlasWarpIterations){m_FluidAtlasWarpIterations = _FluidAtlasWarpIterations;};
    void SetFluidAtlasWarpMaxStep(float _FluidAtlasWarpMaxStep){m_FluidAtlasWarpMaxStep = _FluidAtlasWarpMaxStep;};
    void SetAtlasLinearMapping(const char *_AtlasLinearMapping){std::strcpy(m_AtlasLinearMapping,_AtlasLinearMapping);};
    void SetImageLinearMapping(const char *_ImageLinearMapping){std::strcpy(m_ImageLinearMapping,_ImageLinearMapping);};
    void SetLoop(bool _Loop){m_Loop=_Loop;};
    void SetAtlasLoop(const char *_AtlasLoop){std::strcpy(m_AtlasLoop,_AtlasLoop);};
    void SetLoopIteration(int _LoopIteration){m_LoopIteration=_LoopIteration;};
    // Reorientation Parameters
    void SetReorientation(bool _Reorientation){m_Reorientation = _Reorientation;};
    void SetInputDataOrientation(const char *_InputDataOrientation){std::strcpy(m_InputDataOrientation, _InputDataOrientation);};
    void SetOutputDataOrientation(const char *_OutputDataOrientation){std::strcpy(m_OutputDataOrientation, _OutputDataOrientation);};
    // N4ITKBiasFieldCorrection Parameters
    void SetN4ITKBiasFieldCorrection(bool _N4ITKBiasFieldCorrection){m_N4ITKBiasFieldCorrection=_N4ITKBiasFieldCorrection;};
    void SetNbOfIterations (const char * _NbOfIterations){std::strcpy(m_NbOfIterations,_NbOfIterations);};
    void SetBSplineGridResolutions (const char * _BSplineGridResolutions){std::strcpy(m_BSplineGridResolutions,_BSplineGridResolutions);};
    void SetConvergenceThreshold(float _ConvergenceThreshold){m_ConvergenceThreshold=_ConvergenceThreshold;};
    void SetSplineDistance(float _SplineDistance){m_SplineDistance=_SplineDistance;};
    void SetShrinkFactor(int _ShrinkFactor){m_ShrinkFactor=_ShrinkFactor;};
    void SetBSplineOrder(int _BSplineOrder){m_BSplineOrder=_BSplineOrder;};
    void SetBSplineAlpha(float _BSplineAlpha){m_BSplineAlpha=_BSplineAlpha;};
    void SetBSplineBeta(float _BSplineBeta){m_BSplineBeta=_BSplineBeta;};
    void SetHistogramSharpening(const char *  _HistogramSharpening){std::strcpy(m_HistogramSharpening,_HistogramSharpening);};

    // Rigid Registration Parameters
    void SetRigidRegistration(bool _RigidRegistration){m_RigidRegistration = _RigidRegistration;};
    void SetROIAtlasGridTemplate(bool _IsROIAtlasGridTemplate){m_IsROIAtlasGridTemplate = _IsROIAtlasGridTemplate;};
    void SetGridTemplateSizeX(int _GridTemplateSizeX){m_GridTemplateSizeX = _GridTemplateSizeX;};
    void SetGridTemplateSizeY(int _GridTemplateSizeY){m_GridTemplateSizeY = _GridTemplateSizeY;};
    void SetGridTemplateSizeZ(int _GridTemplateSizeZ){m_GridTemplateSizeZ = _GridTemplateSizeZ;};
    void SetGridTemplateSpacingX(float _GridTemplateSpacingX){m_GridTemplateSpacingX = _GridTemplateSpacingX;};
    void SetGridTemplateSpacingY(float _GridTemplateSpacingY){m_GridTemplateSpacingY = _GridTemplateSpacingY;};
    void SetGridTemplateSpacingZ(float _GridTemplateSpacingZ){m_GridTemplateSpacingZ = _GridTemplateSpacingZ;};
    void SetRegistrationInitialization(const char *_RegistrationInitialization){std::strcpy(m_RegistrationInitialization, _RegistrationInitialization);};
    void SetInitRegUseT1InitTransform(bool _InitRegUseT1InitTransform){m_InitRegUseT1InitTransform = _InitRegUseT1InitTransform;};

    // Warping Parameters
    void SetClassicWarpingMethod(bool _IsClassicWarpingMethod){m_IsClassicWarpingMethod = _IsClassicWarpingMethod;};
    void SetBRAINSDemonWarpMethod(bool _IsBRAINSDemonWarpMethod){m_IsBRAINSDemonWarpMethod = _IsBRAINSDemonWarpMethod;};
    void SetAlpha(float _Alpha){m_Alpha = _Alpha;};
    void SetBeta(float _Beta){m_Beta = _Beta;};
    void SetGamma(float _Gamma){m_Gamma = _Gamma;};
    void SetMaxPerturbation(float _MaxPerturbation){m_MaxPerturbation = _MaxPerturbation;};
    void SetNumBasis(float _NumBasis){m_NumBasis = _NumBasis;};
    void SetScale4NbIterations(int _Scale4NbIterations){m_Scale4NbIterations = _Scale4NbIterations;};
    void SetScale2NbIterations(int _Scale2NbIterations){m_Scale2NbIterations = _Scale2NbIterations;};
    void SetScale1NbIterations(int _Scale1NbIterations){m_Scale1NbIterations = _Scale1NbIterations;};
    void SetPyramidLevels(int _PyramidLevels){m_PyramidLevels = _PyramidLevels;};
    void SetMovingShrinkFactors(const char * _MovingShrinkFactors){std::strcpy(m_MovingShrinkFactors,_MovingShrinkFactors);};
    void SetFixedShrinkFactors(const char * _FixedShrinkFactors){std::strcpy(m_FixedShrinkFactors,_FixedShrinkFactors);};
    void SetIterationCountPyramidLevels(const char * _IterationCountPyramidLevels){std::strcpy(m_IterationCountPyramidLevels,_IterationCountPyramidLevels);};
    void SetDeformationFieldSmoothingSigma(float _DeformationFieldSmoothingSigma){m_DeformationFieldSmoothingSigma = _DeformationFieldSmoothingSigma;};
    void SetRegistrationFilterType(const char * _RegistrationFilterType){std::strcpy(m_RegistrationFilterType,_RegistrationFilterType);};
    // Skull Stripping
    void SetDeleteVessels(bool _DeleteVessels){m_DeleteVessels = _DeleteVessels;};
    // Intensity Rescaling
    void SetIntensityRescalingMethod(int _IntensityRescalingMethod){m_IntensityRescalingMethod = _IntensityRescalingMethod;};
    //Data
    void SetNbData(int _NbData){m_NbData = _NbData;};
    void SetNbAuxData(int _NbAuxData){m_NbAuxData = _NbAuxData;};
    void AllocateDataList();
    void AllocateAuxDataList();
    void DesallocateDataList();
    void DesallocateAuxDataList();
    void SetDataList(const char *_Data, int _DataNumber, bool _GUIMode);
    void SetAuxDataList(const char *_Data, int _DataNumber);
   // Regional histogram
    void SetQuantiles(const char *_Quantiles){std::strcpy(m_Quantiles, _Quantiles);};
    void SetPointSpacing(float _PointSpacing){m_PointSpacing = _PointSpacing;};
   // Compute Automatic Data
    void ComputeData();
  // Compute Automatic Segmentation
    void Computation();
    void ComputationWithoutGUI(const char *_computationFile, const char *_parameterFile);
    void StopBatchMake();
  // Show MRML Scene
    void ExecuteSlicer3withScene(std::string pathSlicer);
  // Load Files
    bool LoadParameterFile(const char *_FileName, enum Mode mode=file);
    void LoadComputationFile(const char *_FileName);
    void LoadAuxComputationFile(const char *_FileName);
  private:

    char m_AutoSegPath[512];
  
  // Tab Computation
    char *GetProcessDataDirectory(){return m_ProcessDataDirectory;};
     // Get T2Image: if T2image is computed, m_IsT2Image = 1 else 0
    bool GetT2Image(){return m_IsT2Image;};
     // Get PDImage: if PDfile, m_IsPDImage = 1 else 0
    bool GetPDImage(){return m_IsPDImage;};
		
    bool GetAuxT1Image(){return m_IsAuxT1Image;};
    bool GetAuxT2Image(){return m_IsAuxT2Image;};
    bool GetAuxPDImage(){return m_IsAuxPDImage;};
    bool GetAux1Image(){return m_IsAux1Image;};
    bool GetAux2Image(){return m_IsAux2Image;};
    bool GetAux3Image(){return m_IsAux3Image;};
    bool GetAux4Image(){return m_IsAux4Image;};
    bool GetAux5Image(){return m_IsAux5Image;};
    bool GetAux6Image(){return m_IsAux6Image;};
    bool GetAux7Image(){return m_IsAux7Image;};
    bool GetAux8Image(){return m_IsAux8Image;};

    //  Data AutoSeg Directory
    char *GetDataAutoSegDirectory(){return m_DataAutoSegDirectory;};
    // Automatic Data Selection
    char *GetDataDirectory(){return m_DataDirectory;};
    char *GetAuxDataDirectory(){return m_AuxDataDirectory;};
    char *GetT1(){return m_T1;};
    char *GetT2(){return m_T2;};
    char *GetPD(){return m_PD;};
    char *GetAux1(){return m_Aux1;};
    char *GetAux2(){return m_Aux2;};
    char *GetAux3(){return m_Aux3;};
    char *GetAux4(){return m_Aux4;};
    char *GetAux5(){return m_Aux5;};
    char *GetAux6(){return m_Aux6;};
    char *GetAux7(){return m_Aux7;};
    char *GetAux8(){return m_Aux8;};
    char *GetAux1Label(){return m_Aux1Label;};
    char *GetAux2Label(){return m_Aux2Label;};
    char *GetAux3Label(){return m_Aux3Label;};
    char *GetAux4Label(){return m_Aux4Label;};
    char *GetAux5Label(){return m_Aux5Label;};
    char *GetAux6Label(){return m_Aux6Label;};
    char *GetAux7Label(){return m_Aux7Label;};
    char *GetAux8Label(){return m_Aux8Label;};

    // Computation Options
    bool GetComputeVolume(){return m_ComputeVolume;};
    bool GetComputeCorticalThickness(){return m_ComputeCorticalThickness;};
    bool GetRecompute(){return m_Recompute;};
    bool GetUseCondor(){return m_UseCondor;};
		
    // Regional Histogram Options
    bool GetAtlasSpaceImage(){return m_AtlasSpaceImage;};
    bool GetBiasCorrectedImage(){return m_BiasCorrectedImage;};
    bool GetSkullStrippedImage(){return m_SkullStrippedImage;};
    bool GetRigidTransformation(){return m_RigidTransformation;};
    bool GetAffineTransformation(){return m_AffineTransformation;};
    bool GetBsplineTransformation(){return m_BsplineTransformation;};
		
  
  // Tab Parameters
    // Atlases
    char *GetCommonCoordinateImage(){return m_CommonCoordinateImage;};
    char *GetTissueSegmentationAtlasDirectory(){return m_TissueSegmentationAtlasDirectory;};
    char *GetROIAtlasFile(){return m_ROIAtlasFile;};
    char *GetTissueSegmentationAtlasType(){return m_TissueSegmentationAtlasType;};
    char *GetCommonCoordinateImageType(){return m_CommonCoordinateImageType;};
    // Probabilistic Subcortical Structures Parameters
    char *GetAmygdalaLeft(){return m_AmygdalaLeft;};
    char *GetAmygdalaRight(){return m_AmygdalaRight;};
    char *GetCaudateLeft(){return m_CaudateLeft;};
    char *GetCaudateRight(){return m_CaudateRight;};
    char *GetHippocampusLeft(){return m_HippocampusLeft;};
    char *GetHippocampusRight(){return m_HippocampusRight;};
    char *GetPallidusLeft(){return m_PallidusLeft;};
    char *GetPallidusRight(){return m_PallidusRight;};
    char *GetPutamenLeft(){return m_PutamenLeft;};
    char *GetPutamenRight(){return m_PutamenRight;};
    char *GetLateralVentricleLeft(){return m_LateralVentricleLeft;};
    char *GetLateralVentricleRight(){return m_LateralVentricleRight;};
    bool GetSubcorticalStructureSegmentation(){return m_IsSubcorticalStructureSegmentation;};
    // Generic ROI Map
    char *GetROIFile1(){return m_ROIFile1;};
    char *GetROIFile2(){return m_ROIFile2;};
    char *GetROIFile3(){return m_ROIFile3;};
    char *GetROIFile4(){return m_ROIFile4;};
    char *GetROIFile5(){return m_ROIFile5;};
    bool GetGenericROISegmentation(){return m_IsGenericROISegmentation;};
  // Parcellation Map
    char *GetParcellationFile1(){return m_ParcellationFile1;};
    char *GetParcellationFile2(){return m_ParcellationFile2;};
    char *GetParcellationFile3(){return m_ParcellationFile3;};
    bool GetParcellationMapSegmentation(){return m_IsParcellationMapSegmentation;};
    char *GetSoftTissueMap(){return m_SoftTissueMap;};

  // Tab Advanced Parameters
    //Tissue Segmentation Parameters
    char *GetEMSoftware(){return m_EMSoftware;};
    int GetFilterIterations(){return m_FilterIterations;};
    float GetFilterTimeStep(){return m_FilterTimeStep;};
    char *GetFilterMethod(){return m_FilterMethod;};
    int GetMaxBiasDegree(){return m_MaxBiasDegree;};
    char *GetInitialDistributionEstimator(){return m_InitialDistributionEstimator;};
    float GetPrior1(){return m_Prior1;};
    float GetPrior2(){return m_Prior2;};
    float GetPrior3(){return m_Prior3;};
    float GetPrior4(){return m_Prior4;};
    bool GetBSplineAtlasWarp(){return m_BSplineAtlasWarp;};
    int GetBSplineAtlasWarpGridX(){return m_BSplineAtlasWarpGridX;};
    int GetBSplineAtlasWarpGridY(){return m_BSplineAtlasWarpGridY;};
    int GetBSplineAtlasWarpGridZ(){return m_BSplineAtlasWarpGridZ;};
    bool GetFluidAtlasWarp(){return m_FluidAtlasWarp;};
    bool GetFluidAtlasAffine(){return m_FluidAtlasAffine;};
    bool GetFluidAtlasFATW(){return m_FluidAtlasFATW;};
    int GetFluidAtlasWarpIterations(){return m_FluidAtlasWarpIterations;};
    float GetFluidAtlasWarpMaxStep(){return m_FluidAtlasWarpMaxStep;};
    char *GetAtlasLinearMapping(){return m_AtlasLinearMapping;};
    char *GetImageLinearMapping(){return m_ImageLinearMapping;};
    bool GetLoop(){return m_Loop;};
    char *GetAtlasLoop(){return m_AtlasLoop;};
    int GetLoopIteration(){return m_LoopIteration;};
    // Reorientation
    bool GetReorientation(){return m_Reorientation;};
    char *GetInputDataOrientation(){return m_InputDataOrientation;};
    char *GetOutputDataOrientation(){return m_OutputDataOrientation;};
    // N4ITKBiasFieldCorrection Parameters
    bool GetN4ITKBiasFieldCorrection(){return m_N4ITKBiasFieldCorrection;};
    char * GetNbOfIterations() {return m_NbOfIterations;};
    char * GetBSplineGridResolutions() {return m_BSplineGridResolutions;};
    float GetConvergenceThreshold(){return m_ConvergenceThreshold;};
    float GetSplineDistance(){return m_SplineDistance;};
    int GetShrinkFactor(){return m_ShrinkFactor;};
    int GetBSplineOrder(){return m_BSplineOrder;};
    float GetBSplineAlpha(){return m_BSplineAlpha;};
    float GetBSplineBeta(){return m_BSplineBeta;};
    char * GetHistogramSharpening(){return m_HistogramSharpening;};
    // Rigid Registration Parameters
    bool GetRigidRegistration(){return m_RigidRegistration;};
    bool GetROIAtlasGridTemplate(){return m_IsROIAtlasGridTemplate;};
    int GetGridTemplateSizeX(){return m_GridTemplateSizeX;};
    int GetGridTemplateSizeY(){return m_GridTemplateSizeY;};
    int GetGridTemplateSizeZ(){return m_GridTemplateSizeZ;};
    float GetGridTemplateSpacingX(){return m_GridTemplateSpacingX;};
    float GetGridTemplateSpacingY(){return m_GridTemplateSpacingY;};
    float GetGridTemplateSpacingZ(){return m_GridTemplateSpacingZ;};	
    char *GetRegistrationInitialization(){return m_RegistrationInitialization;};
    bool GetInitRegUseT1InitTransform(){return m_InitRegUseT1InitTransform;};
    // Warping Parameters
    bool GetClassicWarpingMethod(){return m_IsClassicWarpingMethod;};
    bool GetBRAINSDemonWarpMethod(){return m_IsBRAINSDemonWarpMethod;};
    float GetAlpha(){return m_Alpha;};
    float GetBeta(){return m_Beta;};
    float GetGamma(){return m_Gamma;};
    float GetMaxPerturbation(){return m_MaxPerturbation;};
    float GetNumBasis(){return m_NumBasis;};
    int GetScale4NbIterations(){return m_Scale4NbIterations;};
    int GetScale2NbIterations(){return m_Scale2NbIterations;};
    int GetScale1NbIterations(){return m_Scale1NbIterations;};
    int GetPyramidLevels(){return m_PyramidLevels;};
    char *GetMovingShrinkFactors(){return m_MovingShrinkFactors;};
    char *GetFixedShrinkFactors(){return m_FixedShrinkFactors;};
    char *GetIterationCountPyramidLevels(){return m_IterationCountPyramidLevels;};
    float GetDeformationFieldSmoothingSigma(){return m_DeformationFieldSmoothingSigma;};
    char *GetRegistrationFilterType(){return m_RegistrationFilterType;};
    // Skull Stripping
    bool GetDeleteVessels(){return m_DeleteVessels;};
    // Intensity Rescaling
    int GetIntensityRescalingMethod(){return m_IntensityRescalingMethod;};
    // Regional histogram
    char *GetQuantiles(){return m_Quantiles;};
    float GetPointSpacing(){return m_PointSpacing;};
    // Get Private Parameters
    char *GetBMSAutoSegMainFile(){return m_BMSAutoSegMainFile;};
    char *GetBMSAutoSegAuxFile(){return m_BMSAutoSegAuxFile;};
    char *GetBMSAutoSegFile(){return m_BMSAutoSegFile;};
    char *GetBMSDataFile(){return m_BMSDataFile;};
    char *GetBMSAutoSegMRMLSourceFile(){return m_BMSAutoSegMRMLSourceFile;};
    char *GetBMSAutoSegMRMLParcelFile(){return m_BMSAutoSegMRMLParcelFile;};
    char *GetBMSAutoSegMRMLAllROIFile(){return m_BMSAutoSegMRMLAllROIFile;};
    char *GetParameterFile(){return m_ParameterFile;};
    char *GetComputationFile(){return m_ComputationFile;};
    char *GetLogFile(){return m_LogFile;};
    // Volume Analysis
    char *GetSubcorticalStructuresVolumeFile(){return m_SubcorticalStructuresVolumeFile;};
    char *GetGenericROIMapVolumeFile(){return m_GenericROIMapVolumeFile;};
    char *GetParcellationMapVolumeFileWM(){return m_ParcellationMapVolumeFileWM;};
    char *GetParcellationMapVolumeFileGM(){return m_ParcellationMapVolumeFileGM;};
    char *GetParcellationMapVolumeFileCSF(){return m_ParcellationMapVolumeFileCSF;};
    char *GetTissueSegmentationVolumeFile(){return m_TissueSegmentationVolumeFile;};
    // Set Private Parameters
    void SetBMSAutoSegFile();
    void SetBMSAutoSegMainFile();
    void SetBMSAutoSegAuxFile();
    void SetBMSDataFile();
    void SetBMSAutoSegMRMLSourceFile();
    void SetBMSAutoSegMRMLParcelFile();
    void SetBMSAutoSegMRMLAllROIFile();
    void SetDataFile();
    void SetAuxDataFile();
    void SetParameterFile();
    void SetParameterFile(const char *_ParameterFile);
    void SetComputationFile();
    void SetComputationFile(const char *_ComputationFile);
    void SetLogFile();
    // Volume Analysis
    void SetSubcorticalStructuresVolumeFile();
    void SetGenericROIMapVolumeFile();
    void SetParcellationMapVolumeFileWM();
    void SetParcellationMapVolumeFileGM();
    void SetParcellationMapVolumeFileCSF();
    void SetTissueSegmentationVolumeFile();
    void SetData(const char *_Data, char *_T1);
    void SetData(const char *_Data, char *_T1, char *_SecondImage);
    void SetData(const char *_Data, char *_T1, char *_T2, char *_PD);
    void SetDatawoGUI(const char *_Data, char *_T1, char *_SecondImage);
    void SetDatawoGUI(const char *_Data, char *_T1, char *_T2, char *_PD);
    void SetAuxData(const char *_Data, char *_T1);
    void SetAuxData(const char *_Data, char *_T1, char *_SecondImage);
    void SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2);
    void SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3);
    void SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4);
    void SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5);
    void SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5, char *_Aux6);
    void SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5, char *_Aux6, char *_Aux7);
    void SetAuxData(const char *_Data, char *_T1, char *_Aux1, char *_Aux2, char *_Aux3, char *_Aux4, char *_Aux5, char *_Aux6, char *_Aux7, char *_Aux8);
    int GetNbData(){return m_NbData;};
    int GetNbAuxData(){return m_NbAuxData;};
    void DeleteSpaces(char *_Label);
		
  // Write BatchMake Files  
    void WriteBMSAutoSegFile();
    void WriteBMSAutoSegMainFile();
    void WriteBMSAutoSegAuxFile();
    void WriteBMSDataFile();
    void WriteBMSAutoSegMRMLSourceFile();
    void WriteBMSAutoSegMRMLParcelFile();
    void WriteBMSAutoSegMRMLAllROIFile();

		
  // Execute BatchMake Script to compute Automatic Segmentation
    void ExecuteBatchMake(char *_Input, int _GUIMode);  


  // Output Files (saved in ProcessDataDirectory)
   // Data File (.txt)
    char m_DataFile[512];
    char m_AuxDataFile[512];
   // BatchMake Script to compute Data  
    char m_BMSDataFile[512];
   // BatchMake Script to compute Automatic Segmentation
    char m_BMSAutoSegMainFile[512];
    char m_BMSAutoSegAuxFile[512];
    char m_BMSAutoSegFile[512];
    char m_BMSAutoSegMRMLSourceFile[512];
    char m_BMSAutoSegMRMLParcelFile[512];
    char m_BMSAutoSegMRMLAllROIFile[512];
   // Log File
    std::string m_output;
    char m_LogFile[512];
  // Parameter File
    char m_ParameterFile[512];
    char m_ComputationFile[512];
    char m_AuxComputationFile[512];
    char m_SubcorticalStructuresVolumeFile[512];
    char m_GenericROIMapVolumeFile[512];
    char m_ParcellationMapVolumeFileWM[512];
    char m_ParcellationMapVolumeFileGM[512];
    char m_ParcellationMapVolumeFileCSF[512];
    char m_TissueSegmentationVolumeFile[512];
    bool m_IsAutoSegInProcess;

  //Tab Computation
    char m_ProcessDataDirectory[512];
    // Data AutoSeg Directory
    char m_DataAutoSegDirectory[100];
    // Automatic Data Selection
    char m_DataDirectory[512];
    char m_AuxDataDirectory[512];
    char m_T1[100];
    char m_T2[100];
    char m_PD[100];
    char m_Aux1[100];
    char m_Aux2[100];
    char m_Aux3[100];
    char m_Aux4[100];
    char m_Aux5[100];
    char m_Aux6[100];
    char m_Aux7[100];
    char m_Aux8[100];
    char m_Aux1Label[100];
    char m_Aux2Label[100];
    char m_Aux3Label[100];
    char m_Aux4Label[100];
    char m_Aux5Label[100];
    char m_Aux6Label[100];
    char m_Aux7Label[100];
    char m_Aux8Label[100];
		
    // Computation Options
    bool m_ComputeVolume;
    bool m_ComputeCorticalThickness;
    bool m_Recompute; // Recompute all
    bool m_UseCondor;

    // Regional Histogram Options
    bool m_AtlasSpaceImage;
    bool m_BiasCorrectedImage;
    bool m_SkullStrippedImage;
    bool m_RigidTransformation;
    bool m_AffineTransformation;
    bool m_BsplineTransformation;

    // Display
    TextDisplayGUIControls TextDisplay;
    Fl_Text_Buffer m_TextBuf;
  
  // Tab Parameters
    // Atlases
    char m_CommonCoordinateImage[512];
    char m_TissueSegmentationAtlasDirectory[512];
    char m_ROIAtlasFile[512];
    char m_TissueSegmentationAtlasType[3];
    char m_CommonCoordinateImageType[3];
    // Probabilistic Subcortical Structures
    char m_AmygdalaLeft[512];
    char m_AmygdalaRight[512];
    char m_CaudateLeft[512];
    char m_CaudateRight[512];
    char m_HippocampusLeft[512];
    char m_HippocampusRight[512];
    char m_PallidusLeft[512];
    char m_PallidusRight[512];
    char m_PutamenLeft[512];
    char m_PutamenRight[512];
    char m_LateralVentricleLeft[512];
    char m_LateralVentricleRight[512];
    bool m_IsSubcorticalStructureSegmentation;
    // Generic ROI Map
    char m_ROIFile1[512];
    char m_ROIFile2[512];
    char m_ROIFile3[512];
    char m_ROIFile4[512];
    char m_ROIFile5[512];
    bool m_IsGenericROISegmentation;
    // Parcellation Map
    char m_ParcellationFile1[512];
    char m_ParcellationFile2[512];
    char m_ParcellationFile3[512];
    bool m_IsParcellationMapSegmentation;
    char m_SoftTissueMap[10];
  
  //Tab Advanced Parameters
    // Tissue Segmentation Parameters
    char m_EMSoftware[20];
    int m_FilterIterations;
    float m_FilterTimeStep;
    char m_FilterMethod[50];
    int m_MaxBiasDegree;
    char m_InitialDistributionEstimator[50];
    float m_Prior1;
    float m_Prior2;
    float m_Prior3;
    float m_Prior4;
    bool m_BSplineAtlasWarp;
    int m_BSplineAtlasWarpGridX;
    int m_BSplineAtlasWarpGridY;
    int m_BSplineAtlasWarpGridZ;
    bool m_FluidAtlasWarp;
    bool m_FluidAtlasAffine;
    bool m_FluidAtlasFATW;
    int m_FluidAtlasWarpIterations;
    float m_FluidAtlasWarpMaxStep;
    char m_AtlasLinearMapping[50];
    char m_ImageLinearMapping[50];
    char m_AtlasLoop[512];
    bool m_Loop;
    int m_LoopIteration;
    // Reorientation Parameters
    bool m_Reorientation;
    char m_InputDataOrientation[4];
    char m_OutputDataOrientation[4];
    // N4ITKBiasFieldCorrection Parameters
    bool m_N4ITKBiasFieldCorrection;
    char m_NbOfIterations[50];
    char m_BSplineGridResolutions[50];
    float m_ConvergenceThreshold;
    float m_SplineDistance;
    int m_ShrinkFactor;
    int m_BSplineOrder;
    float m_BSplineAlpha;
    float m_BSplineBeta;
    char m_HistogramSharpening[50];
    // Rigid Registration Parameters
    bool m_RigidRegistration;
    bool m_IsROIAtlasGridTemplate;
    int m_GridTemplateSizeX;
    int m_GridTemplateSizeY;
    int m_GridTemplateSizeZ;
    float m_GridTemplateSpacingX;
    float m_GridTemplateSpacingY;
    float m_GridTemplateSpacingZ;
    char m_RegistrationInitialization[50];
    bool m_InitRegUseT1InitTransform;
    // Warping Parameters
    bool m_IsClassicWarpingMethod;
    bool m_IsBRAINSDemonWarpMethod;
    float m_Alpha;
    float m_Beta;
    float m_Gamma;
    float m_MaxPerturbation;
    float m_NumBasis;
    int m_Scale4NbIterations;
    int m_Scale2NbIterations;
    int m_Scale1NbIterations;
    int m_PyramidLevels;
    char m_MovingShrinkFactors[50];
    char m_FixedShrinkFactors[50];
    char m_IterationCountPyramidLevels[50];
    float m_DeformationFieldSmoothingSigma;
    char m_RegistrationFilterType[50];
    // Skull Stripping
    bool m_DeleteVessels;
    // Intensity Rescaling
    int m_IntensityRescalingMethod;
    // Regional histogram		
    float m_PointSpacing;
    char m_Quantiles[100];
  // Data to be computed
   // Number of data
    int m_NbData;
    int m_NbAuxData;
   // Allocation of data
    int m_AllocationData;
    int m_AllocationAuxData;
   // 1 if T2 Images are computed
    bool m_IsT2Image;
   // 1 if PD Images are computed
    bool m_IsPDImage;
		
    bool m_IsAuxT1Image;
    bool m_IsAuxT2Image;
    bool m_IsAuxPDImage;
    bool m_IsAux1Image;
    bool m_IsAux2Image;
    bool m_IsAux3Image;
    bool m_IsAux4Image;
    bool m_IsAux5Image;
    bool m_IsAux6Image;
    bool m_IsAux7Image;
    bool m_IsAux8Image;

   // T1 Data List: contains all the T1 files 
    char **m_T1List;
   // T2 Data List: contains all the T2 files 
    char **m_T2List;
   // PD Data List: contains all the PD files 
    char **m_PDList;
		
    char **m_AuxT1List;
    char **m_AuxT2List;
    char **m_AuxPDList;
    char **m_Aux1List;
    char **m_Aux2List;
    char **m_Aux3List;
    char **m_Aux4List;
    char **m_Aux5List;
    char **m_Aux6List;
    char **m_Aux7List;
    char **m_Aux8List;

    itksysProcess* m_Process;
    bool m_KillProcess;
    bool m_Manually;
};

#endif
