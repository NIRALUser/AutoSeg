#include "toolsdistance.h"


#if defined(_WIN32) || defined(WIN32)
#define sep	'\\'
#else
#define sep	'/'
#endif

/*
* Constructor -
*/
toolsdistance::toolsdistance()
{
	m_CompIns = 0;
	m_CompBound = 0;
}

/*
* Destructor -
*/
toolsdistance::~toolsdistance(void){}


void  toolsdistance::deletepoint(ImageType::Pointer GrayMatter, ImageType::Pointer GrayMatterBoundaryImage,ImageType::Pointer DistanceMap )
{
	std::cout<<" delete point"<<std::endl;
  //Iterator GrayMatter
	IteratorType GrayMatterIt( GrayMatter, GrayMatter->GetRequestedRegion() );
  //Declaration of temporary Variable
	ImageType::IndexType IndexGM,V1,IndexWM1, IndexWM2,IndexWM3, indextemp;
	ImageType::Pointer TemporaryRegion = ImageType::New();
	HashMapDistance::HashMapstructType HashMapG_temp, HashMapW_temp;
	HashMapDistance::VectorstructType Vector;     
	const ImageType::SpacingType& sp = GrayMatter->GetSpacing(); 
  
  //Declaration of pointer to use methods
	Neighborhood* p_Neighborhood = new  Neighborhood();
	CalculateAngle* p_CalculateAngle = new CalculateAngle();

  //Calcul of the maximum distance in the image  
	p_CalculateAngle->Calcul_Dmax(GrayMatter, DistanceMap);

  //Curse of grayMatter 
	for(GrayMatterIt.GoToBegin(); !GrayMatterIt.IsAtEnd(); ++GrayMatterIt)
	{
		if(GrayMatterIt.Get()>0)
		{
      //initialization of variable
			int inc=1;
			int flag = 0;
			IndexGM = GrayMatterIt.GetIndex();
      //Get the HashMap of the image which contain for each gray matter the closests white matter points
			Vector = p_HashMap.Get_HashMAp_G(IndexGM);		
			int sizevector = Vector.size();  //sizevector to know if there is several whiteMatter point

			IndexWM1 = Vector[0].Index;//the coordinate of the first whiteMatter point
      //Calcul of vector whiteMatter GrayMatter			
			V1[0] = IndexWM1[0] - IndexGM[0];
			V1[1] = IndexWM1[1] - IndexGM[1];
			V1[2] = IndexWM1[2] - IndexGM[2];

      //if it s a grayMatterboundary point, refresh the hashMap and keep this point
			if(GrayMatterBoundaryImage->GetPixel(IndexGM)>0)
			{ 
				p_HashMap.RefreshHashMap(&HashMapG_temp, &HashMapW_temp, IndexGM);
				flag = 1;	                             
			}
			else //if it is not a grayMatterboundary point
			{
				if(sizevector==2) //if the vectorsize == 2 
				{
					IndexWM2 = Vector[inc].Index;
            //angle between the first white matter point and the second white matter point
					float cosphi = p_CalculateAngle->CalculAngleVector( IndexWM2, IndexGM,  V1 );
            //Calcul anglemin wich depends of the distance
					p_CalculateAngle->calcul_Angle(sp, Vector[inc].distance);
					if(acos(cosphi)>p_CalculateAngle->Get_Angle())//if phi>anglemin
					{
              //refresh hash map to keep this point
						flag=1;
						p_HashMap.RefreshHashMap(&HashMapG_temp, &HashMapW_temp, IndexGM);
					}
				}
				else if(sizevector > 2 /*|| flag==0*/)
				{
					inc=0;
					while(inc<sizevector-1 && flag==0)
					{
						IndexWM2 = Vector[inc].Index;
						int counter = 1;
						float phi=0;
            //algo which permit to find the biggest angle between the vectors
						while(counter<sizevector)
						{
							IndexWM3 = Vector[counter].Index;
							V1[0] = IndexWM3[0] - IndexGM[0];
							V1[1] = IndexWM3[1] - IndexGM[1];
							V1[2] = IndexWM3[2] - IndexGM[2];
							float cosphi;
							cosphi = p_CalculateAngle->CalculAngleVector( IndexWM2, IndexGM,  V1 );
							if(acos(cosphi)>phi)
								phi = acos(cosphi);
							counter++;
						}
						if(phi> p_CalculateAngle->Get_Angle())
						{	//if the angle find is superior to the angle min, keep the point
							flag = 1;
							p_HashMap.RefreshHashMap(&HashMapG_temp, &HashMapW_temp, IndexGM);
						}
						inc++;
					}
				}
				else if((sizevector == 1 || flag==0 ) && sizevector!=0)//if it is not a boundary point, if we didn't already find a good angle, we check the Neigborhood
				{        //algo of neigborhood, just the first neigborhood
					p_Neighborhood->NeighborhoodImage(GrayMatter, IndexGM, 1);
					IteratorType outputIt(p_Neighborhood->m_TemporaryRegion , p_Neighborhood->m_TemporaryRegion->GetRequestedRegion() );

					IndexWM1 = Vector[0].Index;
					V1[0] = IndexWM1[0] - IndexGM[0];
					V1[1] = IndexWM1[1] - IndexGM[1];
					V1[2] = IndexWM1[2] - IndexGM[2];
          //curse of neigborhood
					for(outputIt.GoToBegin(); !outputIt.IsAtEnd();++outputIt)
					{
						inc=0;
						indextemp = outputIt.GetIndex();
						Vector = p_HashMap.Get_HashMAp_G(indextemp);
						while(inc< (int)Vector.size() && flag==0)
						{
							IndexWM2 = Vector[inc].Index;
							float cosphi;
							cosphi = p_CalculateAngle->CalculAngleVector( IndexWM2, indextemp,  V1 );
							p_CalculateAngle->calcul_Angle(sp, Vector[inc].distance);
							if(acos(cosphi)>p_CalculateAngle->Get_Angle())
							{
								flag=1;
								p_HashMap.RefreshHashMap(&HashMapG_temp, &HashMapW_temp, IndexGM);
							}
							inc++;
						}
					}
				}
			}
		}
	}
  //clear of  old hashMap
	p_HashMap.m_WMHash_map.clear();
	p_HashMap.m_GMHash_map.clear();
  //replace the old one by the new one
	p_HashMap.m_GMHash_map = HashMapG_temp;     
	p_HashMap.m_WMHash_map = HashMapW_temp;
  
	delete p_Neighborhood; 
	delete p_CalculateAngle;
}


