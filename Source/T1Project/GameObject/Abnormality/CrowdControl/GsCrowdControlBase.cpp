// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCrowdControlBase.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "Data/GsResourceManager.h"

#include "DataSchema/Skill/GsSchemaSkillReaction.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityCrowdControlSet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalityAnimationPriority.h"

#include "Util/GsTableUtil.h"
#include "Animation/AnimMontage.h"
#include "Algo//ForEach.h"
#include "Algo/AnyOf.h"
#include "Util/GsEnum.h"
#include "DrawDebugHelpers.h"
#include "UTIL/GsDir.h"


ENABLE_ENUM_OPERATORS(EGsAbnormalityAnimProcessType)
TArray<FGsCrowdControlBase::AnimationProc> FGsCrowdControlBase::_animationPriority;

FGsCrowdControlBase::FGsCrowdControlBase()
{
}

const FGsAbnormalityReactInfo& FGsCrowdControlBase::GetReactionInfo() const
{
	return _reactionInfo;
}

UAnimMontage* FGsCrowdControlBase::GetPlayAni() const
{
	if (const auto reactionData = GetCurrentReactionData())
	{
		return GResource()->LoadSync<UAnimMontage>(reactionData->aniPath);
	}

	return nullptr;
}

void FGsCrowdControlBase::Initialize(UGsGameObjectBase* Owner)
{
	_owner = Owner;
	_reactionInfo.Clear();
	_arrReactionInfo.Reset();

	if (_animationPriority.Num() <= 0)
	{
		if (auto table = Cast<UGsTableAbnormalityAnimationPriority>(FGsSchemaAbnormalityAnimationPriority::GetStaticTable()))
		{
			TArray<const FGsSchemaAbnormalityAnimationPriority*> list;
			if (table->GetAllRows(list))
			{
				Algo::ForEach(list, [](const auto row) { _animationPriority.Add(AnimationProc{ row->prev, row->next, row->processType });});
			}
		}
	}

	//고정형 reaction
	TMap<EGsAbnormalityAnimProcessType, const FGsSchemaSkillReaction*> reactionData = {
		{EGsAbnormalityAnimProcessType::NONE, nullptr},
		{EGsAbnormalityAnimProcessType::STUNACT, GSkill()->GetReactionData(Owner, EReactionType::STUN)},
		{EGsAbnormalityAnimProcessType::KNOCKDOWN, GSkill()->GetReactionData(Owner, EReactionType::KNOCKDOWN)},
		{EGsAbnormalityAnimProcessType::KNOCKBACK, GSkill()->GetReactionData(Owner, EReactionType::KNOCKBACK)},
		//{EGsAbnormalityAnimProcessType::FearAct, GSkill()->GetReactionData(Owner, EReactionType::MOVELOCATION)}, commonAction
	};

	TMap<EGsAbnormalityAnimProcessType, const FGsSchemaSkillReaction*> polyReactionData;
	if (auto creature = _owner->CastGameObject<UGsGameObjectCreature>())
	{
		if (const FGsSchemaShapeData* shapeData = creature->GetPolymorphShapeData())
		{
			TArray<const FGsSchemaSkillReaction*> reactionSet;
			if (GSkill()->GetPolymorphShapeReactionSet(reactionSet, shapeData->id) > 0)
			{
				if (const auto iter = reactionSet.FindByPredicate([](const auto e) {return e->category == EReactionType::STUN;}))
					polyReactionData.Add(EGsAbnormalityAnimProcessType::STUNACT, *iter);
				if (const auto iter = reactionSet.FindByPredicate([](const auto e) {return e->category == EReactionType::KNOCKDOWN;}))
					polyReactionData.Add(EGsAbnormalityAnimProcessType::KNOCKDOWN, *iter);
				if (const auto iter = reactionSet.FindByPredicate([](const auto e) {return e->category == EReactionType::KNOCKBACK;}))
					polyReactionData.Add(EGsAbnormalityAnimProcessType::KNOCKBACK, *iter);
			}
		}
	}

	for (auto e : { EGsAbnormalityAnimProcessType::NONE,
					EGsAbnormalityAnimProcessType::STUNACT,
					EGsAbnormalityAnimProcessType::FEARACT,
					EGsAbnormalityAnimProcessType::KNOCKBACK,
					EGsAbnormalityAnimProcessType::KNOCKDOWN,
					EGsAbnormalityAnimProcessType::FRAMESTOP,
					EGsAbnormalityAnimProcessType::POLYMORPH, 
					EGsAbnormalityAnimProcessType::TAUNT,})
	{
		auto reactionDatum = reactionData.Find(e);
		auto polyReactionDatum = polyReactionData.Find(e);

		_arrReactionInfo.EmplaceAt(static_cast<int>(e), MakeShared<FGsAbnormalityAnimReactionInfo>(e,
			reactionDatum != nullptr ? *reactionDatum : nullptr,
			polyReactionDatum != nullptr ? *polyReactionDatum : nullptr, nullptr));
	}

	_arrReactionInfo[(int)EGsAbnormalityAnimProcessType::NONE]->_updateEvent = [this]() { };
	_arrReactionInfo[(int)EGsAbnormalityAnimProcessType::STUNACT]->_updateEvent = [this]() { UpdateStun();};
	_arrReactionInfo[(int)EGsAbnormalityAnimProcessType::FEARACT]->_updateEvent = [this]() { UpdateFear();};
	_arrReactionInfo[(int)EGsAbnormalityAnimProcessType::KNOCKBACK]->_updateEvent = [this]() { UpdateKnockBack();};
	_arrReactionInfo[(int)EGsAbnormalityAnimProcessType::KNOCKDOWN]->_updateEvent = [this]() { UpdateKnockDown();};
	_arrReactionInfo[(int)EGsAbnormalityAnimProcessType::FRAMESTOP]->_updateEvent = [this]() { UpdateFrameStop();};
	_arrReactionInfo[(int)EGsAbnormalityAnimProcessType::POLYMORPH]->_updateEvent = [this]() { UpdatePolymorp();};
	_arrReactionInfo[(int)EGsAbnormalityAnimProcessType::TAUNT]->_updateEvent = [this]() { UpdateTaunt(); };
}

