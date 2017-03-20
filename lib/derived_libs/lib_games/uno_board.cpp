#include "../../../include/uno_board.h"
#include "../../../include/enum_classes.h"

bool uno_board::check_move(std::string _seed , std::string _val)
{
  bool _res = true;
  int num_val = 0;

  if(strcmp(_seed.c_str() , "F") == 0)
  {
    if((strcmp(_val.c_str() , "JACK") != 0) && (strcmp(_val.c_str() , "QUEEN") != 0) && (strcmp(_val.c_str() , "KING") != 0) )
      _res = false;
  }
  if(strcmp(_seed.c_str() , "C") == 0)
  {
    try
    {
      num_val = std::stoi(_val.c_str());
    }
    catch (std::invalid_argument & e)
    {
      _res = false;
    }
    if((num_val < 0) || (num_val > 9) )
      _res = false;
  }
    return _res ;
}


game_board_status_t uno_board::_check_win()
{
  game_board_status_t game_winner;

  game_winner = game_board_status_t::CORRECT_MOVE;

   if(_score.first == _score.second ) /* parity */
   {   if((_score.first == 6))
	  game_winner = game_board_status_t::WIN_ODD;
   }
   else if((_score.first == 6)) /* player 1 win */
	game_winner = game_board_status_t::WIN_PONE;

   else if((_score.second == 6)) /* player 2 win */
	game_winner = game_board_status_t::WIN_PTWO;

  return game_winner;
}


game_board_status_t uno_board::_move(int game_player_turn , std::string _seed , std::string _val)
{
  bool res;
  game_board_status_t _win;

  res = is_turn(game_player_turn);
  if(res  == false)
  {
    throw generic_exception("uno_board::_move() " , game_board_err_t::TURN_BAD , false );
  }

  res = check_move(_seed,_val);
  if(res  == false)
  {
    throw generic_exception("uno_board::_move() " , game_board_err_t::MOVE_BAD , false );
  }

  if(!_check)
  {
    _first = std::make_pair<std::string,std::string>(_seed.c_str(),_val.c_str());
    _check = true;
    next_turn();
    _win = game_board_status_t::CORRECT_MOVE;
  }
  else
  {
    /* FF || CC */
    if(strcmp(_first.first.c_str() , _seed.c_str()) == 0) /* same seed*/
    {
	if(strcmp(_first.first.c_str(),"F") == 0) /* two figures */
       {

	 if(strcmp(_first.second.c_str() , _val.c_str()) == 0) /* same val*/
         {
           _score.first   += 2;
           _score.second  += 2;
         }
         /* JK KJ */
         else if((strcmp(_first.second.c_str() , "JACK") == 0) && (strcmp(_val.c_str() , "KING") == 0))
         {
  	        _score.second += 2;
         }
         /* JK KJ */
         else if((strcmp(_first.second.c_str() , "KING") == 0) && (strcmp(_val.c_str() , "JACK") == 0))
         {
  	        _score.first += 2;
         }
         /* JQ QJ */
         else if((strcmp(_first.second.c_str() , "JACK") == 0) && (strcmp(_val.c_str() , "QUEEN") == 0))
         {
            _score.second += 2;
         }
         /* JQ QJ */
         else if((strcmp(_first.second.c_str() , "QUEEN") == 0) && (strcmp(_val.c_str() , "JACK") == 0))
         {
            _score.first += 2;
         }
         /* QK KQ */
         else if((strcmp(_first.second.c_str() , "QUEEN") == 0) && (strcmp(_val.c_str() , "KING") == 0))
         {
            _score.second += 2;
         }
         /* QK KQ */
         else if((strcmp(_first.second.c_str() , "KING") == 0) && (strcmp(_val.c_str() , "QUEEN") == 0))
         {
            _score.first += 2;
         }

       }
       else if(strcmp(_first.first.c_str(),"C") == 0) /* two numbers*/
       {
         if(strcmp(_first.second.c_str() , _val.c_str()) == 0) /* same val*/
         {
           _score.first   += 2;
           _score.second  += 2;
         }
         else
         {
           int val_1 = std::stoi(_first.second.c_str());
           int val_2 = std::stoi(_val.c_str());

           if(val_1 < val_2)
              _score.second  += 2;
            else
              _score.first   += 2;
         }
       }
    }
    /* FC || CF*/
    else
    {
      if(strcmp(_first.first.c_str(),"F") == 0)
      {
        _score.first   += 2;
      }
      else
      {
        _score.second  += 2;
      }
    }

    _check = false;
    next_turn();
    _win = _check_win();
  }

 return _win;
}
