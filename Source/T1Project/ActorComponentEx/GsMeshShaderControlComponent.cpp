// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMeshShaderControlComponent.h"
#include "Runtime/Engine/Classes/Components/MeshComponent.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"
#include "T1Project.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "Data/GsDataContainManager.h"

// Sets default values for this component's properties
UGsMeshShaderControlComponent::UGsMeshShaderControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGsMeshShaderControlComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
}

// Called every frame
void UGsMeshShaderControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateShaderParameter(DeltaTime);
}

void UGsMeshShaderControlComponent::EndPlay(EEndPlayReason::Type inType)
{
	Stop();

	_meshArray.Empty();
	_defaultCurveData.Empty();
	_mapShaderEffect.Empty();
	_mapEndCallback.Empty();
	_listRemoveType.Empty();

	Super::EndPlay(inType);
}

void UGsMeshShaderControlComponent::Initialize(const FGsSchemaCreatureCommonInfo* inCommonInfo)
{
	if (AActor* owner = GetOwner())
	{
		if (owner->GetRootComponent())
		{
			TArray<USceneComponent*> componentsArray;

			owner->GetRootComponent()->GetChildrenComponents(true, componentsArray);
			_meshArray.Empty();

			for (USceneComponent* comp : componentsArray)
			{
				if (comp)
				{
					if (comp->IsA(UMeshComponent::StaticClass()))
					{
						_meshArray.Add(Cast<UMeshComponent>(comp));
					}
				}
			}
		}
	}

	_defaultCurveData.Empty();

	if (nullptr != inCommonInfo)
	{
		for (const TPair<EGsMeshShaderEffect, UCurveFloat*>& pair : inCommonInfo->meshShader)
		{
			_defaultCurveData.Emplace(pair.Key, pair.Value);
		}
	}
}

void UGsMeshShaderControlComponent::SetScalarParameterValueOnMaterials(UMeshComponent* MeshComponent,
	const FName& ParameterName, const float ParameterValue)
{
	if (nullptr == MeshComponent)
	{
		return;
	}

	MeshComponent->SetScalarParameterValueOnMaterials(ParameterName, ParameterValue);
	// Overlay Material 
	if (UMaterialInterface* overlayMat = MeshComponent->GetOverlayMaterial())
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(overlayMat);
		if (nullptr == DynamicMaterial)
		{
			// Create and set the dynamic material instance.
			DynamicMaterial = UMaterialInstanceDynamic::Create(overlayMat, MeshComponent);
			MeshComponent->SetOverlayMaterial(DynamicMaterial);
		}

		if (DynamicMaterial)
		{
			DynamicMaterial->SetScalarParameterValue(ParameterName, ParameterValue);
		}
		else
		{
			GSLOG(Error, TEXT("DynamicMaterial Create failed! %s"), *overlayMat->GetPathName())
		}
	}
}

void UGsMeshShaderControlComponent::SetVectorParameterValueOnMaterials(UMeshComponent* MeshComponent, const FName& ParameterName, const FLinearColor& ParameterValue)
{
	if (nullptr == MeshComponent)
	{
		return;
	}

	MeshComponent->SetVectorParameterValueOnMaterials(ParameterName, FVector(ParameterValue));
	// Overlay Material 
	if (UMaterialInterface* overlayMat = MeshComponent->GetOverlayMaterial())
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(overlayMat);
		if (nullptr == DynamicMaterial)
		{
			// Create and set the dynamic material instance.
			DynamicMaterial = UMaterialInstanceDynamic::Create(overlayMat, MeshComponent);
			MeshComponent->SetOverlayMaterial(DynamicMaterial);
		}

		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue(ParameterName, ParameterValue);
		}
		else
		{
			GSLOG(Error, TEXT("DynamicMaterial Create failed! %s"), *overlayMat->GetPathName())
		}
	}
}

