#pragma once

#include "Runtime/Core/Public/Containers/Map.h"
#include "Runtime/Core/Public/Delegates/IDelegateInstance.h"

#include "GsEventActionBase.h"
#include "Message/GsMessageContents.h"

#include "GsEventActionTutorial.generated.h"

struct FGsSchemaBase;
struct IGsMessageParam;

/**
 * 튜토리얼 발동 액션
 */
UCLASS()
class UGsEventActionTutorial final : public UGsEventActionBase
{
	GENERATED_BODY()

private:
	int32 _tutorialId = 0;
	TPair<MessageContentTutorial, FDelegateHandle> _msgHandleTutorial;

public:
	void BeginDestroy() final;

public:	
	void Initialize(const FGsSchemaBase* EventActionData) final;

protected:
	bool OnAfterPlay() final;
	void OnFinish() final;

private:
	void AddObserver();
	void RemoveObserver();
	void OnCompleteTutorial(const IGsMessageParam* InParam);
};
