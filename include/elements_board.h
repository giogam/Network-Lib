#ifndef _elements_board_h
#define _elements_board_h

#include "lib_base.h"
#include "generic_exception.h"
#include "game_board.h"
#include "enum_classes.h"

class elements_board : public game_board
{

  public:

    elements_board(int score_a , int score_b)
      : game_board(score_a,score_b) { };

      ~elements_board() { };

    game_board_status_t _move(int game_player_turn , std::string _seed , std::string _val );
    game_board_status_t _check_win();
    bool check_move(std::string _seed , std::string _val);
};


#endif /* _elements_board_h */
