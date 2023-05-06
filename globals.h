#pragma once
#include <cstdint>
#include <cstddef> 

namespace globals
{
	inline std::uintptr_t bClient = 0;
	inline std::uintptr_t bEngine = 0;
}

namespace offsets
{
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDE997C;
	constexpr ::std::ptrdiff_t m_fFlags = 0x104;
	constexpr ::std::ptrdiff_t dwForceJump = 0x52BAC28;
	constexpr ::std::ptrdiff_t m_clrRender = 0x70;
	constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x5359980;
	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_Dormant = 0xED;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4DFEECC;
	constexpr ::std::ptrdiff_t m_bSpottedByMask = 0x980;
	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
	constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
	constexpr ::std::ptrdiff_t m_flFlashMaxAlpha = 0x1046C;
	constexpr ::std::ptrdiff_t m_vecOrigin = 0x138;
	constexpr ::std::ptrdiff_t m_vecViewOffset = 0x108;
	constexpr ::std::ptrdiff_t dwClientState = 0x59F19C;
	constexpr ::std::ptrdiff_t dwClientState_ViewAngles = 0x4D90;
	constexpr ::std::ptrdiff_t m_aimPunchAngle = 0x303C;
	constexpr ::std::ptrdiff_t m_dwBoneMatrix = 0x26A8;

}
