// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Object.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "T1Project/Json/JsonParserInterface.h"
#include "JsonLoader.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UJsonLoader : public UObject
{
	GENERATED_BODY()

public:	
	template<typename T = IJsonParserInterface>
	static TSharedPtr<FJsonRoot> CreateJson(FString in_path)
	{
		FString contents;
		TSharedPtr<FJsonObject> jsonObj;

		if(TryReadJson(jsonObj, in_path))
		{		
			if (T* parser = NewObject<T>())
			{				
				return parser->Parse(jsonObj);
			}
		}	

		return nullptr;
	}

private:
	static bool TryReadJson(TSharedPtr<FJsonObject>& out_json, const FString& in_path);
};