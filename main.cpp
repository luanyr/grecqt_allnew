#include "qtsingleapplication/qtsingleapplication.h"
#include "headers.h"
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    if(a.isRunning())
    {
        a.sendMessage("message from other instance.");
        return 0;
    }
    myHelper::setCode();
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    MainWidget m;
    m.showMaximized();
    return a.exec();
}
