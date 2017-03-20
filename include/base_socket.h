#ifndef _base_socket_h
#define _base_socket_h

#include "lib_base.h"
#include "ip_address.h"

class base_socket
{
    public:

      base_socket( int family  , int type , int protocol );
     ~base_socket() { ::close(id_); }

        bool bind( const ip_address & adr );
        bool connect(const ip_address & adr );
        bool listen( int max_listen_ = 5);
        bool accept( base_socket & conn_sk);
        bool accept( base_socket * conn_sk );

        ssize_t sendto(const void * data  , size_t data_size , const ip_address & adr);
        ssize_t recvfrom(void * data  , size_t data_size , ip_address & adr);

        ssize_t send(const void * data , size_t data_size);
        ssize_t recv(const void * data , size_t data_size);



        bool set_option(int level, int option_name, void *option_value , size_t option_len);

        static std::string random_port();

        template<typename T>
        static bool sock_compare(T *& a, T *& b)
        {
          return (a->get_id() < b->get_id());
        }

        inline bool close()
        {
          ::close(id_);
          return true;
        }


        inline std::string get_ip( )
        {
          return ip_.to_string();
        }

        inline ip_address::port_t get_port()
        {
          return ip_.get_port();
        }

        inline int get_id()
        {
          return id_;
        }

    private:
        int id_;
        ip_address ip_;
};


#endif /* _base_socket_h */
