#pragma once

#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageUserInfo.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/GsUIWindowCraft.h"

struct IGsMessageParam;

class T1PROJECT_API FGsGameStateCraft final : public FGsContentsGameBase
{

	friend class FGsContentsGameAllocator;

	using Super = FGsContentsGameBase;

private:
	TWeakObjectPtr<UGsUIWindowCraft> _window;

	/** 뒤로 가기 등에 활용하는 히스토리큐 */
	TArray<UGsUIWindowCraft::Parameters> _historyStack;

private:
	TPair<MessageContentCraft, FDelegateHandle> _openCraftWithDelegate;

	TArray<TPair<MessageContentCraft, FDelegateHandle>> _craftDelegates;

	TArray<TPair<MessageUserInfo, FDelegateHandle>> _userInfoDelegates;

	TArray<TPair<MessageItem, FDelegateHandle>> _itemActionDelegates;

private:
	FGsGameStateCraft();

public:
	void Init() final;

	void Close() final;

	void Enter() final;

	void Exit() final;

	bool CanEnter(bool InShowUnableMessage = false) const;

public:
	bool HasHistory() const;

	void PushHistory(UGsUIWindowCraft::Parameters InHistory);

	bool PopHistory(OUT UGsUIWindowCraft::Parameters& OutHistory);

	void ClearHistory();

private:
	void RegisterMessages();

	void UnregisterMessages();

private:
	void CurrencyChanged(const CurrencyType InCurrencyType);

	void ItemChanged(const ItemId InItemId);

private:
	/**
	 * 일반적으로 state 가 enter 되면 동시에 제작창이 오픈되는데, 오픈될 때 파라미터를 전달하여 초기화를 분기 처리해야 하는 경우가 있음.
	 * state enter 시에는 매개변수를 전달할 수 없으므로, 본 딜리게이트를 통해 초기화 매개변수를 미리 설정한 후 state enter 하여 초기화를 올바르게 진행하도록 함.
	 * 이 딜레게이트는 다른 딜레게이트와 다르게 state가 아니라도 동작해야 하므로, Init()/Close() 에서 각각 딜리게이트 할당/해제를 수행함.
	 */
	void OpenCraftWith(const IGsMessageParam* InParam);

	void OnChangeCraftTarget(const IGsMessageParam* InParam);

	void OnChangeFavoriteList(const IGsMessageParam*);

	void OnEndSpelling(const IGsMessageParam* InParam);
	
	void OnCraftResult(const IGsMessageParam* InParam);

	void OnUser_Currency(uint64 InCurrncyType);

	void OnAddItemList(FGsItemMessageParamBase& InParam);

	void OnUpdateItem(FGsItemMessageParamBase& InParam);

	void OnRemoveItem(FGsItemMessageParamBase& InParam);

};