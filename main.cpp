#include "mainwindow.h"

#include <QApplication>
#include<QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/qss/style.qss");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug("open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("打开失败");
        return 1;
    }

    MainWindow w;
    w.setWindowTitle("Album");
    w.showMaximized();
    return a.exec();
}
