/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c ControlCenter -p dbuscontrolcenter.h:dbuscontrolcenter.cpp controlcenter.xml
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef DBUSCONTROLCENTER_H
#define DBUSCONTROLCENTER_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.deepin.dde.ControlCenter
 */
class DBusControlCenter: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.dde.ControlCenter"; }

public:
    DBusControlCenter(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~DBusControlCenter();

    Q_PROPERTY(QRect Rect READ rect)
    inline QRect rect() const
    { return qvariant_cast< QRect >(property("Rect")); }

    Q_PROPERTY(bool ShowInRight READ showInRight)
    inline bool showInRight() const
    { return qvariant_cast< bool >(property("ShowInRight")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Hide()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Hide"), argumentList);
    }

    inline QDBusPendingReply<> HideImmediately()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("HideImmediately"), argumentList);
    }

    inline QDBusPendingReply<> Show()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Show"), argumentList);
    }

    inline QDBusPendingReply<> ShowImmediately()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ShowImmediately"), argumentList);
    }

    inline QDBusPendingReply<> ShowModule(const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(name);
        return asyncCallWithArgumentList(QStringLiteral("ShowModule"), argumentList);
    }

    inline QDBusPendingReply<> Toggle()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Toggle"), argumentList);
    }

    inline QDBusPendingReply<> ToggleInLeft()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ToggleInLeft"), argumentList);
    }

    inline QDBusPendingReply<bool> isNetworkCanShowPassword()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("isNetworkCanShowPassword"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void rectChanged(const QRect &rect);
    void destRectChanged(const QRect &rect);
};

namespace com {
  namespace deepin {
    namespace dde {
      typedef ::DBusControlCenter ControlCenter;
    }
  }
}
#endif
