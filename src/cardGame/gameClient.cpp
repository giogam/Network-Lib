#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/udp_agent.h"
#include "../../include/game_player.h"
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
    game_player * gameTCli = new game_player(mode);

    ip_address ctrl_addr(argv[4] , base_socket::random_port() , mode , SOCK_DGRAM );
    udp_agent ctrlUSock(mode);
    ctrlUSock.bind(ctrl_addr);
    std::cout << "Control Port: " << std::to_string(ctrlUSock.get_port()) << "\n";

    gameTCli->connect(server_addr);
    std::cout << "[+] Welcome to Welcome to Card Game Client!" << "\n";


    maxfd = gameTCli->get_id();

    std::cout << "Select Nickname: ";
    input.clear();
    std::getline(std::cin , input);

    CLEAR_MSG(msg);
    msg << "HELO" << " " << input << " " << std::to_string(ctrlUSock.get_port()) <<"\n"; /* HELO|nickname|ft_mode */

    gameTCli->send_message(msg.str());
    std::vector<std::string> message;
    message = gameTCli->receive_message();

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
    gameTCli->set_nick(input);
    gameTCli->set_status(false);

    loop_always()
    {
      FD_ZERO(&rset);
      FD_SET(fileno(stdin),&rset);
      FD_SET(gameTCli->get_id(),&rset);

      select(maxfd+1,&rset,NULL,NULL,NULL);

      if(FD_ISSET( fileno(stdin) , &rset )) /* something from the stdin */
      {
	       input.clear();
	       std::getline(std::cin , input);
	    if(gameTCli->is_busy())
	    {
	       if(strcmp(input.c_str(),"exit") == 0) /* EXIT|id*/
	        {
	           CLEAR_MSG(msg);
	           msg << "EXIT" << " " << gameTCli->get_session();
	           gameTCli->send_message(msg.str());
	        }
	    else /* MOV|id|row|col */
	    {
        if(!input.empty())
	      {
          CLEAR_MSG(msg);
          msg << "MOV" << " " << gameTCli->get_session() << " " << input;
          gameTCli->send_message(msg.str());
        }
      }
	  }
	  else
	  {

	    if(strcmp(input.c_str(),"list") == 0) /* LIST */
	    {
	      CLEAR_MSG(msg);
	      msg << "LIST" ;
	      gameTCli->send_message(msg.str());
	    }
	     else if(strcmp(input.c_str(),"games") == 0) /*GAMES*/
	    {
	      CLEAR_MSG(msg);
	      msg << "GAMES" << "\n" ;
	      gameTCli->send_message(msg.str());
	    }
	    else if(strcmp(input.c_str(),"chart") == 0) /*CHART*/
	    {
	      CLEAR_MSG(msg);
	      msg << "CHART" << "\n" ;
	      gameTCli->send_message(msg.str());
	    }
	    else if(strcmp(input.c_str(),"play") == 0) /*PLAY|user_2|game */
	    {
	      CLEAR_MSG(msg);
	      std::cout << "Which nickname?: ";
	      input.clear();
	      std::getline(std::cin , input);
	      msg << "PLAY" << " "  << input;
	       std::cout << "Which game?: ";
	      input.clear();
	      std::getline(std::cin , input);
	      msg << " " << input;
	      gameTCli->send_message(msg.str());
	    }
	    else if(strcmp(input.c_str(),"logout") == 0) /* LOGOUT */
	    {
	      CLEAR_MSG(msg);
	      msg << "LOGOUT" ;
	      gameTCli->send_message(msg.str());
	    }
      else if(strcmp(input.c_str(),"resume") == 0) /*RESUME|id|game_type */
	    {
	      CLEAR_MSG(msg);
	      msg << "RESUME" << " " << gameTCli->get_session() << " " << gameTCli->get_game();
        gameTCli->send_message(msg.str());
	    }
	  }
	}
      else if(FD_ISSET( gameTCli->get_id() , &rset )) /* something from the server */
      {
	try{


        std::vector<std::string> message;
        message = gameTCli->receive_message();

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
	else if(strcmp(message[0].c_str(),"GAMES") == 0) /* GAMES|list*/
	{
	  std::string _games;
	  for (auto it = message.begin()+1; it != message.end(); ++it)
	  {
	      _games += *it;
	      _games += "\n";
	  }
	     std::cout << _games << "\n";
	}
	else if(strcmp(message[0].c_str(),"START") == 0) /* START|id|player_num|user_play|game_type|board_status*/
	{
	  std::string _board;
	  std::cout << "Match ID: " << message[1] << " " <<  "Joined" << "\n";
	  gameTCli->set_status(true);
	  gameTCli->set_session(message[1]);
	  gameTCli->set_number(std::stoi(message[2]));
	  gameTCli->set_peer(message[3]);
	  gameTCli->set_game(message[4]);

    for (auto it = message.begin()+5; it != message.end(); ++it)
    {
      _board += *it;
      _board += " ";
    }
    std::cout << _board << "\n";
	}
	else if(strcmp(message[0].c_str(),"RESTART") == 0) /* RESTART|id|player_num|user_play|turn|game_type|board_status*/
	{
	  std::string _board;
	  std::cout << "Match ID: " << gameTCli->get_session() << " " <<  "Resumed" << "\n";
	  std::cout << "New Match ID: " << message[1] << "\n";

	  gameTCli->set_status(true);
	  gameTCli->set_session(message[1]);
	  gameTCli->set_number(std::stoi(message[2]));
	  gameTCli->set_peer(message[3]);
	  gameTCli->set_game(message[5]);


	  (strcmp(message[4].c_str(),message[2].c_str()) == 0) ? std::cout << "It's Your Turn" << "\n" : std::cout << "It's Not Your Turn" << "\n";


    for (auto it = message.begin()+5; it != message.end(); ++it)
    {
      _board += *it;
      _board += " ";
    }
    std::cout << _board << "\n";
	}
	else if(strcmp(message[0].c_str(),"CHART") == 0) /* CHART|chart*/
	{
    std::string _chart;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _chart += *it;
      _chart += "\n";
    }
    std::cout << _chart << "\n";
	}
	else if(strcmp(message[0].c_str(),"DRAW") == 0) /* DRAW|board_status*/
	{
    std::string _board;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _board += *it;
      _board += " ";
    }
    std::cout << _board << "\n";
	}
  else if(strcmp(message[0].c_str(),"WIN") == 0) /* WIN|board_status*/
	{
    std::cout << "You Won!!!" << "\n";
    std::string _board;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _board += *it;
      _board += " ";
    }
    std::cout << _board << "\n";
	}
  else if(strcmp(message[0].c_str(),"LOOSE") == 0) /* LOOSE|board_status*/
	{
    std::cout << "You Loose!!!" << "\n";
    std::string _board;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _board += *it;
      _board += " ";
    }
    std::cout << _board << "\n";
	}
	else if(strcmp(message[0].c_str(),"ODD") == 0) /* ODD|board_status*/
	{
    std::cout << "ODD!!!" << "\n";
    std::string _board;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _board += *it;
      _board += " ";
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
  	  gameTCli->set_status(false);
  	}
  }
  else if(strcmp(message[0].c_str(),"ERR") == 0) /* ERR|cause*/
  {
    if(strcmp(message[1].c_str(),"BUSY")== 0) /* ERR|BUSY */
    {
      std::cout << "User is in another chat" << "\n";
    }
    else if(strcmp(message[1].c_str(),"NONEX") == 0) /* ERR|NONEX*/
  	{
  	  std::cout << "Inexistent User" << "\n";
  	}
    else if(strcmp(message[1].c_str(),"NO_GAME") == 0) /* ERR|NO_GAME*/
    {
      std::cout << "Inexistent Game " << "\n";
    }
    else if(strcmp(message[1].c_str(),"TURN") == 0) /* ERR|TURN*/
    {
      std::cout << "Bad Turn" << "\n";
    }
     else if(strcmp(message[1].c_str(),"MOV") == 0) /* ERR|MOV*/
    {
      std::cout << "Bad Move" << "\n";
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
              std::string old_game;

	      int old_number = 0;

	            if(gameTCli->is_busy())
	             {
		            old_session = gameTCli->get_session();
		            old_number = gameTCli->get_number();
                old_game = gameTCli->get_game();

                std::cout << old_number << "\n";
                gameTCli->set_status(false);
	            }
              old_nick = gameTCli->get_nick();
              std::cout << old_nick << "\n";
              std::vector<std::string> message;
              std::cout << "[-] Server Shutdown " << "\n";
              std::cout << "[-] Waiting For Istructions: " << "\n";
              message = ctrlUSock.receive_message(serv_ctrl);
              mode = (strcmp(message[2].c_str(),"v4") == 0) ? AF_INET : AF_INET6;
              ip_address new_addr(message[0].c_str(),message[1].c_str(),mode,SOCK_STREAM);
              server_addr.reset_address(new_addr.get_address());
              delete gameTCli;
              gameTCli = new game_player(mode);
              gameTCli->connect(new_addr);
	      gameTCli->set_session(old_session);
              gameTCli->set_number(old_number);
              gameTCli->set_nick(old_nick);
              gameTCli->set_game(old_game);
              CLEAR_MSG(msg);
              msg << "HELO" << " " << gameTCli->get_nick() << " " << std::to_string(ctrlUSock.get_port()) << "\n";
              gameTCli->send_message(msg.str());
              message.clear();
              message = gameTCli->receive_message();
              std::cout << "[+] Connection Up " << "\n";
              break;
          }
  }
      }
  }
      return 0;
}
