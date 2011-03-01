#ifndef IMAGESYL_H
#define IMAGESYL_H

#include <iostream>

#include "boundary.h"
#include "toolsdistance.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarConnectedComponentImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkSignedDanielssonDistanceMapImageFilter.h"
#include "itkConvertPixelBuffer.h"

/*! \brief Class which called functions from other class to calculate and visualize thickness or images
*
*  
* 
*/

const unsigned int Dimension = 3;
typedef float PixelType;
typedef unsigned char OverlayPixelType;
typedef itk::Image<PixelType,Dimension> ImageType;
typedef itk::Image<OverlayPixelType,Dimension> OverlayType;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileReader<OverlayType> OverlayReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;
typedef ImageType::Pointer ImagePointer;
typedef OverlayType::Pointer OverlayPointer;
typedef ReaderType::Pointer ReaderPointer;
typedef OverlayReaderType::Pointer OverlayReaderPointer;
typedef WriterType::Pointer WriterPointer;
typedef itk::ScalarConnectedComponentImageFilter < ImageType, OverlayType > ScalarConnectiveFilterType;
typedef itk::RelabelComponentImageFilter < OverlayType, ImageType > relabelFilterType;
typedef itk::BinaryThresholdImageFilter < ImageType, ImageType > binThreshFilterType;
typedef itk::SignedDanielssonDistanceMapImageFilter<ImageType,ImageType >  DanielsonFilterType;
typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;

class ImageSyl{

public:

  ImageSyl();
  ~ImageSyl();
  
  void SetFileName(const char *N);
  void SetOverlayFileName(const char* OverlayN);

  const char * GetFileName();
  const char * GetOverlayFileName();
  
  ImageType::SpacingType GetImageSpacing();
  ImageType::SizeType GetImageSize();
  
  void LoadImage();
  void LoadOverlay();
  
  ImagePointer GetImageSyl();
  void SetImageSyl(ImagePointer);
  OverlayPointer GetOverlaySyl();

  ImagePointer ConnectiveComponent(ImagePointer);
  void SaveImage(const char *);
  
  void ComputeDM(const char *,bool);
  
  ImagePointer GetDistanceMap();
  
  void LoadGreyMatterFile(const char *);
  void LoadWhiteMatterFile(const char *);
  void LoadDanielssonMapFile(const char *);
  void SegImageWhiteAndGrey(const char *, double, double);

  void BoundaryComputation();
  
  void GetImageProp(ImagePointer Im);
  
  ImagePointer GetWhiteMatterImage();
  ImagePointer GetGreyMatterImage();
  void SetWhiteMatterImage(ImagePointer);
  void SetGreyMatterImage(ImagePointer);

  void WriteDanielsson(char*);
  void ComputeDistance(char*, char*,char*, char*, char* ,char* ,bool, bool, bool, bool, bool, bool, char* ,char* , bool, bool,bool,float);

 private:

  const char* m_ImageFileName;
  const char* m_OverlayImageName;
  
  ImagePointer m_ImageDisp;
  OverlayPointer m_OverlayDisp;
  
  ReaderPointer m_reader;
  OverlayReaderPointer m_overlayreader;
  
  ImagePointer m_OriginalImage;  
  
  ImagePointer m_GreyMatterImage;
  ImagePointer m_WhiteMatterImage;
  ImagePointer m_ImageSeg;
  
  ImagePointer m_GreyMatterBoundaryImage;
  ImagePointer m_WhiteMatterBoundaryImage;
  
  ImagePointer m_GreyMatterDistanceMap;
  ImagePointer m_WhiteMatterDistanceMap;
    
  ImagePointer m_DistanceMapImage;

  ImageType::SpacingType m_sp;
  ImageType::SizeType m_SizeImage;
  
  ImagePointer CreateNewImageLike(ImagePointer);
  

};


#endif 
