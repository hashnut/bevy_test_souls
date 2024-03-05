// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSectionData.h"
#include "SkillNotify/CutremineSkillNotify.h"
#include "SkillNotify/HitSkillNotify.h"
#include "SkillNotify/ProjectileSkillNotify.h"
#include "SkillNotify/GuideEffectSkillNotify.h"
//#include "SkillNotify/CollisionSkillNotify.h"
#include "SkillEditorViewportClient.h"

#include "Data/SkillEditorDataAsset.h"
#include "GsTableManagerEditor.h"
#include "Misc/MessageDialog.h"

#if WITH_EDITOR
void USkillSectionData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);
		if (PropertyName == TEXT("cooldownTime"))
		{
			if (SkillSetData.cooldownTime <= FLT_EPSILON)
				SkillSetData.cooldownTime = 0.1f;
		}
		else
		if (PropertyName == TEXT("targetCount"))
		{
			if (SkillSetData.targetFilterInfo.targetCount <= 0)
				SkillSetData.targetFilterInfo.targetCount = 1;
		}
		else
		if (PropertyName == TEXT("id"))
		{
			// 중복 체크
			UGsTableManagerEditor& Dman = UGsTableManagerEditor::GetInstance();
			UGsTable* SkillSetTable = Dman.GetTable(FGsSchemaSkillSet::StaticStruct());
			const FGsSchemaSkillSet* row = nullptr;
			if (SkillSetTable->FindRow(FName(*FString::FromInt(SkillSetData.id)), row))
			{
				//FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillSectionData", "Failed id Setting", "SkillSetData id 가 SkillSet 테이블에 이미 있습니다.(중복)"));
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("SkillEditor", "Failed id Setting", "SkillEditor::SkillSectionData - The SkillSetData.id is a duplicate."));
				//SkillSetData.id = 0;
			}

			if (SkillEditorDataAsset)
				SkillEditorDataAsset->SkillSectionDataNextIdSet();
		}
		else
		if (PropertyName == TEXT("category"))
		{			
			if (SkillEditorDataAsset)
				SkillEditorDataAsset->ChangeSectionCategory(static_cast<int32>(SkillSetData.category));
		}
		else
		if (PropertyName == TEXT("useSkillNotifyCollision"))
		{
			//if (SkillSetData.useSkillNotifyCollision == false)
			{
				if (SkillEditorDataAsset)
				{
					SkillEditorDataAsset->SkillSectionDataSet();
					SkillEditorDataAsset->SetSkillUesNotifyCollisionDraw(drawIndex, SkillSetData.useSkillNotifyCollision);
				}
			}
		}

		// 실시간 반영이라 항시
		DrawSet();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void USkillSectionData::DrawSet()
{
//#if defined(GAMEVIL_ENGINE_SKILL_EDITOR_MODIFY1_BSAM)
	// 한개아 아니고 여러개가 줄어들수도 늘어날수도 있으니 초기화도 같이 해야겠네...
	if (true == SkillSetData.useSkillNotifyCollision)
	{
		TArray<DrawTools::FDrawData*>& DrawDatas = FSkillEditorViewportClient::DrawManager.GetDrawDatas(drawIndex);
		for (int32 i = 0; i < DrawDatas.Num(); ++i)
		{
			delete DrawDatas[i];
			DrawDatas.Reset();
		}

		return;
	}

	TArray<DrawTools::FDrawData*>& DrawDatas = FSkillEditorViewportClient::DrawManager.GetDrawDatas(drawIndex);
	for (int32 i = 0; i < DrawDatas.Num(); ++i)
	{
		delete DrawDatas[i];
		DrawDatas.Reset();
	}

	DrawDatas.AddDefaulted(SkillSetData.collisionInfoList.Num());
	for (int32 i = 0; i < SkillSetData.collisionInfoList.Num(); ++i)
	{
		DrawDatas[i] = new DrawTools::FDrawData();
		DrawDatas[i]->CollisionDraw = CollisionDraw;
		if (SkillCollisionType::CIRCLE == SkillSetData.collisionInfoList[i].type)
			DrawDatas[i]->DrawType = DrawTools::EDrawType::Circle;
		else
		if (SkillCollisionType::RECT == SkillSetData.collisionInfoList[i].type)
			DrawDatas[i]->DrawType = DrawTools::EDrawType::Rect;
		else
		if (SkillCollisionType::FAN == SkillSetData.collisionInfoList[i].type)
			DrawDatas[i]->DrawType = DrawTools::EDrawType::Cone;

		DrawDatas[i]->TM = FTransform::Identity;

		DrawDatas[i]->PosOffset = SkillSetData.collisionInfoList[i].posOffset;
		DrawDatas[i]->OffsetX = SkillSetData.collisionInfoList[i].offsetX;
		DrawDatas[i]->OffsetY = SkillSetData.collisionInfoList[i].offsetY;
		DrawDatas[i]->AngleOffset = SkillSetData.collisionInfoList[i].angleOffset;
		DrawDatas[i]->AngleOffset2 = SkillSetData.collisionInfoList[i].angleOffset2;
		DrawDatas[i]->Width = SkillSetData.collisionInfoList[i].width;
		DrawDatas[i]->Height = SkillSetData.collisionInfoList[i].height;
		DrawDatas[i]->MaxRadius = SkillSetData.collisionInfoList[i].maxRadius;
		DrawDatas[i]->MinRadius = SkillSetData.collisionInfoList[i].minRadius;
		DrawDatas[i]->DegAngle = SkillSetData.collisionInfoList[i].angle;
		
		DrawDatas[i]->LifeTime = EndTime - StartTime;
		DrawDatas[i]->StartTime = StartTime;
		DrawDatas[i]->EndTime = EndTime;		
		DrawDatas[i]->Color = FColor(192, 255, 99, 128); //NotifyColor;
		DrawDatas[i]->FillMode = true;
	}
}

