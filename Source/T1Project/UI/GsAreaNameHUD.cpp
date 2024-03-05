// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAreaNameHUD.h"

#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Image.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "../Management/ScopeGame/GsGameObjectManager.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "../GameObject/Define/GsGameObjectDefine.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"
#include "../GameObject/ObjectClass/GsGameObjectCreature.h"

#include "UI/UIContent/Tray/GsUITrayMovie.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorComponentEx/WidgetComponentEx.h"

#include "T1Project.h"


#include "Editor/Widget/GsEditorWidget.h"


// Sets default values
AGsAreaNameHUD::AGsAreaNameHUD()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	IsInCollider = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	if (BoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.Clear();
		BoxCollision->OnComponentEndOverlap.Clear();

		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGsAreaNameHUD::ShowAreaNameHUD);
		BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AGsAreaNameHUD::EndAreaNameHUD);
		BoxCollision->SetCollisionProfileName("Trigger");
		BoxCollision->SetCanEverAffectNavigation(false);
		BoxCollision->AreaClass = nullptr;
		BoxCollision->bApplyImpulseOnDamage = false;

#if WITH_EDITOR
		BoxCollision->SetVisibility(true);
		BoxCollision->bHiddenInGame = false;
#else
		BoxCollision->SetVisibility(false);
		BoxCollision->bHiddenInGame = true;
#endif
		
		EditorWidgetComp = CreateDefaultSubobject<UWidgetComponentEx>(TEXT("EidtorWidgetComp"));
		if (EditorWidgetComp)
		{
			EditorWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

#if WITH_EDITOR
			EditorWidgetComp->_enableFaceToCamera = true;
			EditorWidgetComp->SetVisibility(true, true);
#else
			EditorWidgetComp->_enableFaceToCamera = false;
			EditorWidgetComp->SetVisibility(false, true);
#endif
		}
	}
}

void AGsAreaNameHUD::ShowAreaNameHUD(UPrimitiveComponent* in_overlappedComponent,
	AActor* in_otherActor,
	UPrimitiveComponent* in_otherComp,
	int32 in_otherBodyIndex,
	bool in_bFromSweep,
	const FHitResult& in_SweepResult)
{
	if (in_otherActor)
	{
		//GSLOG(Log, TEXT("AGsAreaNameHUD::ShowAreaName"));
		// 충돌 발생한 Actor 가 LocalPlayer 아니면 return 처리 (LocalPlayer Actor 말고도 들어옴)
		if (false == in_otherActor->IsA(AGsCharacterLocalPlayer::StaticClass()))
			return;

		if (true == IsInCollider)
			return;

		// 로딩 무비 열린게 아니면 메시지 전송
		// 페이드 이펙트도 열리지 않아야 함(같은 맵은 로딩무비 안뜨고 페이드만 뜸)
		if (GetIsShowLoadingMovie() == false &&
			GetIsShowFadeEffect() == false)
		{
			GMessage()->GetAreaTriggerEvent().SendMessage(MessageContentHud::AREA_TRIGGER_BEGIN, *this);
		}
		
		IsInCollider = true;
	}
}

bool AGsAreaNameHUD::IsIn(const FVector& inLocation)
{
	return UKismetMathLibrary::IsPointInBox(inLocation
		, BoxCollision->GetComponentLocation()
		, BoxCollision->GetScaledBoxExtent());
}

// 로딩 무비가 출력중인지
bool AGsAreaNameHUD::GetIsShowLoadingMovie()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->IsActiveLoadingUI();
	}

	return false;
}

// 로딩시 뜨는 페이드 이펙트 떠있는지
bool AGsAreaNameHUD::GetIsShowFadeEffect()
{
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return false;
	}

	bool isLoadingMovieOpend = false;
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(TEXT("TrayFadeEffect"));
	if (widget.IsValid() == false)
	{
		return false;
	}

	return true;
}

void AGsAreaNameHUD::PlaySound()
{
	/*
	if (0 == (uint16)PlaySoundID)
	{
		GSLOG(Log , TEXT("PlaySoundID value is error : %d") , PlaySoundID);
		return;
	}

	if (true == GSound()->GetSoundPlayer()->IsPlaying((uint64)PlaySoundID))
	{
		GSLOG(Log, TEXT("Already Playing Sound  : %d"), PlaySoundID);
		return;
	}

	GSound()->GetSoundPlayer()->PlaySound((uint16)PlaySoundID);
	*/
	//GSLOG(Log, TEXT("PlaySound value : %d"), PlaySoundID);
}

