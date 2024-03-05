// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillCollisionComponent.h"
#include "Classes/Components/MeshComponent.h"
#include "Classes/Materials/Material.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

//#include "GameObject/Skill/Data/GsSkillNotifyDataCollision.h"
#include "ActorEx/GsCharacterNonPlayer.h"
#include "Util/GsMath.h"
#include "T1Project.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

// Sets default values for this component's properties
UGsSkillCollisionComponent::UGsSkillCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
#if (WITH_EDITOR)
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	bTickInEditor = true;
#endif
	// ...
}

void UGsSkillCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGsSkillCollisionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UGsSkillCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if (WITH_EDITOR)
	if (GetWorld() && !GetWorld()->IsGameWorld() && _useDebugLine)
	{
		DrawCollision();
	}
#endif
	//FVector dest = GetComponentLocation() + AttachScene->GetForwardVector() * 100.f;
	//DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), dest, 20.f, FColor::Black);
}

void UGsSkillCollisionComponent::Set(const FGsSchemaSkillCollision& Data)
{
	// AttachScene TM 초기화
	ResetRelativeTransform();

	SetRelativeRotation(_defaultRotator);

	FQuat rot = GetRelativeRotation().Quaternion() * GetOwner()->GetActorRotation().Quaternion();
	// 1. 기준 위치 변경 (offsetX, offsetY)
	// offsetX = RightVector, offsetY = ForwardVector
	// 문서 : B1_24.1.39_스킬 컬리전 영역 offset 관련 기능 추가_new
	FVector centerPos = rot.RotateVector(FVector(Data.offsetY, Data.offsetX, 0.f));

 	// 2. 회전 반영(angleOffset)
 	rot *= FQuat::MakeFromEuler(FVector(0.f,0.f, Data.angleOffset));
 	// 3. 거리 반영(posOffset)
 	FVector pos = rot.Vector() * Data.posOffset + centerPos;
 	
 	FTransform tm(rot, pos);

 	// 4. 최종 회전 반영 (angleOffset2)
 	tm.SetRotation(rot * FQuat::MakeFromEuler(FVector(0.f, 0.f, Data.angleOffset2)));	
	
	// SetRelativeTransform(tm);
	// SetRelativeTransform(tm); 을 쓰면 스케일까지 반영되어 offset Postion에 영향있음
	MoveComponent(tm.GetTranslation(), tm.GetRotation(), false);

	SetCollision(Data);
}

//임의의 좌표로 설정할 경우(ex 타겟팅)
void UGsSkillCollisionComponent::Set(const FVector& Location, const FGsSchemaSkillCollision& Data)
{
	ResetRelativeTransform();
	SetRelativeRotation(_defaultRotator);

	SetWorldLocation(Location);

	SetCollision(Data);
}

void UGsSkillCollisionComponent::SetCollision(const FGsSchemaSkillCollision& Data)
{
	_collisionData.Empty();
	_collisionData._type = Data.type;
	switch (_collisionData._type)
	{
	case SkillCollisionType::CIRCLE:
		SetCircle(Data);
		break;
	case SkillCollisionType::FAN:
		SetFan(Data);
		break;
	case SkillCollisionType::RECT:
		SetConcave(Data);
		break;
	}

#if (WITH_EDITOR)
	if (_useDebugLine)
	{
		DrawCollision(_debugLineLifeTime);
	}
#endif
}

void UGsSkillCollisionComponent::SetCircle(const FGsSchemaSkillCollision& Data)
{
	_collisionData._maxRadius = Data.maxRadius;
	_collisionData._minRadius = Data.minRadius;
}

void UGsSkillCollisionComponent::SetFan(const FGsSchemaSkillCollision& Data)
{
	_collisionData._maxRadius = Data.maxRadius;
	_collisionData._minRadius = Data.minRadius;
	_collisionData._degAngle = Data.angle;
}

