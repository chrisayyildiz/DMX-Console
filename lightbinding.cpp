#include "lightbinding.h"

LightBinding::LightBinding()
{
    lightName = "";
    faderName = "";
    lightColor = "";
    faderIntensity = "0";

    m_Red = 0;
    m_Green = 0;
    m_Blue = 0;
    m_Intensity = 0;
}

LightBinding::LightBinding(std::string light, std::string fader, std::string fIntensity)
{
    lightName = light;
    faderName = fader;
    faderIntensity = fIntensity;
}

std::string LightBinding::GetLightName()
{
    return lightName;
}

std::string LightBinding::GetFaderName()
{
    return faderName;
}

void LightBinding::SetFaderName(std::string fader)
{
    faderName = fader;
}

void LightBinding::SetColor(std::string color)
{
    lightColor = color;
}

void LightBinding::SetFaderIntensity(std::string val)
{
    faderIntensity = val;
}

std::string LightBinding::GetFaderIntensity()
{
    return faderIntensity;
}

void LightBinding::SetRed(int red)
{
    m_Red = red;
}

int LightBinding::GetRed()
{
    return m_Red;
}

void LightBinding::SetGreen(int green)
{
    m_Green = green;
}

int LightBinding::GetGreen()
{
    return m_Green;
}

void LightBinding::SetBlue(int blue)
{
    m_Blue = blue;
}

int LightBinding::GetBlue()
{
    return m_Blue;
}

void LightBinding::SetIntensity(int intensity)
{
    m_Intensity = intensity;
}

int LightBinding::GetIntensity()
{
    return m_Intensity;
}
