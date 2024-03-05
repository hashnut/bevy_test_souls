// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Client/SharedEnums/SharedNoticeEnum.h"
#include "Classes/Engine/StreamableManager.h"
#include "Components/SlateWrapperTypes.h"
#include "VisibilityBinding.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Manager/GsBlockUIController.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Classes/GsDelegate.h"
#include "UI/UIContent/Tray/GsUITrayTickerBoss.h"

/*
 * FGsUIHelper
 * 각종 UI 편의용 static 함수 등록
 */

struct FLinearColor;
class FGsBMShopProductData;
struct FGsSchemaSoundResData;

 enum class NotiHeaderType { None = 0, Guild, Party, Alert };

struct T1PROJECT_API FGsUIHelper
{
public:
	// 시스템 팝업을 띄운다. (시스템 팝업: Depth 높음, Stack 최상단, 로딩중 닫히지 않음)
	static void PopupSystemMsg(const FText& InMessage, TFunction<void()> InCloseCallback = nullptr);

	// 시스템 YesNo 팝업을 띄운다.
	static void PopupSystemYesNo(const FText& InMessage, TFunction<void(bool)> InCloseCallback = nullptr);

	// 일반 OK 팝업을 띄운다. (일반 Popup과 동일한 Depth, Stack 관리. 로딩중 닫힘)
	static void PopupOK(const FText& InMessage, TFunction<void()> InCloseCallback = nullptr);

	// 일반 YesyNo 팝업을 띄운다. (일반 Popup과 동일한 Depth, Stack 관리. 로딩중 닫힘)
	static class UGsUIPopupYesNo* PopupYesNo(const FText& InMessage, TFunction<void(bool)> InCloseCallback = nullptr,
		TFunction<void(bool)> InDestructCallback = nullptr);
	static class UGsUIPopupYesNoTwoMessage* PopupYesNoTwoMessage(const FText& InMessage1, const FText& InMessage2, TFunction<void(bool)> InCloseCallback = nullptr);
	
	// 네트워크 에러출력 전용. 에러로그를 남기고 시스템 팝업을 띄운다.
	static void PopupNetError(const FText& InMessage);
	static void PopupNetError(const FTextKey& InKey);
	static void PopupNetError(PD::Result InMainResult, PD::Result InDetailResult, bool bIsLauncher = false);
	static void PopupNetError(PD::Result InMainResult, PD::Result InDetailResult, const FString& InHiveHostCategory, bool bIsLauncher = false);
	static void PopupNetError(PD::Result InResult, const FString& InValue, bool bIsLauncher = false);
	static void PopupNetError(PD::Result InResult, bool bIsLauncher = false);
	static void LogNetError(PD::Result InResult);
	//------------------------------------------------------------------------------------------------------------------	 
	// 화면 블록 UI 출력
	static void ShowBlockUI();	
	static void HideBlockUI();
	// 필요시 ShowBlock 후에 세팅(안열려있으면 무시됨) 도중 다른 ShowBlockUI가 들어올 경우 옵션 리셋됨에 유의
	// InLifeSecond는 BLOCK_UI_LIFE_SECOND 이상 넣을 수 없음
	// bInHideUI가 true이면 Throbber, message 모두 표시 안함
	static void SetBlockUISetting(const FText& InMessage, bool bInHideUI = false, float InLifeSecond = 0);

	// HideBlockUI가 호출되어도 여기서 넣어준 항목이 남아있으면 꺼지지 않음
	static void ShowBlockUIExplicit(const FName& InKey);
	static void HideBlockUIExplicit(const FName& InKey);
	// 강제로 블록 UI를 전부 제거할 경우
	static void ForcedHideBlockUI(EGsHideBlockUIFactor InFactor);
	// 블록 UI가 열려있는지 확인
	static bool IsOpenBlockUI();

	//------------------------------------------------------------------------------------------------------------------		
	// 게임 종료 팝업 띄우기. IOS에서는 띄우지 않음.	
	static void PopupQuitGame();

	// 시스템 티커(로딩 중 닫히지 않음). bInPriority 가 true이면 기존 메세지 무조건 끊고 출력
	static void TraySystemTicker(const FText& InMessage, bool bInPriority = false);

	// 일반 티커. bInPriority 가 true이면 기존 메세지 무조건 끊고 출력
	static void TrayMessageTicker(const FText& InMessage, bool bInPriority = false);

