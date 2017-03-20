#include "../../../include/elements_board.h"
#include "../../../include/enum_classes.h"

bool elements_board::check_move(std::string _seed , std::string _val)
{
  bool _res = true;
  int num_val = 0;

  if((strcmp(_seed.c_str() , "AIR") != 0) && (strcmp(_seed.c_str() , "EARTH") != 0) && (strcmp(_seed.c_str() , "WATER") != 0) &&(strcmp(_seed.c_str() , "FIRE") != 0))
    _res = false;

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

    return _res ;
}

game_board_status_t elements_board::_check_win()
{
  game_board_status_t game_winner;

  game_winner = game_board_status_t::CORRECT_MOVE;

   if(_score.first == _score.second ) /* parity */
   {   if((_score.first >= 6))
	  game_winner = game_board_status_t::WIN_ODD;
   }
   else if((_score.first >= 6)) /* player 1 win */
	game_winner = game_board_status_t::WIN_PONE;

   else if((_score.second >= 6)) /* player 2 win */
	game_winner = game_board_status_t::WIN_PTWO;

  return game_winner;
}


game_board_status_t elements_board::_move(int game_player_turn , std::string _seed , std::string _val)
{
  bool res;
  game_board_status_t _win;

  res = is_turn(game_player_turn);
  if(res  == false)
  {
    throw generic_exception("elements_board::_move() " , game_board_err_t::TURN_BAD , false );
  }
  res = check_move(_seed,_val);
  if(res  == false)
  {
    throw generic_exception("elements_board::_move() " , game_board_err_t::MOVE_BAD , false );
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
     if(strcmp(_first.first.c_str() , _seed.c_str()) != 0) /* different seed*/
     {
       /* 1 - AIR - FIRE */
       if((strcmp(_first.first.c_str() , "AIR") == 0) && (strcmp(_seed.c_str() , "FIRE") == 0))
       {
	        _score.first += 2;
       }
       /* 1 -  FIRE - AIR */
       else if((strcmp(_first.first.c_str() , "FIRE") == 0) && (strcmp(_seed.c_str() , "AIR") == 0))
       {
	 _score.second += 2;
       }
       /* 2 - AIR - EARTH */
       else if((strcmp(_first.first.c_str() , "AIR") == 0) && (strcmp(_seed.c_str() , "EARTH") == 0))
       {
	 _score.second += 2;
       }
       /* 2 - EARTH - AIR  */
       else if((strcmp(_first.first.c_str() , "EARTH") == 0) && (strcmp(_seed.c_str() , "AIR") == 0))
       {
	 _score.first += 2;
       }
       /* 3 - AIR - WATER */
       else if((strcmp(_first.first.c_str() , "AIR") == 0) && (strcmp(_seed.c_str() , "WATER") == 0))
       {
	_score.second += 2;
       }
       /* 3 - WATER - AIR  */
       else if((strcmp(_first.first.c_str() , "WATER") == 0) && (strcmp(_seed.c_str() , "AIR") == 0))
       {
	 _score.first += 2;
       }
       /* 4 - FIRE - EARTH */
       else if((strcmp(_first.first.c_str() , "FIRE") == 0) && (strcmp(_seed.c_str() , "EARTH") == 0))
       {
	        _score.first += 2;
       }
       /* 4 - EARTH - FIRE  */
       else if((strcmp(_first.first.c_str() , "EARTH") == 0) && (strcmp(_seed.c_str() , "FIRE") == 0))
       {
	        _score.second += 2;
       }
        /* 5 - FIRE - WATER */
       else if((strcmp(_first.first.c_str() , "FIRE") == 0) && (strcmp(_seed.c_str() , "WATER") == 0))
       {
	 _score.second += 2;
       }
       /* 5 - WATER - FIRE  */
       else if((strcmp(_first.first.c_str() , "WATER") == 0) && (strcmp(_seed.c_str() , "FIRE") == 0))
       {
	 _score.first += 2;
       }
         /* 6 - EARTH - WATER */
       else if((strcmp(_first.first.c_str() , "EARTH") == 0) && (strcmp(_seed.c_str() , "WATER") == 0))
       {
	 _score.first += 2;
       }
       /* 6 - WATER - EARTH  */
       else if((strcmp(_first.first.c_str() , "WATER") == 0) && (strcmp(_seed.c_str() , "EARTH") == 0))
       {
	_score.second += 2;
       }

      _win = _check_win();
      next_turn();
      _check = false;
    }
     else
     {
       if(std::stoi(_first.second) !=  std::stoi(_val)) /* same seed different value*/
       {
	  if(std::stoi(_first.second) >  std::stoi(_val))
	  {
	    next_turn();
	    _check = false;
	    _score.first += 2;
	  }
	  else
	  {
	     next_turn();
	    _check = false;
	   _score.second += 2;
	  }
	  _win = _check_win();
       }
       else /* same seed same value*/
       {
	    next_turn();
	    _check = false;
	    _score.first += 2;
	    _score.second += 2;
	    _win = _check_win();
      }

    }
  }

 return _win;
}
