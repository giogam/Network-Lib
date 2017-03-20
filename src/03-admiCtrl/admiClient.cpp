#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/udp_agent.h"

int main(int argc , char *argv[])
{
  std::string input;
  int mode  = 0;
  std::stringstream msg;
  std::vector<std::string> message;

  if((argc != 4) || ((strcmp(argv[1],"v4") != 0) && (strcmp(argv[1],"v6") != 0)) )
  {
      std::cout << "[-] usage: " << argv[0] << " <mode> <address> <port>" << "\n";
      _exit (1);
  }

    mode = (strcmp(argv[1],"v4") == 0) ? AF_INET : AF_INET6;
    ip_address server_addr(argv[2] , argv[3] , mode , SOCK_DGRAM );
    udp_agent admUCli(mode);

    std::cout << "[+] Welcome to admiClient!" << "\n";

      loop_always()
      {
        CLEAR_MSG(msg);

        std::cout << "> ";
        std::getline(std::cin , input);

        if(std::cin.eof())
        {
          std::cout << "CTRL-D" << "\n";
          break;
        }

        if(input.empty()) continue;

        msg << "ECHO" << " " << input << "\n";   /*  ECHO|string Client request*/

        admUCli.send_message(msg.str(),server_addr);
        message = admUCli.receive_message(server_addr);

        if(strcmp(message[0].c_str(),"ECHO") == 0) /* ECHO|string Server response*/
        {
          std::cout << "[+] Server response: : " << message[1] << "\n";
        }
        else if(strcmp(message[0].c_str(),"ERR") == 0)
        {
          if(strcmp(message[1].c_str(),"ADV") == 0)  /* ERR|ADV First Advertisement */
            std::cout << "[+] Please wait" << "\n";
          else if(strcmp(message[1].c_str(),"BAN") == 0)    /* ERR|BAN Ban for Two Minutes */
            std::cout << "[+] Banned" << "\n";
          else if(strcmp(message[1].c_str(),"BLS") == 0)    /* ERR|BLS Permanent Blacklist */
            std::cout << "[+] Permanently blacklisted" << "\n";
        }
      }

      return 0;
}
