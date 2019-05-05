#include "dlinkinspector.h"
#include "ui_dlinkinspector.h"

/*
QTextEdit *log_te;
QString t_log;
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString msgstr;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        msgstr = QString::asprintf("Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        msgstr = QString::asprintf("Info: %s\n", localMsg.constData());
        break;
    case QtWarningMsg:
        msgstr = QString::asprintf("Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        msgstr = QString::asprintf("Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        msgstr = QString::asprintf("Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    }

    //setDebugInfo(msgstr);

    log_te->append(msgstr);
    QTextCursor cursor=log_te->textCursor();
    cursor.movePosition(QTextCursor::End);
    log_te->setTextCursor(cursor);
}

*/


dlinkinspector::dlinkinspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dlinkinspector)
{
    ui->setupUi(this);



    //qInstallMessageHandler(myMessageOutput);



}

dlinkinspector::~dlinkinspector()
{
    delete ui;
}

void dlinkinspector::setString(QString str)
{
    ui->textBrowser->clear();
    ui->textBrowser->insertPlainText(str);
}

void dlinkinspector::setTextBrowser(QTextBrowser *browser)
{
    //browser = ui->DebugInfo;
}

void dlinkinspector::setDebugInfo(QString str)
{
/*

    ui->DebugInfo->insertPlainText(str);
    QTextCursor cursor=ui->DebugInfo->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->DebugInfo->setTextCursor(cursor);
    */
}













