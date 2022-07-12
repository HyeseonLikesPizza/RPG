// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGCharacter.h"
#include "RPGAnimInstance.h"
#include "DrawDebugHelpers.h"



// Sets default values
ARPGCharacter::ARPGCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	LegStart = CreateDefaultSubobject<USphereComponent>(TEXT("Object"));
	LegStart->SetAllMassScale(0.0f);
	LegStart->SetRelativeLocation(FVector(9.0f, 0.0f, 0.0f));
	LegStart->SetupAttachment(GetCapsuleComponent());


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_SPARROW(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));
	if (SK_SPARROW.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_SPARROW.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		SPARROW_ANIM(TEXT("/Game/Animations/AnimBlueprint.AnimBlueprint_C"));
	if (SPARROW_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SPARROW_ANIM.Class);
	}

	SetControlMode(0);

	FName BowEmitterSocket(TEXT("BowEmitterSocket"));
	if (GetMesh()->DoesSocketExist(BowEmitterSocket))
	{
		SocketObj = CreateDefaultSubobject<UObject>(TEXT("SocketObject"));
		
	}
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();


	
}

void ARPGCharacter::SetControlMode(int32 ControlMode)
{
	if (ControlMode == 0)
	{
		SpringArm->TargetArmLength = 300.0f;
		SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);


	}
}

// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	FVector StartLocation = LegStart->GetComponentLocation();
	FVector DownVec = LegStart->GetUpVector() * -500.0f;
	FVector EndLocation = StartLocation + DownVec;

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool IsHitResult = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_WorldStatic,
		Params
	);

	const float DebugLifeTime = 3.0f;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, DebugLifeTime);

	if (IsHitResult)
	{
	
	}
	*/

	// Leg IK - Slope (Right Leg)

	FVector RightFootLocation(GetMesh()->GetSocketLocation(FName("foot_r")).X,
		GetMesh()->GetSocketLocation(FName("foot_r")).Y,
		GetActorLocation().Z - 95.0f);

	FHitResult RHitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	if (GetWorld()->LineTraceSingleByChannel(
		RHitResult,
		RightFootLocation + FVector(0.0f, 0.0f, 100.0f),
		RightFootLocation - FVector(0.0f, 0.0f, 100.0f),
		ECC_Camera,
		Params))
	{
		Anim->SetIKAlphaRight(((RHitResult.Location - RightFootLocation)).Z / 30.0f);
	}

	const float DebugLifeTime = 3.0f;

	//DrawDebugLine(GetWorld(), RightFootLocation,
	//	RightFootLocation - FVector(0.0f, 0.0f, 100.0f), FColor::Red, false, DebugLifeTime);

	// Leg IK - Slpe (Left Leg)

	FHitResult LHitResult;

	FVector LeftFootLocation(GetMesh()->GetSocketLocation(FName("foot_l")).X,
		GetMesh()->GetSocketLocation(FName("foot_l")).Y,
		GetActorLocation().Z - 95.0f);

	if (GetWorld()->LineTraceSingleByChannel(
		LHitResult,
		LeftFootLocation + FVector(0.0f, 0.0f, 100.0f),
		LeftFootLocation - FVector(0.0f, 0.0f, 100.0f),
		ECC_Camera,
		Params))
	{
		Anim->SetIKAlphaLeft(((LHitResult.Location - LeftFootLocation)).Z / 30.0f);
	}

	//DrawDebugLine(GetWorld(), LeftFootLocation,
	//	LeftFootLocation - FVector(0.0f, 0.0f, 100.0f), FColor::Red, false, DebugLifeTime);


	Anim->SetHipDisplacement(-0.5f*abs(RHitResult.Location.Z - LHitResult.Location.Z));

	
}

void ARPGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Anim = Cast<URPGAnimInstance>(GetMesh()->GetAnimInstance());
}

void ARPGCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ARPGCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ARPGCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ARPGCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ARPGCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("Sprint"), this, &ARPGCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ARPGCharacter::Jump);

}

void ARPGCharacter::Fire()
{
	if (ProjectileClass)
	{
		
	}


}

void ARPGCharacter::UpDown(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void ARPGCharacter::LeftRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void ARPGCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void ARPGCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void ARPGCharacter::Sprint(float NewAxisValue)
{
}

