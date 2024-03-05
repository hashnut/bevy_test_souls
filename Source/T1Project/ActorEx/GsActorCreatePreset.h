#pragma once
#include "CoreMinimal.h"

#include "ActorEx/GsCharacterPlayer.h"
#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "SharedEnums/SharedCreatureEnum.h"
#include "GsActorCreatePreset.generated.h"

/*
	로비 캐릭터 생성 창 프리셋선택
	커스터마이징 쪽 때문에 character player를 상속받아야함
*/

class FGsCustomizeHandler;
class FGsGameObjectDataLocalPlayer;
struct FGsSchemaCustomizePreset;
struct FGsSchemaCharacterCreatePresetData;

UCLASS()
class T1PROJECT_API AGsActorCreatePreset : public AGsCharacterPlayer
{
	GENERATED_BODY()

private:	
	// 데이터 인덱스
	int _dataIndex = -1;
	uint32 _selectCostumeId = 0;

	FGsCustomizeUIData _customizeUIData;

	TSharedPtr<FGsCustomizeUIData> _uiCustomizeData = nullptr;

	FVector _vecOriginSkeletalLoc = FVector::ZeroVector;
	
protected:
	// 커스터마이징 핸들러
	TSharedPtr<FGsCustomizeHandler> _customizeHandler{ nullptr };

	// 생성자
public:
	AGsActorCreatePreset();

	// 가상 함수
public:
	virtual void BeginPlay() override;

public:
	void SetCustomize(const FGsSchemaCharacterCreatePresetData* characterCreatePresetData, int inIndex);
	FVector& GetOriginSkeletalLoc() { return _vecOriginSkeletalLoc; }

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectCH();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUnselect();
};