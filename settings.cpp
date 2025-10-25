#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "settings.h"

bool settings::saveConfig(const std::string &filename)
{
	std::ofstream myconfig(filename);
	myconfig << "toggle_bhop = " << settings::toggle_bhop << "\n";
	myconfig << "toggle_chams = " << settings::toggle_chams << "\n";
	myconfig << "toggle_glow = " << settings::toggle_glow << "\n";
	myconfig << "toggle_radar = " << settings::toggle_radar << "\n";
	myconfig << "toggle_noFlash = " << settings::toggle_noFlash << "\n";
	myconfig << "toggle_pulseGlow = " << settings::toggle_pulseGlow << "\n";
	myconfig << "toggle_aimbot = " << settings::toggle_aimbot << "\n";
	myconfig << "aimbotFOV = " << settings::aimbotFOV << "\n";
    myconfig << "aimbotSmoothness = " << settings::aimbotSmoothness << "\n";
    myconfig << "aimbotRecoilAssist = " << settings::aimbotRecoilAssist << "\n";
	myconfig << "chamsColor = " << settings::chamsColor << "\n";
	myconfig << "glowColor[0] = " << settings::glowColor[0] << "\n";
	myconfig << "glowColor[1] = " << settings::glowColor[1] << "\n";
	myconfig << "glowColor[2] = " << settings::glowColor[2] << "\n";
	myconfig << "glowColor[3] = " << settings::glowColor[3] << "\n";
	myconfig << "glowType = " << settings::glowType << "\n";
	myconfig << "healthGlow = " << settings::toggle_healthGlow << "\n";
	myconfig << "teamGlow = " << settings::toggle_teamGlow << "\n";
	myconfig << "teamGlowColor[0] = " << settings::teamGlowColor[0] << "\n";
	myconfig << "teamGlowColor[1] = " << settings::teamGlowColor[1] << "\n";
	myconfig << "teamGlowColor[2] = " << settings::teamGlowColor[2] << "\n";
	myconfig << "teamGlowColor[3] = " << settings::teamGlowColor[3] << "\n";
	myconfig.close();
    return true;
}

bool settings::loadConfigFrom(const std::string& filename)
{
    std::ifstream myconfig(filename);
    if (!myconfig.is_open()) return false;

    ReadNextSetting(settings::toggle_bhop);
    ReadNextSetting(settings::toggle_chams);
    ReadNextSetting(settings::toggle_glow);
    ReadNextSetting(settings::toggle_radar);
    ReadNextSetting(settings::toggle_noFlash);
    ReadNextSetting(settings::toggle_pulseGlow);
    ReadNextSetting(settings::toggle_aimbot);
    ReadNextSetting(settings::aimbotFOV);
    ReadNextSetting(settings::aimbotSmoothness);
	ReadNextSetting(settings::aimbotRecoilAssist);
    //ReadNextSetting(settings::chamsColor);
    ReadNextSetting(settings::glowColor[0]);
    ReadNextSetting(settings::glowColor[1]);
    ReadNextSetting(settings::glowColor[2]);
    ReadNextSetting(settings::glowColor[3]);
    ReadNextSetting(settings::glowType);
    ReadNextSetting(settings::toggle_healthGlow);
    ReadNextSetting(settings::toggle_teamGlow);
    ReadNextSetting(settings::teamGlowColor[0]);
    ReadNextSetting(settings::teamGlowColor[1]);
    ReadNextSetting(settings::teamGlowColor[2]);
    ReadNextSetting(settings::teamGlowColor[3]);

    myconfig.close();
    return true;
}

std::vector<std::string> settings::listConfigFiles()
{
    std::vector<std::string> out;
    try {
        const std::filesystem::path cwd = std::filesystem::current_path();
        for (auto& p : std::filesystem::directory_iterator(cwd))
        {
            if (!p.is_regular_file()) continue;
            if (p.path().extension() == ".ecfg")
                out.push_back(p.path().filename().string());
        }
        std::sort(out.begin(), out.end());
    }
    catch (...)
    {
    }
    return out;
}
