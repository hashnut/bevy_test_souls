// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUnrealSupportEditorAssetActionUtility.h"
#include "EditorUtilityLibrary.h"
#include "T1Project/ActorEx/GsCharacterLocalPlayer.h"
#include "T1Project/ActorEx/GsCharacterNonPlayer.h"
#include "T1Project/ActorEx/GsActorFieldFairy.h"
#include "T1Project/ActorEx/GsWeaponActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AssetToolsModule.h"
#include "AssetRegistryModule.h"
#include "PackageTools.h"
#include "ObjectTools.h"
#include "Factories/MaterialFactoryNew.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceConstant.h"
#include "Runtime/Engine/Classes/Materials/MaterialParameterCollection.h"
#include "EditorAssetLibrary.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"
#include "Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "Animation/AnimNotify/AnimNotify_PlayParticleParam.h"
#include "T1Project/Animation/AnimNotify/AnimNotify_PlayParticleAnimationSpeedScale.h"
#include "Particles/ParticleSystem.h"
//#include "Misc/FileHelper.h"

/*
	1. BP내에 SkeletalMeshComponent의 메테리얼경로에 M_CH_Cartoon_Outline_Inst(인스턴스)를 M_CH_Cartoon_Outline_Inst(인스턴스)_Name으로 생성
	2. BP내에 SkeletalMeshComponent의 메테리얼을 0번을 얻어와서 OutLine 파라미터 값을 읽은후에  M_CH_Cartoon_Outline_Inst(인스턴스)_Name 생성된 메터리얼에 세팅
	3. SkeletalMeshComponent내에 OverlayMaterial에 생성된 메터리얼 세팅
*/

void UGsUnrealSupportEditorAssetActionUtility::ParameterSetting(UMaterialInterface* MaterialInterface, UMaterialInstance* NewOverlayMaterial, UMaterialParameterCollection* MaterialParameterCollection)
{
	bool OutlineColor0Check, OutlineColor1Check, OutlineColor2Check, OutlineColor3Check;
	FLinearColor OutlineColor0, OutlineColor1, OutlineColor2, OutlineColor3;
	OutlineColor0Check = MaterialInterface->GetVectorParameterValue(TEXT("Outline Color 0"), OutlineColor0);
	OutlineColor1Check = MaterialInterface->GetVectorParameterValue(TEXT("Outline Color 1"), OutlineColor1);
	OutlineColor2Check = MaterialInterface->GetVectorParameterValue(TEXT("Outline Color 2"), OutlineColor2);
	OutlineColor3Check = MaterialInterface->GetVectorParameterValue(TEXT("Outline Color 3"), OutlineColor3);

	if (OutlineColor0Check == false ||
		OutlineColor1Check == false ||
		OutlineColor2Check == false ||
		OutlineColor3Check == false)
	{
		FSoftObjectPath SrcOutlineColorPath = MaterialInterface;
		UE_LOG(LogTemp, Error, TEXT("%s참조할 Out Line Color가 없습니다."), *(SrcOutlineColorPath.ToString()));
	}

	if (NewOverlayMaterial->IsA<UMaterialInstanceConstant>())
	{
		if (UMaterialInstanceConstant* NewOverlayMaterialConstant = (UMaterialInstanceConstant*)NewOverlayMaterial)
		{
			NewOverlayMaterialConstant->SetVectorParameterValueEditorOnly(TEXT("Outline Color 0"), OutlineColor0);
			NewOverlayMaterialConstant->SetVectorParameterValueEditorOnly(TEXT("Outline Color 1"), OutlineColor1);
			NewOverlayMaterialConstant->SetVectorParameterValueEditorOnly(TEXT("Outline Color 2"), OutlineColor2);
			NewOverlayMaterialConstant->SetVectorParameterValueEditorOnly(TEXT("Outline Color 3"), OutlineColor3);

			if (MaterialParameterCollection != nullptr)
			{
				for (auto ScalarParameter : MaterialParameterCollection->ScalarParameters)
				{
					if (ScalarParameter.ParameterName == TEXT("Outline Width"))
					{
						NewOverlayMaterialConstant->SetScalarParameterValueEditorOnly(TEXT("Outline Width"), ScalarParameter.DefaultValue);
					}

					if (ScalarParameter.ParameterName == TEXT("Ratio Vertex R"))
					{
						NewOverlayMaterialConstant->SetScalarParameterValueEditorOnly(TEXT("Ratio Vertex R"), ScalarParameter.DefaultValue);
					}

					if (ScalarParameter.ParameterName == TEXT("Outline Brightness"))
					{
						NewOverlayMaterialConstant->SetScalarParameterValueEditorOnly(TEXT("Outline Brightness"), ScalarParameter.DefaultValue);
					}
				}
			}
			else
			{
				FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("UGsUnrealSupportEditorAssetActionUtility", "OverlayMaterialSetting", "MaterialParameterCollection이 없습니다."));
			}
		}
	}
}

