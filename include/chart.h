#ifndef _chart_h
#define _chart_h

#include "lib_base.h"

class chart
{
  public:

    chart() { };
   ~chart() { };

    bool in_chart(std::string _player);
    void set_points(int _points , std::string _user);
    std::string get_chart();



  private:
    std::map<std::string, int> _chart;
};


#endif /* _chart_h */ 
