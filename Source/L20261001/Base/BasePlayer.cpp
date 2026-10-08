// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //Important, 추천 분해도 해보셈.
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Weapons/WeaponBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABasePlayer::ABasePlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0, -90.f, 0)
	);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//animation Blueprint
	//static bool bIdle = false;
	//static bool bJog = false;

	//if (GetCharacterMovement()->Velocity.SizeSquared() == 0)
	//{
	//	if (!bIdle)
	//	{
	//		GetMesh()->PlayAnimation(IdleAnimation, true);
	//		bIdle = true;
	//		bJog = false;
	//	}
	//}
	//else
	//{
	//	if (!bJog)
	//	{
	//		GetMesh()->PlayAnimation(JogAnimation, true);
	//		bJog = true;
	//		bIdle = false;
	//	}
	//}
}

// Called to bind functionality to input
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		UIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ACharacter::StopJumping);

		UIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABasePlayer::Move);

		UIC->BindAction(IA_MouseLook, ETriggerEvent::Triggered, this, &ABasePlayer::Look);

		UIC->BindAction(IA_Jog, ETriggerEvent::Triggered, this, &ABasePlayer::Jog);
		UIC->BindAction(IA_Jog, ETriggerEvent::Completed, this, &ABasePlayer::Jog);
	}

}

void ABasePlayer::Jog(const FInputActionValue& Value)
{
	bool IsJog = Value.Get<bool>();

	if (IsJog)
	{
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	
}

void ABasePlayer::Move(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();

	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)) * Direction.Y);
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)) * Direction.X);

}

void ABasePlayer::Look(const FInputActionValue& Value)
{
	FVector2D Rotation = Value.Get<FVector2D>();





	AddControllerYawInput(Rotation.X);
	AddControllerPitchInput(Rotation.Y);
	//AddControllerRollInput();

}

FRotator ABasePlayer::GetAimRotation() const
{
	const FVector AimWS = GetBaseAimRotation().Vector();
	const FVector AimLS = ActorToWorld().InverseTransformVectorNoScale(AimWS);
	const FRotator AimRotationLS = AimLS.Rotation();

	return AimRotationLS;
}

void ABasePlayer::AttachWeapon(TSubclassOf<class AWeaponBase> WeaponTemplate)
{
	AWeaponBase* SpawnWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponTemplate, FTransform());
	HaveWeapons.Add(SpawnWeapon);

	SpawnWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HandGrip_R"));

	SpawnWeapon->SetOwner(this);
}


void ABasePlayer::Fire()
{
	if (CurrentWeapon && CurrentWeapon->WeaponType != EWeaponType::Unarmed)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			int32 SizeX = 0;
			int32 SizeY = 0;
			PC->GetViewportSize(SizeX, SizeY);
			int32 CenterX = SizeX / 2;
			int32 CenterY = SizeY / 2;

			FVector WorldPosition;
			FVector WorldDirection;

			//2D -> 3D(World) Deprojection , 3D(World) -> 2D Projection
			PC->DeprojectScreenPositionToWorld(CenterX, CenterY,
				WorldPosition, WorldDirection);

			FVector CameraLocation;
			FRotator CameraRotation;
			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector Start = CameraLocation;
			FVector RandomVector = WorldDirection + (UKismetMathLibrary::RandomUnitVector() * 0.008f);
			FVector End = CameraLocation + (RandomVector * 99999.0f);

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			TArray<AActor*> IgnoreActors;
			FHitResult OutHit;

			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

			if (UKismetSystemLibrary::LineTraceSingleForObjects(
				GetWorld(),
				Start,
				End,
				ObjectTypes,
				true,
				IgnoreActors,
				EDrawDebugTrace::ForDuration,
				OutHit,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				3.0f
			))
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *OutHit.GetActor()->GetName());

				UE_LOG(LogTemp, Warning, TEXT("Hit BoneName : %s"), *OutHit.BoneName.ToString());
			
			}

			FRotator CurrentRotator = GetControlRotation();
			float RandomAddPitch = FMath::RandRange(0.5f, 1.5f);
			CurrentRotator.Pitch += RandomAddPitch;
			GetController()->SetControlRotation(CurrentRotator);

			const UEnum* EnumPtr = FindObject<UEnum>(GetOuter(), TEXT("EWeaponType"), EFindObjectFlags::ExactClass);

			if (EnumPtr)
			{
				FName SectionName = EnumPtr->GetNameByIndex((int32)CurrentWeapon->WeaponType);
				PlayAnimMontage(CurrentWeapon->FireMontage, 1.0f, SectionName);
			}

			CurrentWeapon->Fire();
		}
	}
	else
	{
		//call C++, Execute BP
		PlayMeleeAttack();
	}
}