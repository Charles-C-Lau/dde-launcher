#include "appsmanager.h"

#include <QDebug>
#include <QX11Info>

AppsManager *AppsManager::INSTANCE = nullptr;

QSettings AppsManager::APP_ICON_CACHE("deepin", "dde-launcher-app-icon", nullptr);
QSettings AppsManager::APP_AUTOSTART_CACHE("deepin", "dde-launcher-app-autostart", nullptr);
//QSettings AppsManager::AppInfoCache("deepin", "dde-launcher-app-info", nullptr);

AppsManager::AppsManager(QObject *parent) :
    QObject(parent),
    m_launcherInter(new DBusLauncher(this)),
    m_fileInfoInter(new DBusFileInfo(this)),
    m_startManagerInter(new DBusStartManager(this)),
    m_dockedAppInter(new DBusDockedAppManager(this))
{
//    refreshAppIconCache();
//    refreshAppAutoStartCache();

    m_appInfoList = m_launcherInter->GetAllItemInfos().value();
    refreshCategoryInfoList();

    m_newInstalledAppsList = m_launcherInter->GetAllNewInstalledApps().value();

    connect(m_launcherInter, &DBusLauncher::SearchDone, this, &AppsManager::searchDone);
    connect(this, &AppsManager::handleUninstallApp, this, &AppsManager::unInstallApp);
}

void AppsManager::appendSearchResult(const QString &appKey)
{
    for (const ItemInfo &info : m_appInfoList)
        if (info.m_key == appKey)
            return m_appSearchResultList.append(info);
}

AppsManager *AppsManager::instance(QObject *parent)
{
    if (INSTANCE)
        return INSTANCE;

    INSTANCE = new AppsManager(parent);
    return INSTANCE;
}

void AppsManager::searchApp(const QString &keywords)
{
    m_launcherInter->Search(keywords);
}

void AppsManager::launchApp(const QModelIndex &index)
{
    m_startManagerInter->LaunchWithTimestamp(index.data(AppsListModel::AppDesktopRole).toString(), QX11Info::getTimestamp());
}

const ItemInfoList AppsManager::appsInfoList(const AppsListModel::AppCategory &category) const
{
    switch (category)
    {
    case AppsListModel::Custom:
    case AppsListModel::All:        return m_appInfoList;           break;
    case AppsListModel::Search:     return m_appSearchResultList;   break;
    default:;
    }

    return m_appInfos[category];
}

bool AppsManager::appIsNewInstall(const QString &key)
{
    return m_newInstalledAppsList.contains(key);
}

bool AppsManager::appIsAutoStart(const QString &desktop)
{
    if (APP_AUTOSTART_CACHE.contains(desktop))
        return APP_AUTOSTART_CACHE.value(desktop).toBool();

    const bool isAutoStart = m_startManagerInter->IsAutostart(desktop).value();

    APP_AUTOSTART_CACHE.setValue(desktop, isAutoStart);

    return isAutoStart;
}

bool AppsManager::appIsOnDock(const QString &appName)
{
    return m_dockedAppInter->IsDocked(appName).value();
}

bool AppsManager::appIsOnDesktop(const QString &desktop)
{
    return m_launcherInter->IsItemOnDesktop(desktop).value();
}

const QPixmap AppsManager::appIcon(const QString &desktop, const int size)
{
    const QString cacheKey = QString("%1-%2").arg(desktop)
                                             .arg(size);

    const QPixmap cachePixmap = APP_ICON_CACHE.value(cacheKey).value<QPixmap>();
    if (!cachePixmap.isNull())
        return cachePixmap;

    // cache fail
    const QString iconFile = m_fileInfoInter->GetThemeIcon(desktop, size).value();
    QPixmap iconPixmap = QPixmap(iconFile);

    if (iconPixmap.isNull())
        iconPixmap = QPixmap(":/skin/images/application-default-icon.svg");

    APP_ICON_CACHE.setValue(cacheKey, iconPixmap);

    return iconPixmap;
}

void AppsManager::refreshCategoryInfoList()
{
    m_appInfos.clear();

    for (const ItemInfo &info : m_appInfoList)
    {
        const AppsListModel::AppCategory category = info.category();
        if (!m_appInfos.contains(category))
            m_appInfos.insert(category, ItemInfoList());

        m_appInfos[category].append(info);
    }
}

int AppsManager::appNums(const AppsListModel::AppCategory &category) const
{
    return appsInfoList(category).size();
}

void AppsManager::unInstallApp(const QModelIndex &index, int value) {
    QString appKey = index.data(AppsListModel::AppKeyRole).toString();
    if (value==1) {
        // begin to unInstall app;
        QDBusPendingReply<> reply = m_launcherInter->RequestUninstall(appKey, false);
        if (!reply.isError()) {
               qDebug() << "unistall function excute finished!";
           } else {
               qDebug() << "unistall action fail, and the error reason:" << reply.error().message();
           }
    } else {
        //cancle to unInstall app;
        qDebug() << "cancle to unInstall app" << appKey;
    }
}

void AppsManager::refreshAppIconCache()
{
    APP_ICON_CACHE.sync();
    APP_ICON_CACHE.clear();
}

void AppsManager::refreshAppAutoStartCache()
{
    APP_AUTOSTART_CACHE.sync();
    APP_AUTOSTART_CACHE.clear();
}

void AppsManager::searchDone(const QStringList &resultList)
{
    m_appSearchResultList.clear();

    for (const QString &key : resultList)
        appendSearchResult(key);

    emit dataChanged(AppsListModel::Search);
}
