// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIDialogNpc.h"

#include "Engine/World.h"
#include "T1Project.h"
#include "TimerManager.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"

#include "UserWidget.h"
#include "TextBlock.h"
#include "RichTextBlock.h"
#include "WidgetSwitcher.h"
#include "PaperSprite.h"
#include "Image.h"
#include "CanvasPanel.h"
#include "ScrollBox.h"

#include "Data/GsDataContainManager.h"
#include "Quest/Dialog/GsDialogType.h"
#include "NpcDialog/GsSchemaNpcDialogImgData.h"
#include "NpcDialog/GsSchemaNpcDialogImgResData.h"
#include "UI/UIControlLib/TextAnimation/GsTextAnimation.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Sound/GsSoundPlayer.h"
#include "Sound/SoundBase.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Message/GsMessageInput.h"
#include "Management/GsMessageHolder.h"
#include "Input/GsInputEventMsgBase.h"
#include "DataSchema/KeyMapping/GsSchemaKeyMapping.h"
#include "Option/GsOptionEnumType.h"

#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UILib/Container/GsWidgetPool.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "UMG/Public/Components/Spacer.h"

#include "T1Project.h"
#include "Engine/Public/Slate/SceneViewport.h"
#include "UI/ScaleRule/GsUIScaleRule.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"


void UGsUIDialogNpc::BeginDestroy()
{
	_textAnimation = nullptr;
	Super::BeginDestroy();
}

void UGsUIDialogNpc::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _textAnimation)
	{
		if (const UGsGlobalConstant* globalConstant = GData()->GetGlobalData())
		{
			UDataTable* table = Cast<UDataTable>(UAssetManager::GetStreamableManager().LoadSynchronous(globalConstant->_textStyleTalk));
			_textAnimation = NewObject<UGsTextAnimation>(this);
			_textAnimation->Initialize(globalConstant->_cutStringVelocity, table);
		}
	}

	_btnNext->OnClicked.AddDynamic(this, &UGsUIDialogNpc::OnClickNext);
	_btnSkip->OnClicked.AddDynamic(this, &UGsUIDialogNpc::OnClickSkip);
	_btnTouchNarration->OnPressed.AddDynamic(this, &UGsUIDialogNpc::OnPressedTouchNarration);
	_btnTouchNarration->OnReleased.AddDynamic(this, &UGsUIDialogNpc::OnReleasedTouchNarration);

}

void UGsUIDialogNpc::NativeConstruct()
{
#if PLATFORM_WINDOWS || WITH_EDITOR
	InitializeInputComponent();
	RegisterInputComponent();

	if (nullptr != InputComponent)
	{
		if (const auto table = Cast<UGsTableKeyMapping>(FGsSchemaKeyMapping::GetStaticTable()))
		{			
			const FGsSchemaKeyMapping* data = nullptr;
			if (table->FindRowById(EGsKeyMappingType::KEY_MAPPING_OK, data))
			{
				for (auto iter : data->keyInputEvents)
				{
					InputComponent->BindAction<TDelegate<void(EGsKeyMappingType, EInputEvent)>>(data->keyName, iter.GetValue(),
						this, &UGsUIDialogNpc::InputMapKeyAction, data->keyMappingType, iter.GetValue());
				}
			}
		}		
	}
#endif

	Super::NativeConstruct();

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->AddUIScaleFlag(EGsUIScaleFlags::UI_SCALE_NPC_DIALOG);
	}
}

void UGsUIDialogNpc::NativeDestruct()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->RemoveUIScaleFlag(EGsUIScaleFlags::UI_SCALE_NPC_DIALOG);
	}

	if (UGsSoundPlayer* SoundPlayer = GSound()->GetOrCreateSoundPlayer())
	{
		if (SoundPlayer->IsPlaying(_voiceAudioId))
		{
			SoundPlayer->StopSound(_voiceAudioId);
		}
	}

	if (_effectBpNarration != nullptr)
	{
		ReleaseContinentalBP(_effectBpNarration);
		_effectBpNarration = nullptr;
	}

	_addStayTime = 0.0f;
	_textAnimation->Finalize();	
	_voiceAudioId = 0;
	_currDist = 0.f;
	_totalDist = 0.f;
	_currTime = 0.f;
	_isPressed = false;
	_isNarrationPlaying = false;
	
#if PLATFORM_WINDOWS || WITH_EDITOR
	UnregisterInputComponent();
