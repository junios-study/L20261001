// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPCM.h"
#include "BasePlayer.h"

void ABasicPCM::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	ABasePlayer* Player = Cast<ABasePlayer>(GetOwningPlayerController()->GetPawn());

	if (Player)
	{
		float TargetFOV = Player->bIsZoom ? 60.0f : 90.0f;
		float ResultFOV = FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, 15.f);

		SetFOV(ResultFOV);
	}
}
