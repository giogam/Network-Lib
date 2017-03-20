#include "../../../include/tcp_client.h"
#include "../../../include/enum_classes.h"


void tcp_client::wait_for(std::string msg)
{
  ssize_t n_read = 0;

    memset(buffer , '\0' , MAXLINE);
    n_read = recv(buffer,MAXLINE);
    if(n_read == 0)
    {
      throw generic_exception("tcp_client::::wait_for() " , socket_status_t::BAD_READ , false );
    }

    std::string buff(buffer);
    std::stringstream ss(buff);
    std::string tmp;
    std::vector<std::string> message;

    while (ss >> tmp) message.push_back(tmp);

    if((message.size() != 1) || (message[0].compare(msg.c_str()) != 0) )
    {
      throw generic_exception("tcp_client::wait_for() " , user_err_status_t::US_BAD_FORMAT , true );
    }

}


void tcp_client::send_message(std::string  msg)
{


    memset(buffer , '\0' , MAXLINE);
    std::sprintf(buffer,"%s",msg.data());
    send(buffer,MAXLINE);
}


std::vector<std::string> tcp_client::receive_message()
{
  ssize_t n_read = 0;

    memset(buffer , '\0' , MAXLINE);
    n_read = recv(buffer,MAXLINE);

    if(n_read == 0)
    {
      throw generic_exception("tcp_client::receive_message() " , socket_status_t::BAD_READ , false );
    }

    std::string buff(buffer);
    std::stringstream ss(buff);
    std::string tmp;
    std::vector<std::string> message;

    while (ss >> tmp) message.push_back(tmp);

    return message;
}
