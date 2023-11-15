// Created by Ashechol


#include "ShooterAnimInstance.h"
#include "Shooter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Shooter = Cast<AShooter>(TryGetPawnOwner());
}

void UShooterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Shooter) return;

	FVector Velocity = Shooter->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Length();

	bIsInAir = Shooter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = Shooter->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0;
}
