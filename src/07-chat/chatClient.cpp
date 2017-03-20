#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/chat_user.h"
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
    chat_user chatTCli(mode);

    chatTCli.connect(server_addr);
    std::cout << "[+] Welcome to chat Client!" << "\n\n";

    maxfd = chatTCli.get_id();

    std::cout << "Select Nickname: ";
    input.clear();
    std::getline(std::cin , input);
    CLEAR_MSG(msg);
    msg << "HELO" << " " << input << "\n"; /* HELO|nickname */
    chatTCli.send_message(msg.str());
    std::vector<std::string> message;
    message = chatTCli.receive_message();

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
    chatTCli.set_nick(input);
    chatTCli.set_status(false);

    loop_always()
    {
      FD_ZERO(&rset);
      FD_SET(fileno(stdin),&rset);
      FD_SET(chatTCli.get_id(),&rset);

      select(maxfd+1,&rset,NULL,NULL,NULL);

      if(FD_ISSET( fileno(stdin) , &rset )) /* something from the stdin */
      {
	    input.clear();
	    std::getline(std::cin , input);
	    if(chatTCli.is_busy())
	    {
	    if(strcmp(input.c_str(),"exit") == 0) /* EXIT|id*/
	    {
	      CLEAR_MSG(msg);
	      msg << "EXIT" << " " << chatTCli.get_session();
	      chatTCli.send_message(msg.str());
	    }
	    else /* MSG|id|message */
	    {
        if(!input.empty())
	      {
          CLEAR_MSG(msg);
          msg << "MSG" << " " << chatTCli.get_session() << " " << input;
          chatTCli.send_message(msg.str());
        }
      }
	  }
	  else
	  {

	    if(strcmp(input.c_str(),"list") == 0) /* LIST */
	    {
	      CLEAR_MSG(msg);
	      msg << "LIST" ;
	      chatTCli.send_message(msg.str());
	    }
	    else if(strcmp(input.c_str(),"hang") == 0) /*HANG|user_2 */
	    {
	      CLEAR_MSG(msg);
	      std::cout << "Which nickname?: ";
	      input.clear();
	      std::getline(std::cin , input);
	      msg << "HANG" << " "  << input;
	      chatTCli.send_message(msg.str());
	    }
	    else if(strcmp(input.c_str(),"logout") == 0) /* LOGOUT */
	    {
	      CLEAR_MSG(msg);
	      msg << "LOGOUT" ;
	      chatTCli.send_message(msg.str());
	    }
	  }
	}
      else if(FD_ISSET( chatTCli.get_id() , &rset )) /* something from the server */
      {
        try
        {
        std::vector<std::string> message;
        message = chatTCli.receive_message();

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
	else if(strcmp(message[0].c_str(),"START") == 0) /* START|id|user_hang*/
	{
	  std::cout << "Chat ID: " << message[1] << " " <<  "Joined" << "\n";
	  chatTCli.set_status(true);
	  chatTCli.set_session(message[1]);
	  chatTCli.set_peer(message[2]);
	}
	else if(strcmp(message[0].c_str(),"MSG") == 0) /* MSG|message*/
	{
    std::string _msg;
    for (auto it = message.begin()+1; it != message.end(); ++it)
    {
      _msg += *it;
      _msg += " ";
    }
    std::cout << "[" << chatTCli.get_peer() << "] " << _msg << "\n";
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
  	  std::cout << "Chat ID: " << message[2] << " " << "Exited" << "\n";
  	  chatTCli.set_status(false);
  	}
  }
  else if(strcmp(message[0].c_str(),"ERR") == 0) /* ERR|cause*/
  {
    if(strcmp(message[1].c_str(),"BUSY")== 0) /* ERR|BUSY */
    {
      std::cout << "User is in another chat" << "\n";
    }
    else if(strcmp(message[1].c_str(),"NONEX") == 0) /* ERR|BAD_USER*/
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
