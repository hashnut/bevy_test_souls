#pragma once

#include "CoreMinimal.h"
#include "Math/Rotator.h"

// 이클래스는 서버에서 주는 int16타입의 Angle값을 클라이언트 방향으로 포팅해주는 역활을 한다.
// 또한 서버로 방향을 

// degree   | FDir
// 0        | 0
// 90       | 16384
// -90      | -16384
// 180      | 32768
// 181      | -1
// 270      | -16384
// 359      | -32767

const float WZ_PI = 3.141592f;

class FGsDir
{
public:		
	static const int FULL_SHORT = 65536;
	static const int HALF_SHORT = 32768;
	static const int FULL_C360 = 360;
	static const int HALF_C360 = 180;

	static float _sinIndex[FULL_SHORT];
	static float _cosIndex[FULL_SHORT];
	
	static FGsDir* Instance;

public:
	FGsDir()
	{
		for (int i = 0; i < FULL_SHORT; i++)
		{
			_sinIndex[i] = FMath::Sin(angleToRad((int16)(i - HALF_SHORT)));
			_cosIndex[i] = FMath::Cos(angleToRad((int16)(i - HALF_SHORT)));
		}
	}

	static void CleanUp()
	{
		if (nullptr != Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

	static float angleToRad(int16 angle)
	{
		return ((float)angle) * WZ_PI / HALF_SHORT;
	}

	static int16 radToAngle(float rad)
	{
		short angle = (short)(FMath::RoundToInt((rad * HALF_SHORT) / WZ_PI));
		if (HALF_SHORT < angle) angle = (short)(-(HALF_SHORT - (angle % HALF_SHORT)));
		if (-HALF_SHORT > angle) angle = (short)(HALF_SHORT - (abs(angle % HALF_SHORT)));
		return angle;
	}

	static float c360ToRad(int16 c360)
	{
		if (HALF_C360 < c360) c360 = (short)(-(HALF_C360 - (c360 % HALF_C360)));
		if (-HALF_C360 > c360) c360 = (short)(HALF_C360 - (abs(c360 % HALF_C360)));
		return ((float)c360) * WZ_PI / HALF_C360;
	}

	static int16 c360ToAngle(int16 c360)
	{
		return radToAngle(c360ToRad(c360));
	}

	static float c360ToRad(float c360)
	{
		if (HALF_C360 < c360) c360 = (short)(-(HALF_C360 - ((short)c360 % HALF_C360)));
		if (-HALF_C360 > c360) c360 = (short)(HALF_C360 - (abs((short)c360 % HALF_C360)));
		return ((float)c360) * WZ_PI / HALF_C360;
	}

	static int16 radToC360(float rad)
	{
		short c360 = (short)(FMath::RoundToInt((rad * HALF_C360) / WZ_PI));
		if (HALF_C360 < c360) c360 = (short)(-(HALF_C360 - (c360 % HALF_C360)));
		if (-HALF_C360 > c360) c360 = (short)(HALF_C360 - (abs(c360 % HALF_C360)));
		return c360;
	}	

	static FVector angleToDir(int16 angle)
	{
		Access();

		FVector dir = FVector::ZeroVector;
		dir.X = cos(angle);
		dir.Y = sin(angle);
		
		return dir;
	}

	static int16 dirToAngle(FVector dir)
	{
		return (short)(FMath::Atan2(dir.Y, dir.X) * HALF_SHORT / WZ_PI);
	}

	static float AngleToDegree(int16 InServer)
	{
		return radToC360(angleToRad(InServer));
	}

	static FRotator AngleToRotator(int16 InServer)
	{
		return FRotator(0, radToC360(angleToRad(InServer)), 0);
	}

	static int16 DgreeToAngle(float InDegree)
	{
		return radToAngle(c360ToRad(InDegree));
	}

	static int16 RotatorToAngle(const FRotator& InRot)
	{
		return DgreeToAngle(InRot.Yaw);
	}

private:
	static void Access()
	{
		if (nullptr == Instance)
		{
			Instance = new FGsDir();
		}
	}

	void* operator new(size_t size)
	{
		return FMemory::Malloc(size);
	}

	void operator delete(void* ptr)
	{
		return FMemory::Free(ptr);
	}

	static int16 abs(int16 angle)
	{
		return (int16)((angle >= 0) ? angle : -angle);
	}

	static float sin(int16 angle)
	{
		return _sinIndex[angle + HALF_SHORT];
	}

	static float cos(int16 angle)
	{
		return _cosIndex[angle + HALF_SHORT];
	}
};
