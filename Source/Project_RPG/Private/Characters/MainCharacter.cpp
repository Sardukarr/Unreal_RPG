// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"

#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GroomComponent.h"

#include "Camera/CameraComponent.h"
#include "Items/Weapons/Weapon.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagsManager.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

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
	
	Tags.Add(FName("PlayerCharacter"));
	//GameplayTags = UGameplayTagsManager::Get().RequestGameplayTag("Character.Player");

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
	if (ActionState != EActionState::EAS_Unoccupied) return;
	
	FVector2D moveValue = Value.Get<FVector2D>();

	if (Controller)
	{
		if (moveValue.X != 0.f)
		{
			const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, moveValue.X);

		}
		if (moveValue.Y != 0.f)
		{
			
			const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, moveValue.Y);
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
		Weapon->OnPickup(GetMesh(), this, this);

		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		EquippedWeapon = Weapon;
		OverlappingItem = nullptr;

	}
	else
	{
		if (ActionState == EActionState::EAS_Unoccupied &&
			CharacterState != ECharacterState::ECS_Unequipped)
		{

			PlayMontage(EquipMontage, FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;

		}
		else if (ActionState == EActionState::EAS_Unoccupied &&
			CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon)
		{
			PlayMontage(EquipMontage, FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}


	}
}
void AMainCharacter::Attack()
{
	if (CanAttack())
	{
	int32 Selection = FMath::RandRange(0, AttackMontageSections.Num()-1);
	FName SectionName = AttackMontageSections[Selection];
	
	//FName SectionName = FName(FString("Attack" + FString::FromInt(Selection)));
	

		PlayMontage(AttackMontage, SectionName);
		ActionState = EActionState::EAS_Attacking;
	}
}


bool AMainCharacter::CanAttack()
{
	return AttackMontage && ActionState == EActionState::EAS_Unoccupied && CharacterState!=ECharacterState::ECS_Unequipped;
}

void AMainCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->OnUnequip(GetMesh());
	}
}
void AMainCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->OnEquip(GetMesh());
	}
}

void AMainCharacter::ResetCharacterState()
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

void AMainCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);
	ActionState = EActionState::EAS_HitReaction;
}

