// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitSwitch.h"
#include "HitSwitchOpenDoor.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AHitSwitchOpenDoor : public AHitSwitch
{
	GENERATED_BODY()
	
public:
	AHitSwitchOpenDoor();

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitSwitch")
	//	TArray<class ADoor*> DoorsToOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitSwitch")
		TArray<class ADungeonDoorway*> DungeonDoorwaysToOpen;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SwitchFunction() override;

	virtual void ToggleMeshMaterial() override;

	virtual void ToggleReadyStatus() override;

	virtual void ReadyTimer() override;

};
