#ifndef MOTIONSYSTEMSTRUCTURES_H
#define MOTIONSYSTEMSTRUCTURES_H


#include <stdint.h>

using namespace std;

//Структура для функции move()
#pragma pack(push,1)
struct axes
{
    uint16_t a1: 1;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct speed
{
    uint16_t speed;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct acceleration
{
    uint16_t acceleration;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct direction
{
    uint8_t direction;
};
#pragma pack(pop)


//Структура для функции goTo()
#pragma pack(push, 1)
struct dist
{
    int32_t distance;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct GoTo
{
    uint16_t nCmd = 0x02;
    axes axes;
    speed speed;
    acceleration acceleration;
    dist distance;
};
#pragma pack(pop)

//Структура для функции goOn()
#pragma pack(push, 1)
struct GoOn
{
    uint16_t nCmd = 0x04;
    axes axes;
    speed speed;
    acceleration acceleration;
    dist distance;
};
#pragma pack(pop)

//Структура для функции stop()
#pragma pack(push, 1)
struct Stop
{
    uint16_t nCmd = 0x05;
    axes axes;
};
#pragma pack(pop)


#endif // MOTIONSYSTEMSTRUCTURES_H