void UGsSkillCollisionComponent::SetConcave(const FGsSchemaSkillCollision& Data)
{
	//Concave.ListLine = Data->ListPoint;

	// width / height 로 직접 계산
	// Rect타입만 지원됨
	float halfW = Data.width * 0.5f;
	float halfH = Data.height * 0.5f;

	const FTransform& tm = GetComponentToWorld();
	float zPos = tm.GetLocation().Z;
	FVector lt = tm.TransformPositionNoScale(FVector(-halfH, -halfW, zPos));
	FVector rt = tm.TransformPositionNoScale(FVector(halfH, -halfW, zPos));
	FVector rb = tm.TransformPositionNoScale(FVector(halfH, halfW, zPos));
	FVector lb = tm.TransformPositionNoScale(FVector(-halfH, halfW, zPos));
	//lt.Z = zPos;
	//rt.Z = zPos;
	//rb.Z = zPos;
	//lb.Z = zPos;

	_collisionData._listLine.Emplace(lt);
	_collisionData._listLine.Emplace(rt);
	_collisionData._listLine.Emplace(rb);
	_collisionData._listLine.Emplace(lb);
}

bool UGsSkillCollisionComponent::IsInSideCheck(const FVector& Point) const
{
	if (GetWorld() && _useDebugLine)
	{
		DrawDebugSphere(GetWorld(), Point, 5.f, 100, _debugColor, false, 2.f);
	}

	switch (_collisionData._type)
	{
	case SkillCollisionType::CIRCLE:
		return IsInSideCircle(Point);
	case SkillCollisionType::FAN:
		return IsInSideFan(Point);
	case SkillCollisionType::RECT:
		break;
		//return IsInSideConcave(Point);
	}

	return false;
}

// AttachScene 의 현재 좌표 정보 얻기
FVector UGsSkillCollisionComponent::GetCollisionLocation() const
{
	return GetComponentLocation();
}
// AttachScene 의 현재 회전 정보 얻기
FRotator UGsSkillCollisionComponent::GetCollisionRotate() const
{
	return GetComponentRotation();
}

//int UGsSkillCollisionComponent::GetRectZone(const FBox& Rect, const FVector& Point) const
//{
//	// 현재 좌표에서 검출 영역 Index 찾기
//	// 참고 : http://dolphin.ivyro.net/file/mathematics/tutorial07.html
//	int xZone = (Point.X <  Rect.Min.X) ? 0 :
//		(Point.X >  Rect.Max.X) ? 2 : 1;
//	int yZone = (Point.Y <  Rect.Min.Y) ? 0 :
//		(Point.Y >  Rect.Max.Y) ? 2 : 1;
//	return xZone + 3*yZone;
//}
//
//bool UGsSkillCollisionComponent::IsInSideRect2D(const FBox& Rect, const FVector& Point, float Radius) const
//{
//	const FVector& center = Rect.GetCenter();
//	// FRect 클래스는 회전에 버그가 있는것 같다. 따로 half값 저장
//	float halfHeight = CollisionData.HalfHeight;
//	float halfWidth = CollisionData.HalfWidth;
//
//	// 검사 영역 확인
//	int nZone = GetRectZone(Rect, Point);
//	switch (nZone)
//	{
//		// top, bottom 변의 영역에서, 원의 센터와 수직거리를 검사한다.
//	case 1:
//	case 7:
//	{
//		float distY = fabs(Point.Y - center.Y);
//		if (distY <= (Radius + halfHeight))
//		{
//			return true;
//		}
//	}
//	break;
//
//	// left, right 변의 영역에서. 원의 센터와 수평거리를 검사한다.
//	case 3:
//	case 5:
//	{
//		float distX = fabs(Point.X - center.X);
//		if (distX <= (Radius + halfWidth))
//		{
//			return true;
//		}
//	}
//	break;
//	// 사각형 영역의 내부
//	case 4:
//		return true;
//		// 모서리 영역, 모서리가 원의 내부에 포함되는지 검사한다.
//	default:
//	{
//		FVector corver;
//		corver.X = (nZone == 0 || nZone == 6) ? Rect.Min.X : Rect.Max.X;
//		corver.Y = (nZone == 0 || nZone == 2) ? Rect.Min.Y : Rect.Max.Y;
//
//		FVector dir = corver - Point;
//		if (dir.SizeSquared() <= Radius * Radius)
//		{
//			return true;
//		}
//	}
//	break;
//	}
//
//	return false;
//}

//bool UGsSkillCollisionComponent::IsInSideConcave(const FVector& Point) const
//{
//	return IsInSideConcave2D(FVector2D(Point.X, Point.Y));
//}