void FGsCrowdControlBase::Finalize()
{
	_reactionInfo.Clear();
	_reactDataQueue.Reset();
}

void FGsCrowdControlBase::ChangeReactionState(EGsAbnormalityAnimProcessType animationType, bool isForced)
{
	static auto GetCurrentState = [](UGsGameObjectBase* owner) {
		if (owner)
		{
			if (FGsGameObjectStateManager* fsm = owner->GetBaseFSM())
			{
				return fsm->GetCurrentState();
			}
		}
		return EGsStateBase::None;
	};

	static auto CheckChangeStateType = [](EGsAbnormalityAnimProcessType animType, UGsGameObjectBase* owner = nullptr) {
		switch (animType)
		{
		case EGsAbnormalityAnimProcessType::FEARACT: 
			return EGsStateBase::Run;
		case EGsAbnormalityAnimProcessType::KNOCKBACK:
		case EGsAbnormalityAnimProcessType::KNOCKDOWN:
		case EGsAbnormalityAnimProcessType::FRAMESTOP:
		case EGsAbnormalityAnimProcessType::STUNACT:
			return EGsStateBase::Reaction;
		case EGsAbnormalityAnimProcessType::POLYMORPH:
			return GetCurrentState(owner);
		default: return EGsStateBase::None;
		}

		return EGsStateBase::None;
	};

	static auto ChangeState = [](EGsStateBase changeState, UGsGameObjectBase* owner, float delay = 0.f, bool forceCurStateEnd = false)
	{
		if (changeState == EGsStateBase::None || owner == nullptr) return;
		// 상태 전환 호출 
		if (FGsGameObjectStateManager* fsm = owner->GetBaseFSM())
		{
			auto currentState = fsm->CurrentState();

			if (forceCurStateEnd && currentState &&
				currentState->CanTransition(owner, static_cast<int>(changeState)) == false)
			{
				fsm->ProcessEvent(EGsStateBase::ReactionEnd);
			}

/*			if (delay > 0.f)
			{
				fsm->DelayProcessEvent(delay, changeState, changeState); //reactionEnd 가 우선 설정되어 있으면 delay가 동작안됨 실행 전에 바로 clear됨
			}
			else */ 
#if UE_BUILD_DEBUG
			if(changeState == EGsStateBase::Reaction)
			DrawDebugSphere(owner->GetWorld(), owner->GetLocation(), 10.f, 50.f, FColor::Magenta, false, 30.f);
#endif
			fsm->ProcessEvent(changeState);
		}
	};

	auto prevReactType = _reactionInfo._currentReaction.IsValid() ? _reactionInfo._currentReaction.Pin()->_type : EGsAbnormalityAnimProcessType::NONE;
	auto nextReactType = animationType;

	const auto reactData = _animationPriority.FindByPredicate([prevReactType, nextReactType](auto e) {
		return e._prev == prevReactType && e._next == nextReactType; });

	if (reactData == nullptr) return;

	GSLOG(Log, TEXT(" *********** ChangeReactionState [%d] -> [%d] : %d"), prevReactType, nextReactType, reactData->_processType);
#if UE_BUILD_DEBUG
	FString DebugText = FString::Printf(TEXT("ChangeReactionState [%d] -> [%d] : %d"), prevReactType, nextReactType, reactData->_processType);
	DrawDebugString(_owner->UObject::GetWorld(), _owner->GetLocation(), DebugText, nullptr, FColor::Magenta,  0.f, true);
#endif

	auto candidateAction = _arrReactionInfo[static_cast<int>(nextReactType)];

	switch (reactData->_processType)
	{
	case EGsAbnormalityProcessType::None:	// 처리 안함
	case EGsAbnormalityProcessType::Ignore:	// 후입 무시
		return ;
	case EGsAbnormalityProcessType::Retry:	// 현재 액션에 다시 진입 (count > 1) : 넉백
		{
			ChangeState(GetCurrentState(_owner), _owner);
		}
		return;
	case EGsAbnormalityProcessType::Post:	// 현재 리액션이 끝나고 다시 체크
		break;
	case EGsAbnormalityProcessType::Replay:	// 변이 후 현재 상태를 재실행 (polyReactionData)
		{
			ChangeState(CheckChangeStateType(reactData->_next, _owner), _owner, 0.1f, true);
		}
		return;
	case EGsAbnormalityProcessType::Remove: // 선입 제거, 후입 실행
		{
			_reactDataQueue.Remove(_reactionInfo._currentReaction);
			if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
			{
				fsm->ProcessEvent(EGsStateBase::ReactionEnd);
			}

			_reactionInfo.SetCurrentReaction(candidateAction);
			ChangeState(CheckChangeStateType(reactData->_next, _owner), _owner,
				isForced && reactData->_next == EGsAbnormalityAnimProcessType::FEARACT ? 0.02f : 0.f, true);
		}
		break;
	case EGsAbnormalityProcessType::Insert: // 선입 미루고, 후입 우선 실행
		{
			_reactionInfo.SetCurrentReaction(candidateAction);
			
			ChangeState(CheckChangeStateType(reactData->_next, _owner), _owner,
				isForced && reactData->_next == EGsAbnormalityAnimProcessType::FEARACT ? 0.02f : 0.f, true);
		}
		break;
	}

	_reactDataQueue.EmplaceBack(candidateAction);
}

