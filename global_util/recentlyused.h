#ifndef RECENTLYUSED_H
#define RECENTLYUSED_H

#include <QObject>
#include <QXmlStreamReader>

#include <memory>

class RecentlyUsed : public QObject
{
    Q_OBJECT

public:
    explicit RecentlyUsed(QObject *parent = 0);

    const QList<QString> history() const { return m_history; }

signals:
    void dataChanged(const QList<QString> &history) const;

public slots:
    void clear();
    void reload();

private:
    std::unique_ptr<QXmlStreamReader> m_xmlRdr;

    QList<QString> m_history;
};

#endif // RECENTLYUSED_H