// 다각형 처리때 사용
//bool UGsSkillCollisionComponent::IsInSideConcave2D(const FVector2D& Point) const
//{
//	int ptcount = CollisionData.ListLine.Num();
//	if (ptcount < 3)
//		return false;
//
//	bool inside = false;
//	auto worldPos = FVector2D(AttachScene->GetComponentLocation().X, AttachScene->GetComponentLocation().Y);
//	auto newPos = CollisionData.ListLine[ptcount - 1] + worldPos;
//
//	uint32 xold = newPos.X;
//	uint32 yold = newPos.Y;
//
//	uint32 xnew = 0;
//	uint32 ynew = 0;
//	uint32 x1 = 0;
//	uint32 y1 = 0;
//	uint32 x2 = 0;
//	uint32 y2 = 0;
//
//	for (int i = 0; i < ptcount; ++i)
//	{
//		newPos = CollisionData.ListLine[i] + worldPos;
//		xnew = newPos.X;
//		ynew = newPos.Y;
//		if (xnew > xold) {
//			x1 = xold;
//			x2 = xnew;
//			y1 = yold;
//			y2 = ynew;
//		}
//		else {
//			x1 = xnew;
//			x2 = xold;
//			y1 = ynew;
//			y2 = yold;
//		}
//		if ((xnew < Point.X) == (Point.X <= xold)          /* edge "open" at one end */
//			&& ((long)Point.Y - (long)y1)*(long)(x2 - x1)
//			< ((long)y2 - (long)y1)*(long)(Point.X - x1)) {
//			inside = !inside;
//		}
//		xold = xnew;
//		yold = ynew;
//	}
//
//	return(inside);
//}

bool UGsSkillCollisionComponent::IsInSideCircle(const FVector& Point) const
{
	return IsInSideCircle2D(FVector2D(Point.X, Point.Y));
}

bool UGsSkillCollisionComponent::IsInSideCircle2D(const FVector2D& Point) const
{
	auto worldPos = FVector2D(GetComponentLocation().X, GetComponentLocation().Y);
	auto dir = Point - worldPos;

	// Min 값이 존재하면 체크
	if (_collisionData._minRadius > 0.f)
	{
		if (dir.SizeSquared() < _collisionData._minRadius * _collisionData._minRadius)
		{
			return false;
		}
	}

	return dir.SizeSquared() <= _collisionData._maxRadius * _collisionData._maxRadius;
}

bool UGsSkillCollisionComponent::IsInSideFan(const FVector& Point) const
{
	return IsInSideFan2D(FVector2D(Point.X, Point.Y));
}

bool UGsSkillCollisionComponent::IsInSideFan2D(const FVector2D& Point) const
{
	auto worldPos = FVector2D(GetComponentLocation().X, GetComponentLocation().Y);
	auto dir = Point - worldPos;
	// Min 값이 존재하면 체크
	if (_collisionData._minRadius > 0.f)
	{
		if (dir.SizeSquared() < _collisionData._minRadius * _collisionData._minRadius)
		{
			return false;
		}
	}

	if (dir.SizeSquared() <= _collisionData._maxRadius * _collisionData._maxRadius)
	{
		auto fanDir = GetComponentRotation().Vector();
		auto fanDir2D = FVector2D(fanDir.X, fanDir.Y);
		auto targetDir = dir.GetSafeNormal();

		//float targetLen = dir.Size();
		//DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), GetComponentLocation() + fanDir * targetLen, 20.f, FColor::Yellow);
		//DrawDebugDirectionalArrow(GetWorld(), GetComponentLocation(), GetComponentLocation() + FVector(targetDir, GetComponentLocation().GetSafeNormal().Z) * targetLen, 20.f, FColor::Blue);

		float dot = FVector2D::DotProduct(fanDir2D, targetDir);
		dot = FMath::Acos(dot);
		dot = FMath::RadiansToDegrees(dot);

		return dot <= _collisionData._degAngle * 0.5f;
	}

	return false;
}

