#include "dlink.h"
#include "QDebug"
//#include "QMessageBox"

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


    WayPointTimer = new QTimer();
    connect(WayPointTimer,SIGNAL(timeout()),
            this,SLOT(WayPointTimeOut()));


    SimuTimer = new QTimer();
    connect(SimuTimer,SIGNAL(timeout()),
            this,SLOT(SimuTimerTimeOut()));

}

DLink::~DLink()
{
    WayPointTimer->stop();
    delete WayPointTimer;
    WayPointTimer = NULL;

    SimuTimer->stop();
    delete SimuTimer;
    SimuTimer = NULL;

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

    //qDebug() << datagram;

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
        dframe.LEN = (SerialData[5] + ((uint16_t)SerialData[6]<<8)) + 9;//数据长度加9等于帧长

        /*qDebug() << dframe.CID
                 << dframe.SYN
                 << dframe.LEN -9;
                 */
        if(SerialData.size() >= dframe.LEN)//如果当前数据长度大于或等于一帧，那么认为可以解码
        {
                QByteArray AFrame = SerialData.left(dframe.LEN);
                SerialData = SerialData.mid(SerialData.indexOf(Fstart) + 2);//去除头部

                dframe.SCK = (uint8_t)AFrame[dframe.LEN - 2] + ((uint16_t)AFrame[dframe.LEN - 1]<<8);//校验
                //如果通过校验，那么把数据给解码函数进行解码

                /*
                qDebug() << dframe.SCK
                         << (uint8_t)AFrame[dframe.LEN - 2]
                         << (uint8_t)AFrame[dframe.LEN - 1]
                         << dframe.CID
                         << dframe.SYN
                         << dframe.LEN
                         << CRC_CheckSum((uint8_t *)AFrame.data(),dframe.LEN-2);
                         */


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
              data = data.mid(7);
              memcpy(&vehicle.HeardBeat,data,sizeof(vehicle.HeardBeat));//从第7个复制
              emit dlinkUpdate();
         }break;

         case 0x0004://ECHO
         {
              qDebug() << "echo";
              uint32_t ID = 0,Value = 0;
              union{uint8_t B[4];uint32_t W;}src;

              src.B[0] = data[7];
              src.B[1] = data[8];
              src.B[2] = data[9];
              src.B[3] = data[10];
              ID = src.W;

              src.B[0] = data[11];
              src.B[1] = data[12];
              src.B[2] = data[13];
              src.B[3] = data[14];
              Value = src.W;
              //qDebug() << ID << Value;
              switch(ID)
              {
                  case 0x0040://命令回复
                  {
                       if(Value == 0x07)//航点收到回复
                       {
                           isSendWayPointCompelet = true;

                           RecieveWaypoint(1);
                           qDebug() << ID << Value << isSendWayPointCompelet;

                       }
                       else if(Value == 0x08)//航点发送结束
                       {
                           //isSendWayPointCompelet = true;
                           RecieveWaypoint(-1);
                           isGetNextWayPoint = false;
                           qDebug() << ID << Value;

                       }
                  }break;
              }


         }break;

         case 0x0005://CMD
         {
              data = data.mid(7);
              memcpy(&vehicle.CMD,data,sizeof(vehicle.CMD));//从第7个复制
         }break;

         case 0x0010://parameter
         {
              data = data.mid(7);
              memcpy(&vehicle.Parameter,data,sizeof(vehicle.Parameter));
              emit flushParameter();
              //qDebug() << "parameter recieve" << data.data()+7 <<data;
         }break;

         case 0x0020://gps
         {
              data = data.mid(7);
              memcpy(&vehicle.GPS,data,dframe.LEN-9);//从第7个复制

              //qDebug() << vehicle.GPS.fixtype
              //         <<vehicle.GPS.latitude ;

              //qDebug() << "recieve gps";
              emit dlinkUpdate();
         }break;

         case 0x0030://Ultrasonic
         {
              data = data.mid(7);
              memcpy(&vehicle.Ultrasonic,data,dframe.LEN-9);//从第7个复制



              emit dlinkUpdate();
         }break;

         case 0x0040://WayPoint
         {
              data = data.mid(7);
              memcpy(&vehicle.WayPoint,data,dframe.LEN-9);//从第7个复制o
              waypointlist<<vehicle.WayPoint;//存起来
              emit recievePoint(vehicle.WayPoint);
              //emit flushWayPoint();

              qDebug() << "waypoint" << vehicle.WayPoint.id;

              emit RecieveWaypoint(vehicle.WayPoint.id);
              if(isGetNextWayPoint == true)
              {
                  qDebug() << "getnext" << isGetNextWayPoint;
                  SendCMD(0x40,0x07);
              }

         }break;

         case 0x0050://Satuts
         {
              data = data.mid(7);
              memcpy(&vehicle.Satuts,data,dframe.LEN-9);//从第7个复制
              emit dlinkUpdate();
         }break;
     }
}




DLink::_vehicle DLink::readvehicle()
{
    return vehicle;
}


//|EB 90|CIDL CIDH|SYN|LENL LENH|Payload|CKL CKH|



