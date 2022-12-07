// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	RootComponent = Capsule;

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	BirdMesh->SetupAttachment(Capsule);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);



	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast< APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
	
}

void ABird::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ABird::MoveRight(float Value)
{
	if (Controller && (Value != 0.f))
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ABird::Turn(float Value)
{

	AddControllerYawInput(Value);


}

void ABird::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABird::Move(const FInputActionValue& Value)
{
	FVector2D moveValue=Value.Get<FVector2D>();

	if (Controller)
	{
		if (moveValue.X != 0.f)
		{
			AddMovementInput(GetActorRightVector(), moveValue.X);
		}
		if (moveValue.Y != 0.f)
		{
			AddMovementInput(GetActorForwardVector(), moveValue.Y);
		}
	}
}

void ABird::Look(const FInputActionValue& Value)
{
	FVector2D AxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(AxisValue.X);
		AddControllerPitchInput(AxisValue.Y);
	}
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//old way

	//PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
	//PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABird::MoveRight);
	//PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
	//PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABird::LookUp);

	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}
}

