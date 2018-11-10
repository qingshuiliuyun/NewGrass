#ifndef DLINKINSPECTOR_H
#define DLINKINSPECTOR_H

#include <QWidget>

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


private:
    Ui::dlinkinspector *ui;
};

#endif // DLINKINSPECTOR_H
