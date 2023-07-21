// Copyright by Witold Studencki 2022


#include "Items/Weapons/Weapon.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

	MainSocketName = FName("RightHandSocket");
	ScabbardSocketName  = FName("SwordScabbard");
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FHitResult BoxHit;
	BoxTrace(BoxHit);
	if (BoxHit.GetActor())
	{
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		}


		CreateForceFields(BoxHit.ImpactPoint);

	}
}
void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		BoxTraceStart->GetComponentLocation(),
		BoxTraceEnd->GetComponentLocation(),
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());

}

void AWeapon::OnEquip(USceneComponent* InParent)
{
	Super::OnEquip(InParent);
}

void AWeapon::OnPickup(USceneComponent* InParent, AActor* NewOwner, APawn* NewInstigator)
{
	Super::OnPickup(InParent, NewOwner, NewInstigator);
	OnEquip(InParent);
}

void AWeapon::AttachToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	Super::AttachToSocket(InParent, InSocketName);
}

void AWeapon::OnUnequip(USceneComponent* InParent)
{
	Super::OnUnequip(InParent);
	AttachToSocket(InParent, ScabbardSocketName);
}