void toolsdistance::calculdistance(ImageType::Pointer GrayMatterBoundaryImage,
				   ImageType::Pointer outputImageWM,
				   ImageType::Pointer outputImageGM, 
				   ImageType::Pointer WhiteMatterBoundaryImage, 
				   ImageType::Pointer WhiteMatterImage, 
				   ImageType::Pointer GrayMatterImage,ImageType::Pointer DistanceMapImage  )
{
	std::cout<<" Calcul Distance"<<std::endl;
  //Declaration Iterator
	IteratorType WhiteMatterBoundaryIt(WhiteMatterBoundaryImage ,WhiteMatterBoundaryImage->GetRequestedRegion() );
  //Declaration of temporay variable	
	const ImageType::SpacingType& sp = WhiteMatterBoundaryImage->GetSpacing();

  //calculate HashMap white and Gray
	std::cout<<" Hash Map "<<std::endl;
	p_HashMap.Hash_mapWG( WhiteMatterImage, WhiteMatterBoundaryImage,  GrayMatterImage,  DistanceMapImage );
  
  //Delete Gray Matter point to obtain the squeletor of grayMatterImage
	deletepoint( GrayMatterImage, GrayMatterBoundaryImage, DistanceMapImage);
     
  //fill of whiteMatterDistanceMap and GrayMatterDistanceMap
  //curse of white matter boundary
	for(WhiteMatterBoundaryIt.GoToBegin() ; !WhiteMatterBoundaryIt.IsAtEnd(); ++WhiteMatterBoundaryIt )
	{
		if(WhiteMatterBoundaryIt.Get()>0)//if its a whiteMatterBoundary point
		{	//declaration and initialization of temporary variable
			float distancetemp=0;
			float distance=0;
			ImageType::IndexType indexW, indextemp;
			indexW =WhiteMatterBoundaryIt.GetIndex();
			HashMapDistance::VectorstructType VectorG;                
      //VectorG=p_HashMap.Get_HashMAp_W(indexW);
			VectorG=p_HashMap.m_WMHash_map[indexW];
			int size = VectorG.size();
    
			if(size==0) //if it is whiteBoundarypoint whithout a gray pointer point
			{
				outputImageWM->SetPixel(indexW, 0); //fill 0 for this point
			}
			else //if it is a whiteMatter point pointed by a or several grayMatterPoint
			{
				for(int i=0; i<size; i++)
				{	
          //the distance between \f$(Gx,Gy,Gz)\f$ and \f$(Wx,Wy,Wz)\f$ is
          //\f$\sqrt{(Wx-Gx)�*sp[0]�+(Wy-Gy)�*sp[1]�+(Wz-Gz)�*sp[2]�}\f$
					distancetemp=sqrt((VectorG[i].Index[0]-indexW[0])*(VectorG[i].Index[0]-indexW[0])*sp[0]*sp[0]+
							(VectorG[i].Index[1]-indexW[1])*(VectorG[i].Index[1]-indexW[1])*sp[1]*sp[1]+
							(VectorG[i].Index[2]-indexW[2])*(VectorG[i].Index[2]-indexW[2])*sp[2]*sp[2]) ;
                                    
					if(distancetemp>=distance)//if the distance is superior, keep the max distance
					{
						distance=distancetemp;
						indextemp=VectorG[i].Index;
						if(distance==sp[0]||distance==sp[1]||distance==sp[2])//if it s the minimum distance
						{//and if it is not a GrayMatterBoundary Image delete this point
							if(GrayMatterBoundaryImage->GetPixel(indextemp)==0) distance=0;
						}        
					}                                                                               
				}
        //fill the finalImage whith distances found
				outputImageWM->SetPixel(indexW, distance);
				outputImageGM-> SetPixel(indextemp, distance);                     
			}
		}
	}             
}


