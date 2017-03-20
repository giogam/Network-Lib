#include "../../../include/file_logger.h"

void file_logger::write_log(std::string _message )
{
  std::ofstream log_file;

    log_file.open(_log_path);

      log_file << _message;
      log_file << std::endl;

    log_file.close();
}
