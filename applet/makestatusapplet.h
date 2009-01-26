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

#ifndef MAKESTATUSAPPLET_H
#define MAKESTATUSAPPLET_H

#include <Plasma/Applet>
#include <Plasma/DataEngine>

/**
 * This is a simple visualization for the MakeStatusEngine which
 * displays the progress information of the make jobs.
 */
class MakeStatusApplet : public Plasma::Applet
{
    Q_OBJECT
public:
    MakeStatusApplet(QObject *parent, const QVariantList& args);
    virtual ~MakeStatusApplet();

    virtual void paintInterface(QPainter *painter,
                        const QStyleOptionGraphicsItem* opt,
                        const QRect& contentsRect);

    virtual void init();

public Q_SLOTS:
    void dataUpdated(const QString& name, const Plasma::DataEngine::Data &data);

protected Q_SLOTS:
    void constraintsEvent(Plasma::Constraints constraints);

private:
    int calculateFontSize(const QRect &rect);
    void updateSize();

    QString m_progressText;
    QString m_jobText;
    const QString m_makeSourceKey;
    QFont m_font;
    int m_cachedFontSize;
    QRect m_cachedContentsRect;
};

#endif