void toolsdistance::AverageNeighborhood(ImageType::Pointer outputImageWM, ImageType::Pointer outputImageWMBoundary)
{
	IteratorType outputItImageWM( outputImageWM, outputImageWM->GetRequestedRegion() );
	IteratorType outputItImageWMBoundary( outputImageWMBoundary, outputImageWMBoundary->GetRequestedRegion() );
	ImageType::Pointer TemporaryRegion = ImageType::New();
	ImageType::IndexType indexWMB, indextemp;

	const ImageType::SpacingType& sp = outputImageWM->GetSpacing();
	Neighborhood* p_Neighborhood = new  Neighborhood;              
  //course of the WhiteMatter image previously calculates including the distances and from the image of the border of WhiteMatter GrayMatter.
	outputItImageWMBoundary.GoToBegin();
	for( outputItImageWM.GoToBegin(); !outputItImageWM.IsAtEnd(); ++outputItImageWM)
	{           
		indexWMB = outputItImageWMBoundary.GetIndex();
		if(outputItImageWMBoundary.Get() > 0)//f it s a white boundary point
		{			
      //declaration of temporary varaible
			float distance = 0;
			float distancetemp = outputItImageWM.Get();          
			Tableau1D<float> *A_distance = new Tableau1D<float>();//array to keep distances
			int box = 0;
			int level = 2;
			int flag = 0;

      //if there is no distance in the whiteMatterBoundary Point
			if(((distancetemp <(sp[0])) && (distancetemp < (sp[1])) && (distancetemp < (sp[2]))))
			{	//while there is no good distance found
				while(((distancetemp <(sp[0])) && (distancetemp < (sp[1])) && (distancetemp < (sp[2]))))
				{                                      //find Neigborhood                              
					p_Neighborhood->NeighborhoodImage(outputImageWM, indexWMB, level); 
					IteratorType outputIt(p_Neighborhood->m_TemporaryRegion , p_Neighborhood->m_TemporaryRegion->GetRequestedRegion() ); 	//curse Neigborhood
					// outputIt.GoToBegin();
          //while(!outputIt.IsAtEnd())
					for(outputIt.GoToBegin(); !outputIt.IsAtEnd();++outputIt)
					{
						distance = outputIt.Get();
						indextemp = outputIt.GetIndex();
            //if it s whiteMatter Boundary point
						if(outputImageWMBoundary->GetPixel(indextemp)==1)
						{	//if the distance is superior at distancem min
							if((distance >(sp[0])) && (distance > (sp[1])) && (distance > (sp[2])))
							{
								if(flag==0)
								{
									distancetemp = distance;
									flag = 1;
								}//fill the array whith distances found
								A_distance->Set(box, distance);                                                          
								box++;      
							}
						}      
					}
              //std::cout<<"box"<<box<<std::endl;  
					float nb_box_distance = 0;
					float sum = 0;
					float average = 0;
            //Calculate the average of each distances found for each whiteMatterboundary point without 0 for ditance
					for(int i = 0 ; i < box; i++)
					{
						if((A_distance->Get(i)>(sp[0])) || (A_distance->Get(i) > (sp[1])) || (A_distance->Get(i)> (sp[2])))
						{
                            //std::cout<<" A_distance->Get(i) "<<A_distance->Get(i)<<std::endl;
							sum = sum + A_distance->Get(i);
							nb_box_distance++;
						}
					}
					average = sum / nb_box_distance;
					outputImageWM->SetPixel(indexWMB, average);
					level = level + 1;
				} 
			}
			else outputImageWM->SetPixel(indexWMB,distancetemp );
			delete A_distance;                      
		}
		else outputImageWM->SetPixel(indexWMB, 0);
		++outputItImageWMBoundary;                
	}
	delete p_Neighborhood;
}        

// not use 
// useful to write the squeleton of grey matter, which mean use to create an image where there is only the non delete points
void toolsdistance::Write_Gray_After_Delete(ImageType::Pointer GrayImage, char * Image, bool write)
{
  //iterator GrayMatter
	IteratorType GrayItImage( GrayImage, GrayImage->GetRequestedRegion() );        
  
  ///<allocation Image
	m_GrayImageAfterdel = CreateNewImageLike( GrayImage);      
  
	HashMapDistance::VectorstructType Vector;

  //Write squeletor points 
	ImageType::IndexType Indextemp;
	for(GrayItImage.GoToBegin(); !GrayItImage.IsAtEnd(); ++GrayItImage)
	{
		Indextemp = GrayItImage.GetIndex();
		if(GrayItImage.Get()>0)
		{
			Vector =p_HashMap.m_GMHash_map[(Indextemp)];
			if( Vector.size() > 0)
			{
				m_GrayImageAfterdel->SetPixel(Indextemp,Vector[0].distance);
                 //GrayItImage.Set(Vector[0].distance);
			}
			else
			{
				m_GrayImageAfterdel->SetPixel(Indextemp , 0);
                 //GrayItImage.Set(0);
			}
		}
		else
		{
			m_GrayImageAfterdel->SetPixel(Indextemp, 0);
		}
	}
  
	if(write)
	{
		WriterType::Pointer writer = WriterType::New();
  
    //write squeletor Image
		writer->SetFileName( Image );
		writer->SetInput(m_GrayImageAfterdel);
		try{
			writer->Update();
		}
		catch(itk::ExceptionObject exp )
		{
			std::cerr << "Exception caught !" << std::endl;
			std::cerr <<     exp    << std::endl;
		}
	} 
} 


