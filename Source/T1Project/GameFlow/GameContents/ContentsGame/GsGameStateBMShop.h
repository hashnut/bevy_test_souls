#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"

struct IGsMessageParam;
/**
 * BMShop State
 */
class FGsGameStateBMShop : public FGsContentsGameBase
{
public:
	TArray<TPair<MessageContentStoreBM, FDelegateHandle>> _bmShopMsgHandlerList;

public:
	FGsGameStateBMShop();
	virtual ~FGsGameStateBMShop();

public:
	virtual void Enter() override;
	virtual void Exit() override;

private:
	void RegisterMessages();
	void RemoveMessage();

private:
	void OpenBMShop(const struct IGsMessageParam* InMessageParam);
	void CloseBMShop(const struct IGsMessageParam* InMessageParam);

public:
	void InvalidateServerMigrate(struct FGsSharedParam& InMessageParam);
};
