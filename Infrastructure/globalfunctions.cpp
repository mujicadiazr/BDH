#include "globalfunctions.h"

QStringList ValidValues(QString pName)
{
    QStringList list;

    if      (pName == "proxyType") list << "TCP" << "DBUS";
    else if (pName == "connectionType") list << "Ethernet" << "Serie";
    else if (pName == "networkInterface") list << "/dev/ttyS0" << "/dev/ttyS1" << "/dev/ttyS2" << "/dev/ttyS3";
    else if (pName == "baudRate") list << "110" << "300" << "600" << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    else if (pName == "dataBits") list << "5" << "6" << "7" << "8";
    else if (pName == "stopBits") list << "1" << "2";
    else if (pName == "flowControl") list << "None" << "Hardware";
    else if (pName == "parity") list << "None" << "Odd" << "Even";
    else if (pName == "dataType") list << "Bool" << "Byte" << "SInt" << "USInt" << "Int" << "UInt" << "DInt" << "UDInt";
    else if (pName == "type") list << "HighHigh" << "High" << "Low" << "LowLow" << "ChangeRatio" << "NotVariantion"  << "InstrumentFail" << "CommandFail" <<  "NotCommanded" << "State";
    else if (pName == "severity") list << "Critical" << "Moderate" << "Light";
    else if (pName == "priority") list << "1" << "2" << "3" << "4" << "5";
    else if (pName == "scale") list << "0.5" << "0.75" << "1" << "1.5" << "2" <<  "3" << "4";
    else if (pName == "rotation") list << "-90" << "-60" << "-30" << "0" << "30" << "60" << "90";
    else if (pName == "opacity") list << "0.1" << "0.2" << "0.3" << "0.4" << "0.5" << "0.6" << "0.7" << "0.8" << "0.9" << "1";

    return list;
}

bool IsEncriptedProperty(const QString &pName)
{
    return QString("dataType,type,severity").split(",").contains(pName);
}

bool IsCommunicationSubProperty(const QString &pName)
{
    return (QString("ipAddress,subnetMask,gateWay,port,networkInterface,baudRate,dataBits,stopBits,flowControl,parity").split(",").contains(pName));
}

bool IsIpProperty(const QString &pName)
{
    return (QString("ipAddress,subnetMask,gateWay,host").split(",").contains(pName));
}

bool IsNameProperty(const QString &pName)
{
    return (QString("name").split(",").contains(pName));
}

bool IsGeometryProperty(const QString &pName)
{
    return (QString("position,size,scale,rotation").split(",").contains(pName));
}

bool IsIntegralProperty(const QString &pName)
{
    return IsIpProperty(pName) || QString("address").split(",").contains(pName);
}

bool IsAsyncronousProperty(const QString &pName) //FIX review if is necesary
{
    return IsIpProperty(pName) || QString("name,address,port").split(",").contains(pName);
}

bool IsComposedProperty(const QString &pName)
{
    return (QString("size,position").split(",").contains(pName));
}

bool IsTextProperty(const QString &pName)
{
    return (QString("description,m_text").split(",").contains(pName));
}

bool IsComboProperty(const QString &pName)
{
    return ValidValues(pName).length() > 0;
}

bool IsTimeProperty(const QString &pName)
{
    return (QString("requestTime").split(",").contains(pName));
}

bool IsVariableType(const QString &pName)
{
    return (QString("bool,char,uchar,sint,usint,int,uint,dint,udint,real,lreal").split(",").contains(pName));
}

bool IsNumberProperty(const QString &pName)
{
    return IsVariableType(pName) || QString("baudRate,dataBits,stopBits,scale,rotation,opacity,port,level").split(",").contains(pName);
}

