#pragma once

/** 제작대상 디스플레이 상태 */
enum class EGsCraftTargetDisplayState : uint8
{
	Hide,
	ShowNotCraftable,
	ShowCraftable,
};