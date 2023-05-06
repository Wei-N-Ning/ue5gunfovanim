// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DebugHud.generated.h"

/**
 * 
 */
UCLASS()
class UE5GUNFOVANIM_API ADebugHud : public AHUD
{
	GENERATED_BODY()

public:
	ADebugHud();

	virtual void DrawHUD() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void AddDebugMessage(const FString& Message, const FColor& Color, float TextSize);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	UFont* DebugFont;

	UPROPERTY(Transient)
	TArray<FText> DebugMessages;

	UPROPERTY(Transient)
	TArray<FColor> DebugMessageColors;

	UPROPERTY(Transient)
	TArray<float> DebugMessageTextSizes;

	bool bDisplayDebugMessages;
};
