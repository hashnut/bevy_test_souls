#pragma once

#include "GsEventActionBase.h"

#include "Message/GsMessageGameObject.h"

#include "GsEventActionDialog.generated.h"

struct FGsSchemaBase;
struct IGsMessageParam;

UCLASS()
class UGsEventActionDialog final : public UGsEventActionBase
{
	GENERATED_BODY()

private:
	int _eventActionNpcDialogId = 0;
	MsgGameObjHandle _dialogEndHandle;
public:
	void Initialize(const FGsSchemaBase* EventActionData) final;
	bool OnAfterPlay() final;
	void OnFinish() final;

	// event
public:
	void CloseNpcDialog(const IGsMessageParam* In_data);
	
private:
	void AddObserver();
	void RemoveObserver();
};
