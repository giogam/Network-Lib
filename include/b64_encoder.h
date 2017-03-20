#ifndef _b64_encoder_h
#define _b64_encoder_h

#include "lib_base.h"

class B64_Encoder
{
  public:

    B64_Encoder() { };
    ~B64_Encoder() { };

    const static char *encodeLookup;
    const static char padCharacter ;

    std::string base64Encode(std::vector<unsigned char> inputBuffer);
    std::vector<unsigned char> base64Decode(const std::string & input);

};

#endif /* _b64_encoder_h */
