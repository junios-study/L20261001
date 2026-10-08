// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ClassCheckAttack.generated.h"

/**
 *
 */
UCLASS()
class L20261001_API UAnimNotify_ClassCheckAttack : public UAnimNotify
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackDamage = 10.f;

	FString GetNotifyName_Implementaion() const
	{
		return TEXT("ClassCheckAttack");
	}

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
