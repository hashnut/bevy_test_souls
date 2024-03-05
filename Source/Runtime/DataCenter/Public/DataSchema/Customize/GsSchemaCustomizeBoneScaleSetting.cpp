/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/Customize/GsSchemaCustomizeBoneScaleSetting.h"
FGsSchemaCustomizeBoneScaleSetting::FGsSchemaCustomizeBoneScaleSetting()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCustomizeBoneScaleSetting)
	
	genderType = CreatureGenderType::ALL;
	Height = TArray<FGsSchemaCustomizeBoneScaleGroup> ();
	Height.AddUninitialized(1);
	FGsSchemaCustomizeBoneScaleGroup::StaticStruct()->InitializeStruct(Height.GetData(), 1);
	auto& __Local__0 = Height[0];
	__Local__0.Desc = FString(TEXT("키"));
	__Local__0.BoneName = FName(TEXT("Bip001"));
	__Local__0.ScaleVector = FVector(1.000000, 1.000000, 1.000000);
	__Local__0.MaxVector = FVector(1.500000, 1.500000, 1.500000);
	__Local__0.MinVector = FVector(0.500000, 0.500000, 0.500000);
	Head = TArray<FGsSchemaCustomizeBoneScaleGroup> ();
	Head.AddUninitialized(1);
	FGsSchemaCustomizeBoneScaleGroup::StaticStruct()->InitializeStruct(Head.GetData(), 1);
	auto& __Local__1 = Head[0];
	__Local__1.Desc = FString(TEXT("머리"));
	__Local__1.BoneName = FName(TEXT("Bip001-Head"));
	__Local__1.ScaleVector = FVector(1.000000, 1.000000, 1.000000);
	__Local__1.MaxVector = FVector(1.500000, 1.500000, 1.500000);
	__Local__1.MinVector = FVector(0.500000, 0.500000, 0.500000);
	Chest = TArray<FGsSchemaCustomizeBoneScaleGroup> ();
	Chest.AddUninitialized(1);
	FGsSchemaCustomizeBoneScaleGroup::StaticStruct()->InitializeStruct(Chest.GetData(), 1);
	auto& __Local__2 = Chest[0];
	__Local__2.Desc = FString(TEXT("가슴"));
	__Local__2.BoneName = FName(TEXT("Bip001-Spine2"));
	__Local__2.ScaleVector = FVector(1.000000, 1.000000, 1.000000);
	__Local__2.MaxVector = FVector(1.500000, 1.500000, 1.500000);
	__Local__2.MinVector = FVector(0.500000, 0.500000, 0.500000);
	Waist = TArray<FGsSchemaCustomizeBoneScaleGroup> ();
	Waist.AddUninitialized(2);
	FGsSchemaCustomizeBoneScaleGroup::StaticStruct()->InitializeStruct(Waist.GetData(), 2);
	auto& __Local__3 = Waist[0];
	__Local__3.Desc = FString(TEXT("허리"));
	__Local__3.BoneName = FName(TEXT("Bip001-Spine"));
	__Local__3.ScaleVector = FVector(1.000000, 1.000000, 1.000000);
	__Local__3.MaxVector = FVector(1.500000, 1.500000, 1.500000);
	__Local__3.MinVector = FVector(0.500000, 0.500000, 0.500000);
	auto& __Local__4 = Waist[1];
	__Local__4.Desc = FString(TEXT("허리"));
	__Local__4.BoneName = FName(TEXT("Bip001-Spine1"));
	__Local__4.ScaleVector = FVector(1.000000, 1.000000, 1.000000);
	__Local__4.MaxVector = FVector(1.500000, 1.500000, 1.500000);
	__Local__4.MinVector = FVector(0.500000, 0.500000, 0.500000);
	Pelvis = TArray<FGsSchemaCustomizeBoneScaleGroup> ();
	Pelvis.AddUninitialized(1);
	FGsSchemaCustomizeBoneScaleGroup::StaticStruct()->InitializeStruct(Pelvis.GetData(), 1);
	auto& __Local__5 = Pelvis[0];
	__Local__5.Desc = FString(TEXT("골반"));
	__Local__5.BoneName = FName(TEXT("Bip001-Pelvis"));
	__Local__5.ScaleVector = FVector(1.000000, 1.000000, 1.000000);
	__Local__5.MaxVector = FVector(1.500000, 1.500000, 1.500000);
	__Local__5.MinVector = FVector(0.500000, 0.500000, 0.500000);
	LegThickness = TArray<FGsSchemaCustomizeBoneScaleGroup> ();
	LegThickness.AddUninitialized(2);
	FGsSchemaCustomizeBoneScaleGroup::StaticStruct()->InitializeStruct(LegThickness.GetData(), 2);
	auto& __Local__6 = LegThickness[0];
	__Local__6.Desc = FString(TEXT("오른쪽 다리 두께"));
	__Local__6.BoneName = FName(TEXT("Bip001-R-Thigh"));
	__Local__6.ScaleVector = FVector(1.000000, 1.000000, 1.000000);
	__Local__6.MaxVector = FVector(1.500000, 1.500000, 1.500000);
	__Local__6.MinVector = FVector(0.500000, 0.500000, 0.500000);
	auto& __Local__7 = LegThickness[1];
	__Local__7.Desc = FString(TEXT("왼쪽 다리 두께"));
	__Local__7.BoneName = FName(TEXT("Bip001-R-Thigh"));
	__Local__7.ScaleVector = FVector(1.000000, 1.000000, 1.000000);
	__Local__7.MaxVector = FVector(1.500000, 1.500000, 1.500000);
	__Local__7.MinVector = FVector(0.500000, 0.500000, 0.500000);
}
