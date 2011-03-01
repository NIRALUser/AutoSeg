
#ifndef toolsdistance_H
#define toolsdistance_H

#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


#include "itkImageRegionIteratorWithIndex.h"
#include "itkImage.h"
#include "itk_hash_map.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkConnectedComponentImageFilter.h"
#include <itkRelabelComponentImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkOffset.h"
#include "itkIndex.h"
#include "itkImageDuplicator.h"

#include "Tableau1D.h"
#include "Tableau2D.h"
#include "Neighborhood.h"
#include "CalculateAngle.h"
#include "HashMapDistance.h"
#include "ImageSyl.h"
#include "Vector3D.h"
#include "itkVector.h"
//#include "PipelineCommand.h"


/*! \brief tools to calcul VectorMap and distanceMap.
*
* For each gray matter point, the closest white matter point is calculate and for each white matter point, 
* the furthest gray matter point is calculate which pointed on the closest white matter point.
* 
*/

class toolsdistance
{
protected:

  typedef float PixelType;
  typedef itk::Image< PixelType, 3 > ImageType;
  typedef itk::Image< PixelType, 6 > ImageType6;
  typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;
  typedef itk::ImageFileWriter< ImageType >  WriterType;
  typedef itk::ImageFileReader< ImageType >  ReaderType;
  typedef itk::ConnectedComponentImageFilter < ImageType, ImageType > conCompFilterType;
  typedef itk::RelabelComponentImageFilter < ImageType, ImageType > relabelFilterType;
  typedef itk::BinaryThresholdImageFilter < ImageType, ImageType > binThreshFilterType;
  typedef itk::MinimumMaximumImageCalculator <ImageType> maxImageCalculatorType;
  typedef ImageType::Pointer ImagePointer;
  typedef std::vector<float> VectordistType;
  typedef itk::DanielssonDistanceMapImageFilter < ImageType , ImageType > DanielssonDMFilter;
  typedef itk::ImageRegionIterator< ImageType > RegionIteratorType;
  typedef itk::Offset< 3 > OffsetVector;
  typedef DanielssonDMFilter::VectorImageType   OffsetImageType;
  typedef itk::Index< 3 > IndexType;
  typedef itk::ImageDuplicator< ImageType >  DuplicatorType;
  
  typedef struct
  {
    int name;
    VectordistType dist;
  }IndexVectType;
   
        
private:
	HashMapDistance p_HashMap;
  ImageType::Pointer m_NeighDistanceW;
  ImageType::Pointer m_NeighDistanceG;
  ImageType::Pointer m_GrayImageAfterdel;

  ImagePointer CreateNewImageLike(ImagePointer);  

  int m_CompIns;
  int m_CompBound;               

public:                
  toolsdistance(void);
  ~toolsdistance(void);
  
  ImageType::Pointer m_InsideFoldsThicknessIm;
  ImageType::Pointer m_BoundaryThicknessIm;
  char  m_insideFileName[512];
  char  m_BoundaryFileName[512];
         
  /** This function calculate the distances and map distances on gray matter and on white matter */
   void calculdistance(ImageType::Pointer GrayMatterBoundaryImage,ImageType::Pointer outputImageWM,ImageType::Pointer outputImageGM, ImageType::Pointer WhiteMatterBoundaryImage, ImageType::Pointer WhiteMatterImage, ImageType::Pointer GrayMatterImage,ImageType::Pointer DistanceMapImage );
        
	/** This function permit to fill whitematter point with no distance with the average of its neighborhood */
  void AverageNeighborhood(ImageType::Pointer outputImageWM, ImageType::Pointer outputImageWMBoundary);
	
	/** To delete point on gray matter to produce the squeletal of the brain */
  void deletepoint(ImageType::Pointer GrayMatter,ImageType::Pointer GrayMatterBoundaryImage, ImageType::Pointer DistanceMap);
  
	/** Write the squeletal Image of the brain */	
  void Write_Gray_After_Delete(ImageType::Pointer GrayImage, char * Image, bool);
  
  /** To use VectorViz, a .viz file should be created where vectors are stored. */           
  void GenerateVectorImage(ImageType::Pointer GrayMatterImage, const char* file);
  
  /** To vizualise cortical thickness on grey matter surface by using MeshCortThick and after MeshVisu, you have to create a image which is the DistanceMap maps on grey matter. */
  void WriteGreyDanielsson(ImageType::Pointer, ImageType::Pointer, const char*);
  
  /** This function create the default output, it writes a file where the white matter distance map is store, If there is a parcellation file, it is stored by label */
  void WriteWhiteFile(ImageType::Pointer, ImageType::Pointer,char* ParFileName, char* FileNameWhite, bool, bool, float);
        
  HashMapDistance::VectorstructType GetHashMap(ImageType::IndexType IndexG) { return p_HashMap.Get_HashMAp_G(IndexG); }
 
  /** Not use : it is a neigborhood algo for calculate the max distance in the neigborhood, in this case is a 3 neigh wich means the distance max is 3 voxels     */ 
  void ComputeNeigborhoodDistance(ImageType::Pointer GrayImage, ImageType::Pointer DistanceMap, ImageType::Pointer BoundaryWhite);

  /** To separate cortex inside folds versus cortex at the boundary and write it like ???? */
  void SepareInsideVSBoundary(ImageType::Pointer GreyBoundaryImage, ImageType::Pointer DistanceMapGrey, char *);
 
 ImageType::Pointer Get_NeighDistanceW(){return m_NeighDistanceW;}
 ImageType::Pointer Get_NeighDistanceG(){return m_NeighDistanceG;}
 
 /** Write floating point pixel type image */
 void WriteImage(ImageType::Pointer, const char *);

 /** Count number of white matter and grey matter point */
 void CounterWhiteGrey(ImageType::Pointer, ImageType::Pointer);


};

#endif        
