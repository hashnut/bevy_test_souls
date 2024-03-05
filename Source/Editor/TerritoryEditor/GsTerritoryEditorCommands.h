#pragma once
#include "Commands.h"

//UE4
#include "EditorStyleSet.h"
#include "Framework/Commands/UICommandList.h"

class FGsTerritoryEditorCommands : public TCommands<FGsTerritoryEditorCommands>
{
public:
	TSharedPtr<FUICommandInfo> _openTerritoryEditor;
	TSharedPtr<FUICommandInfo> _changeTerrActorWidgetShowType;
	TSharedPtr<FUICommandInfo> _changeTerrActorShapeShowType;
	TSharedPtr<FUICommandInfo> _changeTransparentFloorState;
	TSharedPtr<FUICommandInfo> _changeSightRadiusState;
	TSharedPtr<FUICommandInfo> _changeScalabilityQaulityLowState;
	TSharedPtr<FUICommandInfo> _deleteTerritoryActor;
	static TSharedRef<FUICommandList> _actionList;

public:
	FGsTerritoryEditorCommands();	
	virtual void RegisterCommands() override;

public:
	void AddMenuExtension(FMenuBuilder& inBuilder);
	static void OnOpenTerritoryEditor();
	static bool OnCanOpenTerritoryEditor();

public:
	static void OnChangeTerrActorWidgetShowType();
	static bool OnCanChangeTerrActorWidgetShowType();

	static void OnChangeTerrActorShapeShowType();
	static bool OnCanChangeTerrActorShapeShowType();

	static void OnChangeTerrActorTransparentFloorState();
	static bool OnCanChangeTerrActorTransparentFloorState();

	static void OnChangeTerrActorSightRadiusState();
	static bool OnCanChangeTerrActorSightRadiusState();

	static void OnChangeScalabilityQaulityState();
	static bool OnCanScalabilityQualityState();

	static bool OnCanDeleteTerritoryActor();
	static void OnDeleteTerritoryActor();
};