#include <QCoreApplication>
#include <QtCore>
#include <QTimer>
#include <QDebug>
#include <QSettings>
#include "logger.h"

QSettings* settings;
QTimer     timer;
int        loopCounter = 0;
int        rebootCounter = 0;

void monitorLxFunction()
{
    // prepare status message
    loopCounter++;
    QString msg = QString("lxmon: iteration #%1").arg(QString::number(loopCounter));
    LOGINFO("%s", msg.toStdString().c_str());
    qDebug() << "\n" << msg;

    // immediately after app start the timer's interval is == 0
    if ( timer.interval() == 0 ) {

        // set timer to 20s for next iterations
        timer.setInterval(20000);

        // settings basics
        settings = new QSettings(QCoreApplication::applicationFilePath()+".ini", QSettings::IniFormat);

        // read absolute system reboot count ignited by monitor
        rebootCounter = settings->value("RebootCount", 0).toInt();

        // give completely up after 10 system reboots and shut down system
        if ( rebootCounter > 10 ) {
            msg = QString("lxmon gives up after #%1 reboots and shuts down the OS").arg(QString::number(rebootCounter));
            LOGINFO("%s", msg.toStdString().c_str());
            qDebug() << msg;
            // "RebootCount" is most important, otherwise the OS would end up dead
            settings->setValue("RebootCount", 0);
            settings->setValue("RebootReason", "");
            settings->setValue("ShutDownSystem", QDateTime::currentDateTime().toString());
            settings->sync();
            // shut down OS
            system("sudo shutdown now");
            exit(0);
        }
    }

    // what to monitor: check for 'lxsession', which means whether the GUI has started or not
    QString output = "";
    auto command = QString("bash");
    auto arguments = QStringList() << "-c" << "ps -e | grep lxsession";
    QProcess p;
    p.start(command, arguments);
    if ( !p.waitForFinished( -1 ) ) {
        qDebug() << "Error:" << p.readAllStandardError();
    } else {
        output = p.readAllStandardOutput();
        qDebug() << "Output:" << output;
    }

    // result in output indicates, all is fine
    if ( output.indexOf("lxsession") != -1 ) {
        msg = QString("success: lxmon ended, lxsession was started");
        LOGINFO("%s", msg.toStdString().c_str());
        qDebug() << msg;
        settings->setValue("RebootCount", 0);
        settings->setValue("RebootReason", "<none>");
        settings->setValue("ShutDownSystem", "NO, lxsession is running");
        settings->sync();
        timer.stop();
        exit(0);
    }

    // give up after 10 iterations each 20s and reboot OS
    if ( loopCounter > 10 ) {
        timer.stop();
        msg = QString("lxmon gives up after #%1 tries and reboots the system").arg(QString::number(loopCounter));
        LOGINFO("%s", msg.toStdString().c_str());
        qDebug() << msg;
        // write reboot count to ini
        rebootCounter++;
        // this value is read back at app start and decides, whether to shutdown the OS or to allow another reboot
        settings->setValue("RebootCount", rebootCounter);
        settings->setValue("RebootReason", msg);
        settings->setValue("ShutDownSystem", "not yet, still trying");
        settings->sync();
        // reboot
        system("sudo reboot now");
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // a new entry for today
    LOGINFO("----------------------------------------------------------");

    // start timer
    QObject::connect(&timer, &QTimer::timeout, monitorLxFunction);
    timer.start(0);

    return a.exec();
}
