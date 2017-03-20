#ifndef _file_handler_h
#define _file_handler_h

#include "lib_base.h"

class file_handler
{

public:
  file_handler(std::string lst_name);
  inline std::vector<std::string> get_list()
  {
    return fileList;
  }
  void get_stream(std::string filename, std::fstream & stream);

private:
  std::vector<std::string> fileList;

};

#endif /* _file_handler_h */
