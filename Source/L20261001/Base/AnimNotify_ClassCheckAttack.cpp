// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ClassCheckAttack.h"
#include "../Base/BasePlayer.h"

void UAnimNotify_ClassCheckAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);


	ABasePlayer* Player = Cast<ABasePlayer>(MeshComp->GetOwner());
	if (Player)
	{
		Player->CheckCombo();
	}

}
