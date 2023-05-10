// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Containers/Map.h"
#include "DebugHud.generated.h"

struct FDebugMessage
{
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

	TMap<uint64, TArray<FDebugMessage>> DebugMessagesById;

	bool bDisplayDebugMessages;
};
