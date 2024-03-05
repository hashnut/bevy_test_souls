#pragma once
#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconBase.h"

#include "GsRegionMapIconInvasionNPC.generated.h"

// region icon controller: invasion npc(guardian, monster, wagon)

class FGsRegionInvadeIconInfo;


UCLASS()
class T1PROJECT_API UGsRegionMapIconInvasionNPC : public UGsRegionMapIconBase
{
	GENERATED_BODY()

private:
	// invasion npc info
	FGsRegionInvadeIconInfo* _iconInfo;

	// virtual func
public:
	virtual void Tick(float In_delta) override;
	virtual void UpdateTimmer() override;

	// virtual get
public:
	virtual FVector2D GetPos() override;

	// get, set
public:
	void SetInvasionNPCInfo(FGsRegionInvadeIconInfo* In_info)
	{
		_iconInfo = In_info;
	}

};