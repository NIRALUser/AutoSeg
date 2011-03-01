#ifndef boundary_H
#define boundary_H

#include <itkImageRegionIteratorWithIndex.h>
#include <itkImage.h>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

/*! \brief class to calculate the white boundary and the gray boundary.
*
* the function to calculate white boundary needs the white segmented image
* the function to calculate the outside graymatter boundary needs white and gray matter segmented image
*/

class boundary
{
protected:
        
private:
  typedef float PixelType;
  typedef itk::Image< PixelType, 3 > ImageType;
  typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;
  typedef itk::ImageFileReader< ImageType  >  ReaderType;
  typedef itk::ImageFileWriter< ImageType >  WriterType;
	typedef ImageType::Pointer ImagePointer;
        
  ImageType::Pointer m_BoundaryW_Image;
  ImageType::Pointer m_BoundaryG_Image;

public:
  
  /** Calculate the white boundary */      
  void BoundaryWhite(ImagePointer );

  /** Calculate the Grey Boundary */
  void BoundaryGray(ImagePointer, ImagePointer);

  ImagePointer Get_Boundary_White() { return m_BoundaryW_Image;}              
  ImagePointer Get_Boundary_Gray()  { return m_BoundaryG_Image;} 

  /** Write White and Grey Boundary Image */                     
  void Write_Boundary_Image(char *, char *);
        
  boundary(){}
  ~boundary(){}

        
};

#endif
