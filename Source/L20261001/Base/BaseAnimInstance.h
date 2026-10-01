// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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
	bool bIsFalling = false;
};
