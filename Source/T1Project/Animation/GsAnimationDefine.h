#pragma once

/*
* define: animation
*/
// Idle 타입이 여러개로 세분화 되면서 Enum타입으로 정의
UENUM(BlueprintType)
enum class EGsAnimStateIdleType : uint8
{
	Normal,					// 평화 Idle
	Battle,					// 전투 Idle
	Social,					// 대화 준비 Idle
	Abnormality,			// 상태이상 Idle
};

// 블렌딩이 필요한 타입 정의
UENUM(BlueprintType)
enum class EGsAnimationBlendType : uint8
{
	Default,			// 아무것도 지정하지 않고 블렌딩할 경우 이타입을 사용

	MAX
};

class FGsAnimationDefine
{
public:
	static const FName LOBBY_M_IDLE_POSE;
	static const FName LOBBY_F_IDLE_POSE;
};