#endif

	Super::NativeDestruct();
}

void UGsUIDialogNpc::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (EGsNpcDialogType::DIALOG_TYPE_DIALOG == _dialogType)
	{
		if (_textAnimation->IsAnimation())
			return;

		if (false == _isAutoNextCut)
		{
			SetVisibleNextBtn(true);
			return;
		}

		if (-1.0f < _addStayTime)
		{
			if (0 < _addStayTime)
			{
				_addStayTime -= InDeltaTime;
				_addStayTime = 0 > _addStayTime ? 0 : _addStayTime;
			}
			else
			{
				_addStayTime = -1.0f;
				OnRemainTimeEnd();
			}
		}
	}
	else
	{
		if (false == _isNarrationPlaying)
			return;

		float deltaSeconds = GetWorld()->GetDeltaSeconds();

		float speeed = (_isPressed) ? _speedNarration * 2 : _speedNarration;
		_currDist += deltaSeconds * speeed;

		float scrollOffset =  _scrollBox->GetScrollOffset();
		float scrollOffsetOfEnd = _scrollBox->GetScrollOffsetOfEnd();		
		if (scrollOffset >= scrollOffsetOfEnd)
		{
			_scrollBox->SetScrollOffset(scrollOffsetOfEnd);
			OnRemainTimeEnd();
		}
		else
		{
			_scrollBox->SetScrollOffset(_currDist);
		}
	}
}

void UGsUIDialogNpc::SetTextEmpty()
{
	_richText->SetText(FText::FromString(""));
	_richTextMy->SetText(FText::FromString(""));
	_richTextFairy->SetText(FText::FromString(""));
	_textActorName->SetText(FText::FromString(""));
}

