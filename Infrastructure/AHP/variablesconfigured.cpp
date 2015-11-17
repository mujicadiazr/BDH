#include "variablesconfigured.h"
#include "../serializer.h"


AHP::VariablesConfigured::VariablesConfigured(std::vector<Variable> vars)
{
    m_vars = vars;
}

std::vector<Variable> AHP::VariablesConfigured::vars() const
{
    return m_vars;
}

void AHP::VariablesConfigured::setVars(const std::vector<Variable> &vars)
{
    m_vars = vars;
}

bool AHP::VariablesConfigured::serialize(Serializer *serializer, QByteArray *retValue)
{
    return serializer->serialize(*this,retValue);
}
