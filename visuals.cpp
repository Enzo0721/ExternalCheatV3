#include <thread>
#include "visuals.h" 
#include "gui.h"
#include "globals.h"
#include "settings.h"

struct GlowStruct
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
}GlowStructVar;

void visuals::visualThread(const Memory& mem) noexcept
{
	while (gui::exit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		const auto& localBase = mem.read<uintptr_t>(globals::bClient + offsets::dwLocalPlayer);

		if (!localBase)
			continue;

		visuals::glow(mem, localBase);

	}
}

void visuals::makeGlow(const Memory& mem, int human)
{
	int glowIndex = mem.read<uintptr_t>(human + offsets::m_iGlowIndex);
	int glowObjectManager = mem.read<uintptr_t>(globals::bClient + offsets::dwGlowObjectManager);

	/*int calculation = glowIndex * 0x38 + 0x8;
	mem.write<float>(glowObject + calculation, settings::glowColor[0]);
	calculation = glowIndex * 0x38 + 0xC;
	mem.write<float>(glowObject + calculation, settings::glowColor[1]);
	calculation = glowIndex * 0x38 + 0x10;
	mem.write<float>(glowObject + calculation, settings::glowColor[2]);
	calculation = glowIndex * 0x38 + 0x14;
	mem.write <float>(glowObject + calculation, settings::glowColor[3]);
	calculation = glowIndex * 0x38 + 0x28;
	mem.write<bool>(glowObject + calculation, true); //(0x24)gCol.light
	calculation = glowIndex * 0x38 + 0x29;//temp
	mem.write<bool>(glowObject + calculation, true);//temp*/
	mem.write<GlowStruct>(glowObjectManager + (glowIndex * 0x38) + 0x8, GlowStructVar);
}

void visuals::glow(const Memory& mem, uintptr_t localBase)
{
	if (settings::toggle_glow)
	{
		int glowObjectManager = mem.read<uintptr_t>(globals::bClient + offsets::dwGlowObjectManager);
		for (int i = 1; i <= 64; i++)
		{
			const auto entity = mem.read<uintptr_t>(globals::bClient + offsets::dwEntityList + i * 0x10);
			
			if (mem.read<uintptr_t>(entity + offsets::m_iTeamNum) == mem.read<uintptr_t>(localBase + offsets::m_iTeamNum))
				continue;

			const auto glowIndex = mem.read<uintptr_t>(entity + offsets::m_iGlowIndex);

			mem.write<GlowStruct>(glowObjectManager + (glowIndex * 0x38) + 0x8, GlowStructVar);
			//mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x8, settings::glowColor[0]);
			//mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0xC, settings::glowColor[1]);
			//mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, settings::glowColor[2]);
			//mem.write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, settings::glowColor[3]);

			//mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x27, true);
			//mem.write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
		}
	}
}

/*GLOW NEEDS A LOT OF FIXING*/