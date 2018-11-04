#ifndef DLINK_H
#define DLINK_H

#include <QObject>
#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>
#include "QFile"
#include "QDateTime"


#define TXBUFFSIZE 1000
#define RXBUFFSIZE 1000

class DLink : public QObject
{
    Q_OBJECT
public:

    enum {
        Engine_OK = 0x50,
        Engine_UC,
        Engine_PA,
        Engine_NA,
        Engine_PR,
        Engine_PL,
        Engine_DF,
        Engine_OE,
        Engine_SE,
        Engine_CE
    };


    typedef struct {
        uint16_t HEAD;
        uint16_t CID;
        uint8_t  SYN;
        uint16_t LEN;
        QByteArray Payload;
        uint16_t SCK;

    }_frametypedef;


    typedef struct {
        uint8_t version;
        uint8_t heardbeat;
        uint8_t id;
    }_heardbeat;


    typedef struct {
      float speed_Kp;
      float speed_Ki;
      float speed_Kd;

      float distance_Kp;
      float distance_Ki;
      float distance_Kd;

      float heading_Kp;
      float heading_Ki;
      float heading_Kd;

    }_parameter;

    typedef struct {
        uint8_t fixtype;
        uint8_t svn;
        float  altitude;
        double latitude;
        double longitude;
        float  groundspeed;
        float  course;
    }_gps;

    typedef struct {
        float left;
        float front;
        float right;
    }_ultrasonic;

    typedef struct {
        uint16_t id;
        uint16_t type;//0 point 1 line 2 circle
        uint16_t action;
        float altitude;
        double latitude;
        double longitude;
        float speed;
        float course;
    }_waypoint;

    typedef struct {
        uint16_t currentwaypoint;
    }_status;






    typedef struct {
        //主缓存
        uint8_t txbuf[TXBUFFSIZE];
        uint16_t txtail;
        uint16_t txhead;
        //备份缓存
        uint8_t txbuf_bak[TXBUFFSIZE];
        uint16_t txtail_bak;
        uint16_t txhead_bak;

        uint8_t rxbuf[RXBUFFSIZE];
        uint8_t rxtemp;
        uint16_t rxtail;
        uint16_t rxhead;

        uint8_t isReadyToSend;
        uint8_t isRecieveReady;

        uint8_t txsyn;
        uint8_t rxsyn;

    }_uart;




    typedef  struct {

        uint32_t LED_Count;
        uint32_t HL_Count;
        uint32_t DIO_Count;
        uint32_t VOLTAGE_Count;
        uint32_t PWM_Count;
        uint32_t ENGINE_Count;


        _heardbeat HeardBeat;
        _parameter Parameter;
        _gps GPS;
        _ultrasonic Ultrasonic;
        _waypoint WayPoint;
        _status Satuts;

        _uart U1;
        _uart U2;

    }_vehicle;








    explicit DLink(QObject *parent = nullptr);
    ~DLink();
    void setup_port(const QString port, qint32 baudrate, QSerialPort::Parity parity);
    bool state_port();
    void stop_port();

    uint16_t CRC_CheckSum (uint8_t *pBuffer,uint8_t Size);

    void R_Decode(QByteArray data);

    _vehicle readvehicle();






    _vehicle vehicle;
    _frametypedef dframe;

    QByteArray recievedata;

    quint8 CurrentSetting = 0x01;


    QList<_waypoint> waypointlist;

signals:



public slots:
    void readPendingDatagrams(void);

    //发送
    void SendParameter(void);


private:
    QSerialPort *serialPort = NULL;

    QFile *FCCLogFile = NULL;
    QFile *InfoLogFile = NULL;




};

#endif // DLINK_H
