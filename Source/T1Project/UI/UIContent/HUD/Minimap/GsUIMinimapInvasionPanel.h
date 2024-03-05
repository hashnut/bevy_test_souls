#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageInvasion.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMinimapInvasionPanel.generated.h"

UCLASS()
class T1PROJECT_API UGsUIMinimapInvasionPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _invasionServerTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _userCountTextBlock;

private:
	int32 _maxUserCount;
	MapId _currentMapId;
	MsgGameObjHandleArray _gameObjectMsg;
	MsgInvasionHandleArray _invasionMsg;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:	
	void InvalidUserCount();
	void InvalidInvasionServerName();

private:
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);
	void OnInvalidUserCount(const struct IGsMessageParam*);
};