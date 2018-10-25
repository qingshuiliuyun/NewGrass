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
        uint8_t RED;
        uint8_t GREEN;
        uint8_t BLUE;

        uint8_t REDCount;
        uint8_t GREENCount;
        uint8_t BLUECount;

        uint8_t isControl;

    }_led;


    typedef struct {
        uint16_t PWM1;      //TIM1->CH1
        uint16_t PWM2;      //TIM1->CH2
        uint16_t PWM3;      //TIM1->CH3
        uint16_t PWM4;      //TIM1->CH4
        uint16_t PWM5;      //TIM2->CH1
        uint16_t PWM6;      //TIM2->CH2
        uint16_t PWM7;      //TIM2->CH3
        uint16_t PWM8;      //TIM2->CH4
        uint16_t PWM9;      //TIM3->CH1
        uint16_t PWM10;     //TIM3->CH2
        uint16_t PWM11;     //TIM3->CH3
        uint16_t PWM12;     //TIM3->CH4
        uint16_t PWM13;     //TIM4->CH1
        uint16_t PWM14;     //TIM4->CH2
        uint16_t PWM15;     //TIM4->CH3
        uint16_t PWM16;     //TIM4->CH4
        uint16_t PWM17;     //TIM5->CH1
        uint16_t PWM18;     //TIM5->CH2
        uint16_t PWM19;     //TIM5->CH3
        uint16_t PWM20;     //TIM5->CH4
        uint16_t PWM21;     //TIM8->CH1
        uint16_t PWM22;     //TIM8->CH2
        uint16_t PWM23;     //TIM8->CH3
        uint16_t PWM24;     //TIM8->CH4
    }_pwm;

    typedef struct {
        float HL1;
        float HL2;
        float HL3;
        float HL4;
        float HL5;
    }_hl;

    typedef struct {
        uint8_t DI0;
        uint8_t DI1;
        uint8_t DI2;
        uint8_t DI3;
        uint8_t DI4;
        uint8_t DI5;

        uint8_t DO0;
        uint8_t DO1;
        uint8_t DO2;
        uint8_t DO3;
        uint8_t DO4;
        uint8_t DO5;
    }_diox;

    typedef struct {

        uint32_t adc_value[4];

        float Voltage1;
        float Voltage2;
        float Voltage3;
        float Voltage4;
    }_adc;

    typedef struct {
        uint16_t Ignition1Flag;
        uint16_t PumpFlag;
        uint16_t RPMRegulatorFlag;
        uint16_t ChokeFlag;
        uint16_t Ignition2Flag;
        uint16_t AmbientTemperatur;
        uint16_t AirPressure;
        uint16_t ActualFuelPressure;
        uint16_t FuelPumpDutyCycle;
        uint16_t ActualJet1DutyCycle;
        uint16_t ActualRPM;
        uint16_t ServoPositionThrottleIdle;
        uint16_t ServoPositionThrottleFull;
        uint16_t ServoPositionAirClosed;
        uint16_t ServoPositionAirOpen;
        uint16_t CHTemperature1;
        uint16_t CHTemperature2;
        uint16_t CHTemperature3;
        uint16_t CHTemperature4;
        uint16_t IMainPower;
        uint16_t IIgnition1;
        uint16_t IIgnition2;
        uint16_t IServos;
        uint16_t IPump;
        uint16_t UMainPower;
        uint16_t ActualValueThrottlePlate;
        uint16_t ActualValueAirChoke;
        uint16_t FirmwareVersionNumber;
    }_enginestatus;


    typedef struct {
        uint16_t Ignition1;
        uint16_t Pump;
        uint16_t RPMRegulator;
        uint16_t Choke;
        uint16_t Ignition2;
        uint16_t SetValueRPM;
        uint16_t StopEngine;

    }_enginecmd;

    typedef struct {

        _enginestatus STATUS;
      _enginecmd    CMD;

    }_engine;





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


        _led LED;
        _hl  HL;
        _diox DIO;
        _adc VOLTAGE;
        _pwm PWM;
        _engine ENGINE;

        _uart U1;
        _uart U2;

    }_hal_io;








    explicit DLink(QObject *parent = nullptr);
    ~DLink();
    void setup_port(const QString port, qint32 baudrate, QSerialPort::Parity parity);
    bool state_port();
    void stop_port();


    _hal_io HAL_IO;

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
