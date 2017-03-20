#ifndef _ttt_session_h
#define _ttt_session_h

#include "lib_base.h"
#include "item_queue.h"
#include "ttt_player.h"
#include "ttt_msg.h"
#include "ttt_board.h"
#include "match_logger.h"
#include "ttt_handler.h"
#include "thread.h"

class ttt_session
{
  public:
    ttt_session(bool _log)
    {
      if(_log)
      {
	_logged= true;
	game_logger = new match_logger("./resources/matches/");
      }
      else _logged = false;
    };

    ~ttt_session() { };

    std::string new_ttt_session(ttt_player *one , ttt_player *two);
    bool dispatch_ttt_move(ttt_player * from ,std::string _id , std::string _row , std::string _col);
    void close_full_ttt_session(ttt_player * a, std::string _id );
    void close_half_ttt_session(ttt_player * a, std::string _id );
    std::string resume_ttt_session(ttt_player *one , ttt_player *two , std::string _id , std::string _player_num);
    void do_ttt_clean();

    typedef std::pair<ttt_player* , ttt_player*> chat_pair;

  private:
    bool _logged;
    match_logger * game_logger;
    std::map<std::string , ttt_board *>         _boardsV;
    std::map<std::string , chat_pair>           _pairsV;
    std::vector<std::string> 			              _closedV;
    std::map<std::string,thread *>  		        _handlersV;
    std::map<std::string ,item_queue<ttt_msg*>> _sessionV;
};



#endif /* _ttt_session_h */
