// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCameraPostProcessComponent.h"
#include "Curves/CurveFloat.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UTIL/GsTimeUtil.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

void UPostProcessCurve::Initialize()
{
	_timer = NewObject<UGsSimpleTimer>();

	if (_timer)
	{
		if (_curve)
		{
			float min;
			float max;

			_curve->GetTimeRange(min, max);
			_timer->SetMaxTime(max);			
		}
	}
}

void UPostProcessCurve::Tick(float inTime)
{	
	if (_timer)
	{
		if (_timer->CanTick())
		{
			if (_timer->IsFinish())
			{
				PostProcessCurveMessage.SendMessage(EGsPostProcessCurveEvent::FinishEvent, this);
			}			

			_timer->Tick(inTime);
			
			if (_curve)
			{
				_currentWeight = _curve->GetFloatValue(_timer->GetCurrentTime());
				PostProcessCurveMessage.SendMessage(EGsPostProcessCurveEvent::TickEvent, this);
			}
		}		
	}
}

float UPostProcessCurve::GetCurrentWeight() const
{
	return _currentWeight;
}

bool UGsCameraPostProcessComponent::_activateMotionBlurMode = false;

// Sets default values for this component's properties
UGsCameraPostProcessComponent::UGsCameraPostProcessComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGsCameraPostProcessComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UGsCameraPostProcessComponent::EndPlay(EEndPlayReason::Type inType)
{
	for (TPair<EGsPlayerPostProcess, UPostProcessCurve*>& postProcess : _postProcessMap)
	{
		if (postProcess.Value)
		{
			postProcess.Value->PostProcessCurveMessage.RemoveAll();
		}		
	}

	_postProcessMap.Empty();

	if (nullptr != _motionBlurTimer)
	{
		_motionBlurTimer->ConditionalBeginDestroy();
	}

	Super::EndPlay(inType);
}

void UGsCameraPostProcessComponent::Initialize(UCameraComponent* inCamera)
{
	_camera = inCamera;

	if (_camera)
	{
		int32 origin = _camera->PostProcessSettings.WeightedBlendables.Array.Num();

		if (GData())
		{
			if (GData()->GetGlobalData())
			{
				TArray< EGsPlayerPostProcess> activeCameraPostProcessType;
				const TArray<FPostProcessMaterialData>& materials = GData()->GetGlobalData()->_playerCameraPostProcessMaterials;

				for (int i = 0; i< materials.Num(); ++i)
				{
				#if PLATFORM_ANDROID
					if(false == materials[i]._support_Platform_Android)
						continue;
				#elif PLATFORM_IOS
					if (false == materials[i]._support_Platform_IOS)
						continue;
				#elif PLATFORM_WINDOWS
					if (false == materials[i]._support_Platform_PC)
						continue;
				#endif
					if (nullptr != materials[i]._materialInstance)
					{
						UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(materials[i]._materialInstance, nullptr);
						_camera->PostProcessSettings.AddBlendable(dynamicMaterial, materials[i]._initialBlendWeight);

						for (auto itr : materials[i]._usageType)
						{
							activeCameraPostProcessType.AddUnique(itr);
						}
					}
				}

				const TMap<EGsPlayerPostProcess, FPostProcessCurveData>& postProcessMap = GData()->GetGlobalData()->_PlayerCameraPostProcessMap;

				int indexType = 0;
				for (const TPair<EGsPlayerPostProcess, FPostProcessCurveData>& postProcess : postProcessMap)
				{
					if(false == activeCameraPostProcessType.Find(postProcess.Key, indexType))
						continue;

					if (UPostProcessCurve* temp = NewObject<UPostProcessCurve>())
					{
						temp->Set(postProcess.Value);
						temp->Initialize();
						temp->SetRealMaterialIndex(postProcess.Value._materialIndex + origin);

						_postProcessMap.Add(postProcess.Key, temp);
					}					
				}
			}

			bool SupportMotionBlur = false;
#if PLATFORM_ANDROID
			SupportMotionBlur = GData()->GetGlobalData()->_motionBlur._support_Platform_Android;
#elif PLATFORM_IOS
			SupportMotionBlur = GData()->GetGlobalData()->_motionBlur._support_Platform_IOS;
#elif PLATFORM_WINDOWS
			SupportMotionBlur = GData()->GetGlobalData()->_motionBlur._support_Platform_PC;
#endif
			if (SupportMotionBlur)
			{
				_motionBlurTimer = NewObject<UGsCurveTimer>();
				_motionBlurTimer->SetCurve(GData()->GetGlobalData()->_motionBlur._motionBlurTime);
				_motionBlurTimer->_finishDelegate.AddDynamic(this, &UGsCameraPostProcessComponent::FinishMotionBlur);
				_amoutMotionBlur = GData()->GetGlobalData()->_motionBlur._motionBlurAmount;
			}
		}
	}	
}

