#include <thread>
#include "visuals.h" 
#include "gui.h"
#include "globals.h"
#include "settings.h"
#include <iostream>
#include <vector>

unsigned int b = 29399u;

struct GlowObject
{
	float red = settings::glowColor[0];
	float green = settings::glowColor[1];
	float blue = settings::glowColor[2];
	float alpha = settings::glowColor[3];
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;
}GlowObjectVar;

void visuals::visualThread(const Memory& mem) noexcept
{
	while (gui::exit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		const auto localBase = mem.read<uintptr_t>(globals::bClient + offsets::dwLocalPlayer);

		if (!localBase)
			continue;

		visuals::glow(mem, localBase);

	}
}

void visuals::chamsThread(const Memory& mem) noexcept {
	while (gui::exit) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		const auto localBase = mem.read<uintptr_t>(globals::bClient + offsets::dwLocalPlayer);

		if (!localBase)
			continue;

		visuals::chams(mem, localBase);
	}
}

std::vector<int> gradient(int value) {
	std::vector<int> rgb(3); // RGB values

	if (value >= 75) {
		// green to yellow gradient
		rgb[0] = 0;
		rgb[1] = 255;
		rgb[2] = 0;
	}
	else if (value >= 50) {
		// yellow to orange gradient
		rgb[0] = 255;
		rgb[1] = 255 - ((value - 50) * 128) / 25;
		rgb[2] = 0;
	}
	else if (value >= 25) {
		// orange to red gradient
		rgb[0] = 255;
		rgb[1] = 155;
		rgb[2] = 0;
	}
	else {
		// red gradient
		rgb[0] = 255;
		rgb[1] = 0;
		rgb[2] = 0;
	}

	return rgb;
}


void visuals::glow(const Memory& mem, const uintptr_t localBase)
{
	if (settings::toggle_glow || settings::toggle_healthGlow)
	{
		int glowObjectManager = mem.read<uintptr_t>(globals::bClient + offsets::dwGlowObjectManager);
		for (int i = 1; i <= 64; i++)
		{
			const auto player = mem.read<uintptr_t>(globals::bClient + offsets::dwEntityList + (i * 0x10));
			
			if (!settings::toggle_teamGlow) {
				if (mem.read<int32_t>(player + offsets::m_iTeamNum) == mem.read<int32_t>(localBase + offsets::m_iTeamNum))
					continue;
			}
			const auto glowIndex = mem.read<uintptr_t>(player + offsets::m_iGlowIndex);

			if (settings::toggle_teamGlow && !settings::toggle_healthGlow) {
				if (mem.read<int32_t>(player + offsets::m_iTeamNum) != mem.read<int32_t>(localBase + offsets::m_iTeamNum))
					continue;
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x08, settings::teamGlowColor[0]);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x0C, settings::teamGlowColor[1]);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, settings::teamGlowColor[2]);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, settings::teamGlowColor[3]);
			}

			if (!settings::toggle_healthGlow) {
				if (mem.read<int32_t>(player + offsets::m_iTeamNum) == mem.read<int32_t>(localBase + offsets::m_iTeamNum))
					continue;
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x08, settings::glowColor[0]);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x0C, settings::glowColor[1]);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, settings::glowColor[2]);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, settings::glowColor[3]);
			}
			else if (settings::toggle_healthGlow) {
				int playerHealth = mem.read<int>(player + offsets::m_iHealth);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x08, gradient(playerHealth)[0]);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x0C, gradient(playerHealth)[1]);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, gradient(playerHealth)[2]);
				mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, settings::glowColor[3]);
			}
			//mem.write<GlowObject>(glowObjectManager + (glowIndex * 0x38) + 0x08, GlowObjectVar);
			mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
			mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x29, false);//idk quite

			if (settings::glowType == 2) {
				mem.write<int>(glowObjectManager + (glowIndex * 0x38) + 0x30, 1); //aura pulse
			}
			else if (settings::glowType == 3){
				mem.write<int>(glowObjectManager + (glowIndex * 0x38) + 0x30, 2); //outline 1
			}
			else if (settings::glowType == 4) {
				mem.write<int>(glowObjectManager + (glowIndex * 0x38) + 0x30, 3); //outline 2
			}
			else {
				mem.write<int>(glowObjectManager + (glowIndex * 0x38) + 0x30, 0); //nothing
			}
			if (settings::glowType == 1) {
				mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x2A, true); // full bloom glow
			}
			else {
				mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x2A, false);
			}
		}
	}
}

void visuals::chams(const Memory& mem, const uintptr_t localBase) {
	while (settings::toggle_chams) {
		int localTeam = mem.read<int>(localBase + offsets::m_iTeamNum);
		for (unsigned int i = 0; i <= 64; ++i) {
			int human = mem.read<int>(globals::bClient + offsets::dwEntityList + (i - 1) * 0x10);
			int playerTeam = mem.read<int>(human + offsets::m_iTeamNum);

			if (localTeam != playerTeam) {
				mem.write<int>(human + offsets::m_clrRender, settings::chamsColor[0] * 255);
				mem.write<int>(human + offsets::m_clrRender + 0x1, settings::chamsColor[1] * 255);
				mem.write<int>(human + offsets::m_clrRender + 0x2, settings::chamsColor[2] * 255);
				mem.write<int>(human + offsets::m_clrRender + 0x3, settings::chamsColor[3] * 255);
			}

			/*if (!settings::toggle_chams) {
				mem.write<int>(human + offsets::m_clrRender, 255);
				mem.write<int>(human + offsets::m_clrRender + 0x1, 255);
				mem.write<int>(human + offsets::m_clrRender + 0x2, 255);
				mem.write<int>(human + offsets::m_clrRender + 0x3, 0);
			}*/
		}
		Sleep(1000);
	}
}
