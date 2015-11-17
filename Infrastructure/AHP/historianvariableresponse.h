#ifndef HISTORIANVARIABLERESPONSE_H
#define HISTORIANVARIABLERESPONSE_H

#include <msgpack.hpp>
#include <iostream>
#include <vector>


#include "Common/globals.h"
#include "Domain/variablepoint.h"
#include "Infrastructure/iserializable.h"

namespace AHP {

class HistorianVariableResponse : public ISerializable
{

protected:
    std::vector<quint32> m_responseIds;
    std::vector<std::vector<VariablePoint> > m_responseData;

public:
    HistorianVariableResponse() = default;
    HistorianVariableResponse(const HistorianVariableResponse &) = default;
    ~HistorianVariableResponse() = default;

    HistorianVariableResponse(std::vector<quint32> responseId, std::vector<std::vector<VariablePoint> > responseData);

    std::vector<quint32> responseIds() const;
    void setResponseIds(const std::vector<quint32> &responseIds);
    std::vector<std::vector<VariablePoint> > responseData();
    void setResponseData(const std::vector<std::vector<VariablePoint> > &responseData);

    void  append(quint32 id, std::vector<VariablePoint> values);
    quint64 sizeAt(quint32 i);

    virtual bool serialize(Serializer *serializer, QByteArray *retValue);

//    std::ostream operator<< (std::ostream & out, const HistorianVariableResponse& vr);

    MSGPACK_DEFINE(m_responseIds,m_responseData)
};

}

Q_DECLARE_METATYPE(AHP::HistorianVariableResponse)

#endif // HISTORIANVARIABLERESPONSE_H
