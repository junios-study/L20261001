// Fill out your copyright notice in the Description page of Project Settings.


#include "PickerableItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Base/BasePlayer.h"

// Sets default values
APickerableItemBase::APickerableItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(100.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APickerableItemBase::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APickerableItemBase::ProcessActorBeginOverlap);
	
}

// Called every frame
void APickerableItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickerableItemBase::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ABasePlayer* Player = Cast<ABasePlayer>(OtherActor);

	if (Player)
	{
		Player->AttachWeapon(WeaponTemplate);
		Destroy();
	}
}

