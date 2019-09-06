#include "orderthread.h"

OrderThread::OrderThread()
{
    isQuery = false;
}

void OrderThread::run()
{
    if(isQuery)
    {
        emit QuerySpeed();
        msleep(500);
        emit QueryPosition();
        msleep(500);
        emit QueryCurrent();
        msleep(500);
        emit QueryState();
    }
    else
    {
        emit SetMode();
        msleep(200);
        emit SetSpeed();
        msleep(200);
        emit SetCtrlWord();
    }
}

void OrderThread::SetIsQuery(bool value)
{
    isQuery = value;
}
