#ifndef _udp_agent_h
#define _udp_agent_h

#include "lib_base.h"
#include "udp_socket.h"
#include "generic_exception.h"

class udp_agent : public udp_socket
{
public:

  udp_agent(int family , int protocol  = 0 )
  : udp_socket( family , protocol ) {}

  virtual ~udp_agent() { }


  void send_message( std::string msg , ip_address & adr );
  std::vector<std::string> receive_message(ip_address & adr);


private:

  char buffer[MAXLINE];


};

#endif /* _udp_agent_h */
