#ifndef HISTORIANALARMRESPONSE_H
#define HISTORIANALARMRESPONSE_H

#include <msgpack.hpp>

#include "Infrastructure/iserializable.h"
#include "Domain/alarmpoint.h"

namespace AHP {
class HistorianAlarmResponse : public ISerializable {
 public:
  bool serialize(Serializer *serializer, QByteArray *retValue);
  void append(quint32 id, std::vector<AlarmPoint> values);

 private:
  std::vector<quint32> m_responseIds;
  std::vector<std::vector<AlarmPoint> > m_responseData;

 public:
  std::vector<quint32> responseIds() const;
  void setResponseIds(const std::vector<quint32> &responseIds);
  std::vector<std::vector<AlarmPoint> > responseData() const;
  void setResponseData(const std::vector<std::vector<AlarmPoint> > &responseData);

  MSGPACK_DEFINE(m_responseIds, m_responseData)
};
}  // namespace AHP

Q_DECLARE_METATYPE(AHP::HistorianAlarmResponse)


#endif // HISTORIANALARMRESPONSE_H
