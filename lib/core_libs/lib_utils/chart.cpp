#include "../../../include/chart.h"


bool chart::in_chart(std::string _player)
{
  bool _res = false;

  for (auto& k : _chart)
  {
    if(strcmp(_player.c_str(),k.first.c_str()) == 0)
    {
      _res = true;
      break;
    }
  }

  return _res;

}

std::string chart::get_chart()
{
  std::stringstream _res;

  for (auto& k : _chart)
  {
    _res << k.first << " " << k.second << "\n";
  }

  return _res.str();
}


void chart::set_points(int _points , std::string _user)
{
    _chart[_user] += _points;
}
