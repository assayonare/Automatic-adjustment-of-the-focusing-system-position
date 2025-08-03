#ifndef DEFINITIONS
#define DEFINITIONS

#define BUFFER_COUNT                ( 10 )

#define MAX_FREQUENCY               2000
#define BITS                        65535

#define FORWARD                     1
#define BACK                        -1

#define FORMAT_NAME_OF_IMAGE        3  // количество цифр в названии изображения

#define ENABLE                      1
#define DISABLE                     0

#define M_PI                        3.14159265358979323846

#define GAMMA_STEP                  100.0

#define PIXEL_SIZE                  0.14 // мм // размер пикселя приемника

#define RATE_OF_INCREASE            1 // коэффициент увеличения

//#define FULL_TURN                   288000
#define NUM_OF_AXES                 6
#define LIMITS_COUNT                4
#define PROGRAMABLE_POINTS_COUNT    3

// цветовые маски статусов //ADD
#define SUCCESS_STATUS              0x3eb100
#define FAULT_STATUS                0xB22222
#define PROCESS_STATUS              0xa66d00
#define ERROR_INPUT                 0x8d0f0f

//// смещения концевиков от источника и приемника в мм
//#define RECEIVER_LIMIT_OFFSET 15
//#define SOURCE_LIMIT_OFFSET 10

//// программные смещения крайних положений в шагах
//#define HORIZONTAL_LIMIT_OFFSET 1000
//#define VERTICAL_LIMIT_OFFSET 1000

#define MIN_VOLTAGE_FOR_CALIBRATION 50
#define MAX_VOLTAGE_FOR_CALIBRATION /*60*/150
#define STEP_VOLTAGE_FOR_CALIBRATION 10

#define RELEASE_MTL 1
#define TEST_PODVAL 0

#define WITHOUT_DETECTOR 0

#define WITHOUT_MOTION 1

#define WITH_SYSTEM 0

enum ConnectionType
{
    CONNECTION_FAULT,
    RS232_CONNECT,
    ETHERNET_CONNECT_TCP,
    ETHERNET_CONNECT_UDP,
    UNKNOWN_CONNECT = 99
};

#endif // DEFINITIONS

