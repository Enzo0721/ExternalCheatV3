#pragma once
#include "memory.h"

namespace misc
{
	void miscThread(const Memory& mem) noexcept;
	void noFlash(const Memory& mem, const uintptr_t localBase);
	void bhop(const Memory& mem, const uintptr_t localBase);
}