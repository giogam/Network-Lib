#ifndef _ip_address_h
#define _ip_address_h

#include "lib_base.h"

class ip_address
{
    public:

        typedef sockaddr_storage     address_t;
        typedef socklen_t            addrlen_t;
        typedef in_port_t            port_t;


    /* Constructors -------------------------------------------------------------------*/

        /* Inline Constructors --------------------------------------------------------*/

        /**
         * Standard constructor , blank address
         */
        inline ip_address() : size_(0)
      { clear_addr(); }

        /**
         * Copy constructor
         * @param const ip_address & ip_addr
         */
        inline ip_address( const ip_address & ip_addr) : address_(ip_addr.address_) , size_(ip_addr.size_) {  }

        /**
         * Costructor from local address
         * @param address_t addr
         */
        inline ip_address(address_t addr) : address_(addr) , size_(0)
            {
                if(address_.ss_family == AF_INET) size_ = sizeof( struct sockaddr_in);
                if(address_.ss_family == AF_INET6) size_ = sizeof( struct sockaddr_in6);
            }

        /* Normal constructors --------------------------------------------------------*/

        /**
         * Constructor from dotted string std::string version can handle different formats
         * @param const std::string & s
         */
        ip_address( const std::string & s , const std::string & p = "0" , int ai_family= AF_INET , int ai_socktype= SOCK_STREAM, int ai_flags= 0,  const std::string & i = "eth0" )
            {  parse_addr(s , p , i , ai_family ,ai_socktype ,ai_flags); }

        /**
         * Constructor from dotted string char * version can handle different formats
         * @param const char * s
         */
        ip_address( const char * s , const char * p = "0" ,int ai_family= AF_INET , int ai_socktype= SOCK_STREAM, int ai_flags =0,const char * i = "eth0")
            {  parse_addr(s , p, i , ai_family ,ai_socktype ,ai_flags); }

        /**
         * Constructor from ipv4 address type
         * @param const struct sockaddr_in sa
         */
        ip_address(const struct sockaddr_in sa)
            { operator =(sa); }

        /**
         * Constructor from ipv6 address type
         * @param const struct sockaddr_in6 sa
         */
        ip_address(const struct sockaddr_in6 sa)
            { operator =(sa); }


    /* Destructor -------------------------------------------------------------------*/

        /**
         * Standard destructor
         */
        virtual ~ip_address()
            { }

    /* Public Methods ----------------------------------------------------------------*/

        /* Inline methods ------------------------------------------------------------*/

        /**
         *  Invalidates the address
         */
        inline void clear_addr()
            {
                memset(&address_,0,sizeof(address_t));
                size_ = 0;
            }

        /**
         * Copy assignment
         * @param const ip_address & ip_addr
         */
        inline void operator=( const ip_address & ip_addr) throw()
            {
                address_ = ip_addr.address_;
                size_ = ip_addr.size_;
            }

        /**
         *  Copy assignment IPV4 version
         *  @param const sockaddr_in & ip_addr
         */
        inline void operator=( const sockaddr_in & ip_addr ) throw()
            {
                *((sockaddr_in *)&address_) = ip_addr;
                address_.ss_family = AF_INET;
                size_ = sizeof(sockaddr_in);
            }

        /**
         * Copy assignment IPV6 version
         * @param const sockaddr_in6 & ip_addr
         */
        inline void operator=( const sockaddr_in6 & ip_addr ) throw()
            {
                *((sockaddr_in6 *)&address_) = ip_addr;
                address_.ss_family = AF_INET6;
                size_ = sizeof(sockaddr_in6);
            }

        /**
         * Tests if address is valid
         * @return bool
         */
        inline bool is_valid() const throw()
            {
                return address_.ss_family != 0 && size_ != 0;
            }

        /**
         * Tests if address is IPV4
         * @return bool
         */
        inline bool is_v4() const throw()
            {
                return address_.ss_family == AF_INET;
            }

        /**
         * Tests if address is IPV6
         * @return bool
         */
        inline bool is_v6() const throw()
            {
                return address_.ss_family == AF_INET6;
            }

        /**
         * Return address sockaddr_storage type
         * @return address_t
         */
        inline const address_t & get_address() const throw()
            {
                return address_;
            }

        /**
         * Return address family
         * @return sa_family_t
         */
        inline sa_family_t get_family() const throw()
            {
                return address_.ss_family;
            }

        /**
         * Sets address family
         * @param sa_family_t family_
         */
        inline void set_family(sa_family_t family_) throw()
            {
                address_.ss_family = family_;
            }
        /**
        * Return address size
        * @return socklen_t
        */
        inline socklen_t get_size() const throw()
            {
                return size_;
            }

        /**
         * Set size of the address_data structure
         * @param socklen_t size
         */
        inline void set_size(socklen_t size) throw()
            {
                size_ = size;
            }

        /**
         * Return port only for AF_INET and AF_INET6
         * @return port_t
         */
        inline port_t get_port() const throw()
            {
                if(address_.ss_family == AF_INET) return ntohs(((sockaddr_in*)&address_)->sin_port);
                if(address_.ss_family == AF_INET6) return ntohs(((sockaddr_in6*)&address_)->sin6_port);
                return 0;
            }

        /**
        * Set port only for AF_INET and AF_INET6
        * @return bool
        */
        inline bool set_port(port_t port_) throw()
            {
              if(address_.ss_family == AF_INET)
              {
                  ((sockaddr_in*)&address_)->sin_port = htons(port_);
                  return true;
              }
              else if(address_.ss_family == AF_INET6)
              {
                  ((sockaddr_in6*)&address_)->sin6_port = htons(port_);
                  return true;
              }
            return false;
            }

        /**
         * Type cast to sockaddr_in
         * @return const struct sockaddr_in & ()
         */
        inline operator const struct sockaddr_in & () const throw()
            {
                return *((sockaddr_in*) &address_);
            }

        /**
         * Type cast to sockaddr_in6
         * @return const struct sockaddr_in6 & ()
         */
        inline operator const struct sockaddr_in6 & () const throw()
        {
            return *((sockaddr_in6*) &address_);
        }

        /**
        * Type cast to sockaddr_in6
        * @param const address_t addr_
        */
        inline void reset_address(const address_t addr_)
            {
                clear_addr();
                if (addr_.ss_family == AF_INET) size_ = sizeof(sockaddr_in);
                if (addr_.ss_family == AF_INET6) size_ = sizeof(sockaddr_in6);
                address_ = addr_;

            }

        /* Normal methods ------------------------------------------------------------*/

        /**
         * String representation of the address
         * @return std::string
         */
        std::string to_string() const;

        /**
         * Set resource with ip and port.
         *
         * - 192.168.0.1        (IPV4 address)
         * - f:ab:cd            (IPV6 address)
         * @param std::string addr_
         * @param std::string port_
         * @return bool
         */
        bool parse_addr(std::string addr_ , std::string port_= "0",std::string _iface = "eth0" , int ai_family= AF_INET , int ai_socktype= SOCK_STREAM, int ai_flags= 0);

    private:

        address_t address_;
        addrlen_t size_;

};

#endif /* _ip_address_h */
