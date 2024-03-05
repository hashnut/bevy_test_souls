// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryActor.h"

//UE4
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/PropertyEditor/Public/DetailCategoryBuilder.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

//T1 Project
#include "T1Project/UTIL/GsText.h"
#include "ActorcomponentEx/WidgetComponentEx.h"

//Territory editor
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"
#include "GsTerritoryEditorSetting.h"
#include "Editor/Widget/GsEditorWidget.h"

#include "Editor/TerritoryEditor/TerritoryEditor.h"
#include "Editor/TerritoryEditor/Component/GsTerrEdWidgetComponent.h"
#include "T1Project/UTIL/GsUIUtil.h"

AGsTerritoryActor::AGsTerritoryActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

	ConstructorHelpers::FClassFinder<UUserWidget> widgetTemplate(TEXT("/Game/TerritoryEditor/UI/TerritoryActorWidgetBP"));	
	_widgetComp = CreateDefaultSubobject<UGsTerrEdWidgetComponent>(TEXT("TerrEdWidgetComp"));
	if (_widgetComp)
	{		
		_widgetComp->SetWidgetClass(widgetTemplate.Class);
		_widgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_widgetComp->SetRelativeLocation(FVector(0, 0, _widgetHeight));
		_widgetComp->_enableFaceToCamera = true;
		_widgetComp->SetDrawSize(_widgetScreenSize);
	}

	PrimaryActorTick.bCanEverTick = false;
	SetActorTickInterval(0.2f);
	SetActorHiddenInGame(false);

	bIsEditorOnlyActor = true;
	bRunConstructionScriptOnDrag = false;

	_selectHandle = USelection::SelectObjectEvent.AddUObject(this, &AGsTerritoryActor::OnSelected);
}

void AGsTerritoryActor::OnConstruction(const FTransform& inTransform)
{
	Super::OnConstruction(inTransform);

	FVector location;
	bool canSpawnOnNavmesh = true;
	if (IsNeedToCheckOnNavmesh()
		&& FGsTerritoryLevelManager::_isLoadArtLevel)
	{
		canSpawnOnNavmesh = UGsTerritoryLevelUtil::IsSpawnActorSpawnOnNavmesh(this, location);
	}

	SetCanSpawnOnNavmesh(canSpawnOnNavmesh);

	//New create child actor's shape id not setted yet, only saved actor refresh shape	
	InvalidShape();	
	CreateWidget(inTransform);

	_isCanDelete = false;
	_onTerritoryActorConstruct.Broadcast(this);
}

void AGsTerritoryActor::BeginDestroy()
{
	_onTerritoryActorConstruct.Clear();
	_onTerrEdActorPostEditPropertyChange.Clear();

	if (_selectHandle.IsValid())
	{
		USelection::SelectObjectEvent.Remove(_selectHandle);
	}	

	Super::BeginDestroy();
}

void AGsTerritoryActor::Destroyed()
{
	_onTerritoryActorConstruct.Clear();

	if (_selectHandle.IsValid())
	{
		USelection::SelectObjectEvent.Remove(_selectHandle);
	}

	Super::Destroyed();
}

void AGsTerritoryActor::ReregisterComponent()
{	
	//When reregister component, actor spawn component like widget component and child actor component recalled
	InvalidWidgetPosition(GetActorLocation());
	InvalidWidgetText();
}

void AGsTerritoryActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AGsTerritoryActor::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AGsTerritoryActor::TickActor(float in_delta, ELevelTick in_type, FActorTickFunction& in_tickFuc)
{
	Super::TickActor(in_delta, in_type, in_tickFuc);

	if (in_type == ELevelTick::LEVELTICK_ViewportsOnly)
	{
		TickActorInEditor(in_delta);
	}
}

void AGsTerritoryActor::TickActorInEditor(float inDelta)
{
	
}

void AGsTerritoryActor::PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent)
{
	Super::PostEditChangeProperty(inPropertyChangedEvent);

	if (_onTerrEdActorPostEditPropertyChange.IsBound())
	{
		_onTerrEdActorPostEditPropertyChange.Broadcast(this);
		_onTerrEdActorPostEditPropertyChange.Clear();
	}
}

