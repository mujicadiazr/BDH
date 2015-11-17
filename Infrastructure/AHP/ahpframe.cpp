#include "ahpframe.h"

AHP::AHPFrame::AHPFrame()
{
    m_version = 0;
    m_code = 0;
    m_size = 0;
    m_payload = QByteArray();
    m_crc = 0;
}

AHP::AHPFrame::AHPFrame(quint8 version, quint8 code, quint32 size, QByteArray payload, quint32 crc)
{
    m_version = version;
    m_code = code;
    m_size = size;
    m_payload = payload;
    m_crc = crc;
}

quint8 AHP::AHPFrame::version() const
{
    return m_version;
}

void AHP::AHPFrame::setVersion(const quint8 &version)
{
    m_version = version;
}

quint8 AHP::AHPFrame::code() const
{
    return m_code;
}

void AHP::AHPFrame::setCode(const quint8 &code)
{
    m_code = code;
}

quint32 AHP::AHPFrame::size() const
{
    return m_size;
}

void AHP::AHPFrame::setSize(const quint32 &size)
{
    m_size = size;
}

QByteArray AHP::AHPFrame::payload() const
{
    return m_payload;
}

void AHP::AHPFrame::setPayload(const QByteArray &payload)
{
    m_payload = payload;
}

quint32 AHP::AHPFrame::crc() const
{
    return m_crc;
}

void AHP::AHPFrame::setCrc(const quint32 &crc)
{
    m_crc = crc;
}

