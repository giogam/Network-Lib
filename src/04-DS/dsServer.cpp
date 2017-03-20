#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/tcp_client.h"
#include "../../include/phone_db.h"
#include "../../include/enum_classes.h"

int main(int argc , char *argv[])
{
  int mode  = 0;
  std::vector<std::string> result;
  std::vector<std::string> message;
  std::stringstream msg;
  tcp_client *connTSock = NULL;

  if((argc != 4) || ((strcmp(argv[1],"v4") != 0) && (strcmp(argv[1],"v6")) != 0) )
  {
      std::cout << "[-] usage: " << argv[0] << " <mode> <address> <port>" << "\n";
      _exit (1);
  }

    mode = (strcmp(argv[1],"v4") == 0) ? AF_INET : AF_INET6;
    ip_address server_addr(argv[2] , argv[3] , mode , SOCK_STREAM , AI_PASSIVE );
    tcp_socket counTServ(mode);

    counTServ.bind(server_addr);
    counTServ.listen();

    phone_db phDbase("phone_db.pdb");

    std::cout << "[+] Welcome to directoryServer!" << "\n";
    std::cout << "[+] Waiting for connections.." << "\n";


    loop_always()
    {
      try
      {
	      connTSock = new tcp_client(mode);
	      counTServ.accept(connTSock);
	      std::cout << "[+] Incoming connection from:" << connTSock->get_ip() << ": " << connTSock->get_port() << "\n";

	loop_always()
  {

	  message = connTSock->receive_message();

	  CLEAR_MSG(msg);
	  if(strcmp(message[0].c_str(),"LOOKUP") == 0)
	  {
	    std::cout << "Client is looking for : " << message[1] << "\n";
	    result = phDbase.search(message[1]);
	    int _entries = result.empty() ? 0 : result.size();

	    msg << "FOUND" << " " << _entries;
	    if(_entries > 0 )
	    {
	      std::stringstream _ent;
	      for(auto _it : result) _ent << _it << " ";
	      msg << " " << _ent.str();
	    }
	    connTSock->send_message(msg.str());
	  }
	  else if (strcmp(message[0].c_str(),"CLOSE") == 0)
	  {
	    msg << "ACK" << " " << "CLOSE" << "\n";
	    connTSock->send_message(msg.str());
	    delete connTSock;
	    break;
	  }
	}
      }
	     catch(generic_exception & e)
      {
        switch(e.get_errcode())
        {
          case socket_status_t::BAD_READ:
            std::cout << "[-] Client Shutdown " << "\n";
            delete connTSock;
            continue;
        }
      }
      }

      return 0;
}
