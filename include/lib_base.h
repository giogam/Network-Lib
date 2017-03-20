#ifndef _lib_base_h
#define _lib_base_h

/* classic C includes */
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

/* C++ includes*/
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>
#include <regex>
#include <map>
#include <set>
#include <list>
#include <numeric>

#define CLEAR_MSG(  message ) \
  message.str(""); \
  message.clear(); \

#define MAXLINE 1024


#define loop_always() while(true)

#endif /* _lib_base_h */
