// Copyright by Witold Studencki 2022


#include "Items/BaseEquipable.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"

void ABaseEquipable::OnEquip(USceneComponent* InParent)
{
	AttachToSocket(InParent, MainSocketName);
	ItemState = EItemState::EIS_Equipped;
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ABaseEquipable::OnPickup(USceneComponent* InParent, AActor* NewOwner, APawn* NewInstigator)
{
	ItemMesh->SetSimulatePhysics(false);
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);

	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
		);
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (SparksEffect)
	{
		SparksEffect->Deactivate();
	}



}

void ABaseEquipable::OnUnequip(USceneComponent* InParent )
{
	ItemState = EItemState::EIS_Unequiped;
}

void ABaseEquipable::AttachToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	RootComponent->AttachToComponent(InParent, TransformRules, InSocketName);

}