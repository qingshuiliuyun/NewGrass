#include "inifile.h"
#include "qdebug.h"

IniFile::IniFile()
{

}

IniFile::~IniFile()
{

}

//初始化数据保存
void IniFile::WriteIni(QString FileName, QString Group, QString Name, QString Value)
{
      QDir *temp = new QDir;
      if(!temp->exists("./"))
      temp->mkdir("./");//如果文件夹不存在就新建

      //Qt中使用QSettings类读写ini文件
       //QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省
       QSettings *configIniWrite = new QSettings("./"+FileName, QSettings::IniFormat);
       //向ini文件中写入内容,setValue函数的两个参数是键值对
       //向ini文件的第一个节写入内容,ip节下的第一个参数
       QString str = "/";
       str.append(Group);
       str.append("/");
       str.append(Name);
       configIniWrite->setValue(str,Value);
       //写入完成后删除指针
       delete configIniWrite;
}

QString IniFile::ReadIni(QString FileName, QString Group,QString Name)
{
     /* QDir *temp = new QDir;
      if(!temp->exists("./"))
      {
         //return;//如果文件夹不存在就不执行
      }
      */

      QSettings *configIniRead = new QSettings("./"+FileName, QSettings::IniFormat);
       //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
      QString str = "/";
      str.append(Group);
      str.append("/");
      str.append(Name);
      QString ipResult = configIniRead->value(str).toString();
       //读入入完成后删除指针
      delete configIniRead;
      //qDebug() << ipResult;
      return ipResult;
}

void IniFile::Clear(QString FileName)
{

}
