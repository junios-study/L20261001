// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasicPCM.generated.h"

/**
 * 
 */
UCLASS()
class L20261001_API ABasicPCM : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	virtual void UpdateCamera(float DeltaTime) override;

	
};
