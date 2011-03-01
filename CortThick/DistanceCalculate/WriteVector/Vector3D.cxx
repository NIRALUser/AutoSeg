#include "Vector3D.h"

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstring>

Vector3D::Vector3D()
{
   m_normemin = 9999999;
   m_normemax = 0;
   m_normecomputed = false;
}

Vector3D::~Vector3D()
{
}

void Vector3D::Load(const char* filename)
{
     Clear();
    m_normemin = 9999999;
    m_normemax = 0;
    m_normecomputed = false;
    FILE* m_file;
    m_file = fopen(filename,"rb");
    char car;
    std::string m_text;

    while (!feof(m_file))
    {
      int byteread = fread(&car,1,1,m_file);    
     /* if (byteread == 0)
      {
        if (m_text.length() > 10)
        {
          float x1,y1,z1,x2,y2,z2;
          sscanf(m_text,"%f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2);
          Add(x1,y1,z1,x2,y2,z2);
          fclose(m_file);
        }
        return;
      }
*/
      if (car == '\n')
      {
         float x1,y1,z1,x2,y2,z2;
         sscanf(m_text.c_str(),"%f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2);
         Add(x1,y1,z1,x2,y2,z2);
         m_text = "";
      }
      else
      {
	//Not totally sure of this one...
       	  m_text.append(&car);
      }
    }

    if (m_text.length() > 10)
    {
      float x1,y1,z1,x2,y2,z2;
      sscanf(m_text.c_str(),"%f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2);
      Add(x1,y1,z1,x2,y2,z2);
    }


    fclose(m_file);
}

void Vector3D::Save(const char* filename)
{
    FILE* m_file;
    m_file = fopen(filename,"wb");
    char m_text[400];
    for (unsigned int i=0;i<m_vectorlist.size();i++)
    {
      sprintf(m_text,"%f %f %f %f %f %f\n\0",m_vectorlist[i].x1,m_vectorlist[i].y1,m_vectorlist[i].z1,m_vectorlist[i].x2,m_vectorlist[i].y2,m_vectorlist[i].z2);     
      fwrite(m_text,strlen(m_text),1,m_file);
    }

    fclose(m_file);
}


void Vector3D::Clear()
{
  m_vectorlist.clear();
}


void Vector3D::Add(float x1,float y1,float z1,float x2,float y2,float z2)
{
  VectorType m_vector;
  m_vector.x1 = x1;
  m_vector.y1 = y1;
  m_vector.z1 = z1;
  m_vector.x2 = x2;
  m_vector.y2 = y2;
  m_vector.z2 = z2;
  m_vectorlist.push_back(m_vector);
}

void Vector3D::Get(unsigned int offset,float &x1,float &y1,float &z1,float &x2,float &y2,float &z2)
{
  x1 = Get(offset,0);
  y1 = Get(offset,1);
  z1 = Get(offset,2);
  x2 = Get(offset,3);
  y2 = Get(offset,4);
  z2 = Get(offset,5);

}

float Vector3D::Get(unsigned int offset,unsigned int coord)
{
  switch(coord)
  {
     case 0: return m_vectorlist[offset].x1; break;
     case 1: return m_vectorlist[offset].y1; break;
     case 2: return m_vectorlist[offset].z1; break;
     case 3: return m_vectorlist[offset].x2; break;
     case 4: return m_vectorlist[offset].y2; break;
     case 5: return m_vectorlist[offset].z2; break;
     default: return 0;
  }
}

unsigned int Vector3D::Size()
{
  return m_vectorlist.size();
}

void Vector3D::ComputeNorme()
{
  m_normemin = 9999999;
  m_normemax  = 0;
  for (unsigned int i=0;i<m_vectorlist.size();i++)
  {
     float dirx = m_vectorlist[i].x1-m_vectorlist[i].x2;
     float diry = m_vectorlist[i].y1-m_vectorlist[i].y2;
     float dirz = m_vectorlist[i].z1-m_vectorlist[i].z2;
     float m_norme = sqrt(dirx*dirx+diry*diry+dirz*dirz);

     if (m_norme > m_normemax)
        m_normemax = m_norme;

     if (m_norme < m_normemin)
        m_normemin = m_norme;
  }

  m_normecomputed = true;
}

float Vector3D::GetNormeMin()
{
  if (m_normecomputed == false)
     ComputeNorme();

  return m_normemin;
}

float Vector3D::GetNormeMax()
{
  if (m_normecomputed == false)
     ComputeNorme();

  return m_normemax;
}
