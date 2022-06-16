// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGAnimInstance.h"

URPGAnimInstance::URPGAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}
