// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetail.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

class UWidget;
class FGsItem;
/**
 * 아이템 관련 인게임 로직에서 사용되는 ParamData
 */
using	FGsItemContentsMessageParamBase = IGsMessageParam;

// ItemDetail
//////////////////////////////////////////////////////////////////////////////////////////
// ItemDetail Open Data
struct FGsItemDetailOpenMessageParam final : public FGsItemContentsMessageParamBase
{
	FGsItem* _item;
	bool _bHideButtons; // 인벤토리에서 열 경우 false
	bool _bHideBG; // 뒷편에 검은BG
	UGsUIPopupItemDetailMain::CenterPosType _centerPosType;
	bool _bHideButtonsBottom; // 버리기, 분해, 장착을 표시 여부
	bool _bUseLock;	// 잠금 기능 사용 가능 여부

	FGsItemDetailOpenMessageParam() = default;
	virtual ~FGsItemDetailOpenMessageParam() = default;
	explicit FGsItemDetailOpenMessageParam(FGsItem* InItem)
		: _item(InItem), _bHideButtons(false), _centerPosType(UGsUIPopupItemDetailMain::CenterPosType::CenterPos), _bHideButtonsBottom(false), _bUseLock(true)
	{
	}

	explicit FGsItemDetailOpenMessageParam(FGsItem* InItem, bool bInHideButtons, bool bInHideBG, UGsUIPopupItemDetailMain::CenterPosType InCenterType)
		: _item(InItem), _bHideButtons(bInHideButtons), _bHideBG(bInHideBG), _centerPosType(InCenterType), _bHideButtonsBottom(false), _bUseLock(true)
	{
	}

	explicit FGsItemDetailOpenMessageParam(FGsItem* InItem, bool bInHideButtons, bool bInHideBG, UGsUIPopupItemDetailMain::CenterPosType InCenterType, bool bInHideButtonsBottom, bool bInUseLock = true)
		: _item(InItem), _bHideButtons(bInHideButtons), _bHideBG(bInHideBG), _centerPosType(InCenterType), _bHideButtonsBottom(bInHideButtonsBottom), _bUseLock(bInUseLock)
	{
	}
};

struct FGsItemDetailOpenMessageParamEX final : public FGsItemContentsMessageParamBase
{
	struct DetailOption
	{
		bool _bHideButtons = false;				// 인벤토리에서 열 경우 false
		bool _bHideBG = false;					// 뒷편에 검은BG
		bool _bHideButtonsBottom = false;		// 버리기, 분해, 장착을 표시 여부
		bool _bUseLock = true;					// 잠금 기능 사용 가능 여부
		bool _bUseCollectionMarking = false;	// 2023/4/18 PKT - 아이템 컬렉션 표기 하는지 여부( 인벤토리 / 창고 아이템 상세창에서 사용.. )
	};

	using PosDir = UGsUIPopupItemDetailMain::CenterPosType;

	FGsItem* _item = nullptr;
	PosDir _posDir = PosDir::CenterPos;
	DetailOption _option;

	FGsItemDetailOpenMessageParamEX() = default;
	virtual ~FGsItemDetailOpenMessageParamEX() = default;
	explicit FGsItemDetailOpenMessageParamEX(FGsItem* InItem, PosDir InPosDir, const DetailOption& InDetailOption) : _item(InItem), _posDir(InPosDir), _option(InDetailOption)
	{
	}
};

// ItemDetail Open Data2
struct FGsItemDetailOpenMessageParamTID final : public FGsItemContentsMessageParamBase
{
	uint32 _itemTID;
	bool _bHideButtons; // 인벤토리에서 열 경우 false
	bool _bHideBG; // 뒷편에 검은BG
	UGsUIPopupItemDetailMain::CenterPosType _centerPosType;

	FGsItemDetailOpenMessageParamTID() = default;
	virtual ~FGsItemDetailOpenMessageParamTID() = default;
	explicit FGsItemDetailOpenMessageParamTID(uint32 itemTID, bool bInHideButtons, bool bInHideBG, UGsUIPopupItemDetailMain::CenterPosType InCenterType)
		: _itemTID(itemTID), _bHideButtons(bInHideButtons), _bHideBG(bInHideBG), _centerPosType(InCenterType)
	{
	}
};

