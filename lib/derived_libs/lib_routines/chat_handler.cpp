#include "../../../include/chat_handler.h"


void * chat_handler::run()
{
  chat_msg * msg;
  std::string message;

   std::cout << "[+] Thread chat_handler running " << self() << "\n";
  for(int i = 0; ;i++)
  {
    msg = _msg_pool.remove();

    message = msg->get_message();
    chat_user * user = msg->get_dest();

    if(strcmp(message.c_str(),"EXIT") == 0)
    {
	std::cout << "[-] Thread chat_handler " << self() << " terminating execution "<< "\n";
	pthread_exit(NULL);
    }
    else
      {
        std::cout << "message from" << user->get_nick() << " " << message << "\n";
        user->send_message(message);
      }
   }

   return NULL;
}
