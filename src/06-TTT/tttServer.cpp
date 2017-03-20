#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/base_socket.h"
#include "../../include/ttt_player.h"
#include "../../include/enum_classes.h"
#include "../../include/user_manager.h"
#include "../../include/ttt_session.h"
#include "../../include/enum_classes.h"

int main(int argc , char *argv[])
{
  int mode  = 0;
  int maxfd;
  int ret;
  fd_set rset;
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

    supervisor = new user_manager<ttt_player>(usersV);
    sessionV = new ttt_session(false);

    std::cout << "[+] Welcome to TTT Server!" << "\n";
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
          usersV.push_back(newUser);
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
                  supervisor->check_user(message[1]);
                  (*it)->set_status(true);
                  session = sessionV->new_ttt_session((*it),supervisor->get_user(message[1]));
                  (*it)->set_session(session);
                  (*it)->set_number(0);
                  std::cout << "[+] Started new session " << session << "\n";

                }
                else if(strcmp( message[0].c_str(),"MOV") == 0 ) /* MOV|id|row|col */
                {
                  if(message.size() != 4)
		  {
		    CLEAR_MSG(msg);
		    msg << "ERR" << " " <<"MOV" << "\n";
		    (*it)->send_message(msg.str());
		    continue;
		  }
		  sessionV->dispatch_ttt_move((*it),message[1],message[2],message[3]);
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
                   sessionV->close_full_ttt_session((*it),message[1]);
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
                      sessionV->close_half_ttt_session((*it),(*it)->get_session());
                    }
                    lazyV.push_back(*it);
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
