#ifndef APPSMANAGER_H
#define APPSMANAGER_H

#include "appslistmodel.h"
#include "../dbus/dbuslauncher.h"
#include "../dbus/dbusfileinfo.h"

#include <QMap>
#include <QSettings>

class AppsManager : public QObject
{
    Q_OBJECT

public:
    static AppsManager *instance(QObject *parent = nullptr);

    void removeRow(const int row);
    const ItemInfoList appsInfoList(const AppsListModel::AppCategory &category) const;

    const QPixmap appIcon(const QString &desktop, const int size);

private:
    explicit AppsManager(QObject *parent = 0);

    void refreshCategoryInfoList();
    void refreshAppIconCache();

private:
    DBusLauncher *m_launcherInter;
    DBusFileInfo *m_fileInfoInter;

    ItemInfoList m_appInfoList;
    ItemInfoList m_appSearchResultList;
    QMap<AppsListModel::AppCategory, ItemInfoList> m_appInfos;

    static AppsManager *INSTANCE;
    static QSettings APP_ICON_CACHE;
//    static QSettings AppInfoCache;
};

#endif // APPSMANAGER_H
