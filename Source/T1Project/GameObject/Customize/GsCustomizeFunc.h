#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/*
	커스터마이징 관련 컨텐츠 함수
*/

class FGsGameObjectDataLocalPlayer;
class FGsCustomizeHandler;


class FGsCustomizeFunc
{
	// static 함수
public:
	//---------------------------------------------------------------------------------------------
	// 무기를 붙여준다.
	//---------------------------------------------------------------------------------------------
	// FGsCustomizeHandler* customizeHandler : 작업 진행용 Handler
	// UserLookInfo* lookInfo : 무기외형 참고용
	// int32 Tid : DTGsUserData의 Key (소켓 정보를 얻어오기 위함)
	// bool inBattle = false : 전투(손), 평화(등) 소켓에 붙여준다.
	static bool AttachWeapon(FGsCustomizeHandler* customizeHandler, const UserLookInfo* lookInfo, int32 Tid, bool inBattle = false);



	//---------------------------------------------------------------------------------------------
	// 로컬데이터를 통해 커스터마이즈를 진행한다.
	//---------------------------------------------------------------------------------------------
	// FGsCustomizeHandler* In_customizeHandler : 작업 진행용 Handler
	// FGsGameObjectDataLocalPlayer* localData : 내플레이어 GameData정보
	// CostumeId costumeId : 코스튬 정보
	// bool showCustomHair : 커스터 마이즈정보에 포함된 헤어를 세팅한다.
	// bool applyBoneScale = true : 본스케일 (커스터마이즈)를 적용한다.
	// bool useWeapon = false : 무기를 장착할지 여부를 결정한다.
	// bool applyFace = false : 얼굴 커스터마이즈 정보를 적용한다.
	static void SetCustomizeByLocalData(FGsCustomizeHandler* customizeHandler,FGsGameObjectDataLocalPlayer* localData, CostumeId costumeId,
		bool showCustomHair = true, bool applyBoneScale = true, bool useWeapon = false, bool applyFace = true );
};