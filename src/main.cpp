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

QString getCommandLineUrlArgument()
{
    const QStringList args = QCoreApplication::arguments();
    if (args.count() > 1) {
        const QString lastArg = args.last();
        const bool isValidUrl = QUrl::fromUserInput(lastArg).isValid();
        if (isValidUrl)
            return lastArg;
    }
    return QString();
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    QtWebEngine::initialize();

    const QString url = getCommandLineUrlArgument();
    if (url.isEmpty()) {
        return 1;
    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("destURL", url);
    engine.load(QUrl("qrc:/ui.qml"));
    QObject::connect(&engine, SIGNAL(quit()), qApp, SLOT(quit()));

    return app.exec();
}
