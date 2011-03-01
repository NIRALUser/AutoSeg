#include "Neighborhood.h"
#include "itkImage.h"

void Neighborhood::NeighborhoodImage(ImageType::Pointer InputImage, ImageType::IndexType Index, int level)
{	
      m_TemporaryRegion = ImageType::New();
      //declaration of temporary variable
      const ImageType::SpacingType& sp = InputImage->GetSpacing();
      ImageType::RegionType::SizeType size, sizeInput;  
      const ImageType::PointType& inputOrigin = InputImage->GetOrigin();
      ImageType::IndexType startInput, startoutput;  
      sizeInput = InputImage->GetLargestPossibleRegion().GetSize();
/*******************************************************************************************************/
        int levelx_pos, levelx_neg, levely_pos, levely_neg, levelz_pos, levelz_neg;
        levelx_pos = level; levelx_neg = level; levely_pos = level; levely_neg = level;   
         levelz_neg = level; levelz_pos = level;
         
         while( Index[0] - levelx_neg < 0) levelx_neg --;
         while( Index[0] + levelx_pos > (int)sizeInput[0]) {
         levelx_pos --;
         }
         startInput[0] = Index[0]- levelx_neg;
         startoutput[0] = Index[0] - levelx_neg;
         size[0] = levelx_neg + levelx_pos;
         
         while( Index[1] - levely_neg < 0) levely_neg --;
         while( Index[1] + levely_pos > (int)sizeInput[1]) levely_pos --;
         startInput[1] = Index[1]- levely_neg;
         startoutput[1] = Index[1] - levely_neg;
         size[1] = levely_neg + levely_pos;
         
         while( Index[2] - levelz_neg < 0) levelz_neg --;
         while( Index[2] + levelz_pos > (int)sizeInput[2]) levelz_pos --;
         startInput[2] = Index[2]- levelz_neg;
         startoutput[2] = Index[2] - levelz_neg;
         size[2] = levelz_neg + levelz_pos;
         
	
	//initialization of the temporay region
      ImageType::RegionType InputRegion, OutputRegion;
      InputRegion.SetSize( size );
      InputRegion.SetIndex( startInput );
      OutputRegion.SetSize( size );
      OutputRegion.SetIndex( startoutput);
      (m_TemporaryRegion)->SetRegions( OutputRegion );
      double outputOrigin[ 3 ];

      for(unsigned int i=0; i< 3; i++)
      {
                outputOrigin[i] = inputOrigin[i] + sp[i] * startInput[i];
      }

      (m_TemporaryRegion)->SetSpacing( sp );
      (m_TemporaryRegion)->SetOrigin( outputOrigin );
      (m_TemporaryRegion)->Allocate();

                              
      IteratorType inputIt( InputImage, InputRegion );
      IteratorType outputIt((m_TemporaryRegion) , OutputRegion );
	
      for ( inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();++inputIt, ++outputIt)
      {
                outputIt.Set( inputIt.Get() );
      }   
}


