#pragma once

#include "CoreMinimal.h"
#include "DataSchema/TalkBalloon/GsSchemaTalkBalloonData.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "GsWidgetComponentTalkBalloon.generated.h"


struct FGsSchemaTalkBalloonData;
/**
* 위젯 컴퍼넌트 NPC 말풍선
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsWidgetComponentTalkBalloon : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Talk Balloon")
	struct FGsSchemaTalkBalloonDataRow _talkBalloonDataRow;

	const FGsSchemaTalkBalloonData* _talkBalloonData;

public:
	UGsWidgetComponentTalkBalloon();

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	bool IsAvailable();

	void SetData(const FGsSchemaTalkBalloonData* InTalkBalloonData);

	void SetIsEnabled(bool bInIsEnabled);
};