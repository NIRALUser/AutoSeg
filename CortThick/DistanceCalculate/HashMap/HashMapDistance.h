#ifndef HashMapDistance_H
#define HashMapDistance_H

#include "itkImageRegionIteratorWithIndex.h"
#include "itkImage.h"
#include "itk_hash_map.h"

#include "Neighborhood.h"

/*! \brief Brief Description: this class compute the HashMap from the distanceMap.
*
* there is to HashMap, the first is the Gray_HashMap, for each GrayMatterPoint the closest WhiteMatterPoint is stored
* the second is the White_HashMap, for each WhiteMatterPoint, Each GrayMatterPoint which are pointed on the WhiteMatterPoint
* is stored.
*/

class HashMapDistance
{
public:
  typedef float PixelType;
  typedef itk::Image< PixelType, 3 > ImageType;        
  typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;
  
  typedef struct 
  {
    ImageType::IndexType Index;
    float distance;
  }structMatter;        
  typedef std::vector<structMatter> VectorstructType;        
  
  /** a hash function struct that returns a hash value for a given index. */
  typedef struct
  {
    unsigned int operator()(const ImageType::IndexType IndexGM) const
    {
            unsigned int sum=0;
            sum = IndexGM[0]*100000000 +  IndexGM[1]*100000 + IndexGM[2]*100;
            return sum;
    }
  }structhashfunction; 

  typedef itk::hash_map< const ImageType::IndexType,VectorstructType,structhashfunction > HashMapstructType;
  
public:
  HashMapDistance(void){}
  ~HashMapDistance(void){}
  
  HashMapstructType m_GMHash_map;
  HashMapstructType m_WMHash_map;
  HashMapstructType cp_m_GMHash_map;
  HashMapstructType cp_m_WMHash_map;
  VectorstructType m_Vector;
        
	/** this Function calculate Gray_HashMap and WhiteHashMap. */
  void Hash_mapWG(ImageType::Pointer WhiteMatterImage,ImageType::Pointer WhiteMatterBoundaryImage, ImageType::Pointer GrayMatterImage, ImageType::Pointer DistanceMapImage );
        
	VectorstructType Get_HashMAp_W(ImageType::IndexType indexWM) { return m_WMHash_map[(indexWM)];}
        
	VectorstructType Get_HashMAp_G(ImageType::IndexType indexGM) { return m_GMHash_map[(indexGM)];}
        
	/** this function permit to refresh the hash_Map for each points you want to keep after the calculation of the angle. */
	void RefreshHashMap(HashMapstructType *HashMapG_temp , HashMapstructType *, ImageType::IndexType IndexGM);
};

#endif                

