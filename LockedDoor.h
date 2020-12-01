// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockedDoor.generated.h"

UCLASS()
class RPG_API ALockedDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockedDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Locked Door")
		class UBoxComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LockedDoor")
		class UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Locked Door")
		UStaticMeshComponent* Lock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Locked Door")
		UStaticMeshComponent* DoorFrame;

	UPROPERTY(BlueprintReadWrite, Category = "Locked Door")
		FVector InitialDoorLocation;

	bool bIsCharacterInRange;

	bool bIsLocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locked Door")
		class UParticleSystem* UnlockParticles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Locked Door")
		void UpdateDoorLocation(float Z);

	UFUNCTION(BlueprintImplementableEvent, Category = "Locked Door")
		void LowerDoor();

	void UnlockDoor();

};
