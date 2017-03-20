#ifndef _tcp_socket_h
#define _tcp_socket_h

#include "lib_base.h"
#include "ip_address.h"
#include "base_socket.h"
#include "socket_exception.h"


class tcp_socket : public base_socket
{

  public:

    tcp_socket( int family , int protocol = 0 )
      try : base_socket(family , SOCK_STREAM , protocol)
      {

      } catch ( socket_exception e )
      {
          std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
      }


  virtual ~tcp_socket() { }


  bool bind( const ip_address & adr );
  bool connect(const ip_address & adr );
  bool listen( int max_listen_ = 5);
  bool accept( base_socket & conn_sk);
  bool accept( base_socket * conn_sk);

  ssize_t send(const void * data , size_t data_size);
  ssize_t recv(const void * data , size_t data_size);

  bool set_option(int level, int option_name, void *option_value , size_t option_len);


};

#endif /* _tcp_socket_h */
