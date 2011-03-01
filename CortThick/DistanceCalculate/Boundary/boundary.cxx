#include "boundary.h"

void boundary::BoundaryWhite(ImagePointer inputImageW )
{
  //Iterator WhiteMatter
  IteratorType inputItImageW( inputImageW, inputImageW->GetRequestedRegion() );

  //allocation Boundary WhiteMatter Image
  m_BoundaryW_Image = ImageType::New();
  m_BoundaryW_Image ->SetRegions( inputImageW->GetRequestedRegion() );
  m_BoundaryW_Image ->CopyInformation( inputImageW );
  m_BoundaryW_Image ->Allocate();

         //Iterator WhiteMatter Bondary Image
  IteratorType outputItImageWMBoundary( m_BoundaryW_Image, m_BoundaryW_Image->GetRequestedRegion() );
  
  ImageType::IndexType indexWM, indexxpos, indexxneg, indexypos, indexyneg, indexzpos, indexzneg;
  outputItImageWMBoundary.GoToBegin();               
  for( inputItImageW.GoToBegin(); !inputItImageW.IsAtEnd(); ++inputItImageW)
  {
     indexWM = inputItImageW.GetIndex();
     if(inputItImageW.Get()>0)
     {
        //Initialization of index
        indexxpos = indexWM; indexxneg = indexWM; indexypos = indexWM; indexyneg = indexWM; indexzpos = indexWM; indexzneg = indexWM;
        //Definition of Neigborhood
        indexxpos[0]=indexxpos[0]+1;
        indexxneg[0]=indexxneg[0]-1;
        indexypos[1]=indexypos[1]+1;
        indexyneg[1]=indexyneg[1]-1;
        indexzpos[2]=indexzpos[2]+1;
        indexzneg[2]=indexzneg[2]-1;

        //If its a boundary point 
        if(inputImageW->GetPixel(indexxpos)==0 ||  inputImageW->GetPixel(indexxneg)==0 || inputImageW->GetPixel(indexypos)==0 || 
                  inputImageW->GetPixel(indexyneg)==0 || inputImageW->GetPixel(indexzpos)==0 || inputImageW->GetPixel(indexzneg)==0)                
          m_BoundaryW_Image->SetPixel(indexWM, 1);//Pixel intensity = 1 if its a boundary point
      } 
      else //else, if it is not a boundary point, intensity pixel = 0
          m_BoundaryW_Image->SetPixel(indexWM, 0);        
      ++outputItImageWMBoundary;
  }        
}
      
void boundary::BoundaryGray(ImagePointer GrayImage, ImagePointer inputImageW)
{
  //In order to detect the boundary of gray matter we need to add white matter and gray matter so we create a temporary image which there is gray+white
  ImageType::Pointer GraywhiteImage = ImageType::New();
  GraywhiteImage->SetRegions( GrayImage->GetRequestedRegion() );
  GraywhiteImage->CopyInformation( GrayImage );
  GraywhiteImage->Allocate();
  
  //allocation Boundary grayMatter Image
  m_BoundaryG_Image = ImageType::New();
  m_BoundaryG_Image->SetRegions( GrayImage->GetRequestedRegion() );
  m_BoundaryG_Image ->CopyInformation( GrayImage );
  m_BoundaryG_Image ->Allocate();
  
  //Iterator WhiteMatter
  IteratorType inputItImageW( inputImageW, inputImageW->GetRequestedRegion() );        
  //Iterator grayMatter
  IteratorType inputItImage( GrayImage, GrayImage->GetRequestedRegion() );
  //Iterator of temporary image gray+white
  IteratorType GrayWhiteItImage (GraywhiteImage, GraywhiteImage->GetRequestedRegion() );
  //Iterator grayMatter Bondary Image
  IteratorType outputItImageGMBoundary( m_BoundaryG_Image, m_BoundaryG_Image->GetRequestedRegion() );

  //Algorithm to add gray and white matter
  for( inputItImageW.GoToBegin(), inputItImage.GoToBegin(), GrayWhiteItImage.GoToBegin() ; 
        !inputItImageW.IsAtEnd(),!inputItImage.IsAtEnd(), !GrayWhiteItImage.IsAtEnd() ;
           ++inputItImageW, ++inputItImage, ++GrayWhiteItImage )
  {
    if(inputItImage.Get()>0 || inputItImageW.Get()>0)//If it s a gray matter ou a white matter point
      GrayWhiteItImage.Set(1); // White+gray Image
  }
  
  //Algorithm edge detection
  ImageType::IndexType indexWM, indexxpos, indexxneg, indexypos, indexyneg, indexzpos, indexzneg;         
  outputItImageGMBoundary.GoToBegin();       
  for(GrayWhiteItImage.GoToBegin(); !GrayWhiteItImage.IsAtEnd(); ++GrayWhiteItImage)
  {
    indexWM =GrayWhiteItImage.GetIndex();
    ImageType::RegionType::SizeType sizeInput = GrayImage->GetLargestPossibleRegion().GetSize();
    if(GrayWhiteItImage.Get()>0)
    {
      indexxpos = indexWM; indexxneg = indexWM; indexypos = indexWM; indexyneg = indexWM; indexzpos = indexWM; indexzneg = indexWM;
      indexxpos[0]=indexxpos[0]+1;
      indexxneg[0]=indexxneg[0]-1;
      indexypos[1]=indexypos[1]+1;
      indexyneg[1]=indexyneg[1]-1;
      indexzpos[2]=indexzpos[2]+1;
      indexzneg[2]=indexzneg[2]-1; 
      if(GraywhiteImage->GetPixel(indexxpos)==0 ||  GraywhiteImage->GetPixel(indexxneg)==0 || GraywhiteImage->GetPixel(indexypos)==0 
      || GraywhiteImage->GetPixel(indexyneg)==0 || GraywhiteImage->GetPixel(indexzpos)==0 || GraywhiteImage->GetPixel(indexzneg)==0
	 || indexxpos[0]>=(ImageType::IndexType::IndexValueType)sizeInput[0] || indexxneg[0]<0 ||indexypos[1]>= (ImageType::IndexType::IndexValueType)sizeInput[1] || indexyneg[1]<0 || indexzpos[2]>=(ImageType::IndexType::IndexValueType)sizeInput[2] || indexyneg[2]<0)                
      // if it s a boundary point        
      {
        if (GrayImage->GetPixel(indexWM)>0)//if it s a gray matter point to keep just gray matter boundary point
         m_BoundaryG_Image->SetPixel(indexWM, 1);
      }
      
      else
        m_BoundaryG_Image->SetPixel(indexWM, 0); 
    } 
   else
     m_BoundaryG_Image->SetPixel(indexWM, 0);
   ++outputItImageGMBoundary;
   }                        
}

void boundary::Write_Boundary_Image(char * ImageW, char * ImageG)
{
  WriterType::Pointer writer = WriterType::New();
  
  //write white boundary Image
  writer->SetFileName( ImageW );
  writer->SetInput(m_BoundaryW_Image);
  try{
          writer->Update();
     }
 catch(itk::ExceptionObject exp )
  {
          std::cerr << "Exception caught !" << std::endl;
          std::cerr <<     exp    << std::endl;
  }
  
  //Write Gray boundary Image
  writer->SetFileName( ImageG );
  writer->SetInput(m_BoundaryG_Image);
  try{
          writer->Update();
     }
 catch(itk::ExceptionObject exp )
  {
          std::cerr << "Exception caught !" << std::endl;
          std::cerr <<     exp    << std::endl;
  }
}
