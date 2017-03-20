#include "../../../include/ttt_board.h"
#include "../../../include/enum_classes.h"

ttt_board::ttt_board()
{
    _board["A"].assign(3,"-");
    _board["B"].assign(3,"-");
    _board["C"].assign(3,"-");

    _turn = 0;
}

ttt_board_status_t ttt_board::_check_diag(int a, int b, int c)
{
  if((strcmp(_board["A"].at(a).c_str(),"O") == 0) && (strcmp(_board["B"].at(b).c_str(),"O") == 0) && (strcmp(_board["C"].at(c).c_str(),"O") == 0) ) return ttt_board_status_t::PONE_WIN;
  else if ((strcmp(_board["A"].at(a).c_str(),"X") == 0) && (strcmp(_board["B"].at(b).c_str(),"X") == 0) && (strcmp(_board["C"].at(c).c_str(),"X") == 0) ) return ttt_board_status_t::PTWO_WIN;
  else return ttt_board_status_t::NO_WIN;
}

ttt_board_status_t ttt_board::_check_row(std::string row)
{
  if((strcmp(_board[row].at(0).c_str(),"O") == 0) && (strcmp(_board[row].at(1).c_str(),"O") == 0) && (strcmp(_board[row].at(2).c_str(),"O") == 0) ) return ttt_board_status_t::PONE_WIN;
  else if ((strcmp(_board[row].at(0).c_str(),"X") == 0) && (strcmp(_board[row].at(1).c_str(),"X") == 0) && (strcmp(_board[row].at(2).c_str(),"X") == 0) ) return ttt_board_status_t::PTWO_WIN;
  else return ttt_board_status_t::NO_WIN;
}

ttt_board_status_t ttt_board::_check_col(int col)
{
  if((strcmp(_board["A"].at(col).c_str(),"O") == 0) && (strcmp(_board["B"].at(col).c_str(),"O") == 0) && (strcmp(_board["C"].at(col).c_str(),"O") == 0) ) return ttt_board_status_t::PONE_WIN;
  else if ((strcmp(_board["A"].at(col).c_str(),"X") == 0) && (strcmp(_board["B"].at(col).c_str(),"X") == 0) && (strcmp(_board["C"].at(col).c_str(),"X") == 0) ) return ttt_board_status_t::PTWO_WIN;
  else return ttt_board_status_t::NO_WIN;
}

ttt_board_status_t ttt_board::_check_odd()
{
  int _cnt = 0;
  for(auto it = _board.begin() ; it != _board.end(); ++it )
  {
    for(int i = 0; i < 3 ; i++)
    {
      if((strcmp(it->second.at(i).c_str(),"O") == 0)  || (strcmp(it->second.at(i).c_str(),"X") == 0))
        _cnt++;
    }
  }
  if(_cnt == 9)
    return ttt_board_status_t::TTT_ODD;
  else
    return ttt_board_status_t::NO_WIN;
}

ttt_board_status_t ttt_board::_check_win()
{
  ttt_board_status_t ttt_winner;

    /*D A[0]B[1]C[2] */
    ttt_winner = _check_diag(0,1,2);
    if(ttt_winner != ttt_board_status_t::NO_WIN) return ttt_winner;
    /*D A[2]B[1]C[0]*/
    ttt_winner = _check_diag(2,1,0);
    if(ttt_winner != ttt_board_status_t::NO_WIN) return ttt_winner;
    /*R A[0]A[1]A[2]*/
    ttt_winner = _check_row("A");
    if(ttt_winner != ttt_board_status_t::NO_WIN) return ttt_winner;
    /*R B[0]B[1]B[2]*/
    ttt_winner = _check_row("B");
    if(ttt_winner != ttt_board_status_t::NO_WIN) return ttt_winner;
    /*R C[0]C[1]C[2]*/
    ttt_winner = _check_row("C");
    if(ttt_winner != ttt_board_status_t::NO_WIN) return ttt_winner;
    /*C A[0]B[0]C[0] */
    ttt_winner = _check_col(0);
    if(ttt_winner != ttt_board_status_t::NO_WIN) return ttt_winner;
    /*C A[1]B[1]C[1] */
    ttt_winner = _check_col(1);
    if(ttt_winner != ttt_board_status_t::NO_WIN) return ttt_winner;
    /*C A[2]B[2]C[2] */
    ttt_winner = _check_col(2);
    if(ttt_winner != ttt_board_status_t::NO_WIN) return ttt_winner;
    ttt_winner = _check_odd();
    if(ttt_winner != ttt_board_status_t::NO_WIN) return ttt_winner;


  return ttt_winner;
}

void ttt_board::set_status(std::string status)
{
   _board["A"].at(0) = status[0];
   _board["A"].at(1) = status[2];
   _board["A"].at(2) = status[4];

   _board["B"].at(0) = status[6];
   _board["B"].at(1) = status[8];
   _board["B"].at(2) = status[10];

   _board["C"].at(0) = status[12];
   _board["C"].at(1) = status[14];
   _board["C"].at(2) = status[16];
}


std::string ttt_board::get_status()
{
  std::stringstream ss;

    for(auto it = _board.begin() ; it != _board.end(); ++it )
    {
      ss << it->second.at(0) << " ";
      ss << it->second.at(1) << " ";
      ss << it->second.at(2) << " ";
    }

    return ss.str();
}

bool ttt_board::check_move(std::string row , std::string col)
{
  if((strcmp(row.c_str(),"A") != 0) && (strcmp(row.c_str(),"B") != 0) && (strcmp(row.c_str(),"C") != 0))
    return false;
  else if(std::atoi(col.c_str()) > 2)
    return false;
  else if(strcmp((_board[row].at(std::atoi(col.c_str()))).c_str(), "-") != 0)
    return false;
  else
    return true;
}

ttt_board_status_t ttt_board::_move(int ttt_player_turn ,std::string row , std::string col )
{
  bool res;
  ttt_board_status_t _win;

  res = is_turn(ttt_player_turn);
  if(res  == false)
  {
    throw generic_exception("ttt_board::_move() " , ttt_board_err_t::BAD_TURN , false );
  }

  res = check_move(row,col);
  if(res == false)
  {
    throw generic_exception("ttt_board::_move() " , ttt_board_err_t::BAD_MOVE , false );
  }

  if(_turn == 0)
  {
    _board[row].at(std::atoi(col.c_str())) = "O";
    _win = _check_win();
    if(_win == ttt_board_status_t::NO_WIN)
    {
      next_turn();
      return ttt_board_status_t::GOOD_MOVE;
    }
    else return _win;
  }
  else
  {
    _board[row].at(std::atoi(col.c_str())) = "X";
    _win = _check_win();
    if(_win == ttt_board_status_t::NO_WIN)
    {
      next_turn();
      return ttt_board_status_t::GOOD_MOVE;
    }
    else return _win;
  }

}
