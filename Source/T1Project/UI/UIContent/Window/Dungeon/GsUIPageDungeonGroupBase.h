#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Dungeon/GsDungeonUICaptureData.h"
#include "../Dungeon/GsDungeonCommonData.h"
#include "GsUIPageDungeonGroupBase.generated.h"



UCLASS()
class T1PROJECT_API UGsUIPageDungeonGroupBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	const class FGsDungeonGroupBase* _data = nullptr;

public:	
	virtual void Enter(const class FGsDungeonGroupBase* InData, const FGsDungeonUICaptureData& InCaptureData);
	virtual void Leave() {}	
	virtual void Capture(FGsDungeonUICaptureData* OutCaptherData) const {/**/ }
	
	virtual void Invalidate() {}
	virtual void InvalidatePartyInfoSet(const TArray<FGsPartyDungeonPartyStatusList>* InPartyStatusList) {/**/ }

	const class FGsDungeonGroupBase* GetData() const;
};