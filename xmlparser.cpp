#include "xmlparser.h"

XMLParser::XMLParser()
{

}

XMLParser::XMLParser(std::string xmlFile)
{
    m_strXmlFile = xmlFile;
}

int XMLParser::ParseTotalLightCounts()
{
    int numberOfLights = 8;
    QDomDocument xmlBOM;
    QString str = QString::fromUtf8(m_strXmlFile.c_str());
    QFile f(str);

    if(!f.open(QIODevice::ReadOnly))
    {
        //std::cerr << "" << std::endl;
        return numberOfLights;
    }
    xmlBOM.setContent(&f);
    f.close();
    QDomElement root = xmlBOM.documentElement();

    // Get root name and attributes
    QString type = root.tagName();

    QDomElement component = root.firstChild().toElement();
    while(!component.isNull())
    {
        if(component.tagName() == "TotalLights")
        {
            QString ID = component.attribute("NumberLights");
            QDomElement child = component.firstChild().toElement();
            while(!child.isNull())
            {
                child = child.nextSibling().toElement();
            }
        }

        component = component.nextSibling().toElement();
    }
    return numberOfLights;
}

std::list<LightBinding> XMLParser::ParseXML()
{
    std::list<LightBinding> tempList;

    QDomDocument xmlBOM;
    QString str = QString::fromUtf8(m_strXmlFile.c_str());
    QFile f(str);

    if(!f.open(QIODevice::ReadOnly))
    {
        return tempList;
    }
    xmlBOM.setContent(&f);
    QDomElement root = xmlBOM.documentElement();

    QString type = root.tagName();
    QDomElement component = root.firstChild().toElement();

    while(!component.isNull())
    {
        if(component.tagName() == "Binding")
        {
            QString light = component.attribute("Light");
            std::string lightStdStr = light.toUtf8().constData();

            QString fader = component.attribute("Fader");
            std::string faderStdStr = fader.toUtf8().constData();

            QString faderIntensity = component.attribute("FaderIntensity");
            std::string faderIntensityStr = faderIntensity.toUtf8().constData();

            LightBinding binding(lightStdStr, faderStdStr, faderIntensityStr);
            tempList.push_back(binding);

        }

        component = component.nextSibling().toElement();
    }
    return tempList;
}

void XMLParser::InitializeXMLFile()
{
    QString str = QString::fromUtf8(m_strXmlFile.c_str());
    QFileInfo f(str);

    if(f.exists() && f.isFile())
    {
        return;
    }

    QDomDocument doc("TheatericalLighting");
    QDomElement root = doc.createElement("TheatericalLighting");
    doc.appendChild(root);

    QDomElement tag = doc.createElement("TotalLights");
    root.appendChild(tag);

    tag.setAttribute("NumberLights","8");

    QFile file(str);
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
}

void XMLParser::UpdateXMLFile(std::list<LightBinding> bindings, int totalLightsCount)
{
    QString str = QString::fromUtf8(m_strXmlFile.c_str());

    QDomDocument doc("TheatericalLighting");
    QDomElement root = doc.createElement("TheatericalLighting");
    doc.appendChild(root);

    QDomElement tagTotalLights = doc.createElement("TotalLights");
    root.appendChild(tagTotalLights);
    tagTotalLights.setAttribute("NumberLights", "8");

    std::list<LightBinding>::iterator it = bindings.begin();
    for(; it != bindings.end(); ++it)
    {
        QDomElement tag = doc.createElement("Binding");
        root.appendChild(tag);
        tag.setAttribute("Light", QString::fromUtf8(it->GetLightName().c_str()));
        tag.setAttribute("Fader", QString::fromUtf8(it->GetFaderName().c_str()));
        tag.setAttribute("FaderIntensity", QString::fromUtf8(it->GetFaderIntensity().c_str()));
    }

    // Write to file
    QFile file(str);
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
}
