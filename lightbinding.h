
#ifndef LIGHTBINDING_H
#define LIGHTBINDING_H

#include <string>

class LightBinding
{
public:
    LightBinding();
    LightBinding(std::string light, std::string fader, std::string faderIntensity);
    std::string GetLightName();
    std::string GetFaderName();
    std::string GetFaderIntensity();
    void SetFaderIntensity(std::string val);
    void SetFaderName(std::string fader);

    void SetColor(std::string color);

    void SetRed(int red);
    void SetGreen(int green);
    void SetBlue(int blue);
    void SetIntensity(int intensity);

    int GetRed();
    int GetGreen();
    int GetBlue();
    int GetIntensity();
private:
    std::string lightName;
    std::string faderName;
    std::string lightColor;
    std::string faderIntensity;

    int m_Red;
    int m_Green;
    int m_Blue;
    int m_Intensity;
};

#endif // LIGHTBINDING_H
