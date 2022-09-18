#include "misc.h"
#include "globals.h"
#include "gui.h"
#include <thread>
#include "settings.h"

void misc::miscThread(const Memory& mem) noexcept
{
	while (gui::exit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		const auto localBase = mem.read<uintptr_t>(globals::bClient + offsets::dwLocalPlayer);

		if (!localBase)
			continue;
		
		noFlash(mem, localBase);
		bhop(mem, localBase);
	}
}

void misc::noFlash(const Memory& mem, const uintptr_t localBase)
{
	if (settings::toggle_noFlash)
	{
		mem.write<uintptr_t>(localBase + offsets::m_flFlashMaxAlpha, 0);
	}
	else
	{
		mem.write<uintptr_t>(localBase + offsets::m_flFlashMaxAlpha, 1132396544);
	}
}

void misc::bhop(const Memory& mem, const uintptr_t localBase)
{
	if (settings::toggle_bhop)
	{
		int localFlag = mem.read<uintptr_t>(localBase + offsets::m_fFlags);
		if (GetAsyncKeyState(VK_SPACE) && (localFlag == 257 || localFlag == 263))
		{
			mem.write<uintptr_t>(globals::bClient + offsets::dwForceJump, 5);
		}
		else
		{
			mem.write<uintptr_t>(globals::bClient + offsets::dwForceJump, 4);
		}
	}
}