#ifndef _game_handler_h
#define _game_handler_h

#include "lib_base.h"
#include "thread.h"
#include "item_queue.h"
#include "game_msg.h"


class game_handler : public thread
{

  public:
    game_handler(item_queue<game_msg *>& msg_pool)
    :thread(),
    _msg_pool(msg_pool){  };

    virtual ~game_handler() { };

    void * run();

  private:
  item_queue<game_msg *>& _msg_pool;
};

#endif /* _game_handler_h */
