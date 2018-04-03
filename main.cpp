#include <cstdlib>
#include <ctime>
#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QtPlugin>

#include "padgui.h"

#ifdef _WIN32
//    Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin)
#endif

int main(int argc, char *argv[])
{
    std::srand(std::time(0));

    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<PadGUI>("PadGUI", 1, 0, "PadGUI");

    QQuickStyle::setStyle("Default");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
