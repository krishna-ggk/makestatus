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

#ifndef MAKESTATUSENGINE_H
#define MAKESTATUSENGINE_H

#include <Plasma/DataEngine>

/**
 * This engine provides the progress information of make jobs. The
 * percentage is reported for cmake based Makefiles. However, the make
 * should be invoked indirectly using \b makeinvoker wrapper program.
 *
 * MakeStatusEngine provides a D-Bus interace, where all its slots are
 * accesible through d-bus. \b makeinvoker informs MakeStatusEngine
 * about the make progress using d-bus calls.
 */
class MakeStatusEngine : public Plasma::DataEngine
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.MakeStatusEngine")
public:
    MakeStatusEngine(QObject* parent, const QVariantList& args);
    virtual ~MakeStatusEngine();

    virtual void init();

public Q_SLOTS:
    // The following slots are mostly invoked by makeinvoker through
    // d-bus.

    /**
     * Call this method to register a make job in the dataengine. This
     * method also updates the total progress.
     *
     * @param path Every make job is identified by the path of the
     *             Makefile involved in the make.
     *
     * @retval true  If path isn't already registered.
     * @retval false If path is already registered.
     */
    bool addMakeSource(const QString& path);

    /**
     * Call this method to un-register a make job. This method also
     * updates the total progress.
     *
     * @param path Every make job is identified by the path of the
     *             Makefile involved in the make.
     *
     * @retval true  If path is registered before.
     * @retval false If path isn't registered.
     */
    bool removeMakeSource(const QString& path);

    /**
     * Call this method to set the progress of a make job. This method
     * also updates the total progress.
     *
     * @param path Every make job is identified by the path of the
     *             Makefile involved in the make.
     *
     * @param progress The progress value (0-100)
     *
     * @retval true  If the path is registered.
     *
     * @retval false If the path isn't registered or progress is not
     *               in range (0 - 100)
     */
    bool setMakeProgress(const QString& path, int progress);

protected:
    bool sourceRequestEvent(const QString& name);
    bool updateSourceEvent(const QString& source);

private:
    /**
     * This method calculates and updates the \i "TotalProgress" -
     * which is a special data source.
     */
    void updateTotalProgress();

    /**
     * The data source in which the make progress data is published.
     * Value = "MakeSoruces"
     */
    const QString m_makeSourceKey;
    /**
     * Special data in m_makeSourceKey data source which publishes the
     * total progress.
     * Value = "TotalProgress"
     */
    const QString m_totalProgressLiteral;
};

#endif // MAKESTATUSENGINE_H
