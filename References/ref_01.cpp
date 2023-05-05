    APlayerController* playerController = ()->GetFirstPlayerController();
	if (playerController)
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(playerController->Player);

		if (LocalPlayer != NULL && LocalPlayer->ViewportClient != NULL && LocalPlayer->ViewportClient->Viewport != NULL)
		{
			FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
				LocalPlayer->ViewportClient->Viewport,
				()->Scene,
				LocalPlayer->ViewportClient->EngineShowFlags)
				.SetRealtimeUpdate(true));

			FVector ViewLocation;
			FRotator ViewRotation;
			FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);

			float MinZ = SceneView->NearClippingDistance;
			float MaxZ = MinZ;

			// Avoid zero ViewFOV's which cause divide by zero's in projection matrix
			float MatrixFOV = FMath::Max(0.001f, WeaponFOV) * (float)PI / 360.0f;

			FMatrix projMatrix = FReversedZPerspectiveMatrix(
					MatrixFOV,
					MatrixFOV,
					1.0f,
					1920.0f / 1080.0f,
					MinZ,
					MaxZ
					);

			FScaleMatrix ClipSpaceFixScale(FVector(1.0f, 1.0f, 1.0f - 0.0f));
			FTranslationMatrix ClipSpaceFixTranslate(FVector(0.0f, 0.0f, 0.0f));
			projMatrix =  projMatrix * ClipSpaceFixScale * ClipSpaceFixTranslate;

			FMatrix adjustedViewProjectMatrix = SceneView->ViewMatrices.ViewMatrix * projMatrix;
			FMatrix inverseOldViewProjectMatrix = SceneView->InvViewProjectionMatrix;
			FMatrix adjTransform = ComponentToWorld.ToMatrixWithScale() * adjustedViewProjectMatrix * inverseOldViewProjectMatrix;

			return adjTransform;
		}
		else
		{
			return Super::GetRenderMatrix();
		}
	}
	else
	{
		return Super::GetRenderMatrix();
	}