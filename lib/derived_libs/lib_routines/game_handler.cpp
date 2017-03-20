#include "../../../include/game_handler.h"


void * game_handler::run()
{
  game_msg * msg;
  std::string message;

   std::cout << "[+] Thread game_handler running " << self() << "\n";
  for(int i = 0; ;i++)
  {
    msg = _msg_pool.remove();

    message = msg->get_message();
    game_player * user = msg->get_dest();

    if(strcmp(message.c_str(),"EXIT") == 0)
    {
	std::cout << "[-] Thread game_handler " << self() << " terminating execution "<< "\n";
	pthread_exit(NULL);
    }
    else
      user->send_message(message);
   }

   return NULL;
}
