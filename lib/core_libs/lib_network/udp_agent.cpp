#include "../../../include/udp_agent.h"
#include "../../../include/enum_classes.h"

void udp_agent::send_message(std::string  msg , ip_address & adr)
{
  memset(buffer , '\0' , MAXLINE);
  std::sprintf(buffer,"%s",msg.c_str());
  sendto(buffer,MAXLINE,adr);
}


std::vector<std::string> udp_agent::receive_message(ip_address & adr)
{
  ssize_t n_read = 0;

    memset(buffer , '\0' , MAXLINE);
    n_read = recvfrom(buffer,MAXLINE,adr);

    if(n_read == 0)
    {
      throw generic_exception("udp_agent::receive_message() " , socket_status_t::BAD_READ , false );
    }

    std::string buff(buffer);
    std::stringstream ss(buff);
    std::string tmp;
    std::vector<std::string> message;

    while (ss >> tmp) message.push_back(tmp);

    return message;
}
