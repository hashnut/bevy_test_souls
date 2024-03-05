#pragma once

#include "IDataTableConverterEditor.h"

#include "Editor/PropertyEditor/Public/PropertyEditorDelegates.h"

#include "PostExcelImportScripter.h"

class FDataTableConverterEditor : public IDataTableConverterEditor
{

private:
	FPostExcelImportScripter _postImportExeclBinder;

	FDelegateHandle _onHandleSystemErrorDelegate;

	TSet<FName> RegisteredPropertyTypes;

private:
	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;

private:
	void RegisterPropertyTypeCustomizations();

	/**
	 * Registers a custom class
	 *
	 * @param ClassName				The class name to register for property customization
	 * @param DetailLayoutDelegate	The delegate to call to get the custom detail layout instance
	 */
	void RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate);

	/**
	* Registers a custom struct
	*
	* @param StructName				The name of the struct to register for property customization
	* @param StructLayoutDelegate	The delegate to call to get the custom detail layout instance
	*/
	void RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate);

private:
	/**
	 * 로컬 크래시 리포트로 전송
	 * https://historia.co.jp/archives/11157/
	 */
	static void SendLocalCrashReport();

};

struct FGsCustomizationFactory
{
	template<typename TDetailCustomizationType, typename TReturnType>
	static TSharedRef<TReturnType> MakeInstance()
	{
		return MakeShared<TDetailCustomizationType>();
	}
};
