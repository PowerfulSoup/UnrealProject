// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Main.h"

// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = Mesh;

	NorthBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("NorthBoxCollision"));
	NorthBoxCollision->SetupAttachment(GetRootComponent());

	SouthBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SouthBoxCollision"));
	SouthBoxCollision->SetupAttachment(RootComponent);

	EastBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EastBoxCollision"));
	EastBoxCollision->SetupAttachment(RootComponent);

	WestBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WestBoxCollision"));
	WestBoxCollision->SetupAttachment(RootComponent);

	bIsBeingPushed = false;

	PushStatus = EPushStatus::EMS_Idle;

}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();

	NorthBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABlock::NorthBoxOnOverlapBegin);
	NorthBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABlock::NorthBoxOnOverlapEnd);

	SouthBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABlock::SouthBoxOnOverlapBegin);
	SouthBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABlock::SouthBoxOnOverlapEnd);

	EastBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABlock::EastOnOverlapBegin);
	EastBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABlock::EastOnOverlapEnd);

	WestBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABlock::WestOnOverlapBegin);
	WestBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABlock::WestOnOverlapEnd);
	
	InitialLocation = GetActorLocation();
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (PushStatus)
	{
	case EPushStatus::EMS_Idle:
		break;
	case EPushStatus::EMS_NorthOn:
		SetActorLocation((GetActorLocation() + FVector(1.f, 0.f, 0.f)), false, nullptr);

		break;
	case EPushStatus::EMS_SouthOn:
		SetActorLocation((GetActorLocation() + FVector(-1.f, 0.f, 0.f)), false, nullptr);

		break;
	case EPushStatus::EMS_EastOn:
		SetActorLocation((GetActorLocation() + FVector(0.f, 1.f, 0.f)), false, nullptr);

		break;
	case EPushStatus::EMS_WestOn:
		SetActorLocation((GetActorLocation() + FVector(0.f, -1.f, 0.f)), false, nullptr);

		break;
	case EPushStatus::EMS_MAX:
		break;
	default:
		break;
	}

}

void ABlock::NorthBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On "));

	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		SetPushStatus(EPushStatus::EMS_NorthOn);
		Main->BeginPushing();
	}

}

void ABlock::NorthBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Off "));

	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		SetPushStatus(EPushStatus::EMS_Idle);
		Main->StopPushing();

	}

}

void ABlock::SouthBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On south"));

	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		SetPushStatus(EPushStatus::EMS_SouthOn);
		Main->BeginPushing();

	}
}

void ABlock::SouthBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Off south"));
	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		SetPushStatus(EPushStatus::EMS_Idle);
		Main->StopPushing();

	}

}
void ABlock::EastOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On east"));

	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		SetPushStatus(EPushStatus::EMS_EastOn);
		Main->BeginPushing();

	}
}

void ABlock::EastOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Off east"));

	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		SetPushStatus(EPushStatus::EMS_Idle);
		Main->StopPushing();

	}
}
void ABlock::WestOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On West"));

	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		SetPushStatus(EPushStatus::EMS_WestOn);
		Main->BeginPushing();

	}
}

void ABlock::WestOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Off WEst"));

	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		SetPushStatus(EPushStatus::EMS_Idle);
		Main->StopPushing();
	}
}

