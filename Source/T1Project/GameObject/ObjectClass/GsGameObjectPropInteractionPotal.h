
#pragma once


#include "GsGameObjectPropTouch.h"
#include "GsGameObjectPropInteractionPotal.generated.h"

/**
* file		GsGameObjectPropInteractionPotal.h
* @brief	Prop 기능 중 Potal기능을 가진 Object(ex : 정예 던전 랜덤 포탈)
* @author	PKT
* @date		2022/08/08
**/

UCLASS()
class T1PROJECT_API UGsGameObjectPropInteractionPotal : public UGsGameObjectPropTouch
{
	GENERATED_BODY()

protected:
	const struct FGsSchemaMapData* _destinationData = nullptr;

protected:
	virtual void InitializeActor(class AActor* InOwner) override;

	virtual void OnClickInteract(bool In_isByWidget) override;

	void OnStartPropInteraction(const struct IGsMessageParam* inMessage);
	void OnEndPropInteraction(const struct IGsMessageParam* inMessage);
	
protected:
	void SetPotalHUDName();

public:
	const struct FGsSchemaMapData* GetDestinationMapData() const;

	void SetDestinationMapData(const struct FGsSchemaMapData* InDestinationData); 

	int32 GetUseCount() const;

	virtual void SendInteractionEnd(const struct FGsPropInteractionMessageParamContents* inParam) override;

	virtual void UpdatePropState() override;
};