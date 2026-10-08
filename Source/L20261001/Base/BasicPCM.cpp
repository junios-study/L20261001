// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPCM.h"

void ABasicPCM::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
	//GetFOVAngle();
	SetFOV(60.0f);
}
