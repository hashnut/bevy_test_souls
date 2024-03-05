//~=============================================================================
// DrawTools
// 디버그용이든 게임용이든 간편하게 Draw에 필요한 함수들(Debug류는 느리고 질이 너무 나쁨)
//~=============================================================================

#pragma once

namespace DrawTools
{
	enum class EDrawType : uint8
	{
		None,
		Circle,
		Rect,
		Cone,
		Max
	};

	class FDrawData
	{
	public:
		FDrawData(EDrawType InDrawType = EDrawType::None, FTransform InTM =FTransform::Identity, float InRadius = 0.0f, float InDegAngle = 0.0f, float InLifeTime = 0.3f, float InStartTime = 0.0f, bool InFillMode = true)
			: DrawType(InDrawType),
			TM(InTM),
			Radius(InRadius),
			DegAngle(InDegAngle),
			LifeTime(InLifeTime),
			StartTime(InStartTime),
			FillMode(InFillMode),
			CollisionDraw(false)
		{
			CheckTime = InStartTime + InLifeTime;
		}

		EDrawType DrawType;
		FTransform TM;
		float Radius;
		float DegAngle;
		float LifeTime;
		float Width;
		float Height;
		float StartTime;
		float EndTime;
		float CheckTime;
		FLinearColor Color;
		bool FillMode;
		bool CollisionDraw;
		float PosOffset;
		float OffsetX;
		float OffsetY;
		float AngleOffset;
		float AngleOffset2;
		float MinRadius;
		float MaxRadius;
	};

	class FDrawManager
	{
	public:
		FDrawManager(float* InCurrentTime = nullptr);

		static UMaterial* DrawMaterial;
		static UMaterial* SM5DrawMaterial;

	private:
		float CurrentTime;
		float TotalTime;

		TArray<TArray<class FDrawData*>> DrawDatas;
		
	public:
		TArray<class FDrawData*>& GetDrawDatas(int32 InIndex);
		void SetCurrentTime(float InCurrentTime) { CurrentTime = InCurrentTime; }
		float GetCurrentTime() { return CurrentTime;  }

		void ClearDrawData();
		void ClearDrawData(int32 InSectionIndex);
		int32 PushDrawData(int32 InIndex, FDrawData& InDrawData);
		int32 PushDrawData(FDrawData& InDrawData);
		void AddDrawDataList(int32 InCount);
		void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI);
	};

	SKILLEDITOR_API void DrawCircleFast(FPrimitiveDrawInterface* PDI, const FTransform& InTransform, float InMinRadius, float InMaxRadius, const FLinearColor& InSideLineColor, float InPosOffset = 0.0f, float InOffsetX = 0.0f, float InOffsetY = 0.0f, float InAngleOffset = 0.0f, float InAngleOffset2 = 0.0f, bool FillMode = true);
	SKILLEDITOR_API void DrawRectFast(FPrimitiveDrawInterface* PDI, const FTransform& InTransform, float InWidth, float InHeight, float InAngle, const FLinearColor& InSideLineColor, float InPosOffset = 0.0f, float InOffsetX = 0.0f, float InOffsetY = 0.0f, float InAngleOffset = 0.0f, float InAngleOffset2 = 0.0f, bool FillMode=true);
	SKILLEDITOR_API void DrawConeFast(FPrimitiveDrawInterface* PDI, const FTransform& InTransform, float InMinRadius, float InMaxRadius, float InAngle, const FLinearColor& InSideLineColor, float InPosOffset = 0.0f, float InOffsetX = 0.0f, float InOffsetY = 0.0f, float InAngleOffset = 0.0f, float InAngleOffset2 = 0.0f, bool FillMode = true);
}