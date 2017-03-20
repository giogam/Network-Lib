#include "../../include/lib_base.h"
#include "../../include/ip_address.h"
#include "../../include/tcp_client.h"
#include "../../include/b64_encoder.h"
#include "../../include/phone_db.h"
#include "../../include/file_handler.h"
#include "../../include/enum_classes.h"

int main(int argc , char *argv[])
{
  int mode  = 0;
  std::vector<std::string> result;
  std::stringstream msg;
  std::vector<std::string> message;
  tcp_client *connTSock = NULL;
  B64_Encoder *encoder;

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

    file_handler fileHandle("./files/list.pdb");
    encoder = new B64_Encoder();
    std::cout << "[+] Welcome to FileFtransferServer!" << "\n";
    std::cout << "[+] Waiting for connections.." << "\n\n";

    loop_always()
    {
      try
      {
        connTSock = new tcp_client(mode);
        counTServ.accept(connTSock);
        std::cout << "[+] Incoming connection from:" << connTSock->get_ip() << ": " << connTSock->get_port() << "\n";

        loop_always()
        {
          CLEAR_MSG(msg);
          message.clear();
          message = connTSock->receive_message();

          if(strcmp(message[0].c_str(),"LIST") == 0) /* LIST */
          {
            std::vector<std::string> list;
            std::string itStr;
            list = fileHandle.get_list();
            msg << "ACK" << " " << "LIST" << " " << list.size() << " ";
            for(auto it = list.begin() ; it!= list.end() ; ++it)
            {
              itStr += *it;
              itStr += " ";
            }
            msg << itStr << "\n";
            connTSock->send_message(msg.str());
          }
          else if (strcmp(message[0].c_str(),"GET") == 0) /* GET|fname*/
    	     {
	            std::string path("./files/");
              path += message[1];
              std::ifstream inFile (path.c_str(), std::ifstream::binary);

	             if (!inFile.is_open())
               {
                  msg << "ERR" << " " << "FILE" << "\n";
                  connTSock->send_message(msg.str());
                  continue;
               }
               else
               {
                 inFile.seekg (0, inFile.end);
                 std::streampos _size = inFile.tellg();
                 inFile.seekg (0, inFile.beg);

                 std::cout << "File Size: " << _size;
                 char * _buffer = new char [(MAXLINE/2)];
                 int    _chunk = 0;

                 CLEAR_MSG(msg);
                 msg << "ACK" << " " << "START" << " " << message[1];
                 connTSock->send_message(msg.str());

                 while(inFile.readsome(_buffer , (MAXLINE/2)))
                 {
                   CLEAR_MSG(msg);
  	               std::streamsize _buflen = inFile.gcount();
                   std::cout << "Read " << inFile.gcount()  << "\n";

                   unsigned char *p = reinterpret_cast<unsigned char*>(_buffer);
                   std::vector<unsigned char> _input( p , p+_buflen);
                   std::string _encoded = encoder->base64Encode(_input);

                   msg << "ACK" << " " << "CHUNK" << " " << _chunk << " " << _encoded.length() << " " << _encoded << " " << "\n";
                   _chunk++;
                   connTSock->send_message(msg.str());
                   connTSock->wait_for("ACK");
                 }

                 CLEAR_MSG(msg);
                 msg << "ACK" << " " << "DONE";
                 connTSock->send_message(msg.str());
                 inFile.close();

                 delete [] _buffer;
               }
             }
             else if (strcmp(message[0].c_str(),"CLOSE") == 0) /* CLOSE */
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
