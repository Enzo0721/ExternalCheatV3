#include <thread>
#include "visuals.h" 
#include "gui.h"
#include "globals.h"
#include "settings.h"

/* GLOW = INSTANT RED TRUST*/

struct GlowObject
{
	float red = 0.f;
	float green = 0.f;
	float blue = 0.f;
	float alpha = 0.f;
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


void visuals::glow(const Memory& mem, const uintptr_t localBase)
{
	GlowObjectVar.red = settings::glowColor[0];
	GlowObjectVar.green = settings::glowColor[1];
	GlowObjectVar.blue = settings::glowColor[2];
	GlowObjectVar.alpha = settings::glowColor[3];
	GlowObjectVar.fullBloom = settings::toggle_glowFullBloom;

	if (settings::toggle_glow)
	{
		int glowObjectManager = mem.read<uintptr_t>(globals::bClient + offsets::dwGlowObjectManager);
		for (int i = 1; i <= 64; i++)
		{
			const auto player = mem.read<uintptr_t>(globals::bClient + offsets::dwEntityList + (i * 0x10));
			
			if (mem.read<int32_t>(player + offsets::m_iHealth) == 0)
				continue;

			if (mem.read<bool>(player + offsets::m_Dormant) == true)
				continue;

			if (mem.read<int32_t>(player + offsets::m_iTeamNum) == mem.read<int32_t>(localBase + offsets::m_iTeamNum))
				continue;

			const auto glowIndex = mem.read<uintptr_t>(player + offsets::m_iGlowIndex);

			mem.write<GlowObject>(glowObjectManager + (glowIndex * 0x38) + 0x08, GlowObjectVar);
		}
	}
}
