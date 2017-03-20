#ifndef _tcp_client_h
#define _tcp_client_h

#include "lib_base.h"
#include "tcp_socket.h"
#include "generic_exception.h"

class tcp_client : public tcp_socket
{
public:

  tcp_client(int family , int protocol  = 0 )
  : tcp_socket( family , protocol ) {}

  virtual ~tcp_client() { }


  void send_message( std::string msg );
  std::vector<std::string> receive_message();
  void wait_for(std::string msg);

private:

  char buffer[MAXLINE];


};

#endif /* _tcp_client_h */
