#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/tcp_client.h"
#include "../../include/enum_classes.h"

int main(int argc , char *argv[])
{
  int maxInt = 0;
  int recvNum = 0;
  int mode  = 0;
  std::regex numeric("^[0-9]+$");
  std::stringstream msg;

  if((argc != 4) || ((strcmp(argv[1],"v4") != 0) && (strcmp(argv[1],"v6")) != 0) )
  {
      std::cout << "[-] usage: " << argv[0] << " <mode> <address> <port>" << "\n";
      _exit (1);
  }

    mode = (strcmp(argv[1],"v4") == 0) ? AF_INET : AF_INET6;
    ip_address server_addr(argv[2] , argv[3] , mode , SOCK_STREAM , AI_PASSIVE );
    tcp_socket maxTServ(mode);
    tcp_client *connTSock = NULL;

    maxTServ.bind(server_addr);
    maxTServ.listen();

    std::cout << "[+] Welcome to maxServer!" << "\n";
    std::cout << "[+] Current maxInt: " << maxInt << "\n";
    std::cout << "[+] Waiting for connections.." << "\n\n";

      loop_always()
      {
        try
        {

        connTSock = new tcp_client(mode);
        maxTServ.accept(connTSock);
        std::cout << "[+] Incoming connection from:" << connTSock->get_ip() << ": " << connTSock->get_port() << "\n";

        loop_always()
        {
          std::vector<std::string> message;
          message = connTSock->receive_message();

          if(strcmp(message[0].c_str(),"CLOSE") == 0) /* CLOSE */
          {
            std::cout << "[-] Closing connection. Bye Bye! " << connTSock->get_ip() << ": " << connTSock->get_port() << "\n";
            CLEAR_MSG(msg);
            msg << "ACK" << " " << "CLOSE" << "\n";
            connTSock->send_message(msg.str());
            delete connTSock;
            break;
          }
          else if((strcmp(message[0].c_str(),"NUM") == 0)) /* NUM|number*/
          {
            CLEAR_MSG(msg);

            if(!regex_match(message[1].c_str(),numeric))
            {
              std::cout << "[-] Wrong message format " << connTSock->get_ip() << ": " << connTSock->get_port() << "\n";
              msg << "ERR" << " " << "NAN" << " " << "\n";
              connTSock->send_message(msg.str());
              continue;
            }
            else
            {
              std::cout << "[+] Client sends: "  << message[1] << "\n";
              recvNum = atoi(message[1].c_str());
              if(recvNum > maxInt ) maxInt = recvNum;
              msg << "ACK" << " " << "MAX" << " " << maxInt << "\n";
              connTSock->send_message(msg.str());
            }
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
