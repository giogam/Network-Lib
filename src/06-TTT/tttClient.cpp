#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/ttt_player.h"
#include "../../include/enum_classes.h"

int main(int argc , char *argv[])
{
  std::string input;
  std::stringstream msg;
  fd_set rset;
  int maxfd;
  int mode  = 0;

    if((argc != 4) || ((strcmp(argv[1],"v4") != 0) && (strcmp(argv[1],"v6") != 0)) )
    {
      std::cout << "[-] usage: " << argv[0] << " <mode> <address> <port>" << "\n";
      _exit (1);
    }

    mode = (strcmp(argv[1],"v4") == 0) ? AF_INET : AF_INET6;
    ip_address server_addr(argv[2] , argv[3] , mode , SOCK_STREAM );
    ttt_player tttTCli(mode);

    tttTCli.connect(server_addr);
    std::cout << "[+] Welcome to TTT Client!" << "\n\n";

    maxfd = tttTCli.get_id();

    std::cout << "Select Nickname: ";
    input.clear();
    std::getline(std::cin , input);

    CLEAR_MSG(msg);
    msg << "HELO" << " " << input << " " << "NOFT" << "\n"; /* HELO|nickname|ft_mode */

    tttTCli.send_message(msg.str());
    std::vector<std::string> message;
    message = tttTCli.receive_message();

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

    tttTCli.set_nick(input);
    tttTCli.set_status(false);

      loop_always()
      {
      FD_ZERO(&rset);
      FD_SET(fileno(stdin),&rset);
      FD_SET(tttTCli.get_id(),&rset);

      select(maxfd+1,&rset,NULL,NULL,NULL);

      if(FD_ISSET( fileno(stdin) , &rset )) /* something from the stdin */
      {
	    input.clear();

      std::getline(std::cin , input);

      if(tttTCli.is_busy())
	    {
	       if(strcmp(input.c_str(),"exit") == 0) /* EXIT|id*/
	        {
	          CLEAR_MSG(msg);
	          msg << "EXIT" << " " << tttTCli.get_session();
	          tttTCli.send_message(msg.str());
	        }
	        else /* MOV|id|row|col */
	        {
            if(!input.empty())
	          {
              CLEAR_MSG(msg);
              msg << "MOV" << " " << tttTCli.get_session() << " " << input;
              tttTCli.send_message(msg.str());
            }
          }
	    }
	    else
	    {

	    if(strcmp(input.c_str(),"list") == 0) /* LIST */
	    {
	      CLEAR_MSG(msg);
	      msg << "LIST" ;
	      tttTCli.send_message(msg.str());
	    }
	    else if(strcmp(input.c_str(),"play") == 0) /*PLAY|user_2 */
	    {
	      CLEAR_MSG(msg);
	      std::cout << "Which nickname?: ";
	      input.clear();
	      std::getline(std::cin , input);
	      msg << "PLAY" << " "  << input;
	      tttTCli.send_message(msg.str());
	    }
	    else if(strcmp(input.c_str(),"logout") == 0) /* LOGOUT */
	    {
	      CLEAR_MSG(msg);
	      msg << "LOGOUT" ;
	      tttTCli.send_message(msg.str());
	    }
	  }
	}
      else if(FD_ISSET( tttTCli.get_id() , &rset )) /* something from the server */
      {
      try{
        std::vector<std::string> message;
        message = tttTCli.receive_message();

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
	  tttTCli.set_status(true);
	  tttTCli.set_session(message[1]);
    tttTCli.set_number(std::stoi(message[2]));
	  tttTCli.set_peer(message[3]);

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
  	  tttTCli.set_status(false);
  	}
  }
  else if(strcmp(message[0].c_str(),"ERR") == 0) /* ERR|cause*/
  {
    if(strcmp(message[1].c_str(),"BUSY")== 0) /* ERR|BUSY */
    {
      std::cout << "User is in another match" << "\n";
    }
    else if(strcmp(message[1].c_str(),"MOVE") == 0) /* ERR|MOVE*/
    {
      std::cout << "Bad Move" << "\n";
    }
    else if(strcmp(message[1].c_str(),"TURN") == 0) /* ERR|TURN*/
    {
      std::cout << "Bad Turn" << "\n";
    }
    else if(strcmp(message[1].c_str(),"NONEX") == 0) /* ERR|NOEX*/
    {
      std::cout << "Inexistent User" << "\n";
    }
  }
}catch(generic_exception & e)
    {
        switch(e.get_errcode())
        {
          case socket_status_t::BAD_READ:
          std::cout << "[-] Server Shutdown " << "\n";
          _exit (1);
        }
    }
}
  }
      return 0;
}
