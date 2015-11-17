#ifndef HISTORIANVARIABLEREQUEST_H
#define HISTORIANVARIABLEREQUEST_H

#include <msgpack.hpp>

#include "variablerequest.h"
#include "Infrastructure/iserializable.h"
#include <utility>

namespace AHP {

class HistorianVariableRequest : public VariableRequest
{

protected:
    std::vector<std::pair<quint64,quint64> > m_timeRanges;

public:
    HistorianVariableRequest() = default;
    HistorianVariableRequest(const HistorianVariableRequest&) = default;
    ~HistorianVariableRequest() = default;

    HistorianVariableRequest(std::vector<quint32> varsId, std::vector<std::pair<quint64,quint64> > timeRanges);

    virtual bool serialize(Serializer *serializer, QByteArray *retValue=NULL);

    std::vector<std::pair<quint64, quint64> > timeRanges() const;
    void setTimeRanges(const std::vector<std::pair<quint64, quint64> > &timeRanges);
    void appendRange(std::pair<quint64,quint64> range);
    void clear();

    MSGPACK_DEFINE(m_varsId,m_timeRanges)
};

}
#endif // HISTORIANVARIABLEREQUEST_H
