// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGCharacter.h"


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
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_SPARROW(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));
	if (SK_SPARROW.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_SPARROW.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		SPARROW_ANIM(TEXT("/Game/Blueprints/AnimBlueprint.AnimBlueprint_C"));
	if (SPARROW_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SPARROW_ANIM.Class);
	}

	SetControlMode(0);

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
		SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
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

}

void ARPGCharacter::PostInitializeComponents()
{
}

void ARPGCharacter::PossessedBy(AController * NewController)
{
}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ARPGCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ARPGCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ARPGCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ARPGCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ARPGCharacter::Jump);

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

