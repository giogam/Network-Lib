#include "../../../include/ip_address.h"
#include "../../../include/lib_base.h"

std::string ip_address::to_string() const
{
    char dest[INET6_ADDRSTRLEN+2];
    dest[sizeof(dest)-1] = '\0';

    if(address_.ss_family == AF_INET)
        return !::inet_ntop(AF_INET,&(((struct sockaddr_in *) &address_)->sin_addr) , dest ,sizeof(dest)) ? std::string() : dest;

    else if(address_.ss_family == AF_INET6)
        return !::inet_ntop(AF_INET6,&(((struct sockaddr_in6 *) &address_)->sin6_addr), dest ,sizeof(dest)) ? std::string() : dest;

    else
        return std::string();
}


bool ip_address::parse_addr(std::string addr_ , std::string port_ ,std::string _iface , int ai_family , int ai_socktype , int ai_flags )
{

    ip_address::clear_addr();

      struct addrinfo hints , * host_info;
      memset(&hints , 0, sizeof(struct addrinfo));

      switch(ai_family)
      {

        case AF_INET:
          hints.ai_family = ai_family;
          hints.ai_socktype = ai_socktype;
          hints.ai_flags = ai_flags;
          break;

        case AF_INET6:
          hints.ai_family = ai_family;
          hints.ai_socktype = ai_socktype;
          hints.ai_flags = ai_flags;
          break;
      }

      getaddrinfo(addr_.c_str(), port_.c_str(), &hints, &host_info);

      if(host_info->ai_family == AF_INET )
      {
        size_ = sizeof(struct sockaddr_in);
        address_.ss_family = AF_INET;
        ((struct sockaddr_in *)&address_)->sin_port = ((struct sockaddr_in *) host_info->ai_addr)->sin_port;
        ((struct sockaddr_in *)&address_)->sin_addr = ((struct sockaddr_in *) host_info->ai_addr)->sin_addr;
      }
      else if(host_info->ai_family == AF_INET6 )
      {
        size_ = sizeof(struct sockaddr_in6);
        address_.ss_family = AF_INET6;
        ((struct sockaddr_in6 *)&address_)->sin6_port = ((struct sockaddr_in6 *) host_info->ai_addr)->sin6_port;
        ((struct sockaddr_in6 *)&address_)->sin6_addr = ((struct sockaddr_in6 *) host_info->ai_addr)->sin6_addr;
	      ((struct sockaddr_in6 *)&address_)->sin6_scope_id = if_nametoindex(_iface.c_str());

      }

      freeaddrinfo(host_info);


    return false;
}
