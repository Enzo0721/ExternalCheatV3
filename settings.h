#pragma once

void saveConfig();
void loadConfig();

namespace settings {
	inline bool toggle_bhop = false;
	inline bool toggle_chams = false;
	inline bool toggle_glow = false;
	inline bool toggle_radar = false;
	inline bool toggle_noFlash = false;
	inline bool toggle_pulseGlow = false;
	inline bool toggle_aimbot = false;
	inline int chamsColor = 1;
	inline float glowColor[] = { 1.f, 0.f, 0.f, 1.f };
	inline int aimbotFOV = 0;
}

# define ReadNextSetting(s) myconfig.ignore(80, '='); myconfig >> s