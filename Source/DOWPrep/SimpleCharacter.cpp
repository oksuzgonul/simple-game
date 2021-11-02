// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SimplePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASimpleCharacter::ASimpleCharacter() :
	// Control parameters
	BaseTurnRate(50.f),
	BaseLookupRate(50.f),
	MouseTurnRate(1.f),
	MouseLookupRate(1.f),
	// Movement parameters
	BaseMovementSpeed(650.f),
	RunSpeed(1200.f),
	RollHoldTime(1.f),
	bRollButtonPressed(false),
	// Character State
	CharacterState(ECharacterState::ECS_Unoccupied)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->SocketOffset = FVector(0.f, 100.f, 100.f);
	
	// Rotate arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
	// Camera does not rotate wrto the arm
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void ASimpleCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	
}

void ASimpleCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && (CharacterState == ECharacterState::ECS_Unoccupied || CharacterState == ECharacterState::ECS_Running))
	{
		// Get the forward direction
		const FRotator Rotation{ Controller->GetControlRotation() };

		// Rotation.Yaw is between 0 to 360
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void ASimpleCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && (CharacterState == ECharacterState::ECS_Unoccupied || CharacterState == ECharacterState::ECS_Running))
	{
		// Get the forward direction
		const FRotator Rotation{ Controller->GetControlRotation() };

		// Rotation.Yaw is between 0 to 360
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void ASimpleCharacter::TurnAtRate(float Rate)
{
	/** Calculate the delta turn rate each frame 
	* deg/sec * sec/frame
	*/
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASimpleCharacter::LookupRate(float Rate)
{	
	/** Calculate the delta turn rate each frame
	* deg/sec * sec/frame
	*/
	AddControllerPitchInput(Rate * BaseLookupRate * GetWorld()->GetDeltaSeconds());
}

void ASimpleCharacter::Turn(float Value)
{
	AddControllerYawInput(MouseTurnRate * Value);
}

void ASimpleCharacter::Lookup(float Value)
{
	AddControllerPitchInput(MouseLookupRate * Value);
}

void ASimpleCharacter::RollRunPressed()
{
	bRollButtonPressed = true;
	SetRollTimer();
}

void ASimpleCharacter::RollRunHold()
{
	if (bRollButtonPressed && (CharacterState == ECharacterState::ECS_Unoccupied || CharacterState == ECharacterState::ECS_Running))
	{
		Run();
		SetRollTimer();
	}
}

void ASimpleCharacter::RollRunReleased()
{
	bRollButtonPressed = false;
	if (CharacterState == ECharacterState::ECS_Running)
	{
		StopRunning();
	}
	else
	{
		Roll();
	}
}

void ASimpleCharacter::SetRollTimer()
{
	GetWorldTimerManager().SetTimer(
		RollHoldTimer,
		this,
		&ASimpleCharacter::RollRunHold,
		RollHoldTime);
}

void ASimpleCharacter::Run()
{
	CharacterState = ECharacterState::ECS_Running;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ASimpleCharacter::StopRunning()
{
	CharacterState = ECharacterState::ECS_Unoccupied;
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
}

void ASimpleCharacter::Roll()
{
	if (CharacterState == ECharacterState::ECS_Unoccupied || CharacterState == ECharacterState::ECS_Running) 
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		FName Section = FName(TEXT("Roll"));
		if (AnimInstance && RollMontage)
		{
			AnimInstance->Montage_Play(RollMontage);
			AnimInstance->Montage_JumpToSection(Section, RollMontage);
		}
	}
}

void ASimpleCharacter::CharacterRollingStart()
{
	CharacterState = ECharacterState::ECS_Rolling;
}

void ASimpleCharacter::CharacterRollingEnd()
{
	CharacterState = ECharacterState::ECS_Unoccupied;
}

// Called every frame
void ASimpleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASimpleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASimpleCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASimpleCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASimpleCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASimpleCharacter::LookupRate);
	PlayerInputComponent->BindAxis("Turn", this, &ASimpleCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASimpleCharacter::Lookup);

	PlayerInputComponent->BindAction("RollRun", IE_Pressed, this, &ASimpleCharacter::RollRunPressed);
	PlayerInputComponent->BindAction("RollRun", IE_Released, this, &ASimpleCharacter::RollRunReleased);
}