void FGsCrowdControlBase::UpdateReactionControl(const TMap<AbnormalityInstanceId, TSharedPtr<FGsAbnormalityData>>& TotalData)
{
	EGsAbnormalityAnimProcessType currentReaction = _reactionInfo._currentReaction.IsValid() ?
		_reactionInfo._currentReaction.Pin()->_type : EGsAbnormalityAnimProcessType::NONE;

	auto prePolymorph = _arrReactionInfo[(int)EGsAbnormalityAnimProcessType::POLYMORPH]->IsValidData();
	auto GetLongestTime = [&TotalData](EGsAbnormalityAnimProcessType animType) //static 이면 안됨
	{
		float remainTime = 0.f;
		Algo::ForEachIf(TotalData, [animType](auto& pair) { return pair.Value->IsEnableAnimationProcessType(animType);
				}, [&remainTime](auto& pair) {
					remainTime = pair.Value->GetRemainTime() > remainTime ? pair.Value->GetRemainTime() : remainTime;
			});
		return remainTime;
	};

	for (auto e : _arrReactionInfo)
	{
		e->SetCurrentData(nullptr);
	}
	
	for (const auto& el : TotalData)
	{
		TWeakPtr<FGsAbnormalityData> data = el.Value;
		if(data.IsValid() == false) continue;
		GSLOG(Log, TEXT(" *********** UpdateReactionControl [%d]"), data.Pin()->GetTableId());

		TArray<EGsAbnormalityAnimProcessType> animList;
		el.Value->GetAnimationControlTypeList(animList);
		
		Algo::ForEach(animList, [this, data](auto animType) {

			int animIndex = static_cast<int>(animType);
			if (data.IsValid() && _arrReactionInfo.IsValidIndex(animIndex) && !_arrReactionInfo[animIndex]->IsValidData())
			{
				_arrReactionInfo[animIndex]->SetCurrentData(data.Pin()->GetAnimationControlByAnimType(animType));
			}
			});
	}

	EGsAbnormalityAnimProcessType changedReaction = IsValidCurrentReaction() ?
		_reactionInfo._currentReaction.Pin()->_type : EGsAbnormalityAnimProcessType::NONE;


	auto curPolymorph = _arrReactionInfo[(int)EGsAbnormalityAnimProcessType::POLYMORPH]->IsValidData();
	if (!curPolymorph && prePolymorph)
	{
		EndCurrentReactionState(currentReaction);
	}
	else if (currentReaction != EGsAbnormalityAnimProcessType::NONE && changedReaction == EGsAbnormalityAnimProcessType::NONE) //현재 우선 순위 action이 끝남
	{	
		EndCurrentReactionState(currentReaction);
	}
	else if(currentReaction == changedReaction)
	{
		if (const FGsSchemaSkillReaction* reactionTable = GetCurrentReactionData())
		{
			if (IsVariableLengthReactionType(reactionTable->category))
			{
				auto remainTime = GetLongestTime(currentReaction);
				SetVariableLengthReaction(remainTime, LoopReactionAnimationEndName); //넉다운, 넉다운 (계속 누워 있도록 한다)
			}
		}
	}

	for (auto e : _arrReactionInfo)
	{
		if (e->IsValidData())
		{
			e->_updateEvent();
		}
	}
}

