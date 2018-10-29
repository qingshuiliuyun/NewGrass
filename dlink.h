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
        float    altitude;
        double   latitude;
        double   longitude;
        float    speed;
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







    _vehicle vehicle;
    _frametypedef dframe;

    QByteArray recievedata;

    quint8 CurrentSetting = 0x01;
//landing gear
    quint8 CMDMode = 0;

    quint8 CMDFrame[50];
    quint8 CMDEngine[50];

    quint8 FrameCount = 0;

    quint8 CMD_Landing = 0;
    quint8 CMD_Umbrella = 0;
    qint8  CMD_ForeWheelAngle = 0;
    quint8 CMD_LeftBrakeForce = 0;
    quint8 CMD_RightBrakeForce = 0;


    quint8 Feedback_ForeLanding_up;
    quint8 Feedback_ForeLanding_down;
    quint8 Feedback_LeftLanding_up;
    quint8 Feedback_LeftLanding_down;
    quint8 Feedback_RightLanding_up;
    quint8 Feedback_RightLanding_down;

    quint8 Feedback_ForeTouchLand;
    quint8 Feedback_LeftTouchLand;
    quint8 Feedback_RightTouchLand;

    quint8 Feedback_OpenUmbrella;
    quint8 Feedback_CutUmbrella;

    quint16 Feedback_Pressure1;
    quint16 Feedback_Pressure;

    quint8 Feedback_GearStates;
    quint8 Feedback_g_gearupFK;
    quint8 Feedback_isSoft;

//engine

    quint8 Engine_Ordor = 0;
    quint8 Engine_Count = 0;
    quint8 Engine_Start_Stop = 0;
    quint8 Engine_Throttle = 0;


    quint8 Feedback_Engine_SS1 = 0;
    quint8 Feedback_Engine_SS2 = 0;
    quint8 Feedback_Engine_SS3 = 0;
    quint8 Feedback_Engine_SS4 = 0;

    quint8 Feedback_Engine_Throttle1 = 0;
    quint8 Feedback_Engine_Throttle2 = 0;
    quint8 Feedback_Engine_Throttle3 = 0;
    quint8 Feedback_Engine_Throttle4 = 0;



    quint8  CMD_States1,CMD_States2,CMD_States3,CMD_States4;
    quint8  TurbineStates1,TurbineStates2,TurbineStates3,TurbineStates4;
    quint32 TurbineRPM1,TurbineRPM2,TurbineRPM3,TurbineRPM4;
    qint16  TurbineTMP1,TurbineTMP2,TurbineTMP3,TurbineTMP4;
    quint8  TurbineVoltage1,TurbineVoltage2,TurbineVoltage3,TurbineVoltage4;
    quint8  TurbineCurrent1,TurbineCurrent2,TurbineCurrent3,TurbineCurrent4;

signals:



public slots:
    void readPendingDatagrams(void);

private:
    QSerialPort *serialPort = NULL;

    QFile *FCCLogFile = NULL;
    QFile *InfoLogFile = NULL;




};

#endif // DLINK_H
