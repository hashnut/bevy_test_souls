// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"

#include "Message/GsMessageItem.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"

#include "GsUIPopupPlayerStat.generated.h"


class UGsUIRedDotBase;
class UGsButton;
class UTextBlock;
class UWidgetSwitcher;
class UScrollBox;
class UVerticalBox;
class UGsDynamicPanelSlotHelper;
class UGsUIPlayerEquipInfo;
class FGsStatInfo;
struct FGsItemMessageParamBase;
struct FGsPlayerDetailInfoMessageParamBase;
struct IGsMessageParam;

enum class EGsPlayerStatInvalidateFlags : uint8
{
	NONE = 0,

	STAT_DEFAULT = 1 << 0,
	STAT_BOOKMARK = 1 << 1,
	EQUIP_SLOT = 1 << 2,
	RED_DOT = 1 << 3,

	ALL = STAT_DEFAULT | STAT_BOOKMARK | EQUIP_SLOT | RED_DOT,
};
ENUM_CLASS_FLAGS(EGsPlayerStatInvalidateFlags);
/**
 *  캐릭터 상세창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupPlayerStat : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStatDistribution;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStatDetailInfo;
	UPROPERTY(BlueprintReadOnly, Category = "Reddot", meta = (BindWidget))
	UGsUIRedDotBase* _statDistributionRedDot;

	// 플레이어 스탯
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStr;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDex;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockInt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockAgi;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockWis;

	// 플레이어 상세 스탯 : 즐겨찾기
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBookmark;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxBookmark;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _panelBookmarkEntry;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfBookmarkEntry;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperBookmark;

	// 장착 장비 UI
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPlayerEquipInfo* _equipInfo;

protected:
	TArray<TPair<MessageContentPlayerInfo, FDelegateHandle>>	_listPlayerDetailInfoDelegate;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;
	MsgUIHandleArray _listUIDelegate;
	MsgHUDRedDotHandleArray _listRedDotDelegate;
	MsgGameObjHandleArray _listGameObjectDelegate;

	TMap<StatType, UTextBlock*> _statTextBlockMap;

	EGsPlayerStatInvalidateFlags _invalidateFlags = EGsPlayerStatInvalidateFlags::NONE;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void Close(bool bImmediate = false, bool bRecoverPrevStack = true) override;

	void RegisterMessage();
	void RemoveMessage();

public:
	virtual void InvalidateAllInternal() override;
	void InvalidateStatDefault();
	void InvalidateStatFavorite();
	void InvalidateEquipSlot(EGsEquipSlotType InSlotType);
	void InvalidateEquipSlotAll();
	void InvalidateStatDistributionRedDot(bool InIsActive = false);

protected:
	void OnUpdateItem(FGsItemMessageParamBase& InParam);
	void OnInvalidatePlayerStat(const IGsMessageParam* InParam);
	void OnInvalidatePlayerStatDistribution(const IGsMessageParam* InParam);
	void OnInvalidateEquip(FGsPlayerDetailInfoMessageParamBase* InParam);
	void OnInvalidateEquipAll(FGsPlayerDetailInfoMessageParamBase* InParam);
	void OnPlayerStatUpdate(const IGsMessageParam* InParam);

protected:
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnClickStatDistribution();
	UFUNCTION()
	void OnClickStatDetailInfo();
	UFUNCTION()
	void OnRefreshBookmark(int32 InIndex, UWidget* InEntry);

protected:
	void SetMainStatValueText(StatType InStatType);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};