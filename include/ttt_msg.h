#ifndef _ttt_msg_h
#define _ttt_msg_h

#include "lib_base.h"
#include "ttt_player.h"

class ttt_msg
{
  public:

    ttt_msg(std::string msg, ttt_player * player_to  )
      : _msg(msg), _player_to(player_to) { }
    virtual ~ttt_msg( ) { delete _player_to;};

    std::string get_message()
    {
      return _msg;
    }

    ttt_player * get_dest()
    {
      return _player_to;
    }

  private:
    std::string _msg;
    ttt_player * _player_to;

};

#endif /* _ttt_msg_h */
