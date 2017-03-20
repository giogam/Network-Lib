#include "../../../include/game_session.h"


std::string game_session::new_session(game_player *one , game_player *two , std::string _game_type)
{
  int _rand;
  std::string _id;
  std::string _game;
  std::stringstream msg;


    srand ( time(NULL) );
    _rand = rand() % 9000 + 1000;
    _id = std::to_string(_rand);


    _game = (strcmp(_game_type.c_str() , "elements" ) == 0 )? "elements" : "uno";

    two->set_status(true);
    two->set_session(_id);
    two->set_number(1);
    two->set_game(_game);

    _pairsV[_id] = chat_pair(one,two);

    if(strcmp(_game_type.c_str() , "elements" ) == 0 )
    {
      elements_board * new_board = new elements_board(0,0);
      _boardsV[_id] = new_board;
    }
    else
    {
       uno_board * new_board = new uno_board(0,0);
      _boardsV[_id] = new_board;
    }

    if(_logged) game_logger->new_match(_id);

    CLEAR_MSG(msg);
    msg << "START" << " " << _id << " " << one->get_number() << " "<< two->get_nick() << _boardsV[_id]->get_turn() << " " << _game << " " << _boardsV[_id]->get_status();
    game_msg * msg_one = new game_msg(msg.str(),_pairsV[_id].first);
    CLEAR_MSG(msg);
    msg << "START" << " " << _id << " " << two->get_number() << " "<< one->get_nick() << _boardsV[_id]->get_turn() << " " << _game << " " << _boardsV[_id]->get_status();
    game_msg * msg_two = new game_msg(msg.str(),_pairsV[_id].second);

    _sessionV[_id].add(msg_one);
    _sessionV[_id].add(msg_two);

    game_handler *handler = new game_handler(_sessionV[_id]);
    handler->start();
    _handlersV[_id] = handler;

     if(_logged)
    {
      std::stringstream _to_log;
      _to_log  <<  _boardsV[_id]->get_status() << "\n" <<  _boardsV[_id]->get_turn();
      game_logger->write_log( _to_log.str() ,_id);
    }

    return _id;
}


bool game_session::dispatch_move(game_player * from ,std::string _id , std::string _seed , std::string _val)
{
  game_msg  * msg_one = NULL;
  game_msg  * msg_two = NULL;
  game_board_status_t status;
  std::stringstream msg;
  game_player * to  = NULL;
  bool _clean = false;

    to = (from->get_id() == _pairsV[_id].first->get_id()) ? _pairsV[_id].second : _pairsV[_id].first;

    status = _boardsV[_id]->_move(from->get_number(),_seed,_val);

    if(_logged)
    {
      std::stringstream _to_log;
      _to_log <<  _boardsV[_id]->get_status() << "\n" <<  _boardsV[_id]->get_turn() << "\n" <<  _seed << "\n" <<  _val ;
      game_logger->write_log( _to_log.str() ,_id);
    }
    switch(status)
    {
      case game_board_status_t::CORRECT_MOVE:
        CLEAR_MSG(msg);
        msg << "DRAW" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_one = new game_msg(msg.str(),from);
        msg_two = new game_msg(msg.str(),to);
        _sessionV[_id].add(msg_one);
        _sessionV[_id].add(msg_two);
        break;
      case game_board_status_t::WIN_PONE:
        std::cout << "WIN_PONE" << "\n";
        CLEAR_MSG(msg);
        msg << "WIN" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_one = new game_msg(msg.str(),to);
        _sessionV[_id].add(msg_one);
        CLEAR_MSG(msg);
        msg << "LOOSE" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_two = new game_msg(msg.str(),from);
        _sessionV[_id].add(msg_two);
        _chart->set_points(10,to->get_nick());
	      _chart->set_points(0,from->get_nick());
        if(_logged)
        {
          std::stringstream _to_log;
          _to_log <<  get_chart() << "\n";
          chart_logger->write_log( _to_log.str());
        }
        close_full_session(from,_id);
	_clean = true;
        break;
      case game_board_status_t::WIN_PTWO:
        std::cout << "WIN_PTWO" << "\n";
        CLEAR_MSG(msg);
        msg << "LOOSE" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_one = new game_msg(msg.str(),to);
        _sessionV[_id].add(msg_one);
        CLEAR_MSG(msg);
        msg << "WIN" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_two = new game_msg(msg.str(),from);
        _sessionV[_id].add(msg_two);
        _chart->set_points(10,from->get_nick());
	      _chart->set_points(0,to->get_nick());
        if(_logged)
        {
          std::stringstream _to_log;
          _to_log <<  get_chart() << "\n";
          chart_logger->write_log( _to_log.str());
        }
        close_full_session(to,_id);
	_clean = true;
        break;
       case game_board_status_t::WIN_ODD:
        CLEAR_MSG(msg);
        msg << "ODD" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_one = new game_msg(msg.str(),from);
        _sessionV[_id].add(msg_one);
        CLEAR_MSG(msg);
        msg << "ODD" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_two = new game_msg(msg.str(),to);
        _sessionV[_id].add(msg_two);
        _chart->set_points(10,from->get_nick());
        _chart->set_points(10,to->get_nick());
        if(_logged)
        {
          std::stringstream _to_log;
          _to_log <<  get_chart() << "\n";
          chart_logger->write_log( _to_log.str());
        }
        close_full_session(to,_id);
	_clean = true;
        break;
      }

  return _clean;
}

