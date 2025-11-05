// Made by ArmlessMaster

#include "GeoLocationWidget.h"
#include "EditorUtilityWidgetComponents.h"
#include "Components/TextBlock.h"

#define GEO_CONFIG_SECTION TEXT("/Script/EditorGeoLocation.GeoLocationWidget")

void UGeoLocationWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ConvertToUnrealButton)
        ConvertToUnrealButton->OnClicked.AddDynamic(this, &UGeoLocationWidget::OnConvertToUnrealClicked);

    if (ConvertToGeoButton)
        ConvertToGeoButton->OnClicked.AddDynamic(this, &UGeoLocationWidget::OnConvertToGeoClicked);

    if (MoveCameraButton)
        MoveCameraButton->OnClicked.AddDynamic(this, &UGeoLocationWidget::OnMoveCameraClicked);

    LoadLastValues();
}

void UGeoLocationWidget::NativeDestruct()
{
    SaveCurrentValues();
    
    Super::NativeDestruct();
}

FVector UGeoLocationWidget::LatLonToUnreal(const double Lat, const double Lon, const double Alt, const double OriginLat, const double OriginLon, const double OriginAlt)
{
    constexpr double OneDegreeLat = 111320.0;
    const double CosLat = FMath::Cos(FMath::DegreesToRadians(OriginLat));
    const double OneDegreeLon = OneDegreeLat * CosLat;

    const double DeltaLat = Lat - OriginLat;
    const double DeltaLon = Lon - OriginLon;
    const double DeltaAlt = Alt;

    return FVector(DeltaLon * OneDegreeLon, DeltaLat * OneDegreeLat, DeltaAlt * 100);
}

void UGeoLocationWidget::UnrealToLatLon(const FVector& Unreal, const double OriginLat, const double OriginLon, const double OriginAlt,
                                        double& OutLat, double& OutLon, double& OutAlt)
{
    constexpr double OneDegreeLat = 111320.0;
    const double CosLat = FMath::Cos(FMath::DegreesToRadians(OriginLat));
    const double OneDegreeLon = OneDegreeLat * CosLat;

    OutLat = OriginLat + Unreal.Y / OneDegreeLat;
    OutLon = OriginLon + Unreal.X / OneDegreeLon;
    OutAlt = Unreal.Z / 100.f;
}

void UGeoLocationWidget::OnConvertToUnrealClicked()
{
    FlashResultColor();
    
    const double OriginLat = FCString::Atod(*OriginLatBox->GetText().ToString());
    const double OriginLon = FCString::Atod(*OriginLonBox->GetText().ToString());
    const double OriginAlt = FCString::Atod(*OriginAltBox->GetText().ToString());

    const double TargetLat = FCString::Atod(*TargetLatBox->GetText().ToString());
    const double TargetLon = FCString::Atod(*TargetLonBox->GetText().ToString());
    const double TargetAlt = FCString::Atod(*TargetAltBox->GetText().ToString());

    ConvertedCoords = LatLonToUnreal(TargetLat, TargetLon, TargetAlt, OriginLat, OriginLon, OriginAlt);

    ResultText->SetText(FText::FromString(FString::Printf(TEXT("X=%.2f Y=%.2f Z=%.2f"), ConvertedCoords.X, ConvertedCoords.Y, ConvertedCoords.Z)));

    SaveCurrentValues();
}

void UGeoLocationWidget::OnConvertToGeoClicked()
{
    FlashResultColor();
    
    const double OriginLat = FCString::Atod(*OriginLatBox->GetText().ToString());
    const double OriginLon = FCString::Atod(*OriginLonBox->GetText().ToString());
    const double OriginAlt = FCString::Atod(*OriginAltBox->GetText().ToString());

    const double X = FCString::Atod(*XBox->GetText().ToString());
    const double Y = FCString::Atod(*YBox->GetText().ToString());
    const double Z = FCString::Atod(*ZBox->GetText().ToString());

    FVector UnrealCoords(X, Y, Z);

    double Lat, Lon, Alt;
    UnrealToLatLon(UnrealCoords, OriginLat, OriginLon, OriginAlt, Lat, Lon, Alt);

    TargetLatBox->SetText(FText::AsNumber(Lat));
    TargetLonBox->SetText(FText::AsNumber(Lon));
    TargetAltBox->SetText(FText::AsNumber(Alt));

    ResultText->SetText(FText::FromString(FString::Printf(TEXT("Lat=%.6f Lon=%.6f Alt=%.2f"), Lat, Lon, Alt)));

    SaveCurrentValues();
}

void UGeoLocationWidget::OnMoveCameraClicked()
{
    SaveCurrentValues();
    
#if WITH_EDITOR
    if (GEditor)
    {
        for (FEditorViewportClient* Client : GEditor->GetAllViewportClients())
        {
            if (Client && Client->IsPerspective())
            {
                Client->SetViewLocation(ConvertedCoords);
                Client->Invalidate();
            }
        }
    }
#endif
}

void UGeoLocationWidget::LoadLastValues() const
{
    FString Str;

    auto LoadField = [&](const FString& Key, UEditableTextBox* Box)
    {
        if (!Box) return;
        if (GConfig->GetString(GEO_CONFIG_SECTION, *Key, Str, GEditorPerProjectIni))
        {
            Box->SetText(FText::FromString(Str));
        }
    };

    LoadField(TEXT("OriginLat"), OriginLatBox);
    LoadField(TEXT("OriginLon"), OriginLonBox);
    LoadField(TEXT("OriginAlt"), OriginAltBox);
    LoadField(TEXT("TargetLat"), TargetLatBox);
    LoadField(TEXT("TargetLon"), TargetLonBox);
    LoadField(TEXT("TargetAlt"), TargetAltBox);
    LoadField(TEXT("TargetX"), XBox);
    LoadField(TEXT("TargetY"), YBox);
    LoadField(TEXT("TargetZ"), ZBox);
}

void UGeoLocationWidget::SaveCurrentValues()
{
    auto SaveField = [&](const FString& Key, const UEditableTextBox* Box)
    {
        if (!Box) return;
        GConfig->SetString(GEO_CONFIG_SECTION, *Key, *Box->GetText().ToString(), GEditorPerProjectIni);
    };

    SaveField(TEXT("OriginLat"), OriginLatBox);
    SaveField(TEXT("OriginLon"), OriginLonBox);
    SaveField(TEXT("OriginAlt"), OriginAltBox);
    SaveField(TEXT("TargetLat"), TargetLatBox);
    SaveField(TEXT("TargetLon"), TargetLonBox);
    SaveField(TEXT("TargetAlt"), TargetAltBox);
    SaveField(TEXT("TargetX"), XBox);
    SaveField(TEXT("TargetY"), YBox);
    SaveField(TEXT("TargetZ"), ZBox);

    GConfig->Flush(false, GEditorPerProjectIni);
}

void UGeoLocationWidget::FlashResultColor()
{
    if (!ResultText) return;

    DefaultResultColor = ResultText->GetColorAndOpacity().GetSpecifiedColor();

    ResultText->SetColorAndOpacity(FSlateColor(SuccessColor));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(ResultColorTimer, this,
            &UGeoLocationWidget::ResetResultColor, 1.2f, false);
    }
}

void UGeoLocationWidget::ResetResultColor()
{
    if (ResultText)
    {
        ResultText->SetColorAndOpacity(FSlateColor(DefaultResultColor));
    }
}
