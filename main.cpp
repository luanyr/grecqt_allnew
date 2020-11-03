#include "qtsingleapplication/qtsingleapplication.h"
#include "mainwindow.h"
#include "myhelper.h"
#include "menuui.h"
#include "top_form.h"
#include "headers.h"
#include "contentui.h"
#include "mainu1.h"
#include "mainwidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    if (a.isRunning())
//    {
//        a.sendMessage("message from other instance.");
//        return 0;
//    }
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    //a.setActivationWindow(&cu);
//    Top_Form tf;
//    tf.show();
//    MainU1 mu;
    //a.setActiveWindow(&mu);
//    mu.show();
    MainWidget m;
    m.showMaximized();
    return a.exec();
}
