#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/tcp_client.h"
#include "../../include/math_box.h"
#include "../../include/enum_classes.h"

int main(int argc , char *argv[])
{

  int mode  = 0;
  std::regex polish_form("^[-]?\\d+\\s{1}[-]?\\d+\\s{1}[-+*\\/\\%]?$");
  tcp_client *connTSock = NULL;
  std::stringstream msg;

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
    std::cout << "[+] Welcome to calcServer!" << "\n";
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
          std::vector<std::string> message;
          message = connTSock->receive_message();

          CLEAR_MSG(msg);
          if(strcmp(message[0].c_str(),"CALC") == 0)
          {
            std::stringstream p_stream;
            std::string p_str;
            if(message.size() != 4)
            {
              std::cout << "[-] Not in polish notation " << connTSock->get_ip() << ": " << connTSock->get_port() << "\n";
              msg << "ERR" << " " << "NAP" << " " << "\n";
              connTSock->send_message(msg.str());
              continue;
            }
            p_stream << message[1] << " " << message[2] << " " << message[3];
            p_str = p_stream.str();
            if(!regex_match(p_str.c_str(),polish_form))
            {
              std::cout << "[-] Not in polish notation " << connTSock->get_ip() << ": " << connTSock->get_port() << "\n";
              msg << "ERR" << " " << "NAP" << " " << "\n";
              connTSock->send_message(msg.str());
              continue;
            }
            else
            {
              int res = math_box::calc_expr(p_str.c_str());
              msg << "ACK" << " " << "CALC" << " " << res << "\n";
              connTSock->send_message(msg.str());
            }
          }

          else if(strcmp(message[0].c_str(),"CLOSE") == 0)
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
