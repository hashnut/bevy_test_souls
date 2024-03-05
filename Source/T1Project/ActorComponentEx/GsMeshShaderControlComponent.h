// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"
#include "GsMeshShaderControlComponent.generated.h"

const FName DISSOLVE = TEXT("Dissolve Intensity");
const FName FRESNEL_EXP = TEXT("Fresnel Exponent");
const float DISSOLVE_MIN_FACTOR = -1.f;
const float DISSOLVE_MAX_FACTOR = 2.0f; // Default
const float FRESNEL_MIN_FACTOR = 0.f;
const float FRESNEL_MAX_FACTOR = 10.f; // Default

class UCurveFloat;
class UMeshComponent;
struct FGsSchemaCreatureCommonInfo;
class UMeshComponent;

// 머터리얼 쉐이더 처리 정보 구조체
USTRUCT()
struct T1PROJECT_API FMeshShaderData
{
	GENERATED_BODY()
		
	UPROPERTY(Transient)
	TArray<UMeshComponent*> _listMeshComponent;		// 적용이 필요한 메시 
	UPROPERTY(Transient)
	const UCurveFloat* _curve = nullptr;			// 커브 데이터
	float _endTime = 0.1f;							// 종료 시간
	float _deltaTime = 0.f;							// 흐른 시간
	FName _parameterName;							// Parameter 이름
	float _min = 0.f;								// Parameter Min, Max,
	float _max = 0.f;
	bool _inverse = false;
	bool _restore = true;							// 완료후 복원 여부
	bool _isStop = false;							// 강제 정지 여부

	void InitDeltaTime()
	{
		_deltaTime = (_inverse) ? _endTime : 0.f;
	}
	void UpdateDeltaTime(float Delta)
	{
		_deltaTime = (_inverse) ? _deltaTime - Delta : _deltaTime + Delta;
	}
	bool IsEnd()
	{
		return _deltaTime > _endTime || _deltaTime < 0.f;
	}
	void Clear()
	{
		_listMeshComponent.Empty();
		_curve = nullptr;
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsMeshShaderControlComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// Mesh 정보 캐싱
	UPROPERTY(Transient)
	TArray<UMeshComponent*> _meshArray;

	// Default CurveData 저장
	UPROPERTY(Transient)
	TMap<EGsMeshShaderEffect, UCurveFloat*> _defaultCurveData;

	// 효과 적용메시 정보
	UPROPERTY(Transient)
	TMap<EGsMeshShaderEffect, FMeshShaderData> _mapShaderEffect;
	// 종료 처리 콜백 
	//FMeshShaderData 안으로 들어가게 되면 Remove시 애매한 상황들이 많다.
	TMap<EGsMeshShaderEffect, TFunction<void()>> _mapEndCallback;
	TArray<EGsMeshShaderEffect> _listRemoveType;


public:
	// Sets default values for this component's properties
	UGsMeshShaderControlComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(EEndPlayReason::Type inType);

public:
	void Initialize(const FGsSchemaCreatureCommonInfo* inCommonInfo = nullptr);

public:
	// 기본 Curve데이터로 설정
	void Add(EGsMeshShaderEffect Type, bool Restore = true);
	// Owner가 포함한 모든 Mesh
	void Add(EGsMeshShaderEffect Type, const UCurveFloat* CurveData, bool Restore = true);
	// 특정 Mesh
	void Add(EGsMeshShaderEffect Type, UMeshComponent* MeshComponent, const UCurveFloat* CurveData, 
		TFunction<void()> EndCallback = nullptr, bool Restore = true);
	// 미구현
	//void Add(EGsMeshShaderEffect Type, UMeshComponent* MeshComponent, float EndTime) {}
	void Remove(EGsMeshShaderEffect Type);

	// 복원
	void Restore(EGsMeshShaderEffect Type, UMeshComponent* MeshComponent);
	void Restore(EGsMeshShaderEffect Type);
	
public:
	void Start(bool Inverse = false);
	void Stop();

protected:
	void AddMesh(EGsMeshShaderEffect Type, UMeshComponent* MeshComponent, const UCurveFloat* CurveData, bool Restore);
	void UpdateShaderParameter(float DeltaTime);

public:
	// Overlay 머터리얼을 포함 시킨 걸로 재정의
	// UMeshComponent.bEnableMaterialParameterCaching(Protected) 사용시에대한 고려가 안되어 있으므로 추후 사용예정이면 구현 필요
	// @see : void UMeshComponent::SetScalarParameterValueOnMaterials(const FName ParameterName, const float ParameterValue)
	static void SetScalarParameterValueOnMaterials(UMeshComponent* MeshComponent, const FName& ParameterName, const float ParameterValue);
	static void SetVectorParameterValueOnMaterials(UMeshComponent* MeshComponent, const FName& ParameterName, const FLinearColor& ParameterValue);

public:
	bool IsStart(EGsMeshShaderEffect Type);
	void SetMeshList(const TArray<UMeshComponent*>& inMeshArray) { _meshArray = inMeshArray; }
	void ClearMeshList(){_meshArray.Empty();}

protected:
	float GetMaterialParameterMinValue(EGsMeshShaderEffect inType);
	float GetMaterialParameterMaxValue(EGsMeshShaderEffect inType);
	FName GetMaterialParameterName(EGsMeshShaderEffect inType);
	FString GetMaterialParameterTypeName(EGsMeshShaderEffect inType);	
};
