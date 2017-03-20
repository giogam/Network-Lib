#ifndef _timer_h
#define _timer_h

#include "lib_base.h"

class timer
{
  public:

    void start();
    long int get_time();

  private:

    std::chrono::time_point<std::chrono::high_resolution_clock> starTime;
};

#endif /* _timer_h */
