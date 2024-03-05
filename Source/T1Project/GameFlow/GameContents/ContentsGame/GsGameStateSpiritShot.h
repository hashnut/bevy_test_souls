#pragma once


#include "GsContentsGameBase.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageUserInfo.h"
#include "UI/UIContent/Window/GsUIWindowSpiritShot.h"

struct IGsMessageParam;

class FGsGameStateSpiritShot final : public FGsContentsGameBase
{
	/** 생성자가 private 이며, 오직 FGsContentsGameAllocator 만 접근 가능 */
	friend class FGsContentsGameAllocator;

private:
	using Super = FGsContentsGameBase;

private:
	/** State 는 Window 를 참조하고 관리해주는 컨트롤러(중개자) 역할을 수행한다 */
	TWeakObjectPtr<UGsUIWindowSpiritShot> _window;

	/** 뒤로 가기 등에 활용하는 히스토리 스택 */
	TArray<UGsUIWindowSpiritShot::Parameters> _historyStack;

private:
	/** 천마석 시스템 윈도우 오픈시 사용 */
	TPair<MessageContentSpiritShot, FDelegateHandle> _openDelegate;

	/** 천마석 시스템 패킷 등의 이벤트에 대응 */
	TArray<TPair<MessageContentSpiritShot, FDelegateHandle>> _spiritShotDelegates;

	/** 유저 정보 변경에 대응 */
	TArray<TPair<MessageUserInfo, FDelegateHandle>> _userInfoDelegates;

	/** 아이템 갯수 변화에 대응 */
	TArray<TPair<MessageItem, FDelegateHandle>> _itemActionDelegates;

	/** 컨텐츠 언락에 대응 */
	TPair<MessageContentUnlock, FDelegateHandle> _unlockDelegate;

private:
	FGsGameStateSpiritShot();

public:
	void Init() final;
	void Close() final;
	void Enter() final;
	void Exit() final;
	bool CanEnter(bool InShowUnableMessage = false) const;

private:
	bool HasHistory() const;
	void PushHistory(UGsUIWindowSpiritShot::Parameters InParameters);
	bool PopHistory(OUT UGsUIWindowSpiritShot::Parameters& OutParameters);
	void ClearHistory();

private:
	void RegisterMessages();
	void UnregisterMessages();

	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
private:
	void CurrencyChanged(const CurrencyType InCurrencyType);
	void ItemChanged(const ItemId InItemId);

	/************************************************************************/
	/* Event                                                                */
	/************************************************************************/
private:
	/** 천마석 윈도우 오픈 이벤트. 윈도우를 열 때 필요한 파라미터 값을 전달한다 */
	void Open(const IGsMessageParam* InParam);

	/** 천마석 정보 전달 */
	void OnSpiritShotInfo(const IGsMessageParam* InParam);

	/** 천마석 강화 성공/실패 여부 전달 */
	void OnSpiritShotAckEnchant(const IGsMessageParam* InParam);

	/** 천마석 연출 완료 시 전달 */
	void OnSpiritShotEnchantEffectFinished(const IGsMessageParam* InParam);

	/** 재화 수치 변경 전달 */
	void OnUserCurrency(uint64 InCurrncyType);

	/** 아이템 */
	void OnAddItemList(FGsItemMessageParamBase& InParam);

	void OnUpdateItem(FGsItemMessageParamBase& InParam);

	void OnRemoveItem(FGsItemMessageParamBase& InParam);

	/** 컨텐츠 언락 */
	void OnContentsUnlock(const IGsMessageParam* InParam);
};