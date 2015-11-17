/**
  @file XmlLoader.h
  @brief Clase que permite parsear el archivo de configuración exportado por el editor.

  La clase emite señales con el objetivo de notificar a los observadores sobre
  los elementos que se van encontrando en el archivo de configuración.

  @author Jordanis Viltres Chávez
  @date 2013-2014
*/

#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <QObject>
#include <QXmlDefaultHandler>

class XmlLoader : public QObject, public QXmlDefaultHandler
{
    Q_OBJECT

public:
    //CONTRUCTORS AND DESTROYERS
    explicit XmlLoader(QObject* parent = 0);

    //REIMPLEMENTED METHODS
    bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& attributes);
    bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName);
    bool endDocument();
    bool fatalError(const QXmlParseException& exception);
    QString errorString() const;

signals:
    void newProject(const QXmlAttributes& properties);
    void newCollector(const QXmlAttributes& properties);
    void newBDH(const QXmlAttributes& properties);
    void newDevice(const QXmlAttributes& properties);
    void newVariable(const QXmlAttributes& properties);
    void newAlarm(const QXmlAttributes& properties);
    void newHmi(const QXmlAttributes& properties);
    void newDeployment(const QXmlAttributes& properties);
    void newGraphic(const QXmlAttributes& properties);
    void loadFinished();

protected:
    //TEMP
    QString m_errorStr;
    bool m_projectTag;
};


#endif // XMLLOADER_H
