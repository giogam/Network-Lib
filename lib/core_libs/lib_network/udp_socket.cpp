#include "../../../include/lib_base.h"
#include "../../../include/ip_address.h"
#include "../../../include/base_socket.h"
#include "../../../include/udp_socket.h"
#include "../../../include/socket_exception.h"


bool udp_socket::bind(const ip_address & adr)
{
  try
    {

      base_socket::bind(adr);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
    }

  return true;
}


bool udp_socket::connect(const ip_address & adr)
{
  try
    {

      base_socket::connect(adr);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
    }

  return true;
}


ssize_t udp_socket::sendto(const void * data  , size_t data_size , const ip_address & adr)
{
  ssize_t n_sent = 0;

  try
    {

      n_sent = base_socket::sendto(data , data_size , adr);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
    }

  return n_sent;
}


ssize_t udp_socket::recvfrom(void * data  , size_t data_size , ip_address & adr)
{
  ssize_t n_read = 0;

  try
    {
      n_read = base_socket::recvfrom(data, data_size, adr);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
    }

  return n_read;
}


bool udp_socket::set_option(int level, int option_name, void *option_value , size_t option_len)
{
  try
    {

      base_socket::set_option(level,option_name,option_value,option_len);

    } catch ( socket_exception e )
    {
      std::cout << e.get_where()  << std::strerror(e.get_errcode()) << "\n";
    }

    return true;
}
