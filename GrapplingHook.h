// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "GrapplingHook.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AGrapplingHook : public ATool
{
	GENERATED_BODY()
	
public:

	AGrapplingHook();

	bool bThrown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool | Hook")
		class UProjectileMovementComponent* HookMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tool | Hook")
		class AEnemy* Target;

protected:
	virtual void BeginPlay() override;

public:

	virtual void PrimaryFunction() override;

	virtual void SecondaryFunction() override;

	virtual void Equip(class AMain* Char) override;

	virtual void SetInstigator(AController* Inst) override;

	virtual void SetToolOwner(AMain* Char) override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable)
	void PullEnemy(AEnemy* Enemy);

	void ResetHookPosition();
};