//AbnormalityEffectSubType subType
const FGsSchemaSkillReaction* FGsCrowdControlBase::GetCurrentReactionData() const 
{
	if (auto reactionData = _reactionInfo._currentReaction.Pin())
	{
		return IsPolymorpShape() ? reactionData->_polyReactionData :
			reactionData->_overrideReactionData ? reactionData->_overrideReactionData : 
			reactionData->_reactionData;
	}

	return nullptr;
}

void FGsCrowdControlBase::EndCurrentReactionState(EGsAbnormalityAnimProcessType endReaction)
{
	GSLOG(Log, TEXT(" *********** EndCurrentReactionState [%d]"), endReaction);

	if (_reactionInfo.GetCurrentReactionProcessType() == endReaction)
	{
		_reactionInfo.SetCurrentReaction(nullptr); //현재 reaction이 끝남
	}

	FGsDequeue<TWeakPtr<FGsAbnormalityAnimReactionInfo>> emptyQueue;
	Swap(emptyQueue, _reactDataQueue);

	while (auto iter = emptyQueue.Peek())
	{
		if (iter->IsValid() && iter->Pin()->IsValidData())
		{
			ChangeReactionState(iter->Pin()->_data->param.animationTypeInfo, true);
		}
		emptyQueue.Pop();
	}

	if (IsValidCurrentReaction() == false)
	{
		if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
		{
			fsm->ProcessEvent(EGsStateBase::ReactionEnd);
		}
	}
}

