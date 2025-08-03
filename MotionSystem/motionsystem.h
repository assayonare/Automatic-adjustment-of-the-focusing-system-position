#ifndef MOTIONSYSTEM_H
#define MOTIONSYSTEM_H

#include <QObject>
#include <QTimer>
#include <QDebug> //

#include "motionsystemstructures.h"

class MotionSystem : public QObject
{
    Q_OBJECT
public:
    explicit MotionSystem(QObject *parent = nullptr);
    int32_t getCoordinates();
    uint8_t goTo(ushort nCmd, axes axes, speed speed, acceleration acceleration, int32_t coord);
    uint8_t goOn(ushort nCmd, axes axes, speed speed, acceleration acceleration, int32_t coord);
    uint8_t stop(ushort nCmd, axes axes);
    ~MotionSystem();

private slots:
    void onTimeout1();

private:
    void setCoordinate(axes axes, int32_t coord);

    ushort nCmd;
    int32_t coordinates = 0;
    int32_t temp_user_coordinates = 0;
    int32_t previous_coordinates = 0;
    direction temp_direction;
    acceleration temp_acceleration;
    speed temp_speed;
    axes temp_axes;
    QTimer *timer1;
    void timeoutFunction(ushort nCmd, QTimer* timer, int i);
};

#endif // MOTIONSYSTEM_H