void AGsTerritoryActor::PostDuplicate(bool bDuplicateForPIE)
{
	Super::PostDuplicate(bDuplicateForPIE);
}

void AGsTerritoryActor::PostDuplicate(EDuplicateMode::Type inDuplicateMode)
{
	Super::PostDuplicate(inDuplicateMode);

	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(this);

	if (IsNeedToCopy())
	{
		Copy();
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryActor::PostDuplicate - label : %s"), *GetActorLabel());
}

bool AGsTerritoryActor::IsCreatedInNonServerLevel()
{
	if (HasAllFlags(RF_Transient))
	{
		return  false;
	}

	if (ULevel* level = GetLevel())
	{
		const FString& path = level->GetPathName();
		if (false == UGsTerritoryLevelUtil::IsServerLevel(path))
		{
			UE_LOG(LogTerritoryEditor, Error, TEXT("level is not for server level - level : %s \tname : %s"), *path, *GetActorLabel());

			return true;
		}
	}

	return false;
}

bool AGsTerritoryActor::RecoveryTerritoryUniqueID()
{
	if (_uniqueID == INDEX_NONE)
	{
		FString invalidReason;
		return InvalidTerritoryUniqueID(invalidReason);
	}

	return false;
}

bool AGsTerritoryActor::InvalidTerritoryUniqueID(FString& invalidReason)
{
	if (false == _isValid)
	{
		invalidReason = TEXT("Invalid state");

		return false;
	}

	EGsTerritoryObjType objType = GetTerritoryObjType();
	int64 uniqueID;
	int64 instanceID;
	if (TryGenerateTerritoryUniqueID(this, objType, uniqueID, instanceID))
	{
		SetTerritoryUniqueID(uniqueID, instanceID);
		return true;
	}

	invalidReason = TEXT("Can't generate unique id");

	return false;
}

bool AGsTerritoryActor::TryGenerateTerritoryUniqueID(AActor* inOwner, EGsTerritoryObjType inType, int64& outUniqueID, int64& outInstanceID)
{
	if (inOwner)
	{
		if (FTerritoryEditor* instance = FTerritoryEditor::GetInstance())
		{
			if (instance->GetUniqueIDManager()->TryGenerateUniqueID(inOwner->GetLevel(), inType, outUniqueID, outInstanceID))
			{
				//#if WITH_EDITOR
				//				FString log = TEXT("Set unique ID to") + FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryObjType"), inType);
				//
				//				GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, log);
				//#endif
				return true;
			}
		}
	}

	return false;
}

bool AGsTerritoryActor::UnregisterTerritoryUniqueID(AActor* inOwner, EGsTerritoryObjType inType, int64& inInstanceID)
{
	if (inOwner)
	{
		if (FTerritoryEditor* instance = FTerritoryEditor::GetInstance())
		{
			if (instance->GetUniqueIDManager()->Remove(inOwner->GetLevel(), inType, inInstanceID))
			{
				//#if WITH_EDITOR
				//				FString log = TEXT("Set unique ID to") + FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryObjType"), inType);
				//
				//				GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, log);
				//#endif
				return true;
			}
		}
	}

	return false;
}

FString AGsTerritoryActor::GetLevelName()
{
	if (GetLevel())
	{
		if (UObject* outer = GetLevel()->GetOuter())
		{
			return outer->GetFName().ToString();
		}
	}

	return "";
}

void AGsTerritoryActor::SetValid(bool isIsValid)
{
	_isValid = isIsValid;
}

void AGsTerritoryActor::PrintTerritoryUniqueID()
{
#if WITH_EDITOR
	const FString& log = TEXT("Set unique ID to") + ToString();
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Yellow, log);
#endif
}

bool AGsTerritoryActor::CheckEditableTerritoryUniqueID()
{
	if (false == HasAllFlags(RF_Transient))
	{
		int64 uniqueID = GetTerritoryUniqueID();
		if (uniqueID < 0)
		{
			return true;
		}
	}

	return false;
}