void FGsCrowdControlBase::UpdateStun()
{	
}

//void FGsCrowdControlBase::UpdateBind() //bind
//{
//	// SC_BEHAVIOR_STATUS 패킷이 브로드 캐스팅이 아닌 관계로
//	// 타 유저 및 몬스터 처리를 할수가 없다.
//	// 일단은 예외적으로 AbnormalityEffectSubType 타입으로 처리
//	// @See : void UGsGameObjectLocalPlayer::NetBehaviorStatusAck(PD::SC::PKT_SC_BEHAVIOR_STATUS_READ* Packet)
//	if (UGsAnimInstanceState* anim = _owner->GetAnimInstance())
//	{
//		bool isEnable = _arrCrowdControlInfo[(int)AbnormalityEffectSubType::CC_BIND].IsValid();
//		ERootMotionMode::Type rootmotionType = (isEnable) ? ERootMotionMode::Type::IgnoreRootMotion :
//			ERootMotionMode::Type::RootMotionFromMontagesOnly;
//
//		anim->SetRootMotionMode(rootmotionType);
//	}
//}

void FGsCrowdControlBase::UpdateKnockDown()
{
// 	SetVariableLengthReaction(AbnormalityEffectSubType::CC_KNOCK_DOWN,
// 		GetReactionData(AbnormalityEffectSubType::CC_KNOCK_DOWN),
// 		TEXT("ReactionEnd"));
}

void FGsCrowdControlBase::UpdateKnockBack()
{
// 	SetVariableLengthReaction(AbnormalityEffectSubType::CC_KNOCK_BACK,
// 		GetReactionData(AbnormalityEffectSubType::CC_KNOCK_BACK),
// 		TEXT("ReactionEnd"));
}

void FGsCrowdControlBase::UpdateFear()
{
}

void FGsCrowdControlBase::UpdatePolymorp()
{
}

void FGsCrowdControlBase::UpdateFrameStop()
{
	// SC_BEHAVIOR_STATUS 패킷이 브로드 캐스팅이 아닌 관계로
	// 타 유저 및 몬스터 처리를 할수가 없다.
	// 일단은 예외적으로 AbnormalityEffectSubType 타입으로 처리
	// @See : void UGsGameObjectLocalPlayer::NetBehaviorStatusAck(PD::SC::PKT_SC_BEHAVIOR_STATUS_READ* Packet)
	if (UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>())
	{
		castObject->SetRootmotionLock(_arrReactionInfo[static_cast<int>(EGsAbnormalityAnimProcessType::FRAMESTOP)]->IsValidData());
	}
}

void FGsCrowdControlBase::UpdateTaunt()
{	
}

bool FGsCrowdControlBase::IsAnimationContollType(EGsAbnormalityAnimProcessType type)
{
	return type != EGsAbnormalityAnimProcessType::NONE;
}

bool FGsCrowdControlBase::IsValidAnimReaction(EGsAbnormalityAnimProcessType type)
{
	auto iType = static_cast<int>(type);
	if (IsAnimationContollType(type) && _arrReactionInfo.IsValidIndex(iType))
	{
		return _arrReactionInfo[iType]->IsValidData();
	}
	return false;
}

