// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Doorway.h"
#include "WorldDoorway.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AWorldDoorway : public ADoorway
{
	GENERATED_BODY()
	
public:
	AWorldDoorway();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	bool bMoving;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	bool bOpened;

	UPROPERTY(BlueprintReadWrite, Category = "Door")
	FRotator InitialDoorRotation;

	FTimerHandle CloseTimer;

protected:
	virtual void BeginPlay() override;

public:

	virtual void InteractFunction(AMain* Char) override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	virtual void OpenDoor() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
		void SwingOpen();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
		void SwingClosed();

	UFUNCTION(BlueprintCallable, Category = "Door")
		void UpdateDoorRotation(float Yaw);
};
