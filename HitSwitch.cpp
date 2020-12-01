// Fill out your copyright notice in the Description page of Project Settings.


#include "HitSwitch.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHitSwitch::AHitSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target Mesh"));
	TargetMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");

	RootComponent = TargetMesh;


}

// Called when the game starts or when spawned
void AHitSwitch::BeginPlay()
{
	Super::BeginPlay();
	TargetMesh->OnComponentHit.AddDynamic(this, &AHitSwitch::OnHit);
}

// Called every frame
void AHitSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitSwitch::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

}

