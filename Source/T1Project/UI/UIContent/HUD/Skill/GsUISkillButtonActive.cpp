// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUISkillButtonActive.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICoolTimeEffect.h"
#include "UI/UIContent/HUD/Skill/GsUISkillChainEffect.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Message/GsMessageContentHud.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Data/GsResourceManager.h"
#include "Skill/GsSkill.h"
#include "Skill/GsComboSkillGroup.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "Materials/MaterialInstanceDynamic.h"


void UGsUISkillButtonActive::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUISkillButtonActive::OnClickSlot);

	_btnSkill->OnPressed.AddDynamic(this, &UGsUISkillButtonActive::OnPresseSkill);
	_btnSkill->OnReleased.AddDynamic(this, &UGsUISkillButtonActive::OnReleaseSkill);
	_btnSkill->OnClicked.AddDynamic(this, &UGsUISkillButtonActive::OnClickSkill);
	_btnSkill->OnMouseButtonEventDownEx.BindUObject(this, &UGsUISkillButtonActive::OnMouseButtonDownEx);

	_btnDimmed->OnReleased.AddDynamic(this, &UGsUISkillButtonActive::OnReleaseAny);
	_btnDimmed->OnMouseButtonEventDownEx.BindUObject(this, &UGsUISkillButtonActive::OnMouseButtonDownEx);

	_btnReserve->OnReleased.AddDynamic(this, &UGsUISkillButtonActive::OnReleaseAny);
	_btnReserve->OnMouseButtonEventDownEx.BindUObject(this, &UGsUISkillButtonActive::OnMouseButtonDownEx);

	_dragDownLogic.OnSlotDragDownTranslation.BindUObject(this, &UGsUISkillButtonActive::OnSlotDragDownTranslation);
	_dragDownLogic.OnSlotDragDownStateChanged.BindUObject(this, &UGsUISkillButtonActive::OnSlotDragDownStateChanged);

	_uiChainEffect->OnFinishAnimation.BindUObject(this, &UGsUISkillButtonActive::OnFinishChainAnimation);

	_switcherChainList.Empty();
	_switcherChainList.Emplace(_switcherChain0);
	_switcherChainList.Emplace(_switcherChain1);
	_switcherChainList.Emplace(_switcherChain2);
}

void UGsUISkillButtonActive::NativeConstruct()
{
	Super::NativeConstruct();

	_dragDownLogic.SetSensitiveValue(_dragDownSensitive);
	_dragDownLogic.SetDragDownValue(_dragDownValue);
}

FReply UGsUISkillButtonActive::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	// 버튼때문에 TouchStart는 안들어옴
	// 주의: 버튼이 PreciseClick 옵션일 때만 이곳에 들어온다
	FReply reply = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	if (_bIsCaptureMouse)
	{
		if (false == _pressStartPosAbs.IsZero())
		{
			if (_bCanDragDown)
			{
				_dragDownLogic.OnMouseButtonMove(InGeometry, InGestureEvent);
			}

			FVector2D startPos = InGeometry.AbsoluteToLocal(_pressStartPosAbs);
			FVector2D movePos = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
			float diffY = FMath::Abs(startPos.Y - movePos.Y);
			float diffX = FMath::Abs(startPos.X - movePos.X);
			
			// 스크롤을 방해하는 문제로 X축도 체크
			if (_dragDownSensitive < diffY &&
				_dragDownSensitive > diffX)
			{
				return FReply::Handled().CaptureMouse(TakeWidget());
			}
		}
	}

	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

FReply UGsUISkillButtonActive::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
		
	_pressStartPosAbs = FVector2D::ZeroVector;

	if (_bIsCaptureMouse)
	{
		if (_bCanDragDown)
		{
			_dragDownLogic.OnButtonReleased();
		}

		_bIsCaptureMouse = false;

		return FReply::Handled().ReleaseMouseCapture();
	}

	_bIsCaptureMouse = false;

	return FReply::Unhandled();
}

