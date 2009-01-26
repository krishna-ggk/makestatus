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

#include "makestatusapplet.h"

#include <QFontMetrics>
#include <QPainter>
#include <QTextOption>

#include <KGlobalSettings>
#include <KIconLoader>
#include <KDebug>

MakeStatusApplet::MakeStatusApplet(QObject *parent, const QVariantList& args) :
    Plasma::Applet(parent, args),
    m_makeSourceKey("MakeSources"),
    m_font(font())
{
    m_cachedFontSize = m_font.pointSize();
    setBackgroundHints(NoBackground);
    m_font.setBold(true);
    resize(160, 120);
}

MakeStatusApplet::~MakeStatusApplet()
{
    if (hasFailedToLaunch()) {
        kWarning() << "Failed to launch";
    }
    else {
        // Add settings code
    }
}

void MakeStatusApplet::init()
{
    updateSize();
    // Setup dataengine connection
    dataEngine("makestatus")->connectSource(m_makeSourceKey, this);
    resize(minimumSize());
}

void MakeStatusApplet::paintInterface(QPainter *painter,
                                      const QStyleOptionGraphicsItem* opt,
                                      const QRect& contentsRect)
{
    Q_UNUSED(opt);

    if (m_cachedContentsRect != contentsRect) {
        m_cachedContentsRect = contentsRect;
        m_cachedFontSize = calculateFontSize(m_cachedContentsRect);
    }

    painter->setPen(Qt::white);
    painter->setRenderHint(QPainter::Antialiasing);

    QFont fnt(m_font);
    QTextOption textOption(Qt::AlignHCenter | Qt::AlignTop);
    textOption.setWrapMode(QTextOption::NoWrap);

    fnt.setPointSize(m_cachedFontSize);
    painter->setFont(fnt);
    painter->drawText(contentsRect, m_progressText, textOption);


    fnt.setPointSize(m_cachedFontSize - 1);
    textOption.setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    painter->setFont(fnt);
    painter->drawText(contentsRect, m_jobText, textOption);
}

void MakeStatusApplet::dataUpdated(const QString& name, const Plasma::DataEngine::Data &data)
{
    if (name == m_makeSourceKey) {
        m_progressText = i18n("%1 %").arg(data["TotalProgress"].toInt());
        m_jobText = i18n("%1 jobs").arg(data.count()-1);
        update();
    }
}

void MakeStatusApplet::constraintsEvent(Plasma::Constraints constraints)
{
    Q_UNUSED(constraints);
    // For now nothing to do.
}

int MakeStatusApplet::calculateFontSize(const QRect &rect)
{
    QFont fnt(m_font);

    const QString percentString(i18n("000 %"));
    const QString jobString(i18n("22 jobs"));

    int pointSize = 2;
    while(true) {
        fnt.setPointSize(pointSize);
        QRect percentRect = QFontMetrics(fnt).boundingRect(percentString);
        fnt.setPointSize(pointSize-1);
        QRect jobRect = QFontMetrics(fnt).boundingRect(jobString);

        int width = qMax(percentRect.width(), jobRect.width());
        int height = percentRect.height() + jobRect.height();

        if (width >= rect.width() || height >= rect.height()) {
            --pointSize; // prev point size best fits the rect
            break;
        }
        ++pointSize;
    }

    return pointSize;
}

void MakeStatusApplet::updateSize()
{
    // First setup minimum sizes
    QFont fnt(m_font);
    const QString percentString(i18n("000 %"));
    const QString jobString(i18n("22 jobs"));

    m_cachedFontSize = 5; // ensure minimum 5 point size
    fnt.setPointSize(m_cachedFontSize);
    QRect percentRect = QFontMetrics(fnt).boundingRect(percentString);
    fnt.setPointSize(m_cachedFontSize-1);
    QRect jobRect = QFontMetrics(fnt).boundingRect(jobString);

    int width = qMax(percentRect.width(), jobRect.width());
    int height = percentRect.height() + jobRect.height();

    setMinimumSize(QSize(width, height));
}

K_EXPORT_PLASMA_APPLET(makestatus_app, MakeStatusApplet)

#include "makestatusapplet.moc"