void UGsMeshShaderControlComponent::UpdateShaderParameter(float DeltaTime)
{
	for (TPair<EGsMeshShaderEffect, FMeshShaderData>& el : _mapShaderEffect)
	{
		FMeshShaderData& val = el.Value;
		// 종료 체크
		if (val.IsEnd() || val._isStop)
		{
			_listRemoveType.Emplace(el.Key);
		}
		else 
		{
			if (const UCurveFloat* curveData = val._curve)
			{
				float parameterValue = curveData->GetFloatValue(val._deltaTime);
				for (UMeshComponent* mesh : val._listMeshComponent)
				{
					SetScalarParameterValueOnMaterials(mesh, val._parameterName, parameterValue);
				}
			}
			else
			{
				// 선형 처리(필요할때 구현)
			}

			// 시간 갱신
			val.UpdateDeltaTime(DeltaTime);
		}
	}

	if (_listRemoveType.Num() > 0)
	{
		for (EGsMeshShaderEffect type : _listRemoveType)
		{
			Remove(type);
		}
		_listRemoveType.Empty();
	}
}

void UGsMeshShaderControlComponent::Add(EGsMeshShaderEffect Type, bool Restore)
{
	if (UCurveFloat** findData = _defaultCurveData.Find(Type))
	{
		Add(Type, *findData, Restore);
	}
}

void UGsMeshShaderControlComponent::Add(EGsMeshShaderEffect Type, const UCurveFloat* CurveData, bool Restore)
{
	if (nullptr != CurveData)
	{
		for (UMeshComponent* mesh : _meshArray)
		{
			AddMesh(Type, mesh, CurveData, Restore);
		}
	}
	else
	{
		Add(Type, Restore);
	}
}

void UGsMeshShaderControlComponent::Add(EGsMeshShaderEffect Type, UMeshComponent* MeshComponent, const UCurveFloat* CurveData, 
	TFunction<void()> EndCallback, bool Restore)
{
	if (nullptr != MeshComponent && nullptr != CurveData)
	{
		AddMesh(Type, MeshComponent, CurveData, Restore);

		// 종료 콜백 등록
		if (_mapEndCallback.Contains(Type))
		{
			_mapEndCallback[Type] = EndCallback;
		}
		else
		{
			_mapEndCallback.Emplace(Type, EndCallback);
		}
	}
}

void UGsMeshShaderControlComponent::Remove(EGsMeshShaderEffect Type)
{
	if (FMeshShaderData* find = _mapShaderEffect.Find(Type))
	{
		if (find->_restore)
		{
			for (UMeshComponent* mesh : find->_listMeshComponent)
			{
				Restore(Type, mesh);
			}
		}

		find->_listMeshComponent.Empty();
	}

	_mapShaderEffect.Remove(Type);
	if (_mapShaderEffect.Num() == 0)
	{
		// Update 비활성
		SetComponentTickEnabled(false);
	}

	// 종료 콜백 실행
	if (TFunction<void()>* findFunc = _mapEndCallback.Find(Type))
	{
		if (nullptr != (*findFunc))
		{
			(*findFunc)();
		}
	}
}

void UGsMeshShaderControlComponent::Restore(EGsMeshShaderEffect Type, UMeshComponent* MeshComponent)
{
	SetScalarParameterValueOnMaterials(MeshComponent, GetMaterialParameterName(Type),
		GetMaterialParameterMaxValue(Type));
}

void UGsMeshShaderControlComponent::Restore(EGsMeshShaderEffect Type)
{
	for (UMeshComponent* mat : _meshArray)
	{
		SetScalarParameterValueOnMaterials(mat, GetMaterialParameterName(Type),
			GetMaterialParameterMaxValue(Type));
	}
}

void UGsMeshShaderControlComponent::Start(bool Inverse)
{
	for (TPair<EGsMeshShaderEffect, FMeshShaderData>& el : _mapShaderEffect)
	{
		el.Value._inverse = Inverse;

		// delta 초기화는 함부로 진행 하면 안된다.
		// 실행중 inverse 처리가 들어올수 있다.
		if (el.Value.IsEnd())
		{
			el.Value.InitDeltaTime();
		}

		// 초기값 설정
		float initValue = (Inverse) ? el.Value._max : el.Value._min;
		for (UMeshComponent* mesh : el.Value._listMeshComponent)
		{
			SetScalarParameterValueOnMaterials(mesh, el.Value._parameterName, initValue);
		}
	}
	
	// Update 활성
	SetComponentTickEnabled(true);
}