void UGsUnrealSupportEditorAssetActionUtility::OverlayMaterialSetting(UStaticMeshComponent* TargetCom, UMaterialInstance* SrcMaterial, FSoftObjectPath SrcMatPath, FSoftObjectPath DestPath, UMaterialParameterCollection* MaterialParameterCollection)
{
	if (TargetCom != nullptr)
	{
		UMaterialInterface* MaterialInterface = TargetCom->GetMaterial(0);
		if (MaterialInterface != nullptr)
		{
			// 1. BP내에 SkeletalMeshComponent의 메테리얼경로에 M_CH_Cartoon_Outline_Inst(인스턴스)를 M_CH_Cartoon_Outline_Inst(인스턴스)_Name으로 생성			
			// 저장할 경로 구하기
			FSoftObjectPath MatPath = MaterialInterface;
			FString AssetName = MatPath.GetAssetName();
			FString SaveAssetPath = MatPath.GetLongPackageName();
			SaveAssetPath.RemoveFromEnd(AssetName);

			// 저장할 애셋 이름 만들기	_C				
			FString NewAssetName = DestPath.GetAssetName();
			NewAssetName.RemoveFromEnd(TEXT("_C"));
			FString SaveAssetName = SrcMatPath.GetAssetName() + TEXT("_") + NewAssetName;

			FString Name, PackageName;
			const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
			AssetToolsModule.Get().CreateUniqueAssetName(SaveAssetPath, *SaveAssetName, PackageName, Name);
			const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);

			UMaterialInstance* NewOverlayMaterial = (UMaterialInstance*)TargetCom->GetOverlayMaterial();
			if (NewOverlayMaterial == nullptr)
			{
				NewOverlayMaterial = Cast<UMaterialInstance>(UEditorAssetLibrary::LoadAsset(SaveAssetPath + SaveAssetName));
				if (NewOverlayMaterial == nullptr)
					NewOverlayMaterial = Cast<UMaterialInstance>(AssetToolsModule.Get().DuplicateAsset(SaveAssetName, PackagePath, SrcMaterial));
			}

			if (NewOverlayMaterial != nullptr)
			{
				// 2. BP내에 SkeletalMeshComponent의 메테리얼을 0번을 얻어와서 OutLine 파라미터 값을 읽은후에  M_CH_Cartoon_Outline_Inst(인스턴스)_Name 생성된 메터리얼에 세팅
				ParameterSetting(MaterialInterface, NewOverlayMaterial, MaterialParameterCollection);

				// 3. SkeletalMeshComponent내에 OverlayMaterial에 생성된 메터리얼 세팅
				TargetCom->SetOverlayMaterial(NewOverlayMaterial);

				UE_LOG(LogTemp, Log, TEXT("%s SetOverlayMaterial Complate"), *(DestPath.ToString()));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("OverlayMaterial is Null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s 에 참조할 index 0번에 Material이 없습니다."), *(DestPath.ToString()));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s 에 UStaticMeshComponent 가 없습니다."), *(DestPath.ToString()));
	}
}