void toolsdistance::GenerateVectorImage(ImageType::Pointer GrayMatter, const char* filename)
{    
  //create a file .viz to store vectors
	std::string vizFileName (filename);
	int i ;
	i = vizFileName.find_last_of ('.') ;
	vizFileName.replace ( i+1, 3, "viz" ) ;
	char FileName[512];
	std::strcpy(FileName,vizFileName.c_str());
  
  //Declaration temporary variable 
	ImageType::IndexType indexG, indexW;
	HashMapDistance::VectorstructType W_vector;
	int inc = 0;
	Vector3D m_vectormanager ;
	float size;
  //iterator grey matter
	IteratorType GrayMatterImageIt( GrayMatter, GrayMatter->GetRequestedRegion() );
  
	for(GrayMatterImageIt.GoToBegin();!GrayMatterImageIt.IsAtEnd();++GrayMatterImageIt)
	{
		          
		if(GrayMatterImageIt.Get()>0)
		{
			indexG = GrayMatterImageIt.GetIndex();
			W_vector = p_HashMap.Get_HashMAp_G(indexG);
			size = W_vector.size();

			for(int l = 0; l<size; l++)
			{
				indexW=W_vector[l].Index;
				m_vectormanager.Add(indexG[0],indexG[1],indexG[2],indexW[0], indexW[1],indexW[2]);
				inc++;
			}
		} 
	}
	m_vectormanager.Save(FileName) ;
}

//not use
//create a neighborhood and calculate the max distance in the neighborhood
void toolsdistance::ComputeNeigborhoodDistance(ImageType::Pointer GrayImage, ImageType::Pointer DistanceMap, ImageType::Pointer BoundaryWhite)
{
  //declaration of temporary variable
	IteratorType BoundaryWhiteIt( BoundaryWhite, BoundaryWhite->GetRequestedRegion() );
	ImageType::IndexType IndexG, IndexGtemp;
  //create new images for save the noighborhood distances
	m_NeighDistanceW = CreateNewImageLike(BoundaryWhite);
	m_NeighDistanceG = CreateNewImageLike(GrayImage);

	for(BoundaryWhiteIt.GoToBegin();!BoundaryWhiteIt.IsAtEnd();++BoundaryWhiteIt)
	{
		if(BoundaryWhiteIt.Get()>0)// if it is a white matter boundary point
		{
			Neighborhood* p_Neighborhood = new  Neighborhood;
			p_Neighborhood->NeighborhoodImage(BoundaryWhite,BoundaryWhiteIt.GetIndex(), 3);//3 neigh algo
			IteratorType outputIt(p_Neighborhood->m_TemporaryRegion , p_Neighborhood->m_TemporaryRegion->GetRequestedRegion() );
      
			float D_max = 0;
			float D_temporary = 0;
      //curse of neigborhood
			for(outputIt.GoToBegin(); !outputIt.IsAtEnd();++outputIt)
			{              
				IndexGtemp = outputIt.GetIndex();
				if(GrayImage->GetPixel(outputIt.GetIndex())>0)//if it is a grey matter pixel
				{
					D_temporary = DistanceMap->GetPixel(outputIt.GetIndex());
          //maximum distance operator
					if(D_temporary >= D_max)
					{
						D_max = D_temporary;
						IndexG = outputIt.GetIndex();
					}              
				}
			}     
			m_NeighDistanceG->SetPixel(IndexG, D_max); // white matter boundary intensity = Dmax
			m_NeighDistanceW->SetPixel(BoundaryWhiteIt.GetIndex(),D_max); //grey matter point = Dmax, it is the furthest grey matter points pointing on the white matter
			delete p_Neighborhood;
		}
		else  m_NeighDistanceW->SetPixel(BoundaryWhiteIt.GetIndex(),0);
	}
}

ImagePointer toolsdistance::CreateNewImageLike(ImagePointer OriginalImage )
{
	ImagePointer outputImage = ImageType::New();
	outputImage->SetRegions( OriginalImage->GetRequestedRegion() );
	outputImage->CopyInformation( OriginalImage );
	outputImage->Allocate();
	return outputImage;
}

