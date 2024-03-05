#include "GsTerritoryEditorCommands.h"

//UE4
#include "Modules/ModuleManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "LevelEditor.h"
#include "LevelEditorActions.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

//Territory editor
#include "GsTerritoryEditorStyle.h"
#include "Util/GsTerritoryLevelUtil.h"

#include "Editor/TerritoryEditor/GsTerritoryLevelManager.h"
#include "Editor/Blutility/Public/EditorUtilityLibrary.h"

TSharedRef<FUICommandList> FGsTerritoryEditorCommands::_actionList(new FUICommandList());

//Register terrain editor command
FGsTerritoryEditorCommands::FGsTerritoryEditorCommands()
	: TCommands<FGsTerritoryEditorCommands>(FName(TEXT("TerritoryEditor"))
		, FText::FromString(TEXT("Territory editor commands"))
		, NAME_None
		, FGsTerritoryEditorStyle::GetStyleSetName())
{

}

void FGsTerritoryEditorCommands::RegisterCommands()
{
	//Must decalare TerritoryEditor for UI_COMMAND
#define LOCTEXT_NAMESPACE "TerritoryEditor"

	//register ui command
	//shift ctrl alt 
	UI_COMMAND(_changeTerrActorWidgetShowType
		, "ChangeTerrActorWidgetShowType"
		, "Territory editor toolbar command"
		, EUserInterfaceActionType::Button
		, FInputChord(EKeys::W, true, false, true, false));

	UI_COMMAND(_changeTerrActorShapeShowType
		, "ShowTerritoryInfo"
		, "Territory editor toolbar command"
		, EUserInterfaceActionType::Button
		, FInputChord(EKeys::S, true, false, true, false));

	UI_COMMAND(_changeTransparentFloorState
		, "ChangeTransparentFloorState"
		, "Change Transparent Floor State"
		, EUserInterfaceActionType::Button
		, FInputChord(EKeys::T, true, false, true, false));

	UI_COMMAND(_changeSightRadiusState
		, "ChangeSightRadius"
		, "Change Sight Radius State"
		, EUserInterfaceActionType::Button
		, FInputChord(EKeys::R, false, true, true, false));
	
	UI_COMMAND(_changeScalabilityQaulityLowState
		, "ScalabilityQualityLow"
		, "Scalability Quality Low"
		, EUserInterfaceActionType::Button
		, FInputChord(EKeys::Q, true, false, true, false));

	UI_COMMAND(_deleteTerritoryActor
		, "DeleteTerritoryActor"
		, "Delete Territory Actor"
		, EUserInterfaceActionType::Button
		, FInputChord(EKeys::D, true, false, true, false));

	//bind
	_actionList->MapAction(_changeTerrActorWidgetShowType
		, FExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnChangeTerrActorWidgetShowType)
		, FCanExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnCanChangeTerrActorWidgetShowType));
	_actionList->MapAction(_changeTerrActorShapeShowType
		, FExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnChangeTerrActorShapeShowType)
		, FCanExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnCanChangeTerrActorShapeShowType));
	_actionList->MapAction(_changeTransparentFloorState
		, FExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnChangeTerrActorTransparentFloorState)
		, FCanExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnCanChangeTerrActorTransparentFloorState));
	_actionList->MapAction(_changeSightRadiusState
		, FExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnChangeTerrActorSightRadiusState)
		, FCanExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnCanChangeTerrActorSightRadiusState));
	_actionList->MapAction(_changeScalabilityQaulityLowState
		, FExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnChangeScalabilityQaulityState)
		, FCanExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnCanScalabilityQualityState));
	_actionList->MapAction(_deleteTerritoryActor
		, FExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnDeleteTerritoryActor)
		, FCanExecuteAction::CreateStatic(&FGsTerritoryEditorCommands::OnCanDeleteTerritoryActor));

	//add ui commmand to level editor
	FLevelEditorModule& levelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	TSharedRef<FUICommandList> levelEditorCommandBindings = levelEditor.GetGlobalLevelEditorActions();

	levelEditorCommandBindings->Append(_actionList);

