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

	UPROPERTY(EditAnywhere, Category=Mesh)
	float DesiredHorizontalFov;

	UPROPERTY(EditAnywhere, Category=Mesh)
	bool EnableCustomRenderMatrix = true;

	virtual FMatrix GetRenderMatrix() const override;
};
