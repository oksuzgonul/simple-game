// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAnimInstance.h"
#include "SimpleCharacter.h"

USimpleAnimInstance::USimpleAnimInstance() {

}

void USimpleAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (SimpleCharacter == nullptr)
	{
		SimpleCharacter = Cast<ASimpleCharacter>(TryGetPawnOwner());
	}

	if (SimpleCharacter)
	{
		FVector Velocity{ SimpleCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();
	}
}