void game_session::close_half_session(game_player * a ,std::string _id)
{
  game_msg * msg_one =   NULL;
  game_msg * msg_two = 	 NULL;
  game_player * b  = NULL;
  std::stringstream _msg;

    b = (a->get_id() == _pairsV[_id].first->get_id()) ? _pairsV[_id].second : _pairsV[_id].first;

    a->set_status(false);
    b->set_status(false);

    CLEAR_MSG(_msg);
    _msg << "ACK" << " " << "EXIT" << " " << _id << "\n";
    msg_one = new game_msg(_msg.str(),b);
    CLEAR_MSG(_msg);
    _msg << "EXIT";
    msg_two = new game_msg(_msg.str(),NULL);

    _sessionV[_id].add(msg_one );
    _sessionV[_id].add(msg_two );

    _closedV.push_back(_id);
}

/* game_player * a will always be the one who wants to close the session  */
void game_session::close_full_session(game_player * a ,std::string _id)
{
  game_msg * msg_one =   NULL;
  game_msg * msg_two = 	 NULL;
  game_msg * msg_three = NULL;
  game_player * b  = NULL;
  std::stringstream _msg;

    b = (a->get_id() == _pairsV[_id].first->get_id()) ? _pairsV[_id].second : _pairsV[_id].first;

    a->set_status(false);
    b->set_status(false);

    CLEAR_MSG(_msg);
    _msg << "ACK" << " " << "EXIT" << " " << _id << "\n";
    msg_one = new game_msg(_msg.str(),a);
    msg_two = new game_msg(_msg.str(),b);
    CLEAR_MSG(_msg);
    _msg << "EXIT";
    msg_three = new game_msg(_msg.str(),NULL);

    _sessionV[_id].add(msg_one );
    _sessionV[_id].add(msg_two );
    _sessionV[_id].add(msg_three );

    _closedV.push_back(_id);
}

std::string game_session::resume_session( game_player *one , game_player *two , std::string _resume_id, std::string _game_type )
{
  int _rand;
  std::string _id;
  std::fstream _matchFile;
  std::string _line;
  std::stringstream _path;
  std::vector<std::string> _matchInfo;
  std::stringstream msg;
  std::string _game;

  _game = (strcmp(_game_type.c_str() , "elements" ) == 0 )? "elements" : "uno";

  _path << "./resources/matches/" << _resume_id << ".match";

  _matchFile.open(_path.str());

    while(std::getline(_matchFile, _line))
    {
      if(!_line.empty())
      _matchInfo.push_back(_line);
    }

    srand ( time(NULL) );
    _rand = rand() % 9000 + 1000;
    _id = std::to_string(_rand);

    one->set_status(true);
    one->set_session(_id);
    one->set_game(_game);

    two->set_status(true);
    two->set_session(_id);
    two->set_game(_game);

    one->set_number(0);
    two->set_number(1);

    _pairsV[_id] = chat_pair(one,two);

     if(strcmp(_game_type.c_str() , "elements" ) == 0 )
    {
      int _turn = std::stoi(_matchInfo[2]);
      elements_board * new_board = new elements_board(0,0);
      new_board->set_status(_matchInfo[0],_matchInfo[1]);
      std::cout << "P_TURN: " << _matchInfo[2]  << "\n";
      new_board->set_turn(_turn);
      if(_turn == 1)
      {
        new_board->set_move(_matchInfo[3],_matchInfo[4]);
        new_board->set_check(true);
      }
      _boardsV[_id] = new_board;
    }
    else
    {
      int _turn = std::stoi(_matchInfo[2]);
      uno_board * new_board = new uno_board(0,0);
      new_board->set_status(_matchInfo[0],_matchInfo[1]);
      std::cout << "P_TURN: " << _matchInfo[2]  << "\n";
      new_board->set_turn(_turn);
      if(_turn == 1)
      {
        new_board->set_move(_matchInfo[3],_matchInfo[4]);
        new_board->set_check(true);
      }
      _boardsV[_id] = new_board;
    }

    if(_logged) game_logger->new_match(_id);

    CLEAR_MSG(msg);
    msg << "RESTART" << " " << _id << " " << one->get_number() << " "<< two->get_nick() << " " << _boardsV[_id]->get_turn() << " " << _game << " " << _boardsV[_id]->get_status();
    game_msg * msg_one = new game_msg(msg.str(),_pairsV[_id].first);
    CLEAR_MSG(msg);
    msg << "RESTART" << " " << _id << " " << two->get_number() << " "<< one->get_nick() << " " << _boardsV[_id]->get_turn() << " " << _game << " " << _boardsV[_id]->get_status();
    game_msg * msg_two = new game_msg(msg.str(),_pairsV[_id].second);

    _sessionV[_id].add(msg_one);
    _sessionV[_id].add(msg_two);

    game_handler *handler = new game_handler(_sessionV[_id]);
    handler->start();
    _handlersV[_id] = handler;

     if(_logged)
    {
      std::stringstream _to_log;
      int _turn = _boardsV[_id]->get_turn();
      _to_log <<  _boardsV[_id]->get_status() << "\n" <<  _boardsV[_id]->get_turn();
      /* succede solo nel caso in cui sia stata effettuata almeno una mossa*/
      if(_turn == 1 )
        _to_log  << "\n" << _matchInfo[3] << "\n" << _matchInfo[4] ;
      game_logger->write_log( _to_log.str() ,_id);
    }

    return _id;

}

void game_session::do_clean()
{
      for(auto itV = _closedV.begin(); itV != _closedV.end(); ++itV )
      {
        std::cout << "Cleaning Up: " << (*itV) << "\n";
        auto _pair    = _pairsV.find(*itV);
        auto _handle  = _handlersV.find(*itV);
        auto _session = _sessionV.find(*itV);
        auto _board   = _boardsV.find(*itV);

        _handle->second->join();
        _handlersV.erase(_handle);
        _sessionV.erase(_session);
        _pairsV.erase(_pair);
        _boardsV.erase(_board);
      }
      _closedV.clear();
}
