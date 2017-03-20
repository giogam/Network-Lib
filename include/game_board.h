#ifndef _game_board_h
#define _game_board_h

#include "lib_base.h"
#include "generic_exception.h"
#include "enum_classes.h"

class game_board
{

  public:

    game_board(int a_score , int b_score);
    ~game_board() { };

    std::string get_status();
    int get_turn();
    bool is_turn( int game_player_turn );
    void next_turn();
    void set_status(std::string score_a , std::string score_b);
    void set_turn(int turn);
    void set_move(std::string _seed, std::string _val);
    void set_check(bool val);

    virtual game_board_status_t _move(int game_player_turn , std::string _seed , std::string _val ) = 0;
    virtual game_board_status_t _check_win() = 0;

    int _turn;
    bool _check;
    std::pair<int,int> _score;
    std::pair<std::string,std::string> _first;

};


#endif /* _game_board_h */
