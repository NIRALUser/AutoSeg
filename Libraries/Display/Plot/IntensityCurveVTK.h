/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: IntensityCurveVTK.h,v $
  Language:  C++
  Date:      $Date: 2004/07/28 13:05:57 $
  Version:   $Revision: 1.2 $
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __IntensityCurveVTK_h_
#define __IntensityCurveVTK_h_

#include "IntensityCurveInterface.h"

#include <vector>

#include <vtkKochanekSpline.h>
#include <vtkCardinalSpline.h>
/**
 * \class IntensityCurveVTK
 * \brief The spline intensity mapping based on the VTK spline class.
 */
class ITK_EXPORT IntensityCurveVTK : public IntensityCurveInterface 
{
public:

  /** Standard class typedefs. */
  typedef IntensityCurveVTK Self;
  typedef IntensityCurveInterface Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(IntensityCurveVTK,IntensityCurveInterface);

  /** New method */
  itkNewMacro(Self);

  // Defined in the parent class
  void Initialize(unsigned int nControlPoints);
  void GetControlPoint(unsigned int iControlPoint, float &t, float &x) const;
  void UpdateControlPoint(unsigned int iControlPoint, float t, float x);
  bool IsMonotonic() const;
  void ScaleControlPointsToWindow(float tMin, float tMax);
  void ScaleControlPointsYToWindow(float tMin, float tMax);

  unsigned int GetControlPointCount() const {
    return m_ControlPoints.size();
  }

  // Evaluate the curve
  float Evaluate(const float &t) const {
    return m_Spline->Evaluate(t);
  }

protected:
  IntensityCurveVTK();
  virtual ~IntensityCurveVTK();
  void PrintSelf(std::ostream &s, itk::Indent indent) const;

private:
  IntensityCurveVTK(const Self& ); //purposely not implemented
  void operator=(const Self& ); //purposely not implemented

  // The spline object
   vtkKochanekSpline *m_Spline;
	//vtkCardinalSpline * m_Spline;

  // Control point structure
  struct ControlPoint {
    float t;
    float x;
  };

  // A storage for the control points
  std::vector<struct ControlPoint> m_ControlPoints;
  typedef std::vector<struct ControlPoint>::iterator IteratorType;
};

#endif // __IntensityCurveVTK_h_
