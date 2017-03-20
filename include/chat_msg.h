#ifndef _chat_msg_h
#define _chat_msg_h

#include "lib_base.h"
#include "chat_user.h"

class chat_msg
{
  public:

    chat_msg(std::string msg, chat_user * user_to  )
      : _msg(msg), _user_to(user_to) { }
    virtual ~chat_msg( ) { delete _user_to;};

    std::string get_message()
    {
      return _msg;
    }

    chat_user * get_dest()
    {
      return _user_to;
    }

  private:
    std::string _msg;
    chat_user * _user_to;

};

#endif /* _chat_msg_h */
