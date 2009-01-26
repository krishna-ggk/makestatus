/*
 *   Copyright 2009 Gopala Krishna A <krishna.ggk@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "makeinvoker.h"

#include <QtCore/QTimer>

#include <KAboutData>
#include <KApplication>
#include <KCmdLineArgs>

int main(int argc, char *argv[])
{
    KAboutData aboutData("makeinvoker", 0, ki18n("Make invoker"),
                         "0.1.0",
                         ki18n("D-Bus enabled make wrapper which acts as informer to makestatus dataengine"),
                         KAboutData::License_GPL,
                         ki18n("(c) 2009")
        );

    // The following doesn't accept make args. For now disable this
    // cmd line parsing entirely.
    int dummyArgc = 1;
    char *dummyArgv[1];
    dummyArgv[0] = new char[qstrlen(argv[0]) + 1];
    qstrcpy(dummyArgv[0], argv[0]);

    KCmdLineArgs::init(dummyArgc, dummyArgv, &aboutData);

    KApplication app(true);

    QStringList args;
    for(int i = 1; i < argc; ++i)
        args.append(QLatin1String(argv[i]));

    MakeInvoker invoker("make", args);

    QTimer::singleShot(1000, &invoker, SLOT(startProcess()));

    return app.exec();
}
