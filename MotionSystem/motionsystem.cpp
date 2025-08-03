#include "motionsystem.h"

MotionSystem::MotionSystem(QObject *parent)
    : QObject{parent}
{
    timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &MotionSystem::onTimeout1);

    //temp_axes.a1 = 1;
    //temp_speed.speed = 2000;
    //temp_acceleration.acceleration = 0;

    //goTo(0x02,temp_axes,temp_speed,temp_acceleration,-500);
}

int32_t MotionSystem::getCoordinates()
{
    return coordinates;
}

void MotionSystem::setCoordinate(axes axes, int32_t coord)
{
    if(axes.a1 == 1)
        coordinates = coord;
}



uint8_t MotionSystem::goTo(ushort nCmd, axes axes, speed speed, acceleration acceleration, int32_t coord)
{
    Q_UNUSED(acceleration);

    this->nCmd = nCmd;

    temp_user_coordinates = coord;

    temp_axes = axes;
    temp_speed = speed;

    if(axes.a1 == 1)
        timer1->start(10000/speed.speed);

    return 0;
}

uint8_t MotionSystem::goOn(ushort nCmd, axes axes, speed speed, acceleration acceleration, int32_t coord)
{
    Q_UNUSED(acceleration);

    this->nCmd = nCmd;

    temp_user_coordinates = coord;

    previous_coordinates = getCoordinates();

    temp_axes = axes;
    temp_speed = speed;

    if(axes.a1 == 1)
        timer1->start(10000/speed.speed);

    return 0;
}

uint8_t MotionSystem::stop(ushort nCmd, axes axes)
{
    this->nCmd = nCmd;

    if(axes.a1 == 1)
        timer1->stop();

    return 0;
}

void MotionSystem::onTimeout1()
{
    timeoutFunction(nCmd, timer1, 0);
    //qDebug() << getCoordinates();
}

void MotionSystem::timeoutFunction(ushort nCmd, QTimer *timer, int i)
{
    switch (nCmd)
    {
    case 1:
    {
        if(temp_direction.direction)
            setCoordinate(temp_axes, getCoordinates() + 1);
        else
            setCoordinate(temp_axes, getCoordinates() - 1);
        break;
    }
    case 2:
    {
        if(getCoordinates() < temp_user_coordinates)
        {
            if(abs(getCoordinates() - temp_user_coordinates) > 0)
                setCoordinate(temp_axes, getCoordinates() + 1);
            else
            {
                timer->stop();
                delete timer;
            }
        }
        else if(getCoordinates() > temp_user_coordinates)
        {
            if(abs(getCoordinates() - temp_user_coordinates) > 0)
                setCoordinate(temp_axes, getCoordinates() - 1);
            else
            {
                timer->stop();
                delete timer;
            }
        }
        break;
    }
    case 4:
    {
        if(temp_user_coordinates > 0)
        {
            if(abs(getCoordinates() - (temp_user_coordinates + previous_coordinates)) > 0)
                setCoordinate(temp_axes, getCoordinates() + 1);
            else
            {
                timer->stop();
                delete timer;
            }
        }
        else if(temp_user_coordinates < 0)
        {
            if(abs(getCoordinates() - (temp_user_coordinates + previous_coordinates)) > 0)
                setCoordinate(temp_axes, getCoordinates() - 1);
            else
            {
                timer->stop();
                delete timer;
            }
        }
        break;
    }
    default:
        break;
    }
}

MotionSystem::~MotionSystem()
{
    qDebug() << "Motion system destructor";
}