void AGsAreaNameHUD::EndAreaNameHUD(UPrimitiveComponent* in_overlappedComponent, AActor* in_otherActor, UPrimitiveComponent* in_otherComp, int32 in_otherBodyIndex)
{
	if (nullptr == in_otherActor)
	{
		return;
	}

	// 충돌 발생한 Actor 가 LocalPlayer 아니면 return 처리 (LocalPlayer Actor 말고도 들어옴)
	if (false == in_otherActor->IsA(AGsCharacterLocalPlayer::StaticClass()))
	{
		return;
	}

	if (false == IsInCollider)
	{
		return;
	}		

	IsInCollider = false;

	bool isDead = false;
	if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(localPlayer))
		{
			isDead = creature->IsDeadState();
		}
	}

	if (false == GetIsShowLoadingMovie() &&
		false == GetIsShowFadeEffect() && 
		false == isDead)
	{
		GMessage()->GetAreaTriggerEvent().SendMessage(MessageContentHud::AREA_TRIGGER_END, *this);
	}
}

void AGsAreaNameHUD::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if (EditorWidgetComp)
	{
		RefreshWidget();
	}
#endif
}

void AGsAreaNameHUD::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

#if WITH_EDITOR
	RefreshWidget();
#endif
}

void AGsAreaNameHUD::BeginPlay()
{
	Super::BeginPlay();

	SetHidden(true);

#if WITH_EDITOR
	RefreshWidget();
#endif
	
	_msgDelegates.Emplace(
		GMessage()->GetGameObject().AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
			&AGsAreaNameHUD::OnLocalPlayerSpawnComplete)
	);

	_msgDelegates.Emplace(
		GMessage()->GetGameObject().AddUObject(MessageGameObject::LOCAL_REVIVE, this,
			&AGsAreaNameHUD::OnLocalPlayerSpawnComplete)
	);
	
}
void AGsAreaNameHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FGsMessageHolder* msg = GMessage();
	{
		if (_msgDelegates.Num() != 0)
		{
			for (auto iter : _msgDelegates)
			{
				msg->GetGameObject().Remove(iter);
			}
			_msgDelegates.Empty();
		}
	}
}
#if WITH_EDITOR
void AGsAreaNameHUD::ShowAreaNameText()
{
	if (EditorWidgetComp)
	{
		if (UUserWidget* widget = EditorWidgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* editorWidget = Cast<UGsEditorWidget>(widget))
			{
				if (editorWidget->DescImg)
				{
					editorWidget->DescImg->SetColorAndOpacity(FLinearColor::Gray);
				}
				if (editorWidget->DescriptionText)
				{
					FString strName = AreaAtTheTopName.ToString();
					strName.Append("\n");
					strName.Append(AreaAtTheBottomName.ToString());

					editorWidget->DescriptionText->SetColorAndOpacity(FLinearColor::Black);
					editorWidget->DescriptionText->SetText(FText::FromString(strName));
				}
			}
		}
	}
}

void AGsAreaNameHUD::RefreshWidget()
{
	if (EditorWidgetComp)
	{
		FTransform trs = GetTransform();
		FVector scale = trs.GetScale3D();
		FVector widgetScale = FVector::OneVector / scale;

		EditorWidgetComp->SetRelativeScale3D(widgetScale);
		EditorWidgetComp->SetDrawSize(WidgetScreenSize);
		RefreshWidgetLocation();
		ShowAreaNameText();
	}
}

void AGsAreaNameHUD::RefreshWidgetLocation()
{
	if (EditorWidgetComp)
	{
		if (BoxCollision)
		{
			EditorWidgetComp->SetWorldLocation(BoxCollision->GetComponentLocation());
		}
	}
}

#endif

// 로컬 스폰 끝
void AGsAreaNameHUD::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	if (IsInCollider == false)
	{
		return;
	}

	// 로딩 되었을때 컬리더 안이면 처리 호출
	GMessage()->GetAreaTriggerEvent().SendMessage(MessageContentHud::AREA_TRIGGER_BEGIN, *this);
}