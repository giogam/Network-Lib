#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/base_socket.h"
#include "../../include/enum_classes.h"
#include "../../include/chat_user.h"
#include "../../include/user_manager.h"
#include "../../include/chat_msg.h"
#include "../../include/chat_session.h"

int main(int argc , char *argv[])
{
  int mode  = 0;
  int maxfd;
  int ret;
  fd_set rset;
  user_manager<chat_user> * supervisor;
  std::vector<chat_user *> usersV;
  std::vector<chat_user *> lazyV;
  std::vector<std::string> exitV;
  chat_session *sessionV;

  if((argc != 4) || ((strcmp(argv[1],"v4") != 0) && (strcmp(argv[1],"v6")) != 0) )
  {
      std::cout << "[-] usage: " << argv[0] << " <mode> <address> <port>" << "\n";
      _exit (1);
  }

    mode = (strcmp(argv[1],"v4") == 0) ? AF_INET : AF_INET6;
    ip_address server_addr(argv[2] , argv[3] , mode , SOCK_STREAM , AI_PASSIVE );
    tcp_socket chatTServ(mode);

    chatTServ.bind(server_addr);
    chatTServ.listen();

    supervisor = new user_manager<chat_user>(usersV);
    sessionV = new chat_session();

    std::cout << "[+] Welcome to chat Server!" << "\n";
    std::cout << "[+] Waiting for connections.." << "\n\n";

    loop_always()
    {
      FD_ZERO(&rset);

      FD_SET(chatTServ.get_id(),&rset);

      sessionV->do_clean();

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
        auto it = std::max_element(usersV.begin(),usersV.end(),base_socket::sock_compare<chat_user>);
        maxfd = (*it)->get_id();
      }
      else
      {
        maxfd = chatTServ.get_id();
      }

      for(auto it = usersV.begin(); it != usersV.end(); ++it )
          FD_SET((*it)->get_id(),&rset);

      ret = select(maxfd+1,&rset,NULL,NULL,NULL);
      if(ret == -1)
      {
        std::cout << "chatServer::select(): " << std::strerror(errno) << "\n";
        _exit (1);
      }

      if(FD_ISSET(chatTServ.get_id(),&rset)) /* qualcosa sul listener*/
      {
        chat_user* newUser = new chat_user(mode);
        chatTServ.accept(newUser);
        std::stringstream msg;
        std::string nickname;

        try
        {
          nickname = newUser->hail();
          supervisor->check_nick(nickname);
          CLEAR_MSG(msg);
          msg << "OLEH" << " " << "WELCOME" << "\n";
          newUser->send_message(msg.str());
          newUser->set_nick(nickname);
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
                else if(strcmp( message[0].c_str(),"HANG") == 0 ) /*HANG|user_2 */
                {
                  std::string session;
                  supervisor->check_user(message[1]);
                  (*it)->set_status(true);
                  session = sessionV->new_session((*it),supervisor->get_user(message[1]));
                  (*it)->set_session(session);
                  std::cout << "[+] Started new session " << session << "\n";

                }
                else if(strcmp( message[0].c_str(),"MSG") == 0 ) /* MSG|id|message */
                {
                  std::string _msg;
                  for (auto it = message.begin()+2; it != message.end(); ++it)
                  {
                      _msg += *it;
                      _msg += " ";
                  }
                  sessionV->dispatch_msg((*it),message[1],_msg);
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
                   sessionV->close_full_session((*it),message[1]);
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
                      sessionV->close_half_session((*it),(*it)->get_session());
                    }
                    lazyV.push_back(*it);
                    continue;
                  case user_status_t::NONEX:
                    CLEAR_MSG(msg);
                    msg << "ERR" << " " << "NONEX" << "\n";
                    (*it)->send_message(msg.str());
                    break;
                  case user_status_t::BUSY:
                    CLEAR_MSG(msg);
                    msg << "ERR" << " " << "BUSY" << "\n";
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
