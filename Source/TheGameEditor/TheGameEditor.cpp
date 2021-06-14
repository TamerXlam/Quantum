// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TheGameEditor.h"
#include "PropertyEditorModule.h"
#include "AssetToolsModule.h"
#include "UnrealEd.h"

class FTheGameEditorModuleImpl : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{

		UE_LOG(LogTemp, Display, TEXT(" TheGameEditorModule::StartupModule "));

		{
			FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

			//PropertyModule.RegisterCustomClassLayout		("TLSDamageZonesComponent", FOnGetDetailCustomizationInstance::CreateStatic(&FTLSDamageZonesDetails::MakeInstance));
			//PropertyModule.RegisterCustomPropertyTypeLayout	("TLSWeaponSpreadDataArray",FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTLSWeaponImportantDataDetails::MakeInstance));

			PropertyModule.NotifyCustomizationModuleChanged();
		}

		// register asset actions
		{
			auto& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
			//AssetTools.RegisterAssetTypeActions(MakeShareable(new TLSAssetTypeActions_GameplayZoneTable));
		}
	}


	virtual void ShutdownModule() override
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		//PropertyModule.UnregisterCustomClassLayout("TLSDamageZonesComponent");
		//PropertyModule.UnregisterCustomPropertyTypeLayout(TEXT("TLSWeaponSpreadDataArray"));

		PropertyModule.NotifyCustomizationModuleChanged();                                                                                                           
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FTheGameEditorModuleImpl, TheGameEditor, "TheGameEditor" );
 