void toolsdistance::WriteGreyDanielsson(ImageType::Pointer GreyMatter, ImageType::Pointer DanMap, const char* FileName)
{
	ImagePointer MeshGrey;
  //create an image with the same porperties as the previous images
	MeshGrey = CreateNewImageLike( GreyMatter);
	IteratorType GrayMatterIt( GreyMatter, GreyMatter->GetRequestedRegion() );
	IteratorType DanIt(DanMap , DanMap->GetRequestedRegion() );
	IteratorType MeshGreyIt(MeshGrey ,MeshGrey->GetRequestedRegion() );

  //fill the MeshGrey image with the value of the danielsson distance map combines with the grey matter map
	for(GrayMatterIt.GoToBegin(),DanIt.GoToBegin(), MeshGreyIt.GoToBegin() ;
		   !GrayMatterIt.IsAtEnd(), !DanIt.IsAtEnd(), !MeshGreyIt.IsAtEnd()
				   ;++GrayMatterIt, ++DanIt, ++MeshGreyIt)
	{
		if(GrayMatterIt.Get()!=0)
		{
			MeshGreyIt.Set(DanIt.Get());
		}
		else  MeshGreyIt.Set(0);
	}
  
  //Write Image Grey Danielsson 
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName( FileName );
	writer->SetInput(MeshGrey);
	try{
		writer->Update();
	}
	catch(itk::ExceptionObject exp )
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr <<     exp    << std::endl;
	}                                                    
}

