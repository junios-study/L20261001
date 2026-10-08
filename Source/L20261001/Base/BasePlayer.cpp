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

		UIC->BindAction(IA_ChangeWeapon, ETriggerEvent::Triggered, this, &ABasePlayer::ChangeWeapon);

		UIC->BindAction(IA_Lean, ETriggerEvent::Triggered, this, &ABasePlayer::Lean);
		UIC->BindAction(IA_Lean, ETriggerEvent::Completed, this, &ABasePlayer::Lean);


		UIC->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ABasePlayer::Zoom);
		UIC->BindAction(IA_Zoom, ETriggerEvent::Completed, this, &ABasePlayer::Zoom);
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

void ABasePlayer::ChangeWeapon(const FInputActionValue& Value)
{
	float Delta = Value.Get<float>();

	int MaxWeaponCount = HaveWeapons.Num();
	int CurrentUsedWeaponIndex = -1;
	if (MaxWeaponCount <= 1)
	{
		return;
	}

	//현재 사용하는 무기 번호 가져오기
	for (int i = 0; i < MaxWeaponCount; ++i)
	{
		//무기 이름 같냐?
		if (CurrentWeapon.GetClass() == HaveWeapons[i].GetClass())
		{
			CurrentUsedWeaponIndex = i;
		}
	}

	int64 NewUsedWeapon = CurrentUsedWeaponIndex + Delta;

	if (NewUsedWeapon < 0)
	{
		NewUsedWeapon = MaxWeaponCount - 1;
	}
	else if (NewUsedWeapon >= MaxWeaponCount)
	{
		NewUsedWeapon = 0;
	}
	//NewUsedWeapon = FMath::Clamp(NewUsedWeapon, 0, MaxWeaponCount - 1);
	
	CurrentWeapon->SetActorHiddenInGame(true);

	CurrentWeapon = HaveWeapons[NewUsedWeapon];

	CurrentWeapon->SetActorHiddenInGame(false);
}

void ABasePlayer::Zoom(const FInputActionValue& Value)
{
	if (CurrentWeapon && CurrentWeapon->WeaponType != EWeaponType::Unarmed)
	{
		bIsZoom = Value.Get<bool>();
	}
	else
	{
		bIsZoom = false;
	}
}

void ABasePlayer::Lean(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	TargetLeanAngle = 30.0f * Direction;
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
	for (auto Weapon : HaveWeapons)
	{
		//클래스 이름 똑같냐?
		if (Weapon->GetClass() == WeaponTemplate)
		{
			return;
		}
	}

	//같은 총 집으면 교체 안 먹기
	AWeaponBase* SpawnWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponTemplate, FTransform());
	HaveWeapons.Add(SpawnWeapon);

	SpawnWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HandGrip_R"));

	SpawnWeapon->SetOwner(this);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	CurrentWeapon = SpawnWeapon;

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

			//리플렉션 
			//EWeaponType::Unarmed -> "Unarmed"
			//Enum to String(Unarmed, Pistol)
			const UEnum* EnumPtr = StaticEnum<EWeaponType>();
			if (EnumPtr)
			{
				FString SectionName = EnumPtr->GetNameStringByValue((int32)CurrentWeapon->WeaponType);
				PlayAnimMontage(CurrentWeapon->FireMontage, 1.0f, FName(SectionName));
			}

			CurrentWeapon->Fire();
		}
	}
	else
	{
		//call C++, Execute BP
		//PlayMeleeAttack();
		AttackCombo();
	}
}

void ABasePlayer::ChangeBigHeadMode()
{
	bIsBigHeadMode = ~bIsBigHeadMode;
}

void ABasePlayer::PlayMontageMeleeAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		FString SectionName = FString::Printf(TEXT("Attack0%d"), ComboCount);

		float MontageLength = PlayAnimMontage(MeleeAttackMontage, 1.0f, FName(SectionName));
		if (MontageLength > 0)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {
				if (!bInterrupted)
				{
					ComboCount = 0;
					PlayingComboIndex = 0;
					bIsMeleeAttacking = false;
				}
			});

			AnimInstance->Montage_SetEndDelegate(EndDelegate);
		}
	}
}

void ABasePlayer::AttackCombo()
{
	if (!bIsMeleeAttacking) //처음 공격
	{
		ComboCount++;
		PlayMontageMeleeAttack();
		bIsMeleeAttacking = true;
		PlayingComboIndex = ComboCount; // 1 = 1
	}
	else if (bIsMeleeAttacking && PlayingComboIndex == ComboCount)
	{
		ComboCount++;
	}
	
}

void ABasePlayer::CheckCombo()
{
	//anim notify에서 호출
	if (PlayingComboIndex != ComboCount)
	{
		PlayMontageMeleeAttack();
		PlayingComboIndex = ComboCount;
	}
}
