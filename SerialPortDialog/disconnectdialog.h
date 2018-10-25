#ifndef DISCONNECTDIALOG_H
#define DISCONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class disconnectdialog;
}

class disconnectdialog : public QDialog
{
    Q_OBJECT

public:
    explicit disconnectdialog(QWidget *parent = 0);
    ~disconnectdialog();

    int exec();

    void accept();
    void reject();

private:
    Ui::disconnectdialog *ui;
};

#endif // DISCONNECTDIALOG_H
