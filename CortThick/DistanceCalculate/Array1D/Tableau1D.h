
#ifndef TABLEAU1D_H
#define TABLEAU1D_H

/*! \brief class template array.
*
* allocation of new box each ten box full
*/

template<class xxx>
class Tableau1D
{
private:

  int nb_box;
  xxx* box;
 
  /**	Check if the indice can be stored, if not new allocation */
  void Verifie(int indice);
 
public:

  Tableau1D(void);
  ~Tableau1D(void);

  /** this function fill the array */
  void Set(int indice, xxx value);
  xxx Get(int indice);
  
  int GetNbbox(void) {return nb_box;}
  
};

#endif

