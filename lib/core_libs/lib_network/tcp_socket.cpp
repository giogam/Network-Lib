#include "../../../include/lib_base.h"
#include "../../../include/ip_address.h"
#include "../../../include/base_socket.h"
#include "../../../include/tcp_socket.h"
#include "../../../include/socket_exception.h"


bool tcp_socket::bind(const ip_address & adr)
{
  try
    {

      base_socket::bind(adr);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
      if(e.is_fatal()) _exit (1);
    }

  return true;
}

bool tcp_socket::listen(int max_listen_)
{
  try
    {

      base_socket::listen(max_listen_);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
      if(e.is_fatal()) _exit (1);
    }

  return true;
}

bool tcp_socket::accept( base_socket & conn_sk)
{

  try
    {

      base_socket::accept(conn_sk);

    } catch ( socket_exception e )
  {
    std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
    if(e.is_fatal()) _exit (1);
  }

  return true;
}

bool tcp_socket::accept( base_socket * conn_sk)
{

  try
    {

      base_socket::accept(conn_sk);

    } catch ( socket_exception e )
  {
    std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
    if(e.is_fatal()) _exit (1);
  }

  return true;
}


bool tcp_socket::connect(const ip_address & adr)
{
  try
    {

      base_socket::connect(adr);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
      if(e.is_fatal()) _exit (1);
    }

  return true;
}


ssize_t tcp_socket::send(const void * data , size_t data_size)
{
  ssize_t n_sent = 0;

  try
    {

      n_sent = base_socket::send(data , data_size);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
      if(e.is_fatal()) _exit (1);
    }

  return n_sent;
}


ssize_t tcp_socket::recv(const void * data , size_t data_size)
{
  ssize_t n_read = 0;

  try
    {
      n_read = base_socket::recv(data, data_size);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
      if(e.is_fatal()) _exit (1);
    }

  return n_read;
}


bool tcp_socket::set_option(int level, int option_name, void *option_value , size_t option_len)
{
  try
    {

      base_socket::set_option(level,option_name,option_value,option_len);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
      if(e.is_fatal()) _exit (1);
    }
  return true;
}
