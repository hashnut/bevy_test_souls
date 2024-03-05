// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Territory editor
#include "GsTerritoryZoneShape.h"
#include "Define/GsTerritoryZoneShapeDefine.h"

#include "GsTerritoryZoneLineShape.generated.h"

const FString LINE_TEXT = TEXT("line");

///----------------------------------------------------------------
// Scene 내 이벤트에 필요한 영역과 기능을 관리해주는 에디터용 액터
//----------------------------------------------------------------
UCLASS(Blueprintable)
class TERRITORYEDITOR_API AGsTerritoryZoneLineShape : public AGsTerritoryZoneShape
{
	GENERATED_BODY()
	
public:
	AGsTerritoryZoneLineShape();
	UFUNCTION(BlueprintCallable, Category = "Game service eidtor terrain line")
	virtual void InitZoneShape() override;
	UFUNCTION(BlueprintCallable, Category = "Game service eidtor terrain line")
	virtual void DrawZoneShape(const FTransform& in_transform) override;

public://get set
	virtual FString GetZoneShapeTypeString() const override { return LINE_TEXT; }
	virtual EGsTerritoryZoneShapeType GetZoneShapeType() const override { return EGsTerritoryZoneShapeType::Line; }
	virtual bool IsInvalidePointNum(int32 in_num) const override;
	virtual void GetDefaultPoints(TArray<FVector>& out_pointArray) const override;
	virtual bool IsClosedShape() const override { return false; }
};
