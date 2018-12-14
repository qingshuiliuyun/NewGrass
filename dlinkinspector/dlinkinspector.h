#ifndef DLINKINSPECTOR_H
#define DLINKINSPECTOR_H

#include <QWidget>
#include "QTextBrowser"
namespace Ui {
class dlinkinspector;
}

class dlinkinspector : public QWidget
{
    Q_OBJECT

public:
    explicit dlinkinspector(QWidget *parent = 0);
    ~dlinkinspector();

    void setString(QString str);


    void setTextBrowser(QTextBrowser *browser);

    void setDebugInfo(QString str);

    QTextBrowser *debuginfo;


private:
    Ui::dlinkinspector *ui;
};

#endif // DLINKINSPECTOR_H
