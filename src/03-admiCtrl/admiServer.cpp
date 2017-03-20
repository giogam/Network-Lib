#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/udp_agent.h"
#include "../../include/enum_classes.h"
#include "../../include/admission_controller.h"

int main(int argc , char *argv[])
{

  int mode  = 0;
  int status = 0;
  ip_address client_addr;
  admission_controller admCtrl;
  std::stringstream msg;
  std::vector<std::string> message;

  if((argc != 4) || ((strcmp(argv[1],"v4") != 0) && (strcmp(argv[1],"v6")) != 0) )
  {
      std::cout << "[-] usage: " << argv[0] << " <mode> <address> <port>" << "\n";
      _exit (1);
  }

    mode = (strcmp(argv[1],"v4") == 0) ? AF_INET : AF_INET6;
    ip_address server_addr(argv[2] , argv[3] , mode , SOCK_DGRAM , AI_PASSIVE );
    udp_agent admUServ(mode);

    admUServ.bind(server_addr);
    std::cout << "[+] Welcome to admiServer!" << "\n";
    std::cout << "[+] Waiting for connections.." << "\n";

    loop_always()
    {
      CLEAR_MSG(msg);
      message = admUServ.receive_message(client_addr);
      std::cout << "[+] Incoming request from: " << client_addr.to_string() << client_addr.get_port() << "\n";

      status = admCtrl.admission_control(client_addr.to_string());

      switch (status)
      {
        case client_status_t::CLI_SRV:
          msg << "ECHO" << " " << message[1] << "\n";
          std::cout << "ECHO" << "\n";
          break;
        case client_status_t::CLI_ADV:
          msg << "ERR" << " " << "ADV" << "\n";
          std::cout << "ADV" << "\n";
          break;
        case client_status_t::CLI_BAN:
          msg << "ERR" << " " << "BAN" << "\n";
          std::cout << "BAN" << "\n";
          break;
        case client_status_t::CLI_BLS:
          msg << "ERR" << " " << "BLS" << "\n";
          std::cout << "BLS" << "\n";
          break;
      }
      admUServ.send_message(msg.str(),client_addr);
    }

      return 0;
}
