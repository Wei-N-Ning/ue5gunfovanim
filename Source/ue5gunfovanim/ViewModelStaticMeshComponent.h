// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ViewModelStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE5GUNFOVANIM_API UViewModelStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Mesh)
	float DesiredHorizontalFov;

	UPROPERTY(EditAnywhere, Category=Mesh)
	bool EnableCustomRenderMatrix = true;

protected:
	virtual FMatrix GetRenderMatrix() const override;

private:
	void AddDebugMessage(FString&& Message) const;
};
