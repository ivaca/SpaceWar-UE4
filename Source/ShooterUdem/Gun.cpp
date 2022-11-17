// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "DrawDebugHelpers.h"
#include "Character/ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh2"));
	Mesh2->SetupAttachment(RootComponent);
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh2,TEXT("MuzzleFlashSocket"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	auto OwnerController = OwnerPawn->GetController();
	if (!OwnerController) return;;


	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwnerPawn);

	
	FVector End = Location + Rotation.Vector() * MaxRange;
	FHitResult HitResult;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECC_GameTraceChannel1, QueryParams);
	if (HitResult.bBlockingHit)
	{
		FVector ShotDir = -Rotation.Vector();
		//DrawDebugPoint(GetWorld(), HitResult.Location, 25, FColor::Cyan, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash, HitResult.Location, ShotDir.Rotation());

		AActor* HitActor = HitResult.GetActor();
		if (!HitActor) return;

		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDir, nullptr);
		HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