bool UGsSkillCollisionComponent::IsInsideCheck(const FVector& Point, float Radius) const
{
	bool bInside = false;
	switch (_collisionData._type)
	{
	case SkillCollisionType::CIRCLE:
		bInside = IsInSideCircle(Point, Radius);
		break;
	case SkillCollisionType::FAN:
		bInside = IsInSideFan(Point, Radius);
		break;
	case SkillCollisionType::RECT:
	{
		bInside = IsInSideRect(Point, Radius);
		//bInside = IsInSideRect2D(FBox(CollisionData.ListLine), Point, Radius);
		//const FTransform& tm = AttachScene->GetComponentTransform();
		////FVector lt = tm.InverseTransformPositionNoScale(FVector(CollisionData.ListLine[0]));
		//bInside = FMath::SphereAABBIntersection(FSphere(Point, Radius),
		//	FBox::BuildAABB(AttachScene->GetComponentLocation(), CollisionData.Rect.GetExtent()));
		break;
	}
	}
	/*
	if (GetWorld() && bInside && Type == SkillCollisionType::RECT)
	{
		DrawDebugSphere(GetWorld(), Point, Radius, 100, FColor::Red, false, 2.f);
	}
	*/
	return bInside;
}
bool UGsSkillCollisionComponent::IsInSideRect(const FVector& Point, float Radius) const
{
	return IsInSideRect2D(FVector2D(Point.X, Point.Y), Radius);
}

bool UGsSkillCollisionComponent::IsInSideRect2D(const FVector2D& Point, float Radius) const
{
	int ptcount = _collisionData._listLine.Num();
	if (ptcount != 4)
		return false;

	//(-50, 50) 0-------------------1 (50, 50)  
	//			|					|
	//			|					|
	//			|					|
	//			|					|
	//(-50,-50) 3)------------------2 (50,-50)
	
	auto worldPos = FVector2D(GetComponentLocation().X, GetComponentLocation().Y);
	// 로컬 좌표로 변경
	FVector targetLocalPos2D = FVector(Point.X, Point.Y, 0.0f) - FVector(worldPos.X, worldPos.Y, 0);

	// 내가 회전한 반대축으로 변환된 좌표를 회전
	float rotAngle = -(GetComponentRotation().Euler().Z);
	targetLocalPos2D = targetLocalPos2D.RotateAngleAxis(rotAngle, FVector::UpVector);

	// 각 좌표를 로컬 좌표로 변경
	const FTransform& tm = GetComponentToWorld();
	TArray<FVector> listVector;
	listVector.Emplace(tm.InverseTransformPositionNoScale(_collisionData._listLine[0]));
	listVector.Emplace(tm.InverseTransformPositionNoScale(_collisionData._listLine[1]));
	listVector.Emplace(tm.InverseTransformPositionNoScale(_collisionData._listLine[2]));
	listVector.Emplace(tm.InverseTransformPositionNoScale(_collisionData._listLine[3]));
	FBox rect(listVector);

	// 내 로컬좌표계로 변환완료했기때문에 로컬 좌표의 AABB검사가 가능하다
	FVector2D covert2d = FVector2D(targetLocalPos2D.X, targetLocalPos2D.Y);
	FVector2D min = FVector2D(rect.Min);
	FVector2D max = FVector2D(rect.Max);
	return FGsMath::SphereAABBIntersection2D(covert2d, Radius*Radius, min, max);
}

bool UGsSkillCollisionComponent::IsInSideCircle(const FVector& Point, float Radius) const
{
	return IsInSideCircle2D(FVector2D(Point.X, Point.Y), Radius);
}

bool UGsSkillCollisionComponent::IsInSideCircle2D(const FVector2D& Point, float Radius) const
{
	auto worldPos = FVector2D(GetComponentLocation().X, GetComponentLocation().Y);
	auto dir = Point - worldPos;
	// Min 값이 존재하면 체크
	if (_collisionData._minRadius > 0.f)
	{
		if (dir.SizeSquared() < _collisionData._minRadius * _collisionData._minRadius)
		{
			return false;
		}
	}
	float sizeSquared = dir.SizeSquared();
	return sizeSquared <= (_collisionData._maxRadius * _collisionData._maxRadius);
}

bool UGsSkillCollisionComponent::IsInSideFan(const FVector& Point, float Radius) const
{
	return IsInSideFan2D(FVector2D(Point.X, Point.Y), Radius);
}

