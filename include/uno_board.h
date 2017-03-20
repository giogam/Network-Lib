#ifndef _uno_board_h
#define _uno_board_h

#include "lib_base.h"
#include "generic_exception.h"
#include "game_board.h"
#include "enum_classes.h"

class uno_board : public game_board
{

  public:

    uno_board(int score_a , int score_b)
      : game_board(score_a,score_b) { };

      ~uno_board() { };

    game_board_status_t _move(int game_player_turn , std::string _seed , std::string _val );
    game_board_status_t _check_win();
    bool check_move(std::string _seed , std::string _val);

};


#endif /* _uno_board_h */
