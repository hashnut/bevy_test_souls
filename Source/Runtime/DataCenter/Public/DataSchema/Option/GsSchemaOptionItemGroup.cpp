/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionItemGroup.h"
FGsSchemaOptionItemGroup::FGsSchemaOptionItemGroup()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaOptionItemGroup)
	
	optionItemType = EGsOptionItem::AutoTuning;
	isToggleOn = false;
	textOn = FTextStringHelper::CreateFromBuffer(
TEXT("NSLOCTEXT(\"[84078E96408DC1D2E29F55888425EED9]\", \"C180FB6E4C3A0FA426FDA88468289264\", \"\ubaa8\ub450 On\")")	);
	textOff = FTextStringHelper::CreateFromBuffer(
TEXT("NSLOCTEXT(\"[84078E96408DC1D2E29F55888425EED9]\", \"BD88F495416EDB9127EF0B8D7691AB31\", \"\ubaa8\ub450 OFF\")")	);
}
