#include "../../../include/match_logger.h"


void match_logger::new_match(std::string _matchID)
{
  std::stringstream _match_fname;
  
  _match_fname << _log_path << _matchID << ".match";
  _match_log_files[_matchID]= _match_fname.str();
  
}

void match_logger::write_log(std::string _message , std::string _matchID )
{	
  std::ofstream match_file;
    
    auto _fname = _match_log_files.find(_matchID);
    
    match_file.open(_fname->second);
    
      match_file << _message;
      match_file << std::endl;
 
    match_file.close();
}