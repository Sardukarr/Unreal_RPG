// Copyright by Witold Studencki 2023


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{

	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	AttackMontageSections.Add(FName("Attack1"));
	DeathMontageSections.Add(FName("Death1"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("BaseCharacter"));
}

void ABaseCharacter::Attack()
{
}

bool ABaseCharacter::CanAttack()
{
	return false;
}


void ABaseCharacter::Die_Implementation()
{
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}


void ABaseCharacter::DirectionalHit(const FVector& ImpactPoint)
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

float ABaseCharacter::PlayMontage(UAnimMontage* montage, const FName& sectionName, bool bOverride)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && montage)
	{
		AnimInstance->Montage_Play(montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, bOverride);
		if (!sectionName.IsNone())
		{
			AnimInstance->Montage_JumpToSection(sectionName, montage);

			return montage->GetSectionLength(montage->GetSectionIndex(sectionName));
		}

		return montage->GetPlayLength();
	}
	return 0.0f;
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}