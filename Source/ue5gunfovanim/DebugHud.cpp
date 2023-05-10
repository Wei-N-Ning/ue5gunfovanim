// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugHud.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"

ADebugHud::ADebugHud()
{
	PrimaryActorTick.bCanEverTick = true;
	bDisplayDebugMessages = true;
	DebugFont = nullptr;
}

void ADebugHud::BeginPlay()
{
	Super::BeginPlay();

	// Load the font to use for rendering debug text
	DebugFont = GEngine->GetMediumFont();
}

void ADebugHud::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Toggle the display of debug messages with LeftALT-F3
	if (PlayerOwner &&
		PlayerOwner->IsInputKeyDown(EKeys::LeftAlt) &&
		PlayerOwner->WasInputKeyJustPressed(EKeys::F3))
	{
		bDisplayDebugMessages = !bDisplayDebugMessages;
	}
}

void ADebugHud::DrawHUD()
{
	Super::DrawHUD();
	if (bDisplayDebugMessages)
	{
		float Y = 5.0f;
		Ids.Sort();
		for (const auto Id : Ids)
		{
			const auto [Message, Color, TextSize] = DebugMessageById[Id];
			constexpr float LineHeight = 17.0f;
			constexpr float LineHorizontalMargin = 5.0f;
			const FVector2D Position(LineHorizontalMargin, Y);
			Y += LineHeight * TextSize;
			FCanvasTextItem TextItem(Position, Message, DebugFont, Color);
			TextItem.Scale = FVector2D(TextSize, TextSize);
			Canvas->DrawItem(TextItem);
		}
		Ids.Reset();
		DebugMessageById.Reset();
	}
}

// font size consideration:
// 1.0 is 17 pixels on screen
// 1.0: a bit too small but readable; probably ok for 1k full screen
// 1.5: probably better for small screen
// 2.0: a bit too large, can use as title; don't overuse
// 
// color consideration:
// green: bright and easy to see
// blue: bad, avoid!
// red: a bit hard to read
// yellow: ok
// cyan: ok
// orange: bad
// 
// example:
// GetWorld()->GetFirstPlayerController()->GetHUD()->AddDebugMessage("Hello World!", FColor::Red, 1.0f);
// AddDebugMessage("there is a cow", FColor::Red, 1.25f);
// AddDebugMessage("there is a cow", FColor::Green, 1.25f);
// AddDebugMessage("there is a cow", FColor::Yellow, 1.25f);
// AddDebugMessage("there is a cow", FColor::Cyan, 1.25f);
// AddDebugMessage("there is a cow", FColor::Orange, 1.25f);
void ADebugHud::AddDebugMessage(uint64 Id, const FString& Message, const FColor& Color, const float TextSize)
{
	Ids.Add(Id);
	DebugMessageById.Add(TTuple<int64, FDebugMessage>{
		Id, FDebugMessage{FText::FromString(Message), Color, TextSize}
	});
}
