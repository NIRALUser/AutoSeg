/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: IntensityCurveInterface.h,v $
  Language:  C++
  Date:      $Date: 2004/07/28 13:05:57 $
  Version:   $Revision: 1.2 $
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __IntensityCurveInterface_h_
#define __IntensityCurveInterface_h_

#include <itkFunctionBase.h>

/**
 * \class IntensityCurveInterface
 * \brief The base class for intensity mapping splines
 */
class ITK_EXPORT IntensityCurveInterface : public itk::FunctionBase<float,float> {
public:

  /** Standard class typedefs. */
  typedef IntensityCurveInterface Self;
  typedef itk::FunctionBase<float,float> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(IntensityCurveInterface,itk::FunctionBase);

  /**
   * Initialize the spline with initial number of control points.
   * The spline will be a linear mapping from (0,0) to (1,1)
   */
  virtual void Initialize(unsigned int nControlPoints) = 0;

  /**
   * Get the value of a control point
   */
  virtual void GetControlPoint(unsigned int iControlPoint, 
    float &t, float &x) const = 0;

  /**
   * Get the value of a control point
   */
  virtual unsigned int GetControlPointCount() const = 0;

  /**
   * Update the value of a control point
   */
  virtual void UpdateControlPoint(unsigned int iControlPoint, 
    float t, float x) = 0;

  /**
   * This method linearly maps the t-values of all the control points to 
   * the range between tMin and tMax.  It's used for intensity windowing 
   * where we want to adjust the domain of the curve without changing its 
   * shape
   */
  virtual void ScaleControlPointsToWindow(float tMin, float tMax) = 0;
  virtual void ScaleControlPointsYToWindow(float tMin, float tMax) = 0;
  /**
   * Check the monotonicity of the spline curve
   */
  virtual bool IsMonotonic() const = 0;

protected:
  IntensityCurveInterface(){};
  virtual ~IntensityCurveInterface(){};

private:
  IntensityCurveInterface(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented
};

#endif // __IntensityCurveInterface_h_
