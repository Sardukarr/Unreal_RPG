// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Project_RPG/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/MainCharacter.h"
AItem::AItem()
{

	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();



	if(GEngine)
	{
	//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("before check")));
	//UWorld* World = GetWorld();
	//FVector Forward = GetActorForwardVector();
	//FVector Location = GetActorLocation();
	//DRAW_SPHERE(Location)
	//DRAW_LINE(Location, Location + Forward * 100.f)
	//DRAW_VECTOR(Location, Location + Forward * 100.f);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
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

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
		if (Player)
		{
			Player->SetOverlappingItem(this);
		}
	}	
	//GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
	if (Player)
	{
		Player->SetOverlappingItem(nullptr);
	}
}


void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	//AddActorWorldRotation(FRotator(0.f, Amplitude* DeltaTime, 0.f));
}

