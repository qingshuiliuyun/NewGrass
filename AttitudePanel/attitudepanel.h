#ifndef ATTITUDEPANEL_H
#define ATTITUDEPANEL_H

#include <QWidget>
#include <QMouseEvent>
#include "battery.h"

class QColor;

class AttitudePanel: public QWidget
{
    Q_OBJECT
public:
    enum ErrorCode {MaxValueError=1,MinValueError,ThresholdError,TargetError,PrecisionError,ColorError,UnitsEmpty,OutOfRange};

    explicit AttitudePanel(QWidget *parent = 0);



typedef struct {
        quint8 fixtype;
        quint8 svn;
        double altitude;
        double latitude;
        double longitude;
        float  course;
        float  speed;
        float  vn;
        float  ve;
        float  vd;
    }_gps;

typedef struct {
        float ax;
        float ay;
        float az;

        float gx;
        float gy;
        float gz;

        float roll;
        float pitch;
        float yaw;

        float pressurealtitude;

        float airspeed;
        float verticalspeed;
        float horizontalspeed;

    }_attitude;

typedef struct {
        float voltage;
        float current;
    }_battery;


Q_SIGNALS:
    void errorSignal(int);

public Q_SLOTS:
    void setPitch(double Pitch);
    void setRoll(double Roll);
    void setYaw(double Yaw);
    void setAttitude(double Pitch,double Roll,double Yaw);

    void setGPSCourse(double Course);
    void setGPSSVN(int Number);
    void setGPSAltitude(double Altitude);
    void setGPSSpeed(double speed);
    void setPressureAltitude(double Altitude);
    void setAirSpeed(double speed);
    void setVoltage(double Voltage);

    void setLed(QColor LED);


protected:


    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);


    void paintEvent(QPaintEvent *);

    void drawLed(QPainter *painter);

    void drawAttitude(QPainter *painter);
    void drawYawScale(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawCenterLine(QPainter *painter);
    void drawText(QPainter *painter);
    void drawGPS(QPainter *painter);
    void drawHight(QPainter *painter);
    void drawSpeed(QPainter *painter);
    void drawBattery(QPainter *painter);


    void drawLowPanel(QPainter *painter);

private:

    Battery *bat;

    _gps m_GPS;
    _attitude m_Attitude;
    _battery  m_Battery;

    double m_PitchValue;
    double m_RollValue;
    double m_YawValue;
    double m_CourseValue;

    double m_BatteryValue;


    double m_GPSAltitudeValue;
    double m_PressureAltitudeValue;

    int m_Max_Roll;
    int m_Min_Roll;

    int m_Max_Altitude;
    int m_Min_Altitude;

    double m_gps_altitude;
    double m_pre_altitude;

    double m_gps_speed;
    double m_air_speed;

    QString m_CtrlModeValue;
    QString m_AirplaneModeValue;
    QString m_CurrentStatusValue;
    QString m_LockStatusValue;
    QString m_TurningStatusValue;

    int m_GPSStatusValue;
    int m_GPSStarValue;

    float m_Z_SpeedValue;
    float m_Z_HightValue;
    //float m_X_SpeedValue;

    //保护
    QString m_ProtectString;
    quint8  m_Protect;
    //数值的最大最小值
    double m_Pitch_MinValue;
    double m_Roll_MinValue;
    double m_Yaw_MinValue;
    double m_CourseMinValue;

    double m_Pitch_MaxValue;
    double m_Roll_MaxValue;
    double m_Yaw_MaxValue;
    double m_CourseMaxValue;

    double m_BatteryMinValue;
    double m_BatteryMaxValue;

    QColor m_CentreLineColor;
    QColor m_ForeColor;
    QColor m_CornerColor;

    QColor m_GroundColor;//大地颜色
    QColor m_SkyColor;//天空颜色

    QColor m_LedColor;

    QPoint m_c;

};

#endif // ALTITUDEPANEL_H
