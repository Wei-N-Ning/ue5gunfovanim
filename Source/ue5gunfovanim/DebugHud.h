// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Containers/Map.h"
#include "DebugHud.generated.h"

USTRUCT()
struct FDebugMessage
{
	GENERATED_BODY()

	FText Message;
	FColor Color;
	float TextSize;
};

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

	void AddDebugMessage(uint64 Id, const FString& Message, const FColor& Color, float TextSize);

private:
	UPROPERTY()
	UFont* DebugFont;

	UPROPERTY()
	TArray<uint64> Ids;

	UPROPERTY()
	TMap<uint64, FDebugMessage> DebugMessageById;

	bool bDisplayDebugMessages;
};