void UGsUIDialogNpc::HideAll(bool inIsHide)
{
	if (_mainCanvasPanel)
	{
		if (_mainCanvasPanel->IsVisible() && true == inIsHide)
		{
			_mainCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (false == _mainCanvasPanel->IsVisible() && false == inIsHide)
		{
			_mainCanvasPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}	
}

void UGsUIDialogNpc::StartAction(const FGsSchemaNpcDialogCutData* inCutData, bool inIsAutoNextCut)
{
	// 자동으로 다음 컷으로 넘어가는가?
	_isAutoNextCut = inIsAutoNextCut;

	// 다음넘김 유도 연출 숨김
	SetVisibleNextBtn(false);

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		CloseInternal();
		return;
	}	

	if (nullptr != inCutData)
	{
		_dialogType = inCutData->dialogType;
		_swicherType->SetActiveWidgetIndex((int)_dialogType);
		if (EGsNpcDialogType::DIALOG_TYPE_DIALOG == _dialogType)
		{
			inCutData->isHideAllUI ? HideAll(true) : HideAll(false);

			_richText->SetText(FText::FromString(""));
			_richTextMy->SetText(FText::FromString(""));
			_richTextFairy->SetText(FText::FromString(""));

			_strScript = inCutData->scriptText.ToString();
			if (_strScript.Contains(TEXT("{0}")))
			{
				_strScript = (FText::Format(inCutData->scriptText, FText::FromString(userData->mUserName))).ToString();
			}

			switch (inCutData->textColorType)
			{
			case EGsDialogTextColorType::TEXT_COLOR_DEFAULT:
			{
				_textActorName->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NPCDIALOG_BASE_NAME_COLOR));
				_textActorName->SetText(inCutData->actorName);
				_textAnimation->SetData(_richText, _strScript);
			}
			break;
			case EGsDialogTextColorType::TEXT_COLOR_MY:
			{
				_textActorName->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NPCDIALOG_MY_NAME_COLOR));
				_textActorName->SetText(FText::FromString(userData->mUserName));
				_textAnimation->SetData(_richTextMy, _strScript);
			}
			break;
			case EGsDialogTextColorType::TEXT_COLOR_FAIRY:
			{
				_textActorName->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NPCDIALOG_FAIRY_NAME_COLOR));
				_textActorName->SetText(inCutData->actorName);
				_textAnimation->SetData(_richTextFairy, _strScript);
			}
			break;
			default:
				break;
			}

			_panelImgLeft->SetRenderOpacity(0.0f);
			_panelImgCenter->SetRenderOpacity(0.0f);
			_panelImgRight->SetRenderOpacity(0.0f);

			if (const FGsSchemaNpcDialogImgData* imgInfo = inCutData->imgInfo.GetRow())
			{
				const FGsSchemaNpcDialogImgResData* resData = imgInfo->itemRes.GetRow();
				if (nullptr != resData)
				{
					switch (imgInfo->posType)
					{
					case EGsNpcDialogItemResPosType::LEFT:
						_panelImgLeft->SetRenderOpacity(1.0f);
						_imgItem = _imgLeft;
						break;
					case EGsNpcDialogItemResPosType::CENTER:
						_panelImgCenter->SetRenderOpacity(1.0f);
						_imgItem = _imgCenter;
						break;
					case EGsNpcDialogItemResPosType::RIGHT:
						_panelImgRight->SetRenderOpacity(1.0f);
						_imgItem = _imgRight;
						break;
					default:
						break;
					}

					SetImg(resData->resPath);
				}
			}

			const UGsGlobalConstant* globalConstant = GData()->GetGlobalData();
			float addStayTime = 1.0f;
			if (nullptr != globalConstant)
			{
				addStayTime = globalConstant->_cutAddStayTime;
			}
			_addStayTime = addStayTime;

			if (UGsSoundPlayer* SoundPlayer = GSound()->GetOrCreateSoundPlayer())
			{
				// 사운드 재생 로직
				// 내 캐릭터가 여자인지 검사 
				bool bIsFemail = false;
				if (inCutData->isMy)
				{
					bIsFemail = (userData->mGender == CreatureGenderType::FEMALE) ? true : false;
				}

				FString strFileName = LexToString(inCutData->voiceId);
				if (USoundBase* sound = GLocalization()->GetDialogueSound(strFileName, bIsFemail))
				{
					_voiceAudioId = SoundPlayer->PlaySoundObject(sound);
				}
			}

			if (0 < inCutData->playEffectDelayTime)
			{
				_isDelayed = true;
				GetWorld()->GetTimerManager().SetTimer(_effectDelayTimerHandle,
					FTimerDelegate::CreateLambda([this, inCutData]()
						{
							PlayEffect(inCutData);

						}), inCutData->playEffectDelayTime, false);
			}
			else
			{
				PlayEffect(inCutData);
			}
		}
		else
		{
			// 나레이션
			_currDist = 0.f;
			_totalDist = 0.f;
			_currTime = 0.f;
			_isPressed = false;
			_isNarrationPlaying = false;

			_speedNarration = GData()->GetGlobalData()->_speedNarration;			
			_richTextNarration->SetText(FText::GetEmpty());	
			_scrollBox->ScrollToStart();

			// 딜레이 타이머 설정			
			FTimerHandle timehandle;
			GetWorld()->GetTimerManager().SetTimer(timehandle, FTimerDelegate::CreateWeakLambda(this, [inCutData, this]()
				{
					const FGeometry& imgGeometry = _imgNarration->GetCachedGeometry();
					_spacerTopNarration->SetSize(FVector2D(_spacerTopNarration->Size.X, imgGeometry.GetLocalSize().Y));
					_spacerBottomNarration->SetSize(FVector2D(_spacerBottomNarration->Size.X, imgGeometry.GetLocalSize().Y));

					_richTextNarration->SetText(inCutData->scriptText);
					_isNarrationPlaying = true;

				}), 0.5f, false);

			PlayEffectNarration(inCutData);
		}
	}
}

void UGsUIDialogNpc::SetImg(const FSoftObjectPath& iconPath)
{
	if (nullptr == _imgItem)
		return;

	UPaperSprite* sprite = Cast<UPaperSprite>(UAssetManager::GetStreamableManager().LoadSynchronous(iconPath));
	if (nullptr == sprite)
		return;

	FSlateAtlasData atlasData = sprite->GetSlateAtlasData();
	FVector2D size = atlasData.GetSourceDimensions();

	_imgItem->SetBrushFromAtlasInterface(sprite);
	_imgItem->SetBrushSize(size);
}

void UGsUIDialogNpc::OnClickNext()
{
	if (_textAnimation->IsAnimation())
	{
		_textAnimation->StopAnimation();
		return;
	}

	if (0 != _addStayTime)
	{
		_addStayTime = 0.f;
	}

	if (false == _isAutoNextCut)
	{
		OnRemainTimeEnd();
	}
}

