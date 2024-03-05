#pragma once

#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "UI/UIContent/Window/GsUIWindowMultiLevelRank.h"

class FDelegateHandle;
struct IGsMessageParam;

class T1PROJECT_API FGsGameStateMultiLevelRank final : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<UGsUIWindowMultiLevelRank> _window;

	TArray<TPair<MessageContentMultiLevelRank, FDelegateHandle>> _messageUiDelegates;	

public:
	FGsGameStateMultiLevelRank();

public:
	void Enter() final;
	void Exit() final;

protected:
	void RegisterMessages();
	void UnregisterMessages();

protected:
	void OnInvalidateSlotAll(const IGsMessageParam* InParam);
	void OnInvalidateQuest(const IGsMessageParam* InParam);
	void OnInitializeSlotAll(const IGsMessageParam* InParam);
	void OnChangeFocusedSlot(const IGsMessageParam* InParam);
};