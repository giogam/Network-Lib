#include "../../../include/ttt_player.h"
#include "../../../include/enum_classes.h"

std::vector<std::string> ttt_player::hail()
{
  ssize_t n_read = 0;

    memset(buffer , '\0' , MAXLINE);
    n_read = recv(buffer,MAXLINE);
    if(n_read == 0)
    {
      throw generic_exception("ttt_player::hail() " , socket_status_t::BAD_READ , false );
    }

    std::string buff(buffer);
    std::stringstream ss(buff);
    std::string tmp;
    std::vector<std::string> message;

    while (ss >> tmp) message.push_back(tmp);

    if((message.size() != 3) || (message[0].compare("HELO") != 0) )
    {
      throw generic_exception("ttt_player::hail() " , user_err_status_t::US_BAD_FORMAT , true );
    }

    return message;
}


void ttt_player::send_message(std::string  msg)
{
  memset(buffer , '\0' , MAXLINE);
  std::sprintf(buffer,"%s",msg.c_str());
  send(buffer,MAXLINE);
}

std::vector<std::string> ttt_player::receive_message()
{
  ssize_t n_read = 0;

    memset(buffer , '\0' , MAXLINE);
    n_read = recv(buffer,MAXLINE);
    if(n_read == 0)
    {
      throw generic_exception("ttt_player::receive_message() " , socket_status_t::BAD_READ , false );
    }

    std::string buff(buffer);
    std::stringstream ss(buff);
    std::string tmp;
    std::vector<std::string> message;

    while (ss >> tmp) message.push_back(tmp);

    return message;
}
