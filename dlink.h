#ifndef DLINK_H
#define DLINK_H

#include <QObject>
#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>
#include "QFile"
#include "QDateTime"
#include "QTimer"
#include "QDir"
#include "Mission/mission.h"


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

/*    typedef struct {
        uint16_t id;
        uint16_t type;//0 point 1 line 2 circle
        uint16_t action;
        float altitude;
        double latitude;
        double longitude;
        float speed;
        float course;
    }_waypoint;
    */

    typedef struct {
        uint16_t currentwaypoint;
        float voltage1;
        float voltage2;
        float voltage3;
        float voltage4;

        uint16_t leftfront_p;
        uint16_t leftfront_n;
        uint16_t leftback_p;
        uint16_t leftback_n;

        uint16_t rightfront_p;
        uint16_t rightfront_n;
        uint16_t rightback_p;
        uint16_t rightback_n;

        float dis2wp;
        float detaP;
        float gyroZ;

        bool isDebug;
    }_status;

    typedef struct {
            float speed_p;
          float speed_i;
          float speed_d;
            float speed_o;

            float position_p;
          float position_i;
          float position_d;
            float position_o;

          float heading_p;
          float heading_i;
          float heading_d;
            float heading_o;


    }_par;

    typedef struct {

        bool ReadParameter;
        bool StartMission;
        bool StopMission;
        bool BackHome;

    }_cmd;



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
        _gps SIM;//simulation point
        _ultrasonic Ultrasonic;
        Mission::_waypoint WayPoint;
        _status Satuts;
        _par Par;
        _cmd CMD;


        _uart U1;
        _uart U2;

    }_vehicle;








    explicit DLink(QObject *parent = nullptr);
    ~DLink();
    void setup_port(const QString port, qint32 baudrate, QSerialPort::Parity parity);
    bool state_port();
    void stop_port();

    void StartRecord(void);
    void RecordReplay(void);

    uint16_t CRC_CheckSum (uint8_t *pBuffer,uint8_t Size);

    void R_Decode(QByteArray data);

    _vehicle readvehicle();

    void SendCMD(uint32_t ID, uint32_t Value);

    void WayPointClear();
    void WayPointAppend(Mission::_waypoint point);

    void SendSimu(void);
    void SimuStart(void);
    void SimuStop(void);



    _vehicle vehicle;
    _frametypedef dframe;

    QByteArray recievedata;

    quint8 CurrentSetting = 0x01;


    QList<Mission::_waypoint> waypointlist;

    bool isSendWayPointCompelet = false;
    bool isGetNextWayPoint = true;


signals:

   void flushParameter(void);

   void SendingWayPoint(int total,int current);

   void flushWayPoint(void);

   void recievePoint(Mission::_waypoint P);

   void dlinkUpdate(void);


   void RecieveWaypoint(int32_t);
   void SendWaypoint(int32_t);




public slots:
    void readPendingDatagrams(void);

    //发送

    void SendParameter(void);


    void WayPointTimeOut(void);

    void SendWayPoint(void);

    void SendWayPointThread(void);

    void SimuTimerTimeOut(void);


private:
    QSerialPort *serialPort = nullptr;

    QFile *FCCLogFile = nullptr;
    QFile *InfoLogFile = nullptr;

    QTimer *WayPointTimer = nullptr;


    int SendWayPointCount = 0;

    QTimer *SimuTimer = nullptr;

    QFile *LogFile = NULL;
};

#endif // DLINK_H
