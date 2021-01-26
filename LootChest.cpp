// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChest.h"
#include "Main.h"
#include "Components/StaticMeshComponent.h"
#include "Item.h"
#include "Components/WidgetComponent.h"

ALootChest::ALootChest()
{


	ChestLid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestLid"));
	ChestLid->SetupAttachment(GetRootComponent());
	ChestLid->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void ALootChest::BeginPlay()
{
	Super::BeginPlay();

	InitialLidLocation = ChestLid->GetComponentLocation();
	InitialLidRotation = ChestLid->GetComponentRotation();
}

void ALootChest::InteractFunction(AMain* Char)
{
	Super::InteractFunction(Char);

	OpenItemChestStart();
	InteractWidget->DestroyComponent();
}

void ALootChest::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ALootChest::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ALootChest::UpdateLidLocation(float Value)
{
	FVector NewLocation = InitialLidLocation;
	NewLocation.X -= Value;
	ChestLid->SetWorldLocation(NewLocation);
}

void ALootChest::UpdateLidRotation(float Value)
{
	FRotator NewRotation = InitialLidRotation;
	NewRotation.Pitch -= Value;
	ChestLid->SetWorldRotation(NewRotation);
}

void ALootChest::SpawnChestContents()
{
	FActorSpawnParameters SpawnInfo;
	FRotator Direction = GetActorRotation();
	FVector Location = GetActorLocation() + FVector(0.f, 0.f, 100.f);

	AItem* ItemToSpawn = GetWorld()->SpawnActor<AItem>(ChestContents->GetDefaultObject()->GetClass(), Location, Direction, SpawnInfo);
}

