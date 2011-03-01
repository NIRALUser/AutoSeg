#include <stdlib.h> /* Définit NULL */
#include "Tableau1D.h"
#include "itkImage.h"


typedef float PixelType;
typedef itk::Image< PixelType, 6 > ImageType6;


//! the memory is alloued by ten box each time
#define BOX 10

template<class xxx> Tableau1D<xxx>::Tableau1D(void)
{
  nb_box=BOX;
  box=new xxx[BOX];
}

template<class xxx> Tableau1D<xxx>::~Tableau1D(void)
{
  delete [] box;
}

template<class xxx> void Tableau1D<xxx>::Verifie(int indice)
{
  xxx* old_box;
  int old_nb_box;
  int i;

  if(indice>=nb_box)
    {

      old_box=box;
      old_nb_box=nb_box;

      nb_box=(indice/BOX+1)*BOX;
      box=new xxx[nb_box];
      
      for(i=0;i<old_nb_box;i++)
	box[i]=old_box[i];

      delete [] old_box;
    }
} 

template<class xxx> void Tableau1D<xxx>::Set(int indice, xxx value)
{
  Verifie(indice);
  box[indice]=value;
}

template<class xxx> xxx Tableau1D<xxx>::Get(int indice)
{
  return box[indice];
}


template class Tableau1D<int>;
template class Tableau1D<float>;
template class Tableau1D<ImageType6::IndexType>;

