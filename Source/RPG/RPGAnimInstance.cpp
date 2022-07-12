// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGAnimInstance.h"
#include "DrawDebugHelpers.h"

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
		// Jump

		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}

		// Tilt

		auto ActorRotation = Pawn->GetActorRotation();
		auto delta = RotatorLastTick - ActorRotation;
		delta.Normalize();

		delta.Yaw = delta.Yaw / DeltaSeconds;
		delta.Yaw /= 12.0f;

		YawDelta = FMath::FInterpTo(YawDelta, delta.Yaw, DeltaSeconds, 6.0f);

		RotatorLastTick = Pawn->GetActorRotation();


		



		
	}

	

}

void URPGAnimInstance::NativeBeginPlay()
{
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		RotatorLastTick = Pawn->GetActorRotation();
		YawDelta = 0;
	}
}

