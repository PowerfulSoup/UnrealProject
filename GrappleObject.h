// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleObject.generated.h"

UCLASS()
class RPG_API AGrappleObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GrappleObject")
		class UStaticMeshComponent* HookMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GrappleObject")
		UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GrappleObject")
		class UParticleSystemComponent* HookParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GrappleObject")
	bool bActivated;

	FRotator InitialRotation;
	FVector InitialLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void GrappleObjectFuntion();
};
