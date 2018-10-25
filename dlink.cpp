#include "dlink.h"
#include "QDebug"


QByteArray Fstart;
DLink::DLink(QObject *parent) : QObject(parent)
{
    Fstart[0] = 0xeb;
    Fstart[1] = 0x90;
}

DLink::~DLink()
{
    if (serialPort)
    {
        serialPort->close();
        delete serialPort;
        serialPort = NULL;
    }

}


void DLink::setup_port(const QString port, qint32 baudrate, QSerialPort::Parity parity)
{
    if (serialPort)
    {
        serialPort->close();
        delete serialPort;
    }
    serialPort = new QSerialPort(port, this);

    if (serialPort->open(QIODevice::ReadWrite))
    {
        serialPort->setBaudRate(baudrate);
        serialPort->setParity(parity);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setStopBits(QSerialPort::OneStop);

        connect(serialPort, &QSerialPort::readyRead, this, &DLink::readPendingDatagrams);
    }
    else
    {
        delete serialPort;
        serialPort = NULL;
    }
}

bool DLink::state_port()
{
    return (serialPort != NULL)?(true):(false);
}

void DLink::stop_port()
{
    serialPort->close();
    delete serialPort;
    serialPort = NULL;
}


QByteArray SerialData;
void DLink::readPendingDatagrams(void)
{
    QByteArray datagram = serialPort->readAll();

    //recievedata = datagram;

    SerialData.append(datagram);

    if(SerialData.contains(Fstart))
    {
        SerialData = SerialData.mid(SerialData.indexOf(Fstart));

        while(SerialData.size() >= 12)
        {
                QByteArray AFrame = SerialData.left(12);
                SerialData = SerialData.mid(12);

                //qDebug() << RawData << "landing";

        }

    }
}


