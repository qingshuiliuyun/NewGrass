#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QHostAddress>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

    int exec();

    void accept();
    void reject();



private:
    Ui::ConnectDialog *ui;
    QSerialPortInfo *serialInfo;

public:
    QString port = "COM1";
    qint32 baudrate = 115200;
    QSerialPort::Parity parity = QSerialPort::NoParity;
private slots:
    void on_combox_local_currentIndexChanged(const QString &arg1);
};

#endif // CONNECTDIALOG_H
