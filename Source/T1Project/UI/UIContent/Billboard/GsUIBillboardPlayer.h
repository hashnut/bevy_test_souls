// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Option/GsOptionValueDefine.h"
#include "GsUIBillboardPlayer.generated.h"

class UImage;
class UTextBlock;
class UPanelWidget;
class UGsUITalkBubble;
class UGsUIGuildIcon;
class FGsTitlePlayerData;
struct FGsGuildPlayerData;
struct IGsMessageParam;
class FGsGameObjectDataPlayer;

/**
 * 플레이어 빌보드(Abstract)
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIBillboardPlayer : public UGsUIBillboardCreature
{
	GENERATED_BODY()

protected:
	// 길드
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelGuild;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildIcon* _iconGuild;
	
	// 파티(1P, 2P, ...)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockParty;

	// 칭호
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitleFront;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitleBack;

	// 서버명
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockServer;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgContribution;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgAggro;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITalkBubble* _talkBubble;
	
protected:
	const float _talkRemainTime = 10.f;
	float _talkBubbleTime = 0.f;

protected:
	// UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// UGsUIWidgetBase override
	virtual void InvalidateAllInternal() override;

	// UGsUIBillboardCreature override
	virtual void InitFunctionMap() override;
	virtual void SetNameColor(const FLinearColor& InColor) override;

protected:
	// 옵션값으로 이름이 보여야하는지 검사
	virtual bool IsShowNameOption() const { return true; }
	virtual bool CheckNameColor(OUT FLinearColor& outColor) { return false; }
	virtual bool HasServerPrefix() const { return false; }

	virtual PVPAlignmentGrade GetPVPAlignmentGrade() { return PVPAlignmentGrade::NETURAL; }
	
	virtual bool SetUIGuild(const FGsGuildPlayerData* InData, bool bIsShowOption) { return false; }
	virtual void SetUITitle(const class FGsTitlePlayerData* InTitleData, int32 InLevel, bool bIsShowOption);

protected:
	void UpdateName(); // 이름 갱신
	void UpdateNameColor(); // 이름 색상 갱신
	void UpdateGuild(); // 길드 표시 갱신
	void UpdateParty(); // 파티 표시 갱신
	void UpdateTitle(); // 칭호 갱신	
	void UpdateContribution();
	void UpdateAggro();
	

protected:	
	void SetUIName(const FGsGameObjectDataPlayer* InData, bool bIsShowOption);
	void SetUIParty(int32 InPartySlotId);
	void SetUIContribution(bool bIsShow);
	void SetUIAggro(bool bIsShow);

protected:
	EGsOptionValuePCNameType GetOptionPCNameType() const;	
	bool IsInvadeWorld() const;

public:
	void SetTalkBubbleVisible(bool bInIsShow);
	void SetTalkText(FText text);
	void SetEmoticon(int32 emojiID);

	//PVPAlignment (카오)
protected:
	bool IsPVPAlignmentNeutral();
	FLinearColor GetPVPAlignmentGradeColor(PVPAlignmentGrade inPvpAlignmentGrade);
};
