#ifndef AHPFRAME_H
#define AHPFRAME_H

#include <QByteArray>

#include "Common/globals.h"

namespace AHP {

class AHPFrame
{
    quint8 m_version;
    quint8 m_code;
    quint32 m_size;
    QByteArray m_payload;
    quint32 m_crc;

public:
    AHPFrame();
    AHPFrame(quint8 version, quint8 code, quint32 size, QByteArray payload, quint32 crc);
    QString getAsQString() {
        return QString("VERSION: %1\n CODE: %2\n SIZE: %3\n PYLOAD: %4\n").arg(m_version).arg(m_code).arg(m_size).arg(QString(m_payload));
    }

    quint8 version() const;
    void setVersion(const quint8 &version);
    quint8 code() const;
    void setCode(const quint8 &code);
    quint32 size() const;
    void setSize(const quint32 &size);
    QByteArray payload() const;
    void setPayload(const QByteArray &payload);
    quint32 crc() const;
    void setCrc(const quint32 &crc);
};

}
#endif // AHPFRAME_H
