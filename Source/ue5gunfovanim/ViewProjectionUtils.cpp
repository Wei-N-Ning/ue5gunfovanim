#include "ViewProjectionUtils.h"

ViewProjectionUtils::FMatrices ViewProjectionUtils::GetMatrices(const UWorld& World)
{
	FMatrices Matrices;

	// Source: https://answers.unrealengine.com/questions/25526/custom-mesh-fov.html
	const APlayerController* PlayerController = World.GetFirstPlayerController();
	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);

	// Get View Origin (the "eye" of the player at the current frame)
	FVector ViewOrigin;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(/*out*/ ViewOrigin, /*out*/ ViewRotation);

	// The reason for using an inverse rotation matrix to calculate the ViewRotationMatrix is
	// due to how the View matrix is constructed. The View matrix is responsible for
	// transforming world coordinates to camera/view space. Essentially, it represents the
	// inverse transformation of the camera in the world. Therefore, to create the View matrix,
	// we need the inverse of the camera's rotation and translation.
	FMatrix ViewRotationMatrix = FInverseRotationMatrix{ViewRotation} * FMatrix(
		FPlane(0, 0, 1, 0),
		FPlane(1, 0, 0, 0),
		FPlane(0, 1, 0, 0),
		FPlane(0, 0, 0, 1));

	// it is worth noting that, in most cases, the ViewRotationMatrix should not have any translation component,
	// and this line of code would have no effect on the ViewOrigin.
	if (!ViewRotationMatrix.GetOrigin().IsNearlyZero(0.0f))
	{
		// The InverseTransformPosition function essentially applies the inverse of the ViewRotationMatrix
		// to the input vector (in this case, FVector::ZeroVector, which is a vector of zeros). Since the
		// input vector is all zeros, the result of this operation is simply the translation component of
		// the ViewRotationMatrix.
		// Then, the translation component of the ViewRotationMatrix is added to the ViewOrigin. This step
		// effectively compensates for any translation present in the ViewRotationMatrix, ensuring that the
		// final ViewOrigin represents the correct position of the camera in the world.
		ViewOrigin += ViewRotationMatrix.InverseTransformPosition(FVector::ZeroVector);
		ViewRotationMatrix = ViewRotationMatrix.RemoveTranslation();
	}

	// Calculate view matrix T . R
	const FMatrix ViewMatrix = FTranslationMatrix(-ViewOrigin) * ViewRotationMatrix;

	// Calculate project matrix

	// Calculate the pixel coordinates of the viewport rectangle that will be used for rendering,
	const int32 X = FMath::TruncToInt(LocalPlayer->Origin.X * LocalPlayer->ViewportClient->Viewport->GetSizeXY().X);
	const int32 Y = FMath::TruncToInt(LocalPlayer->Origin.Y * LocalPlayer->ViewportClient->Viewport->GetSizeXY().Y);
	const uint32 SizeX = FMath::TruncToInt(LocalPlayer->Size.X * LocalPlayer->ViewportClient->Viewport->GetSizeXY().X);
	const uint32 SizeY = FMath::TruncToInt(LocalPlayer->Size.Y * LocalPlayer->ViewportClient->Viewport->GetSizeXY().Y);
	const FIntRect UnconstrainedRectangle = FIntRect(X, Y, X + SizeX, Y + SizeY);

	FSceneViewProjectionData ProjectionData;
	ProjectionData.SetViewRectangle(UnconstrainedRectangle);

	// a lightweight data structure in Unreal Engine that contains essential information about a camera
	// view. It is used to simplify the process of working with camera views by providing a minimal set
	// of properties that describe the camera's position, orientation, field of view (FOV), and other
	// relevant parameters.
	// Some of the properties included in FMinimalViewInfo are:
	// Location: The position of the camera in world space.
	// Rotation: The orientation of the camera, represented as a FRotator.
	// FOV: The field of view angle, in degrees.
	// AspectRatio: The aspect ratio of the camera view (width/height).
	// OrthoWidth: The width of the camera view when using an orthographic projection.
	// OrthoNearClipPlane: The near clipping plane distance for an orthographic projection.
	// OrthoFarClipPlane: The far clipping plane distance for an orthographic projection.
	// bConstrainAspectRatio: A flag indicating whether the aspect ratio should be constrained.
	// bUseFieldOfViewForLOD: A flag indicating whether the field of view should be used for level-of-detail
	// (LOD) calculations.
	FMinimalViewInfo OutViewInfo;
	if (PlayerController->PlayerCameraManager != nullptr)
	{
		OutViewInfo = PlayerController->PlayerCameraManager->GetCameraCacheView();
		OutViewInfo.FOV = PlayerController->PlayerCameraManager->GetFOVAngle();
	}

	PlayerController->GetPlayerViewPoint(/*out*/ OutViewInfo.Location, /*out*/ OutViewInfo.Rotation);

	FMinimalViewInfo::CalculateProjectionMatrixGivenView(
		OutViewInfo,
		LocalPlayer->AspectRatioAxisConstraint, // taking aspect ratio into account
		LocalPlayer->ViewportClient->Viewport,
		ProjectionData /*inout*/
	);

	Matrices.ViewMatrix = ViewMatrix;
	Matrices.InverseViewMatrix = ViewMatrix.Inverse();
	Matrices.ProjectionMatrix = ProjectionData.ProjectionMatrix;
	Matrices.InverseProjectionMatrix = ProjectionData.ProjectionMatrix.Inverse();
	Matrices.InverseViewProjectionMatrix = Matrices.InverseProjectionMatrix * Matrices.InverseViewMatrix;
	Matrices.NearClippingPlaneDistance = Matrices.ProjectionMatrix.M[3][2];

	return Matrices;
}
