// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitSwitch.h"
#include "HitSwitchSpawnActor.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AHitSwitchSpawnActor : public AHitSwitch
{
	GENERATED_BODY()

public:
	AHitSwitchSpawnActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Switch")
		AActor* ActorToShow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Switch")
		class UParticleSystem* ActorSpawnParticle;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SwitchFunction() override;

	virtual void ToggleMeshMaterial() override;

	virtual void ToggleReadyStatus() override;

	virtual void ReadyTimer() override;

};
