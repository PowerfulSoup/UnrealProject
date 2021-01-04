// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "Bomb.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ABomb : public ATool
{
	GENERATED_BODY()
public:
	ABomb();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool | Bomb")
		class USphereComponent* ExplosionRadius;

	FTimerHandle FuseTimer;

protected:

	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void PrimaryFunction() override;
	
	virtual void SecondaryFunction() override;

	virtual void Equip(class AMain* Char) override;

	virtual void SetInstigator(AController* Inst) override;

	void BeginCountDown();

	void Detonate();
};
