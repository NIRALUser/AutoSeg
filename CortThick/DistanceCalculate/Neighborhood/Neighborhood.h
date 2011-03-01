
#ifndef Neighborhood_H
#define Neighborhood_H

#include <itkImage.h>
#include <itkImageRegionIteratorWithIndex.h>

/*! \brief Brief Description: class to find the neigborhood.
*
* this class permit to fill a temporary region which correspond to a level neigborhood which is define in the method *NeighborhoodImage
*/
class Neighborhood
{
protected:
  typedef float PixelType;
  typedef itk::Image< PixelType, 3 > ImageType;
  typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;
        
public:
   Neighborhood(void){}
   ~Neighborhood(void){}
	/**this function calculate the neigborhoodImage, which correspond to the declaration of a temporary region */
  void NeighborhoodImage(ImageType::Pointer InputImage, ImageType::IndexType , int level);
  ImageType::Pointer m_TemporaryRegion;
        
};        
        
        
#endif
