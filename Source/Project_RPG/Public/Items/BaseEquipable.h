// Copyright by Witold Studencki 2022

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "BaseEquipable.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_RPG_API ABaseEquipable : public AItem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual void OnPickup(USceneComponent* InParent, AActor* NewOwner, APawn* NewInstigator);
	void DisablePickableEffect();
	void DisableSphereCollision();
	void PlayEquipSound();
	UFUNCTION(BlueprintCallable)
	virtual void OnEquip(USceneComponent* InParent);
	UFUNCTION(BlueprintCallable)
	virtual void OnUnequip(USceneComponent* InParent);
	UFUNCTION(BlueprintCallable)
	virtual void AttachToSocket(USceneComponent* InParent, const FName& InSocketName);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsEquiped = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MainSocketName;
};