void USkillSectionData::CopyData(USkillMetaData* InSkillMetaData)
{
	USkillSectionData* BaseData = Cast<USkillSectionData>(InSkillMetaData);
	if (BaseData == nullptr) return;

	SkillSetData = BaseData->SkillSetData;

	//if (SkillEditorDataAsset)
	//{
	//	for (int32 i = 0; i < SkillEditorDataAsset->SkillSetJson.Num(); ++i)
	//	{
	//		// json 에도 같은게 잇다
	//		if (SkillEditorDataAsset->SkillSetJson[i].id == SkillSetData.id)
	//		{
	//			for (int32 j = 0; j < SkillSetData.notifyInfoList.Num(); ++j)
	//			{
	//				if (j < SkillEditorDataAsset->SkillSetJson[i].notifyInfoList.Num())
	//				{
	//					SkillSetData.notifyInfoList[j].skillNotifyId.RowName = FName(*FString::FromInt(SkillEditorDataAsset->SkillSetJson[i].notifyInfoList[j]));
	//				}
	//			}

	//			break;
	//		}
	//	}		
	//}
}

void USkillSectionData::NotifiesChanged(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex)
{
	// 원본에서 필요한 정보를 가져온다.
	//UAnimMontage* AnimMontage = Cast<UAnimMontage>(InSkillEditorDataAsset->AnimMontage);
	//if (AnimMontage == nullptr) return;

	//SkillSetData.aniPath = AnimMontage->GetPathName();
	//SkillSetData.totalRuntime = AnimMontage->GetSectionLength(InSectionIndex);

	//// 기존에 있던 스킬 노티파이를 리셋 하고 현재 섹션길아 안에 포함된것만 넣는다.	
	//AnimMontage->GetSectionStartAndEndTime(InSectionIndex, StartTime, EndTime);

	//// 스킬 노티파이 이벤트를 가져온다.
	//TArray< FAnimNotifyEvent* > SkillNotifyEvent;
	//for (int32 index = 0; index < AnimMontage->GetNotifies().Num(); ++index)
	//{
	//	if (AnimMontage->GetNotifies()[index].SkillNotify != nullptr)
	//		SkillNotifyEvent.Add(&(AnimMontage->GetNotifies()[index]));
	//}	

	// 해당 섹션안에 있는 스킬 노티파이만 추린다.
	SkillSetData.notifyInfoList.Reset(); // Server용
	SkillSetData.clientNotifyInfoList.Reset(); // Client용
	TArray< USkillNotify*> SkillNotifys;

	if (InSkillEditorDataAsset->IsDivide() == false)
	{
		for (int32 index = 0; index < InSkillEditorDataAsset->SkillNotifies.Num(); ++index)
		{
			if (InSkillEditorDataAsset->SkillNotifies[index].SkillNotify != nullptr)
			{
				InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime;

				UCutremineSkillNotify* CutremineSkillNotify = Cast< UCutremineSkillNotify >(InSkillEditorDataAsset->SkillNotifies[index].SkillNotify);
				UHitSkillNotify* HitSkillNotify = Cast< UHitSkillNotify >(InSkillEditorDataAsset->SkillNotifies[index].SkillNotify);
				UProjectileSkillNotify* ProjectileSkillNotify = Cast< UProjectileSkillNotify >(InSkillEditorDataAsset->SkillNotifies[index].SkillNotify);
				UGuideEffectSkillNotify* GuideEffectSkillNotify = Cast< UGuideEffectSkillNotify >(InSkillEditorDataAsset->SkillNotifies[index].SkillNotify);

				if (CutremineSkillNotify)
				{
					CutremineSkillNotify->NotifyCutremineInfo.delayOffset = InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime;
					SkillSetData.cutRemainInfo = CutremineSkillNotify->NotifyCutremineInfo;
				}
				else
				{
					if (HitSkillNotify)
					{
						if (HitSkillNotify->SkillNotifySet.RowName == FName("None"))
						{
							// [B.Y]테이블 타입을 커스텀으로 지정할수 있게 변경되어서 강제로 테이블 설정을 하면 안된다.
							// HitSkillNotify->SkillNotifySet.DataTable = FGsSchemaSkillNotifySet::GetStaticTable()->GetCachedDataTable();
						}
						else
						{
							// 서버용
							const FGsSchemaSkillNotifySet* schemaRewardData = HitSkillNotify->SkillNotifySet.GetRow();
							if (schemaRewardData == nullptr)
								continue;

							int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

							SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = HitSkillNotify->SkillNotifySet.DataTable;
							SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = HitSkillNotify->SkillNotifySet.RowName;

							SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
							InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;;
							SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;

							// 클라용
							// 2022/05/11 PKT - 기획 요청으로 "GsSchemaSkillNotifyEffect" 삭제 이슈로 해당 Enum값 주석 처리함.
							//if (HitSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILLGUIDE_EFFECT
							//	//|| HitSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILL_EFFECT
							//	|| HitSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::BEAM_PARTICLE_EFFECT)
							//{
							//	int32 addIndex = SkillSetData.clientNotifyInfoList.AddDefaulted();

							//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.DataTable = HitSkillNotify->SkillNotifySet.DataTable;
							//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.RowName = HitSkillNotify->SkillNotifySet.RowName;

							//	SkillSetData.clientNotifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
							//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
							//	SkillSetData.clientNotifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
							//}
							//// 서버용
							//else
							//{
							//	int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

							//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = HitSkillNotify->SkillNotifySet.DataTable;
							//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = HitSkillNotify->SkillNotifySet.RowName;

							//	SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
							//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;;
							//	SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
							//}
						}
					}
					else
						if (ProjectileSkillNotify)
						{
							if (ProjectileSkillNotify->SkillNotifySet.RowName == FName("None"))
							{
								// [B.Y]테이블 타입을 커스텀으로 지정할수 있게 변경되어서 강제로 테이블 설정을 하면 안된다.
								// ProjectileSkillNotify->SkillNotifySet.DataTable = FGsSchemaSkillNotifySet::GetStaticTable()->GetCachedDataTable();
							}
							else
							{
								const FGsSchemaSkillNotifySet* schemaRewardData = ProjectileSkillNotify->SkillNotifySet.GetRow();
								if (schemaRewardData == nullptr)
									continue;

								int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

								SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = ProjectileSkillNotify->SkillNotifySet.DataTable;
								SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = ProjectileSkillNotify->SkillNotifySet.RowName;

								SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;

								// 클라용
								// 2022/05/11 PKT - 기획 요청으로 "GsSchemaSkillNotifyEffect" 삭제 이슈로 해당 Enum값 주석 처리함.
								//if (ProjectileSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILLGUIDE_EFFECT
								//	//|| ProjectileSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILL_EFFECT
								//	|| ProjectileSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::BEAM_PARTICLE_EFFECT)
								//{
								//	int32 addIndex = SkillSetData.clientNotifyInfoList.AddDefaulted();

								//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.DataTable = ProjectileSkillNotify->SkillNotifySet.DataTable;
								//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.RowName = ProjectileSkillNotify->SkillNotifySet.RowName;

								//	SkillSetData.clientNotifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								//	SkillSetData.clientNotifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
								//}
								//// 서버용
								//else
								//{
								//	int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

								//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = ProjectileSkillNotify->SkillNotifySet.DataTable;
								//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = ProjectileSkillNotify->SkillNotifySet.RowName;

								//	SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								//	SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
								//}
							}
						}
						else
							if (GuideEffectSkillNotify)
							{
								if (GuideEffectSkillNotify->SkillNotifySet.RowName == FName("None"))
								{
									// [B.Y]테이블 타입을 커스텀으로 지정할수 있게 변경되어서 강제로 테이블 설정을 하면 안된다.
									// GuideEffectSkillNotify->SkillNotifySet.DataTable = FGsSchemaSkillNotifySet::GetStaticTable()->GetCachedDataTable();
								}
								else
								{
									const FGsSchemaSkillNotifySet* schemaRewardData = GuideEffectSkillNotify->SkillNotifySet.GetRow();
									if (schemaRewardData == nullptr)
										continue;

									int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

									SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = GuideEffectSkillNotify->SkillNotifySet.DataTable;
									SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = GuideEffectSkillNotify->SkillNotifySet.RowName;

									SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
									InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
									SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;

									// 클라용
									// 2022/05/11 PKT - 기획 요청으로 "GsSchemaSkillNotifyEffect" 삭제 이슈로 해당 Enum값 주석 처리함.
									//if (GuideEffectSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILLGUIDE_EFFECT
									//	//|| GuideEffectSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILL_EFFECT
									//	|| GuideEffectSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::BEAM_PARTICLE_EFFECT)
									//{
									//	int32 addIndex = SkillSetData.clientNotifyInfoList.AddDefaulted();

									//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.DataTable = GuideEffectSkillNotify->SkillNotifySet.DataTable;
									//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.RowName = GuideEffectSkillNotify->SkillNotifySet.RowName;

									//	SkillSetData.clientNotifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
									//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
									//	SkillSetData.clientNotifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
									//}
									//// 서버용
									//else
									//{
									//	int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

									//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = GuideEffectSkillNotify->SkillNotifySet.DataTable;
									//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = GuideEffectSkillNotify->SkillNotifySet.RowName;

									//	SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
									//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
									//	SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
									//}
								}
							}
				}

			}
		}
	}
	else
	{
		for (int32 index = 0; index < InSkillEditorDataAsset->SkillNotifies.Num(); ++index)
		{
			if ((StartTime <= InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue())
				&& (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() < EndTime))
			{
				if (InSkillEditorDataAsset->SkillNotifies[index].SkillNotify != nullptr)
				{
					InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime;

					UCutremineSkillNotify* CutremineSkillNotify = Cast< UCutremineSkillNotify >(InSkillEditorDataAsset->SkillNotifies[index].SkillNotify);
					UHitSkillNotify* HitSkillNotify = Cast< UHitSkillNotify >(InSkillEditorDataAsset->SkillNotifies[index].SkillNotify);
					UProjectileSkillNotify* ProjectileSkillNotify = Cast< UProjectileSkillNotify >(InSkillEditorDataAsset->SkillNotifies[index].SkillNotify);
					UGuideEffectSkillNotify* GuideEffectSkillNotify = Cast< UGuideEffectSkillNotify >(InSkillEditorDataAsset->SkillNotifies[index].SkillNotify);

					if (CutremineSkillNotify)
					{
						CutremineSkillNotify->NotifyCutremineInfo.delayOffset = InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime;
						SkillSetData.cutRemainInfo = CutremineSkillNotify->NotifyCutremineInfo;
					}
					else
					{
						if (HitSkillNotify)
						{
							if (HitSkillNotify->SkillNotifySet.RowName == FName("None"))
							{
								// [B.Y]테이블 타입을 커스텀으로 지정할수 있게 변경되어서 강제로 테이블 설정을 하면 안된다.
								// HitSkillNotify->SkillNotifySet.DataTable = FGsSchemaSkillNotifySet::GetStaticTable()->GetCachedDataTable();
							}
							else
							{
								// 서버용
								const FGsSchemaSkillNotifySet* schemaRewardData = HitSkillNotify->SkillNotifySet.GetRow();
								if(schemaRewardData == nullptr)
									continue;

								int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

								SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = HitSkillNotify->SkillNotifySet.DataTable;
								SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = HitSkillNotify->SkillNotifySet.RowName;

								SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;;
								SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;

								// 클라용
								//if (HitSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILLGUIDE_EFFECT
								//	//|| HitSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILL_EFFECT
								//	|| HitSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::BEAM_PARTICLE_EFFECT)
								//{
								//	int32 addIndex = SkillSetData.clientNotifyInfoList.AddDefaulted();

								//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.DataTable = HitSkillNotify->SkillNotifySet.DataTable;
								//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.RowName = HitSkillNotify->SkillNotifySet.RowName;

								//	SkillSetData.clientNotifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								//	SkillSetData.clientNotifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
								//}
								//// 서버용
								//else
								//{
								//	int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

								//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = HitSkillNotify->SkillNotifySet.DataTable;
								//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = HitSkillNotify->SkillNotifySet.RowName;

								//	SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;;
								//	SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
								//}
							}						
						}
						else
						if (ProjectileSkillNotify)
						{
							if (ProjectileSkillNotify->SkillNotifySet.RowName == FName("None"))
							{
								// [B.Y]테이블 타입을 커스텀으로 지정할수 있게 변경되어서 강제로 테이블 설정을 하면 안된다.
								// ProjectileSkillNotify->SkillNotifySet.DataTable = FGsSchemaSkillNotifySet::GetStaticTable()->GetCachedDataTable();
							}
							else
							{							
								const FGsSchemaSkillNotifySet* schemaRewardData = ProjectileSkillNotify->SkillNotifySet.GetRow();
								if (schemaRewardData == nullptr)
									continue;

								int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

								SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = ProjectileSkillNotify->SkillNotifySet.DataTable;
								SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = ProjectileSkillNotify->SkillNotifySet.RowName;

								SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;

								// 클라용
								// 2022/05/11 PKT - 기획 요청으로 "GsSchemaSkillNotifyEffect" 삭제 이슈로 해당 Enum값 주석 처리함.
								//if (ProjectileSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILLGUIDE_EFFECT
								//	//|| ProjectileSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILL_EFFECT
								//	|| ProjectileSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::BEAM_PARTICLE_EFFECT)
								//{
								//	int32 addIndex = SkillSetData.clientNotifyInfoList.AddDefaulted();

								//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.DataTable = ProjectileSkillNotify->SkillNotifySet.DataTable;
								//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.RowName = ProjectileSkillNotify->SkillNotifySet.RowName;

								//	SkillSetData.clientNotifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								//	SkillSetData.clientNotifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
								//}
								//// 서버용
								//else
								//{
								//	int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

								//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = ProjectileSkillNotify->SkillNotifySet.DataTable;
								//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = ProjectileSkillNotify->SkillNotifySet.RowName;

								//	SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								//	SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
								//}
							}
						}
						else
						if (GuideEffectSkillNotify)
						{
							if (GuideEffectSkillNotify->SkillNotifySet.RowName == FName("None"))
							{
								// [B.Y]테이블 타입을 커스텀으로 지정할수 있게 변경되어서 강제로 테이블 설정을 하면 안된다.
								// GuideEffectSkillNotify->SkillNotifySet.DataTable = FGsSchemaSkillNotifySet::GetStaticTable()->GetCachedDataTable();
							}
							else
							{							
								const FGsSchemaSkillNotifySet* schemaRewardData = GuideEffectSkillNotify->SkillNotifySet.GetRow();
								if (schemaRewardData == nullptr)
									continue;

								int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

								SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = GuideEffectSkillNotify->SkillNotifySet.DataTable;
								SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = GuideEffectSkillNotify->SkillNotifySet.RowName;

								SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;

								// 클라용
								// 2022/05/11 PKT - 기획 요청으로 "GsSchemaSkillNotifyEffect" 삭제 이슈로 해당 Enum값 주석 처리함.
								//if (GuideEffectSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILLGUIDE_EFFECT
								//	//|| GuideEffectSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::SKILL_EFFECT
								//	|| GuideEffectSkillNotify->SkillNotifySet.GetRow()->type == SkillNotifyType::BEAM_PARTICLE_EFFECT)
								//{
								//	int32 addIndex = SkillSetData.clientNotifyInfoList.AddDefaulted();

								//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.DataTable = GuideEffectSkillNotify->SkillNotifySet.DataTable;
								//	SkillSetData.clientNotifyInfoList[addIndex].skillNotifyId.RowName = GuideEffectSkillNotify->SkillNotifySet.RowName;

								//	SkillSetData.clientNotifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								//	SkillSetData.clientNotifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
								//}
								//// 서버용
								//else
								//{
								//	int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

								//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.DataTable = GuideEffectSkillNotify->SkillNotifySet.DataTable;
								//	SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = GuideEffectSkillNotify->SkillNotifySet.RowName;

								//	SkillSetData.notifyInfoList[addIndex].repeatCount = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->RepeatCount;
								//	InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset = (InSkillEditorDataAsset->SkillNotifies[index].GetLinkValue() - StartTime) * InSkillEditorDataAsset->RateScale;
								//	SkillSetData.notifyInfoList[addIndex].offset = InSkillEditorDataAsset->SkillNotifies[index].SkillNotify->Offset;
								//}
							}
						}
					}

				}
			}
		}
	}
}

