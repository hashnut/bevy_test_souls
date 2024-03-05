#pragma once
#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"
/*
    파티 함수
*/

struct FGsSchemaWeaponUIInfo;

class FGsPartyFunc
{
public:
	// 초대할 사람 이름 체크
	static bool CheckPartyInviteeName(const FString& In_name);
    // 초대할 사람 밴 대상인가
    static bool CheckPartyInviteBan(const FString& In_name);
    // 초대할 사람 이름 글자 크기 체크
    static bool CheckPartyInviteeNameLength(const FString& In_name);
    // 초대 응답 패킷 result 티커 출력
    static bool ShowTickerPartyInviteError(PD::Result In_result);
    // show error ticker: ack party leader change
    static bool ShowTickerPartyChangeLeader(PD::Result In_result);
    
    // weapon ui data 접근
    static const FGsSchemaWeaponUIInfo* GetWeaponUIInfo(CreatureWeaponType In_weaponType);
    // 파티 나감 팝업 열기
    static void OpenLeavePopup();
	// 파티 나감 팝업 열기
	static void OpenLeavePopup(TFunction<void(void)> Infunc);
    // 추방 티커 출력
    static void ShowTickerKicked(const FString& In_name);

    static void ShowTickerInviteFailed();
    // drop ownership change ticker
    static void ShowTickerChangeDropOwnershipType(PartyDropOwnershipType In_type);

    static void ShowChatLeaderChange(FString In_newLeader);

    static void ShowTickerPartyMemberTarget();
    static void ShowTickerTryShareByPartyMember();

    // 파티 초대 공통 로직
    static void InviteParty(const FString& In_inviteeName, WorldId In_homeWorldId = 0);
    // 파티원 이름 구하기(prefix 붙일지 말지 현재 맵에따라 처리)
    static FString GetPartyMemberName(const FString& In_name, WorldId In_homeWorldId);
};