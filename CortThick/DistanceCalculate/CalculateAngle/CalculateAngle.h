
#ifndef CalculateAngle_H
#define CalculateAngle_H

#include <itkImageRegionIteratorWithIndex.h>
#include <itkImage.h>
#include "Tableau1D.h"
#include <math.h>

/*! \brief  class to calculate max distance in an Image, the angle between to vector and the angle 
* min depending on the distance.
*/
 
class CalculateAngle
{
protected:
  typedef float PixelType;
  typedef itk::Image< PixelType, 3 > ImageType;
  typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;
public:
  CalculateAngle(void);
  ~CalculateAngle(void);
  float Get_Dmax();
	/** this function calculate The distance Max in an Image according to the Danielsson distance Map */
  void Calcul_Dmax(ImageType::Pointer GreyMatter, ImageType::Pointer DistanceMap);

  float Get_Angle();

	/** this function calculate the angle depending on the distance whith an exponentiel function */
  void calcul_Angle(const ImageType::SpacingType& sp, float Distance);

	/** this function calculate the angle between two vector and return the acos */
  float CalculAngleVector(ImageType::IndexType IndexWM2,ImageType::IndexType IndexGM, ImageType::IndexType V1);        

        
private:
  float m_Dmax;
  float m_Angle;
  float min(float a, float b) {if(a<b) return a; else return b;}
};

#endif        


