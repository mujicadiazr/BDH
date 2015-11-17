#include "historianvariableresponse.h"
#include "../serializer.h"


AHP::HistorianVariableResponse::HistorianVariableResponse(std::vector<quint32> responseId, std::vector<std::vector<VariablePoint> > responseData)
    :ISerializable(), m_responseIds(responseId), m_responseData(responseData)
{
}

std::vector<quint32> AHP::HistorianVariableResponse::responseIds() const
{
    return m_responseIds;
}

void AHP::HistorianVariableResponse::setResponseIds(const std::vector<quint32> &responseIds)
{
    m_responseIds = responseIds;
}

std::vector<std::vector<VariablePoint> > AHP::HistorianVariableResponse::responseData()
{
    return m_responseData;
}

void AHP::HistorianVariableResponse::setResponseData(const std::vector<std::vector<VariablePoint> > &responseData)
{
    m_responseData = responseData;
}

void AHP::HistorianVariableResponse::append(quint32 id, std::vector<VariablePoint> values)
{
    m_responseIds.push_back(id);
    m_responseData.push_back(values);
}

quint64 AHP::HistorianVariableResponse::sizeAt(quint32 i)
{
    std::vector<VariablePoint> temp = m_responseData.at(i);
    return temp.size();
}

bool AHP::HistorianVariableResponse::serialize(Serializer *serializer, QByteArray *retValue)
{
    return serializer->serialize(*this,retValue);
}

//std::ostream AHP::HistorianVariableResponse::operator<<(std::ostream &out, const AHP::HistorianVariableResponse &vr)
//{

//    for (std::map<quint32, std::vector<VariablePoint> >::iterator it=vr.data().begin(); it!=vr.data().end(); ++it)
//        out << std::string("ID Variable: ") << (*it).first << "\n" << (*it).second;

//    return out;
//}