void UGsUnrealSupportEditorAssetActionUtility::OverlayMaterialSetting(USkeletalMeshComponent* TargetCom, UMaterialInstance* SrcMaterial, FSoftObjectPath SrcMatPath, FSoftObjectPath DestPath, UMaterialParameterCollection* MaterialParameterCollection)
{
	if (TargetCom != nullptr)
	{		
		UMaterialInterface* MaterialInterface = TargetCom->GetMaterial(0);
		if (MaterialInterface != nullptr)
		{
			// 1. BP내에 SkeletalMeshComponent의 메테리얼경로에 M_CH_Cartoon_Outline_Inst(인스턴스)를 M_CH_Cartoon_Outline_Inst(인스턴스)_Name으로 생성			
			// 저장할 경로 구하기
			FSoftObjectPath MatPath = MaterialInterface;
			FString AssetName = MatPath.GetAssetName();
			FString SaveAssetPath = MatPath.GetLongPackageName();
			SaveAssetPath.RemoveFromEnd(AssetName);

			// 저장할 애셋 이름 만들기	_C				
			FString NewAssetName = DestPath.GetAssetName();
			NewAssetName.RemoveFromEnd(TEXT("_C"));
			FString SaveAssetName = SrcMatPath.GetAssetName() + TEXT("_") + NewAssetName;

			FString Name, PackageName;
			const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
			AssetToolsModule.Get().CreateUniqueAssetName(SaveAssetPath, *SaveAssetName, PackageName, Name);
			const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);

			UMaterialInstance* NewOverlayMaterial = (UMaterialInstance*)TargetCom->GetOverlayMaterial();
			if (NewOverlayMaterial == nullptr)
			{
				NewOverlayMaterial = Cast<UMaterialInstance>(UEditorAssetLibrary::LoadAsset(SaveAssetPath + SaveAssetName));
				if(NewOverlayMaterial == nullptr)
					NewOverlayMaterial = Cast<UMaterialInstance>(AssetToolsModule.Get().DuplicateAsset(SaveAssetName, PackagePath, SrcMaterial));
			}
			
			if (NewOverlayMaterial != nullptr)
			{
				// 2. BP내에 SkeletalMeshComponent의 메테리얼을 0번을 얻어와서 OutLine 파라미터 값을 읽은후에  M_CH_Cartoon_Outline_Inst(인스턴스)_Name 생성된 메터리얼에 세팅
				ParameterSetting(MaterialInterface, NewOverlayMaterial, MaterialParameterCollection);

				// 3. SkeletalMeshComponent내에 OverlayMaterial에 생성된 메터리얼 세팅
				TargetCom->SetOverlayMaterial(NewOverlayMaterial);

				UE_LOG(LogTemp, Log, TEXT("%s SetOverlayMaterial Complate"), *(DestPath.ToString()));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("OverlayMaterial is Null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s 에 참조할 index 0번에 Material이 없습니다."), *(DestPath.ToString()));
		}
	}
	else
	{		
		UE_LOG(LogTemp, Error, TEXT("%s 에 USkeletalMeshComponent 가 없습니다."), *(DestPath.ToString()));
	}
}