void USkillSectionData::SetData(USkillEditorDataAsset* InSkillEditorDataAsset, int32 InSectionIndex)
{	
	SkillEditorDataAsset = InSkillEditorDataAsset;
	drawIndex = InSectionIndex;

	const FString& AssetName = FPackageName::GetShortName(*InSkillEditorDataAsset->GetOutermost()->GetPathName());
	//SkillSetData.nameText = FName(*AssetName);
	SkillSetData.nameText = AssetName;

	SkillSetData.category = InSkillEditorDataAsset->SkillCategory;

	// 원본에서 필요한 정보를 가져온다.
	UAnimMontage* AnimMontage = Cast<UAnimMontage>(InSkillEditorDataAsset->AnimMontage);
	if (AnimMontage != nullptr)
	{
		SkillSetData.aniPath = AnimMontage->GetPathName();

		if (InSkillEditorDataAsset->AutoTotalRuntimeSetting)
		{
			if (InSkillEditorDataAsset->IsDivide() == false)
				SkillSetData.totalRuntime = AnimMontage->SequenceLength;
			else
				SkillSetData.totalRuntime = AnimMontage->GetSectionLength(InSectionIndex);
		}

		// 기존에 있던 스킬 노티파이를 리셋 하고 현재 섹션길아 안에 포함된것만 넣는다.	
		AnimMontage->GetSectionStartAndEndTime(InSectionIndex, StartTime, EndTime);

		// 디폴트 값 검증 및 셋팅
		if (SkillSetData.skillSequenceType != SkillSequenceType::CHAIN)
		{
			SkillSetData.sequenceInfo.maxStage = AnimMontage->GetCompositeSections().Num();
			SkillSetData.sequenceInfo.comboInfo.currentStage = InSectionIndex + 1;
		}

		/*if (SkillSetData.levelInfoList.Num() == 0)
		{
			FGsSchemaSkillLevel defaultLevelInfo;
			defaultLevelInfo.level = 1;
			defaultLevelInfo.learnLevel = 1;

			SkillSetData.levelInfoList.Add(defaultLevelInfo);
		}*/

		if (SkillSetData.cooldownTime <= FLT_EPSILON)
			SkillSetData.cooldownTime = 0.1f;

		if (SkillSetData.targetFilterInfo.targetCount <= 0)
			SkillSetData.targetFilterInfo.targetCount = 1;

		// Rootmotion 정보 추출
		SetRootmotionData(AnimMontage);
	}

	UAnimSequence* AnimSequence = Cast<UAnimSequence>(InSkillEditorDataAsset->AnimSequence);
	if (AnimSequence != nullptr)
	{
		SkillSetData.aniPath = AnimSequence->GetPathName();

		if (InSkillEditorDataAsset->AutoTotalRuntimeSetting)
			SkillSetData.totalRuntime = AnimSequence->GetPlayLength();

		StartTime = 0.0f;
		EndTime = AnimSequence->GetPlayLength();

		/*if (SkillSetData.levelInfoList.Num() == 0)
		{
			FGsSchemaSkillLevel defaultLevelInfo;
			defaultLevelInfo.level = 1;
			defaultLevelInfo.learnLevel = 1;

			SkillSetData.levelInfoList.Add(defaultLevelInfo);
		}*/

		if (SkillSetData.cooldownTime <= FLT_EPSILON)
			SkillSetData.cooldownTime = 0.1f;

		if (SkillSetData.targetFilterInfo.targetCount <= 0)
			SkillSetData.targetFilterInfo.targetCount = 1;
		// 기존에 있던 스킬 노티파이를 리셋 하고 현재 섹션길아 안에 포함된것만 넣는다.
		//AnimSequence->GetSectionStartAndEndTime(InSectionIndex, StartTime, EndTime);
	}

	//// 스킬 노티파이 이벤트를 가져온다.
	//TArray< FAnimNotifyEvent* > SkillNotifyEvent;
	//for (int32 index = 0; index < AnimMontage->GetNotifies().Num(); ++index)
	//{
	//	if (AnimMontage->GetNotifies()[index].SkillNotify != nullptr)
	//		SkillNotifyEvent.Add(&(AnimMontage->GetNotifies()[index]));
	//}

	//// 해당 섹션안에 있는 스킬 노티파이만 추린다.
	//SkillSetData.notifyInfoList.Reset();
	//TArray< USkillNotify*> SkillNotifys;
	//for (int32 index = 0; index < SkillNotifyEvent.Num(); ++index)
	//{
	//	if ((StartTime <= SkillNotifyEvent[index]->GetLinkValue())
	//		&& (SkillNotifyEvent[index]->GetLinkValue() < EndTime))
	//	{
	//		if (SkillNotifyEvent[index]->SkillNotify != nullptr)
	//		{
	//			SkillNotifyEvent[index]->SkillNotify->Offset = SkillNotifyEvent[index]->GetLinkValue() - StartTime;

	//			UCutremineSkillNotify* CutremineSkillNotify = Cast< UCutremineSkillNotify >(SkillNotifyEvent[index]->SkillNotify);
	//			UHitSkillNotify* HitSkillNotify = Cast< UHitSkillNotify >(SkillNotifyEvent[index]->SkillNotify);
	//			UProjectileSkillNotify* ProjectileSkillNotify = Cast< UProjectileSkillNotify >(SkillNotifyEvent[index]->SkillNotify);

	//			if (CutremineSkillNotify)
	//			{
	//				CutremineSkillNotify->NotifyCutremineInfo.delayOffset = SkillNotifyEvent[index]->GetLinkValue() - StartTime;
	//				SkillSetData.cutRemainInfo = CutremineSkillNotify->NotifyCutremineInfo;
	//			}
	//			else
	//			{
	//				int32 addIndex = SkillSetData.notifyInfoList.AddDefaulted();

	//				if (HitSkillNotify)
	//				{
	//					SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = HitSkillNotify->SkillNotifySet.RowName;
	//					SkillSetData.notifyInfoList[addIndex].repeatCount = SkillNotifyEvent[index]->SkillNotify->RepeatCount;
	//					SkillSetData.notifyInfoList[addIndex].offset = SkillNotifyEvent[index]->SkillNotify->Offset = SkillNotifyEvent[index]->GetLinkValue() - StartTime;
	//				}
	//				else
	//				if (ProjectileSkillNotify)
	//				{
	//					SkillSetData.notifyInfoList[addIndex].skillNotifyId.RowName = ProjectileSkillNotify->SkillNotifySet.RowName;
	//					SkillSetData.notifyInfoList[addIndex].repeatCount = SkillNotifyEvent[index]->SkillNotify->RepeatCount;
	//					SkillSetData.notifyInfoList[addIndex].offset = SkillNotifyEvent[index]->SkillNotify->Offset = SkillNotifyEvent[index]->GetLinkValue() - StartTime;
	//				}
	//			}

	//		}
	//	}
	//}	
}