// Currency ItemDetail Open Data
struct FGsCurrencyDetailOpenMessageParam final : public FGsItemContentsMessageParamBase
{
	CurrencyType _currencyType;
	Currency _amount;
	bool _bHideButtons; // 인벤토리에서 열 경우 false
	bool _bHideBG; // 뒷편에 검은BG
	UGsUIPopupItemDetailMain::CenterPosType _centerPosType;

	FGsCurrencyDetailOpenMessageParam() = default;
	virtual ~FGsCurrencyDetailOpenMessageParam() = default;
	explicit FGsCurrencyDetailOpenMessageParam(const CurrencyType InCurrencyType, const Currency InAmount)
		: _currencyType(InCurrencyType), _amount(InAmount), _bHideButtons(false), _centerPosType(UGsUIPopupItemDetailMain::CenterPosType::CenterPos)
	{
	}

	explicit FGsCurrencyDetailOpenMessageParam(const CurrencyType InCurrencyType, const Currency InAmount, bool bInHideButtons, bool bInHideBG, UGsUIPopupItemDetailMain::CenterPosType InCenterType)
		: _currencyType(InCurrencyType), _amount(InAmount), _bHideButtons(bInHideButtons), _bHideBG(bInHideBG), _centerPosType(InCenterType)
	{
	}
};

// ItemDetail Compare Open Data
struct FGsItemDetailCompareOpenMsgParam final : public FGsItemContentsMessageParamBase
{
	FGsItem * _targetItem;
	bool _bHideButtons;
	bool _bCenterPopup;
	bool _bHideBG; // 뒷편에 검은BG
	bool _bHideButtonsBottom; // 버리기, 분해, 장착을 표시 여부
	bool _bUseLock;	// 잠금 기능 사용 가능 여부

	FGsItemDetailCompareOpenMsgParam() = default;
	virtual ~FGsItemDetailCompareOpenMsgParam() = default;
	explicit FGsItemDetailCompareOpenMsgParam(FGsItem * InItem , bool InHideButtons, bool InCenterPopup, bool InHideBG, bool bInHideButtonsBottom = false, bool bInUseLock = true)
		: _targetItem(InItem), _bHideButtons(InHideButtons), _bCenterPopup(InCenterPopup), _bHideBG(InHideBG), _bHideButtonsBottom(bInHideButtonsBottom), _bUseLock(bInUseLock)
	{
	}
};

// ItemDetail ItemCollection Open Data
struct FGsItemDetailItemCollectionOpenMsgParam final : public FGsItemContentsMessageParamBase
{
	const FGsItem* _item;
	bool _isShowBtn = false;
	bool _isEnchant = false;
	bool _bChanged = false;
	bool _bRegister = false;	
	bool _btnRegistedrType = true;

	FGsItemDetailItemCollectionOpenMsgParam() = default;
	virtual ~FGsItemDetailItemCollectionOpenMsgParam() = default;
	explicit FGsItemDetailItemCollectionOpenMsgParam(const FGsItem* InItem , bool InShowBtn, bool InEnchant, bool InChanged, bool InRegister, bool InRegistedrType=true)
		: _item(InItem), _isShowBtn(InShowBtn), _isEnchant(InEnchant), _bChanged(InChanged), _bRegister(InRegister), _btnRegistedrType(InRegistedrType)
	{
	}
};

struct FGsItemDetailTextPropertyParam final : public FGsItemContentsMessageParamBase
{
	const FText * _textParam = nullptr;

	FGsItemDetailTextPropertyParam() = default;
	virtual ~FGsItemDetailTextPropertyParam() = default;
	explicit FGsItemDetailTextPropertyParam(const FText * InTextParam)
		: _textParam(InTextParam)
	{
	}
};

struct FGsItemDetailCloseMessageParam final : public FGsItemContentsMessageParamBase
{
	
public:
	FGsItemDetailCloseMessageParam() = default;
	virtual ~FGsItemDetailCloseMessageParam() = default;
};

