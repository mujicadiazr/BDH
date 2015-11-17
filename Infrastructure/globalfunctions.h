/**
  @file globalfunctions.h
  @brief Archivo de funciones mas usadas

  @author Jordanis Viltres Chavez
  @date 2013-2014
*/

#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <QtCore/QPoint>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtWidgets/QComboBox>
#include <QtXml/QXmlAttributes>

//Check Property type
QStringList ValidValues(QString pProperty);
bool IsEncriptedProperty(const QString &pName);
bool IsCommunicationSubProperty(const QString &pName);
bool IsIpProperty(const QString &pName);
bool IsNameProperty(const QString &pName);
bool IsGeometryProperty(const QString &pName);
bool IsIntegralProperty(const QString &pName);
bool IsAsyncronousProperty(const QString &pName);
bool IsComposedProperty(const QString &pName);
bool IsTextProperty(const QString &pName);
bool IsComboProperty(const QString &pName);
bool IsTimeProperty(const QString &pName);
bool IsVariableType(const QString &pName);
bool IsNumberProperty(const QString &pName);
QString GetValue(const QXmlAttributes &pProperties, QString pName, QString pDefault = "", QString pAlias = "");
QRegExp RegularExpresion(QString pProperty);
QString MaskExpresion(QString pProperty);
QStringList GetExtraProperties(QObject *pObject);
QVariant ReadMetaProperty(QObject *pObject, QString pName);
bool WriteMetaProperty(QObject *pObject, QString pName, const QVariant& pValue);
void CopyMetaProperty(QObject *pObject, QObject *pTarget, QString pName);
void CopyMetaProperties(QObject *pObject, QObject *pTarget);
//void FillCombo(QComboBox *pCombo, QString pName);
QString ItemClassName(int pType, bool pPlural = false);
bool BlankText(QString pText);
QString FormatKey(QString pKey);
const char *FormatKeyChar(QString pKey);
QString BaseName(QString pName);
bool EqualNames(QString pName1, QString pName2);
void FixExtension(QString &pName, const QString &pExt);
bool IsGreatherThanAddress(QString pFirst, QString pSecond);
bool IsGreatherThanIp(QString pFirst, QString pSecond);


#endif // GLOBALFUNCTIONS_H