void USkillSectionData::ExtractRootMotion(UAnimMontage* InMontage, float InOffset, const FTransform& InTransform,
	OUT FGsSchemaRootmotionSet* InRootmotionSetData)
{
	FTransform rootmotionTm = InMontage->ExtractRootMotionFromTrackRange(0.f, InOffset);
	FTransform convertTm = rootmotionTm.GetRelativeTransform(InTransform);

	const FVector DeltaTranslation = rootmotionTm.GetTranslation() - InTransform.GetTranslation();
	const FVector dir = InTransform.GetRotation() * DeltaTranslation;
	const FVector objectDir = rootmotionTm.GetRotation().Vector();

	// FGsDir::dirToAngle(dir);
	float angle = (short)(FMath::Atan2(dir.Y, dir.X) * 32768 / 3.141592f);
	float objAngle = (short)(FMath::Atan2(objectDir.Y, objectDir.X) * 32768 / 3.141592f);

	FGsSchemaRootmotionInfo newInfo;
	newInfo.Offset = InOffset;
	newInfo.Angle = angle;
	newInfo.ObjectAngle = objAngle;
	newInfo.Pos = dir;
	newInfo.Distance = dir.Size();

	// OutPut
	InRootmotionSetData->timeLine.Add(newInfo);
}

void USkillSectionData::SetRootmotionData(UAnimMontage* InMontage)
{
	UGsDataTable* DTGsRootmotionSet = LoadObject<UGsDataTable>(
		nullptr, TEXT("/Game/DataCenter/DataTable/Skill/RootMotion/DTGsRootmotionSet.DTGsRootmotionSet"));
	if (nullptr == DTGsRootmotionSet)
	{
		return;
	}

	// 디폴트 설정
	SkillSetData.rootMotionId.DataTable = DTGsRootmotionSet;
	SkillSetData.rootMotionId.RowName = TEXT("0");
	if (InMontage && InMontage->HasRootMotion())
	{
		// 스킬 타입 넘버링 부여 2
		FName RowName = FName(*FString::Printf(TEXT("2%d"), SkillSetData.id));

		FGsSchemaRootmotionSet newRootmotion;
		newRootmotion.id = FCString::Atoi(*RowName.ToString());

		// 서버에도 반영되야 되서 totalRuntime을 총길이로 얻기
		float totalLen = SkillSetData.totalRuntime;
		// 클라이언트에서 모델 데이터는 x축을 전방으로 하기 위해 90도 회전이 있다.
		// 애니메이션 원본 데이터는 Y축 전방이므로 90도 회전 적용을 함
		FTransform originTm(FRotator(0.f, -90.f, 0.f));
		int roundupNumber = totalLen * 10;
		float offset = 0.f;
		// 0.1초 간격으로 입력
		for (int i = 0; i < roundupNumber + 1; ++i)
		{
			offset = i * 0.1f;
			ExtractRootMotion(InMontage, offset, originTm, &newRootmotion);
		}

		// 최종 나머지 시간 계산
		if (totalLen - offset > 0.01f)
		{
			offset = totalLen;
			ExtractRootMotion(InMontage, offset, originTm, &newRootmotion);
		}

		// 기존 데이터 검색
		if (FGsSchemaRootmotionSet* rootmotionSet =
			DTGsRootmotionSet->FindRow<FGsSchemaRootmotionSet>(RowName, FString()))
		{
			// 필요 정보 설정 
			newRootmotion.collisionDisable = rootmotionSet->collisionDisable;
			// 기존 정보 지우기
			DTGsRootmotionSet->RemoveRow(RowName);
		}
		DTGsRootmotionSet->AddRow(RowName, newRootmotion);

		// RowPick 연결
		SkillSetData.rootMotionId.DataTable = DTGsRootmotionSet;
		SkillSetData.rootMotionId.RowName = RowName;

		// 테이블 저장
		DTGsRootmotionSet->SavePackage();
	}
}
#endif