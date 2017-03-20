#ifndef _admission_controller_h
#define _admission_controller_h

#include "lib_base.h"
#include "timer.h"


class admission_controller
{

  public:
    /* cliStat conterrà il tempo dell'ultima richiesta 
     * da parte di un client e un valore booleano per
     * indicare se un client è stato già avvertito o no.
     */
    typedef std::pair<timer,bool> cliStat;

    int admission_control(std::string address);

  private:
    bool check_flist(std::string address);
    bool check_blist(std::string address);

    /* firstList è la lista dove vengono inseriti i client avvertiti */  
    std::map<std::string,cliStat> firstList;
    /* blackList è la lista dove vengono inseriti i client bannati in modo permanente */  
    std::set<std::string> blackList;

};


#endif /* _admission_controller_h */
