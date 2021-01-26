// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"
#include "Main.h"
#include "Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "GrappleObject.h"

AGrapplingHook::AGrapplingHook()
{
	SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	HookMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowMovement"));
	HookMovement->ProjectileGravityScale = 0.f;

	SkeletalMesh->SetSimulatePhysics(false);

	ToolName = "GrapplingHook";
	bIsEquippedInRightHand = true;

	Damage = 5.f;
	bHasTarget = false;
	ThrowDuration = 1.65f;
}

void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();
}

void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHasTarget)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector SocketLocation = ToolOwner->GetMesh()->GetSocketLocation("RightHandSocket");
		FVector Interp = FMath::VInterpConstantTo(CurrentLocation, SocketLocation, DeltaTime, 2200.f);
		SetActorLocation(Interp);
		if (SocketLocation.Equals(CurrentLocation,10.f))
		{
			SetActorLocation(SocketLocation);
			AttachToOwner();
			bHasTarget = false; 
		}
	}
}

void AGrapplingHook::PrimaryFunction()
{
	Super::PrimaryFunction();
	if (!bThrown)
	{
		bThrown = true;
		//CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); to make sure it can overlap Puzzle Channel
		CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

		SkeletalMesh->SetSimulatePhysics(true);
		SkeletalMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		FVector Rotation = ToolOwner->GetActorForwardVector() * 1000.f; //2000

		HookMovement->Velocity = FVector(Rotation.X,Rotation.Y,Rotation.Z+270);
		HookMovement->ProjectileGravityScale = 0.8f;

		FRotator HookRotation = GetActorRotation();
		HookRotation.Roll -= 100.f;
		SetActorRotation(HookRotation);

		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		GetWorldTimerManager().SetTimer(ThrownTimer, this, &AGrapplingHook::ResetHookParams, ThrowDuration);
	}
}

void AGrapplingHook::SecondaryFunction()
{
	Super::SecondaryFunction();
	ToolOwner->ToggleZoomCamera();
}

void AGrapplingHook::Equip(AMain* Char)
{
	Super::Equip(Char);
}

void AGrapplingHook::SetInstigator(AController* Inst)
{
	Super::SetInstigator(Inst);
}

void AGrapplingHook::SetToolOwner(AMain* Char)
{
	Super::SetToolOwner(Char);
}

void AGrapplingHook::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			Target = Enemy;
			UGameplayStatics::ApplyDamage(Target, Damage, ToolInstigator, this, DamageTypeClass);
			if (Target->Alive())
			{
				bHasTarget = true;
				PullEnemy(Target);
				ResetHookParams();
			}
			if (ToolOwner->bIsZoomed)
			{
				ToolOwner->ToggleZoomCamera();
			}
		}
		else
		{
			AGrappleObject* GrappleObject = Cast<AGrappleObject>(OtherActor);
			if (GrappleObject)
			{
				GrappleObject->GrappleObjectFuntion();
				bHasTarget = true;
				ResetHookParams();

				if (ToolOwner->bIsZoomed)
				{
					ToolOwner->ToggleZoomCamera();
				}
			}
		}
	}
}

void AGrapplingHook::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AGrapplingHook::PullEnemy(AEnemy* Enemy)
{
	FVector Location = ToolOwner->GetActorLocation() + 75.f;
	Enemy->GetGrappled(Location);

	Target = nullptr;
}

void AGrapplingHook::ResetHookParams()
{
	if (bThrown)
	{
		ToolOwner->RecallGrapplingHook();
		HookMovement->Velocity = FVector(0.f, 0.f, 0.f);
		HookMovement->ProjectileGravityScale = 0.f;
		if (!bHasTarget)
		{
			bHasTarget = true;
		}

		//CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); 
		CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		bThrown = false;
	}
}

void AGrapplingHook::AttachToOwner()
{
	const USkeletalMeshSocket* RightHandSocket = ToolOwner->GetMesh()->GetSocketByName("RightHandSocket");

	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(this, ToolOwner->GetMesh());
	}
	SkeletalMesh->SetSimulatePhysics(false);
}
