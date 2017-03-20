#ifndef _game_session_h
#define _game_session_h

#include "lib_base.h"
#include "item_queue.h"
#include "game_player.h"
#include "game_msg.h"
#include "game_board.h"
#include "elements_board.h"
#include "uno_board.h"
#include "match_logger.h"
#include "file_logger.h"
#include "game_handler.h"
#include "chart.h"
#include "thread.h"

class game_session
{
  public:
    game_session(bool _log)
    {
      if(_log)
      {
	       _logged= true;
	        game_logger = new match_logger("./resources/matches/");
          chart_logger = new file_logger("./resources/chart.crt");
      }
      else _logged = false;

      _chart = new chart();
    };

    ~game_session() { };

    std::string new_session(game_player *one , game_player *two , std::string _game_type);
    bool dispatch_move(game_player * from ,std::string _id , std::string _seed , std::string _val);
    void close_full_session(game_player * a, std::string _id );
    void close_half_session(game_player * a, std::string _id );
    std::string resume_session(game_player *one , game_player *two , std::string _id, std::string _game_type);
    void do_clean();

    typedef std::pair<game_player* , game_player*> chat_pair;

    inline std::string get_chart()
    {
      return _chart->get_chart();
    }

    inline void restore_points( std::string _name , std::string _points)
    {
      _chart->set_points(std::stoi(_points),_name);
    }

  private:
    bool _logged;
    chart * _chart;
    match_logger * game_logger;
    file_logger * chart_logger;
    std::map<std::string , game_board *>         _boardsV;
    std::map<std::string , chat_pair>           _pairsV;
    std::vector<std::string> 			              _closedV;
    std::map<std::string,thread *>  		        _handlersV;
    std::map<std::string ,item_queue<game_msg*>> _sessionV;
};



#endif /* _game_session_h*/
