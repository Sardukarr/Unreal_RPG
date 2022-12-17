// Copyright by Witold Studencki 2022


#include "Characters/Enemies/Enemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"

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

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBar>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	
	DeathMontageSections.Add(FName("Death1"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();
		if (DistanceToTarget > CombatRadius)
		{
			CombatTarget = nullptr;
			if (HealthBarWidget)
			{
				HealthBarWidget->SetVisibility(false);
			}
		}
	}

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}


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

void AEnemy::DirectionalHit(const FVector& ImpactPoint)
{
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(GetActorForwardVector(), ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	/*UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);*/


	FName Section("LargeBack");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("LargeFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("LargeLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("LargeRight");
	}

	PlayMontage(HitMontage, Section);
}

void AEnemy::PlayMontage(UAnimMontage* montage, const FName& sectionName, bool bOverride)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && montage)
	{
		AnimInstance->Montage_Play(montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, bOverride);
		if (!sectionName.IsNone())
		{
			AnimInstance->Montage_JumpToSection(sectionName, montage);
		}
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
	return DamageAmount;

	CombatTarget = EventInstigator->GetPawn();
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		int32 Selection = FMath::RandRange(0, DeathMontageSections.Num() - 1);
		FName SectionName = DeathMontageSections[Selection];

		PlayMontage(DeathMontage, SectionName);
	}
	State = EEnemyState::EES_Dead;

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(120.f);
}
