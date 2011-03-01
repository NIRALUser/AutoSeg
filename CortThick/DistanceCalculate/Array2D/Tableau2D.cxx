#include <stdlib.h> /* Définit NULL */
#include "Tableau2D.h"
#include "itkImage.h"


typedef float PixelType;
typedef itk::Image< PixelType, 6 > ImageType6;

template<class xxx, int BOX> 
Tableau2D<xxx, BOX>::Tableau2D(void)
{
  nb_box=BOX;
  box=new xxx*[BOX];
  for( int x = 0 ; x < BOX; x++)
	box[x] = new xxx[6];
}

template<class xxx, int BOX> 
Tableau2D<xxx, BOX>::~Tableau2D(void)
{ 
  for(int x =0; x < BOX; x++)
	delete [] box[x];
  delete [] box;
}

template<class xxx, int BOX> 
void Tableau2D<xxx, BOX>::Verifie(int indicex)
{
  xxx** old_box;
  int old_nb_box;
  int i;

  if(indicex>=nb_box)
    {
      old_box=box;
      old_nb_box=nb_box;

      nb_box=(indicex/BOX+1)*BOX;
 
	box=new xxx*[nb_box];
      for( int x = 0 ; x < nb_box; x++)
	box[x] = new xxx[6];

      for(i=0;i<old_nb_box;i++)
	{
	for(int j = 0; j<6 ; j++)
	box[i][j]=old_box[i][j];
	}

	for(int x =0; x < old_nb_box; x++)
	delete [] old_box[x];
      delete [] old_box;
    }
} 

template<class xxx, int BOX> 
void Tableau2D<xxx, BOX>::Set(int indicex, int indicey, xxx value)
{
  Verifie(indicex);
  box[indicex][indicey]=value;
}

template<class xxx, int BOX> 
xxx Tableau2D<xxx, BOX>::Get(int indicex, int indicey)
{
  return box[indicex][indicey];
}

template<class xxx, int BOX> 
xxx* Tableau2D<xxx, BOX>::Get_Adresse(int indicex)
{
        return box[indicex];
}        

template class Tableau2D<int>;
template class Tableau2D<float>;
template class Tableau2D<ImageType6::IndexType>;


