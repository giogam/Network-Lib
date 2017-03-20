#include "../../../include/ttt_session.h"


std::string ttt_session::new_ttt_session(ttt_player *one , ttt_player *two)
{
  int _rand;
  std::string _id;
  std::stringstream msg;


    srand ( time(NULL) );
    _rand = rand() % 9000 + 1000;
    _id = std::to_string(_rand);

    two->set_status(true);
    two->set_session(_id);
    two->set_number(1);

    _pairsV[_id] = chat_pair(one,two);

    ttt_board * new_board = new ttt_board();
    _boardsV[_id] = new_board;

    if(_logged) game_logger->new_match(_id);

    CLEAR_MSG(msg);
    msg << "START" << " " << _id << " " << one->get_number() << " "<< two->get_nick() << " " << _boardsV[_id]->get_status();
    ttt_msg * msg_one = new ttt_msg(msg.str(),_pairsV[_id].first);
    CLEAR_MSG(msg);
    msg << "START" << " " << _id << " " << two->get_number() << " "<< one->get_nick() << " " << _boardsV[_id]->get_status();
    ttt_msg * msg_two = new ttt_msg(msg.str(),_pairsV[_id].second);

    _sessionV[_id].add(msg_one);
    _sessionV[_id].add(msg_two);

    ttt_handler *handler = new ttt_handler(_sessionV[_id]);
    handler->start();
    _handlersV[_id] = handler;

     if(_logged)
    {
      std::stringstream _to_log;
      _to_log <<  _boardsV[_id]->get_status() << "\n" <<  _boardsV[_id]->get_turn();
      game_logger->write_log( _to_log.str() ,_id);
    }

    return _id;
}


bool ttt_session::dispatch_ttt_move(ttt_player * from ,std::string _id , std::string _row,std::string _col)
{
  ttt_msg  * msg_one = NULL;
  ttt_msg  * msg_two = NULL;
  ttt_board_status_t status;
  std::stringstream msg;
  ttt_player * to  = NULL;
  bool _clean = false;

    to = (from->get_id() == _pairsV[_id].first->get_id()) ? _pairsV[_id].second : _pairsV[_id].first;

    status = _boardsV[_id]->_move(from->get_number(),_row,_col);

    if(_logged)
    {
      std::stringstream _to_log;
      _to_log <<  _boardsV[_id]->get_status() << "\n" <<  _boardsV[_id]->get_turn();
      game_logger->write_log( _to_log.str() ,_id);
    }
      switch(status)
    {
      case ttt_board_status_t::GOOD_MOVE:
        CLEAR_MSG(msg);
        msg << "DRAW" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_one = new ttt_msg(msg.str(),from);
        msg_two = new ttt_msg(msg.str(),to);
        _sessionV[_id].add(msg_one);
        _sessionV[_id].add(msg_two);
        break;
      case ttt_board_status_t::PONE_WIN:
        CLEAR_MSG(msg);
        msg << "WIN" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_one = new ttt_msg(msg.str(),from);
        _sessionV[_id].add(msg_one);
        CLEAR_MSG(msg);
        msg << "LOOSE" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_two = new ttt_msg(msg.str(),to);
        _sessionV[_id].add(msg_two);
        close_full_ttt_session(from,_id);
	_clean = true;
        break;
      case ttt_board_status_t::PTWO_WIN:
        CLEAR_MSG(msg);
        msg << "LOOSE" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_one = new ttt_msg(msg.str(),to);
        _sessionV[_id].add(msg_one);
        CLEAR_MSG(msg);
        msg << "WIN" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_two = new ttt_msg(msg.str(),from);
        _sessionV[_id].add(msg_two);
        close_full_ttt_session(from,_id);
	_clean = true;
        break;
       case ttt_board_status_t::TTT_ODD:
        CLEAR_MSG(msg);
        msg << "ODD" << " " <<  _boardsV[_id]->get_status() << "\n";
        msg_one = new ttt_msg(msg.str(),from);
        msg_two = new ttt_msg(msg.str(),to);
        _sessionV[_id].add(msg_one);
        _sessionV[_id].add(msg_two);
        close_full_ttt_session(from,_id);
	_clean = true;
        break;
        //case ttt_board_status_t::NO_WIN:
      default:
        break;
      }

  return _clean;
}