#undef  LOCTEXT_NAMSPACE
}

void FGsTerritoryEditorCommands::AddMenuExtension(FMenuBuilder& inBuilder)
{
	//inBuilder.AddMenuEntry(FGsTerritoryEditorCommands::Get().OpenTerritoryEditor);
}

void FGsTerritoryEditorCommands::OnOpenTerritoryEditor()
{
	UE_LOG(LogTemp, Log, TEXT("Open test"));
}

bool FGsTerritoryEditorCommands::OnCanOpenTerritoryEditor()
{
	UE_LOG(LogTemp, Log, TEXT("Can open test"));

	return true;
}

void FGsTerritoryEditorCommands::OnChangeTerrActorWidgetShowType()
{
	switch (FGsTerritoryLevelManager::_widgetShowType)
	{
	case EGsTerrEdShowType::HideAll:
		FGsTerritoryLevelManager::ShowOnlySelectedWidget();
		break;
	case EGsTerrEdShowType::ShowOnlySelect:
		FGsTerritoryLevelManager::ShowAllWidget();
		break;
	case EGsTerrEdShowType::ShowAll:
		FGsTerritoryLevelManager::HideAllWidget();
		break;
	}
}

bool FGsTerritoryEditorCommands::OnCanChangeTerrActorWidgetShowType()
{
	return true;
}

void FGsTerritoryEditorCommands::OnChangeTerrActorShapeShowType()
{
	switch (FGsTerritoryLevelManager::_shapeShowType)
	{
	case EGsTerrEdShowType::HideAll:
		FGsTerritoryLevelManager::ShowOnlySelectedShape();
		break;
	case EGsTerrEdShowType::ShowOnlySelect:
		FGsTerritoryLevelManager::ShowAllShape();
		break;
	case EGsTerrEdShowType::ShowAll:
		FGsTerritoryLevelManager::HideAllShape();
		break;
	}
}

bool FGsTerritoryEditorCommands::OnCanChangeTerrActorShapeShowType()
{
	return true;
}

void FGsTerritoryEditorCommands::OnChangeTerrActorTransparentFloorState()
{
	FGsTerritoryLevelManager::SetTransparentFloor(!FGsTerritoryLevelManager::IsTransparentFloor());	
}

bool FGsTerritoryEditorCommands::OnCanChangeTerrActorTransparentFloorState()
{
	return true;
}

void FGsTerritoryEditorCommands::OnChangeTerrActorSightRadiusState()
{
	switch (FGsTerritoryLevelManager::_sightRadiusShowType)
	{
	case EGsTerrEdShowType::HideAll:
		FGsTerritoryLevelManager::SelectOnlySightRadius();
		break;
	case EGsTerrEdShowType::ShowOnlySelect:
		FGsTerritoryLevelManager::ShowSightRadius();
		break;
	case EGsTerrEdShowType::ShowAll:
		FGsTerritoryLevelManager::HideAllSightRadius();
		break;
	}
}

bool FGsTerritoryEditorCommands::OnCanChangeTerrActorSightRadiusState()
{
	return true;
}

void FGsTerritoryEditorCommands::OnChangeScalabilityQaulityState()
{
	FGsTerritoryLevelManager::SetScalabilityQaulityLow(!FGsTerritoryLevelManager::_isScalabilityQualityLow);
}

bool FGsTerritoryEditorCommands::OnCanScalabilityQualityState()
{
	return true;
}

bool FGsTerritoryEditorCommands::OnCanDeleteTerritoryActor()
{
	return true;
}

void FGsTerritoryEditorCommands::OnDeleteTerritoryActor()
{
	//get all selected territory actor
	AGsTerritoryActor* territoryActor = nullptr;
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	for (AActor* actor : selectArray)
	{
		if (nullptr == actor)
		{
			continue;
		}

		territoryActor = Cast<AGsTerritoryActor>(actor);
		if (territoryActor)
		{
			//delete territory actor
			territoryActor->DestroyManually();
		}
	}	
}
