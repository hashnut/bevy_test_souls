// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITraySkillEnchantWait.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Tray/SkillEnchant/GsUISkillEnchantProgress.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Skill/Cost/GsSchemaSkillUpCostSet.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillHelper.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"
#include "T1Project.h"


void UGsUITraySkillEnchantWait::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITraySkillEnchantWait::OnClickConfirm);
	_progressUI->OnProgressTimeFinished.BindUObject(this, &UGsUITraySkillEnchantWait::OnProgressTimeFinished);

	_checkerEnchantLock.SetIntervalSecond(2.f);
}

void UGsUITraySkillEnchantWait::NativeConstruct()
{
	Super::NativeConstruct();

	_panelWaitUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_progressUI->SetVisibility(ESlateVisibility::Collapsed);

	// 시작 사운드 재생
	PlayEffectSound(TEXT("Enchant_Start"));
}

void UGsUITraySkillEnchantWait::NativeDestruct()
{
	StopEffectSound();

	Super::NativeDestruct();
}

void UGsUITraySkillEnchantWait::SetData(SkillId InSkillId)
{
	_skillId = InSkillId;
}

bool UGsUITraySkillEnchantWait::IsProgressState() const
{
	return _progressUI->IsVisible() ? true : false;
}

void UGsUITraySkillEnchantWait::OnClickConfirm()
{
	// 광클 방지
	if (false == _checkerEnchantLock.IsTimeOver())
	{
		return;
	}

	// 재접속 상황에서 보내지 않도록 블럭
	if (nullptr == GNet()->GetActive())
	{
		return;
	}

	// 재료 아이템 정보 수집
	TArray<ItemDBIdAmountPair> costItemList;
	if (false == GetSkillUpCostList(costItemList))
	{
		return;
	}

	// 서버에 스킬 강화 요청
	const FGsSkill* skillData = GSkill()->FindSkill(_skillId, CreatureWeaponType::MAX, true);
	if (nullptr == skillData)
	{
		return;
	}

	SkillId rootSkillId = FGsSkillHelper::GetUpgradeRootSkillId(skillData);
	if (INVALID_SKILL_ID == rootSkillId)
	{
		rootSkillId = _skillId;
	}

	FGsNetSendServiceWorld::SendRequestSkillEnchant(rootSkillId, _skillId, costItemList);

	// 진행 사운드 재생	
	PlayEffectSound(TEXT("SkillEnchant_Process"));

	// 강화 진행 상태로 넘어감
	_progressUI->StartProgress();
	_progressUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_panelWaitUI->SetVisibility(ESlateVisibility::Collapsed);

	// 광클 방지용 시간 갱신
	_checkerEnchantLock.UpdateTime();
}

void UGsUITraySkillEnchantWait::OnProgressTimeFinished()
{
	// 연출이 끝나고 결과창 열 수 있도록 메시지를 보낸다
	GMessage()->GetUI().SendMessage(MessageUI::SKILL_ENCHANT_EFFECT_END_PROGRESS, nullptr);
}

void UGsUITraySkillEnchantWait::PlayEffectSound(const FString& InName)
{
	// 이전 재생 중인 사운드가 있다면 제거
	StopEffectSound();

	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		_soundId = soundPlayer->PlaySound2D(InName);
	}
}

void UGsUITraySkillEnchantWait::StopEffectSound()
{
	// 재생 중이던 사운드 중지
	if (0 != _soundId)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			if (soundPlayer->IsPlaying(_soundId))
			{
				soundPlayer->StopSound(_soundId);
			}
		}

		_soundId = 0;
	}
}

bool UGsUITraySkillEnchantWait::GetSkillUpCostList(OUT TArray<ItemDBIdAmountPair>& OutList)
{
	const FGsSkill* skill = GSkill()->FindSkill(_skillId, CreatureWeaponType::MAX, true);
	if (nullptr == skill)
	{
		GSLOG(Error, TEXT("[SkillEnchant] Cannot find FGsSkill. skillIId: %d"), _skillId);
		return false;
	}

	// 업그레이드 스킬은 rootSkill의 값을 따름, level 공유
	const FGsSkill* rootSkill = nullptr;
	TWeakPtr<const FGsSkill> rootSkillPtr = FGsSkillHelper::GetUpgradeRootSkill(skill);
	if (rootSkillPtr.IsValid())
	{
		rootSkill = rootSkillPtr.Pin().Get();
	}
	else
	{
		rootSkill = skill;
	}

	const FGsSchemaSkillUpCostSet* costTable = nullptr;
	if (const FGsSchemaSkillLevelInfo* levelInfo = rootSkill->GetLevelInfo(skill->GetLevel()))
	{
		costTable = FGsSkillHelper::GetSkillUpCostSet(levelInfo->enchantSkillUpCostId);
	}

	if (nullptr == costTable)
	{
		GSLOG(Error, TEXT("[SkillEnchant] Cannot find SkillUpCostSet. skillIId: %d"), _skillId);
		return false;
	}

	UGsItemManager* itemMgr = GItem();
	TMap<ItemDBId, ItemAmount> countMap;

	// 재료 아이템 정보 수집(여러 뭉터기일 경우에 전부 보내야함. 적은 순으로 
	for (const FGsSchemaSkillUpItemInfo& costInfo : costTable->itemList)
	{
		// 전부 가져옴
		TArray<TWeakPtr<FGsItem>> itemList = itemMgr->FindByTID(costInfo.itemId);

		// 수가 적은 순으로 정렬
		itemList.Sort([](const TWeakPtr<FGsItem>& A, const TWeakPtr<FGsItem>& B)
			{
				return (A.Pin()->GetAmount() <= B.Pin()->GetAmount()) ? true : false;
			});
		
		int64 needCount = costInfo.itemCount;

		for (int i = 0; i < itemList.Num(); ++i)
		{
			if (false == itemList[i].IsValid())
			{
				continue;
			}

			const FGsItem* item = itemList[i].Pin().Get();
			if (nullptr == item)
			{
				continue;
			}

			// 락 걸린 것은 소모하지 않음
			if (item->GetLock())
			{
				continue;
			}
			
			if (0 == needCount)
			{
				break;
			}
			else if (needCount > item->GetAmount())
			{
				if (countMap.Contains(item->GetDBID()))
				{
					countMap[item->GetDBID()] += item->GetAmount();
				}
				else
				{
					countMap.Emplace(item->GetDBID(), item->GetAmount());
				}

				needCount -= item->GetAmount();
			}
			else
			{
				if (countMap.Contains(item->GetDBID()))
				{
					countMap[item->GetDBID()] += needCount;
				}
				else
				{
					countMap.Emplace(item->GetDBID(), needCount);
				}

				needCount = 0;
				break;
			}
		}
	}

	for (TPair<ItemDBId, ItemAmount>& iter : countMap)
	{
		OutList.Emplace(ItemDBIdAmountPair(iter.Key, iter.Value));
	}

	return true;
}