//bool FGsCrowdControlBase::IsCrowdControlType(AbnormalityEffectSubType Type)
//{
//	switch (Type)
//	{
//	case AbnormalityEffectSubType::CC_SILENCE:	
//	case AbnormalityEffectSubType::CC_BIND:	
//	case AbnormalityEffectSubType::CC_STUN:
//	case AbnormalityEffectSubType::CC_KNOCK_DOWN:
//	case AbnormalityEffectSubType::CC_KNOCK_BACK:
//	case AbnormalityEffectSubType::CC_FEAR:
//	case AbnormalityEffectSubType::CC_MOVE_LOCATION:
//	case AbnormalityEffectSubType::CC_POLYMORP:
//	case AbnormalityEffectSubType::CC_FREEZE:
//	case AbnormalityEffectSubType::CC_COCOON:
//	case AbnormalityEffectSubType::CC_ROOT_WINDING:
//	case AbnormalityEffectSubType::CC_CANNOT_WARP:
//	case AbnormalityEffectSubType::CC_BURY:
//	case AbnormalityEffectSubType::CC_TAUNT:
//		return true;
//	}
//	return false;
//}

bool FGsCrowdControlBase::IsAnyOfValidAnimReaction()
{
	return Algo::AnyOf(_arrReactionInfo, [](auto e) { return e->IsValidData(); });
}

bool FGsCrowdControlBase::IsVariableLengthReactionType(EReactionType Type)
{
	switch (Type)
	{	
	case EReactionType::KNOCKDOWN:
	//case EReactionType::KNOCKBACK:
		return true;
	}
	return false;
}

void FGsCrowdControlBase::SetVariableLengthReaction(float VariableLength, const FName& SectionName)
{
	if (const auto reactionData = GetCurrentReactionData())
	{
		if (IsVariableLengthReactionType(reactionData->category))
		{
			ClearVariableLengthReaction();

			_reactionInfo._variableLengthInfo._endSectionName = SectionName;

			// 루프 구간찾음
			UAnimMontage* reactionAni = GetPlayAni();
			if (nullptr == reactionAni)
			{
				GSLOG(Error, TEXT("not found Reaction AnimMontage Data reactionDataId = %d"), reactionData->id);
				return;
			}
			int sectionIdx = reactionAni->GetSectionIndex(_reactionInfo._variableLengthInfo._endSectionName);
			if (sectionIdx < 0)
			{
				_reactionInfo._variableLengthInfo._endSectionName = NAME_None;
			}
			else
			{
				float aniLen = reactionAni->SequenceLength;
				float endPostion = aniLen - reactionAni->GetSectionLength(sectionIdx);
				float variableLength = VariableLength - aniLen;

				_reactionInfo._variableLengthInfo._jumpToEndTime = endPostion + variableLength;
				_reactionInfo._variableLengthInfo._timer = FDateTime::Now();
			}
		}
	}
}

void FGsCrowdControlBase::SetVariableLengthReaction(const FGsSchemaSkillReaction* ReactionData, const FName& SectionName)
{
	if (nullptr == ReactionData)
	{
		return;
	}

	SetVariableLengthReaction(ReactionData->totalRuntime, SectionName);
}

void FGsCrowdControlBase::ClearVariableLengthReaction()
{
	_reactionInfo._variableLengthInfo._endSectionName = NAME_None;
	_reactionInfo._variableLengthInfo._animationPlayeRate = 1.f;
}

const FGsSchemaAbnormalityCrowdControlSet* FGsCrowdControlBase::GetPriorValidAbnormality()
{
	if (IsValidCurrentReaction())
	{
		return _reactionInfo._currentReaction.Pin()->_data;
	}

	return nullptr;
}

bool FGsCrowdControlBase::IsPolymorpShape() const
{
	if (_owner->IsObjectType(EGsGameObjectType::Player))
	{
		if (UGsGameObjectPlayer* playerObject = _owner->CastGameObject<UGsGameObjectPlayer>())
		{
			return playerObject->IsTransPolymorph();
		}
	}
	return false;
}

bool FGsCrowdControlBase::IsPoseAnimation() const
{	
	return _reactionInfo._currentReaction.IsValid() && _reactionInfo._currentReaction.Pin()->IsValidData() &&
		   _reactionInfo._currentReaction.Pin()->_type == EGsAbnormalityAnimProcessType::FRAMESTOP;

}
