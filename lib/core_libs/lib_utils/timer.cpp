#include "../../../include/timer.h"


void timer::start()
{
  starTime = std::chrono::high_resolution_clock::now();
}

long int timer::get_time()
{
  std::chrono::time_point<std::chrono::high_resolution_clock> thisTime;

    thisTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(thisTime - starTime).count();
}
