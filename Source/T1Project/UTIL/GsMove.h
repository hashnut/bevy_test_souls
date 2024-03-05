#pragma once

#include "CoreMinimal.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Engine/Classes/AI/Navigation/NavigationTypes.h"


/* 
	이동 관련 static 함수
*/
class AGsPlayerController;
class AGsCharacterLocalPlayer;
class AGsPlayerController;
class FGsMovementStateAuto;

struct FAIMoveRequest;

class FGsMove
{
	// 이동시 더 가까이 접근하기 위해서 곱할 비율
	static constexpr float APPROACH_TO_TARGET_SCALE_FACTOR = 0.4f;

public:
	// 이동시 
	static float GetMovePosRange(bool In_isOrigin = false)
	{
		float scale = (In_isOrigin) ? 1.f : APPROACH_TO_TARGET_SCALE_FACTOR;
		return GData()->GetGlobalData()->_posMoveAcceptanceRadius * scale;
	}
	// 이동 가능 한 곳인가
	static bool IsMovePossible(AGsPlayerController* In_controller, 
		AGsCharacterLocalPlayer* In_localChar,
		const FVector& In_destPos, const FVector& In_startPos,
		float In_acceptanceRadius, 
		bool In_isTestDrawLine= false);

	// check path move arrived
	// (0.0f or 1.0f make success callback)
	static bool IsAlmostPathMoveArrived(AGsPlayerController* In_controller, 
		const FAIMoveRequest& In_requestMoveData, const FGsMovementStateAuto& In_moveState, 
		float In_addAcceptanceRadius = 1.0f);

	//static bool GetNearMovePossiblePos(const FVector& In_originGoal, FVector& Out_resultPos);
	static bool GetNearMovePossiblePos(const FVector& In_originGoal, FVector& Out_resultPos);

	static void DrawPathLine(UWorld* In_world, FNavPathSharedPtr In_pathSharedPtr, float In_second, FColor In_color);
};