// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUserInfo.h"
#include "GsUIPopupGuildBuildingLevelUp.generated.h"

class UImage;
class UTextBlock;
class UGsButton;
class UGsUICurrencyButton;
class UGsUITooltipGuildCampBuffInfo;

/**
 * 길드 건물 레벨업 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildBuildingLevelUp : public UGsUIPopup
{
	GENERATED_BODY()

public:
	using MsgGuildHandle = TPair<MessageContentGuild, FDelegateHandle>;
	using MsgUserInfoHandle = TPair<MessageUserInfo, FDelegateHandle>;

public:
	struct FGsBuildingEffectRow
	{
		UTextBlock* _textBlockName;
		UTextBlock* _textBlockValue;

		explicit FGsBuildingEffectRow(UTextBlock* InTextBlockName, UTextBlock* InTextBlockValue)
			: _textBlockName(InTextBlockName), _textBlockValue(InTextBlockValue)
		{
		}

		void SetData(const TPair<FText, FText>& InData);
		void SetEmpty();
		void SetColorValue(const FLinearColor& InColor);
	};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIGuild")
	FLinearColor _colorChangedEffectValue;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;

	// 최대 2개표시 고정이므로 그냥 생성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCurrEffectName0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCurrEffectValue0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCurrEffectName1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCurrEffectValue1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNextEffectName0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNextEffectValue0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNextEffectName1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNextEffectValue1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGuildAsset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNeedGuildLevel;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnLevelUp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	// 캠프일 때만 활성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

protected:
	GuildBuildingId _guildBuildingId = 0;
	int32 _currBuildingLevel = 0;
	int32 _needCost = 0;
	bool _bIsNextLevel = false;
	TArray<FGsBuildingEffectRow> _currEffectList;
	TArray<FGsBuildingEffectRow> _nextEffectList;
	TArray<MsgGuildHandle> _msgGuildHandleList;
	MsgUserInfoHandle _msgUserInfoHandle;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetData(GuildBuildingId InId);

protected:
	void OnReceiveBuildingUpgrade(const struct IGsMessageParam*);
	void OnReceiveChangeMyAuthority(const struct IGsMessageParam*);

	void OnChangeCurrency(uint64 InCurrencyType);

protected:
	UFUNCTION()
	void OnClickLevelUp();
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickTooltip();
};
