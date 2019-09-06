#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QByteArray>
#include <orderthread.h>

namespace Ui {
class MainWindow;
}

enum BEHAVIOR{
    UNKNOWN = 0,
    SET_MODE,
    SET_SPEED,
    SET_CTRLWORD,
    SET_STOP,
    QUERY_SPEED,
    QUERY_POSITION,
    QUERY_CURRENT,
    QUERY_STATE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_OpenComBtn_clicked();

    void on_GetDataBtn_clicked();

    void on_OkBtn_clicked();

    void on_StopBtn_clicked();

    void OpenCom();//打开串口实现函数

    void GetData();//获取返回串口数据实现函数

    int CRC16(unsigned char& orderArray, int Len);

    void SetMode();

    void SetSpeed();

    void SetCtrlWord();

    void GetSpeed();

    void GetCurrent();

    void GetPosition();

    void GetState();

public:
    QByteArray RequestData;

    void SetBehaviorType(int value);

private:
    Ui::MainWindow *ui;
    QSerialPort* My_Com;
    OrderThread* Thread;
    bool isOpen;
    int behaviorType;
};

#endif // MAINWINDOW_H
