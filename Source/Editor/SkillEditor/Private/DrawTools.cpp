#include "DrawTools.h"

#include "EngineMinimal.h"

#include "SceneManagement.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogDrawTools, Log, All);

namespace DrawTools
{
	UMaterial* FDrawManager::DrawMaterial = nullptr;
	UMaterial* FDrawManager::SM5DrawMaterial = nullptr;

	/*FDrawData* FDrawManager::GetDrawData(int32 InIndex)
	{
		if(DrawDatas.IsValidIndex(InIndex))
			return DrawDatas[InIndex][0];

		return nullptr;
	}*/

	TArray<class FDrawData*>& FDrawManager::GetDrawDatas(int32 InIndex)
	{
		return DrawDatas[InIndex];
		//if (DrawDatas.IsValidIndex(InIndex))
		//return DrawDatas[InIndex];
		//return nullptr;
	}

	FDrawManager::FDrawManager(float* InCurrentTime)
	{
		const FString Paths = FString(TEXT("/Game/SkillEditor/DrawMaterial.DrawMaterial"));
		FDrawManager::DrawMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *Paths));

		const FString PathsSM5 = FString(TEXT("/Game/SkillEditor/DrawMaterialSM5.DrawMaterialSM5"));
		FDrawManager::SM5DrawMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *PathsSM5));
	}

	void FDrawManager::ClearDrawData(int32 InSectionIndex)
	{
		if (DrawDatas.IsValidIndex(InSectionIndex))
			//for (int32 i = 0; i < DrawDatas.Num(); ++i)
		{
			for (int32 j = 0; j < DrawDatas[InSectionIndex].Num(); ++j)
			{
				if (DrawDatas[InSectionIndex][j] != nullptr)
					delete DrawDatas[InSectionIndex][j];
			}

			DrawDatas[InSectionIndex].Reset();
		}

		//DrawDatas.Reset();
		//CurrentTime = 0.0f;
		TotalTime = 0.0f;
	}

	void FDrawManager::ClearDrawData()
	{
		for (int32 i = 0; i < DrawDatas.Num(); ++i)
		{
			for (int32 j = 0; j < DrawDatas[i].Num(); ++j)
				delete DrawDatas[i][j];
		}

		DrawDatas.Reset();
		CurrentTime = 0.0f;
		TotalTime = 0.0f;
	}

	void FDrawManager::AddDrawDataList(int32 InCount)
	{
		DrawDatas.AddDefaulted(InCount);
	}

	int32 FDrawManager::PushDrawData(int32 InIndex, FDrawData& InDrawData)
	{
		InDrawData.CheckTime = InDrawData.StartTime + InDrawData.LifeTime;

		if (DrawDatas.IsValidIndex(InIndex))
		{
			return DrawDatas[InIndex].Add(new FDrawData(InDrawData));
		}

		return -1;
	}

	int32 FDrawManager::PushDrawData(FDrawData& InDrawData)
	{
		InDrawData.CheckTime = InDrawData.StartTime + InDrawData.LifeTime;

		if (DrawDatas.IsValidIndex(0))
		{
			return DrawDatas[0].Add(new FDrawData(InDrawData));
		}

		return -1;
	}

	void FDrawManager::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
	{
		if (PDI == nullptr || DrawDatas.Num() == 0) return;

		for (int32 index = 0; index < DrawDatas.Num(); index++)
		{
			for (int32 j = 0; j < DrawDatas[index].Num(); ++j)
			{
				if (DrawDatas[index][j]->StartTime < CurrentTime && CurrentTime < DrawDatas[index][j]->EndTime
					&& DrawDatas[index][j]->CollisionDraw)
				{
					if (DrawDatas[index][j]->DrawType == EDrawType::Circle)
					{
						DrawCircleFast(PDI, DrawDatas[index][j]->TM, DrawDatas[index][j]->MinRadius, DrawDatas[index][j]->MaxRadius,
							FLinearColor(DrawDatas[index][j]->Color.R, DrawDatas[index][j]->Color.G, DrawDatas[index][j]->Color.B, DrawDatas[index][j]->Color.A),
							DrawDatas[index][j]->PosOffset, DrawDatas[index][j]->OffsetX, DrawDatas[index][j]->OffsetY,
							DrawDatas[index][j]->AngleOffset, DrawDatas[index][j]->AngleOffset2, DrawDatas[index][j]->FillMode);
					}
					else
						if (DrawDatas[index][j]->DrawType == EDrawType::Cone)
						{
							DrawConeFast(PDI, DrawDatas[index][j]->TM, DrawDatas[index][j]->MinRadius, DrawDatas[index][j]->MaxRadius,
								DrawDatas[index][j]->DegAngle, DrawDatas[index][j]->Color, DrawDatas[index][j]->PosOffset, DrawDatas[index][j]->OffsetX, DrawDatas[index][j]->OffsetY,
								DrawDatas[index][j]->AngleOffset, DrawDatas[index][j]->AngleOffset2, DrawDatas[index][j]->FillMode);
						}
						else
							if (DrawDatas[index][j]->DrawType == EDrawType::Rect)
							{
								// BSAM - 기획팀으로부터 Width, Height가 반대로 된다고 왔다. 기능상에 이상은 없고 다만 캐릭터를 -90로 기본 셋팅하기에 생기는 이슈이다. 그냥 바꿔준다 잊지 말자 이내용.
								DrawRectFast(PDI, DrawDatas[index][j]->TM, DrawDatas[index][j]->Height, DrawDatas[index][j]->Width, DrawDatas[index][j]->DegAngle, DrawDatas[index][j]->Color,
									DrawDatas[index][j]->PosOffset, DrawDatas[index][j]->OffsetX, DrawDatas[index][j]->OffsetY,
									DrawDatas[index][j]->AngleOffset, DrawDatas[index][j]->AngleOffset2, DrawDatas[index][j]->FillMode);
							}
				}
			}
		}
	}

	FVector CalcConeVert(float Angle1, float Angle2, float AzimuthAngle)
	{
		float ang1 = FMath::Clamp<float>(Angle1, 0.01f, (float)PI - 0.01f);
		float ang2 = FMath::Clamp<float>(Angle2, 0.01f, (float)PI - 0.01f);

		float sinX_2 = FMath::Sin(0.5f * ang1);
		float sinY_2 = FMath::Sin(0.5f * ang2);

		float sinSqX_2 = sinX_2 * sinX_2;
		float sinSqY_2 = sinY_2 * sinY_2;

		float tanX_2 = FMath::Tan(0.5f * ang1);
		float tanY_2 = FMath::Tan(0.5f * ang2);


		float phi = FMath::Atan2(FMath::Sin(AzimuthAngle) * sinY_2, FMath::Cos(AzimuthAngle) * sinX_2);
		float sinPhi = FMath::Sin(phi);
		float cosPhi = FMath::Cos(phi);
		float sinSqPhi = sinPhi * sinPhi;
		float cosSqPhi = cosPhi * cosPhi;

		float rSq, r, Sqr, alpha, beta;

		rSq = sinSqX_2 * sinSqY_2 / (sinSqX_2 * sinSqPhi + sinSqY_2 * cosSqPhi);
		r = FMath::Sqrt(rSq);
		Sqr = FMath::Sqrt(1 - rSq);
		alpha = r * cosPhi;
		beta = r * sinPhi;

		FVector ConeVert;

		ConeVert.X = (1 - 2 * rSq);
		ConeVert.Y = 2 * Sqr * alpha;
		ConeVert.Z = 2 * Sqr * beta;

		return ConeVert;
	}

	void DrawCircleFast(FPrimitiveDrawInterface* PDI, const FTransform& InTransform, float InMinRadius, float InMaxRadius, const FLinearColor& InSideLineColor, float InPosOffset, float InOffsetX, float InOffsetY, float InAngleOffset, float InAngleOffset2, bool FillMode)
	{
		// Max Radian Color
		UMaterial* DrawMaterial = nullptr;
		if (PDI->View->GetFeatureLevel() == ERHIFeatureLevel::ES3_1)
			DrawMaterial = FDrawManager::DrawMaterial;
		else if (PDI->View->GetFeatureLevel() == ERHIFeatureLevel::SM5)
			DrawMaterial = FDrawManager::SM5DrawMaterial;		

		//FQuat rot;
		//rot.Normalize();
		InOffsetX = -InOffsetX;
		FVector centerPos = FVector(InOffsetX, InOffsetY, 0.f);

		FVector XAxis = InTransform.GetUnitAxis(EAxis::X);
		FVector YAxis = InTransform.GetUnitAxis(EAxis::Y);

		// 실상 위에 Draw 머터리얼 EmissiveColor에 셋팅을 하기 때문에 의미는 없지만 인자 전달을 위해 추가해준다.
		FColor XYPlaneColor = FColor(InSideLineColor.R, InSideLineColor.G, InSideLineColor.B, InSideLineColor.A);
		FVector ZAxis = InTransform.GetUnitAxis(EAxis::Z);
		FQuat qDir(ZAxis, FMath::DegreesToRadians(InAngleOffset + 90.0f));
		FVector pos = qDir.Vector() * InPosOffset + centerPos;
		FTransform Transform(pos);
		FQuat qDir2(ZAxis, FMath::DegreesToRadians(InAngleOffset2 + 90.0f));
		Transform.SetRotation(qDir2);
		FVector vPosMove = Transform.GetTranslation();

		UMaterialInstanceDynamic* ConstraintLimitMaterialColor = nullptr;
		if (DrawMaterial)
		{
			ConstraintLimitMaterialColor = UMaterialInstanceDynamic::Create(DrawMaterial, nullptr);
			ConstraintLimitMaterialColor->SetVectorParameterValue(FName("EmissiveColor"), InSideLineColor);
		}
		DrawDisc(PDI, vPosMove, XAxis, YAxis, XYPlaneColor, InMaxRadius, 200.0f, ConstraintLimitMaterialColor->GetRenderProxy(), SDPG_Foreground);

		// Min Radian Color
		UMaterialInstanceDynamic* ConstraintLimitMaterialColor2 = nullptr;
		if (DrawMaterial)
		{
			ConstraintLimitMaterialColor2 = UMaterialInstanceDynamic::Create(DrawMaterial, nullptr);
			ConstraintLimitMaterialColor2->SetVectorParameterValue(FName("EmissiveColor"), FLinearColor::Gray);
		}
		vPosMove.Z += 1.0f;
		DrawDisc(PDI, vPosMove, XAxis, YAxis, XYPlaneColor, InMinRadius, 400.0f, ConstraintLimitMaterialColor2->GetRenderProxy(), SDPG_Foreground);
	}

	void DrawRectFast(FPrimitiveDrawInterface* PDI, const FTransform& InTransform, float InWidth, float InHeight, float InAngle, const FLinearColor& InSideLineColor, float InPosOffset, float InOffsetX, float InOffsetY, float InAngleOffset, float InAngleOffset2, bool FillMode)
	{
		UMaterial* DrawMaterial = nullptr;
		if (PDI->View->GetFeatureLevel() == ERHIFeatureLevel::ES3_1)
			DrawMaterial = FDrawManager::DrawMaterial;
		else if (PDI->View->GetFeatureLevel() == ERHIFeatureLevel::SM5)
			DrawMaterial = FDrawManager::SM5DrawMaterial;		

		float Width = InWidth * 0.5f;
		float Height = InHeight * 0.5f;

		FVector ZAxis = InTransform.GetUnitAxis(EAxis::Z);

		FQuat rot = FQuat::Identity;
		FQuat qDir(ZAxis, FMath::DegreesToRadians(90.0f));
		rot *= qDir;
		
		FVector centerPos = rot.RotateVector(FVector(InOffsetY, InOffsetX, 0.f));
		rot *= FQuat::MakeFromEuler(FVector(0.f, 0.f, InAngleOffset));
		FVector pos = rot.Vector() * InPosOffset + centerPos;
		FTransform tm(rot, pos);

		tm.SetRotation(rot * FQuat::MakeFromEuler(FVector(0.f, 0.f, InAngleOffset2)));

		UMaterialInstanceDynamic* ConstraintLimitMaterialColor = nullptr;
		if (DrawMaterial)
		{
			ConstraintLimitMaterialColor = UMaterialInstanceDynamic::Create(DrawMaterial, nullptr);
			ConstraintLimitMaterialColor->SetVectorParameterValue(FName("EmissiveColor"), InSideLineColor);
		}
		DrawBox(PDI, FScaleMatrix(FVector(Width, Height, 1.0f)) * tm.ToMatrixWithScale(), FVector(1.0f), ConstraintLimitMaterialColor->GetRenderProxy(), SDPG_World);
	}

	void DrawConeFast(FPrimitiveDrawInterface* PDI, const FTransform& InTransform, float InMinRadius, float InMaxRadius, float InAngle, const FLinearColor& InSideLineColor, float InPosOffset, float InOffsetX, float InOffsetY, float InAngleOffset, float InAngleOffset2, bool FillMode)
	{
		UMaterial* DrawMaterial = nullptr;
		if (PDI->View->GetFeatureLevel() == ERHIFeatureLevel::ES3_1)
			DrawMaterial = FDrawManager::DrawMaterial;
		else if (PDI->View->GetFeatureLevel() == ERHIFeatureLevel::SM5)
			DrawMaterial = FDrawManager::SM5DrawMaterial;

		FVector ZAxis = InTransform.GetUnitAxis(EAxis::Z);

		FQuat rot = FQuat::Identity;
		FQuat qDir(ZAxis, FMath::DegreesToRadians(90.0f));
		rot *= qDir;

		FVector centerPos = rot.RotateVector(FVector(InOffsetY, InOffsetX, 0.f));
		rot *= FQuat::MakeFromEuler(FVector(0.f, 0.f, InAngleOffset));
		FVector pos = rot.Vector() * InPosOffset + centerPos;
		FTransform tm(rot, pos);

		tm.SetRotation(rot * FQuat::MakeFromEuler(FVector(0.f, 0.f, InAngleOffset2)));

		float Angle = InAngle * 0.5f;
		const float Swing1Ang = FMath::DegreesToRadians(Angle);
		const float Swing2Ang = -FMath::DegreesToRadians(Angle);

		FMatrix ConeToWorld = FScaleMatrix(FVector(InMaxRadius * 0.9f)) * tm.ToMatrixWithScale();

		UMaterialInstanceDynamic* ConstraintLimitMaterialColor = nullptr;
		if (DrawMaterial)
		{
			ConstraintLimitMaterialColor = UMaterialInstanceDynamic::Create(DrawMaterial, nullptr);
			ConstraintLimitMaterialColor->SetVectorParameterValue(FName("EmissiveColor"), InSideLineColor);
		}
		DrawCone(PDI, ConeToWorld, Swing1Ang, Swing2Ang, 40, false, InSideLineColor, ConstraintLimitMaterialColor->GetRenderProxy(), SDPG_World);
	}
}