void UGsUIDialogNpc::OnClickSkip()
{
	OnStopVoice();
	if (_delegateSkip.IsBound())
	{
		_delegateSkip.ExecuteIfBound();
	}
}

void UGsUIDialogNpc::OnRemainTimeEnd()
{
	OnStopVoice();
	if (_delegateRemainTimeOver.IsBound())
	{
		_delegateRemainTimeOver.ExecuteIfBound();
	}
}

void UGsUIDialogNpc::SetVisibleNextBtn(bool inValue)
{
	// 자동으로 넘어갈때는 표현하지 않는다. 숨겨숨겨
	if (inValue == false)
	{
		_nextEffectWidget->SetVisibility(ESlateVisibility::Collapsed);		
	}
	else
	{
		if (!_nextEffectWidget->GetIsVisible())
		{
			_nextEffectWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UGsUIDialogNpc::OnStopVoice()
{
	if (UGsSoundPlayer* SoundPlayer = GSound()->GetOrCreateSoundPlayer())
	{
		SoundPlayer->StopSound(_voiceAudioId);
	}
}

void UGsUIDialogNpc::PlayEffect(const FGsSchemaNpcDialogCutData* inCutData)
{
	if (nullptr == inCutData)
		return;

	FSoftObjectPath softPath = inCutData->playEffectObject;

	if (softPath.IsValid() == false)
	{
		return;
	}

	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(softPath);
	if (load == nullptr)
	{
		return;
	}

	UParticleSystem* particle = Cast<UParticleSystem>(load);

	if (particle == nullptr)
	{
		return;
	}
	
	FVector dest = inCutData->playEffectOffset;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(dest);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, SpawnTransform, true, EPSCPoolMethod::AutoRelease, true);
}

void UGsUIDialogNpc::InputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent)
{
	switch (keyMapType)
	{
	case EGsKeyMappingType::KEY_MAPPING_OK:
		OnClickNext();
		break;
	default:
		break;
	}
}

void UGsUIDialogNpc::OnPressedTouchNarration()
{
	_isPressed = true;
}

void UGsUIDialogNpc::OnReleasedTouchNarration()
{
	_isPressed = false;
}

void UGsUIDialogNpc::PlayEffectNarration(const FGsSchemaNpcDialogCutData* inCutData)
{
	if (nullptr == inCutData)
		return;

	FSoftObjectPath softPath = inCutData->narrationLoopBP;

	if (softPath.IsValid() == false)
	{
		return;
	}

	_effectBpNarration = MakeContinentalBP(softPath);
	if (_panelNarration != nullptr &&
		_effectBpNarration != nullptr)
	{
		UCanvasPanelSlot* canvasPanelSlot = _panelNarration->AddChildToCanvas(_effectBpNarration);
		if (nullptr == canvasPanelSlot)
		{
			return;
		}

		canvasPanelSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		canvasPanelSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
	}	
}

// BP 만들기
UUserWidget* UGsUIDialogNpc::MakeContinentalBP(const FSoftObjectPath& In_path)
{
	UClass* widgetClass = nullptr;
	if (UGsPathManager* pathMgr = GPath())
	{
		widgetClass = pathMgr->GetClassByObjectPath(In_path);
	}

	if (nullptr == widgetClass)
	{
		return nullptr;
	}

	UUserWidget* widget = Claim(widgetClass);
	if (widget)
	{
		widget->AddToViewport();
	}

	return widget;
}

// BP 생성
UUserWidget* UGsUIDialogNpc::Claim(UClass* In_class)
{
	if (false == _continentalBPPoolManager.IsValid())
	{
		_continentalBPPoolManager = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
		if (false == _continentalBPPoolManager.IsValid())
		{
			return nullptr;
		}
	}

	UGsWidgetPool* pool = _continentalBPPoolManager->FindPool(In_class);
	if (nullptr == pool)
	{
		pool = _continentalBPPoolManager->CreatePool(In_class);
		if (nullptr == pool)
		{
			return nullptr;
		}
	}

	return pool->Claim<UUserWidget>();
}

// BP 해제
void UGsUIDialogNpc::ReleaseContinentalBP(UUserWidget* In_BP)
{
	if (In_BP == nullptr)
	{
		return;
	}

	if (_continentalBPPoolManager.IsValid())
	{
		In_BP->RemoveFromParent();
		_continentalBPPoolManager->ReleaseWidget(In_BP->GetClass(), In_BP);
	}
}
