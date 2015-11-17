#include "realtimecache.h"
#include "qtconcurrentrun.h"

using namespace QtConcurrent;

RealtimeCache::RealtimeCache(QObject *parent) :
    QObject(parent)
{
    m_currentCache = 1;
    m_cachePointer = &m_cache1;

    m_intTemp = -123456789;
    m_doubleTemp = -1.23456789;
}

void RealtimeCache::persist(QList<VariablePoint> oldCache, RealtimeCache *ptr)
{
    quint32 len = oldCache.count();

    for (int i = 0; i < len; ++i) {
        VariablePoint vp = oldCache.at(i);

        emit ptr->persistData(vp.idVariable(), vp.quality(), vp.timeStamp(), vp.value());
     }
}

void RealtimeCache::storeVariablePoint(quint32 idVariable, quint8 quality, quint64 timeStamp, QVariant value)
{
    //! Guardo el valor a persistir si cambia el valor: Almacenamiento ciclico
    if (value.type() == QMetaType::LongLong){
        if (value.toLongLong() != m_intTemp) {
            m_intTemp = value.toLongLong();
            m_cachePointer->push_back(VariablePoint(idVariable,quality,timeStamp,value.toLongLong(),INTEGER));
        }
    }
    else {
        if (value.toDouble() != m_doubleTemp) {
            m_doubleTemp = value.toDouble();
            m_cachePointer->push_back(VariablePoint(idVariable,quality,timeStamp,value.toDouble(),DOUBLE));
        }
    }

    //! Difundo la muestra a los clientes de tiempo real que la este solicitando
    emit storedVariablePoint(idVariable, quality, timeStamp, value);

    //! Persisto si se llena la cache actual y lo hago en otro hilo
     if (cacheFull()) {
        swapCache();
        QList<VariablePoint> vars = *oldCache();
        run(RealtimeCache::persist,vars,this); //! Corro la funcion persist en otro hilo

        oldCache()->clear();
    }
}

bool RealtimeCache::cacheFull()
{
    return m_cachePointer->size() == MAX_CACHE_VALUES;
}

void RealtimeCache::swapCache()
{
    if (m_currentCache == 1) {
        m_currentCache = 2;
        m_cachePointer = &m_cache2;
    } else {
        m_currentCache = 1;
        m_cachePointer = &m_cache1;
    }
}

QList<VariablePoint>* RealtimeCache::currentCache()
{
    return (m_currentCache == 1) ? &m_cache1 : &m_cache2;
}

QList<VariablePoint> *RealtimeCache::oldCache()
{
    return (m_currentCache == 2) ? &m_cache1 : &m_cache2;
}
