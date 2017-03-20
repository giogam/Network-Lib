#ifndef _game_msg_h
#define _game_msg_h

#include "lib_base.h"
#include "game_player.h"

class game_msg
{
  public:

    game_msg(std::string msg, game_player * player_to  )
      : _msg(msg), _player_to(player_to) { }
    virtual ~game_msg( ) { delete _player_to;};

    std::string get_message()
    {
      return _msg;
    }

    game_player * get_dest()
    {
      return _player_to;
    }

  private:
    std::string _msg;
    game_player * _player_to;

};

#endif /* _game_msg_h */