void UGsUnrealSupportEditorAssetActionUtility::ApplyOverlayMaterialSetting()
{
	// 복사할 원본
	UMaterialInstance* CopyMID = nullptr;
	static const FSoftObjectPath CopyMatPath(TEXT("/Game/MasterMaterials/Default/Cartoon/New/M_CH_Cartoon_Outline_Inst.M_CH_Cartoon_Outline_Inst"));
	if (UMaterialInstance* BaseMat = Cast<UMaterialInstance>(CopyMatPath.TryLoad()))
	{
		CopyMID = BaseMat;
	}

	static const FSoftObjectPath MaterialParameterCollectionPath(TEXT("/Game/MasterMaterials/Default/Cartoon/New/MPC_CH_Outline.MPC_CH_Outline"));
	UMaterialParameterCollection* MaterialParameterCollection = Cast<UMaterialParameterCollection>(MaterialParameterCollectionPath.TryLoad());

	int32 ApplyOverlayMaterialSettingCnt = 0;
	TArray<UClass*> selectedBPArray = UEditorUtilityLibrary::GetSelectedBlueprintClasses();
	for (UClass* selectedBP : selectedBPArray)
	{
		UObject* Object = selectedBP->GetDefaultObject<UObject>();
		AGsCharacterPlayer* GsCharacterPlayer = Cast<AGsCharacterPlayer>(Object);
		if (GsCharacterPlayer != nullptr)
		{
			++ApplyOverlayMaterialSettingCnt;

			FSoftObjectPath BPPath = selectedBP;

			USkeletalMeshComponent* HairCom = GsCharacterPlayer->GetPartsSkeletalComponentHAIR();
			OverlayMaterialSetting(HairCom, CopyMID, CopyMatPath, BPPath, MaterialParameterCollection);

			USkeletalMeshComponent* FaceCom = GsCharacterPlayer->GetPartsSkeletalComponentFACE();
			OverlayMaterialSetting(FaceCom, CopyMID, CopyMatPath, BPPath, MaterialParameterCollection);

			USkeletalMeshComponent* BodyCom = GsCharacterPlayer->GetPartsSkeletalComponentBODY();
			OverlayMaterialSetting(BodyCom, CopyMID, CopyMatPath, BPPath, MaterialParameterCollection);
			
			FEditorFileUtils::SaveDirtyPackage(selectedBP->GetOuterUPackage(), true, true, true, false, false, false);
		}

		AGsWeaponActor* GsWeaponActor = Cast<AGsWeaponActor>(Object);
		if (GsWeaponActor != nullptr)
		{
			++ApplyOverlayMaterialSettingCnt;

			FSoftObjectPath BPPath = selectedBP;

			UStaticMeshComponent* StaticCom = GsWeaponActor->GetStaticMeshComponent();
			OverlayMaterialSetting(StaticCom, CopyMID, CopyMatPath, BPPath, MaterialParameterCollection);

			FEditorFileUtils::SaveDirtyPackage(selectedBP->GetOuterUPackage(), true, true, true, false, false, false);
		}

		AGsCharacterNonPlayer* GsCharacterNonPlayer = Cast<AGsCharacterNonPlayer>(Object);
		if (GsCharacterNonPlayer != nullptr)
		{
			++ApplyOverlayMaterialSettingCnt;

			FSoftObjectPath BPPath = selectedBP;			

			USkeletalMeshComponent* FaceCom = GsCharacterNonPlayer->GetPartsSkeletalComponentFACE();
			OverlayMaterialSetting(FaceCom, CopyMID, CopyMatPath, BPPath, MaterialParameterCollection);

			USkeletalMeshComponent* BodyCom = GsCharacterNonPlayer->GetPartsSkeletalComponentBODY();
			OverlayMaterialSetting(BodyCom, CopyMID, CopyMatPath, BPPath, MaterialParameterCollection);

			FEditorFileUtils::SaveDirtyPackage(selectedBP->GetOuterUPackage(), true, true, true, false, false, false);
		}

		AGsActorFieldFairy* GsActorFieldFairy = Cast<AGsActorFieldFairy>(Object);
		if (GsActorFieldFairy != nullptr)
		{
			++ApplyOverlayMaterialSettingCnt;

			FSoftObjectPath BPPath = selectedBP;

			TArray<USkeletalMeshComponent*> OutskeletalMesh;
			GsActorFieldFairy->GetSkeletalMeshs(OutskeletalMesh);
			for (USkeletalMeshComponent* skeletalMesh : OutskeletalMesh)
				OverlayMaterialSetting(skeletalMesh, CopyMID, CopyMatPath, BPPath, MaterialParameterCollection);

			FEditorFileUtils::SaveDirtyPackage(selectedBP->GetOuterUPackage(), true, true, true, false, false, false);
		}
	}

	if (ApplyOverlayMaterialSettingCnt > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("ApplyOverlayMaterialSetting Complated"));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("UGsUnrealSupportEditorAssetActionUtility", "ApplyOverlayMaterialSetting", "적용 셋팅할 Actor가 포함되어 있지 않습니다.(AGsCharacterPlayer, AGsWeaponActor, AGsCharacterNonPlayer, AGsActorFieldFairy"));
	}
}