void AGsTerritoryActor::DestroyManually()
{
	//unique id manager에 정상적으로 등록됐을 때 삭제 
	if (_isValid)
	{
		UnregisterTerritoryUniqueID(this, GetTerritoryObjType(), _instanceID);
	}

	//에디터에서 정상적으로 삭제되도록 플래그 변경
	_isCanDelete = true;

	Destroy();
}

void AGsTerritoryActor::InvalidShape()
{

}

void AGsTerritoryActor::CreateWidget(const FTransform& in_transform)
{
#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		_widgetComp->SetVisibility(true);		

		InvalidWidgetPosition(in_transform.GetLocation());
		InvalidWidgetText();

		if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
		{
			_widgetComp->_viewDistance = settings->_territoryActorViewDistance;
		}
	}
#endif
}

void AGsTerritoryActor::InvalidWidgetPosition(const FVector& inLocation)
{
	const FVector& location = FVector(inLocation.X, inLocation.Y, inLocation.Z + _widgetHeight);
	if (_widgetComp)
	{
		_widgetComp->SetWorldLocation(location);
	}
}

void AGsTerritoryActor::InvalidWidgetText()
{
#if TERR_ED_WIDGET_COMP
	if (nullptr == _widgetComp)
	{
		return;
	}

	InvalidDestription();

	if (UUserWidget* widget = _widgetComp->GetUserWidgetObject())
	{
		if (UGsEditorWidget* editorWidget = Cast<UGsEditorWidget>(widget))
		{
			if (_isValid)
			{
				if (false == _canSpawnOnNavmesh)
				{
					_description += TEXT("Navmesh 위에 있지 않습니다.\n");
					UE_LOG(LogTerritoryEditor, Error, TEXT("TERRITORY_EDITOR_DEBUG - %s 액터는 네비메쉬에 있지 않거나 네비메쉬 검사를 갱신하지 않았을 수도 있습니다. 네비메쉬 위에 배치하고 네비 메쉬에 검사해주세요."), *GetActorLabel());
				}

				editorWidget->SetDescriptionText(_description);
			}
			else
			{
				const FString& log = INVALID_TERRITORY_ACTOR + TEXT("\nreason - ") + _invalidReason;
				editorWidget->SetDescriptionText(log);
			}
		}
	}

	InvalidWidgetColor();
#endif
}

void AGsTerritoryActor::InvalidDestription()
{

}

void AGsTerritoryActor::InvalidWidgetColor()
{
#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		if (UUserWidget* widget = _widgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* editorWidget = Cast<UGsEditorWidget>(widget))
			{
				editorWidget->SetTitleImgColor(_descWidgetBGColor);

				if (false == _isValid)
				{
					editorWidget->SetDescriptionImgColor(FColor::Red);
				}
				else if (false == _canSpawnOnNavmesh)
				{
					editorWidget->SetDescriptionImgColor(FColor::Yellow);
				}
				else
				{
					editorWidget->SetDescriptionImgColor(FColor::White);
				}
			}
		}
	}
#endif
}

FString AGsTerritoryActor::ToString()
{
	const FString& log = TEXT("Territory type : ") + FGsTextUtil::GetStringFromEnum(TEXT("ETerritoryObjType"), GetTerritoryObjType())
		+ TEXT("\tName : ") + GetActorLabel()
		+ TEXT("\tLevel : ") + GetLevelName()
		+ TEXT("\tTerritory unique iD : ") + FString::Printf(TEXT("%lld"), GetTerritoryUniqueID());

	return log;
}

void AGsTerritoryActor::SetWidgetVisibility(bool inVisible)
{
#if TERR_ED_WIDGET_COMP	
	if (_widgetComp)
	{
		_widgetComp->SetVisibility(inVisible);
	}
#endif
}

