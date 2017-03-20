#include "../../../include/chat_session.h"


std::string chat_session::new_session(chat_user *one , chat_user *two)
{

  int _rand;
  std::string _id;
  std::stringstream msg;


    srand ( time(NULL) );
    _rand = rand() % 9000 + 1000;
    _id = std::to_string(_rand);

    two->set_status(true);
    two->set_session(_id);

    _pairsV[_id] = chat_pair(one,two);

    CLEAR_MSG(msg);
    msg << "START" << " " << _id << " "<< two->get_nick() << "\n";
    chat_msg * msg_one = new chat_msg(msg.str(),_pairsV[_id].first);
    CLEAR_MSG(msg);
    msg << "START" << " " << _id << " "<< one->get_nick() << "\n";
    chat_msg * msg_two = new chat_msg(msg.str(),_pairsV[_id].second);

    _sessionV[_id].add(msg_one);
    _sessionV[_id].add(msg_two);

    chat_handler *handler = new chat_handler(_sessionV[_id]);
    handler->start();
    _handlersV[_id] = handler;

    return _id;
}


void chat_session::dispatch_msg(chat_user * from ,std::string _id , std::string _msg)
{
  chat_msg  * msg_one = NULL;
  chat_user * to  = NULL;
  std::stringstream msg;

    to = (from->get_id() == _pairsV[_id].first->get_id()) ? _pairsV[_id].second : _pairsV[_id].first;

    CLEAR_MSG(msg);
    msg << "MSG" << " " << _msg << "\n";
    msg_one = new chat_msg(msg.str(),to);
    _sessionV[_id].add(msg_one);
}

void chat_session::close_half_session(chat_user * a ,std::string _id)
{
  chat_msg * msg_one =   NULL;
  chat_msg * msg_two = 	 NULL;
  chat_user * b  = NULL;
  std::stringstream _msg;

    b = (a->get_id() == _pairsV[_id].first->get_id()) ? _pairsV[_id].second : _pairsV[_id].first;

    a->set_status(false);
    b->set_status(false);

    CLEAR_MSG(_msg);
    _msg << "ACK" << " " << "EXIT" << " " << _id << "\n";
    msg_one = new chat_msg(_msg.str(),b);
    CLEAR_MSG(_msg);
    _msg << "EXIT";
    msg_two = new chat_msg(_msg.str(),NULL);

    _sessionV[_id].add(msg_one );
    _sessionV[_id].add(msg_two );

    _closedV.push_back(_id);
}

/* chat_user * a will always be the one who wants to close the session  */
void chat_session::close_full_session(chat_user * a ,std::string _id)
{
  chat_msg * msg_one =   NULL;
  chat_msg * msg_two = 	 NULL;
  chat_msg * msg_three = NULL;
  chat_user * b  = NULL;
  std::stringstream _msg;

    b = (a->get_id() == _pairsV[_id].first->get_id()) ? _pairsV[_id].second : _pairsV[_id].first;

    a->set_status(false);
    b->set_status(false);

    CLEAR_MSG(_msg);
    _msg << "ACK" << " " << "EXIT" << " " << _id << "\n";
    msg_one = new chat_msg(_msg.str(),a);
    msg_two = new chat_msg(_msg.str(),b);
    CLEAR_MSG(_msg);
    _msg << "EXIT";
    msg_three = new chat_msg(_msg.str(),NULL);

    _sessionV[_id].add(msg_one );
    _sessionV[_id].add(msg_two );
    _sessionV[_id].add(msg_three );

    _closedV.push_back(_id);
}

void chat_session::do_clean()
{
      for(auto itV = _closedV.begin(); itV != _closedV.end(); ++itV )
      {
        std::cout << "Cleaning Up: " << (*itV) << "\n";
        auto _pair    = _pairsV.find(*itV);
        auto _handle  = _handlersV.find(*itV);
        auto _session = _sessionV.find(*itV);

        _handle->second->join();
        _handlersV.erase(_handle);
        _sessionV.erase(_session);
        _pairsV.erase(_pair);
      }
      _closedV.clear();
}
