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

#include "makestatusengine.h"

#include <KDebug>

// The d-bus adaptor file.
#include "makestatusengineadaptor.h"

MakeStatusEngine::MakeStatusEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent, args),
      m_makeSourceKey("MakeSources"),
      m_totalProgressLiteral("TotalProgress")
{
    new MakeStatusEngineAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerObject("/MakeStatusEngine", this)) {
        kWarning() << "Failed to register D-Bus connection";
    }
}

MakeStatusEngine::~MakeStatusEngine()
{
}


void MakeStatusEngine::init()
{
    addMakeSource(m_totalProgressLiteral);
}

bool MakeStatusEngine::addMakeSource(const QString& path)
{
    if (query(m_makeSourceKey).contains(path)) {
        return false;
    }

    setData(m_makeSourceKey, path, int(0));

    kDebug() << "Added source " << path;
    return true;
}

bool MakeStatusEngine::removeMakeSource(const QString& path)
{
    if (!query(m_makeSourceKey).contains(path)) {
        return false;
    }

    removeData(m_makeSourceKey, path);
    updateTotalProgress();

    kDebug() << "Removed source " << path;
    return true;
}

bool MakeStatusEngine::setMakeProgress(const QString& path, int progress)
{
    if (progress < 0 || progress > 100) {

        return false;
    }

    if (!query(m_makeSourceKey).contains(path)) {
        return false;
    }

    setData(m_makeSourceKey, path, progress);

    kDebug() << "Request Progress for " << path << "to" << progress;

    // Avoid infinite indirect recursion.
    if (path != m_totalProgressLiteral) {
        updateTotalProgress();
    }

    return true;
}

void MakeStatusEngine::updateTotalProgress()
{
    int jobCount = 0;
    int totalJobProgress = 0;
    QHashIterator<QString, QVariant> i = query(m_makeSourceKey);

    while (i.hasNext()) {
        i.next();
        if (i.key() != m_totalProgressLiteral) {
            ++jobCount;
            totalJobProgress += i.value().toInt();
        }
    }

    if (jobCount)
        totalJobProgress /= jobCount;

    kDebug() << "Job count: " << jobCount
             << "Total job progress: " << totalJobProgress;

    setData(m_makeSourceKey, m_totalProgressLiteral, totalJobProgress);
}

bool MakeStatusEngine::sourceRequestEvent(const QString &name)
{
    Q_UNUSED(name);
    // We only add source through dbus api
    return false;
}

bool MakeStatusEngine::updateSourceEvent(const QString &name)
{
    Q_UNUSED(name);
    updateTotalProgress();
    return true;
}

K_EXPORT_PLASMA_DATAENGINE(makestatus, MakeStatusEngine)

#include "makestatusengine.moc"