bool AGsTerritoryActor::IsCanDeleteSelectedActor(FText& out_reasone) const
{
	const FString& log = TEXT("터레인 액터는 Delete키로 삭제할 수 없습니다. 우클릭 - ScriptedAction - Delete를 사용하여 삭제해 주세요(삭제 상황에 나오지 않을 수도 있기 때문에 무시하셔도 됩니다)");
	out_reasone = FText::FromString(log);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);

#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, *log);
#endif

	return _isCanDelete;
}

void AGsTerritoryActor::OnSelected(UObject* inSelect)
{
	if (FGsTerritoryLevelManager::_isLoadArtLevel == false)
	{
		return;
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT(" AGsTerritoryActor::OnSelected - label : %s"), *GetActorLabel());

	SelectInternal(inSelect);
}

void AGsTerritoryActor::SelectInternal(UObject* inSelect)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryActor::SelectInternal - label : %s"), *GetActorLabel());

	if (this == inSelect)
	{
		if (FGsTerritoryLevelManager::_widgetShowType == EGsTerrEdShowType::ShowOnlySelect)
		{
			FGsTerritoryLevelManager::ActiveTerritoryWidget(FGsTerritoryLevelManager::_widgetShowType);

			UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryActor::SelectInternal - only select - label : %s"), *GetActorLabel());
		}
	}
}

void AGsTerritoryActor::SetTerritoryUniqueIDForce(int64 inUniqueId)
{	
	//remove current unique id 
	if (false == FTerritoryEditor::GetInstance()->GetUniqueIDManager()->Remove(GetLevel(), GetTerritoryObjType(), _instanceID))
	{
		const FString& newLabel = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(GetTerritoryObjType());
		SetActorLabel(newLabel);

		return;
	}

	//get instance id
	uint32 newInstanceId = FGsTerritoryObjectInstanceIDController::GetInstanceIDFromUniqueID(GetTerritoryObjType(), inUniqueId);

	//set unique force
	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->AddUniqueIdForce(GetLevel(), GetTerritoryObjType(), newInstanceId);

	//set instance id
	_uniqueID = inUniqueId;
	_instanceID = newInstanceId;

	FString levelName = UGsTerritoryLevelUtil::GetLevelName(GetLevel());
	FString persistantLevelName;
	FString levelKey;

	if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
	{
		//change label
		const FString& newLabel = FGsTerritoryUniqueIDManager::CombineTerritoryActorName(levelKey, GetTerritoryObjType(), newInstanceId);
		SetActorLabel(newLabel);
	}
	else
	{
		const FString& newLabel = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(GetTerritoryObjType());
		SetActorLabel(newLabel);
	}
	
	InvalidWidgetText();
}

void AGsTerritoryActor::SetTerritoryTitleText(FString inTitle)
{
#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		if (UUserWidget* widget = _widgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* infoWidget = Cast<UGsEditorWidget>(widget))
			{
				infoWidget->SetTitleText(inTitle);
			}
		}
	}
#endif
}

void AGsTerritoryActor::SetTerritoryTitleColor(FColor inColor)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryActor::SetTerritoryTitleColor - label : %s"), *GetActorLabel());
#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		if (UUserWidget* widget = _widgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* infoWidget = Cast<UGsEditorWidget>(widget))
			{
				infoWidget->SetTitleImgColor(inColor);
			}
		}
	}
#endif
}

void AGsTerritoryActor::SetTerritoryDescriptionText(FString inDescription)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryActor::SetTerritoryDescriptionText - label : %s"), *GetActorLabel());

#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		if (UUserWidget* widget = _widgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* infoWidget = Cast<UGsEditorWidget>(widget))
			{
				infoWidget->SetDescriptionText(inDescription);
			}
		}
	}
#endif
}

void AGsTerritoryActor::SetTerritoryDescriptionColor(FColor inColor)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryActor::SetTerritoryDescriptionText - label : %s"), *GetActorLabel());

#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		if (UUserWidget* widget = _widgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* infoWidget = Cast<UGsEditorWidget>(widget))
			{
				infoWidget->SetDescriptionImgColor(inColor);
			}
		}
	}
#endif
}