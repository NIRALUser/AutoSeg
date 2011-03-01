        #include "CalculateAngle.h"

CalculateAngle::CalculateAngle(){}
CalculateAngle::~CalculateAngle(){}

float CalculateAngle::Get_Dmax()
{
  return m_Dmax;
}

float CalculateAngle::Get_Angle()
{
  return m_Angle;
}

void CalculateAngle::Calcul_Dmax(ImageType::Pointer GreyMatter, ImageType::Pointer DistanceMap)
{
  //Iterartor Grey and Danielsson Distance Map
  IteratorType GreyMatterIt(GreyMatter ,GreyMatter->GetRequestedRegion() );
  IteratorType DistanceMapIt(DistanceMap,DistanceMap->GetRequestedRegion());
  //temporary varible to calculate distance max
  float Dist_max = 0;
  float Dist_max_temp = 0;
  //curse of GreyMatterImage and Danielsson DistanceMap
  for(GreyMatterIt.GoToBegin(),DistanceMapIt.GoToBegin() ; !GreyMatterIt.IsAtEnd(), !DistanceMapIt.IsAtEnd(); ++GreyMatterIt,++DistanceMapIt )
  {
    //If it s a Grey Matter point
    if(GreyMatterIt.Get()>0)
    {
       Dist_max_temp = DistanceMapIt.Get();
       if(Dist_max_temp > Dist_max)
        Dist_max = Dist_max_temp;
    }                
            
  }
  //As image are noisy, the xx% of the distance max is tooken
  m_Dmax  = 7*Dist_max/10;     
}
             
void CalculateAngle::calcul_Angle(const ImageType::SpacingType& sp, float Distance)
{
  //Caculation of the factor a and b in f(x) = b*exp(-a*x)        
  float a, b, spmin;
  float Pi = 3.141592;
  
  //Caculation of the minimum distance which is the spmin, space min
  spmin = min (sp[0], min(sp[1], sp[2]));
  // a = 
  a = -0.6931/(spmin-m_Dmax);
  // b = 
  b =Pi*exp(a*spmin);
     
  if((Distance < m_Dmax) || (Distance > spmin))
    m_Angle = b*exp(-a*Distance);
  else if(Distance > m_Dmax)
    m_Angle = b*exp(-a*m_Dmax);
}

float CalculateAngle::CalculAngleVector(ImageType::IndexType IndexWM2,ImageType::IndexType IndexGM, ImageType::IndexType V1 )
{
  ImageType::IndexType V2;
  V2[0] = IndexWM2[0] - IndexGM[0];
  V2[1] = IndexWM2[1] - IndexGM[1];
  V2[2] = IndexWM2[2] - IndexGM[2];
  //Calcul de l'angle V1, V2
  //scalar
  float scalar = V1[0] * V2[0] + V1[1] * V2[1] + V1[2] * V2[2] ;
  float normeV1 = sqrt( (float)V1[0] * V1[0] + V1[1] * V1[1] + V1[2] * V1[2]);
  float normeV2 = sqrt( (float)V2[0] * V2[0] + V2[1] * V2[1] + V2[2] * V2[2]);
  float cosphi = scalar/(normeV1*normeV2);
  return cosphi;
}
                        
