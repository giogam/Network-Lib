#include "../../../include/lib_base.h"
#include "../../../include/ip_address.h"
#include "../../../include/base_socket.h"
#include "../../../include/socket_exception.h"

base_socket::base_socket( int family  , int type , int protocol )
{

    if((id_ = ::socket(family , type , protocol )) == -1)
    {
      throw socket_exception("socket::socket() " , errno , true );
    }
}


bool base_socket::bind( const ip_address & adr )
{

    base_socket::ip_ = adr;

      ip_address::address_t ip = adr.get_address();

      if(::bind(id_,(struct sockaddr *) (&ip),adr.get_size()) == -1)
      {
        throw socket_exception("socket::bind() " , errno , true );
      }

return true;
}

bool base_socket::connect( const ip_address & adr )
{
   ip_address::address_t ip = adr.get_address();

      if(::connect(id_,(struct sockaddr *) (&ip), adr.get_size()) == -1)
      {
        throw socket_exception("socket::connect() " , errno , true );
      }

  return  true;
}

bool base_socket::listen( int max_listen_)
{
  if(::listen(id_ ,max_listen_ ) == -1)
  {
    throw socket_exception("socket::listen() " , errno , true );
  }
  return  true;
}

bool base_socket::accept( base_socket & conn_sk )
{
  int conn_id = 0;

  ip_address::address_t ip;
  ip_address::addrlen_t l_ip = sizeof(ip);

    if((conn_id = ::accept(id_ , (struct sockaddr*) (&ip), &(l_ip) )) == -1)
    {
      throw socket_exception("socket::accept() " , errno , true );
    }

    conn_sk.id_ = conn_id;

    if(ip.ss_family == AF_INET)
    {
        conn_sk.ip_ = *((sockaddr_in *) &ip);
    }
    else if(ip.ss_family == AF_INET6)
    {
        conn_sk.ip_ = *((sockaddr_in6 *) &ip);
    }

  return true;
}

bool base_socket::accept( base_socket * conn_sk )
{
  int conn_id = 0;

  ip_address::address_t ip;
  ip_address::addrlen_t l_ip = sizeof(ip);

    if((conn_id = ::accept(id_ , (struct sockaddr*) (&ip), &(l_ip) )) == -1)
    {
      throw socket_exception("socket::accept() " , errno , true );
    }

    conn_sk->id_ = conn_id;

    if(ip.ss_family == AF_INET)
    {
        conn_sk->ip_ = *((sockaddr_in *) &ip);
    }
    else if(ip.ss_family == AF_INET6)
    {
        conn_sk->ip_ = *((sockaddr_in6 *) &ip);
    }

  return true;
}

ssize_t  base_socket::sendto(const void * data  , size_t data_size , const ip_address & adr)
{

   ssize_t n_sent = 0;
   ip_address::address_t ip = adr.get_address();


    if((n_sent = ::sendto(id_ , data , data_size , MSG_NOSIGNAL , (struct sockaddr *) (&ip) ,adr.get_size() )) == -1)
    {
      throw socket_exception("socket::sendto() " , errno , true );
    }

  return n_sent;
}

ssize_t base_socket::recvfrom(void * data  , size_t data_size , ip_address & adr)
{

  ssize_t n_read = 0;
  ip_address::address_t ip;
  ip_address::addrlen_t l_ip = sizeof(ip);

    if((n_read = ::recvfrom(id_ , (char * ) data , data_size, 0, (struct sockaddr * )(&ip) , (&l_ip))) == -1)
    {
      throw socket_exception("socket::recvfrom() " , errno , true );
    }

    if(ip.ss_family == AF_INET)
    {
      adr = *((sockaddr_in *) &ip);
    }
    else if(ip.ss_family == AF_INET6)
    {
      adr = *((sockaddr_in6 *) &ip);
    }

  return n_read;

}

ssize_t base_socket::send(const void * data , size_t data_size)
{
    const char * to_send = (const char *) data;
    ssize_t n_towrite = (ssize_t) data_size;
    ssize_t n_written = 0;

    while(n_towrite > 0 )
    {
      if( (n_written = ::send(id_ ,to_send, (unsigned)n_towrite , MSG_NOSIGNAL )) == -1)
      {
        if (errno == EINTR) /* if interrupted by system call */
        {
          std::cout << "EINTR " << "Ignoring it" << "\n";
          continue;
        }
        else
        {
        throw socket_exception("socket::send() " , errno , true );
        }
      }
      else
      {
        n_towrite -= n_written;
        to_send += n_written;
      }

    }

  return (ssize_t) (data_size - n_towrite);
}



ssize_t base_socket::recv(const void * data , size_t data_size)
{
    const char * to_read = (const char *) data;
    ssize_t n_read = 0;
    ssize_t n_toread = (ssize_t) data_size;

    while(n_toread > 0 )
    {
      if(( n_read = ::recv(id_ , (char *)data , data_size ,0)) == -1)
      {
        if (errno == EINTR) /* if interrupted by system call */
        {
          std::cout << "EINTR " << "Ignoring it" << "\n";
          continue;
        }
        else
        {
          throw socket_exception("socket::recv() " , errno , true );
        }
      }
      else if(n_read == 0 )  /* if EOF */
      {
        return n_read;
      }
      else
      {
        n_toread -= n_read;
        to_read += n_read;
      }

    }
      return (ssize_t) (n_toread - n_read);
}


bool base_socket::set_option(int level, int option_name, void *option_value, size_t option_len)
{

  if(::setsockopt(id_ , level, option_name, (const char *)option_value, option_len) == -1)
  {
    throw socket_exception("socket::setsockopt() " , errno , true );
  }
  return  true;
}

std::string base_socket::random_port()
{
  int _rand;
  std::string _port;

    srand ( time(NULL) );
    _rand = rand() % 6000 + 9535;
    _port = std::to_string(_rand);

  return _port;
}
