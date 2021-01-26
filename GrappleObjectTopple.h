// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrappleObject.h"
#include "GrappleObjectTopple.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AGrappleObjectTopple : public AGrappleObject
{
	GENERATED_BODY()

public:
	AGrappleObjectTopple();

protected:

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void GrappleObjectFuntion() override;

	UFUNCTION(BlueprintCallable)
		void Topple(float Roll);

	UFUNCTION(BlueprintImplementableEvent)
		void ToppleStart();
};
