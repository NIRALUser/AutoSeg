#include "ImageSyl.h"


ImageSyl::ImageSyl()
{}

ImageSyl::~ImageSyl()
{}

//Set the file name of the displayed image
void ImageSyl::SetFileName(const char* N)
{
  m_ImageFileName = N;
  std::cout << "File name: " << m_ImageFileName << std::endl;
  LoadImage();
}

//Set the overlay image file name
void ImageSyl::SetOverlayFileName(const char* OverlayN)
{
  m_OverlayImageName = OverlayN;
  LoadOverlay();  
}

//return the displayed image name
const char *ImageSyl::GetFileName()
{
  return m_ImageFileName;
}

//return the Overlay image name
const char * ImageSyl::GetOverlayFileName()
{
  return m_OverlayImageName;
}

//return the displayed image pointer
ImagePointer ImageSyl::GetImageSyl()
{
  return m_ImageDisp;
}

//Set the displayed image pointer
void ImageSyl::SetImageSyl(ImagePointer Im)
{
  m_ImageDisp = Im;
  m_ImageDisp->Update();
}

//return the overlay pointer
OverlayPointer ImageSyl::GetOverlaySyl()
{
  return m_OverlayDisp;
}

//Read the image file
void ImageSyl::LoadImage()
{
  std::cout << "Load file" << m_ImageFileName << std::endl;
  m_reader = ReaderType::New();
  m_reader->SetFileName(m_ImageFileName);
  try{
    m_reader->Update();
  }
  catch(...)
  {
    std::cerr << "Can't read the file" << std::endl;
  }
  
  m_ImageDisp = m_reader->GetOutput();
  //fill the image propertie fields
  GetImageProp(m_ImageDisp);
}

//Read the overlay file
void ImageSyl::LoadOverlay()
{
  std::cout << "Load Overlay file" << std::endl;
  m_overlayreader = OverlayReaderType::New();
  m_overlayreader->SetFileName(m_OverlayImageName);
  try{
    m_overlayreader->Update();
  }
  catch(...)
  {
    std::cerr << "Can't read the file" << std::endl;
  }
  
  m_OverlayDisp = m_overlayreader->GetOutput();
}

//Save the current displayed image
void ImageSyl::SaveImage(const char *Fname)
{
   WriterPointer writer = WriterType::New();
   writer->SetFileName(Fname);
   writer->UseCompressionOn();
   writer->SetInput(m_ImageDisp);
   try{
     writer->Update();
   }
   catch( itk::ExceptionObject exp )
   {
     std::cerr << "Exception caught !" << std::endl;
     std::cerr <<     exp    << std::endl;
   }
}

ImageType::SpacingType ImageSyl::GetImageSpacing()
{
  return m_sp;
}

ImageType::SizeType ImageSyl::GetImageSize()
{
  return m_SizeImage;
}

//Fill the image property vectors
void ImageSyl::GetImageProp(ImagePointer Im)
{
  m_sp = Im->GetSpacing();
  m_SizeImage = Im->GetLargestPossibleRegion().GetSize();  
}

//compute the connective component algorithm onto the image Im
ImagePointer ImageSyl::ConnectiveComponent(ImagePointer Im)
{

  ScalarConnectiveFilterType::Pointer Connective = ScalarConnectiveFilterType::New();
  relabelFilterType::Pointer relabelFilter = relabelFilterType::New();
  binThreshFilterType::Pointer ThresFilter = binThreshFilterType::New();

  //Get the connectivity map of the image
  Connective->SetInput(Im);
  Connective->SetDistanceThreshold(0);
  //Sort the labels according to their size, each labeled object has a different value
  relabelFilter->SetInput(Connective->GetOutput());
  //Keep the 1 label which is the biggest component 
  ThresFilter->SetInput(relabelFilter->GetOutput());
  ThresFilter->SetLowerThreshold(0.1);
  ThresFilter->SetUpperThreshold(1.1);
  ThresFilter->SetInsideValue(1.0);
  ThresFilter->SetOutsideValue(0.0);
  ThresFilter->Update();
    
  Im = ThresFilter->GetOutput();
  Im->Update();   
  return Im;
}

