#include "ahpframefactory.h"

AHP::AHPFrameFactory* AHP::AHPFrameFactory::m_instance = 0;

AHP::AHPFrameFactory::AHPFrameFactory()
{
}

AHP::AHPFrameFactory* AHP::AHPFrameFactory::instance()
{
    if (!m_instance) {
        m_instance = new AHPFrameFactory();
    }
    return m_instance;
}

AHP::AHPFrame AHP::AHPFrameFactory::createFrame(const AHP::AHPCommand code,QByteArray payload)
{
    AHPFrame f;

    if (payload.size())
        f = AHPFrame(1,(quint8)code,payload.size(),payload,1); //TODO: Cambiar el valor del CRC por el calculo del crc con la formula
    else
        f = AHPFrame(1,(quint8)code,0,QByteArray(),1);

    return f;
}
