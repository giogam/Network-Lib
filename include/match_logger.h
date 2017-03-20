#ifndef _match_logger_h
#define _match_logger_h

#include "lib_base.h"

class match_logger
{
  public:
    match_logger(std::string log_path ): _log_path(log_path) { };
   ~match_logger();

    void new_match(std::string _matchID);
    void write_log(std::string _message , std::string _matchID );

  private:
    std::string _log_path;
    std::map<std::string,std::string> _match_log_files;

};

#endif /* _match_logger_h */
