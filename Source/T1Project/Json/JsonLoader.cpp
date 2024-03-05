// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonLoader.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "T1Project.h"

bool UJsonLoader::TryReadJson(TSharedPtr<FJsonObject>& out_json, const FString& in_path)
{
	FString contents;	

	if (false == FFileHelper::LoadFileToString(contents, *in_path))
	{
		FString log = TEXT("Missing json path : ") + in_path;
		GSLOG(Error, TEXT("%s"), *log);

		return false;
	}	

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(contents);

	if (false == FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		FString log = TEXT("Terrain Json serialize fail : ") + in_path;
		GSLOG(Error, TEXT("%s"), *log);

		return false;		
	}	

	if (false == JsonObject.IsValid())
	{
		FString log = TEXT("Terrain Json invalid : ") + in_path;
		GSLOG(Error, TEXT("%s"), *log);
		
		return false;
	}

	out_json = JsonObject;

	return true;
}