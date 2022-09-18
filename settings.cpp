#include <iostream>
#include <fstream>
#include <string>
#include "settings.h"

void saveConfig()
{
	std::ofstream myconfig("example.txt");
	myconfig << "toggle_bhop = " << settings::toggle_bhop << "\n";
	myconfig << "toggle_chams = " << settings::toggle_chams << "\n";
	myconfig << "toggle_glow = " << settings::toggle_glow << "\n";
	myconfig << "toggle_radar = " << settings::toggle_radar << "\n";
	myconfig << "toggle_noFlash = " << settings::toggle_noFlash << "\n";
	myconfig << "toggle_pulseGlow = " << settings::toggle_pulseGlow << "\n";
	myconfig << "toggle_aimbot = " << settings::toggle_aimbot << "\n";
	myconfig << "aimbotFOV = " << settings::aimbotFOV << "\n";
	myconfig << "chamsColor = " << settings::chamsColor << "\n";
	myconfig << "glowColor[0] = " << settings::glowColor[0] << "\n";
	myconfig << "glowColor[1] = " << settings::glowColor[1] << "\n";
	myconfig << "glowColor[2] = " << settings::glowColor[2] << "\n";
	myconfig << "glowColor[3] = " << settings::glowColor[3] << "\n";
	myconfig.close();
}

void loadConfig()
{
	std::ifstream myconfig("example.txt");
	ReadNextSetting(settings::toggle_bhop);
	ReadNextSetting(settings::toggle_chams);
	ReadNextSetting(settings::toggle_glow);
	ReadNextSetting(settings::toggle_radar);
	ReadNextSetting(settings::toggle_noFlash);
	ReadNextSetting(settings::toggle_pulseGlow);
	ReadNextSetting(settings::toggle_aimbot);
	ReadNextSetting(settings::aimbotFOV);
	ReadNextSetting(settings::chamsColor);
	ReadNextSetting(settings::glowColor[0]);
	ReadNextSetting(settings::glowColor[1]);
	ReadNextSetting(settings::glowColor[2]);
	ReadNextSetting(settings::glowColor[0]);
	myconfig.close();
}
