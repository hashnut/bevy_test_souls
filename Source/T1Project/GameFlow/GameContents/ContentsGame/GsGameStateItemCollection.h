#pragma once

#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUI.h"

class UGsUIWindowItemCollection;
class FDelegateHandle;
struct IGsMessageParam;

class T1PROJECT_API FGsGameStateItemCollection final : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<UGsUIWindowItemCollection> _window;

private:
	TArray<TPair<MessageContentItemCollection, FDelegateHandle>> _messageUiDelegates;

	TPair<MessageContentMaterial, FDelegateHandle> _messageMaterialDelegate;

	TPair<MessageContentUnlock, FDelegateHandle> _unlockDelegate;

public:
	FGsGameStateItemCollection();

public:
	void Enter() final;
	void Exit() final;

protected:
	void RegisterMessages();
	void UnregisterMessages();

protected:
	void OnInvalidate(const IGsMessageParam* InParam);
	void OnSearchMaterial(const IGsMessageParam* InParam);
	void OnOpenByParam(const IGsMessageParam* InParam);
	void OnMaterialSetUIExit(const IGsMessageParam* InParam);
	void OnContentsUnlock(const IGsMessageParam* InParam);
};