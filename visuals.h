#pragma once
#include "memory.h"

namespace visuals
{
	void visualThread(const Memory& mem) noexcept;
	void makeGlow(const Memory& mem, int human);
	void glow(const Memory& mem, const uintptr_t localBase);
}
