#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QtXml>
#include <QFile>
#include "lightbinding.h"
#include <list>
#include <QFileInfo>
class XMLParser
{
public:
    XMLParser();
    XMLParser(std::string xmlFile);
    std::list<LightBinding> ParseXML();
    int ParseTotalLightCounts();
    void InitializeXMLFile();
    void UpdateXMLFile(std::list<LightBinding> bindings, int totalLightsCount);
private:
    std::string m_strXmlFile;

};

#endif // XMLPARSER_H
