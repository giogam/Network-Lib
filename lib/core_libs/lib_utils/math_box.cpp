#include "../../../include/math_box.h"


int math_box::calc_expr( const char * expression)
{
  int res = 0;
  std::string expr(expression);
  std::stringstream ss(expr);
  std::string buf;
  std::vector<std::string> tokens;
  
     /* tokenizzo la stringa */
    while (ss >> buf) tokens.push_back(buf);

    if(tokens[2] == "+")
        res = (std::stoi(tokens[0]) + std::stoi(tokens[1]));

    else if(tokens[2] == "-")
        res = (std::stoi(tokens[0]) - std::stoi(tokens[1]));

    else if(tokens[2] == "*")
        res = (std::stoi(tokens[0]) * std::stoi(tokens[1]));

    else if(tokens[2] == "/")
        res = (std::stoi(tokens[0]) / std::stoi(tokens[1]));

    else if(tokens[2] == "%")
        res = (std::stoi(tokens[0]) % std::stoi(tokens[1]));


  return res;
}
