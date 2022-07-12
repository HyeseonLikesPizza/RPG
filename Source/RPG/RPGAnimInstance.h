// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPG.h"
#include "Animation/AnimInstance.h"
#include "RPGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	URPGAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;

	void SetIKAlphaLeft(float Alpha)
	{
		IKAlphaLeft = Alpha;
	}
	void SetIKAlphaRight(float Alpha)
	{
		IKAlphaRight = Alpha;
	}
	void SetHipDisplacement(float num)
	{
		HipDisplacement = num;
	}


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess=true))
	float YawDelta;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IKFoot, Meta=(AllowPrivateAccess = true))
	float IKAlphaLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IKFoot, Meta = (AllowPrivateAccess = true))
	float IKAlphaRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = IKFoot, Meta = (AllowPrivateAccess = true))
	float HipDisplacement;

	UPROPERTY()
	FRotator RotatorLastTick;

	UPROPERTY()
	float Target;
	

};
