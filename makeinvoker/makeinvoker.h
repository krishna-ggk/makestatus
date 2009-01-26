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

#ifndef MAKEINVOKER_H
#define MAKEINVOKER_H

#include <QtCore/QObject>
#include <KProcess>

class QDBusInterface;

class MakeInvoker : public QObject
{
    Q_OBJECT

public:
    MakeInvoker(const QString& command, const QStringList& args, QObject *parent = 0);
    virtual ~MakeInvoker();

public Q_SLOTS:
    void startProcess();
    void terminateProcess();
    void killProcess();

private Q_SLOTS:
    void readStandardOutput();
    void readStandardError();
    void onFinish(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess *m_process;
    QString m_command;
    QStringList m_arguments;

    QString m_identifier;
    QDBusInterface *m_dbusInterface;

    int m_errorCount;
    int m_warningCount;
};

#endif
