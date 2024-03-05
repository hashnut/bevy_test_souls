#pragma once

#include "GsContentsGameBase.h"
#include "UI/UIContent/Window/GsUIWindowOfflinePlay.h"
#include "../../../Message/GsMessageContents.h"

class FDelegateHandle;


class T1PROJECT_API FGsGameStateOfflinePlay final : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<UGsUIWindowOfflinePlay> _window;
	TArray<TPair< MessageContentOfflinePlay, FDelegateHandle>> _listMessageDelegate;

public:
	FGsGameStateOfflinePlay();

public:
	void Enter() final;
	void Exit() final;

protected:
	void RegisterMessages();
	void UnregisterMessages();

public:
	void OnUpdateTime(const IGsMessageParam* InParam);	
};