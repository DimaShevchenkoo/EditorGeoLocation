// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "GeoLocationWidget.generated.h"

class UEditorUtilityButton;
class UEditorUtilityEditableTextBox;
class UButton;
class UTextBlock;

UCLASS()
class EDITORGEOLOCATION_API UGeoLocationWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	// --- Origin fields ---
	UPROPERTY(meta=(BindWidget))
	UEditorUtilityEditableTextBox* OriginLatBox;

	UPROPERTY(meta=(BindWidget))
	UEditorUtilityEditableTextBox* OriginLonBox;

	UPROPERTY(meta=(BindWidget))
	UEditorUtilityEditableTextBox* OriginAltBox;

	// --- Target fields ---
	UPROPERTY(meta=(BindWidget))
	UEditorUtilityEditableTextBox* TargetLatBox;

	UPROPERTY(meta=(BindWidget))
	UEditorUtilityEditableTextBox* TargetLonBox;

	UPROPERTY(meta=(BindWidget))
	UEditorUtilityEditableTextBox* TargetAltBox;

	// --- Unreal coordinate inputs (for reverse conversion) ---
	UPROPERTY(meta = (BindWidget))
	UEditorUtilityEditableTextBox* XBox;

	UPROPERTY(meta = (BindWidget))
	UEditorUtilityEditableTextBox* YBox;

	UPROPERTY(meta = (BindWidget))
	UEditorUtilityEditableTextBox* ZBox;

	// --- Buttons ---
	UPROPERTY(meta=(BindWidget))
	UEditorUtilityButton* ConvertToUnrealButton;

	UPROPERTY(meta=(BindWidget))
	UEditorUtilityButton* ConvertToGeoButton;

	UPROPERTY(meta=(BindWidget))
	UEditorUtilityButton* MoveCameraButton;

	// --- Output ---
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ResultText;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	FVector ConvertedCoords = FVector::ZeroVector;

	// Handlers
	UFUNCTION()
	void OnConvertToUnrealClicked();

	UFUNCTION()
	void OnConvertToGeoClicked();

	UFUNCTION()
	void OnMoveCameraClicked();

	// Helpers
	FVector LatLonToUnreal(double Lat, double Lon, double Alt, double OriginLat, double OriginLon, double OriginAlt);
	void UnrealToLatLon(const FVector& Unreal, double OriginLat, double OriginLon, double OriginAlt,
						double& OutLat, double& OutLon, double& OutAlt);

protected:
	void LoadLastValues() const;

	UFUNCTION(BlueprintCallable)
	void SaveCurrentValues();

private:
	FLinearColor DefaultResultColor = FLinearColor::White;
	FLinearColor SuccessColor = FLinearColor(0.2f, 1.0f, 0.2f, 1.0f);

	FTimerHandle ResultColorTimer;
	void FlashResultColor();
	void ResetResultColor();
};
