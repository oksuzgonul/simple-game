// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SimplePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASimpleCharacter::ASimpleCharacter() : 
	BaseTurnRate(50.f),
	BaseLookupRate(50.f),
	MouseTurnRate(1.f),
	MouseLookupRate(1.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 100.f, 100.f);
	
	// Rotate arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
	// Camera does not rotate wrto the arm
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void ASimpleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASimpleCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
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
	if ((Controller != nullptr) && (Value != 0.0f))
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

}

