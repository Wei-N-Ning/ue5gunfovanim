#pragma once

namespace ViewProjectionUtils
{
	struct FMatrices
	{
		FMatrix ViewMatrix;
		FMatrix InverseViewMatrix;
		FMatrix ProjectionMatrix;
		FMatrix InverseProjectionMatrix;
		FMatrix InverseViewProjectionMatrix;
		float NearClippingPlaneDistance;
	};

	// GetMatrices must only be called when the PlayerController is valid and there is a LocalPlayer
	FMatrices GetMatrices(const UWorld& World);
}
