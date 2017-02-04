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
    printf("usage: ./photon -url <URL> [-fullscreen] [-title <title>] [-width <width>] [-height <height>] [-disableHighDPI]\n");
    printf("    -f | -fullscreen   launch in fullscreen mode\n");
    printf("    -u | -url          supply URL\n");
    printf("    -h | -help         print this help\n");
    printf("    -t | -title        set window title\n");
    printf("    -width             set window width\n");
    printf("    -height            set window height\n");
    printf("    -disableHighDPI    disable high DPI scaling\n");
}

int main(int argc, char **argv)
{
    // init QtWebEngine
    QGuiApplication app(argc, argv);
    QtWebEngine::initialize();

    // defaults
    QString title = "Photon";
    int width = 1000;
    int height = 1000;
    bool highDPI = true;
    bool fullscreenMode = false;
    QString url;

    // handle commandline arguments
    QStringList args = QCoreApplication::arguments();
    for( int i=0; i<args.count(); ++i )
    { 
        if (args[i] == "-fullscreen" || args[i] == "-f") {
            fullscreenMode = true;
        } else if (args[i] == "-url" || args[i] == "-u") {
            if (i+1 >= args.count()) {
                printf("[ERROR] missing URL value\n");
                return 1;
            }
            bool hasHttpPrefix = args[i+1].startsWith("http://");
            const bool isValidUrl = QUrl::fromUserInput(args[i+1]).isValid();
            if (!isValidUrl || !hasHttpPrefix) {
                printf("[ERROR] invalid URL: %s\n", args[i+1].toUtf8().constData());
                return 1;
            }
            url = args[i+1];
        } else if (args[i] == "-help" || args[i] == "-h") {
            printHelp();
            return 0;
        } else if (args[i] == "-disableHighDPI") {
           highDPI = false; 
        } else if (args[i] == "-title" || args[i] == "-t") {
            if (i+1 >= args.count()) {
                printf("[ERROR] missing title value\n");
                return 1;
            }
            bool startsWithDash = args[i+1].startsWith("-");
            if (startsWithDash) {
                printf("[ERROR] value for title seems to be missing. found another flag instead: %s\n", args[i+1].toUtf8().constData());
                return 1;
            }
            title = args[i+1];
        } else if (args[i] == "-width") {
            if (i+1 >= args.count()) {
                printf("[ERROR] missing width value\n");
                return 1;
            }
            bool ok;
            int val = args[i+1].toInt(&ok);
            if (!ok) {
                printf("[ERROR] width expects an integer value, got: %s\n", args[i+1].toUtf8().constData());
                return 1;
            }
            width = val;
        } else if (args[i] == "-height") {
            if (i+1 >= args.count()) {
                printf("[ERROR] missing height value\n");
                return 1;
            }
            bool ok;
            int val = args[i+1].toInt(&ok);
            if (!ok) {
                printf("[ERROR] height expects an integer value, got: %s\n", args[i+1].toUtf8().constData());
                return 1;
            }
            height = val;
        }
    }

    // check if an URL was supplied
    if (url == "") {
        printHelp();
        printf("[ERROR] no URL supplied. use -url flag\n");
        return 1;
    }
    
    qDebug("[INFO] starting photon app. URL: %s", url.toUtf8().constData());
    
    // handle highDPI attribute
    if (highDPI) {
        app.setAttribute(Qt::AA_EnableHighDpiScaling);
    }

    // setup engine
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("destURL", url);
    engine.rootContext()->setContextProperty("windowTitle", title);
    engine.rootContext()->setContextProperty("windowWidth", width);
    engine.rootContext()->setContextProperty("windowHeight", height);

    // load QML
    engine.load(QUrl("qrc:/ui.qml"));

    // connect engine
    QObject::connect(&engine, SIGNAL(quit()), qApp, SLOT(quit()));

    // handle fullscreen mode
    if (fullscreenMode) {
        QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
        window->showFullScreen();
    }

    // lets go
    return app.exec();
}
