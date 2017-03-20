#ifndef _user_manager_h
#define _user_manager_h

#include "lib_base.h"
#include "chat_user.h"

template <typename T>
class user_manager
{

public:

  user_manager(std::vector< T * >& usersV) : _usersV(usersV) {}
  virtual ~user_manager() { }

  std::string get_list()
  {
      std::stringstream list;

      for(auto user = _usersV.begin(); user != _usersV.end(); ++user )
      {
        list << (*user)->get_nick() << "\n";
      }

      return list.str();
  }

  std::string get_ip_list()
  {
      std::stringstream list;

      for(auto user = _usersV.begin(); user != _usersV.end(); ++user )
      {
        list << (*user)->get_ip() << " " << (*user)->get_ctrl() << "\n";
      }

      return list.str();
  }

  void check_nick(std::string nickname)
  {
    for(auto user = _usersV.begin(); user != _usersV.end(); ++user )
    {
      if(strcmp(((*user)->get_nick().c_str()) ,(nickname.c_str())) == 0 )
      {
        throw generic_exception("user_manager::check_nickname() " ,user_status_t::TAKEN , true );
      }
    }
  }


  void check_user(std::string nickname)
  {
    for(auto user = _usersV.begin(); user != _usersV.end(); ++user )
    {
      if(strcmp(((*user)->get_nick().c_str()) ,(nickname.c_str())) == 0 )
      {
        if((*user)->is_busy())
        {
          throw generic_exception("user_manager::check_user() " ,user_status_t::BUSY , true );
        }
      }
    }
  }

  T* get_user(std::string nickname)
  {
    for(auto user = _usersV.begin(); user != _usersV.end(); ++user )
    {
      if(strcmp(((*user)->get_nick().c_str()) ,(nickname.c_str())) == 0 )
      {
        return (*user);
      }
    }

    throw generic_exception("user_manager::get_user() " ,user_status_t::NONEX , true );

    return NULL;
  }



private:
  std::vector< T *> & _usersV;

};

#endif /* _chat_user_h */