void UGsUISkillButtonActive::SetData(const FGsSkill* InData)
{	
	if (nullptr == InData)
	{
		SetEmptySlot();
		return;
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	_panelRootButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UpdateData(InData);

	// 체인 설정
	if (InData->IsChainSkill())
	{
		SetChainStep(InData->GetChainStep(), InData->GetMaxChainStep());
	}
	else
	{
		SetChainStep(0, 0);
	}
}

bool UGsUISkillButtonActive::UpdateData(const FGsSkill* InData)
{
	int32 prevSkill = _skillId;

	if (false == Super::UpdateData(InData))
	{
		return false;
	}

	if (InData->GetSkillId() != prevSkill)
	{
		if (UMaterialInstanceDynamic* mat = _imgIcon->GetDynamicMaterial())
		{
			if (UPaperSprite* icon = GResource()->LoadSync<UPaperSprite>(*InData->GetIconPath()))
			{
				FSlateAtlasData atlasData = icon->GetSlateAtlasData();
				mat->SetTextureParameterValue(TEXT("Texture"), atlasData.AtlasTexture);
				mat->SetScalarParameterValue(TEXT("OffsetU"), atlasData.StartUV.X);
				mat->SetScalarParameterValue(TEXT("OffsetV"), atlasData.StartUV.Y);
				mat->SetScalarParameterValue(TEXT("TilingU"), atlasData.SizeUV.X);
				mat->SetScalarParameterValue(TEXT("TilingV"), atlasData.SizeUV.Y);
			}
		}
	}

	// 오토표시
	_bCanDragDown = CheckCanDragDown(InData);
	_firstChainSkillId = INVALID_SKILL_ID;
	
	const FGsSkill* firstChainSkill = nullptr;
	if (InData->IsChainSkill())
	{
		// 첫 체인스킬이 오토이면 나머지 스킬은 오토가 아니어도 오토처럼 슬롯 내려서 표시
		if (1 < InData->GetChainStep())
		{
			if (const FGsComboSkillGroup* comboGroup = GSkill()->FindChainSkillGroup(_skillId))
			{
				firstChainSkill = comboGroup->GetFirstSkill();
			}
		}
		else
		{
			// 체인 1단계일 경우
			firstChainSkill = InData;
		}
	}

	if (firstChainSkill)
	{
		_firstChainSkillId = firstChainSkill->GetSkillId();
		SetAutoUse(_firstChainSkillId, firstChainSkill->IsAuto());
	}
	else
	{
		SetAutoUse(_skillId, InData->IsAuto());
	}
	

	bool isShowSelf = false;
	if (InData->_isSelectTarget == true &&
		GSkill()->GetIsShowSkillTargetSelect() == true &&
		GSkill()->GetCurrentSkillTargetSlotId() == _slotId)
	{
		isShowSelf = true;
	}

	_panelSelf->SetVisibility(isShowSelf ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	_effectTargetSelectSlot->SetVisibility(isShowSelf ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	// 체인 연출 초기화
	if (false == _uiChainEffect->IsPlayingEffect())
	{
		_imgIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_uiChainEffect->SetVisibility(ESlateVisibility::Collapsed);
	}

	return true;
}

void UGsUISkillButtonActive::SetEnableButton(bool InEnable)
{
	Super::SetEnableButton(InEnable);

	// 기존처럼 UWidget::SetIsEnabled 를 호출하게 되면, NativeTick 이 불리지 않아서 현재 페이지가 아닌 슬롯들의
	// 스킬을 사용할 수 없게 된다 (0.3 초짜리 딜레이가 풀리지 않음)
	_btnSkill->SetIsEnabledGraphics(InEnable);

	SetGrayscale(!InEnable);
}

void UGsUISkillButtonActive::SetCooltimeUI(float InRemainSec, float InRate)
{
	if (0 < InRemainSec)
	{
		_textBlockRemainTime->SetText(FText::AsNumber(FMath::CeilToInt(InRemainSec)));

		// SELF가 겹쳐 보이는 현상 때문
		_panelSelf->SetRenderOpacity(0.f);
	}
	else
	{
		_textBlockRemainTime->SetText(FText::GetEmpty());

		// SELF가 겹쳐 보이는 현상 때문
		_panelSelf->SetRenderOpacity(1.f);
	}

	_uiCoolTime->SetProgress(InRate);
}

void UGsUISkillButtonActive::SetReserveSlot(bool InReserve)
{
	if (IsEmpty() || _bIsCoolTime)
	{
		InReserve = false;
	}

	_btnReserve->SetVisibility(InReserve ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// SELF가 겹쳐 보이는 현상 때문
	_panelSelf->SetRenderOpacity(InReserve ? 0.f : 1.f);
}

void UGsUISkillButtonActive::SetAutoUse(SkillId InSkillId, bool bInIsAuto)
{
	if (INVALID_SKILL_ID != InSkillId)
	{
		if (false == IsEmpty())
		{
			if (InSkillId == _skillId ||
				InSkillId == _firstChainSkillId)
			{
				if (_bCanDragDown)
				{
					SetAutoUseInter(bInIsAuto);
					return;
				}
			}
		}
	}

	SetAutoUseInter(false);
}

void UGsUISkillButtonActive::SetEmptySlot()
{
	Super::SetEmptySlot();

	_firstChainSkillId = INVALID_SKILL_ID;
	SetChainStep(0, 0);
	_panelRootChain->SetVisibility(ESlateVisibility::Hidden);

	_imgIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_uiChainEffect->SetVisibility(ESlateVisibility::Collapsed);
	
	_panelRootButton->SetVisibility(ESlateVisibility::Hidden);
	
	_bAutoChanged = false;
	SetAutoUse(INVALID_SKILL_ID, false);

	_panelSelf->SetVisibility(ESlateVisibility::Hidden);
	_panelSelf->SetRenderOpacity(1.f);
}

void UGsUISkillButtonActive::PlayChangeStepEffect(const FGsSkill* InPrevSkill, const FGsSkill* InSkill)
{
	if (nullptr == InPrevSkill || nullptr == InSkill)
	{
		return;
	}

	bool bIsBtnEnabled = _btnSkill->GetIsEnabled();
	_uiChainEffect->SetData(InPrevSkill, InSkill, bIsBtnEnabled);
	_uiChainEffect->SetVisibility(ESlateVisibility::HitTestInvisible);
	_imgIcon->SetVisibility(ESlateVisibility::Hidden);

	if (bIsBtnEnabled)
	{
		OnSkillChain();
	}
}

void UGsUISkillButtonActive::PlaySkillStartEffect()
{
	OnSkillStartEffect();
}

void UGsUISkillButtonActive::OnKeyboardPress()
{
	if (_btnDimmed->IsVisible())
	{
		OnClickDimmedButton();
	}

	if (false == _btnSkill->GetIsEnabled())
	{
		return;
	}

	OnPresseSkill();
}

void UGsUISkillButtonActive::OnKeyboardRelease()
{
	if (false == _btnSkill->GetIsEnabled())
	{
		return;
	}

	OnReleaseSkill();
	UseSkill();
}

bool UGsUISkillButtonActive::IsReserved() const
{
	return _btnReserve->IsVisible();
}

void UGsUISkillButtonActive::OnClickSlot()
{
	if (IsEmpty())
	{
		if (IsTutorialInteraction())
		{
			OnTutorialInteraction.ExecuteIfBound();
		}
	}
}

void UGsUISkillButtonActive::OnPresseSkill()
{
	// 이미 예약 걸려있으면 무시
	if (IsReserved())
	{
		return;
	}

	_bAutoChanged = false;
}

void UGsUISkillButtonActive::OnReleaseSkill()
{
	OnReleaseAny();

	// 이미 예약 걸려있으면 무시
	if (IsReserved())
	{
		return;
	}
}

void UGsUISkillButtonActive::OnClickSkill()
{
	UseSkill();
}

void UGsUISkillButtonActive::OnReleaseAny()
{
	// 스크롤 판정 혹은 캡쳐되지 않은 상태에서 영역 밖으로 나간경우에 초기화 위함
	if (_bIsCaptureMouse)
	{
		if (false == HasMouseCapture())
		{
			_bIsCaptureMouse = false;
			_pressStartPosAbs = FVector2D::ZeroVector;
		}
	}
}

void UGsUISkillButtonActive::OnMouseButtonDownEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	// 주의: 버튼이 있으면 NativeOnTouchStarted 가 안들어오므로 여기서 저장. 
	// 계산을 버튼이 아닌 위젯으로 할거라 로컬좌표로 변환하지 않은 절대좌표 저장
	if (false == _bIsCaptureMouse)
	{
		_bIsCaptureMouse = true;
		_pressStartPosAbs = MouseEvent.GetScreenSpacePosition();

		if (_bCanDragDown)
		{
			_dragDownLogic.OnMouseButtonDown(Geometry, MouseEvent);
		}
	}
}

void UGsUISkillButtonActive::OnSlotDragDownTranslation(const FVector2D& InTranslation)
{
	if (_bCanDragDown)
	{
		SetRenderTranslation(InTranslation);
	}
	else
	{
		SetRenderTranslation(FVector2D::ZeroVector);
	}
}

void UGsUISkillButtonActive::OnSlotDragDownStateChanged()
{
	bool bRequestAuto = _dragDownLogic.GetIsDown();

	if (_bCanDragDown)
	{
		_bAutoChanged = true;

		if (false == _bIsFakePage)
		{
			SkillId autoTargetSkill = (INVALID_SKILL_ID == _firstChainSkillId) ? _skillId : _firstChainSkillId;
			FGsUIMsgParamRequestSkillAuto param(autoTargetSkill, bRequestAuto);
			GMessage()->GetUI().SendMessage(MessageUI::REQUEST_SKILL_AUTO, &param);
		}

		if (IsTutorialInteraction())
		{
			OnTutorialInteraction.ExecuteIfBound();
		}
	}

	// 내려간것과 오토연출이 따로노는 현상 방지, 패킷 응답이 오면 SetAutoUse를 통해 재세팅됨
	SetAutoUseInter(bRequestAuto);

	if (false == _bIsFakePage)
	{
		// close skill target select
		GSkill()->SetIsShowSkillTargetSelect(false);
	}
}

void UGsUISkillButtonActive::OnFinishChainAnimation()
{
	_imgIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_uiChainEffect->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillButtonActive::SetChainStep(int32 InCurrStep, int32 InMaxStep)
{
	int32 currIndex = InCurrStep - 1;
	if (0 >= InMaxStep)
	{
		_panelRootChain->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	_panelRootChain->SetVisibility(ESlateVisibility::HitTestInvisible);

	int32 maxIndex = InMaxStep - 1;
	for (int32 i = 0; i < _switcherChainList.Num(); ++i)
	{
		if (i > maxIndex)
		{
			_switcherChainList[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			_switcherChainList[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
			_switcherChainList[i]->SetActiveWidgetIndex((i == currIndex) ? 1 : 0);
		}
	}
}

bool UGsUISkillButtonActive::CheckCanDragDown(const FGsSkill* InData) const
{
	if (IsEmpty())
	{
		return false;
	}

	if (nullptr == InData)
	{
		return false;
	}

	if (MAX_COSTUME_SKILL_SLOT_ID == _slotId)
	{
		if (GSkill()->GetCurrentWeaponType() != InData->GetSkillRequireWeaponType())
		{
			return false;
		}
	}

	return true;
}

void UGsUISkillButtonActive::SetAutoUseInter(bool bInIsAuto)
{
	_effectAuto->SetVisibility(bInIsAuto ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_dragDownLogic.SetIsDown(bInIsAuto, false);
}

void UGsUISkillButtonActive::SetGrayscale(bool bInIsGray)
{
	_imgIcon->SetColorAndOpacity((bInIsGray) ?
		FLinearColor(_grayDarkness, _grayDarkness, _grayDarkness, _grayOpacity) : FLinearColor::White);

	if (UMaterialInstanceDynamic* mat = _imgIcon->GetDynamicMaterial())
	{
		mat->SetScalarParameterValue(TEXT("GrayDensity"), (bInIsGray) ? 1.f : 0.f);
	}
}

void UGsUISkillButtonActive::UseSkill()
{
	// 2022/11/04 PKT - 스킬 사용 동작 처리 안해야 할 때!!( 오토 / 버튼 비활성화 / 스킬 사용 대기중 )
	if (_bAutoChanged || false == _btnSkill->GetIsEnabled() || IsReserved())
	{
		return;
	}

	// 터치 시 무조건 이펙트 출력
	OnSkillStartEffect();

	if (_bIsFakePage)
	{
		return;
	}


	if (const FGsSkill* slotSkillData = GSkill()->FindSkillSlot(_slotId))
	{
		if (slotSkillData->_isSelectTarget == true)
		{
			// 1. already show
			// 2. same slot id
			//https://jira.com2us.com/jira/browse/CHR-15477
			// self heal

			if (GSkill()->GetIsShowSkillTargetSelect() == true &&
				GSkill()->GetCurrentSkillTargetSlotId() == _slotId)
			{
				UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
				if (localBase == nullptr)
				{
					return;
				}
				FGsTargetHandlerLocalPlayer* targetHandler = localBase->GetCastTarget<FGsTargetHandlerLocalPlayer>();
				if (targetHandler == nullptr)
				{
					return;
				}

				targetHandler->SetSelfSkillSelectTarget();

				if (_clickSelectTargetUseSkillVFXPath.IsValid())
				{
					UClass* vfxClass = GPath()->GetClassByObjectPath(_clickSelectTargetUseSkillVFXPath);
					if (vfxClass)
					{
						UGsBlueprintFunctionLibraryUI::AddVFXToPointerPos(vfxClass, _clickSelectTargetUseSkillVFXZOrder);
					}
				}
			}
			else
			{
				GSkill()->SetIsShowSkillTargetSelect(true, _slotId);
				UpdateEnableButton();
				return;
			}
		}
		else
		{
			// close skill target select
			GSkill()->SetIsShowSkillTargetSelect(false);
		}
	}

	// 일반 스킬은 클릭 시 발동 처리함
	_inputMsg.Id = _slotId;
	GMessage()->GetInput().SendMessage(MessageInput::ClickSkill, _inputMsg);
}

	

void UGsUISkillButtonActive::GetNameText(OUT FText& OutNameText)
{
	if (const FGsSkill* slotSkillData = GSkill()->FindSkillSlot(_slotId))
	{
		if (nullptr != slotSkillData->_tableData)
		{
			OutNameText = slotSkillData->_userSkillSetData->nameText;
			return;
		}
	}
	OutNameText = FText::GetEmpty();
}

ItemGrade UGsUISkillButtonActive::GetSkillGrade()
{
	if (const FGsSkill* slotSkillData = GSkill()->FindSkillSlot(_slotId))
	{
		if (nullptr != slotSkillData->_tableData)
		{
			return slotSkillData->_tableData->grade;
		}
	}
	return ItemGrade::NORMAL;
}
