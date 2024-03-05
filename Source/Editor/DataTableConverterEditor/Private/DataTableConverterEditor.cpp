#include "DataTableConverterEditor.h"

#include "Editor/PropertyEditor/Public/PropertyEditorDelegates.h"
#include "Runtime/Core/Public/Containers/StringConv.h"
#include "Runtime/Core/Public/CoreGlobals.h"
#include "Runtime/Core/Public/Misc/CoreDelegates.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Runtime/CoreUObject/Public/UObject/Stack.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Runtime/Online/HTTP/Public/Interfaces/IHttpRequest.h"

#include "../Plugins/RSQL/Source/RSQLEditor/Public/PropertyTypeCustomization/GsNameSelectorCustomization.h"

#include "Item/GsStatTypeNameSelector.h"
#include "Item/GsResistStatTypeNameSelector.h"
#include "Item/GsImmuneStatTypeNameSelector.h"
#include "Mail/GsMailCategoryMain.h"
#include "Mail/GsMailCategorySub.h"
#include "PropertyEditor/Public/PropertyEditorModule.h"

void FDataTableConverterEditor::StartupModule()
{
	if (!_onHandleSystemErrorDelegate.IsValid())
	{
		_onHandleSystemErrorDelegate = FCoreDelegates::OnHandleSystemError.AddStatic(&FDataTableConverterEditor::SendLocalCrashReport);
	}

	_postImportExeclBinder.Bind();

	RegisterPropertyTypeCustomizations();
}
void FDataTableConverterEditor::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		// Unregister all structures
		for (const FName& PropertyTypeName : RegisteredPropertyTypes)
		{
			PropertyModule.UnregisterCustomPropertyTypeLayout(PropertyTypeName);
		}

		PropertyModule.NotifyCustomizationModuleChanged();
	}

	_postImportExeclBinder.UnBind();

	FCoreDelegates::OnHandleSystemError.Remove(_onHandleSystemErrorDelegate);
	_onHandleSystemErrorDelegate.Reset();
}

void FDataTableConverterEditor::SendLocalCrashReport()
{
	/* 크래시 콜백이 2회 호출되는 문제를 해결하기 위해 아래 코드를 사용하였으나, 더 많은 관련 정보가 필요하여 임시로 주석 처리함.
	if (GIsRunning)
	{
		// VS에서 디버그로 실행하면, 본 함수가 1회만 호출됨.
		// 이유를 잘 모르겠으나, 디버그로 실행하지 않고, 직접 프로그램을 실행하면, 크래시 메세지 뜨기 전에 1회, 크래시 메세지를 닫은 후 1회, 총 두 번 호출됨.
		// 두번째 호출될 때에 의미있는 로그가 많이 남아 있으므로, 첫번째 호출을 무시하기 위한 조건문.
		return;
	}
	*/

	// const FString Callstack = FFrame::GetScriptCallstack(); blueprint call stack

	// Jenkins External Monitor Job 특성 상, 로그를 일반 문자열이 아닌 utf-8 을 16비트로 변경한 문자열을 생성해서 보내야 함.
	const FTCHARToUTF8 ErrorHistUtf8(GErrorHist);
	const FString ErrorHistUtf8Hex = FString::FromHexBlob((const uint8*)ErrorHistUtf8.Get(), ErrorHistUtf8.Length());

	const FTCHARToUTF8 HLineUtf8(TEXT("\n============================\n\n"));
	const FString HLineUtf8Hex = FString::FromHexBlob((const uint8*)HLineUtf8.Get(), HLineUtf8.Length());

	// 로그가 아닌 경우에는 일반 문자열도 상관없으나, XML 이스케이프는 필요함.
	const FString Description = FString(GErrorHist).TrimStart().TrimEnd().Left(1 << 8).Replace(TEXT("<"), TEXT("&gt;")).Replace(TEXT("&"), TEXT("&amp;"));

	// <run><log>로그내용</log><result>[0|1] (성공|실패)</result><displayName>빌드이름</displayName><description>빌드설명</description></run>
	const FString CrashReportXml = FString::Printf(TEXT("<run><log>%s%s%s</log><result>0</result><description>%s</description></run>"), *HLineUtf8Hex, *ErrorHistUtf8Hex, *HLineUtf8Hex, *Description);
	//UE_LOG(LogTemp, Error, TEXT("CrashReportXml:\n%s"), *CrashReportXml);

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("http://raon_build:11f0434230211288dba4027e6ee4c11e4d@10.28.0.97:8080/job/CrashReporter/postBuildResult"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("Content-Type:text/plain; CharSet=utf-8"));
	Request->SetContentAsString(CrashReportXml);
	Request->ProcessRequest();
}

void FDataTableConverterEditor::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	check(PropertyTypeName != NAME_None);

	RegisteredPropertyTypes.Add(PropertyTypeName);

	static FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}

void FDataTableConverterEditor::RegisterPropertyTypeCustomizations()
{
	// 네임셀렉터로 사용하고자 하는 모든 클래스는 커스터마이즈 된 에디터 UI 사용을 위해, 아래와 같은 방식으로 클래스를 등록함.
	// Customizations for the name lists of our custom types, like Fixture Categories
	RegisterCustomPropertyTypeLayout(FGsStatTypeNameSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGsCustomizationFactory::MakeInstance<FGsNameSelectorCustomization<FGsStatTypeNameSelector>>));
	RegisterCustomPropertyTypeLayout(FGsMailCategoryMain::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGsCustomizationFactory::MakeInstance<FGsNameSelectorCustomization<FGsMailCategoryMain>>));
	RegisterCustomPropertyTypeLayout(FGsMailCategorySub::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGsCustomizationFactory::MakeInstance<FGsNameSelectorCustomization<FGsMailCategorySub>>));

	RegisterCustomPropertyTypeLayout(FGsResistStatTypeNameSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGsCustomizationFactory::MakeInstance<FGsNameSelectorCustomization<FGsResistStatTypeNameSelector>>));
	RegisterCustomPropertyTypeLayout(FGsImmuneStatTypeNameSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FGsCustomizationFactory::MakeInstance<FGsNameSelectorCustomization<FGsImmuneStatTypeNameSelector>>));

}

IMPLEMENT_MODULE(FDataTableConverterEditor, DataTableConverterEditor);