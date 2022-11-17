// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "ShooterUdem/Gun.h"
#include "ShooterUdem/ShooterUdemGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(WeaponBone, EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_socket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorldTimerManager().TimerExists(ShootTimerHandle))
		TimeBetweenShots += DeltaTime;
	else
	{
		TimeBetweenShots = 0.0f;
	}
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this,
	                               &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this,
	                               &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this,
	                               &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this,
	                               &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterCharacter::StopShoot);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                    AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Error, TEXT("Health: %f"), Health);
	if (IsDead())
	{
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AShooterUdemGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AShooterUdemGameModeBase>();
		if(GameMode)
		{
			GameMode->PawnKilled(this); 
		}
		DetachFromControllerPendingDestroy();
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

void AShooterCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AShooterCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AShooterCharacter::Shoot()
{
	if(IsDead()) return;
	FTimerDelegate ShootCallback;
	AAIController* AIController =  Cast<AAIController>(GetController());

	if (AIController)
	{
		Gun->PullTrigger();
		return;
	}
	ShootCallback.BindLambda([this]
	{
		Gun->PullTrigger();
		
		TimeBetweenShots = 0.0f;
	});
	GetWorldTimerManager().SetTimer(ShootTimerHandle, ShootCallback, FireRate, true, 0.0f);
}

void AShooterCharacter::StopShoot()
{
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}