//bak1210
//대상의 좌표와 반지름
bool UGsSkillCollisionComponent::IsInSideFan2D(const FVector2D& Point, float Radius) const
{
	FVector2D worldPos = FVector2D(GetComponentLocation().X, GetComponentLocation().Y);
	FVector2D dir = Point - worldPos;
	// 대상좌표와 원점(판정영역 오리진)의 거리를 구한다.
	float squaredDist = dir.SizeSquared();
	// Min 값이 존재하면 체크
	if (_collisionData._minRadius > 0.f)
	{
		if (squaredDist < _collisionData._minRadius * _collisionData._minRadius)
		{
			return false;
		}
	}

	float checkRadius = Radius + _collisionData._maxRadius;
	//(판정영역 반지름+대상의 반지름)이 두점사이 거리보다 크다면 거리조건은 충족
	if (squaredDist <= checkRadius * checkRadius )
	{
		
		auto fanDir = GetComponentRotation().Vector();
		auto fanDir2D = FVector2D(fanDir.X, fanDir.Y);
		auto targetDir = dir.GetSafeNormal();

		/*acos(x)함수의 인자로 들어가는 값은
		- 1 >= x <= 1
		acos(-1) ~acos(1)이라는 범위는 최종 라디안 값의 0 ~파이 값

		acos(-1) = 0; [두벡터의 각이 0도를 의미]
		acos(0) = 파이 / 2; [두벡터의 각이 90도를 의미]
		acos(1) = 파이; [두벡터의 각이 180도를 의미]*/

		float dot = FVector2D::DotProduct(fanDir2D, targetDir);
		float radianAngle = FMath::Acos(dot);
		float calcHalfAngle = FMath::RadiansToDegrees(radianAngle);

		//정면방향벡터 와 타겟방향벡터계산임(정면에서 각도를 체크했음으로 반의크기로 충돌정보 반분각도보다 작으면 포함관계
		float halfAngle = _collisionData._degAngle * 0.5f;
		if (calcHalfAngle <= halfAngle)
		{
			return true;
		}

		// 아래부터 대상 각도보다 크게 있지만 볼륨(반지름)을가진 객체임으로 외각선에 반지름이 걸쳐진 상황처리

		// 반분백터만큼 왼쪽으로 이동한 방향을 구한다.
		FVector leftDir = fanDir.RotateAngleAxis(-halfAngle, FVector::UpVector);
		FVector2D leftDir2D(leftDir.X, leftDir.Y); leftDir2D.Normalize();
		//투영된 점과 실제 반지름을 비교해서 교차 판정한다.
		float leftDist = FGsMath::PointDistToOnLine(Point, leftDir2D, worldPos);
		if (Radius >= leftDist)
		{
			return true;
		}

		// 반분백터만큼 오른쪽으로 이동한 방향을 구한다.
		FVector rightDir = fanDir.RotateAngleAxis(halfAngle, FVector::UpVector);
		FVector2D rightDir2D(rightDir.X, rightDir.Y); rightDir2D.Normalize();
		//투영된 점과 실제 반지름을 비교해서 교차 판정한다.
		float rightDist = FGsMath::PointDistToOnLine(Point, rightDir2D, worldPos);
		if (Radius >= rightDist)
		{
			return true;
		}
	}

	return false;
}

#if WITH_EDITOR
void UGsSkillCollisionComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	PrimaryComponentTick.bCanEverTick = _useDebugLine;
	Set(_debugNotifyData);
}

void UGsSkillCollisionComponent::DrawCollision(float DrawTime)
{
	if (UWorld* World = GetOwner()->GetWorld())
	{
		FVector2D currPos = FVector2D(GetComponentLocation().X, GetComponentLocation().Y);

		// 천장이 있는 형태의 맵이 있기때문에 현재 좌표에서 약 5m정도만 올림
		FVector StartLocation{ currPos.X, currPos.Y, GetComponentLocation().Z + 500.f };
		FVector EndLocation{ currPos.X, currPos.Y, -10000 };

		// Raytrace for overlapping actors.
		FHitResult HitResult;
		World->LineTraceSingleByObjectType(
			OUT HitResult,
			StartLocation,
			EndLocation,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
			FCollisionQueryParams()
		);
		
		if (HitResult.GetActor()) 
			// Root의 z값을 보정치로 사용
			_debugLineHeight = HitResult.ImpactPoint.Z + 50.f + GetRelativeTransform().GetLocation().Z;
		else
		{
			_debugLineHeight = GetOwner()->GetActorLocation().Z + 50.f;
		}
	}

	switch (_collisionData._type)
	{
	case SkillCollisionType::CIRCLE:
		DrawCircle(DrawTime);
		break;
	case SkillCollisionType::FAN:
		DrawFan(DrawTime);
		break;
	case SkillCollisionType::RECT:
		DrawConcave(DrawTime);
		break;
	}
}

