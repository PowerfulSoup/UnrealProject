// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonDoorway.h"
#include "DungeonDoorwayLocked.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ADungeonDoorwayLocked : public ADungeonDoorway
{
	GENERATED_BODY()

public:
	ADungeonDoorwayLocked();

	UPROPERTY(VisibleAnywhere, Category = "Door")
		class UStaticMeshComponent* LockMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		class UParticleSystem* UnlockParticles;

protected:

	virtual void BeginPlay() override;

public:

	virtual void InteractFunction(AMain* Char) override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	virtual void OpenDoor() override;

	virtual void UpdateDoorLocation(float Z) override;

	virtual bool CheckConditions() override;

	
};
