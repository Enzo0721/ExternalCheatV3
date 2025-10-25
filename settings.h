#pragma once

#include <string>
#include <vector>
#include <set>

namespace settings {
    inline bool toggle_bhop = false;
    inline bool toggle_chams = false;
    inline bool toggle_glow = false;
    inline bool toggle_radar = false;
    inline bool toggle_noFlash = false;
    inline bool toggle_pulseGlow = false;
    inline bool toggle_aimbot = false;
    inline float chamsColor[] = { 1.f, 0.f, 0.f, 1.f };
    inline float glowColor[] = { 1.f, 0.f, 0.f, 1.f };
    inline int aimbotFOV = 0;
    inline float aimbotSmoothness = 1;
    inline std::set<int> aimbotBone = {8}; // Default to head
	inline float aimbotRecoilAssist = 0.0f; // 1.0 = full compensation, 0.0 = none
    inline int glowType = 0;
    inline bool toggle_healthGlow = false;
    inline bool toggle_teamGlow = false;
    inline float teamGlowColor[] = { 1.f, 0.f, 0.f, 1.f };

    bool saveConfig(const std::string& filename);
    bool loadConfigFrom(const std::string& filename);
    std::vector<std::string> listConfigFiles();
}

# define ReadNextSetting(s) myconfig.ignore(80, '='); myconfig >> s