QString GetValue(const QXmlAttributes &pProperties, QString pName, QString pDefault, QString pAlias)
{
    //If not found return default value if was defined
    if (pProperties.index(pName) == -1 &&  pDefault != "") return pDefault;

    //Define some Variables
    if (pAlias == "") pAlias = pName;
    QStringList validValues = ValidValues(pAlias);
    QString value = pProperties.value(pName);
    int index = -1;

    //Return original value if not defined valid values
    if (validValues.size() == 0) return value;

    //Search most correct index of value in validValues array
    if (IsEncriptedProperty(pName)) //Get proposed index
    {
        index = value.toInt();
    }
    else if (IsNumberProperty(pName)) //Get most proximal valid index to proposed value as number
    {
        //Temp
        float valueFloat = QVariant(value).toFloat();
        float diference;
        float minimal = std::numeric_limits<float>::max();

        //Find most proximal numeric value and it index
        QListIterator<QString> i(validValues);
        while (i.hasNext())
        {
            diference = qAbs(valueFloat - i.next().toFloat());
            if (minimal > diference)
            {
                minimal = diference;
                index++;
            }
            else break;
        }
    }
    else //Get index of value
    {
        index = validValues.indexOf(value);
    }

    //Return extreme value if index not match with valid one
    if (index < 0) return validValues.first(); //First
    else if (index > validValues.size() - 1) index = validValues.size() - 1; //Last

    //Return correct value
    return validValues.value(index);
}

QRegExp RegularExpresion(QString pName)
{
    //TEMP
    QRegExp expresion;
    QMap<QString, QString> patterns;
    QString Octet = "(\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5])";

    //Define posible Patterns
    patterns["name"] = "^[a-zA-Z]{1,32}[0-9]{0,32}[_]{0,1}((([a-zA-Z]{0,32}[0-9]{1,32})|([a-zA-Z]{1,32}[0-9]{0,32}))[_]{0,1})*$";
    patterns["address"] = "[0-3]:\\d{1,2}";
    patterns["ip"] = "^" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + "$";

    //Assign correct Pattern
    if (patterns.contains(pName)) expresion.setPattern(patterns[pName]);
    else expresion.setPattern("");

    return expresion;
}

QString MaskExpresion(QString pName)
{
    QString mask = "";

    if (pName == "ip") mask = "000.000.000.000; ";

    return mask;
}

QStringList GetExtraProperties(QObject *pObject)
{
    QStringList properties;
    if (pObject)
    {
        const QMetaObject *metaObject = pObject->metaObject();
        int max = metaObject->propertyCount();
        for (int i = 0; i < max; i++)
        {
            QString propertyName = metaObject->property(i).name();

            if (propertyName.startsWith("m_")) properties.append(propertyName);
        }
    }

    return properties;
}

QVariant ReadMetaProperty(QObject *pObject, QString pName)
{
    if (pObject)
    {
        QMetaProperty metaProperty;
        const QMetaObject *metaObject = pObject->metaObject();
        int max = metaObject->propertyCount();
        for (int i = 0; i < max; i++)
        {
            metaProperty = metaObject->property(i);
            if (pName == metaProperty.name()) return metaProperty.read(pObject);
        }
    }
    else
    {
        return QVariant();
    }
}

bool WriteMetaProperty(QObject *pObject, QString pName, const QVariant& pValue)
{
    if (!pObject) return false;

    const QMetaObject *metaObject = pObject->metaObject();
    int index = metaObject->indexOfProperty(pName.toUtf8().constData());
    if (index >= 0)
    {
        QMetaProperty metaProperty = metaObject->property(index);
        if (metaProperty.isWritable() &&          //Value cant Writed
            metaProperty.read(pObject) != pValue) //Value changed
        {
            metaProperty.write(pObject, pValue);
            return true;
        }
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }
}

void CopyMetaProperty(QObject *pObject, QObject *pTarget, QString pName)
{
    //Temp
    const QMetaObject *metaOrigin = pObject->metaObject();
    const QMetaObject *metaTarget = pTarget->metaObject();
    const char *name = pName.toUtf8().constData();
    int index1 = metaOrigin->indexOfProperty(name);
    bool denied = QString("selected").split(",").contains(name);
    if (index1 >= 0 && !denied)
    {
        QMetaProperty metaPropertyOrigin = metaOrigin->property(index1);
        int index2 = metaTarget->indexOfProperty(name);
        if (index2 >= 0)
        {
            QMetaProperty metaPropertyTarget = metaTarget->property(index2);
            if (metaPropertyTarget.isWritable())
            {
                QVariant value = metaPropertyOrigin.read(pObject);//Read
                metaPropertyTarget.write(pTarget, value);//Write
            }
        }
    }
}