// Item Skill Info Tooltip
struct FGsItemDetailSkillInfoMessage final : public FGsItemContentsMessageParamBase
{
	// UObject 의 UniqueId. 오브젝트가 살아있을 때만 유효하며, 재사용된다. 태그로 생각하면 편함
	uint32 _uniqueId;
	FString _titleString;
	FString _descString;
	bool _isPassive;

public:
	FGsItemDetailSkillInfoMessage() = default;
	virtual ~FGsItemDetailSkillInfoMessage() = default;
	explicit FGsItemDetailSkillInfoMessage(uint32 uniqueId, FString&& InTitleString, FString&& InDescString, bool InIsPassive) :
		_uniqueId(uniqueId), _titleString(MoveTemp(InTitleString)), _descString(MoveTemp(InDescString)), _isPassive(InIsPassive)
	{
	}
	explicit FGsItemDetailSkillInfoMessage(uint32 uniqueId, const FString& InTitleString, const FString& InDescString, bool InIsPassive) :
		_uniqueId(uniqueId), _titleString(InTitleString), _descString(InDescString), _isPassive(InIsPassive)
	{
	}
};

struct FGsItemDetailSummonTickerParam : public FGsItemContentsMessageParamBase
{
	uint8 _type{ 0 };
	int32 _detailId{ 0 };

public:
	FGsItemDetailSummonTickerParam() = default;
	virtual ~FGsItemDetailSummonTickerParam() = default;
	explicit FGsItemDetailSummonTickerParam(uint8 type, int32 detailId)
		:_type(type), _detailId(detailId)
	{
	}
};

//////////////////////////////////////////////////////////////////////////////////////////


// ItemDecompose
//////////////////////////////////////////////////////////////////////////////////////////
struct FGsItemDecomposeSingle final : public FGsItemContentsMessageParamBase
{
	uint64 _itemDBID = 0;
	
public:
	FGsItemDecomposeSingle() = default;
	virtual ~FGsItemDecomposeSingle() = default;
	explicit FGsItemDecomposeSingle(uint64 itemDBID)
		:
		_itemDBID(itemDBID)
	{
	}
};

struct FGsItemMultiDecomposeOpen final : public FGsItemContentsMessageParamBase
{

public:
	FGsItemMultiDecomposeOpen() = default;
	virtual ~FGsItemMultiDecomposeOpen() = default;

};

struct FGsItemMultiDecomposeResult final : public FGsItemContentsMessageParamBase
{
	bool _isAutoDecompose = false;

public:
	FGsItemMultiDecomposeResult() = default;
	virtual ~FGsItemMultiDecomposeResult() = default;
	explicit FGsItemMultiDecomposeResult(bool InIsAutoDecompose)
		:
		_isAutoDecompose(InIsAutoDecompose)
	{
	}
};







//////////////////////////////////////////////////////////////////////////////////////////

// Hud QuickSlot
//////////////////////////////////////////////////////////////////////////////////////////

// Invalidate All QuickSlotList
struct FGsInvalidateQuickSlot final : public FGsItemContentsMessageParamBase
{

public:
	FGsInvalidateQuickSlot() = default;
	virtual ~FGsInvalidateQuickSlot() = default;
};

// Invalidate QuickSlotByIndex
struct FGsInvalidateQuickSlotIndex final : public FGsItemContentsMessageParamBase
{
	TArray<uint8> _quickSlotIndexList;

public:
	FGsInvalidateQuickSlotIndex() = default;
	virtual ~FGsInvalidateQuickSlotIndex() = default;
	explicit FGsInvalidateQuickSlotIndex(TArray<uint8> slotList)
		:
		_quickSlotIndexList(MoveTemp(slotList))
	{
	}
};

struct FGsOnChangeEditEffect final : public FGsItemContentsMessageParamBase
{
	bool _isPlay = false;

public:
	FGsOnChangeEditEffect() = default;
	virtual ~FGsOnChangeEditEffect() = default;
	explicit FGsOnChangeEditEffect(bool isPlay)
		:
		_isPlay(isPlay)
	{
	}
};

// Invalidate QuickSlot PacketData
struct FGsInvalidateQuickSlotPacketData final : public FGsItemContentsMessageParamBase
{
	TArray<uint8> _quickSlotIndexList;

public:
	FGsInvalidateQuickSlotPacketData() = default;
	virtual ~FGsInvalidateQuickSlotPacketData() = default;
	explicit FGsInvalidateQuickSlotPacketData(TArray<uint8> slotList)
		:
		_quickSlotIndexList(MoveTemp(slotList))
	{
	}

};

