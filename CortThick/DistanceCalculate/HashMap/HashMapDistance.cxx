#include "HashMapDistance.h"


void HashMapDistance::Hash_mapWG(ImageType::Pointer WhiteMatterImage,ImageType::Pointer WhiteMatterBoundaryImage, ImageType::Pointer GrayMatterImage, ImageType::Pointer DistanceMapImage )
{
  //Declaration Iterator
  //Gray Matter Boundary Iterator
  IteratorType GrayMatterIt(GrayMatterImage ,GrayMatterImage->GetRequestedRegion() );
  IteratorType WhiteMatterIt(WhiteMatterImage ,WhiteMatterImage->GetRequestedRegion()); 
  const ImageType::SpacingType& sp = WhiteMatterImage->GetSpacing();
  //  ImageType::Pointer TemporaryRegion = ImageType::New();
  
   Neighborhood* p_Neighborhood = new  Neighborhood; 

  //browse of WhiteMatter and Gray Matter
  for(GrayMatterIt.GoToBegin(),WhiteMatterIt.GoToBegin() ; !GrayMatterIt.IsAtEnd(), !WhiteMatterIt.IsAtEnd(); ++GrayMatterIt,++WhiteMatterIt )
  {
    //If it is a gray Matter point 
    if(GrayMatterIt.Get()>0 && WhiteMatterIt.Get()==0 )
    {
      //declaration of temporary variable
      ImageType::IndexType indexGM, indextemp;
      indexGM = GrayMatterIt.GetIndex();//coordinate of the current gray matter point                
      float distance =DistanceMapImage->GetPixel(indexGM)  ;//current distance
      int level = (int)distance;
      int flag = 0;//flag varaible to know if a point is found
      float distancetemp = 0; 
      VectorstructType G_point, W_point;//vector to store Gray MAtter Point
      structMatter IndexDistanceG,IndexDistanceW ; 

      //Vector Map Algorithm
      while(flag==0)//While no WhiteMatter point is found
      {
         //Neighborhood algo
        p_Neighborhood->NeighborhoodImage(GrayMatterImage, indexGM, level);
        IteratorType outputIt(p_Neighborhood->m_TemporaryRegion , p_Neighborhood->m_TemporaryRegion->GetRequestedRegion() );
        //browse of neighborhoods to find the white matter point corresponding to the distance in distance map
        for(outputIt.GoToBegin(); !outputIt.IsAtEnd();++outputIt)                        
        {
          indextemp = outputIt.GetIndex();//index of whitematter point
          if(WhiteMatterBoundaryImage->GetPixel(indextemp)>0)//if it is a white matter boundary image
          {
            //calcul distance between the graymatter point to the white matter point                                      
            distancetemp = sqrt(((indextemp[0]-indexGM[0])*sp[0])*((indextemp[0]-indexGM[0])*sp[0])+((indextemp[1]-indexGM[1])*sp[1])*(indextemp[1]-indexGM[1])*sp[1]+(indextemp[2]-indexGM[2])*(indextemp[2]-indexGM[2])*sp[2]*sp[2]);
            //if the distance in the distnacemap image is the same than the distance just found
            if(distancetemp==distance)
            { 
               W_point=m_GMHash_map[(indexGM)];
               G_point=m_WMHash_map[(indextemp)];
               
               //Fill the vector of white matter point
               IndexDistanceW.Index=indextemp;
               IndexDistanceW.distance=distancetemp;
                W_point.push_back(IndexDistanceW);
               
               // Fill the vector of gray matter point
               IndexDistanceG.Index=indexGM;
               IndexDistanceG.distance=distancetemp;                                               
               G_point.push_back(IndexDistanceG);
               
               //Fill the hash map
               m_GMHash_map[(indexGM)]=W_point;
               m_WMHash_map[(indextemp)]=G_point;
               flag=1;
            }    
          }               
        }                                     
        level = level+ 1;
      }                                                              
    }     
  }
  std::cout<<" Fin HashMap "<<std::endl;
  cp_m_GMHash_map = m_GMHash_map;
  cp_m_WMHash_map = m_WMHash_map;

 delete p_Neighborhood;                
}

void HashMapDistance::RefreshHashMap(HashMapstructType * HashMapG_temp , HashMapstructType * HashMapW_temp, ImageType::IndexType IndexGM )
{
  VectorstructType G_Vector, G_Vectortemp, W_Vector;//vector to store Gray Matter Point
  structMatter IndexDistanceG;

  W_Vector =  m_GMHash_map[(IndexGM)];
  (*HashMapG_temp)[(IndexGM)]=W_Vector;//new_hashMap(point) = old_hashMap(point)
  for(unsigned int i = 0; i< W_Vector.size(); i++)
  {              
    G_Vector = (*HashMapW_temp)[(W_Vector[i].Index)];//old_Vector = new_HashMap(point)
    IndexDistanceG.Index = IndexGM;
    IndexDistanceG.distance = 0;
    G_Vector.push_back(IndexDistanceG);//new_vector = old_vector + new_point
    (*HashMapW_temp)[(W_Vector[i].Index)]=G_Vector;//new_HashMap(point) = new_vector
  }      
}



