#include <QApplication>

//#include "Test/historianrequesttest.h"
//#include "Test/realtimerequesttest.h"

#include "Presentation/clientview.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Declarando tipos para poder emitirlos como sennales
    qRegisterMetaType<Value>("Value");
    qRegisterMetaType<VariablePoint>("VariablePoint");

    qRegisterMetaType<std::vector<VariablePoint>>("std::vector<VariablePoint>");
    qRegisterMetaType<std::vector<std::vector<VariablePoint>>>("std::vector<std::vector<VariablePoint>>");

    qRegisterMetaType<AHP::HistorianVariableResponse>("HistorianVariableResponse");
    qRegisterMetaType<AHP::RealtimeVariableResponse>("RealtimeVariableResponse");

    qRegisterMetaType<AlarmPoint>("AlarmPoint");

    qRegisterMetaType<std::vector<AlarmPoint>>("std::vector<AlarmPoint>");
    qRegisterMetaType<std::vector<std::vector<AlarmPoint>>>("std::vector<std::vector<AlarmPoint>>");

    qRegisterMetaType<AHP::HistorianAlarmResponse>("HistorianAlarmResponse");


    //RealtimeRequestTest test;
    //test.run();

    ClientView w;
    w.show();

    return app.exec();
}
