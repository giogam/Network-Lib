#include "../../../include/ttt_handler.h"


void * ttt_handler::run()
{
  ttt_msg * msg;
  std::string message;

  std::cout << "[+] Thread ttt_handler handler running " << self() << "\n";
  for(int i = 0; ;i++)
  {
    msg = _msg_pool.remove();

    message = msg->get_message();
    ttt_player * user = msg->get_dest();

    if(strcmp(message.c_str(),"EXIT") == 0)
    {
	std::cout << "[-] Thread ttt_handler " << self() << " terminating execution "<< "\n";
	pthread_exit(NULL);
    }
    else
      user->send_message(message);
   }

   return NULL;
}
