// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "../Weapons/WeaponBase.h"

#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class L20261001_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float GroundSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Direction = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float AimPitch = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float AimYaw = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	uint8 bIsFalling : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	uint8 bIsCrouched : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	float TargetLeanAngle;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	float CurrentLeanAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	uint64 bIsBigHeadMode : 1 = false;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	float BigHeadScale = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	float CurrentBigHeadScale = 1.0f;

	UFUNCTION()
	void AnimNotify_CPPAttackCheck(UAnimNotify* Notify);



};
