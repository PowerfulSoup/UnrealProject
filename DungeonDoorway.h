// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Doorway.h"
#include "DungeonDoorway.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ADungeonDoorway : public ADoorway
{
	GENERATED_BODY()
	
public:

	ADungeonDoorway();

	UPROPERTY(BlueprintReadWrite, Category = "Door")
		FVector InitialDoorLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
		AMain* MainCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	bool bLowered;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	bool bMoving;

protected:
	virtual void BeginPlay() override;

public:

	virtual void InteractFunction(AMain* Char) override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	virtual void OpenDoor() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void LowerDoor();

	UFUNCTION(BlueprintCallable, Category = "Door")
	virtual void UpdateDoorLocation(float Z);

	virtual bool CheckConditions();

};
