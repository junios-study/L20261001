// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "BasePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//매 프레임 폰의 정보를 가져와서 업데이트 한다.
	ABasePlayer* Player = Cast<ABasePlayer>(TryGetPawnOwner());
	if (Player)
	{
		Direction = UKismetAnimationLibrary::CalculateDirection(Player->GetCharacterMovement()->Velocity,
			Player->GetActorRotation());
		GroundSpeed = Player->GetCharacterMovement()->Velocity.Size2D();
		bIsFalling = Player->GetCharacterMovement()->IsFalling(); 
		bIsCrouched = Player->bIsCrouched;
		AimPitch = Player->GetAimRotation().Pitch;
		AimYaw = Player->GetAimRotation().Yaw;
		TargetLeanAngle = Player->TargetLeanAngle;

		bIsBigHeadMode = Player->bIsBigHeadMode;
		if (bIsBigHeadMode)
		{
			CurrentBigHeadScale = FMath::FInterpTo(CurrentBigHeadScale, BigHeadScale, DeltaSeconds, 4.0f);
		}
		else
		{
			CurrentBigHeadScale = FMath::FInterpTo(CurrentBigHeadScale, 1.0f, DeltaSeconds, 4.0f);
		}


		CurrentLeanAngle = FMath::FInterpTo(CurrentLeanAngle, TargetLeanAngle, DeltaSeconds, 5.0f);

		if (Player->CurrentWeapon)
		{
			WeaponType = Player->CurrentWeapon->WeaponType;
		}
		else
		{
			WeaponType = EWeaponType::Unarmed;
		}
	}
}