// Quickslot Register
struct FGsRegisterQuickSlot final : public FGsItemContentsMessageParamBase
{
	uint64 _invenItemDBID;
	uint8 _quickSlotIndex;

public:
	FGsRegisterQuickSlot() = default;
	virtual ~FGsRegisterQuickSlot() = default;
	explicit FGsRegisterQuickSlot(uint64 invenItemDBID, uint8 quickSlotIndex)
		:
		_invenItemDBID(invenItemDBID), _quickSlotIndex(quickSlotIndex)
	{
	}
};

// Swap QuickSlot
struct FGsSwapQuickSlot final : public FGsItemContentsMessageParamBase
{
	QuickSlotId _quickSlotIndex1;
	QuickSlotId _quickSlotIndex2;

public:
	FGsSwapQuickSlot() = default;
	virtual ~FGsSwapQuickSlot() = default;
	explicit FGsSwapQuickSlot(QuickSlotId quickSlotIndex1, QuickSlotId quickSlotIndex2)
		:
		_quickSlotIndex1(quickSlotIndex1), _quickSlotIndex2(quickSlotIndex2)
	{
	}
};

// OnClick QuickSlot RemoveIcon
struct FGsOnClickRemoveQuickSlotIcon final : public FGsItemContentsMessageParamBase
{
	uint8 _quickSlotIndex;

public:
	FGsOnClickRemoveQuickSlotIcon() = default;
	virtual ~FGsOnClickRemoveQuickSlotIcon() = default;
	explicit FGsOnClickRemoveQuickSlotIcon(uint8 quickSlotIndex)
		:
		_quickSlotIndex(quickSlotIndex)
	{
	}
};

// OnClick Use QuickSlot Icon
struct FGsOnClickUseQuickSlotItem final : public FGsItemContentsMessageParamBase
{
	uint8 _quickSlotIndex;

public:
	FGsOnClickUseQuickSlotItem() = default;
	virtual ~FGsOnClickUseQuickSlotItem() = default;
	explicit FGsOnClickUseQuickSlotItem(uint8 quickSlotIndex)
		:
		_quickSlotIndex(quickSlotIndex)
	{
	}
};

// OnClick Use Auto Icon
struct FGsOnChangeQuickSlotAutoState final : public FGsItemContentsMessageParamBase
{
	uint8 _quickSlotIndex;
	uint64 _itemDBID;
	bool _autoState;

public:
	FGsOnChangeQuickSlotAutoState() = default;
	virtual ~FGsOnChangeQuickSlotAutoState() = default;
	explicit FGsOnChangeQuickSlotAutoState(uint8 quickSlotIndex , uint64 ItemDBID , bool autoState)
		:
		_quickSlotIndex(quickSlotIndex), _itemDBID(ItemDBID), _autoState(autoState)
	{
	}
};

// OnClick Use Auto Icon
struct FGsOnReceiveQuickSlotAutoState final : public FGsItemContentsMessageParamBase
{
	TArray<QuickSlotId> _slotList;

public:
	FGsOnReceiveQuickSlotAutoState() = default;
	virtual ~FGsOnReceiveQuickSlotAutoState() = default;
};

// OnChange QuickSlot Effect
struct FGsOnChangeQuickSlotEffect final : public FGsItemContentsMessageParamBase
{
	bool _flag;

public:
	FGsOnChangeQuickSlotEffect() = default;
	virtual ~FGsOnChangeQuickSlotEffect() = default;
	explicit FGsOnChangeQuickSlotEffect(bool flag)
		:
		_flag(flag)
	{
	}
};

struct FGsOnSelectInvenItemDbid final : public FGsItemContentsMessageParamBase
{
	ItemDBId _selectionDbid;

public:
	FGsOnSelectInvenItemDbid() = default;
	virtual ~FGsOnSelectInvenItemDbid() = default;
	explicit FGsOnSelectInvenItemDbid(ItemDBId selectionDbid)
		:
		_selectionDbid(selectionDbid)
	{
	}
};

