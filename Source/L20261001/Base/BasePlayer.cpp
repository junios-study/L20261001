// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //Important, 추천 분해도 해보셈.
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

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

