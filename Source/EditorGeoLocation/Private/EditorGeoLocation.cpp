// Made by ArmlessMaster

#include "EditorGeoLocation.h"
#include "EditorGeoLocationStyle.h"
#include "EditorGeoLocationCommands.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName EditorGeoLocationTabName("EditorGeoLocation");

#define LOCTEXT_NAMESPACE "FEditorGeoLocationModule"

void FEditorGeoLocationModule::StartupModule()
{
	FEditorGeoLocationStyle::Initialize();
	FEditorGeoLocationStyle::ReloadTextures();

	FEditorGeoLocationCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEditorGeoLocationCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FEditorGeoLocationModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEditorGeoLocationModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EditorGeoLocationTabName, FOnSpawnTab::CreateRaw(this, &FEditorGeoLocationModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEditorGeoLocationTabTitle", "EditorGeoLocation"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FEditorGeoLocationModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEditorGeoLocationStyle::Shutdown();

	FEditorGeoLocationCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EditorGeoLocationTabName);
}

TSharedRef<SDockTab> FEditorGeoLocationModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
	if (!World)
	{
		return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(STextBlock).Text(FText::FromString("World not found"))
		];
	}

	FString WidgetPath = TEXT("/EditorGeoLocation/EUW_GeoLocation.EUW_GeoLocation");
	UEditorUtilityWidgetBlueprint* WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, *WidgetPath);

	if (!WidgetBP)
	{
		return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(STextBlock).Text(FText::FromString("Widget not found: " + WidgetPath))
		];
	}

	UClass* WidgetClass = WidgetBP->GeneratedClass;
	if (!WidgetClass || !WidgetClass->IsChildOf(UUserWidget::StaticClass()))
	{
		return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(STextBlock).Text(FText::FromString("Invalid widget class"))
		];
	}

	UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(World, WidgetClass);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			WidgetInstance->TakeWidget()
		];
}

void FEditorGeoLocationModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EditorGeoLocationTabName);
}

void FEditorGeoLocationModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEditorGeoLocationCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEditorGeoLocationCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorGeoLocationModule, EditorGeoLocation)