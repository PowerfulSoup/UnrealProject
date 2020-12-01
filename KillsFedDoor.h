// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillsFedDoor.generated.h"

UCLASS()
class RPG_API AKillsFedDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillsFedDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
		class UBoxComponent* LockTriggerVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
		UBoxComponent* EnemyVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
		class UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
		UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
		int32 KillRequirement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
		int32 Kills;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
		TArray<AActor*> RelatedEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
		class APlayerController* MainPlayerController;

	bool bIsTripped;

	FVector InitialDoorLocation;

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
		void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
		void RaiseDoor();

	UFUNCTION(BlueprintCallable, Category = "Door")
		void UpdateDoorLocation(float Z);

	UFUNCTION()
		void UpdateKillCount();
};
