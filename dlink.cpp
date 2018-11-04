#include "dlink.h"
#include "QDebug"

/* Frame typedef
 * |EB 90|CIDL CIDH|SYN|LENL LENH|Payload|CKL CKH|
 * frame head is EB 90
 * CID is identify of a frame
 * SYN is synchronous code of frame ,rate form 0 to 255
 * LEN is only the payload's len
 * Payload is the data we need
 * Sum check is CRC16
 */


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

uint16_t DLink::CRC_CheckSum (uint8_t *pBuffer,uint8_t Size)
{

    uint16_t poly = 0x8408;
    uint16_t crc = 0;
    uint8_t carry;
    uint8_t i_bits;
    uint8_t j;

  for (j=0; j<Size; j++)
    {
        crc = (uint16_t)(crc ^ ((uint8_t)pBuffer[j]));
        for (i_bits=0; i_bits<8; i_bits++)
        {
            carry = (uint8_t)(crc & 1);
            crc = (uint16_t)(crc / 2);
            if (carry)
            {
                crc = (uint16_t)(crc^poly);
            }
        }
    }
    return crc;
}

//|EB 90|CIDL CIDH|SYN|LENL LENH|Payload|CKL CKH|
QByteArray SerialData;
void DLink::readPendingDatagrams(void)
{
    if(SerialData.size()>40960000)//40MByte
    {
        SerialData.clear();
    }
    QByteArray datagram = serialPort->readAll();
    SerialData.append(datagram);
    while(SerialData.contains(Fstart))
    {
        SerialData = SerialData.mid(SerialData.indexOf(Fstart));

        if(SerialData.size() < 7)//如果数据小于7个，那说明不能读到正确的帧长度
        {
            break;//退出while循环
        }
        //寻找CID，SYN，LEN等信息
        dframe.CID = SerialData[2] + ((uint16_t)SerialData[3]<<8);
        dframe.SYN = SerialData[4];
        dframe.LEN = SerialData[5] + ((uint16_t)SerialData[6]<<8) + 9;//数据长度加9等于帧长

        /*qDebug() << dframe.CID
                 << dframe.SYN
                 << dframe.LEN -9;
                 */
        if(SerialData.size() >= dframe.LEN)//如果当前数据长度大于或等于一帧，那么认为可以解码
        {
                QByteArray AFrame = SerialData.left(dframe.LEN);
                SerialData = SerialData.mid(dframe.LEN);

                dframe.SCK = SerialData[dframe.LEN - 2] + ((uint16_t)SerialData[dframe.LEN - 1]<<8);//校验
                //如果通过校验，那么把数据给解码函数进行解码
                if(dframe.SCK == CRC_CheckSum((uint8_t *)AFrame.data(),dframe.LEN-2))
                {
                    R_Decode(AFrame);
                }
        }
        else
        {
            break;//退出循环
        }

    }
}


void DLink::R_Decode(QByteArray data)
{

     switch(dframe.CID)
     {
         case 0x0000://heartbeat
         {
              memcmp(&vehicle.HeardBeat,data+7,dframe.LEN-9);//从第7个复制
         }break;

         case 0x0010://parameter
         {
              memcmp(&vehicle.Parameter,data+7,dframe.LEN-9);//从第7个复制
         }break;

         case 0x0020://gps
         {
              memcmp(&vehicle.GPS,data+7,dframe.LEN-9);//从第7个复制
         }break;

         case 0x0030://Ultrasonic
         {
              memcmp(&vehicle.Ultrasonic,data+7,dframe.LEN-9);//从第7个复制
         }break;

         case 0x0040://WayPoint
         {
              memcmp(&vehicle.WayPoint,data+7,dframe.LEN-9);//从第7个复制o
              waypointlist<<vehicle.WayPoint;//存起来


         }break;

         case 0x0050://Satuts
         {
              memcmp(&vehicle.Satuts,data+7,dframe.LEN-9);//从第7个复制
         }break;
     }
}




DLink::_vehicle DLink::readvehicle()
{
    return vehicle;
}

void DLink::SendParameter(void)
{

}