// Add Depot Item
struct FGsAddDepotItem final : public FGsItemContentsMessageParamBase
{
	uint64 _itemDBID;

public:
	FGsAddDepotItem() = default;
	virtual ~FGsAddDepotItem() = default;
	explicit FGsAddDepotItem(uint64 itemDBID)
		:
		_itemDBID(itemDBID)
	{
	}
};

struct FGsUpdateDepotItem final : public FGsItemContentsMessageParamBase
{
	uint64 _itemDBID;

public:
	FGsUpdateDepotItem() = default;
	virtual ~FGsUpdateDepotItem() = default;
	explicit FGsUpdateDepotItem(uint64 itemDBID)
		:
		_itemDBID(itemDBID)
	{
	}
};

struct FGsRemoveDepotItem final : public FGsItemContentsMessageParamBase
{
	int32 _itemTID;
	uint64 _itemDBID;

public:
	FGsRemoveDepotItem() = default;
	virtual ~FGsRemoveDepotItem() = default;
	explicit FGsRemoveDepotItem(int32 itemTID, uint64 itemDBID)
		:
		_itemTID(itemTID), _itemDBID(itemDBID)
	{
	}
};

// Display Item List
struct FGsDepotInvenItemList final : public FGsItemContentsMessageParamBase
{
	TArray<uint64> _itemDBIDList;

public:
	FGsDepotInvenItemList() = default;
	virtual ~FGsDepotInvenItemList() = default;
	explicit FGsDepotInvenItemList(TArray<uint64> itemDBIDList)
		:
		_itemDBIDList(MoveTemp(itemDBIDList))
	{
	}
};

struct FGsOnChangeDepotBasketItem final : public FGsItemContentsMessageParamBase
{
	uint64 _itemDBID;

public:
	FGsOnChangeDepotBasketItem() = default;
	virtual ~FGsOnChangeDepotBasketItem() = default;
	explicit FGsOnChangeDepotBasketItem(uint64 itemDBID)
		:
		_itemDBID(itemDBID)
	{
	}
};

struct FGsOnAddDepotBasketItemList final : public FGsItemContentsMessageParamBase
{
	TArray<uint64> _itemDBIdList;

public:
	FGsOnAddDepotBasketItemList() = default;
	virtual ~FGsOnAddDepotBasketItemList() = default;
	explicit FGsOnAddDepotBasketItemList(TArray<uint64> InItemDBIdList)
		:
		_itemDBIdList(MoveTemp(InItemDBIdList))
	{
	}
};

// Display Item List
struct FGsDepotInvenOwnedItemListCount final : public FGsItemContentsMessageParamBase
{
	int32 _itemCount;

public:
	FGsDepotInvenOwnedItemListCount() = default;
	virtual ~FGsDepotInvenOwnedItemListCount() = default;
	explicit FGsDepotInvenOwnedItemListCount(int32 itemCount)
		:
		_itemCount(itemCount)
	{
	}
};

struct FGsDepotClickAutoDepositSlot final : public FGsItemContentsMessageParamBase
{
	ItemId _itemId = INVALID_ITEM_ID;
	bool _isSelected = false;

public:
	FGsDepotClickAutoDepositSlot() = default;
	virtual ~FGsDepotClickAutoDepositSlot() = default;
	explicit FGsDepotClickAutoDepositSlot(ItemId InItemId, bool InIsSelected)
		:
		_itemId(InItemId), _isSelected(InIsSelected)
	{
	}
};

// LongPress
//////////////////////////////////////////////////////////////////////////////////////////
// Open Longpress
struct FGsOpenLongPressMessageParam final : public FGsItemContentsMessageParamBase
{
	FGsItem* _item;
	UWidget* _targetWidget;
	bool _bHideButtons; // 인벤토리에서 열 경우 false
	bool _bCenterPopup;
	bool _bHideBG; // 뒷편에 검은BG

	FGsOpenLongPressMessageParam() = default;
	virtual ~FGsOpenLongPressMessageParam() = default;
	explicit FGsOpenLongPressMessageParam(FGsItem* InItem, UWidget* InTargetWidget)
		: _item(InItem), _targetWidget(InTargetWidget), _bHideButtons(false), _bCenterPopup(false)
	{
	}

