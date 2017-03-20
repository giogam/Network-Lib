#ifndef _phone_db_h
#define _phone_db_h

#include "lib_base.h"

class phone_db
{
  public:
    /*
     * listEntry è composto da due stringhe una per 
     * il Nome e uan per il Numero
     */
    typedef std::pair<std::string,std::string> listEntry;

    phone_db(std::string db_name);
    std::vector<std::string> search(std::string param);

  private:
    /*  
     * multimap di oggetti listEntry: multimap permette
     * di avere più oggetti per singola chiave. Dato che
     * ci possono essere più entry per il medesimo cognome
     * contactList sarà indicizzata con il cognome e il 
     * resto sarà nella listEntry.
     */
    std::multimap<std::string,listEntry> contactList;

};

#endif /* _phone_db_h*/
