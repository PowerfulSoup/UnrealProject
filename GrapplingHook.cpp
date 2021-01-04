// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"
#include "Main.h"
#include "Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Math/UnrealMathUtility.h"

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
}

void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();
}

void AGrapplingHook::PrimaryFunction()
{
	Super::PrimaryFunction();
	if (!bThrown)
	{
		bThrown = true;
		CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		SkeletalMesh->SetSimulatePhysics(true);
		SkeletalMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		FVector Rotation = ToolOwner->GetActorForwardVector() * 1000.f; //2000

		HookMovement->Velocity = FVector(Rotation.X,Rotation.Y,Rotation.Z+270);
		HookMovement->ProjectileGravityScale = 0.8f;

		FRotator HookRotation = GetActorRotation();
		HookRotation.Roll -= 100.f;
		SetActorRotation(HookRotation);

		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
				PullEnemy(Target);
				ToolOwner->RecallGrapplingHook();
				ResetHookPosition();
			}
		}
		HookMovement->Velocity = FVector(0.f, 0.f, 0.f);
		HookMovement->ProjectileGravityScale = 0.f;
	}
}

void AGrapplingHook::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AGrapplingHook::PullEnemy(AEnemy* Enemy)
{
	FVector Location = ToolOwner->GetActorLocation() + 75.f;
	Enemy->SetActorLocation(Location);

	Target = nullptr;
}

void AGrapplingHook::ResetHookPosition()
{
	bThrown = false;
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	const USkeletalMeshSocket* RightHandSocket = ToolOwner->GetMesh()->GetSocketByName("RightHandSocket");
	FVector SocketLocation = ToolOwner->GetMesh()->GetSocketLocation("RightHandSocket");

	if (RightHandSocket)
	{
		RightHandSocket->AttachActor(this, ToolOwner->GetMesh());
	}
	SkeletalMesh->SetSimulatePhysics(false);

	FMath::VInterpTo(GetActorLocation(), SocketLocation, GetWorld()->GetDeltaSeconds(), .1f);
	//SetActorLocation(SocketLocation);
}