void DLink::SendCMD(uint32_t ID,uint32_t Value)
{
    union{uint8_t B[4];uint16_t D[2];uint32_t W;}src;
    char  DataToSend[100];
    uint16_t DataCount = 0;

    DataToSend[DataCount++] = 0xEB;
    DataToSend[DataCount++] = 0x90;

    src.D[0] = 0x0005;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];


    DataToSend[DataCount++] = vehicle.U2.txsyn++;

    src.D[0] = 8;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    src.W = ID;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.W = Value;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.D[0] = CRC_CheckSum((uint8_t *)DataToSend,DataCount);
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    if(serialPort)
    {
       serialPort->write(DataToSend,DataCount);
       qInfo() << "Send CMD" << ID << Value;
    }
    else
    {
        qInfo() << "Please Open SerialPort first";
    }
}

void DLink::SendParameter(void)
{
    union{uint8_t B[2];uint16_t D;}src;
    char  DataToSend[100];
    uint16_t DataCount = 0;

    DataToSend[DataCount++] = 0xEB;
    DataToSend[DataCount++] = 0x90;

    src.D = 0x0010;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];


    DataToSend[DataCount++] = vehicle.U2.txsyn++;

    src.D = sizeof(vehicle.Parameter);
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    memcpy(DataToSend + DataCount,&vehicle.Parameter,sizeof(vehicle.Parameter));
    DataCount += sizeof(vehicle.Parameter);


    src.D = CRC_CheckSum((uint8_t *)DataToSend,DataCount);
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    if(serialPort)
    {
       serialPort->write(DataToSend,DataCount);
       qInfo() << "Send parameter";
    }
    else
    {
        qInfo() << "Please Open SerialPort first";
    }

    //qDebug() << (uint8_t)DataCount << "send";

}

void DLink::SendWayPoint(void)
{
    union{uint8_t B[2];uint16_t D;}src;
    char  DataToSend[100];
    uint16_t DataCount = 0;

    DataToSend[DataCount++] = 0xEB;
    DataToSend[DataCount++] = 0x90;

    src.D = 0x0040;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    DataToSend[DataCount++] = vehicle.U2.txsyn++;

    src.D = sizeof(vehicle.WayPoint);
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    memcpy(DataToSend + DataCount,&vehicle.WayPoint,sizeof(vehicle.WayPoint));
    DataCount += sizeof(vehicle.WayPoint);

    src.D = CRC_CheckSum((uint8_t *)DataToSend,DataCount);
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    if(serialPort)
    {
       serialPort->write(DataToSend,DataCount);
    }
    else
    {
        qInfo() << "Please Open SerialPort first";
    }
}


void DLink::SendWayPointThread(void)
{
    if(waypointlist.size() > 0)
    {
        WayPointTimer->start(100);//100ms一个航点

        //初始化第一个航点以及发送标志
        SendWayPointCount = 0;
        vehicle.WayPoint = waypointlist.at(SendWayPointCount);
    }
    else
    {
        qInfo() << "Have no any waypoint,please create first";
    }

}

void DLink::WayPointTimeOut(void)
{
    if(isSendWayPointCompelet)//如果发送成功，那发送下一个航点
    {
        isSendWayPointCompelet = false;
        SendWayPointCount ++;//下一个航点
        if(SendWayPointCount < waypointlist.size())
        {
            vehicle.WayPoint = waypointlist.at(SendWayPointCount);
            emit SendingWayPoint(waypointlist.size(),SendWayPointCount);
            SendWayPoint();//发送一个航点

        }
        else
        {
            //如果发送完成，向设备发送停止发送航点命令

            SendCMD(0,0x08);
            SendWayPointCount = 0;
            WayPointTimer->stop();
        }
    }
    else//未发送成功，那么继续发送当前航点
    {
        emit SendingWayPoint(waypointlist.size(),SendWayPointCount);
        SendWayPoint();//发送一个航点
    }

    emit SendWaypoint(SendWayPointCount);


}

void DLink::WayPointClear()
{
    waypointlist.clear();
}

void DLink::WayPointAppend(Mission::_waypoint point)
{
    waypointlist.append(point);
}


void DLink::SendSimu(void)
{
    union{uint8_t B[2];uint16_t D;}src;
    char  DataToSend[100];
    uint16_t DataCount = 0;

    DataToSend[DataCount++] = 0xEB;
    DataToSend[DataCount++] = 0x90;

    src.D = 0x0021;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    DataToSend[DataCount++] = vehicle.U2.txsyn++;

    src.D = sizeof(vehicle.SIM);
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    memcpy(DataToSend + DataCount,&vehicle.SIM,sizeof(vehicle.SIM));
    DataCount += sizeof(vehicle.SIM);

    src.D = CRC_CheckSum((uint8_t *)DataToSend,DataCount);
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];

    if(serialPort)
    {
       serialPort->write(DataToSend,DataCount);
       //qInfo() << "send simu";
    }
    else
    {
        qInfo() << "Please Open SerialPort first";
    }
}


void DLink::SimuStart(void)
{
    qDebug() << "start simu 5hz";
    SimuTimer->start(200);//5hz
}

void DLink::SimuStop(void)
{
    qDebug() << "stop simu";
    SimuTimer->stop();
}

void DLink::SimuTimerTimeOut(void)
{
    //qDebug() << "into simu";
    SendSimu();
}

