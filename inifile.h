#ifndef INIFILE_H
#define INIFILE_H

#include "QObject"
#include "QString"
#include "QSettings"
#include "QDir"
class IniFile:public QObject
{
    Q_OBJECT
public:
    IniFile();
   ~IniFile();

    void    WriteIni(QString FileName,  QString Group, QString Name, QString Value);
    QString ReadIni(QString FileName, QString Group,QString Name);
    void    Clear(QString FileName);
private:

};

#endif // INIFILE_H
