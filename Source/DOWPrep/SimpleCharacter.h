// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SimpleCharacter.generated.h"

UCLASS()
class DOWPREP_API ASimpleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASimpleCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** --MOVEMENT-- */
	// Forward/Backward movement
	void MoveForward(float Value);
	// Left/Right movement
	void MoveRight(float Value);
	/**
	* Turn at a given rate
	* @param Rate Normalized rate. (1.0 means 100%)
	*/
	void TurnAtRate(float Rate);
	/**
	* Turn at a given rate
	* @param Rate Normalized rate. (1.0 means 100%)
	*/
	void LookupRate(float Rate);
	/**
	* Rotate controller based on mouse X movement
	* @param Value Mouse Input.
	*/
	void Turn(float Value);
	/**
	* Rotate controller based on mouse Y movement
	* @param Value Mouse Input.
	*/
	void Lookup(float Value);

	/** 
	* Action bound to RollRun input.
	* Tap for roll pressed for run
	*/
	void RollRunPressed();
	void RollRunHold();
	void RollRunReleased();
	void SetRollTimer();

	/** Character speed is increased and run animation is activated by setting bIsRunning boolean */
	void Run();
	/** Reverse what Run() does */
	void StopRunning();

	/** Character performs a roll */
	void Roll();

private:
	// Follow camera will be mounted on the camera boom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	// The camera that follows the player, will be mounted on the camera boom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// the multiplier for base turn rate (degree/second)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	// the multiplier for base Lookup rate (degree/second)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookupRate;

	// scaling for turn rate (clamped between 0 - 1)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseTurnRate;

	// scaling for lookup rate (clamped between 0 - 1)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseLookupRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed;

	// is the character running?
	bool bIsRunning;
	FTimerHandle RollHoldTimer;
	float RollHoldTime;

	// is the RollRun button pressed?
	bool bRollButtonPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* RollMontage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE bool IsRunning() const { return bIsRunning; }
};
