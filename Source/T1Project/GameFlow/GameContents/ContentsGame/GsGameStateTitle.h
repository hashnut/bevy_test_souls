#pragma once

/**
* file		GsGameStateTitle.h
* @brief	Game State Title
* @author	PKT
* @date		2022/01/19
**/

#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageInvasion.h"
#include "UI/UIContent/Window/GsUIWindowTitleSystem.h"


class FDelegateHandle;
struct IGsMessageParam;
struct FGsPrimitiveInt32;

class T1PROJECT_API FGsGameStateTitle final : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<UGsUIWindowTitleSystem> _window;

	TArray<TPair<MessageContentTitle, FDelegateHandle>> _messageUiDelegates;

	MsgInvasionHandleArray _listInvadeDelegates;

public:
	FGsGameStateTitle();

	void Enter() final;
	void Exit() final;

protected:
	void RegisterMessages();
	void UnregisterMessages();

protected:
	void UpdateTitle(const IGsMessageParam* InParam);
	void UpdateCategoryRedDot(const IGsMessageParam* InParam);
	void UpdateEquip(const IGsMessageParam* InParam);
	void UpdateCollection(const IGsMessageParam* InParam);

	void UpdateInvadeBattleLevel(const IGsMessageParam* InParam);
};