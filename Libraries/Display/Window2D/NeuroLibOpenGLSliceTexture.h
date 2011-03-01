/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: NeuroLibOpenGLSliceTexture.h,v $
  Language:  C++
  Date:      $Date: 2005/04/02 12:07:26 $
  Version:   $Revision: 1.2 $
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __NeuroLibOpenGLSliceTexture_h_
#define __NeuroLibOpenGLSliceTexture_h_

//#include <FL/gl.h>
#include <GL/glu.h>

#include "itkImage.h"
#include "itkPixelTraits.h"
#include "itkRGBAPixel.h"
#include "itkConstantPadImageFilter.h"
#include "vnl/vnl_vector_fixed.h"
/**
 * \class OpenGLSliceTexture
 * \brief This class is used to turn a 2D ITK image of (arbitrary) type
 * into a GL texture.  
 *
 * The calls to Update will make sure that the texture is up to date.  
 */
template<class TPixel> class NeuroLibOpenGLSliceTexture 
{
public:
  // Image typedefs
  typedef itk::Image<TPixel,2> ImageType;
  typedef typename itk::SmartPointer<ImageType> ImagePointer;

  /** Constructor, initializes the texture object */
  NeuroLibOpenGLSliceTexture();

  /** Destructor, deallocates texture memory */
  virtual ~NeuroLibOpenGLSliceTexture();
  
  /** Pass in a pointer to a 2D image */
  void SetImage(ImagePointer inImage);

  /** Get the dimensions of the texture image, which are powers of 2 */
//  irisGetMacro(TextureSize,Vector2ui);

  /** Get the GL texture number automatically allocated by this object */
  //irisGetMacro(TextureIndex,int);

  /** Set the number of components used in call to glTextureImage */
  void SetGlComponents(GLuint GlComponents)
  {
    m_GlComponents = GlComponents;
  }

  /** Get the format (e.g. GL_LUMINANCE) in call to glTextureImage */
  void SetGlFormat(GLenum GlFormat)
  {
    m_GlFormat = GlFormat;
  }

  /** Get the type (e.g. GL_UNSIGNED_INT) in call to glTextureImage */
  void SetGlType(GLenum GlType)
  {
    m_GlType = GlType;
  }

  /**
   * Make sure that the texture is up to date (reflects the image)
   */
  void Update();

  /**
   * Draw the texture in the current OpenGL context on a polygon with vertices
   * (0,0) - (size_x,size_y). Paramters are the background color of the polygon
   */
  void Draw();

  /**
   * Draw the texture in transparent mode, with given level of alpha blending.
   */
  void DrawTransparent(unsigned char alpha);

private:
  
  // Filter typedefs
  typedef itk::ConstantPadImageFilter<ImageType,ImageType> FilterType;
  typedef typename itk::SmartPointer<FilterType> FilterPointer;

  // The dimensions of the texture as stored in memory
   unsigned int m_TextureSize[2];

  // The pointer to the image from which the texture is computed
  ImagePointer m_Image;

  // The padding filter used to resize the image
  FilterPointer m_PadFilter;

  // The texture number (index)
  GLuint m_TextureIndex;

  // Has the texture been initialized?
  bool m_IsTextureInitalized;

  // The pipeline time of the source image (vs. our pipeline time)
  unsigned long m_UpdateTime;

  // The number of components for Gl op
  GLuint m_GlComponents;

  // The format for Gl op
  GLenum m_GlFormat;

  // The type for Gl op
  GLenum m_GlType;
};

//#ifndef ITK_MANUAL_INSTANTIATION
#include "NeuroLibOpenGLSliceTexture.txx"
//#endif

#endif // __OpenGLSliceTexture_h_
