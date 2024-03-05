#pragma once

#include "NoExportTypes.h"
#include "../Message/GsMessageGameObject.h"
#include "Management/ScopeGame/GsEventManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "EventAction/GsEventActionCommonData.h"
#include "GsEventActionImmediate.generated.h"

/**
 * 이벤트액션을 순서대로 진행시키는 시퀀서
 */
UCLASS()
class UGsEventActionImmediate final : public UObject
{
	GENERATED_BODY()

	friend void UGsEventManager::Initialize();

protected:
	MsgGameObjHandleArray _gameObjectMsg;

	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);

private:
	void CombinText(const struct FGsSchemaMapEventActionDialog* InData, const TArray<EvenParameterValue>& InParamSet, FText& OutMessage);

	class UGsUIHUDEventActionMessageProgressBar* GetWidgetMessageProgressBar();

	class UGsUIHUDEventActionMessageProgressBarEX* GetWidgetMessageProgressBarEX();

	class UGsUIHUDEventActionMessageNotify* GetWidgetMessageNotify();

protected:
	void OnEventStateBoard(const struct FGsSchemaMapEventActionDialog* InData, bool InIsActive, const TArray<EvenParameterValue>& InParamSet);

	void OnEventProgressBar(const struct FGsSchemaMapEventActionDialog* InData, bool InIsActive, const TArray<EvenParameterValue>& InParamSet);

	void OnEventProgressBarEX(const struct FGsSchemaMapEventActionDialog* InData, bool InIsActive, const TArray<EvenParameterValue>& InParamSet);

	void OnEventMessageTicker(const struct FGsSchemaMapEventActionDialog* InData, const TArray<EvenParameterValue>& InParamSet);

	void OnEventPrologueTicker(const struct FGsSchemaMapEventActionDialog* InData);

	void OnEventBattleTicker(const struct FGsSchemaMapEventActionDialog* InData);

	void OnEventInvadeOneTextTicker(const struct FGsSchemaMapEventActionDialog* InData);

	void OnEventInvadeTwoTextTicker(const struct FGsSchemaMapEventActionDialog* InData);

	void OnEventMessageServerNameTicker(const FGsSchemaMapEventActionDialog* InData, const TArray<EvenParameterValue>& InParamSet);

	void OnEventMessageChatTicker(const FGsSchemaMapEventActionDialog* InData, const TArray<EvenParameterValue>& InParamSet);
public:
	void Initialize();

	void Finalize();	

public:
	void Play(EventSubDialogId InId, bool InIsActive, const TArray<EvenParameterValue>& InParamSet);

	void ClearAll();
};