#ifndef AHPFRAMEFACTORY_H
#define AHPFRAMEFACTORY_H

#include "ahpframe.h"
#include "ahp_commands_enum.h"

namespace AHP{

class AHPFrameFactory
{
    static AHPFrameFactory *m_instance;
protected:
    AHPFrameFactory();
public:
    static AHPFrameFactory* instance();

    AHPFrame createFrame(const AHPCommand code, QByteArray payload = QByteArray());


};

}

#endif // AHPFRAMEFACTORY_H
