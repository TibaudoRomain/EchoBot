#ifndef MSG_STRUCTURE_H
#define MSG_STRUCTURE_H

#include <cstdint>
#endif // MSG_STRUCTURE_H

struct message {
    uint32_t securityKey;
    uint8_t angleA;
    uint8_t angleB;
    uint8_t angleC;
    uint8_t angleD;
    uint8_t angleE;
};
