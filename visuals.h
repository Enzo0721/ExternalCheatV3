#pragma once
#include "memory.h"

namespace visuals
{
	void visualThread(const Memory& mem) noexcept;
	void chamsThread(const Memory& mem) noexcept;
	void glow(const Memory& mem, const uintptr_t localBase);
	void chams(const Memory& mem, const uintptr_t localBase);
}
