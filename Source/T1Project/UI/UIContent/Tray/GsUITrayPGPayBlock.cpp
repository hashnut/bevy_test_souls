#include "GsUITrayPGPayBlock.h"

#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

const TCHAR* UGsUITrayPGPayBlock::ASSET_NAME = TEXT("TrayPGPayBlockUI");

void UGsUITrayPGPayBlock::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddUniqueDynamic(this, &UGsUITrayPGPayBlock::OnClickOk);
}

void UGsUITrayPGPayBlock::OnClickOk()
{
	FGsBMShopManager* BMShopManager = GBMShop();
	if (nullptr == BMShopManager || !BMShopManager->OnClickUserPgComplete(true))
	{
		GSLOG(Error, TEXT("nullptr == BMShopManager || !BMShopManager->OnClickUserPgComplete()"));
		return;
	}

	Close();
}
