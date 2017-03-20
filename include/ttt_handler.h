#ifndef _ttt_handler_h
#define _ttt_handler_h

#include "lib_base.h"
#include "thread.h"
#include "item_queue.h"
#include "ttt_msg.h"


class ttt_handler : public thread
{

  public:
    ttt_handler(item_queue<ttt_msg *>& msg_pool)
    :thread(),
    _msg_pool(msg_pool){  };

    virtual ~ttt_handler() { };

    void * run();

  private:
  item_queue<ttt_msg *>& _msg_pool;
};

#endif /* _ttt_handler_h */
