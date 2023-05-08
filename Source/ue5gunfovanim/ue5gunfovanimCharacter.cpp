// Copyright Epic Games, Inc. All Rights Reserved.

#include "ue5gunfovanimCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ViewModelSkeletalMeshComponent.h"


//////////////////////////////////////////////////////////////////////////
// Aue5gunfovanimCharacter

Aue5gunfovanimCharacter::Aue5gunfovanimCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<UViewModelSkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	Mesh1P->DesiredHorizontalFov = 120.f;

	// Initialize the debug gun mesh
	GunMesh = CreateDefaultSubobject<UViewModelSkeletalMeshComponent>(TEXT("GunMesh1P"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->SetupAttachment(Mesh1P, FName("GripPoint"));
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;
	GunMesh->DesiredHorizontalFov = 120.f;
	// GunMesh->SetRelativeLocation(FVector(20.f, 20.f, -20.f));
	// GunMesh->SetRelativeRotation(FRotator(0.f, -100.f, 0.f));
}

void Aue5gunfovanimCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SetHasRifle(true);

	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, false);
	// GunMesh->AttachToComponent(Mesh1P, Rules, TEXT("GripPoint"));
	// GunMesh->SetupAttachment(FirstPersonCameraComponent);
}

//////////////////////////////////////////////////////////////////////////// Input

void Aue5gunfovanimCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Aue5gunfovanimCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Aue5gunfovanimCharacter::Look);
	}
}


void Aue5gunfovanimCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void Aue5gunfovanimCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void Aue5gunfovanimCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool Aue5gunfovanimCharacter::GetHasRifle()
{
	return bHasRifle;
}