//write the white matter distance file
void toolsdistance::WriteWhiteFile(ImageType::Pointer WhiteDistMap,ImageType::Pointer  WhiteBoundaryImage,char* ParFileName, char* FileNameWhite, bool Par , bool Interp , float threshold)
{
	int ModeNb=1;
	if(Interp) ModeNb=2;

  //if there is a parcellation file
	if(Par)
	{
		for(int Mode = 1 ; Mode <= ModeNb ; Mode++)
		{
			//read parcellation file
			//ImagePointer ParImage;
			ImageType::Pointer ParImage = ImageType::New();
			if(Mode==1)//Mode without interpollation
			{
				ReaderType::Pointer ParReader = ReaderType::New();
				ParReader->SetFileName(ParFileName);
		
				try
				{
					ParReader->Update();
				}
				catch(...)
				{
					std::cerr << "Can't read the file" << std::endl;
				}
				ParImage = ParReader->GetOutput();

				//set the name of the updated parcellation file
				std::string par_outfilename(FileNameWhite);
		
				size_t found;
				found=par_outfilename.find_last_of( sep );
				par_outfilename =  par_outfilename.substr(0,found) + sep + "parcellationOnWhite.nrrd";
				std::cout<<" outfilename "<<par_outfilename<<std::endl;

				//write new parcellation 
				WriterType::Pointer par_writer = WriterType::New();
			
				par_writer->SetInput( ParImage );
				par_writer->SetFileName( par_outfilename );
				par_writer->UseCompressionOn();
				par_writer->Update();

			}
			if(Mode==2)//Mode with interpollation
			{	
				//Take the average WhiteDistMap as White Matter Distance Map
				AverageNeighborhood(WhiteDistMap,WhiteBoundaryImage);
		
				//read original parcellation file
				ImagePointer OrigParImage;
				ReaderType::Pointer reader = ReaderType::New();
				reader->SetFileName(ParFileName);
		
				try
				{
					reader->Update();
				}
				catch(...)
				{
					std::cerr << "Can't read the file" << std::endl;
				}
				OrigParImage = reader->GetOutput();
		
				// Create a new parcellation updated on the WM Distance Map
		
				//Create a new image with same spacing and size
				DuplicatorType::Pointer Dupl = DuplicatorType::New();
				Dupl->SetInputImage(WhiteDistMap);
				Dupl->Update();
				ParImage = Dupl->GetOutput();
				//ImageType::Pointer ParImage = Dupl->GetOutput();
		
				//Danielsson Distance Map Filter
				DanielssonDMFilter::Pointer danFilter = DanielssonDMFilter::New();
				danFilter->SetInput( OrigParImage );
				OffsetImageType::Pointer OffsetDistanceMap = OffsetImageType::New();
				ImageType::Pointer DistanceMap = ImageType::New();
				DistanceMap = danFilter->GetDistanceMap();
				OffsetDistanceMap = danFilter->GetVectorDistanceMap();
				danFilter->Update();
		
				//Use Danielsson outputs to create an improved parcellation
				RegionIteratorType WhiteDistMap_Iterator(WhiteDistMap, WhiteDistMap->GetRequestedRegion()) ;
				RegionIteratorType OrigParImage_Iterator(OrigParImage, OrigParImage->GetRequestedRegion()) ;
				RegionIteratorType DistanceMap_Iterator(DistanceMap, DistanceMap->GetRequestedRegion()) ;
				RegionIteratorType ParImage_Iterator(ParImage, ParImage->GetRequestedRegion()) ;
			
				WhiteDistMap_Iterator.GoToBegin();
				OrigParImage_Iterator.GoToBegin();
				DistanceMap_Iterator.GoToBegin();
				ParImage_Iterator.GoToBegin();
		
				float WhiteDistMap_Value,OrigParImage_Value,DistanceMap_Value;
				float background = 0;
		
				while(!WhiteDistMap_Iterator.IsAtEnd())
				{
					WhiteDistMap_Value = WhiteDistMap_Iterator.Get();
					
					if(WhiteDistMap_Value != background)
					{	
						OrigParImage_Value = OrigParImage_Iterator.Get();
						
						if(OrigParImage_Value == background)
						{
							DistanceMap_Value = DistanceMap_Iterator.Get();
							if(DistanceMap_Value <= threshold)
							{
								ImageType::IndexType InputIndex,OutputIndex;
								InputIndex = DistanceMap_Iterator.GetIndex();
								OutputIndex[0] = InputIndex[0] + OffsetDistanceMap->GetPixel(InputIndex)[0];
								OutputIndex[1] = InputIndex[1] + OffsetDistanceMap->GetPixel(InputIndex)[1];
								OutputIndex[2] = InputIndex[2] + OffsetDistanceMap->GetPixel(InputIndex)[2];
								ParImage_Iterator.Set(OrigParImage->GetPixel( OutputIndex ));
							}
							else
							{
								ParImage_Iterator.Set(background);
							}
						}
						else
						{
							ParImage_Iterator.Set(OrigParImage_Value);
						}
					}
					
					++ParImage_Iterator;
					++WhiteDistMap_Iterator;
					++OrigParImage_Iterator;
					++DistanceMap_Iterator;
				}
		
				//set the name of the updated parcellation file
				std::string par_outfilename(FileNameWhite);
		
				size_t found;
				found=par_outfilename.find_last_of( sep );
				par_outfilename =  par_outfilename.substr(0,found) + sep + "parcellationOnWhiteInterp.nrrd";
				std::cout<<" outfilename "<<par_outfilename<<std::endl;
		
				//write new parcellation 
				WriterType::Pointer par_writer = WriterType::New();
			
				par_writer->SetInput( ParImage );
				par_writer->SetFileName( par_outfilename );
				par_writer->UseCompressionOn();
				try
				{
					par_writer->Update();
				}
				catch(...)
				{
					std::cerr << "Can't write the file" << std::endl;
				}
			}

			// Computation of the maximum intensity
			maxImageCalculatorType::Pointer maxImageCalculator = maxImageCalculatorType::New();
			maxImageCalculator->SetImage(ParImage);
			maxImageCalculator->ComputeMaximum();
			int ParImageMax = (int) maxImageCalculator->GetMaximum();
			//declaration temporary variables
			IteratorType ParImageIt(ParImage , ParImage->GetRequestedRegion() );
			IteratorType WhiteIt(WhiteDistMap ,WhiteDistMap->GetRequestedRegion() );
			IndexVectType* A_index = new IndexVectType[ParImageMax+1];
			int MaxParcell = 0;

			//curse of parcellation file
			for(ParImageIt.GoToBegin(),WhiteIt.GoToBegin();!ParImageIt.IsAtEnd(), !WhiteIt.IsAtEnd();++ParImageIt, ++WhiteIt)
			{
				if(ParImageIt.Get()>0 && WhiteIt.Get()>0)//if it is a parcellation file point with a label != 0 and if it is a white matter boundary point
				{
					A_index[(int)ParImageIt.Get()].dist.push_back(WhiteIt.Get());//fill a array of vector : ex A[label] = {distance 1, distance 2, distance 3 ,...}	
				}
				if(ParImageIt.Get() > MaxParcell) MaxParcell = (int) ParImageIt.Get();
			} 
			unsigned int MaxVal = 0;
			for(int i = 0 ; i <= ParImageMax ; i++)
			{
			  if(A_index[i].dist.size()> MaxVal) MaxVal = A_index[i].dist.size();
			}
			//create a file to store the distance stored by label
			std::ofstream outfile;
			std::ofstream outfile2;
	
			std::string outfilename(FileNameWhite);
			std::string outfilename2(FileNameWhite);
			int i ;
			i = outfilename.find_first_of (".", 0) ;
			outfilename.erase(i);
			if(Mode == 1)
			{
				outfilename.insert(outfilename.size(),"_par.csv");
			}
			if(Mode == 2)
			{
				outfilename.insert(outfilename.size(),"_par_interp.csv");
			}
			std::cout<<" outfilename "<<outfilename<<std::endl;
			outfile.open ( outfilename.c_str() ) ; 
			//   outfile<<"Label "<<","<<"Distance"<<std::endl;
			//NEW SYL
			outfile<<"List of cortical thickness values par label"<<std::endl;
	
			i = outfilename2.find_first_of (".", 0) ;
			outfilename2.erase(i);
			if(Mode == 1)
			{
				outfilename2.insert(outfilename2.size(),"_par_array.csv");
			}
			if(Mode == 2)
			{
				outfilename2.insert(outfilename2.size(),"_par_array_interp.csv");
			}
			std::cout<<" outfilename "<<outfilename2<<std::endl;
			outfile2.open (outfilename2.c_str());
			outfile2<<" nbre of cortical thickness voxel cortex inside folds "<<","<<m_CompIns<<std::endl;
			outfile2<<" nbre of cortical thickness voxel cortex at the boundary "<<","<<m_CompBound<<std::endl;
			outfile2<<"Label "<<","<<"Average"<<","<<"Median"<<","<<"Std Dev"<<","<<"Nb Of Elem"<<","<<"Min"<<","<<"Max"<<std::endl;

			// NEW SYL
			//for each line of the file
	
			float ThickVal = 0;
			for(int Line = -1 ; Line < (int)MaxVal+1 ; Line++)
			{		
				//For each column: there are as many columns as labels
				for(int Parcell = 0 ; Parcell <= MaxParcell ; Parcell++)
				{
					//For the first line, the labels are written
					if(Line == -1)
					{
						outfile << A_index[Parcell].dist.size() << ",";
					}
					if(Line == 0)
					{
						outfile<<"Lbl "<< Parcell << ",";
					}
					//For all the other lines, the thickness value is copied
					if(Line > 0)
					{	
					  if((int)A_index[Parcell].dist.size() > Line-1) 
						{
							ThickVal = A_index[Parcell].dist[Line-1];
						}
						else
						{
							ThickVal = 0;
						}
						outfile << ThickVal << ",";
					}
				}
				outfile << std::endl;
		
			}
			// END NEW SYL
			//curse the array of distance in function of label
			for(int i = 0 ; i<=ParImageMax; i++)
			{
				float Sum = 0;
				float Ave = 0;
				float Std = 0;
				//    outfile<<i<<""<<",";
				int k =0;      
				for(unsigned int j=0; j<A_index[i].dist.size(); j++)
				{
					//  outfile<<A_index[i].dist[j]<<",";
					Sum = Sum + A_index[i].dist[j];
					k++;
					if(k>200)
					{
						//  outfile<<std::endl;
						k=0;
						// outfile<<""<<",";
					}
				}
				//Average value
				Ave = Sum/A_index[i].dist.size();
				//Standard deviation computation
				for(unsigned int j=0; j<A_index[i].dist.size(); j++)
				{
					Std = Std + ( (Ave - A_index[i].dist[j]) * (Ave - A_index[i].dist[j]) );
				}
				Std = sqrt (Std / (A_index[i].dist.size()-1) );
				//median,min,max
				//sort the table
				float Median = 0;
				float Min = 0;
				float Max = 0;
	
				std::vector<float> myvector;
				for(unsigned int j=0; j<A_index[i].dist.size(); j++)
				{
					myvector.push_back(A_index[i].dist[j]);
				}
				if(A_index[i].dist.size() != 0)
				{
					sort (myvector.begin(), myvector.begin()+A_index[i].dist.size());
					Min=myvector[0];
					Max=myvector[A_index[i].dist.size()-1];
					if ( (A_index[i].dist.size() % 2) == 0)
					{
						Median = (myvector[(A_index[i].dist.size())/2-1]+myvector[(A_index[i].dist.size())/2])/2;			
					} 
					else
					{
						Median = myvector[(A_index[i].dist.size())/2-0.5];
					}
				}
				//outfile<<std::endl;
				//outfile<<" Average[ "<<i<<"] = "<<","<< Ave <<std::endl;
				//outfile<<" Standard Dev[ "<<i<<"] = "<<","<< Std <<std::endl;
				//outfile<<" Number of Element[ " <<i<< "] = " << "," <<  A_index[i].dist.size() << std::endl;
				outfile2<<i<<","<< Ave << "," <<  Std << "," << Median << "," << A_index[i].dist.size() << "," << Min << "," << Max << std::endl;
				//outfile<<std::endl;
			}
			delete [] A_index;
			outfile.close();
			outfile2.close();
		}
	}
  	//if there is non parcellation file, simply write the distance in a file 
	else
	{
		for(int Mode = 1 ; Mode<=ModeNb ; Mode++)
		{	

			if(Mode==2)
			{
				AverageNeighborhood(WhiteDistMap,WhiteBoundaryImage);
			}
			IteratorType WhiteIt(WhiteDistMap ,WhiteDistMap->GetRequestedRegion() );
	
			std::ofstream outfile;
			std::string outfilename(FileNameWhite);

			int k ;
			k = outfilename.find_first_of (".", 0) ;
			outfilename.erase(k);
			if(Mode==1)
			{
				outfilename.insert(outfilename.size(),"_interp.csv");
			}
			if(Mode==2)
			{
				outfilename.insert(outfilename.size(),".csv");
			}
			outfile.open ( outfilename.c_str() ) ;
			outfile<<" Distance :"<<std::endl;
			//    int k =0;
			float Sum = 0;
			float Ave = 0;
			float Std = 0;
			float Compteur = 0;
			for(WhiteIt.GoToBegin(); !WhiteIt.IsAtEnd(); ++WhiteIt)
			{
				if(WhiteIt.Get()>0)
				{
					outfile<<WhiteIt.Get()<<","<<std::endl;
					Sum = Sum + WhiteIt.Get();
					Compteur ++;
					//        k++;
					//        if(k>200)
					//        {
					//          outfile<<std::endl;
					//          k=0;
					//        }  
				}
			}
			Ave = Sum / Compteur;
			//Standard variation computation
			for(WhiteIt.GoToBegin(); !WhiteIt.IsAtEnd(); ++WhiteIt)
			{
				if(WhiteIt.Get()>0)
				{
					Std = Std + (Ave - WhiteIt.Get()) * (Ave - WhiteIt.Get());	
				}
			}
			Std = sqrt(Std / Compteur);
			outfile<<std::endl;
			outfile<<" Average = "<<","<<Ave<<std::endl;
			outfile<<" Standard Dev = "<<","<< Std <<std::endl;
			outfile<<" Number of Element" << "," <<  Compteur << std::endl;
			outfile.close();
		}
	}
	
}

