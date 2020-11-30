// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AArrow : public AWeapon
{
	GENERATED_BODY()
	
public:
	AArrow();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
		class UProjectileMovementComponent* ArrowMovement;

protected:
	virtual void BeginPlay() override;

public:

	virtual void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