	// 텍스쳐 즉시로드 - 아틀라스 안쓰는 이미지만 사용
	static class UTexture2D* LoadTextureFromPath(const FSoftObjectPath& InPath);
	// 텍스쳐 Async로드 - 아틀라스 안쓰는 이미지만 사용
	static void AsyncLoadTextureFromPath(const FSoftObjectPath& InPath, FStreamableDelegateParam inRes);
	// Sprite 즉시로드
	static class UPaperSprite* LoadSpriteFromPath(const FSoftObjectPath& InPath);	
	// Sprite Async로드
	static void AsyncLoadSpriteFromPath(const FSoftObjectPath& InPath, FStreamableDelegateParam inRes);	

	// 시스템 채팅으로 출력(현재 임시로 메시지 티커 출력)
	static void AddChatMessageSystem(const FText& InMessage);

	// 성공 공용티커
	static void TrayTickerClear(TFunction<void()>&& InCallback = nullptr);
	// 실패 공용티커
	static void TrayTickerFail(TFunction<void()>&& InCallback = nullptr);
	// 전투 공용티커
	static void TrayTickerBattle(const FText& InMainText, const FText& InSubText, TFunction<void()>&& InCallback = nullptr);
	// 카운트 공용티커
	static void TrayTickerCount(TFunction<void()>&& InCallback = nullptr);
	static void TrayTickerCount(time_t InStartTime, int32 InCount, TFunction<void()>&& InCallback = nullptr);

	static void TrayTickerCountEx(int32 InCount, time_t InEndTime, TFunction<void()>&& InCallback = nullptr);
	// 2023/3/6 PKT - 만약 열려 있는 상태라면 
	static bool IsActiveTrayTickerCountEx();
	static void CloseTrayTickerCountEx();

	// 범용 공용티커(소환 티커로 사용)
	static void TrayTickerCommon(const FText& InMainText, const FText& InSubText, TFunction<void()>&& InCallback = nullptr);
	// 화면 딤드처리
	static void TrayDimmed(bool isDimmedOn, const FLinearColor& InColor = FLinearColor::Black);

	static void TrayTickerBoss(EGsBossTickerStateType inState, CreatureId inNpcId = 0);
	// 필드 보스 출현 타이밍 개선(고정 시간 스폰, 디스폰) 작업
	// https://jira.com2us.com/jira/browse/CHR-23655
	static void TrayTickerBossDespawn(CreatureId inNpcId);
	static void TrayTickerInvadeOneText(const FText& inContext, FSoftObjectPath inIconPath);
	static void TrayTickerInvadeTwoText(const FText& inContext1, const FText& inContext2, FSoftObjectPath inIconPath);
	static void TrayTickerBMShopPurchase(TPair<FGsBMShopProductData*, ItemAmount> InPairData, bool InIsBulkSale, bool InIsCash);
	static bool TrayPGPayBlock();
	
	/** 천마석 성공 / 실패 티커 띄우기 */
	static void TrayTickerSpiritShotEnchant(FText InText, bool IsSuccess = true);

	// Section 티커.
	static void TraySectionMessageTicker(EGsNotiSection section, const FText& InTagMessage, 
		TrayCondition condition = TrayCondition::TC_Active, SectionPriority priority = SectionPriority::Instant, float playTime = 2.f);
	static void TraySectionMessageDelayTicker(EGsNotiSection section, const FText& InTagMessage, float delayTime = 1.f, 
		TrayCondition condition = TrayCondition::TC_Active, SectionPriority priority = SectionPriority::Instant, float playTime = 2.f);
	static void TraySectionMessageTickerNetError(PD::Result InResult, EGsNotiSection section = EGsNotiSection::EXTSYSTEM);

	static void TrayOperatorMessageTicker(NoticeType noticeType, const FText& InMessage, SectionPriority priority = SectionPriority::Instant, float playTime = 8.f);

	static void OpenHelpPage(EGsHelpContents key);

	static void TrayFadeIn(float InDuration = 1.f, bool InStartNow = true);
	static void TrayLobbyFadeIn(float InDuration, bool InStartNow = true);
	static void StartTrayFadeInAnimaion();
	static void CloseTrayFadeIn();

	// 다운로드 전에 사용할 런처용 티커
	static void TrayTickerLauncher(const FText& InMessage, bool bInPriority = false);

	static void ProcessNetError(uint16 packetId, int32 result);

	static const FSoftObjectPath* GetBadgeIconPath(AccountGrade InAccountType);

	static void Play2DSound(const FString& InSoundName);

	static void Play2DSound(const FGsSchemaSoundResData* InSoundResData);
};
