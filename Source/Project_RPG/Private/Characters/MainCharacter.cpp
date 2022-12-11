// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Components/InputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GroomComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/Weapons/Weapon.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast< APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState == EActionState::EAS_Attacking) return;
	FVector2D moveValue = Value.Get<FVector2D>();

	if (Controller)
	{
		if (moveValue.X != 0.f)
		{
			const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, moveValue.X);
			
			//AddMovementInput(GetActorRightVector(), moveValue.X);
		//	AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw + 90.f, 0.f)).GetUnitAxis(EAxis::Y), moveValue.X);
		}
		if (moveValue.Y != 0.f)
		{
			
			const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, moveValue.Y);

			//const FVector Direction = FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y);
			//AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y), moveValue.Y);
			
			
			//AddMovementInput(GetActorForwardVector(), moveValue.Y);
		}
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	FVector2D AxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(AxisValue.X);
		AddControllerPitchInput(AxisValue.Y);
	}
}
void AMainCharacter::Equip()
{
	if(AWeapon* Weapon = Cast<AWeapon>(OverlappingItem))
	{
		Weapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
}
void AMainCharacter::Attack()
{
	if (CanAttack())
	{
	FName SectionName = FName();
	int32 Selection = FMath::RandRange(0, 1);

	switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}

		PlayMontage(AttackMontage, SectionName);
		ActionState = EActionState::EAS_Attacking;
	}
}

bool AMainCharacter::CanAttack()
{
	return AttackMontage && ActionState == EActionState::EAS_Unoccupied && CharacterState!=ECharacterState::ECS_Unequipped;
}

void AMainCharacter::PlayMontage(UAnimMontage* montage, FName sectionName, bool bOverride)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(montage,1.0f, EMontagePlayReturnType::MontageLength,0.0f, bOverride);
		if (!sectionName.IsNone())
		{
			AnimInstance->Montage_JumpToSection(sectionName, montage);
		}
	}
}
void AMainCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EIC->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AMainCharacter::Equip);
		EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMainCharacter::Attack);
	}

}