void UGsMeshShaderControlComponent::Stop()
{
	if (_mapShaderEffect.Num() > 0)
	{
		for (TPair<EGsMeshShaderEffect, FMeshShaderData>& el : _mapShaderEffect)
		{
			for (UMeshComponent* mesh : el.Value._listMeshComponent)
			{
				Restore(el.Key, mesh);
			}
			el.Value._isStop = true;
		}

		// 콜백도 무효화 한다.
		_mapEndCallback.Empty();
	}

	if (IsBranch(EGsGameClientBranchType::MATERIAL_PARAM_RESTORE_PROCESS))
	{
		Restore(EGsMeshShaderEffect::Dissolve);
		Restore(EGsMeshShaderEffect::Fresnel);
	}

	_mapShaderEffect.Empty();
	SetComponentTickEnabled(false);
}

void UGsMeshShaderControlComponent::AddMesh(EGsMeshShaderEffect Type, UMeshComponent* MeshComponent,
	const UCurveFloat* CurveData, bool Restore)
{
	if (nullptr != MeshComponent)
	{
		FMeshShaderData* meshData = _mapShaderEffect.Find(Type);
		// 이미 진행중일때의 처리는 어떻게 할것인가?
		if (meshData)
		{
			meshData->_listMeshComponent.AddUnique(MeshComponent);

			// 새로운 커브 데이터 인가 확인
			if (CurveData != meshData->_curve)
			{
				meshData->_curve = CurveData;
			}

			// Restore 확인
			meshData->_restore = Restore;
		}
		else
		{
			FMeshShaderData newInfo;
			newInfo._parameterName = GetMaterialParameterName(Type);
			newInfo._listMeshComponent.Emplace(MeshComponent);
			newInfo._curve = CurveData;
			newInfo._min = GetMaterialParameterMinValue(Type);
			newInfo._max = GetMaterialParameterMaxValue(Type);
			newInfo._restore = Restore;
			newInfo._deltaTime = -1.f;
			meshData = &_mapShaderEffect.Emplace(Type, newInfo);
		}

		if (CurveData)
		{
			float min;
			CurveData->GetTimeRange(min, meshData->_endTime);
		}
	}
}

bool UGsMeshShaderControlComponent::IsStart(EGsMeshShaderEffect Type)
{
	return _mapShaderEffect.Find(Type) != nullptr;
}

float UGsMeshShaderControlComponent::GetMaterialParameterMinValue(EGsMeshShaderEffect inType)
{
	switch (inType)
	{
	case EGsMeshShaderEffect::Dissolve:
		return DISSOLVE_MIN_FACTOR;
		break;
	case EGsMeshShaderEffect::Fresnel:
		return FRESNEL_MIN_FACTOR;
		break;
	}

	return 0.f;
}

float UGsMeshShaderControlComponent::GetMaterialParameterMaxValue(EGsMeshShaderEffect inType)
{
	switch (inType)
	{
	case EGsMeshShaderEffect::Dissolve:
		return DISSOLVE_MAX_FACTOR;
		break;
	case EGsMeshShaderEffect::Fresnel:
		return FRESNEL_MAX_FACTOR;
		break;
	}

	return 1.f;
}

FName UGsMeshShaderControlComponent::GetMaterialParameterName(EGsMeshShaderEffect inType)
{
	switch (inType)
	{
	case EGsMeshShaderEffect::Dissolve:
		return DISSOLVE;
		break;
	case EGsMeshShaderEffect::Fresnel:
		return FRESNEL_EXP;
		break;
	}

	return FName();
}

FString UGsMeshShaderControlComponent::GetMaterialParameterTypeName(EGsMeshShaderEffect inType)
{
	switch (inType)
	{
	case EGsMeshShaderEffect::Dissolve:
		return TEXT("Dissolve");
	case EGsMeshShaderEffect::Fresnel:
		return TEXT("Fresnel");
	}

	return TEXT("");
}
