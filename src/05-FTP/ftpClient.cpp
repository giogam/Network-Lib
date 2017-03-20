#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/tcp_client.h"
#include "../../include/b64_encoder.h"
#include "../../include/enum_classes.h"

int main(int argc , char *argv[])
{
  std::string input;
  int mode  = 0;
  std::stringstream msg;
  std::vector<std::string> message;
  std::ofstream  outFile;
  B64_Encoder *encoder;
  bool _downloading = false;
  std::string path("./");

  if((argc != 4) || ((strcmp(argv[1],"v4") != 0) && (strcmp(argv[1],"v6") != 0)) )
  {
      std::cout << "[-] usage: " << argv[0] << " <mode> <address> <port>" << "\n";
      _exit (1);
  }

    mode = (strcmp(argv[1],"v4") == 0) ? AF_INET : AF_INET6;
    ip_address server_addr(argv[2] , argv[3] , mode , SOCK_STREAM );
    tcp_client countTCli(mode);

    encoder = new B64_Encoder();

    countTCli.connect(server_addr);
    std::cout << "[+] Welcome to FileFtransferClient!" << "\n";
    std::cout << "[+] Type 'quit' to exit!" << "\n";

    loop_always()
    {
      try
        {
          if(!_downloading)
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

            if(strcmp(input.c_str(),"ls") == 0)
            {
              msg << "LIST" << "\n";
            }
            else if(strcmp(input.c_str(),"quit") == 0)
            {
	             msg << "CLOSE" << "\n";
             }
             else if(strcmp(input.c_str(),"get") == 0)
             {
	              std::string _fname;
	              std::cout << "filemane> ";
	              std::getline(std::cin , _fname);

	              if(_fname.empty())
	              {
	                 std::cout << "No filename given" << "\n";
	                 continue;
	              }
	              msg << "GET" << " " << _fname;
              }
              else
              {
	               std::cout << "Invalid command" << "\n";
	               continue;
              }
              countTCli.send_message(msg.str());
            }

            message = countTCli.receive_message();

            if(strcmp(message[0].c_str(),"ACK") == 0)
            {
              if(strcmp(message[1].c_str(),"CLOSE") == 0) /* ACK|CLOSE*/
              {
                std::cout << "[-] Connection closed. Bye Bye! " << "\n";
                break;
              }
              else if(strcmp(message[1].c_str(),"LIST") == 0) /* ACK|LIST|n_entries|entries */
              {
                int _entries = std::atoi(message[2].c_str());
                for(int _iter=0 ; _iter < _entries; _iter++)
                {
                  std::cout << "[+] " << message[_iter+3] << "\n";
                }
              }
              else if(strcmp(message[1].c_str(),"START") == 0) /* ACK|START|fname */
              {
                _downloading = true;
                path.clear();
                path += "./";
                path += message[2];
                outFile.open(path.c_str(),std::ofstream::binary|std::ofstream::app);
              }
              else if(strcmp(message[1].c_str(),"CHUNK") == 0) /* ACK|CHUNK|n|length|enc_ata */
              {
                CLEAR_MSG(msg);
                std::cout << "[+] Received Chunk # " << message[2] << "Bytes " << message[3] << "\n";
                std::cout << "MSG " << message[4].data() << "\n";
                std::vector<unsigned char> _decoded = encoder->base64Decode(message[4]);
                std::string _toWrite(_decoded.begin(),_decoded.end());
                outFile.write(_toWrite.data(),_toWrite.length());
                msg << "ACK" << "\n";
                countTCli.send_message(msg.str());
              }
              else if(strcmp(message[1].c_str(),"DONE") == 0) /* ACK|DONE */
              {
                std::cout << "[+] Transfer Done " << "\n";
                outFile.close();
              }

            }
          }

        catch(generic_exception & e)
        {
          switch(e.get_errcode())
          {
            case socket_status_t::BAD_READ:
            std::cout << "[-] Server Shutdown " << "\n";
            _exit (1);
          }
        }
        }
      return 0;
}
