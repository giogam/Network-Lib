#include "../../../include/game_board.h"


game_board::game_board(int score_a , int score_b)
{
  _turn = 0;
  _check = false;
  _score = std::make_pair<int &,int &>(score_a,score_b);
  _first = std::make_pair<std::string,std::string>(" "," ");
}


std::string game_board::get_status()
{
  std::stringstream _ss;

    _ss << _score.first << "\n" << _score.second << "\n";

    return _ss.str();

}

void game_board::set_status(std::string score_a , std::string score_b )
{
 _score = std::make_pair<int,int>(std::stoi(score_a),std::stoi(score_b));
}

void game_board::set_turn(int turn)
{
 _turn = turn ;
}

void game_board::next_turn()
{
 _turn = ((_turn + 1) % 2);
}

int game_board::get_turn()
{
 return _turn;
}

bool game_board::is_turn( int game_player_turn )
{
 return (game_player_turn == _turn) ? true : false;
}

void game_board::set_check( bool val )
{
  _check = val;
}

void game_board::set_move(std::string _seed, std::string _val)
{
  _first.first = _seed;
  _first.second = _val;
}
