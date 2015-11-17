#include "historianvariablerequest.h"
#include "../serializer.h"

bool AHP::HistorianVariableRequest::serialize(Serializer *serializer, QByteArray *retValue)
{
    return serializer->serialize(*this,retValue);
}

AHP::HistorianVariableRequest::HistorianVariableRequest(std::vector<quint32> varsId, std::vector<std::pair<quint64, quint64> > timeRanges)
    : VariableRequest(varsId), m_timeRanges(timeRanges)
{
}

std::vector<std::pair<quint64, quint64> > AHP::HistorianVariableRequest::timeRanges() const
{
    return m_timeRanges;
}

void AHP::HistorianVariableRequest::setTimeRanges(const std::vector<std::pair<quint64, quint64> > &timeRanges)
{
    m_timeRanges = timeRanges;
}

void AHP::HistorianVariableRequest::appendRange(std::pair<quint64, quint64> range)
{
    m_timeRanges.push_back(range);
}

void AHP::HistorianVariableRequest::clear()
{
    clearIds();
    m_timeRanges.clear();
}
