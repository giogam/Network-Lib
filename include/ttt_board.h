#ifndef _ttt_board_h
#define _ttt_board_h

#include "lib_base.h"
#include "generic_exception.h"
#include "enum_classes.h"

class ttt_board
{

  public:

    ttt_board();
    ~ttt_board();
    std::string get_status();
    ttt_board_status_t _move(int ttt_player_turn ,std::string row , std::string col );
    void set_status(std::string status);


    inline void set_turn(int turn)
    {
      _turn = turn ;
    }


    inline int get_turn()
    {
      return _turn;
    }

  private:

    ttt_board_status_t _check_diag(int a, int b , int c);
    ttt_board_status_t _check_row(std::string row);
    ttt_board_status_t _check_col(int col);
    ttt_board_status_t _check_win();
    ttt_board_status_t _check_odd();

    bool check_move( std::string row , std::string col );

    inline bool is_turn( int ttt_player_turn )
      {
        return (ttt_player_turn == _turn) ? true : false;
      }

    inline void next_turn()
      {
        _turn = ((_turn + 1) % 2);
      }

    int _turn;
    std::map<std::string,std::vector<std::string>> _board;
};


#endif /* _ttt_board_h */
