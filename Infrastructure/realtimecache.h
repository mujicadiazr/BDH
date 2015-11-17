#ifndef REALTIMECACHE_H
#define REALTIMECACHE_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QThread>

#include "Common/globals.h"
#include "Domain/variablepoint.h"

#define MAX_CACHE_VALUES 100

class RealtimeCache : public QObject
{
    Q_OBJECT
public:
    explicit RealtimeCache(QObject *parent = 0);

    static void persist(QList<VariablePoint> oldCache, RealtimeCache *ptr);

signals:
    void storedVariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value);
    void persistData(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value);

public slots:
    void storeVariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value);

private:
    bool cacheFull();
    void swapCache();
    QList<VariablePoint> *currentCache();
    QList<VariablePoint>  *oldCache();

    QList<VariablePoint> m_cache1;
    QList<VariablePoint> m_cache2;
    QList<VariablePoint> *m_cachePointer;
    quint8 m_currentCache;

    quint64 m_intTemp;
    qreal   m_doubleTemp;

};

#endif // REALTIMECACHE_H
