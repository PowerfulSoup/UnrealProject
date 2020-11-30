// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ABow : public ATool
{
	GENERATED_BODY()
	
public:
	ABow();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Bow")
	bool bDrawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bow")
		UStaticMeshComponent* ArrowPlaceHolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
		TSubclassOf<class AArrow> ArrowToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
		FVector InitialArrowLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bow")
		FVector TrueInitialArrowPHLocation;

protected:

	virtual void BeginPlay() override;

public:

	
	virtual void PrimaryFunction() override;

	virtual void SecondaryFunction() override;
	
	virtual void Equip(class AMain* Char) override;

	virtual void SetInstigator(AController* Inst) override;

	UFUNCTION(BlueprintCallable)
	void Fire();

	virtual void SetToolOwner(AMain* Char) override;


	UFUNCTION(BlueprintCallable)
		void DrawArrowPlaceHolder(float X);

};
