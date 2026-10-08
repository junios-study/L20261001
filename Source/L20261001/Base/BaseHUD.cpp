// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Engine/Canvas.h"
#include "BasePlayer.h"
#include "../Weapons/WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABaseHUD::DrawHUD()
{
	int32 CenterX = Canvas->SizeX / 2;
	int32 CenterY = Canvas->SizeY / 2;
	int32 Unit = Canvas->SizeX / 100;
	int32 CrosshairSize = Unit * 2;
	int32 ShotOffset = Unit * 3;

	ABasePlayer* Player = Cast<ABasePlayer>(GetOwningPawn());
	if (Player && Player->CurrentWeapon && Player->CurrentWeapon->WeaponType != EWeaponType::Unarmed)
	{
		float MaxSpeed = Player->JogSpeed;
		float CurrentSpeed = Player->GetCharacterMovement()->Velocity.Size2D();

		ShotOffset = (float)ShotOffset * (CurrentSpeed / MaxSpeed);

		DrawLine(CenterX - CrosshairSize - ShotOffset, CenterY, CenterX - ShotOffset, CenterY, FLinearColor::Red, 1.0f);

		DrawLine(CenterX + ShotOffset, CenterY, CenterX + CrosshairSize + ShotOffset, CenterY, FLinearColor::Red, 1.0f);


		DrawLine(CenterX, CenterY - CrosshairSize - ShotOffset, CenterX, CenterY - ShotOffset, FLinearColor::Red, 1.0f);

		DrawLine(CenterX, CenterY + ShotOffset, CenterX, CenterY + ShotOffset + CrosshairSize, FLinearColor::Red, 1.0f);
	}
}

