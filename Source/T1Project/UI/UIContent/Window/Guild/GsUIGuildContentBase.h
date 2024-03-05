// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIGuildContentBase.generated.h"

/**
 * 길드 각 탭 페이지의 부모 클래스. 여러 페이지에서 같이 쓸 것 같은 함수는 가상함수로 만들기
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIGuildContentBase : public UUserWidget, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

public:
	// 메인 탭 레드닷 갱신 요청
	DECLARE_DELEGATE_OneParam(FOnReqUpdateMainTabRedDot, int32 /*_mainTabIndex*/);

protected:
	int32 _mainTabIndex = 0;

public:
	FOnReqUpdateMainTabRedDot OnReqUpdateMainTabRedDot;

public:
	virtual FString GetTabName() const { return TEXT("None"); }

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	// IGsInvalidateUIInterface override
	virtual void InvalidateAllInternal() override {}

public:	
	virtual void SendRequestPacket() {}
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) {}
	virtual void Hide() {}

	// 시간제 컨텐츠 리셋
	virtual void OnResetTimeContents(ResetTimeTicket InType) {}
	// 권한 변경이 일어날 경우 호출
	virtual void OnChangeMyAuthority() {}
	// 길드 세팅 변경
	virtual void OnReceiveChangeGuildSetting(const struct FGsGuildMsgParamSetting* InParam) {}
	// 멤버 등급 변경 노티
	virtual void OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam) {}	
	// 재화 변경이 있을 경우 호출(기사단 운영비 포함)
	virtual void OnChangeCurrency(CurrencyType InType) {}
	// 길드 레벨 변경 노티(+캠프레벨 체크도 같이 해도 될듯)
	virtual void OnChangeGuildLevel() {}
	// 길드 건물 상태 변경됨
	virtual void OnUpdateBuilding(GuildBuildingId InBuildingId) {}
	// 공지 변경 알림 받음
	virtual void OnNotifyChangeNotice() {}	

public:
	void SetMainTabIndex(int32 InIndex) { _mainTabIndex = InIndex; }
	int32 GetMainTabIndex() const { return _mainTabIndex; }
};
