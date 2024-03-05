// Fill out your copyright notice in the Description page of Project Settings.


#include "GsWidgetComponentPooling.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UILib/Container/GsWidgetPool.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"


void UGsWidgetComponentPooling::SetWidgetClassWithPooling(TSubclassOf<UUserWidget> InWidgetClass, TFunction<void(class UUserWidget*)> InCallbackDestory)
{
	if (WidgetClass != InWidgetClass)
	{
		WidgetClass = InWidgetClass;

		if (FSlateApplication::IsInitialized())
		{
			if (HasBegunPlay() && !GetWorld()->bIsTearingDown)
			{
				if (WidgetClass)
				{
					UUserWidget* NewWidget = Claim(WidgetClass, InCallbackDestory);
					SetWidget(NewWidget);
				}
				else
				{
					SetWidget(nullptr);
				}
			}
			else
			{
				_callbackDestory = InCallbackDestory;
			}
		}
	}
}
void UGsWidgetComponentPooling::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (_poolManager.IsValid())
	{
		_poolManager->ReleaseWidget(GetWidgetClass(), GetUserWidgetObject());
	}

	Super::EndPlay(EndPlayReason);		
}

void UGsWidgetComponentPooling::InitWidget()
{
	// Don't do any work if Slate is not initialized
	if (FSlateApplication::IsInitialized())
	{
		UWorld* World = GetWorld();

		if (WidgetClass && GetUserWidgetObject() == nullptr && World && !World->bIsTearingDown)
		{
			SetWidget(Claim(WidgetClass, _callbackDestory));
			SetTickMode(TickMode);
		}

#if WITH_EDITOR
		if (GetUserWidgetObject() && !World->IsGameWorld() && !bEditTimeUsable)
		{
			if (!GEnableVREditorHacks)
			{
				// Prevent native ticking of editor component previews
				GetUserWidgetObject()->SetDesignerFlags(EWidgetDesignFlags::Designing);
			}
		}
#endif
	}
}


UUserWidget* UGsWidgetComponentPooling::Claim(UClass* InClass, TFunction<void(class UUserWidget*)> InCallbackDestory)
{
	if (false == _poolManager.IsValid())
	{
		_poolManager = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
		if (false == _poolManager.IsValid())
		{
			return nullptr;
		}
	}

	UGsWidgetPool* pool = _poolManager->FindPool(InClass);
	if (nullptr == pool)
	{
		pool = _poolManager->CreatePool(InClass, InCallbackDestory);
		if (nullptr == pool)
		{
			return nullptr;
		}
	}

	return pool->Claim<UUserWidget>();
}