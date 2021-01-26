// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleObject.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AGrappleObject::AGrappleObject()
{
	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	RootComponent = ObjectMesh;

	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookMesh"));
	HookMesh->SetupAttachment(ObjectMesh);

	HookParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HookParticles"));
	HookParticleSystemComponent->SetupAttachment(HookMesh);

	bActivated = false;
}

// Called when the game starts or when spawned
void AGrappleObject::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorRotation();
	InitialLocation = GetActorLocation();
}

// Called every frame
void AGrappleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrappleObject::GrappleObjectFuntion()
{

}