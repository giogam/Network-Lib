#include "../../../include/file_handler.h"

file_handler::file_handler(std::string lst_name)
{
  std::fstream inFile;
  std::string line;
  inFile.open(lst_name.c_str());

    while(std::getline(inFile, line))
    {
      fileList.push_back(line);
    }
}

void file_handler::get_stream(std::string filename, std::fstream & stream)
{
  stream.open(filename.c_str(),std::fstream::binary);
}
