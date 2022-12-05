// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Project_RPG/DebugMacros.h"

AItem::AItem()
{

	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Begin Play called!"));
	if(GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("before check")));
	UWorld* World = GetWorld();
	FVector Forward = GetActorForwardVector();
	FVector Location = GetActorLocation();
	DRAW_SPHERE(Location)
	//DRAW_LINE(Location, Location + Forward * 100.f)
	DRAW_VECTOR(Location, Location + Forward * 100.f);
	}
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}
float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

