#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QWidget>
#include "QColor"
#include <QMouseEvent>

class QColor;

class Battery : public QWidget
{
    Q_OBJECT
public:
    explicit Battery(QWidget *parent = nullptr);

    uint8_t type = 0;



signals:



protected:
    void paintEvent(QPaintEvent *);

    void mouseMoveEvent(QMouseEvent *);


public slots:

    void setSize(int width,int height);

    void setColor(QColor);
    void setBackgroundColor(QColor);
    void setBorderColor(QColor);

    void drawBattery(QPainter *painter);


private:

    QColor m_bodycolor;
    QColor m_backgroundcolor;
    QColor m_bordercolor;


};

#endif // BATTERY_H