// Called every frame
void UGsCameraPostProcessComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	for (TPair<EGsPlayerPostProcess, UPostProcessCurve*>& postProcess : _postProcessMap)
	{				
		if (postProcess.Value)
		{
			postProcess.Value->Tick(DeltaTime);
		}		
	}

	check(_camera);
	if (nullptr != _motionBlurTimer)
	{
		float fMotionBlurAmout = _activateMotionBlurMode ? _amoutMotionBlur : 0.0f;
		if (true == _notifyMotionBlur)
		{
			_motionBlurTimer->Tick(DeltaTime);
			fMotionBlurAmout += _motionBlurTimer->GetCurrentValue();
		}

		if (fMotionBlurAmout != 0.0f)
		{
			_camera->PostProcessSettings.bOverride_MotionBlurAmount = true;
			_camera->PostProcessSettings.MotionBlurAmount = fMotionBlurAmout;
		}
		else
		{
			_camera->PostProcessSettings.bOverride_MotionBlurAmount = false;
		}
	}
}

bool UGsCameraPostProcessComponent::Start(EGsPlayerPostProcess inType)
{
	if (Contains(inType))
	{
		if (UPostProcessCurve* data = GetCurveData(inType))
		{
			if (data->GetTimer())
			{
				data->GetTimer()->Reset();
				data->GetTimer()->Start();

				return true;
			}
		}		
	}	

	return false;
}

bool UGsCameraPostProcessComponent::Reset(EGsPlayerPostProcess inType)
{
	if (Contains(inType))
	{
		if (UPostProcessCurve* data = GetCurveData(inType))
		{
			if (data->GetTimer())
			{
				data->GetTimer()->Reset();

				return true;
			}
		}		
	}

	return false;
}

bool UGsCameraPostProcessComponent::Stop(EGsPlayerPostProcess inType)
{
	if (Contains(inType))
	{
		if (UPostProcessCurve* data = GetCurveData(inType))
		{
			if (data->GetTimer())
			{
				data->GetTimer()->Stop();

				return true;
			}
		}		
	}

	return false;
}

void UGsCameraPostProcessComponent::FinishMotionBlur(const UObject* timer)
{
	_notifyMotionBlur = false;
	if (nullptr != _motionBlurTimer)
	{
		// 디폴트 커브로 변경
		_motionBlurTimer->SetCurve(GData()->GetGlobalData()->_motionBlur._motionBlurTime);
		_motionBlurTimer->Stop();
	}
}

void UGsCameraPostProcessComponent::StartMotionBlurNotify(UCurveFloat* curveData, bool IgnorePreEvent /*= false*/)
{
#if UE_EDITOR
	if (nullptr == GData())
	{
		return;
	}
	if (nullptr == GData()->GetGlobalData())
	{
		return;
	}
#endif

	if (nullptr == _motionBlurTimer)
	{
		return;
	}

	if ((true == _notifyMotionBlur) && (false == IgnorePreEvent))
	{
		return;
	}

	UCurveFloat* curve = (nullptr != curveData) ? curveData : GData()->GetGlobalData()->_motionBlur._motionBlurTime;
	check(curve);

	_motionBlurTimer->SetCurve(curve);

	_notifyMotionBlur = true;
	_motionBlurTimer->Reset();
	_motionBlurTimer->SetTick(true);
}

void UGsCameraPostProcessComponent::SetMotionBlurMode(bool inActive)
{
	_activateMotionBlurMode = inActive;
}

UGsSimpleTimer* UGsCameraPostProcessComponent::GetTimer(EGsPlayerPostProcess inType) const
{
	if (Contains(inType))
	{
		if (UPostProcessCurve* data = GetCurveData(inType))
		{
			return data->GetTimer();
		}		
	}

	return nullptr;
}

UPostProcessCurve* UGsCameraPostProcessComponent::GetCurveData(EGsPlayerPostProcess inType) const
{
	if (Contains(inType))
	{
		return _postProcessMap[inType];
	}	

	return nullptr;
}

bool UGsCameraPostProcessComponent::Contains(EGsPlayerPostProcess inType) const
{	
	return _postProcessMap.Contains(inType);
}