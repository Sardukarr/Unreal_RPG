// Copyright by Witold Studencki 2022


#include "Characters/MainCharacterAnimInstance.h"
#include "Characters/MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if (MainCharacter)
	{
		MainCharacterMovement = MainCharacter->GetCharacterMovement();
	}
	else if(TryGetPawnOwner())
	{
		ControlledByAI = true;
	}
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MainCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(TryGetPawnOwner()->GetVelocity());
		IsFalling = MainCharacterMovement->IsFalling();
		CharacterState = MainCharacter->GetCharacterState();
	}
	else if (ControlledByAI)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(TryGetPawnOwner()->GetVelocity());
	}
}