void UGsUnrealSupportEditorAssetActionUtility::ChangeNotifyPlayParticleEffectToNotifyPlayParticleAnimationSpeedScale()
{
	// test용으로 하나만 변경되게
	/*UAnimSequence* AnimSequence = nullptr;
	UAnimMontage* AnimMontage = nullptr;
	TArray<UObject*> selectedArray = UEditorUtilityLibrary::GetSelectedAssets();
	for (UObject* selected : selectedArray)
	{
		if (false == selected->IsA(UAnimSequence::StaticClass()) && false == selected->IsA(UAnimMontage::StaticClass()))
			continue;

		AnimSequence = Cast<UAnimSequence>(selected);
		AnimMontage = Cast<UAnimMontage>(selected);
		if (AnimSequence)
		{
			for (int32 index = 0; index < AnimSequence->Notifies.Num(); ++index)
			{
				if (AnimSequence->Notifies[index].Notify != nullptr)
				{
					//if (AnimSequence->Notifies[index].Notify->IsA(UAnimNotify_PlayParticleParam::StaticClass()))
					if (AnimSequence->Notifies[index].Notify->GetClass() == UAnimNotify_PlayParticleParam::StaticClass())
					{
						UAnimNotify_PlayParticleParam* particle = Cast<UAnimNotify_PlayParticleParam>(AnimSequence->Notifies[index].Notify);
						if (particle != nullptr)
						{
							class UObject* NewObj = NewObject<UObject>(AnimSequence, UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass(), NAME_None, RF_Transactional);

							if (NewObj->IsA(UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass()))
							{
								UAnimNotify_PlayParticleAnimationSpeedScale* PlayParticleAnimationSpeedScale = Cast<UAnimNotify_PlayParticleAnimationSpeedScale>(NewObj);

								if (PlayParticleAnimationSpeedScale)
								{
									PlayParticleAnimationSpeedScale->PSTemplate = particle->PSTemplate;
									PlayParticleAnimationSpeedScale->LocationOffset = particle->LocationOffset;
									PlayParticleAnimationSpeedScale->RotationOffset = particle->RotationOffset;
									PlayParticleAnimationSpeedScale->Scale = particle->Scale;
									PlayParticleAnimationSpeedScale->Attached = particle->Attached;
									PlayParticleAnimationSpeedScale->SocketName = particle->SocketName;
									PlayParticleAnimationSpeedScale->NotifyColor = particle->NotifyColor;

									PlayParticleAnimationSpeedScale->InstanceParameters = particle->InstanceParameters;
								}

								AnimSequence->Notifies[index].Notify = Cast<UAnimNotify>(NewObj);
							}

							AnimSequence->PostEditChange();
							AnimSequence->MarkPackageDirty();
						}
					}
					else
					//if (AnimSequence->Notifies[index].Notify->IsA(UAnimNotify_PlayParticleEffect::StaticClass()))
					if (AnimSequence->Notifies[index].Notify->GetClass() == UAnimNotify_PlayParticleEffect::StaticClass())
					{
						UAnimNotify_PlayParticleEffect* particle = Cast<UAnimNotify_PlayParticleEffect>(AnimSequence->Notifies[index].Notify);
						if (particle != nullptr)
						{
							class UObject* NewObj = NewObject<UObject>(AnimSequence, UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass(), NAME_None, RF_Transactional);

							if (NewObj->IsA(UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass()))
							{
								UAnimNotify_PlayParticleAnimationSpeedScale* PlayParticleAnimationSpeedScale = Cast<UAnimNotify_PlayParticleAnimationSpeedScale>(NewObj);

								if (PlayParticleAnimationSpeedScale)
								{									
									PlayParticleAnimationSpeedScale->PSTemplate = particle->PSTemplate;
									PlayParticleAnimationSpeedScale->LocationOffset = particle->LocationOffset;
									PlayParticleAnimationSpeedScale->RotationOffset = particle->RotationOffset;
									PlayParticleAnimationSpeedScale->Scale = particle->Scale;
									PlayParticleAnimationSpeedScale->Attached = particle->Attached;
									PlayParticleAnimationSpeedScale->SocketName = particle->SocketName;
									PlayParticleAnimationSpeedScale->NotifyColor = particle->NotifyColor;
								}

								AnimSequence->Notifies[index].Notify = Cast<UAnimNotify>(NewObj);
							}

							AnimSequence->PostEditChange();
							AnimSequence->MarkPackageDirty();
						}
					}
				}
			}
		}
		else if (AnimMontage)
		{
			for (int32 index = 0; index < AnimMontage->Notifies.Num(); ++index)
			{
				if (AnimMontage->Notifies[index].Notify != nullptr)
				{
					//if (AnimMontage->Notifies[index].Notify->IsA(UAnimNotify_PlayParticleParam::StaticClass()))
					if (AnimMontage->Notifies[index].Notify->GetClass() == UAnimNotify_PlayParticleParam::StaticClass())
					{
						UAnimNotify_PlayParticleParam* particle = Cast<UAnimNotify_PlayParticleParam>(AnimMontage->Notifies[index].Notify);
						if (particle != nullptr)
						{
							class UObject* NewObj = NewObject<UObject>(AnimMontage, UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass(), NAME_None, RF_Transactional);

							if (NewObj->IsA(UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass()))
							{
								UAnimNotify_PlayParticleAnimationSpeedScale* PlayParticleAnimationSpeedScale = Cast<UAnimNotify_PlayParticleAnimationSpeedScale>(NewObj);

								if (PlayParticleAnimationSpeedScale)
								{
									PlayParticleAnimationSpeedScale->PSTemplate = particle->PSTemplate;
									PlayParticleAnimationSpeedScale->LocationOffset = particle->LocationOffset;
									PlayParticleAnimationSpeedScale->RotationOffset = particle->RotationOffset;
									PlayParticleAnimationSpeedScale->Scale = particle->Scale;
									PlayParticleAnimationSpeedScale->Attached = particle->Attached;
									PlayParticleAnimationSpeedScale->SocketName = particle->SocketName;
									PlayParticleAnimationSpeedScale->NotifyColor = particle->NotifyColor;

									PlayParticleAnimationSpeedScale->InstanceParameters = particle->InstanceParameters;
								}

								AnimMontage->Notifies[index].Notify = Cast<UAnimNotify>(NewObj);
							}

							AnimMontage->PostEditChange();
							AnimMontage->MarkPackageDirty();
						}
					}
					else
					if (AnimMontage->Notifies[index].Notify->GetClass() == UAnimNotify_PlayParticleEffect::StaticClass())
					//if (AnimMontage->Notifies[index].Notify->IsA(UAnimNotify_PlayParticleEffect::StaticClass()))
					{
						UAnimNotify_PlayParticleEffect* particle = Cast<UAnimNotify_PlayParticleEffect>(AnimMontage->Notifies[index].Notify);
						if (particle != nullptr)
						{
							class UObject* NewObj = NewObject<UObject>(AnimMontage, UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass(), NAME_None, RF_Transactional);

							if (NewObj->IsA(UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass()))
							{
								UAnimNotify_PlayParticleAnimationSpeedScale* PlayParticleAnimationSpeedScale = Cast<UAnimNotify_PlayParticleAnimationSpeedScale>(NewObj);

								if (PlayParticleAnimationSpeedScale)
								{
									PlayParticleAnimationSpeedScale->PSTemplate = particle->PSTemplate;									
									PlayParticleAnimationSpeedScale->LocationOffset = particle->LocationOffset;
									PlayParticleAnimationSpeedScale->RotationOffset = particle->RotationOffset;
									PlayParticleAnimationSpeedScale->Scale = particle->Scale;
									PlayParticleAnimationSpeedScale->Attached = particle->Attached;
									PlayParticleAnimationSpeedScale->SocketName = particle->SocketName;
									PlayParticleAnimationSpeedScale->NotifyColor = particle->NotifyColor;
								}

								AnimMontage->Notifies[index].Notify = Cast<UAnimNotify>(NewObj);
							}

							AnimMontage->PostEditChange();
							AnimMontage->MarkPackageDirty();
						}
					}
				}
			}
		}
		
	}*/

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AnimSequenceBaseAssetData;
	const UClass* Class = UAnimSequence::StaticClass();
	AssetRegistryModule.Get().GetAssetsByClass(Class->GetFName(), AnimSequenceBaseAssetData);

	for (int32 AssetIdx = 0; AssetIdx < AnimSequenceBaseAssetData.Num(); ++AssetIdx)
	{
		const FAssetData& AssetData = AnimSequenceBaseAssetData[AssetIdx];
		UAnimSequence* AnimSequence = Cast<UAnimSequence>(AssetData.GetAsset());
		if (AnimSequence != nullptr)
		{
			for (int32 index = 0; index < AnimSequence->Notifies.Num(); ++index)
			{
				if (AnimSequence->Notifies[index].Notify != nullptr)
				{
					if (AnimSequence->Notifies[index].Notify->GetClass() == UAnimNotify_PlayParticleParam::StaticClass())
					{
						UAnimNotify_PlayParticleParam* particle = Cast<UAnimNotify_PlayParticleParam>(AnimSequence->Notifies[index].Notify);
						if (particle != nullptr)
						{
							class UObject* NewObj = NewObject<UObject>(AnimSequence, UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass(), NAME_None, RF_Transactional);

							if (NewObj->IsA(UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass()))
							{
								UAnimNotify_PlayParticleAnimationSpeedScale* PlayParticleAnimationSpeedScale = Cast<UAnimNotify_PlayParticleAnimationSpeedScale>(NewObj);

								if (PlayParticleAnimationSpeedScale)
								{
									PlayParticleAnimationSpeedScale->PSTemplate = particle->PSTemplate;
									PlayParticleAnimationSpeedScale->LocationOffset = particle->LocationOffset;
									PlayParticleAnimationSpeedScale->RotationOffset = particle->RotationOffset;
									PlayParticleAnimationSpeedScale->Scale = particle->Scale;
									PlayParticleAnimationSpeedScale->Attached = particle->Attached;
									PlayParticleAnimationSpeedScale->SocketName = particle->SocketName;
									PlayParticleAnimationSpeedScale->NotifyColor = particle->NotifyColor;

									PlayParticleAnimationSpeedScale->InstanceParameters = particle->InstanceParameters;
								}

								AnimSequence->Notifies[index].Notify = Cast<UAnimNotify>(NewObj);
							}

							AnimSequence->PostEditChange();
							AnimSequence->MarkPackageDirty();
						}
					}
					else
					if (AnimSequence->Notifies[index].Notify->GetClass() == UAnimNotify_PlayParticleEffect::StaticClass())
					{
						UAnimNotify_PlayParticleEffect* particle = Cast<UAnimNotify_PlayParticleEffect>(AnimSequence->Notifies[index].Notify);
						if (particle != nullptr)
						{
							class UObject* NewObj = NewObject<UObject>(AnimSequence, UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass(), NAME_None, RF_Transactional);

							if (NewObj->IsA(UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass()))
							{
								UAnimNotify_PlayParticleAnimationSpeedScale* PlayParticleAnimationSpeedScale = Cast<UAnimNotify_PlayParticleAnimationSpeedScale>(NewObj);

								if (PlayParticleAnimationSpeedScale)
								{
									PlayParticleAnimationSpeedScale->PSTemplate = particle->PSTemplate;
									PlayParticleAnimationSpeedScale->LocationOffset = particle->LocationOffset;
									PlayParticleAnimationSpeedScale->RotationOffset = particle->RotationOffset;
									PlayParticleAnimationSpeedScale->Scale = particle->Scale;
									PlayParticleAnimationSpeedScale->Attached = particle->Attached;
									PlayParticleAnimationSpeedScale->SocketName = particle->SocketName;
									PlayParticleAnimationSpeedScale->NotifyColor = particle->NotifyColor;
								}

								AnimSequence->Notifies[index].Notify = Cast<UAnimNotify>(NewObj);
							}

							AnimSequence->PostEditChange();
							AnimSequence->MarkPackageDirty();
						}
					}
				}
			}
		}
	}

	TArray<FAssetData> AnimMontageAssetData;
	const UClass* AnimMontageClass = UAnimMontage::StaticClass();
	AssetRegistryModule.Get().GetAssetsByClass(AnimMontageClass->GetFName(), AnimMontageAssetData);

	for (int32 AssetIdx = 0; AssetIdx < AnimMontageAssetData.Num(); ++AssetIdx)
	{
		const FAssetData& AssetData = AnimMontageAssetData[AssetIdx];
		UAnimMontage* AnimMontage = Cast<UAnimMontage>(AssetData.GetAsset());
		if (AnimMontage)
		{
			for (int32 index = 0; index < AnimMontage->Notifies.Num(); ++index)
			{
				if (AnimMontage->Notifies[index].Notify != nullptr)
				{
					if (AnimMontage->Notifies[index].Notify->GetClass() == UAnimNotify_PlayParticleParam::StaticClass())
					{
						UAnimNotify_PlayParticleParam* particle = Cast<UAnimNotify_PlayParticleParam>(AnimMontage->Notifies[index].Notify);
						if (particle != nullptr)
						{
							class UObject* NewObj = NewObject<UObject>(AnimMontage, UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass(), NAME_None, RF_Transactional);

							if (NewObj->IsA(UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass()))
							{
								UAnimNotify_PlayParticleAnimationSpeedScale* PlayParticleAnimationSpeedScale = Cast<UAnimNotify_PlayParticleAnimationSpeedScale>(NewObj);

								if (PlayParticleAnimationSpeedScale)
								{
									PlayParticleAnimationSpeedScale->PSTemplate = particle->PSTemplate;
									PlayParticleAnimationSpeedScale->LocationOffset = particle->LocationOffset;
									PlayParticleAnimationSpeedScale->RotationOffset = particle->RotationOffset;
									PlayParticleAnimationSpeedScale->Scale = particle->Scale;
									PlayParticleAnimationSpeedScale->Attached = particle->Attached;
									PlayParticleAnimationSpeedScale->SocketName = particle->SocketName;
									PlayParticleAnimationSpeedScale->NotifyColor = particle->NotifyColor;

									PlayParticleAnimationSpeedScale->InstanceParameters = particle->InstanceParameters;
								}

								AnimMontage->Notifies[index].Notify = Cast<UAnimNotify>(NewObj);
							}

							AnimMontage->PostEditChange();
							AnimMontage->MarkPackageDirty();
						}
					}
					else
					if (AnimMontage->Notifies[index].Notify->GetClass() == UAnimNotify_PlayParticleEffect::StaticClass())
					{
						UAnimNotify_PlayParticleEffect* particle = Cast<UAnimNotify_PlayParticleEffect>(AnimMontage->Notifies[index].Notify);
						if (particle != nullptr)
						{
							class UObject* NewObj = NewObject<UObject>(AnimMontage, UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass(), NAME_None, RF_Transactional);

							if (NewObj->IsA(UAnimNotify_PlayParticleAnimationSpeedScale::StaticClass()))
							{
								UAnimNotify_PlayParticleAnimationSpeedScale* PlayParticleAnimationSpeedScale = Cast<UAnimNotify_PlayParticleAnimationSpeedScale>(NewObj);

								if (PlayParticleAnimationSpeedScale)
								{
									PlayParticleAnimationSpeedScale->PSTemplate = particle->PSTemplate;
									PlayParticleAnimationSpeedScale->LocationOffset = particle->LocationOffset;
									PlayParticleAnimationSpeedScale->RotationOffset = particle->RotationOffset;
									PlayParticleAnimationSpeedScale->Scale = particle->Scale;
									PlayParticleAnimationSpeedScale->Attached = particle->Attached;

									PlayParticleAnimationSpeedScale->SocketName = particle->SocketName;
									PlayParticleAnimationSpeedScale->NotifyColor = particle->NotifyColor;
								}

								AnimMontage->Notifies[index].Notify = Cast<UAnimNotify>(NewObj);
							}

							AnimMontage->PostEditChange();
							AnimMontage->MarkPackageDirty();
						}
					}
				}
			}
		}
	}
}