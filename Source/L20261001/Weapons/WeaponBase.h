// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UAnimMontage;

UCLASS()
class L20261001_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	void Fire();

	void Reload();

	UPROPERTY(EditAnywhere, Category = "Animations")
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, Category = "Data")
	uint32 MaxBullet = 15;

	UPROPERTY(EditAnywhere, Category = "Data")
	uint32 CurrentBullet = 15;

	UPROPERTY(EditAnywhere, Category = "Data")
	FName WeaponType;
};
