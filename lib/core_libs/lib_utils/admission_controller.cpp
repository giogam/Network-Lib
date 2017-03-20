#include "../../../include/admission_controller.h"
#include "../../../include/enum_classes.h"

int admission_controller::admission_control(std::string address)
{
  /* per prima cosa controllo la Blacklist */  
  if(check_blist(address))
      return client_status_t::CLI_BLS; 

    else if(check_flist(address)) 
    {
      auto res = firstList.find(address);
      /* richiesta dopo meno di 120 sec */
      if((res->second.first).get_time() < 120) 
      {
        /* richiesta dopo meno di 120 sec con primo avvertimento */
	if(res->second.second == true)
        {
          blackList.insert(res->first);
          return client_status_t::CLI_BAN;
        }
        /* richiesta dopo meno di 120 sec senza primo avvertimento */
        else
        {
          res->second.second = true;
          return client_status_t::CLI_ADV;
        }
      }
      /* richiesta dopo più di 120 sec  */
      else
      {
        (res->second.first).start();
        res->second.second = false;
        return client_status_t::CLI_SRV;
      }
    }
    /* richiesta mai avvenuta */
    else
    {
      timer t;
      t.start();
      firstList.insert(std::make_pair(std::string(address), cliStat(t,false)));
      return client_status_t::CLI_SRV;
    }
}


bool admission_controller::check_flist(std::string address)
{
    auto res = firstList.find(address);

    return (res != firstList.end()) ? true : false ;
}

bool admission_controller::check_blist(std::string address)
{
  auto res = blackList.find(address);

  return (res != blackList.end()) ? true : false;

}
