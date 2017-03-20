#ifndef _chat_handler_h
#define _chat_handler_h

#include "lib_base.h"
#include "thread.h"
#include "item_queue.h"
#include "chat_msg.h"


class chat_handler : public thread
{

  public:
    chat_handler(item_queue<chat_msg *>& msg_pool)
    :thread(),
    _msg_pool(msg_pool){  };

    virtual ~chat_handler() { };

    void * run();

  private:
  item_queue<chat_msg *>& _msg_pool;
};

#endif /* _chat_handler_h */