//write float pixel type images  
void toolsdistance::WriteImage(ImageType::Pointer InputImage, const char * FileName)
{
	WriterType::Pointer writer = WriterType::New();
  //write squeletor Image
	writer->SetFileName( FileName );
	writer->SetInput(InputImage);
	try{
		writer->Update();
	}
	catch(itk::ExceptionObject exp )
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr <<     exp    << std::endl;
	}                                            
 
}           

// To separate cortical thickness cortex inside folds versus cortex at the boundary and write it like ????
void toolsdistance::SepareInsideVSBoundary(ImageType::Pointer GreyBoundaryImage, ImageType::Pointer DistanceMapGrey
		, char * FileName)
{
  //iterator Boundary Image
	IteratorType GreyBoundaryIt( GreyBoundaryImage, GreyBoundaryImage->GetRequestedRegion() );
  //iterator Grey distance map Image
	IteratorType GreyDistanceMapIt( DistanceMapGrey, DistanceMapGrey->GetRequestedRegion() );
  
  //Copy the properties of the Image to allocate and create new Image 
	m_InsideFoldsThicknessIm = CreateNewImageLike(GreyBoundaryImage);
	m_BoundaryThicknessIm = CreateNewImageLike(GreyBoundaryImage);  

	m_CompIns = 0;
	m_CompBound = 0;

  //Curse the GreyDistanceMap Image and the Grey boundary Image and fill the new images : cortex inside folds versus cortex at the boundary
	for(GreyBoundaryIt.GoToBegin(),GreyDistanceMapIt.GoToBegin() ;
		   !GreyBoundaryIt.IsAtEnd(), !GreyDistanceMapIt.IsAtEnd();++GreyBoundaryIt, ++GreyDistanceMapIt)
	{
		if (GreyBoundaryIt.Get() !=0) // if it is a boundary point
		{
			m_InsideFoldsThicknessIm->SetPixel(GreyBoundaryIt.GetIndex(), 0);
			m_BoundaryThicknessIm->SetPixel(GreyBoundaryIt.GetIndex(), GreyDistanceMapIt.Get());
			m_CompBound++;
		}
		else if (GreyDistanceMapIt.Get() != 0) // if it is an inside points with a thickness value
		{
			m_InsideFoldsThicknessIm->SetPixel(GreyBoundaryIt.GetIndex(), GreyDistanceMapIt.Get());
			m_BoundaryThicknessIm->SetPixel(GreyBoundaryIt.GetIndex(), 0);
			m_CompIns++;
		}
		else // if it is not a boundary point and if there is no value
		{
			m_InsideFoldsThicknessIm->SetPixel(GreyBoundaryIt.GetIndex(), 0);
			m_BoundaryThicknessIm->SetPixel(GreyBoundaryIt.GetIndex(), 0);
		}
	}

  //Name files
	std::string Name = FileName;
	//std::string Slash = "/";
	int SepPosition = Name.rfind(sep,Name.length());
	std::string Path1;
	Path1 = Name.substr(0,SepPosition+1);
	std::string Name1 = "InsideCorticalThickness.mha";
	std::string Name2 = "BoundaryCorticalThickness.mha";
	std::string Final1 = Path1  + Name1;
	std::string Final2 = Path1  + Name2;
	std::strcpy(m_insideFileName,Final1.c_str());
	std::strcpy(m_BoundaryFileName,Final2.c_str());

  //Write Images
	WriteImage(m_InsideFoldsThicknessIm, m_insideFileName);
	WriteImage(m_BoundaryThicknessIm, m_BoundaryFileName);
  
}

void toolsdistance::CounterWhiteGrey(ImageType::Pointer WhiteMatterImage, ImageType::Pointer GreyMatterImage)
{
  //iterator White Matter Image
	IteratorType WhiteIt( WhiteMatterImage, WhiteMatterImage->GetRequestedRegion() );
  //iterator Grey Matter Image
	IteratorType GreyIt( GreyMatterImage, GreyMatterImage->GetRequestedRegion() );

	int countGrey = 0;
	int countWhite = 0;

	for(GreyIt.GoToBegin(),WhiteIt.GoToBegin() ;
		   !GreyIt.IsAtEnd(), !WhiteIt.IsAtEnd();++GreyIt, ++WhiteIt)
	{
		if(GreyIt.Get()!=0) countGrey++;
		if(WhiteIt.Get()!=0) countWhite++;
	}

	std::cout<<" countGrey = "<<countGrey<<std::endl;
	std::cout<<" countWhite = "<<countWhite<<std::endl;
}


