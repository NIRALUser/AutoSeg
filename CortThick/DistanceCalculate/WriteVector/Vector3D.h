#ifndef VECTOR3D_H
#define VECTOR3D_H


#include <vector>

/*! \brief Class for writing vtk file.
*
*  The Danielsson Distance transform create a vector map. to visualize it, this class contains functions to store and to
*  write a .viz file to use VectorViz
* 
*/
class Vector3D
{
public:
  Vector3D();
  ~Vector3D();
  
  struct VectorType
  {
    float x1;
    float y1;
    float z1;
    float x2;
    float y2;
    float z2;
  };

  void Load(const char* filename);
  void Save(const char* filename);
  void Add(float x1,float y1,float z1,float x2,float y2,float z2);
  void Get(unsigned int offset,float &x1,float &y1,float &z1,float &x2,float &y2,float &z2);
  float Get(unsigned int offset,unsigned int coord);
  unsigned int Size();
  void Clear();
  float GetNormeMin();
  float GetNormeMax();
  void ComputeNorme();



private:
   std::vector<VectorType> m_vectorlist;
   float m_normemin;
   float m_normemax;
   bool m_normecomputed;

};

#endif
