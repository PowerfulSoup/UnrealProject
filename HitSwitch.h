// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitSwitch.generated.h"

UCLASS()
class RPG_API AHitSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitSwitch();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hit Switch")
		class UStaticMeshComponent* TargetMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hit Switch")
	bool bSwitchedOn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Switch")
	bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Switch")
	bool bCanToggle;

	bool bReady;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Switch")
		class UMaterial* HitSwitchMaterialOn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Switch")
		UMaterial* HitSwitchMaterialOff;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Switch")
		float SwitchTimerDuration;

	FTimerHandle SwitchTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SwitchFunction();

	UFUNCTION()
	virtual void ToggleMeshMaterial();

	virtual void ToggleReadyStatus();

	virtual void ReadyTimer();

};
