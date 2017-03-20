#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/base_socket.h"
#include "../../include/udp_agent.h"
#include "../../include/enum_classes.h"
#include "../../include/ttt_player.h"
#include "../../include/user_manager.h"
#include "../../include/ttt_session.h"
#include "../../include/file_logger.h"

int main(int argc , char *argv[])
{
  int mode  = 0;
  int maxfd;
  int ret;
  fd_set rset;
  file_logger *userLog;
  user_manager<ttt_player> * supervisor;
  std::vector<ttt_player *> usersV;
  std::vector<ttt_player *> lazyV;
  std::vector<std::string> exitV;
  ttt_session *sessionV;

  if((argc != 4) || ((strcmp(argv[1],"v4") != 0) && (strcmp(argv[1],"v6")) != 0) )
  {
      std::cout << "[-] usage: " << argv[0] << " <mode> <address> <port>" << "\n";
      _exit (1);
  }

    mode = (strcmp(argv[1],"v4") == 0) ? AF_INET : AF_INET6;
    ip_address server_addr(argv[2] , argv[3] , mode , SOCK_STREAM , AI_PASSIVE );
    tcp_socket tttTServ(mode);

    tttTServ.bind(server_addr);
    tttTServ.listen();

    udp_agent *ctrlUSock = new udp_agent(mode);

    supervisor = new user_manager<ttt_player>(usersV);
    sessionV = new ttt_session(true);

    userLog = new file_logger("./resources/users.lst");

    std::cout << "[+] Welcome to TTT-FT Server!" << "\n";
    std::cout << "[+] Resuming previous connections.." << "\n";

      std::fstream userFile;
      std::string line;
      std::vector<std::string> userList;
      userFile.open("./resources/users.lst");

      while(std::getline(userFile, line))
      {
        if(!line.empty())
          userList.push_back(line);
      }

      if(!userList.empty())
      {
        for(auto it = userList.begin(); it != userList.end(); ++it )
        {
          std::stringstream _msg;
          std::vector<std::string> user;
          std::string buf;
          std::stringstream ss((*it));

          while (ss >> buf)
            user.push_back(buf);

          std::cout << user[0] << "\n";
          std::cout << user[1] << "\n";

          ip_address cli_addr(user[0],user[1],mode,SOCK_DGRAM);
          _msg << argv[2] << " " << argv[3] << " " << argv[1] << "\n ";
          ctrlUSock->send_message(_msg.str(),cli_addr);
        }
        userList.clear();
      }
      std::cout << "[+] Waiting for connections.." << "\n";


    loop_always()
    {
      FD_ZERO(&rset);

      FD_SET(tttTServ.get_id(),&rset);

      sessionV->do_ttt_clean();

      for(auto it = exitV.begin(); it != exitV.end(); ++it )
      {
        auto user = std::find(usersV.begin(),usersV.end(),supervisor->get_user((*it)));
        usersV.erase(user);
      }
      exitV.clear();

      for(auto it = lazyV.begin(); it != lazyV.end(); ++it )
      {
        auto user = std::find(usersV.begin(),usersV.end(),(*it));
        usersV.erase(user);
      }
      lazyV.clear();

      userLog->write_log(supervisor->get_ip_list());

      if(usersV.size() > 0)
      {
        auto it = std::max_element(usersV.begin(),usersV.end(),base_socket::sock_compare<ttt_player>);
        maxfd = (*it)->get_id();
      }
      else
      {
        maxfd = tttTServ.get_id();
      }

      for(auto it = usersV.begin(); it != usersV.end(); ++it )
          FD_SET((*it)->get_id(),&rset);

      ret = select(maxfd+1,&rset,NULL,NULL,NULL);
      if(ret == -1)
      {
        std::cout << "TTTServer::select(): " << std::strerror(errno) << "\n";
        _exit (1);
      }

      if(FD_ISSET(tttTServ.get_id(),&rset)) /* qualcosa sul listener*/
      {
        ttt_player* newUser = new ttt_player(mode);
        tttTServ.accept(newUser);
        std::stringstream msg;
        std::vector<std::string> hail;

        try
        {
          hail = newUser->hail();
          supervisor->check_nick(hail[1]);
          CLEAR_MSG(msg);
          msg << "OLEH" << " " << "WELCOME" << "\n";
          newUser->send_message(msg.str());
          newUser->set_nick(hail[1]);
          newUser->set_status(false);
          newUser->set_ctrl(hail[2]);
          usersV.push_back(newUser);
          userLog->write_log(supervisor->get_ip_list());
        }
        catch( generic_exception & e)
        {
          switch(e.get_errcode())
          {
            case socket_status_t::BAD_READ:
              std::cout << "[-] Client Shutdown " << "\n";
              delete newUser;
              break;
            case user_err_status_t::US_BAD_FORMAT:
              std::cout << "[-] Dati non corretti " << "\n";
              delete newUser;
              break;
            case user_status_t::TAKEN:
              std::cout << "[-] Usernme Already Taken " << "\n";
              CLEAR_MSG(msg);
              msg << "ERR" << " " << "BAD_USER" << "\n";
              newUser->send_message(msg.str());
              delete newUser;
              break;
          }
        }
      }
      else /* qualcosa dai clients */
      {
        std::stringstream msg;

          for(auto it = usersV.begin(); it != usersV.end(); ++it)
          {
            if(FD_ISSET((*it)->get_id(),&rset))
            {
              try
              {
                std::vector<std::string> message;
                message = (*it)->receive_message();

                if(strcmp( message[0].c_str(),"LIST") == 0 ) /* LIST */
                {
                  CLEAR_MSG(msg);
		              msg << "LIST" <<  " " << supervisor->get_list() << "\n";
		              (*it)->send_message(msg.str());
                }
                else if(strcmp( message[0].c_str(),"PLAY") == 0 ) /*PLAY|user_2 */
                {
                  std::string session;
                  std::stringstream _path;
                  std::ofstream match_file;
                  supervisor->check_user(message[1]);
                  (*it)->set_status(true);
                  session = sessionV->new_ttt_session((*it),supervisor->get_user(message[1]));
                  (*it)->set_session(session);
                  (*it)->set_number(0);

                    _path << "./resources/matches/" << session << ".se";
                    match_file.open(_path.str());

                      match_file << (*it)->get_nick();
                      match_file << "\n";
                      match_file << message[1];
                      match_file << std::endl;

                    match_file.close();

                  std::cout << "[+] Started new session " << session << "\n";

                }
                else if(strcmp( message[0].c_str(),"RESUME") == 0 ) /*RESUME|id|player_num */
                {
                  std::vector<std::string> peerList;
                  std::fstream matchFile;
		  std::ofstream newFile;
		  std::string new_session;
                  std::stringstream _path;
                  std::string _line;

                  _path << "./resources/matches/" << message[1] << ".se";
                  matchFile.open(_path.str());

		  while(std::getline(matchFile, _line))
                  {
                      peerList.push_back(_line);
                  }
                  matchFile.close();
                  
		  new_session = sessionV->resume_ttt_session(supervisor->get_user(peerList[0]) , supervisor->get_user(peerList[1]) , message[1],message[2] );
		   
		   std::remove(_path.str().c_str());
		   
		   _path.str("");
		   _path.clear();
		   
		   _path << "./resources/matches/" << message[1] << ".match";
		   std::remove(_path.str().c_str());
		   
		   _path.str("");
		   _path.clear();
		   
		    _path << "./resources/matches/" << new_session << ".se";
		    newFile.open(_path.str());
		  
		    newFile << peerList[0];
		    newFile << "\n";
		    newFile << peerList[1];
		    newFile << std::endl;

                    newFile.close();
		  
		}
                else if(strcmp( message[0].c_str(),"MOV") == 0 ) /* MOV|id|row|col */
                {
		  bool _clean;
		  std::stringstream _path;
		  
		  if(message.size() != 4)
		  {
		    CLEAR_MSG(msg);
		    msg << "ERR" << " " <<"MOV" << "\n";
		    (*it)->send_message(msg.str());
		    continue;
		  }
                  _clean = sessionV->dispatch_ttt_move((*it),message[1],message[2],message[3]);
		  
		  if(_clean)
		  {
		   _path << "./resources/matches/" << message[1] << ".se";
                   std::remove(_path.str().c_str());
		   _path.str("");
		   _path.clear();
		   _path << "./resources/matches/" << message[1] << ".match";
		   std::remove(_path.str().c_str());
		  }
		  
		}
                else if(strcmp( message[0].c_str(),"LOGOUT") == 0 ) /* LOGOUT */
                {
                  std::cout << "Logout " << (*it)->get_nick() << "\n";
                  CLEAR_MSG(msg);
                  msg << "ACK" << " " <<"LOGOUT" << "\n";
                  (*it)->send_message(msg.str());
                  (*it)->close();
                  exitV.push_back((*it)->get_nick());
                }
                 else if(strcmp( message[0].c_str(),"EXIT") == 0 ) /* EXIT|id */
                {
                  std::stringstream _path;
                   sessionV->close_full_ttt_session((*it),message[1]);
                   _path << "./resources/matches/" << message[1] << ".se";
                   std::remove(_path.str().c_str());
		   _path.str("");
		   _path.clear();
		   _path << "./resources/matches/" << message[1] << ".match";
		   std::remove(_path.str().c_str());
                }
                }
                catch(generic_exception & e)
              {
                switch(e.get_errcode())
                {
                  case socket_status_t::BAD_READ:
                    std::cout << "[-] Client Shutdown " << "\n";
                    if((*it)->is_busy())
                    {
                      std::stringstream _path;
		      sessionV->close_half_ttt_session((*it),(*it)->get_session());
		      _path << "./resources/matches/" << (*it)->get_session() << ".se";
		      std::remove(_path.str().c_str());
		      _path.str("");
		      _path.clear();
		      _path << "./resources/matches/" << (*it)->get_session() << ".match";
		      std::remove(_path.str().c_str());
                    }
                    lazyV.push_back(*it);
                    userLog->write_log(supervisor->get_ip_list());
                    continue;
                  case user_status_t::BUSY:
                    CLEAR_MSG(msg);
                    msg << "ERR" << " " << "BUSY" << "\n";
                    (*it)->send_message(msg.str());
                    break;
                    case user_status_t::NONEX:
                      CLEAR_MSG(msg);
                      msg << "ERR" << " " << "NONEX" << "\n";
                      (*it)->send_message(msg.str());
                      break;
                  case ttt_board_err_t::BAD_MOVE:
                    CLEAR_MSG(msg);
                    msg << "ERR" << " " << "MOVE" << "\n";
                    (*it)->send_message(msg.str());
                    break;
                  case ttt_board_err_t::BAD_TURN:
                    CLEAR_MSG(msg);
                    msg << "ERR" << " " <<  "TURN" << "\n";
                    (*it)->send_message(msg.str());
                    break;
                  }
                }
              }
            }
          }

    }
      return 0;
}
