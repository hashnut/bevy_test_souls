#include "SkillNotifyDataFactoryNew.h"

//#include "GameObject/Skill/Data/GsSkillNotifyDataCollision.h"
//#include "GameObject/Skill/Data/GsSkillNotifyDataGroupCollision.h"
//#include "GameObject/Skill/Data/GsSkillNotifyDataRandomCollision.h"
//#include "GameObject/Skill/Data/GsSkillNotifyDataCancel.h"
//#include "GameObject/Skill/Data/GsSkillNotifyDataProjectile.h"
//#include "GameObject/Skill/Data/GsSkillNotifyDataSkillGuide.h"
//
//#include "SkillNotifyData/HitSkillNotifyData.h"
//
//
//USkillNotifyDataFactoryNew::USkillNotifyDataFactoryNew()
//{
//	bCreateNew = true;
//	bEditAfterNew = true;
//	SupportedClass = UGsSkillNotifyDataBase::StaticClass();
//}
//
//UObject* USkillNotifyDataFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
//	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
//{
//	return UFactory::FactoryCreateNew(InClass, InParent, InName, Flags, Context, Warn, CallingContext);
//}
//
//// Collision Data
//USkillNotifyDataCollision::USkillNotifyDataCollision()
//{
//	SupportedClass = UGsSkillNotifyDataCollision::StaticClass();
//}
//
//UObject* USkillNotifyDataCollision::FactoryCreateNew(UClass* InClass, UObject* InParent,
//	FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn,
//	FName CallingContext)
//{
//	UGsSkillNotifyDataCollision* asset = NewObject<UGsSkillNotifyDataCollision>(InParent, InClass, InName, Flags);
//	return asset;
//}
//
//// GroupCollision Data
//USkillNotifyDataGroupCollision::USkillNotifyDataGroupCollision()
//{
//	SupportedClass = UGsSkillNotifyDataGroupCollision::StaticClass();
//}
//
//UObject* USkillNotifyDataGroupCollision::FactoryCreateNew(UClass* InClass, UObject* InParent,
//	FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn,
//	FName CallingContext)
//{
//	UGsSkillNotifyDataGroupCollision* asset = NewObject<UGsSkillNotifyDataGroupCollision>(InParent, InClass, InName, Flags);
//	return asset;
//}
//
//// RandomCollision Data
//USkillNotifyDataRandomCollision::USkillNotifyDataRandomCollision()
//{
//	SupportedClass = UGsSkillNotifyDataRandomCollision::StaticClass();
//}
//
//UObject* USkillNotifyDataRandomCollision::FactoryCreateNew(UClass* InClass, UObject* InParent,
//	FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn,
//	FName CallingContext)
//{
//	UGsSkillNotifyDataRandomCollision* asset = NewObject<UGsSkillNotifyDataRandomCollision>(InParent, InClass, InName, Flags);
//	return asset;
//}
//
//
//// Cancle Data
//USkillNotifyDataCancle::USkillNotifyDataCancle()
//{
//	SupportedClass = UGsSkillNotifyDataCancel::StaticClass();
//}
//
//UObject* USkillNotifyDataCancle::FactoryCreateNew(UClass* InClass, UObject* InParent,
//	FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn,
//	FName CallingContext)
//{
//	UGsSkillNotifyDataCancel* asset = NewObject<UGsSkillNotifyDataCancel>(InParent, InClass, InName, Flags);
//	return asset;
//}
//
//// GuideEffect Data
//USkillNotifyDataGuideEffect::USkillNotifyDataGuideEffect()
//{
//	SupportedClass = UGsSkillNotifyDataSkillGuide::StaticClass();
//}
//
//UObject* USkillNotifyDataGuideEffect::FactoryCreateNew(UClass* InClass, UObject* InParent,
//	FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn,
//	FName CallingContext)
//{
//	UGsSkillNotifyDataSkillGuide* asset = NewObject<UGsSkillNotifyDataSkillGuide>(InParent, InClass, InName, Flags);
//	return asset;
//}
//
//// Projectile Data
//USkillNotifyDataProjectile::USkillNotifyDataProjectile()
//{
//	SupportedClass = UGsSkillNotifyDataProjectile::StaticClass();
//}
//
//UObject* USkillNotifyDataProjectile::FactoryCreateNew(UClass* InClass, UObject* InParent,
//	FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn,
//	FName CallingContext)
//{
//	UGsSkillNotifyDataProjectile* asset = NewObject<UGsSkillNotifyDataProjectile>(InParent, InClass, InName, Flags);
//	return asset;
//}
//
//// Test
//UHitSkillNotifyDataNew::UHitSkillNotifyDataNew()
//{
//	SupportedClass = UHitSkillNotifyData::StaticClass();
//}
//
//UObject* UHitSkillNotifyDataNew::FactoryCreateNew(UClass* InClass, UObject* InParent,
//	FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn,
//	FName CallingContext)
//{
//	UHitSkillNotifyDataNew* asset = NewObject<UHitSkillNotifyDataNew>(InParent, InClass, InName, Flags);
//	return asset;
//}