void ImageSyl::SegImageWhiteAndGrey(const char *SegFileName, double labelWhite, double labelGrey)
{
  m_reader = ReaderType::New();
  m_reader->SetFileName(SegFileName);
  try{
    m_reader->Update();
  }
  catch(...)
  {
    std::cerr << "Can't read the file" << std::endl;
  }
  m_ImageSeg = m_reader->GetOutput();
  m_WhiteMatterImage = CreateNewImageLike(m_ImageSeg);
  m_GreyMatterImage =  CreateNewImageLike(m_ImageSeg);
  IteratorType ImageSegIt( m_ImageSeg, m_ImageSeg->GetRequestedRegion() );
  for ( ImageSegIt.GoToBegin(); !ImageSegIt.IsAtEnd(); ++ImageSegIt)
  {
    if(ImageSegIt.Get() == labelWhite)                                               
      m_WhiteMatterImage->SetPixel(ImageSegIt.GetIndex(),1);
    else
      m_WhiteMatterImage->SetPixel(ImageSegIt.GetIndex(),0);
    if( ImageSegIt.Get() == labelGrey)
      m_GreyMatterImage->SetPixel( ImageSegIt.GetIndex(),1);
    else
      m_GreyMatterImage->SetPixel( ImageSegIt.GetIndex(),0);  
  }
 
}
void ImageSyl::LoadGreyMatterFile(const char *GMFileName)
{
  m_reader = ReaderType::New();
  m_reader->SetFileName(GMFileName);
  try{
    m_reader->Update();
  }
  catch(...)
  {
    std::cerr << "Can't read the file" << std::endl;
  }
  
  m_GreyMatterImage = m_reader->GetOutput();
}

void ImageSyl::LoadWhiteMatterFile(const char *WMFileName)
{
  m_reader = ReaderType::New();
  m_reader->SetFileName(WMFileName);
  try{
    m_reader->Update();
  }
  catch(...)
  {
    std::cerr << "Can't read the file" << std::endl;
  }
  
  m_WhiteMatterImage = m_reader->GetOutput();
}

void ImageSyl::LoadDanielssonMapFile(const char *DanielssonFileName)
{
  m_reader = ReaderType::New();
  m_reader->SetFileName(DanielssonFileName);
  try{
    m_reader->Update();
  }
  catch(...)
  {
    std::cerr << "Can't read the file" << std::endl;
  }
  
  m_DistanceMapImage = m_reader->GetOutput();
}

ImagePointer ImageSyl::GetWhiteMatterImage()
{
  return m_WhiteMatterImage;
}

ImagePointer ImageSyl::GetGreyMatterImage()
{
  return m_GreyMatterImage;
}

void ImageSyl::SetWhiteMatterImage(ImagePointer Im)
{
  m_WhiteMatterImage = Im;
  m_WhiteMatterImage->Update();
}

void ImageSyl::SetGreyMatterImage(ImagePointer Im)
{
  m_GreyMatterImage = Im;
  m_GreyMatterImage->Update();
}

//compute the distance map only onto the white matter image
void ImageSyl::ComputeDM(const char* DMName,/*const char* VectorMapFileName,*/ bool Save)
{
  //SignedDanielsson DistanceMap
  m_DistanceMapImage = ImageType::New();        
  DanielsonFilterType::Pointer filter = DanielsonFilterType::New();
  filter->SetUseImageSpacing(true);
  filter->SetInput( m_WhiteMatterImage );  
  filter->Update();
  m_DistanceMapImage = filter->GetOutput();
  //The map is saved as an image
  if(Save){
    std::cout<<" Save Danielsson "<<std::endl;
    WriterType::Pointer DistanceMapWriter = WriterType::New();
    DistanceMapWriter->SetFileName(DMName);
    DistanceMapWriter->UseCompressionOn();
    DistanceMapWriter->SetInput(m_DistanceMapImage); 
    try
    {
      DistanceMapWriter->Update();
     }
    catch( itk::ExceptionObject exp )
    {
      std::cerr << "Exception caught !" << std::endl;
      std::cerr <<     exp    << std::endl;
    }
  }
  else filter->Update();
}

ImagePointer ImageSyl::GetDistanceMap()
{
  return m_DistanceMapImage;
}

void ImageSyl::WriteDanielsson(char* FileName)
{
   WriterType::Pointer DistanceMapWriter = WriterType::New();
    DistanceMapWriter->SetFileName(FileName);
    DistanceMapWriter->SetInput(GetDistanceMap()); 
    DistanceMapWriter->UseCompressionOn();
    try
    {
      DistanceMapWriter->Update();
    }
    catch( itk::ExceptionObject exp )
    {
      std::cerr << "Exception caught !" << std::endl;
      std::cerr <<     exp    << std::endl;
    }   
}

void ImageSyl::BoundaryComputation()
{
       
  boundary * p_boundary = new boundary();
  p_boundary->BoundaryWhite(m_WhiteMatterImage);
  p_boundary->BoundaryGray(m_GreyMatterImage,m_WhiteMatterImage);
  m_WhiteMatterBoundaryImage = p_boundary->Get_Boundary_White();
  m_GreyMatterBoundaryImage = p_boundary->Get_Boundary_Gray();
    
  delete p_boundary;
  
}

//Create an image with the same properties as the parameter image
ImagePointer ImageSyl::CreateNewImageLike(ImagePointer m_OriginalImage)
{
  ImagePointer outputImage = ImageType::New();
  outputImage->SetRegions( m_OriginalImage->GetRequestedRegion() );
  outputImage->CopyInformation( m_OriginalImage );
  outputImage->Allocate();
  return outputImage;
}

