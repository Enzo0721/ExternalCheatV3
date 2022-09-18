#include <thread>
#include "aimbot.h" 
#include "gui.h"
#include "globals.h"
#include "settings.h"
#include "vector.h"

constexpr Vector3 calculateAngle(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).toAngle() - viewAngles);
}

void aimbot::aimbotThread(const Memory& mem) noexcept
{
	while (gui::exit)
	{
		if (settings::toggle_aimbot)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			if (!GetAsyncKeyState(VK_LBUTTON))
				continue;

			const auto& localBase = mem.read<uintptr_t>(globals::bClient + offsets::dwLocalPlayer);
			const auto& localTeam = mem.read<int32_t>(localBase + offsets::m_iTeamNum);

			const auto localEyePosition = mem.read<Vector3>(localBase + offsets::m_vecOrigin) +
				mem.read<Vector3>(localBase + offsets::m_vecViewOffset);

			const auto& clientState = mem.read<uintptr_t>(globals::bEngine + offsets::dwClientState);

			const auto& localPlayerId = mem.read<int32_t>(clientState + 0x180);

			const auto& viewAngles = mem.read<Vector3>(clientState + offsets::dwClientState_ViewAngles);
			const auto& aimPunch = mem.read<Vector3>(localBase + offsets::m_aimPunchAngle) * 2;

			auto bestFov = settings::aimbotFOV;
			auto bestAngle = Vector3{ };

			for (int i = 1; i <= 32; i++)
			{
				const auto& player = mem.read<uintptr_t>(globals::bClient + offsets::dwEntityList + i * 0x10);

				if (mem.read<int32_t>(player + offsets::m_iTeamNum) == localTeam)
					continue;

				if (mem.read<bool>(player + offsets::m_Dormant))
					continue;

				if (mem.read<int32_t>(player + offsets::m_iHealth))
					continue;

				if (mem.read<int32_t>(player + offsets::m_bSpottedByMask) & (1 << localPlayerId))
					continue;

				const auto boneMatrix = mem.read<uintptr_t>(player + offsets::m_dwBoneMatrix);

				const auto playerHeadPosition = Vector3{
					mem.read<float>(boneMatrix + 0x30 * 8 + 0xC),
					mem.read<float>(boneMatrix + 0x30 * 8 + 0x1C),
					mem.read<float>(boneMatrix + 0x30 * 8 + 0x2C)

				};

				const auto angle = calculateAngle(
					localEyePosition,
					playerHeadPosition,
					viewAngles + aimPunch
				);

				const auto fov = std::hypot(angle.x, angle.y);

				if (fov < bestFov)
				{
					bestFov = fov;
					bestAngle = angle;
				}
			}

			if (!bestAngle.isZero())
				mem.write<Vector3>(clientState + offsets::dwClientState_ViewAngles, viewAngles + bestAngle / 1.f);// last thing is smoothing
		}
	}
}