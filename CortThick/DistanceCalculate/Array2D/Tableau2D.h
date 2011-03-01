
#ifndef TABLEAU2D_H
#define TABLEAU2D_H

/*! \brief class template array.
*
* allocation of new box each ten box full
*/

template<class xxx,int BOX=10>
class Tableau2D
{
private:

  int nb_box;
  xxx** box;
 
  /**	Check if the indice can be stored, if not new allocation. */
  void Verifie(int indicex);
 
public:

  Tableau2D(void);
  ~Tableau2D(void);

  /** this function fill the array. */
  void Set(int indicex, int indicey, xxx value);
  xxx Get(int indicex , int indicey);
  
  int GetNbbox(void) {return nb_box;}
  xxx* Get_Adresse(int indicex);
};

#endif

