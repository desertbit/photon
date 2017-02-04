/*
 *  Photon
 *  Copyright (C) 2016  Roland Singer <roland.singer[at]desertbit.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QQmlContext>
#include <QtWebEngine/qtwebengineglobal.h>
#include <QQuickWindow>
#include <QStringList>
#include <iostream>

void printAscii()
{
    std::cout << R"(       _           _
 _ __ | |__   ___ | |_ ___  _ __
| '_ \| '_ \ / _ \| __/ _ \| '_ \
| |_) | | | | (_) | || (_) | | | |
| .__/|_| |_|\___/ \__\___/|_| |_|
|_|               Lightweight webUI Framework

)" ;
}

void printHelp()
{
    printAscii();
    printf("usage: ./photon [-fullscreen] [-title <title>] [-disableHighDPI] -url <URL>\n");
    printf("    -f | -fullscreen   start in fullscreen mode\n");
    printf("    -u | -url          supply URL\n");
    printf("    -h | -help         print this help\n");
    printf("    -t | -title        set window title\n");
    printf("    -disableHighDPI    disable high DPI scaling\n");
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QtWebEngine::initialize();

    bool highDPI = true;
    bool fullscreenMode = false;
    QString url;
    QString title = "Photon";

    QStringList args = QCoreApplication::arguments();
    for( int i=0; i<args.count(); ++i )
    { 
        if (args[i] == "-fullscreen" || args[i] == "-f") {
            fullscreenMode = true;
        } else if (args[i] == "-url" || args[i] == "-u") {
            const bool isValidUrl = QUrl::fromUserInput(args[i+1]).isValid();
            if (!isValidUrl) {
                printf("[ERROR] invalid URL\n");
                return 1;
            }
            url = args[i+1];
        } else if (args[i] == "-help" || args[i] == "-h") {
            printHelp();
            return 0;
        } else if (args[i] == "-disableHighDPI") {
           highDPI = false; 
        } else if (args[i] == "-title" || args[i] == "-t") {
            title = args[i+1];
        }
    }

    if (highDPI) {
        app.setAttribute(Qt::AA_EnableHighDpiScaling);
    }

    if (url == "") {
        printHelp();
        printf("[ERROR] no URL supplied. use -url flag\n");
        return 1;
    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("destURL", url);
    engine.rootContext()->setContextProperty("windowTitle", title);
    engine.load(QUrl("qrc:/ui.qml"));
    QObject::connect(&engine, SIGNAL(quit()), qApp, SLOT(quit()));

    if (fullscreenMode) {
        QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
        window->showFullScreen();
    }

    return app.exec();
}
