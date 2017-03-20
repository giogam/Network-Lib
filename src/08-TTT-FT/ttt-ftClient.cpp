#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/udp_agent.h"
#include "../../include/ttt_player.h"
#include "../../include/enum_classes.h"

int main(int argc , char *argv[])
{
  std::string input;
  std::stringstream msg;
  fd_set rset;
  int maxfd;
  int mode  = 0;

    if((argc != 5) || ((strcmp(argv[1],"v4") != 0) && (strcmp(argv[1],"v6") != 0)) )
    {
      std::cout << "[-] usage: " << argv[0] << " <mode> <connect_address> <port> <control_address>" << "\n";
      _exit (1);
    }

    mode = (strcmp(argv[1],"v4") == 0) ? AF_INET : AF_INET6;
    ip_address server_addr(argv[2] , argv[3] , mode , SOCK_STREAM );
    ttt_player * tttTCli = new ttt_player(mode);

    ip_address ctrl_addr(argv[4] , base_socket::random_port() , mode , SOCK_DGRAM );
    udp_agent ctrlUSock(mode);
    ctrlUSock.bind(ctrl_addr);
    std::cout << "Control Port: " << std::to_string(ctrlUSock.get_port()) << "\n";

    tttTCli->connect(server_addr);
    std::cout << "[+] Welcome to chat Client!" << "\n\n";

    maxfd = tttTCli->get_id();

    std::cout << "Select Nickname: ";
    input.clear();
    std::getline(std::cin , input);

    CLEAR_MSG(msg);
    msg << "HELO" << " " << input << " " << std::to_string(ctrlUSock.get_port()) <<"\n"; /* HELO|nickname|ft_mode */

    tttTCli->send_message(msg.str());
    std::vector<std::string> message;
    message = tttTCli->receive_message();

    if(strcmp(message[0].c_str() , "OLEH") == 0)
    {
      std::cout << "[+] Connection estabilished username: " << input << "\n";
    }
    else if(strcmp(message[0].c_str() , "ERR") == 0)
    {
      if(strcmp(message[1].c_str() , "BAD_USER") == 0)
      {
        std::cout << "[+] Username already taken" << "\n";
        _exit (1);

      }
    }
      tttTCli->set_nick(input);
      tttTCli->set_status(false);


    loop_always()
    {
      FD_ZERO(&rset);
      FD_SET(fileno(stdin),&rset);
      FD_SET(tttTCli->get_id(),&rset);

      select(maxfd+1,&rset,NULL,NULL,NULL);

      if(FD_ISSET( fileno(stdin) , &rset )) /* something from the stdin */
      {
	       input.clear();
	       std::getline(std::cin , input);
	    if(tttTCli->is_busy())
	    {
	       if(strcmp(input.c_str(),"exit") == 0) /* EXIT|id*/
	        {
	           CLEAR_MSG(msg);
	           msg << "EXIT" << " " << tttTCli->get_session();
	           tttTCli->send_message(msg.str());
	        }
	    else /* MOV|id|row|col */
	    {
        if(!input.empty())
	      {
          CLEAR_MSG(msg);
          msg << "MOV" << " " << tttTCli->get_session() << " " << input;
          tttTCli->send_message(msg.str());
        }
      }
	  }
	  else
	  {

	    if(strcmp(input.c_str(),"list") == 0) /* LIST */
	    {
	      CLEAR_MSG(msg);
	      msg << "LIST" ;
	      tttTCli->send_message(msg.str());
	    }
	    else if(strcmp(input.c_str(),"play") == 0) /*PLAY|user_2 */
	    {
	      CLEAR_MSG(msg);
	      std::cout << "Which nickname?: ";
	      input.clear();
	      std::getline(std::cin , input);
	      msg << "PLAY" << " "  << input;
	      tttTCli->send_message(msg.str());
	    }
	    else if(strcmp(input.c_str(),"logout") == 0) /* LOGOUT */
	    {
	      CLEAR_MSG(msg);
	      msg << "LOGOUT" ;
	      tttTCli->send_message(msg.str());
	    }
      else if(strcmp(input.c_str(),"resume") == 0) /*RESUME|id|player_num */
	    {
	      std::cout << "Player Number "<< tttTCli->get_number() << "\n";
	      CLEAR_MSG(msg);
	      msg << "RESUME" << " " << tttTCli->get_session() << " " << tttTCli->get_number();
	      tttTCli->send_message(msg.str());
	    }
	  }
	}
      else if(FD_ISSET( tttTCli->get_id() , &rset )) /* something from the server */
      {
	try{


        std::vector<std::string> message;
        message = tttTCli->receive_message();

	if(strcmp(message[0].c_str(),"LIST") == 0) /* LIST|user_list*/
  {
    std::string _list;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _list += *it;
      _list += "\n";
    }
    std::cout << _list << "\n";
	}
	else if(strcmp(message[0].c_str(),"START") == 0) /* START|id|player_num|user_play|board_status*/
	{
	  std::string _board;
	  int _cnt = 0;
	  std::cout << "Match ID: " << message[1] << " " <<  "Joined" << "\n";

	  tttTCli->set_status(true);
	  tttTCli->set_session(message[1]);
	  tttTCli->set_number(atoi(message[2].c_str()));
	  std::cout << "Player N: " << tttTCli->get_number() << "\n";
	  tttTCli->set_peer(message[3]);

    for (auto it = message.begin()+4; it != message.end(); ++it)
    {
      _board += *it;
      if(_cnt == 2)
      {
        _board += "\n";
        _cnt = 0;
      }
      else
      {
        _board += " ";
        _cnt++;
      }
    }
    std::cout << _board << "\n";
	}
	else if(strcmp(message[0].c_str(),"RESTART") == 0) /* RESTART|id|player_num|user_play|turn|board_status*/
	{
	  std::string _board;
    int _cnt = 0;
	  std::cout << "Match ID: " << tttTCli->get_session() << " " <<  "Resumed" << "\n";
	  std::cout << "New Match ID: " << message[1] << "\n";
	  std::cout << "Player N " << message[2] << "\n";
	  std::cout << "Turn N " << message[4] << "\n";

	  tttTCli->set_status(true);
	  tttTCli->set_session(message[1]);
	  tttTCli->set_number(std::stoi(message[2]));
	  tttTCli->set_peer(message[3]);

	  (strcmp(message[4].c_str(),message[2].c_str()) == 0) ? std::cout << "It's Your Turn" << "\n" : std::cout << "It's Not Your Turn" << "\n";


    for (auto it = message.begin()+5; it != message.end(); ++it)
    {
      _board += *it;
      if(_cnt == 2)
      {
        _board += "\n";
        _cnt = 0;
      }
      else
      {
        _board += " ";
        _cnt++;
      }
    }
    std::cout << _board << "\n";
	}
	else if(strcmp(message[0].c_str(),"DRAW") == 0) /* DRAW|board_status*/
	{
    std::string _board;
    int _cnt = 0;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _board += *it;
      if(_cnt == 2)
      {
        _board += "\n";
        _cnt = 0;
      }
      else
      {
        _board += " ";
        _cnt++;
      }
    }
    std::cout << _board << "\n";
	}
  else if(strcmp(message[0].c_str(),"WIN") == 0) /* WIN|board_status*/
	{
    std::cout << "You Won!!!" << "\n";
    std::string _board;
    int _cnt = 0;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _board += *it;
      if(_cnt == 2)
      {
        _board += "\n";
        _cnt = 0;
      }
      else
      {
        _board += " ";
        _cnt++;
      }
    }
    std::cout << _board << "\n";
	}
  else if(strcmp(message[0].c_str(),"LOOSE") == 0) /* LOOSE|board_status*/
	{
    std::cout << "You Loose!!!" << "\n";
    std::string _board;
    int _cnt = 0;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _board += *it;
      if(_cnt == 2)
      {
        _board += "\n";
        _cnt = 0;
      }
      else
      {
        _board += " ";
        _cnt++;
      }
    }
    std::cout << _board << "\n";
	}
  else if(strcmp(message[0].c_str(),"ODD") == 0) /* NO_WIN|board_status*/
	{
    std::cout << "There's No Winner!!!" << "\n";
    std::string _board;
    int _cnt = 0;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _board += *it;
      if(_cnt == 2)
      {
        _board += "\n";
        _cnt = 0;
      }
      else
      {
        _board += " ";
        _cnt++;
      }
    }
    std::cout << _board << "\n";
	}
	else if(strcmp(message[0].c_str(),"ACK") == 0) /* ACK|cause|var_payload*/
	{
    if(strcmp(message[1].c_str(),"LOGOUT")== 0) /* ACK|LOGOUT */
    {
      std::cout << "Logging Out Byeee" << "\n";
	    break;
    }
    else if(strcmp(message[1].c_str(),"EXIT") == 0) /* ACK|EXIT|id*/
  	{
  	  std::cout << "Match ID: " << message[2] << " " << "Exited" << "\n";
  	  tttTCli->set_status(false);
  	}
  }
  else if(strcmp(message[0].c_str(),"ERR") == 0) /* ERR|cause*/
  {
    if(strcmp(message[1].c_str(),"BUSY")== 0) /* ERR|BUSY */
    {
      std::cout << "User is in another chat" << "\n";
    }
    else if(strcmp(message[1].c_str(),"MOVE") == 0) /* ERR|BAD_USER*/
  	{
  	  std::cout << "Bad Move" << "\n";
  	}
    else if(strcmp(message[1].c_str(),"TURN") == 0) /* ERR|TURN*/
    {
      std::cout << "Bad Turn" << "\n";
    }
    else if(strcmp(message[1].c_str(),"NONEX") == 0) /* ERR|NONEX*/
    {
      std::cout << "Inexistent User" << "\n";
    }
  }

}catch(generic_exception & e)
  {
      switch(e.get_errcode())
          {
            case socket_status_t::BAD_READ:

              ip_address serv_ctrl;
	      std::string old_session;
              std::string old_nick;
	      int old_number = 0;

	       if(tttTCli->is_busy())
	       {
		old_session = tttTCli->get_session();
		old_number = tttTCli->get_number();
                std::cout << old_number << "\n";
                tttTCli->set_status(false);
	       }
              old_nick = tttTCli->get_nick();
              std::cout << old_nick << "\n";
              std::vector<std::string> message;
              std::cout << "[-] Server Shutdown " << "\n";
              std::cout << "[-] Waiting For Istructions: " << "\n";
              message = ctrlUSock.receive_message(serv_ctrl);
              mode = (strcmp(message[2].c_str(),"v4") == 0) ? AF_INET : AF_INET6;
              ip_address new_addr(message[0].c_str(),message[1].c_str(),mode,SOCK_STREAM);
              server_addr.reset_address(new_addr.get_address());
              delete tttTCli;
              tttTCli = new ttt_player(mode);
              tttTCli->connect(new_addr);
	      tttTCli->set_session(old_session);
              tttTCli->set_number(old_number);
              tttTCli->set_nick(old_nick);
              CLEAR_MSG(msg);
              msg << "HELO" << " " << tttTCli->get_nick() << " " << std::to_string(ctrlUSock.get_port()) << "\n";
              tttTCli->send_message(msg.str());
              message.clear();
              message = tttTCli->receive_message();
              std::cout << "[+] Connection Up" << "\n";
              break;
          }
  }
      }
  }
      return 0;
}
