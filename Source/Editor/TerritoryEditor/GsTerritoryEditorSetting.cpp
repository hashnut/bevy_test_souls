#include "GsTerritoryEditorSetting.h"

//UE4
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"

#if WITH_EDITOR && WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#endif

//Territory Editor
#include "TerritoryEditor.h"



UGsTerritoryEditorSetting::UGsTerritoryEditorSetting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LoadConfig();
}

UGsTerritoryEditorSetting::~UGsTerritoryEditorSetting()
{

}

void UGsTerritoryEditorSetting::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
