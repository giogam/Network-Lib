#ifndef _udp_socket_h
#define _udp_socket_h

#include "lib_base.h"
#include "ip_address.h"
#include "base_socket.h"
#include "socket_exception.h"


class udp_socket : public base_socket
{

public:
  udp_socket( int family , int protocol = 0 )
  try : base_socket(family , SOCK_DGRAM , protocol)
  {

  } catch ( socket_exception e )
  {
    std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
  }


  virtual ~udp_socket() {  }


  bool bind( const ip_address & adr );
  bool connect(const ip_address & adr );

  ssize_t sendto(const void * data  , size_t data_size , const ip_address & adr);
  ssize_t recvfrom(void * data  , size_t data_size , ip_address & adr);

  bool set_option(int level, int option_name, void *option_value , size_t option_len);

};

#endif /* _udp_socket_h */
