#include "ocrapplication.h"
#include "ocrruntimemanager.h"
#include "ttkdumper.h"

#include <QTimer>
#include <QScreen>
#include <QTranslator>
#include <QApplication>

#define TTK_DEBUG

void loadAppScaledFactor(int argc, char *argv[])
{
#if TTK_QT_VERSION_CHECK(5,4,0)
    #if TTK_QT_VERSION_CHECK(5,6,0)
      Q_UNUSED(argc);
      Q_UNUSED(argv);
    #if !TTK_QT_VERSION_CHECK(6,0,0)
      QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif
    #else
      QApplication a(argc, argv);
      qputenv("QT_DEVICE_PIXEL_RATIO", "auto");
      QScreen *screen = QApplication::primaryScreen();
      qreal dpi = screen->logicalDotsPerInch()/96;
      qputenv("QT_SCALE_FACTOR", QByteArray::number(dpi));
      Q_UNUSED(a);
    #endif
#else
    Q_UNUSED(argc);
    Q_UNUSED(argv);
#endif
}

int main(int argc, char *argv[])
{
    loadAppScaledFactor(argc, argv);
    //
    QApplication a(argc, argv);
#if !defined TTK_DEBUG && !defined Q_OS_UNIX
    if(argc <= 1 || QString(argv[1]) != APPNAME)
    {
        return -1;
    }
#endif
    ///////////////////////////////////////////////////////
    QCoreApplication::setOrganizationName(APPNAME);
    QCoreApplication::setOrganizationDomain(APPCOME);
    QCoreApplication::setApplicationName(APPNAME);

    TTKDumper dumper;
    dumper.run();

    OCRRunTimeManager manager;
    manager.run();

    QTranslator translator;
    if(!translator.load(manager.translator()))
    {
        TTK_LOGGER_ERROR("Load translation error");
    }
    a.installTranslator(&translator);

    OCRApplication w;
    w.show();

    return a.exec();
}
