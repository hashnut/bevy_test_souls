#pragma once

#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageItem.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "../UI/UIContent/Window/GsUIWindowMonsterCollection.h"


class FDelegateHandle;
struct IGsMessageParam;

class T1PROJECT_API FGsGameStateMonsterCollection final : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<UGsUIWindowMonsterCollection> _window;

private:
	TArray<TPair<MessageContentMonsterKnowledge, FDelegateHandle>> _messageUiDelegates;
	TArray<TPair<MessageUserInfo, FDelegateHandle>> _userInfoHandlerList;

	/** 아이템 갯수 변화에 대응 */
	TArray<TPair<MessageItem, FDelegateHandle>> _itemActionDelegates;

	/** 플레이어 스폰 완료 시, 컬렉션 데이터 업데이트 */
	MsgGameObjHandle _gameObjectDelegate;

public:
	FGsGameStateMonsterCollection();
	~FGsGameStateMonsterCollection();

private:
	void ItemChanged(const ItemId InItemId);

public:
	void Enter() final;
	void Exit() final;

protected:
	void RegisterMessages();
	void UnregisterMessages();

protected:
	void OnInvalidateBook(const IGsMessageParam* InParam);
	void OnInvalidateBookSet(const IGsMessageParam* InParam);
	void OnInvalidateSlate(const IGsMessageParam* InParam);

	void OnInvalidateCodex(const IGsMessageParam* InParam);
	void OnInvalidateKnowledgeConvert(const IGsMessageParam* InParam);
	void OnInvalidateCategoryLevelUp(const IGsMessageParam* InParam);
	void OnInvalidateKnowledgeAmount(const IGsMessageParam* InParam);

	void OnGoSlate(const IGsMessageParam* InParam);

	void OnUpdateCurrency(const IGsMessageParam* InParam);

	/** 아이템 */
	void OnAddItemList(FGsItemMessageParamBase& InParam);

	void OnUpdateItem(FGsItemMessageParamBase& InParam);

	void OnRemoveItem(FGsItemMessageParamBase& InParam);

	/** 컬렉션 */
	void OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam);
};