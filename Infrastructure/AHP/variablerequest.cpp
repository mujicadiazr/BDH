#include "variablerequest.h"
#include "../serializer.h"

AHP::VariableRequest::VariableRequest(std::vector<quint32> varsId)
{
    m_varsId = varsId;
}

std::vector<quint32> AHP::VariableRequest::varsId() const
{
    return m_varsId;
}

void AHP::VariableRequest::clearIds()
{
    m_varsId.clear();
}

void AHP::VariableRequest::setVarsId(const std::vector<quint32> &varsId)
{
    m_varsId = varsId;
}

void AHP::VariableRequest::appendIdVariable(quint32 variableId)
{
    m_varsId.push_back(variableId);
}