//Compute the final distance map 
void ImageSyl::ComputeDistance(char* WhiteMapFileName,  char* vtkFileName, char* AvgFileName, 
                               char* DanGreyFileName,char* ParFileName, char* DistanceImageFileName ,bool ComputeAveraging, bool WriteVtkFile, 
                               bool ComputeDanGrey, bool ParLoad, bool Separate, bool WriteDistance,
			       char* DistanceMapOnGrey, char* DistanceMapAverageOnGrey, bool WriteDistanceOnGrey,bool WriteAvgDistanceOnGrey, bool Interp , float Threshold)
{
  toolsdistance *p_distance = new toolsdistance();

  //First, allocate the output image memory space
  m_WhiteMatterDistanceMap = CreateNewImageLike(m_WhiteMatterImage);
  m_GreyMatterDistanceMap = CreateNewImageLike(m_WhiteMatterImage);
  
  p_distance->calculdistance(	m_GreyMatterBoundaryImage,
				m_WhiteMatterDistanceMap,
				m_GreyMatterDistanceMap,
				m_WhiteMatterBoundaryImage,
				m_WhiteMatterImage,
				m_GreyMatterImage,
				m_DistanceMapImage);
  //Write file DistanceMap
  p_distance->WriteWhiteFile(m_WhiteMatterDistanceMap,m_WhiteMatterBoundaryImage, ParFileName,WhiteMapFileName, ParLoad, Interp , Threshold);
  p_distance->CounterWhiteGrey(m_WhiteMatterDistanceMap, m_GreyMatterDistanceMap);
  
  //To separate cortex inside folds versus cortex at the boundary
  if(Separate)
    {
      p_distance->SepareInsideVSBoundary(m_GreyMatterBoundaryImage,m_GreyMatterDistanceMap, AvgFileName);
      p_distance->WriteWhiteFile(p_distance->m_InsideFoldsThicknessIm,m_WhiteMatterBoundaryImage ,ParFileName,p_distance->m_insideFileName, ParLoad, Interp , Threshold);
      p_distance->WriteWhiteFile(p_distance->m_BoundaryThicknessIm,m_WhiteMatterBoundaryImage ,ParFileName,p_distance->m_BoundaryFileName, ParLoad, Interp , Threshold);
    }
  
  if(WriteDistance)
    {
      std::cout<<" Write distance map white without averaging "<<std::endl;
      WriterPointer writer = WriterType::New();
      writer->UseCompressionOn();
      writer->SetFileName(DistanceImageFileName);
      writer->SetInput(m_WhiteMatterDistanceMap);
      writer->Update();
    }
  if(WriteDistanceOnGrey)
    {
      std::cout<< "Write distance map on grey boundary" << std::endl;
      WriterPointer writerGM = WriterType::New();
      writerGM->SetFileName(DistanceMapOnGrey);
      writerGM->UseCompressionOn();
      std::cout << "Grey matter values File Name: " <<DistanceMapOnGrey  << std::endl;
      writerGM->SetInput(m_GreyMatterDistanceMap);
      writerGM->Update();      
    }
  if(WriteAvgDistanceOnGrey)
    {
      std::cout<< "Write average distance map on grey boundary" << std::endl;
      WriterPointer writerGM = WriterType::New();
      p_distance->AverageNeighborhood(m_GreyMatterDistanceMap,m_GreyMatterBoundaryImage);
      writerGM->SetFileName(DistanceMapAverageOnGrey);
      std::cout << "Grey matter averaged values File Name: " << DistanceMapAverageOnGrey << std::endl;
      writerGM->UseCompressionOn();
      writerGM->SetInput(m_GreyMatterDistanceMap);
      writerGM->Update();      
    }

  if(ComputeAveraging)
  {
    std::cout << " Averaging process" << std::endl;
    p_distance->AverageNeighborhood(m_WhiteMatterDistanceMap,m_WhiteMatterBoundaryImage);
    WriterPointer writer = WriterType::New();
    std::cout << "Average File Name: " << AvgFileName << std::endl;
    writer->SetFileName(AvgFileName);
    writer->UseCompressionOn();
    writer->SetInput(m_WhiteMatterDistanceMap);
    writer->Update();	
  }

  if(WriteVtkFile)
  {
    std::cout<<" WriteVtkFile "<<std::endl;
    p_distance->GenerateVectorImage(m_GreyMatterImage, vtkFileName);
  }
  if(ComputeDanGrey)
  {
    std::cout<<" Compute Grey Danielsson "<<std::endl;
    p_distance->WriteGreyDanielsson(m_GreyMatterImage, m_DistanceMapImage,DanGreyFileName);
  }     
  
  delete p_distance;
  std::cout<<" End Computation "<<std::endl;
 
}

