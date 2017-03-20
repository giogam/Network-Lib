#ifndef _ttt_player_h
#define _ttt_player_h

#include "lib_base.h"
#include "generic_exception.h"
#include "tcp_socket.h"

class ttt_player : public tcp_socket
{

  public:

    ttt_player(int family , int protocol  = 0 )
    : tcp_socket( family , protocol ) {}

    virtual ~ttt_player() { }

    std::vector<std::string> hail();
    void send_message( std::string msg );
    std::vector<std::string> receive_message();


    inline void set_number(int n)
    {
      _n_player = n;
    }

    inline int get_number()
    {
      return _n_player;
    }

    inline bool is_busy()
    {
      return _in_game;
    }

    inline void set_status(bool status)
    {
      _in_game = status;
    }

    inline void set_nick(std::string name)
    {
      _nickname = name;
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

    inline void set_peer(std::string _peer)
    {
      _peer_name = _peer;
    }

    inline std::string get_peer()
    {
      return _peer_name;
    }

    inline void set_ctrl(std::string ctrl)
    {
      _ctrl = ctrl;
    }

    inline std::string get_ctrl()
    {
      return _ctrl;
    }

  private:
    bool _in_game;
    std::string _nickname;
    std::string _peer_name;
    std::string _session;
    std::string _ctrl;
    int _n_player;
    char buffer[MAXLINE];
};


#endif /* _ttt_player_h */
