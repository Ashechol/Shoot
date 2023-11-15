// Created by Ashechol


#include "Shooter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AShooter::AShooter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	// 绑在 Mesh 上，因为 Collider 的大小可能会调整
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidget->SetupAttachment(RootComponent);
}

void AShooter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
}

void AShooter::Move(const FInputActionValue& Value)
{
	if (!Controller || !Value.IsNonZero()) return;

	FVector2D moveInput = Value.Get<FVector2D>();
	
	const FRotator Yaw(0, Controller->GetControlRotation().Yaw, 0);

	const FVector Forward = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(Forward, moveInput.Y);
	AddMovementInput(Right, moveInput.X);
}

void AShooter::Look(const FInputActionValue& Value)
{
	if (!Controller || !Value.IsNonZero()) return;

	FVector2D lookInput = Value.Get<FVector2D>();

	AddControllerYawInput(lookInput.X);
	AddControllerPitchInput(lookInput.Y);
}

void AShooter::BeginPlay()
{
	Super::BeginPlay();

	SetupMappingContext();
}


void AShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

typedef UEnhancedInputLocalPlayerSubsystem UInputSubsystem;
void AShooter::SetupMappingContext()
{
	if (!Controller) return;
	
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer()))
	{
		if (UInputSubsystem* InputSystem = LocalPlayer->GetSubsystem<UInputSubsystem>())
		{
			InputSystem->AddMappingContext(InputMapping, 0);
		}
	}
}

