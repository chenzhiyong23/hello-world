#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QtDebug>
#include <QtSerialPort>
#include <QThread>

static unsigned char  auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
static unsigned char  auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
int MainWindow::CRC16(unsigned char &pucFrame,int usLen)
{
    unsigned char *ptr = &pucFrame;
    int i = 0;
    unsigned char ucCRCHi = 0xFF;
    unsigned char ucCRCLo = 0xFF;
    int iIndex = 0x0000;
    while (usLen--)
    {
        iIndex = (int)(ucCRCLo ^ *(ptr+i++));
        ucCRCLo = (unsigned char)(ucCRCHi ^ auchCRCHi[iIndex]);
        ucCRCHi = auchCRCLo[iIndex];
    }
    qDebug() << "CRC" << ucCRCLo <<"  " << ucCRCHi;
    return (ucCRCHi << 8 | ucCRCLo);//组装成一条modbus rtu报文时，ucCRCLo放在ucCRCHi前面
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isOpen = true;
    SetBehaviorType(BEHAVIOR::UNKNOWN);
    QStringList ComPortList;
    foreach(const QSerialPortInfo& info, QSerialPortInfo::availablePorts()){
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite)){
            ComPortList.append(serial.portName());
            serial.close();
        }
    }
    ui->ComPortCBox->addItems(ComPortList);
    
    QStringList BaudList;
    BaudList << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    ui->BaudCBox->addItems(BaudList);
    
    QStringList ParityList;
    ParityList << "NONE" << "ODD" << "EVEN";
    ui->ParityCBox->addItems(ParityList);
    ui->ParityCBox->setItemData(0, QSerialPort::NoParity, Qt::UserRole);
    ui->ParityCBox->setItemData(1, QSerialPort::OddParity, Qt::UserRole);
    ui->ParityCBox->setItemData(2, QSerialPort::EvenParity, Qt::UserRole);
    
    QStringList BitList;
    BitList << "6" << "7" <<"8";
    ui->BitCBox->addItems(BitList);
    ui->BitCBox->setItemData(0, QSerialPort::Data6, Qt::UserRole);
    ui->BitCBox->setItemData(1, QSerialPort::Data7, Qt::UserRole);
    ui->BitCBox->setItemData(2, QSerialPort::Data8, Qt::UserRole);
    
    QStringList StopList;
    StopList << "1" << "1.5" << "2";
    ui->StopCBox->addItems(StopList);
    ui->StopCBox->setItemData(0, QSerialPort::OneStop, Qt::UserRole);
    ui->StopCBox->setItemData(1, QSerialPort::OneAndHalfStop, Qt::UserRole);
    ui->StopCBox->setItemData(2, QSerialPort::TwoStop, Qt::UserRole);
    
    My_Com = new QSerialPort;//创建串口对象
    Thread = nullptr;
    Thread = new OrderThread;

    connect(Thread, SIGNAL(SetMode()), this, SLOT(SetMode()));
    connect(Thread, SIGNAL(SetSpeed()), this, SLOT(SetSpeed()));
    connect(Thread, SIGNAL(SetCtrlWord()), this, SLOT(SetCtrlWord()));
    connect(Thread, SIGNAL(QuerySpeed()), this, SLOT(GetSpeed()));
    connect(Thread, SIGNAL(QueryCurrent()), this, SLOT(GetCurrent()));
    connect(Thread, SIGNAL(QueryPosition()), this, SLOT(GetPosition()));
    connect(Thread, SIGNAL(QueryState()), this, SLOT(GetState()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenCom()
{
    My_Com->clear();
    My_Com->setPortName(ui->ComPortCBox->currentText());
    My_Com->setBaudRate(ui->BaudCBox->currentText().toInt());
    My_Com->setDataBits(QSerialPort::Data8);
    My_Com->setParity(QSerialPort::NoParity);
    My_Com->setStopBits(QSerialPort::OneStop);
    My_Com->setFlowControl(QSerialPort::NoFlowControl);

    if(!(My_Com->open(QIODevice::ReadWrite))){
        QMessageBox::critical(this, tr("警告"), tr("打开串口失败!"));
    }

    connect(My_Com, SIGNAL(readyRead()), this, SLOT(GetData()));
}

void MainWindow::GetData()
{
    char* buf;
    int da[1024] = {0};
    int flag = 0;
    QString receiveDataStr;
    QJsonObject json;
    QByteArray receiveBytes;
    receiveBytes = My_Com->readAll();//串口读取所有数据
    if(behaviorType != QUERY_SPEED && behaviorType != QUERY_STATE &&
       behaviorType != QUERY_CURRENT && behaviorType != QUERY_POSITION){
        return;
    }
    RequestData += receiveBytes;
    if(RequestData.size() < 7){
        return;
    }
    const int crc_size = RequestData.size() - 2;
    unsigned char crc[crc_size];
    buf = RequestData.data();
    qDebug() << "<<<<<<<<<<receive Data size:" << RequestData.size() << endl;
    qDebug() << "<<<<<<<<<<receive RequestData:" << RequestData << endl;
    qDebug("buf=%x",buf);
    qDebug("requestData.data()=%x",RequestData.data());

    for (int index; index < RequestData.size(); index++) {
        da[index] = (uchar)(*buf);
        if(index < (RequestData.size() - 2)){
            crc[flag] = (uchar)(*buf);
            flag++;
        }
        buf++;

        if(da[index] < 16){
            receiveDataStr += "0" + QString::number(da[index], 16) + " ";
        }
        else {
            receiveDataStr += QString::number(da[index], 16) + " ";
        }
    }
    qDebug() << "<<<<<<<<<<<<<<<<<<receiveDataStr:" << receiveDataStr << endl;
    qDebug() << "<<<<<<<<<<<<<<<<<<da[0]:" << da[0] << endl;

    int crc16 = CRC16(*crc, crc_size);
    if((crc16 & 0x00ff) != da[RequestData.size() - 2] || (crc16 >> 8 & 0x00ff) != da[RequestData.size() - 1])
        return;

    if(da[0] == 0x01 && da[1] == 0x03 && da[2] == 0x04 && behaviorType == QUERY_POSITION) {
        qDebug() << "enter position";

        qint32 position = da[3] << 24 | da[4] << 16 << da[5] << 8 | da[6];
        ui->PositionEdit->setText(QString::number(position));
    }

    if(da[0] == 0x01 && da[1] == 0x03 && da[2] == 0x02 && behaviorType == BEHAVIOR::QUERY_CURRENT){
        qDebug() << "enter current" ;
        qint16 current =  da[3]<<8|da[4];
        ui->CurrentEdit->setText(QString::number(current));
    }

    if(da[0] == 0x01 && da[1] == 0x03 && da[2] == 0x04 && behaviorType == BEHAVIOR::QUERY_SPEED){
        qDebug() << "speed";

        qint32 speed =  da[3] << 24 | da[4] << 16 | da[5] << 8 | da[6];
        ui->SpeedEdit_2->setText(QString::number(speed));
    }

        if(da[0] == 0x01 && da[1] == 0x03 && da[2] == 0x02 && behaviorType == BEHAVIOR::QUERY_STATE){
        qDebug() << "state" ;
        quint16 state =  da[3] << 8 | da[4] ;
        ui->StateEdit->setText(QString::number(state));
    }
}

void MainWindow::on_OkBtn_clicked()
{
    if(Thread != nullptr && !Thread->isRunning()){
        Thread->SetIsQuery(false);
        Thread->start();
    }
}

void MainWindow::on_OpenComBtn_clicked()
{
    if(isOpen){
        OpenCom();
        isOpen = false;
    }
    else {
        My_Com->close();
        isOpen = true;
    }
    ui->OpenComBtn->setText(isOpen ? "打开串口" : "关闭串口");
}

void MainWindow::on_GetDataBtn_clicked()
{
    if(Thread != nullptr && !Thread->isRunning()){
        Thread->SetIsQuery(true);
        Thread->start();
    }
}

void MainWindow::on_StopBtn_clicked()
{
    SetBehaviorType(BEHAVIOR::SET_STOP);
    unsigned char s[6] = {0x01, 0x06, 0x31, 0x00, 0x00, 0x06};
    int crc16 = CRC16(*s, 6);
    int crc15 = CRC16(*s, 5);
    int crc14 = CRC16(*s, 4);
    qDebug() << "crc16:" << crc16 <<endl;
    qDebug() << "crc15:" << crc15 <<endl;
    qDebug() << "crc14:" << crc14 <<endl;

    unsigned char ctrlWord[8] = {0x01, 0x06, 0x31, 0x00, 0x00, 0x06, crc16 & 0x00ff, crc16 >> 8 & 0x00ff};
    My_Com->write((char*)ctrlWord, 8);
    qDebug() << "setCtrlWord";
}

void MainWindow::SetMode()
{
    SetBehaviorType(BEHAVIOR::SET_MODE);
    unsigned char mode[8] = {0x01, 0x06, 0x35, 0x00, 0x00, 0x03, 0xC6, 0x07};
    My_Com->write((char*)mode, 8);
    qDebug() << "setMode";
}

void MainWindow::SetSpeed()
{
    SetBehaviorType(BEHAVIOR::SET_SPEED);
    unsigned long int speed = ui->SpeedEdit->text().toULong();
    unsigned char speedHH = speed >> 24;
    unsigned char speedHL = speed >> 16;
    unsigned char speedH  = speed >> 8;
    unsigned char speedL = speed;
    unsigned char s[11] = {0x01, 0x10, 0x6F, 0X00, 0X00, 0X02, 0X04, speedH, speedL, speedHH, speedHL};
    int crc16 = CRC16(*s, 11);
    unsigned char speedOrder[13] = {0x01, 0x10, 0x6F, 0x00, 0x00, 0x02, 0x04, speedH, speedL, speedHH,
                                    speedHL, crc16 & 0x00ff, crc16 >> 8 & 0x00ff};
    My_Com->write((char*)speedOrder, 13);
    qDebug() << "setSpeed";
}

void MainWindow::SetCtrlWord()
{
    SetBehaviorType(BEHAVIOR::SET_CTRLWORD);
    unsigned char ctrlWord[8] = {0x01, 0x06, 0x31, 0x00, 0x00, 0x0F, 0xC7, 0x32};
    My_Com->write((char*)ctrlWord, 8);
    qDebug() << "setCtrlWord";
}

void MainWindow::GetSpeed()
{
    SetBehaviorType(BEHAVIOR::QUERY_SPEED);
    unsigned char s[6] = {0x01, 0x03, 0x3B, 0x00, 0x00, 0x04};
    int crc16 = CRC16(*s, 6);
    unsigned char query[8] = {0x01, 0x03, 0x3B, 0x00, 0x00, 0x04, crc16 & 0x00ff, crc16 >> 8 & 0x00ff};
    My_Com->write((char*)query, 8);
    qDebug() << "getSpeed";
}

void MainWindow::GetPosition()
{
    SetBehaviorType(BEHAVIOR::QUERY_POSITION);
    unsigned char s[6] = {0x01, 0x03, 0x37, 0x00, 0x00, 0x04};
    int crc16 = CRC16(*s, 6);
    unsigned char query[8] = {0x01, 0x03, 0x37, 0x00, 0x00, 0x04, crc16 & 0x00ff, crc16 >> 8 & 0x00ff};
    My_Com->write((char*)query, 8);
    qDebug() <<"getPosition";
}

void MainWindow::GetCurrent()
{
    SetBehaviorType(BEHAVIOR::QUERY_CURRENT);
    unsigned char s[6] = {0x01, 0x03, 0x3E, 0x00, 0x00, 0x02};
    int crc16 = CRC16(*s, 6);
    unsigned char query[8] = {0x01, 0x03, 0x37, 0x00, 0x00, 0x02, crc16 & 0x00ff, crc16 >> 8 & 0x00ff};
    My_Com->write((char*)query, 8);
    qDebug() << "getCurrent";
}

void MainWindow::GetState()
{
    SetBehaviorType(BEHAVIOR::QUERY_STATE);
    unsigned char query[8] = {0x01, 0x03, 0x32, 0x00, 0x00, 0x02, 0xCA, 0xB3};
    My_Com->write((char*)query, 8);
    qDebug() << "getState";
}

void MainWindow::SetBehaviorType(int value)
{
    behaviorType = value;
}