void ttt_session::close_half_ttt_session(ttt_player * a ,std::string _id)
{
  ttt_msg * msg_one =   NULL;
  ttt_msg * msg_two = 	 NULL;
  ttt_player * b  = NULL;
  std::stringstream _msg;

    b = (a->get_id() == _pairsV[_id].first->get_id()) ? _pairsV[_id].second : _pairsV[_id].first;

    a->set_status(false);
    b->set_status(false);

    CLEAR_MSG(_msg);
    _msg << "ACK" << " " << "EXIT" << " " << _id << "\n";
    msg_one = new ttt_msg(_msg.str(),b);
    CLEAR_MSG(_msg);
    _msg << "EXIT";
    msg_two = new ttt_msg(_msg.str(),NULL);

    _sessionV[_id].add(msg_one );
    _sessionV[_id].add(msg_two );

    _closedV.push_back(_id);
}

/* ttt_player * a will always be the one who wants to close the session  */
void ttt_session::close_full_ttt_session(ttt_player * a ,std::string _id)
{
  ttt_msg * msg_one =   NULL;
  ttt_msg * msg_two = 	 NULL;
  ttt_msg * msg_three = NULL;
  ttt_player * b  = NULL;
  std::stringstream _msg;

    b = (a->get_id() == _pairsV[_id].first->get_id()) ? _pairsV[_id].second : _pairsV[_id].first;

    a->set_status(false);
    b->set_status(false);

    CLEAR_MSG(_msg);
    _msg << "ACK" << " " << "EXIT" << " " << _id << "\n";
    msg_one = new ttt_msg(_msg.str(),a);
    msg_two = new ttt_msg(_msg.str(),b);
    CLEAR_MSG(_msg);
    _msg << "EXIT";
    msg_three = new ttt_msg(_msg.str(),NULL);

    _sessionV[_id].add(msg_one );
    _sessionV[_id].add(msg_two );
    _sessionV[_id].add(msg_three );

    _closedV.push_back(_id);
}

std::string ttt_session::resume_ttt_session( ttt_player *one , ttt_player *two , std::string _resume_id , std::string _player_num )
{
  int _rand;
  std::string _id;
  std::fstream _matchFile;
  std::string _line;
  std::stringstream _path;
  std::vector<std::string> _matchInfo;
  std::stringstream msg;

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


    two->set_status(true);
    two->set_session(_id);

    one->set_number(0);
    two->set_number(1);

    _pairsV[_id] = chat_pair(one,two);

    ttt_board * new_board = new ttt_board();
    new_board->set_status(_matchInfo[0]);
    new_board->set_turn(std::stoi(_matchInfo[1]));
    _boardsV[_id] = new_board;

    if(_logged) game_logger->new_match(_id);

    CLEAR_MSG(msg);
    msg << "RESTART" << " " << _id << " " << one->get_number() << " "<< two->get_nick() << " " << _boardsV[_id]->get_turn() << " " << _boardsV[_id]->get_status();
    ttt_msg * msg_one = new ttt_msg(msg.str(),_pairsV[_id].first);
    CLEAR_MSG(msg);
    msg << "RESTART" << " " << _id << " " << two->get_number() << " "<< one->get_nick() << " " << _boardsV[_id]->get_turn() << " " << _boardsV[_id]->get_status();
    ttt_msg * msg_two = new ttt_msg(msg.str(),_pairsV[_id].second);

    _sessionV[_id].add(msg_one);
    _sessionV[_id].add(msg_two);

    ttt_handler *handler = new ttt_handler(_sessionV[_id]);
    handler->start();
    _handlersV[_id] = handler;

     if(_logged)
    {
      std::stringstream _to_log;
      _to_log <<  _boardsV[_id]->get_status() << "\n" <<  _boardsV[_id]->get_turn();
      game_logger->write_log( _to_log.str() ,_id);
    }

      std::cout << "Board "<< _matchInfo[0] << "\n";
      std::cout << "Turn " <<_matchInfo[1] << "\n";

    return _id;

}

void ttt_session::do_ttt_clean()
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
