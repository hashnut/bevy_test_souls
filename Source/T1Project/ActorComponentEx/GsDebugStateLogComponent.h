#pragma once


#include "Components/ActorComponent.h"
#include "GsDebugStateLogComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsDebugStateLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectDebug, Meta = (Tooltip = "GameObject 디버그 출력로그 갯수"))
	int _displayLogCount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameObjectDebug, Meta = (Tooltip = "GameObject 디버그 출력로그"))
	TArray<FString> _listLog;

	void AddLog(const FString& log);
};