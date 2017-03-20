#ifndef _chat_user_h
#define _chat_user_h

#include "lib_base.h"
#include "generic_exception.h"
#include "tcp_socket.h"

class chat_user : public tcp_socket
{

  public:

    chat_user(int family , int protocol  = 0 )
    : tcp_socket( family , protocol ) {}

    virtual ~chat_user() { }

    std::string hail();
    void send_message( std::string msg );
    std::vector<std::string> receive_message();

    inline bool is_busy()
      {
        return _in_chat;
      }

    inline void set_status(bool status)
      {
        _in_chat = status;
      }

    inline std::string get_nick()
      {
        return _nickname;
      }

    inline void set_session(std::string _id)
      {
	       _session = _id;
      }

    inline std::string get_session()
      {
	return _session;
      }

    inline void set_nick(std::string name)
      {
        _nickname = name;
      }

    inline void set_peer(std::string _peer)
      {
        _peer_name = _peer;
      }

    inline std::string get_peer()
      {
        return _peer_name;
      }

  private:

    bool _in_chat;

    std::string _nickname;
    std::string _peer_name;
    std::string _session;
    char buffer[MAXLINE];
};


#endif /* _chat_user_h */
