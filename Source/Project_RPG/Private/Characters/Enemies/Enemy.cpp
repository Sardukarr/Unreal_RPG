// Copyright by Witold Studencki 2022


#include "Characters/Enemies/Enemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/Weapons/Weapon.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBar.h"

#include "Project_RPG/DebugMacros.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	HealthBarWidget = CreateDefaultSubobject<UHealthBar>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	
	/*PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);*/

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyAIController = Cast<AMainAIController>(GetController());
	Blackboard = EnemyAIController->GetBlackboardComponent();
	ToggleHealthBar(false);

	//MoveToTarget(PatrolTarget);
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->OnPickup(GetMesh(), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemy::MoveToTarget(AActor* Target, float AcceptanceRadious)
{
	if (EnemyAIController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadious);
	FNavPathSharedPtr NavPath;
	EnemyAIController->MoveTo(MoveRequest, &NavPath);
}

AActor* AEnemy::ChooseRandomPatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		PatrolTarget = ValidTargets[TargetSelection];
		return PatrolTarget;
	}
	return nullptr;
}

AActor* AEnemy::ChooseNextPatrolTarget()
{
	int32 Position;
	if(PatrolTargets.Num()>0)
	{
		if (PatrolTargets.Find(PatrolTarget, Position))
		{			
			int32 NextElement = (Position + 1) % (PatrolTargets.Num());
			PatrolTarget = PatrolTargets[NextElement];
		}
		else
		{
			PatrolTarget = PatrolTargets[0];
		}
		return PatrolTarget;
	}
	

	return nullptr;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (State > EEnemyState::EES_Dead)
	{
		//CheckCombatTarget();
	}
	//CheckPatrolTarget();
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		LoseInterest();
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && State != EEnemyState::EES_Chasing)
	{
		Chase();
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && State != EEnemyState::EES_Attacking)
	{
		Attack();
	}
}

void AEnemy::Chase()
{
	// Outside attack range, chase character
	State = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	//MoveToTarget(CombatTarget);
}

float AEnemy::StartAttack()
{
	// Inside attack range, attack character
	State = EEnemyState::EES_Attacking;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		int32 Selection = FMath::RandRange(0, AttackMontageSections.Num() - 1);
		FName SectionName = AttackMontageSections[Selection];
		return PlayMontage(AttackMontage, SectionName);
	}
	return 0.0f;
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideRadius(CombatRadius) &&
		!IsAttacking() &&
		!IsBusy() &&
		!IsDead();
	return bCanAttack;
}


bool AEnemy::IsInsideRadius(double Radious)
{
	return InTargetRange(CombatTarget, Radious);
}

bool AEnemy::IsChasing()
{
	return State == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return State == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return State == EEnemyState::EES_Dead;
}

bool AEnemy::IsBusy()
{
	return State == EEnemyState::EES_Busy;
}


void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	if(Blackboard)
	{
		//Blackboard->SetValueAsObject(FName("CombatTarget"), nullptr);
	}
	ToggleHealthBar(false);
	State = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	//MoveToTarget(PatrolTarget);
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		ChooseRandomPatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}

}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;

}

void AEnemy::ToggleHealthBar(bool show)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(show);
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		State != EEnemyState::EES_Dead &&
		State != EEnemyState::EES_Chasing &&
		State < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("PlayerCharacter"));

	if (bShouldChaseTarget)
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		CombatTarget = SeenPawn;
		Chase();
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{

	ToggleHealthBar(true);

	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHit(ImpactPoint);

	}
	else
	{
		Die();
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}

}


float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		if (Blackboard)
		{
	//			Blackboard->SetValueAsObject(FName("CombatTarget"), EventInstigator->GetPawn());
		}
	}
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

void AEnemy::Die_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		int32 Selection = FMath::RandRange(0, DeathMontageSections.Num() - 1);
		FName SectionName = DeathMontageSections[Selection];

		PlayMontage(DeathMontage, SectionName);
	}
	State = EEnemyState::EES_Dead;

	ToggleHealthBar(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(120.f);
}


void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}