void UGsSkillCollisionComponent::DrawCircle(float DrawTime)
{
	auto worldPos = GetComponentLocation();
	worldPos.Z = _debugLineHeight;

	DrawDebugCircle(
		GetWorld(), worldPos, _collisionData._maxRadius, 50, _debugColor,
		false, DrawTime, 0, 0.f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f)
	);

	if (_collisionData._minRadius > 0.f)
	{
		DrawDebugCircle(
			GetWorld(), worldPos, _collisionData._minRadius, 50, _debugColor,
			false, DrawTime, 0, 0.f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f)
		);
	}
}

void UGsSkillCollisionComponent::DrawFan(float DrawTime)
{
	auto worldPos = GetComponentLocation();
	worldPos.Z = _debugLineHeight;

	auto dir = GetComponentRotation().Vector();
	float halfAngle = _collisionData._degAngle * 0.5f;

	auto hypotenuse1Pos = worldPos;
	auto hypotenuse2Pos = worldPos;
	auto hypotenuse1 = worldPos + dir.RotateAngleAxis(halfAngle, FVector::UpVector) * _collisionData._maxRadius;
	if (_collisionData._minRadius > 0.f)
	{
		hypotenuse1Pos = worldPos + dir.RotateAngleAxis(halfAngle, FVector::UpVector) * _collisionData._minRadius;
	}
	auto hypotenuse2 = worldPos + dir.RotateAngleAxis(-halfAngle, FVector::UpVector) * _collisionData._maxRadius;
	if (_collisionData._minRadius > 0.f)
	{
		hypotenuse2Pos = worldPos + dir.RotateAngleAxis(-halfAngle, FVector::UpVector) * _collisionData._minRadius;
	}
	
	DrawDebugLine(GetWorld(), hypotenuse1Pos, hypotenuse1, _debugColor, false, DrawTime);
	DrawDebugLine(GetWorld(), hypotenuse2Pos, hypotenuse2, _debugColor, false, DrawTime);

	/*
	DrawDebugCircle(
		GetWorld(), worldPos, Fan.Radius, 50, FColor::White,
		false, -1.f, 0, 0.f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f)
	);*/

	int count = (int)halfAngle * 3;
	float unit = halfAngle / count;
	for (int i = 0; i < count; ++i)
	{
		float delta = halfAngle - i * unit;
		FVector dirl = dir.RotateAngleAxis(-delta, FVector::UpVector).GetSafeNormal();
		FVector dirr = dir.RotateAngleAxis(delta, FVector::UpVector).GetSafeNormal();
		FVector tempFanl = worldPos + dirl * _collisionData._maxRadius;
		FVector tempFanr = worldPos + dirr * _collisionData._maxRadius;

		DrawDebugPoint(GetWorld(), tempFanl, 1.f, _debugColor, false, DrawTime);
		DrawDebugPoint(GetWorld(), tempFanr, 1.f, _debugColor, false, DrawTime);

		if (_collisionData._minRadius > 0.f)
		{
			tempFanl = worldPos + dirl * _collisionData._minRadius;
			tempFanr = worldPos + dirr * _collisionData._minRadius;

			DrawDebugPoint(GetWorld(), tempFanl, 1.f, _debugColor, false, DrawTime);
			DrawDebugPoint(GetWorld(), tempFanr, 1.f, _debugColor, false, DrawTime);
		}
	}
}

void UGsSkillCollisionComponent::DrawConcave(float DrawTime)
{
	if (_collisionData._listLine.IsValidIndex(0) == false) return;

	const FTransform& tm = GetComponentToWorld();
	float zPos = tm.GetLocation().Z;
	FVector startpos = _collisionData._listLine[0];
	startpos.Z = zPos;
	FVector nextPos = startpos;

	for (int i = 1; i < _collisionData._listLine.Num(); ++i)
	{
		FVector endpos = _collisionData._listLine[i];
		endpos.Z = zPos;
		DrawDebugLine(GetWorld(), nextPos, endpos, FColor::Yellow, false, DrawTime + 1.f);
		nextPos = endpos;
	}

	DrawDebugLine(GetWorld(), startpos, nextPos, FColor::Yellow, false, DrawTime + 1.f);
}

#endif



