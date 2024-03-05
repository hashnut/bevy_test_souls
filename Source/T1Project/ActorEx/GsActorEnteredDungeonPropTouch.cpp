
#include "GsActorEnteredDungeonPropTouch.h"
#include "Components/SphereComponent.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "GameObject/Define/GsGameObjectText.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "ActorComponentEx/GsWidgetComponentButton.h"
#include "../UI/UIContent/Billboard/GsUIBillboardEliteDungeonWarp.h"


AGsActorEnteredDungeonPropTouch::AGsActorEnteredDungeonPropTouch()
{
	_rootWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetRoot"));
	_rootWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.f));
	_rootWidget->SetupAttachment(RootComponent);

	_rootWidget->SetActive(true);

	_nameplateWidget = CreateDefaultSubobject<UWidgetComponent>(NameplateWidgetComponentName);
	_nameplateWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	_nameplateWidget->SetWidgetSpace(EWidgetSpace::Screen);
	_nameplateWidget->SetDrawSize(FVector2D(200, 100.0f));
	_nameplateWidget->SetCastShadow(false);
	_nameplateWidget->SetupAttachment(_rootWidget);

	//_interactionWidget = CreateDefaultSubobject<UGsWidgetComponentButton>(TEXT("InteractionEnterWidget"));
	//_interactionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	//_interactionWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	//_interactionWidget->SetDrawSize(FVector2D(200, 100.0f));
	//_interactionWidget->SetCastShadow(false);
	//_interactionWidget->SetupAttachment(_rootWidget);
}

void AGsActorEnteredDungeonPropTouch::BeginPlay()
{
	Super::BeginPlay();

	if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
	{
		if (UClass* widgetClass = GPath()->GetClassByObjectPath(globalData->_eliteDungeonPortalPlatePath))
		{
			_nameplateWidget->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
		}
	}
}

UGsUIBillboard* AGsActorEnteredDungeonPropTouch::GetWidget() const
{
	if (_nameplateWidget && _nameplateWidget->GetUserWidgetObject())
	{
		return Cast<UGsUIBillboard>(_nameplateWidget->GetUserWidgetObject());
	}
	return nullptr;
}

UGsUIBillboardEliteDungeonWarp* AGsActorEnteredDungeonPropTouch::GetPlate() const
{
	return Cast<UGsUIBillboardEliteDungeonWarp>(GetWidget());
}