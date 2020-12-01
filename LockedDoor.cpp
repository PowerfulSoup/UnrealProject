// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedDoor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Main.h"


// Sets default values
ALockedDoor::ALockedDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;

	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	TriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	TriggerVolume->SetBoxExtent(FVector(185.f, 70.f, 32.f));

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DoorFrame);

	Lock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock"));
	Lock->SetupAttachment(Door);



	bIsCharacterInRange = false;
	bIsLocked = true;
}

// Called when the game starts or when spawned
void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ALockedDoor::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ALockedDoor::OnOverlapEnd);

	InitialDoorLocation = Door->GetComponentLocation();

}

// Called every frame
void ALockedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALockedDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor && !bIsCharacterInRange)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			bIsCharacterInRange = true;
			Main->LockedDoorInRange = this;

		}

	}

}

void ALockedDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsCharacterInRange)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
		
		bIsCharacterInRange = false;
		Main->LockedDoorInRange = nullptr;
		}
	}
}

void ALockedDoor::UpdateDoorLocation(float Z)
{
	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += Z;
	Door->SetWorldLocation(NewLocation);
}

void ALockedDoor::UnlockDoor()
{	
	if (bIsCharacterInRange && bIsLocked)
	{ 
		
		if (UnlockParticles)
		{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UnlockParticles, Lock->GetComponentToWorld().GetLocation(), FRotator(0.f), true);
		}
	
	Lock->SetHiddenInGame(true);
	LowerDoor();
	bIsLocked = false;
	Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	} 
}