#include "xmlloader.h"

XmlLoader::XmlLoader(QObject* parent) : QObject(parent)
{
    m_errorStr = "";
    m_projectTag = false;
}

bool XmlLoader::startElement(const QString& /* namespaceURI */, const QString& /* localName */, const QString& qName, const QXmlAttributes& attributes)
 {
    if (!m_projectTag && qName != "project")
    {
        m_errorStr = QObject::tr("The file is not a Arex Project file.");
        return false;
    }

    if (qName == "project")
    {
        //Inform to MainView to add Project
        //emit newProject(attributes);

        m_projectTag = true;
    }
    else if (qName == "collector")
    {
        //Inform to Communication to config Collector
        emit newCollector(attributes);
    }
    else if (qName == "bdh")
    {
        //Inform to Communication to config BDH
        emit newBDH(attributes);
    }
    else if (qName == "device")
    {
        //Inform to CollectorData to add Device
        //emit newDevice(attributes);
    }
    else if (qName == "variable")
    {
        //Inform to CollectorData to add Variable
        emit newVariable(attributes);
    }
    else if (qName == "alarm")
    {
        //Inform to CollectorData to add Graphic
        emit newAlarm(attributes);
    }
    else if (qName == "hmi")
    {
        //Inform to MainView to config Hmi
        //emit newHmi(attributes);
    }
    else if (qName == "deployment")
    {
        //Inform to MainView to add Deployment
        //emit newDeployment(attributes);
    }
    else if (qName == "graphic")
    {
        //Inform to MainView to add Graphic
        //emit newGraphic(attributes);
    }

    return true;
}

bool XmlLoader::endElement(const QString& /*namespaceURI*/, const QString& /*localName*/, const QString& /*qName*/)
{
    return true;
}

bool XmlLoader::endDocument()
{
    emit loadFinished();
    return true;
}

bool XmlLoader::fatalError(const QXmlParseException& exception)
{
    m_errorStr = exception.message();
    return false;
}

QString XmlLoader::errorString() const
{
    return m_errorStr;
}
