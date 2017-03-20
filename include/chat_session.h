#ifndef _chat_session_h
#define _chat_session_h

#include "lib_base.h"
#include "item_queue.h"
#include "chat_user.h"
#include "chat_msg.h"
#include "chat_handler.h"
#include "thread.h"

class chat_session
{
  public:
    chat_session() { };
    ~chat_session() { };

    std::string new_session(chat_user *one , chat_user *two);
    void dispatch_msg(chat_user * from ,std::string _id , std::string _msg);
    void close_full_session(chat_user * a, std::string _id );
    void close_half_session(chat_user * a, std::string _id );
    void do_clean();

    typedef std::pair<chat_user* , chat_user*> chat_pair;

  private:
    std::map<std::string , chat_pair> _pairsV;
    std::vector<std::string> 			 _closedV;
    std::map<std::string,thread *>  		 _handlersV;
    std::map<std::string ,item_queue<chat_msg*>> _sessionV;
};



#endif /* _chat_session_h*/
