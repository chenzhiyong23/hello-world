#ifndef ORDERTHREAD_H
#define ORDERTHREAD_H

#include <QThread>
#include <QObject>

class OrderThread : public QThread
{
    Q_OBJECT
public:
    OrderThread();
    void SetIsQuery(bool value);

signals:
    void SetMode();
    void SetSpeed();
    void SetCtrlWord();
    void QuerySpeed();
    void QueryPosition();
    void QueryCurrent();
    void QueryState();

protected:
    void run();

private:
    bool isQuery;

};

#endif // ORDERTHREAD_H