	explicit FGsOpenLongPressMessageParam(FGsItem* InItem, UWidget* InTargetWidget ,bool bInHideButtons, bool bInCenterPopup, bool bInHideBG)
		: _item(InItem), _targetWidget(InTargetWidget), _bHideButtons(bInHideButtons), _bCenterPopup(bInCenterPopup), _bHideBG(bInHideBG)
	{
	}
};

// Close Longpress
struct FGsCloseLongPressMessageParam final : public FGsItemContentsMessageParamBase
{

public:
	FGsCloseLongPressMessageParam() = default;
	virtual ~FGsCloseLongPressMessageParam() = default;
};

// 아이템 바로가기로 거래소 열기
//////////////////////////////////////////////////////////////////////////////////////////
struct FGsItemDetailOpenMarketParam final : public FGsItemContentsMessageParamBase
{
public:
	enum class EGsMarketSearchOption
	{
		DEFAULT, // 기본규칙(에픽미만 일반서버, 에픽이상 월드서버)
		NORMAL_SERVER, // 일반 서버에서 검색
		WORLD_SERVER, // 월드 서버에서 검색
	};

	uint32 _itemTId = 0;
	EGsMarketSearchOption _searchOption = EGsMarketSearchOption::DEFAULT;

public:
	explicit FGsItemDetailOpenMarketParam(uint32 InItemTId, EGsMarketSearchOption InSearchOption)
		: _itemTId(InItemTId), _searchOption(InSearchOption)
	{
	}

	virtual ~FGsItemDetailOpenMarketParam() = default;
};


// 아이템 컬렉션 관련 등록 / 교환 명령
//////////////////////////////////////////////////////////////////////////////////////////
struct FGsItemItemCollectionRegisterParam final : public FGsItemContentsMessageParamBase
{
	ItemDBId _itemDBId = 0;

public:
	FGsItemItemCollectionRegisterParam() = default;
	virtual ~FGsItemItemCollectionRegisterParam() = default;

	explicit FGsItemItemCollectionRegisterParam(ItemDBId InItemDBId)
		: _itemDBId(InItemDBId)
	{
	}
};

struct FGsItemItemCollectionShiftParam final : public FGsItemContentsMessageParamBase
{
	ItemDBId _itemDBId = INVALID_ITEM_DB_ID;
	ItemId _itemTId = INVALID_ITEM_ID;

public:
	FGsItemItemCollectionShiftParam() = default;
	virtual ~FGsItemItemCollectionShiftParam() = default;

	explicit FGsItemItemCollectionShiftParam(ItemDBId InItemDBId, ItemId itemId)
		: _itemDBId(InItemDBId), _itemTId(itemId)
	{
	}
};


// Open PickOne Box
struct FGsOpenPickOneBox final : public FGsItemContentsMessageParamBase
{
	uint64 _itemDBID;
	int32 _rewardID;
	bool _isSeveralUse;

public:
	FGsOpenPickOneBox() = default;
	virtual ~FGsOpenPickOneBox() = default;
	explicit FGsOpenPickOneBox(uint64 itemDBID, int32 rewardID, bool isSeveralUse)
		:
		_itemDBID(itemDBID), _rewardID(rewardID), _isSeveralUse(isSeveralUse)
	{
	}
};

struct FGsConfirmPickOne final : public FGsItemContentsMessageParamBase
{
	uint64 _itemDBID;
	uint8 _index;
	uint32 _amount;

public:
	FGsConfirmPickOne() = default;
	virtual ~FGsConfirmPickOne() = default;
	explicit FGsConfirmPickOne(uint64 itemDBID, uint8 index, uint32 amount)
		:
		_itemDBID(itemDBID), _index(index), _amount(amount)
	{
	}
};


// 공용 인벤토리
//////////////////////////////////////////////////////////////////////////////////////////
struct FGsInventoryCommonInvalidate final : public FGsItemContentsMessageParamBase
{
	uint64 _itemDBID;

public:
	FGsInventoryCommonInvalidate() = default;
	virtual ~FGsInventoryCommonInvalidate() = default;
	explicit FGsInventoryCommonInvalidate(uint64 itemDBID)
		: _itemDBID(itemDBID)
	{
	}
};