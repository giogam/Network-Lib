#ifndef _file_logger_h
#define _file_logger_h

#include "lib_base.h"

class file_logger
{
  public:
    file_logger(std::string log_path ): _log_path(log_path) { };
   ~file_logger();

    void write_log(std::string _message);

  private:
    std::string _log_path;

};

#endif /* _file_logger_h */
