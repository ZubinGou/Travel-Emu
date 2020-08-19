#include "mainwindow.h"
#include "Passenger.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QDateTime time = QDateTime::currentDateTime();                  //获取现在的时间
    QString strDataTime = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式

    QFile file("log.txt");
    file.open(QFile::WriteOnly| QIODevice::Append);
    QTextStream out(&file);

//    QString strMessage = QString("[%1] %2 File: %3 Line：%4\r\n").arg(strDataTime).arg(msg).arg(context.file).arg(context.line);
    QString strMessage = QString("[%1] %2\r\n").arg(strDataTime).arg(msg);
    switch (type) {
    case QtDebugMsg:
        out << "Debug: " << strMessage;
        break;
    case QtWarningMsg:
        //out << "warning: " << strMessage;
        break;
    case QtCriticalMsg:
        //out << "critical: " << strMessage;
        break;
    case QtFatalMsg:
        //out << "fatal: " << strMessage;
        abort();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(myMessageOutput);
    QString qss;
    QFile qssFile(":/MyQss.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }
    MainWindow w;
    w.setWindowTitle("Travel-Emu");
    w.show();
    return a.exec();
}
