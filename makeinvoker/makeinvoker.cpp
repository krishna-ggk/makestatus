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

#include <QDBusInterface>
#include <QDir>

#include <KAboutData>
#include <KApplication>
#include <KDebug>
#include <KNotification>

#include <cstdio>

MakeInvoker::MakeInvoker(const QString& command, const QStringList& args, QObject *parent) :
    QObject(parent),
    m_command(command),
    m_arguments(args),
    m_errorCount(0),
    m_warningCount(0)
{
    m_process = new QProcess(this);
    m_process->setEnvironment(m_process->systemEnvironment());

    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinish(int, QProcess::ExitStatus)));

    m_identifier = QDir::currentPath();
    m_dbusInterface = new QDBusInterface("org.kde.plasma", "/MakeStatusEngine", "org.MakeStatusEngine");
    m_dbusInterface->call("addMakeSource", m_identifier);
}

MakeInvoker::~MakeInvoker()
{

}

void MakeInvoker::startProcess()
{
    m_process->start(m_command, m_arguments);
}

void MakeInvoker::terminateProcess()
{
    m_process->terminate();
}

void MakeInvoker::killProcess()
{
    m_process->kill();
}

void MakeInvoker::readStandardOutput()
{
    QByteArray output = m_process->readAllStandardOutput();
    int start = output.indexOf('[');
    int end = output.indexOf(']');

    if (start == 0 && end == 5) {
        bool ok;
        int percent = output.mid(1, 3).trimmed().toInt(&ok);
        if (ok) {
            m_dbusInterface->call("setMakeProgress", m_identifier, percent);
        }
    }

    std::printf("%s", output.constData());
}

void MakeInvoker::readStandardError()
{
    QByteArray output = m_process->readAllStandardError();
    if (output.indexOf(QByteArray("warning")) != -1)
        m_warningCount++;
    else if (output.indexOf(QByteArray("error")) != -1)
        m_errorCount++;

    std::printf("%s", output.constData());
}

void MakeInvoker::onFinish(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    int k;
    m_dbusInterface->call("removeMakeSource", m_identifier);

    KNotification *notify = 0;

    if (exitStatus == QProcess::CrashExit) {
        kDebug() << "Crashed";
        notify = new KNotification("MakeCrashed");
        notify->setText(i18n("Make crashed!"));
    }
    else if (m_errorCount != 0) {
        kDebug() << "Some errors";
        notify = new KNotification("MakeCompletedWithErrors");
        QString text = i18n("Make completed with %1 errors").arg(m_errorCount);
        if (m_warningCount != 0) {
            text.append(i18n(" and %1 warnings").arg(m_warningCount));
        }
        notify->setText(text);
    }
    else if (m_warningCount != 0) {
        kDebug() << "Some warnings";
        notify = new KNotification("MakeCompletedWithWarnings");
        notify->setText(i18n("Make completed with %1 warnings").arg(m_warningCount));
    }
    else {
        kDebug() << "Everything's fine";
        notify = new KNotification("MakeCompleted");
        notify->setText(i18n("Make completed succesfully without any errors or warnings"));
    }
    notify->sendEvent();

    kapp->exit(0);
}
