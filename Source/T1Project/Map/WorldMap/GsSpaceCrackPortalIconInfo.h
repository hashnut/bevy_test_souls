#pragma once
#include "CoreMinimal.h"

#include "GsSpaceCrackPortalIconInfo.generated.h"

// 테스트 데이터
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsTestSpaceCrackPortalInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpaceCrack, Meta = (Tooltip = "id"))
	int _id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpaceCrack, Meta = (Tooltip = "맵 id"))
	int _mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpaceCrack, Meta = (Tooltip = "채널 id"))
	int _channelId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpaceCrack, Meta = (Tooltip = "위치 id"))
	FVector _pos;

public:
	const FVector& GetPos() const
	{
		return _pos;
	}
};