void CopyMetaProperties(QObject *pObject, QObject *pTarget)
{
    //Temp
    const QMetaObject *metaOrigin = pObject->metaObject();
    const QMetaObject *metaTarget = pTarget->metaObject();
    QMetaProperty metaPropertyOrigin;
    QMetaProperty metaPropertyTarget;
    QVariant value;
    int index;
    int max = metaOrigin->propertyCount();
    bool denied;

    //Read  pObject  property  and  Write pTarget property if is writetable
    for (int i = 0; i < max; i++)
    {
        metaPropertyOrigin = metaOrigin->property(i);
        const char *name = metaPropertyOrigin.name();
        index = metaTarget->indexOfProperty(name);
        denied = QString("selected").split(",").contains(name);
        if (index >= 0 && !denied)
        {
            metaPropertyTarget = metaTarget->property(index);
            if (metaPropertyTarget.isWritable())
            {
                value = metaPropertyOrigin.read(pObject);//Read
                metaPropertyTarget.write(pTarget, value);//Write
            }
        }
    }
}



QString ItemClassName(int pType, bool pPlural)
{
    switch(pType)
    {
        case 0: return "Project";
        case 1: return "Collector";
        case 2: return "Device";
        case 3: return "Variable";
        case 4: return "Alarm";
        case 5: return "Hmi";
        case 6: return "Deployment";
        case 7: return "Graphic";
    }

    return "";
}

bool BlankText(QString pText)
{
    return pText == "";
}

QString FormatKey(QString pKey)
{
    QString result = "";
    QChar *base = pKey.data();
    QChar *ch;
    int max = pKey.length();
    for (int pos = 0; pos < max; pos++)
    {
        ch = base + pos;
        if (pos == 0) result += QString(ch->toUpper());
        else if (*ch > 64 && *ch < 91) result += " " + QString(*ch);
        else result += QString(*ch);
    }

    return result;
}

const char *FormatKeyChar(QString pKey)
{
    return FormatKey(pKey).toUtf8().constData();
}

QString BaseName(QString pName)
{
    if (pName == "") return "";

    QStringList parts = pName.split("_");
    if (parts.size() == 1) return pName;

    int pos;
    QString last = parts.last();
    QIntValidator intValidator;
    if (intValidator.validate(last, pos) != QValidator::Invalid)
        return pName.left(pName.size() - last.size() - 1);

    return pName;
}

bool EqualNames(QString pName1, QString pName2)
{
    //Simplify names
    QString name1 = pName1.simplified().toLower();
    QString name2 = pName2.simplified().toLower();

    if (name1 == name2) return true;

    return false;
}


void FixExtension(QString &pName, const QString &pExt)
{
    QString baseName = pName;
    if (pName.split(".").last().toLower() == pExt)
        baseName = pName.left(pName.size() - 4);

    pName = baseName + QString(".%1").arg(pExt);
}


bool IsGreatherThanAddress(QString pFirst, QString pSecond)
{
    QList<QString> firstList = pFirst.split(":");
    QList<QString> secondList = pSecond.split(":");

    if (firstList.length() != 2 || secondList.length() != 2) return false;

    qreal first_1 = QVariant(firstList[0]).toReal();
    qreal first_2 = QVariant(firstList[1]).toReal();
    qreal second_1 = QVariant(secondList[0]).toReal();
    qreal second_2 = QVariant(secondList[0]).toReal();

    if (first_1 > second_1) return false;
    if (first_1 == second_1 && first_2 > second_2) return false;
}


bool IsGreatherThanIp(QString pFirst, QString pSecond)
{
    QList<QString> firstList = pFirst.split(".");
    QList<QString> secondList = pSecond.split(".");

    if (firstList.length() != 4 || secondList.length() != 4) return false;

    qreal first_1 = QVariant(firstList[0]).toReal();
    qreal first_2 = QVariant(firstList[1]).toReal();
    qreal first_3 = QVariant(firstList[2]).toReal();
    qreal first_4 = QVariant(firstList[3]).toReal();
    qreal second_1 = QVariant(secondList[0]).toReal();
    qreal second_2 = QVariant(secondList[0]).toReal();
    qreal second_3 = QVariant(secondList[0]).toReal();
    qreal second_4 = QVariant(secondList[0]).toReal();

    if (first_1 > second_1) return false;
    if (first_1 == second_1 && first_2 > second_2) return false;
    if (first_1 == second_1 && first_2 == second_2 && first_3 > second_3) return false;
    if (first_1 == second_1 && first_2 == second_2 && first_3 == second_3 && first_4 > second_4) return false;
}
