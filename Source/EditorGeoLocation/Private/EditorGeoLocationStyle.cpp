// Made by ArmlessMaster

#include "EditorGeoLocationStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FEditorGeoLocationStyle::StyleInstance = nullptr;

void FEditorGeoLocationStyle::Initialize()
{
	if (StyleInstance.IsValid()) return;

	StyleInstance = MakeShareable(new FSlateStyleSet("EditorGeoLocationStyle"));
	FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("EditorGeoLocation"))->GetBaseDir() / TEXT("Resources");
	StyleInstance->SetContentRoot(ContentDir);

	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);

	StyleInstance->Set("EditorGeoLocation.OpenPluginWindow", new FSlateImageBrush(ContentDir / TEXT("Geo.png"), Icon40x40));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance.Get());
}

void FEditorGeoLocationStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance.Get());
		ensure(StyleInstance.IsUnique());
		StyleInstance.Reset();
	}
}

FName FEditorGeoLocationStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("EditorGeoLocationStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FEditorGeoLocationStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("EditorGeoLocationStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("EditorGeoLocation")->GetBaseDir() / TEXT("Resources"));

	Style->Set("EditorGeoLocation.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("Geo"), Icon20x20));

	return Style;
}

void FEditorGeoLocationStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FEditorGeoLocationStyle::Get()
{
	return *StyleInstance;
}
