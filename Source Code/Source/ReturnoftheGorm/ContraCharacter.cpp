// Copyright Epic Games, Inc. All Rights Reserved.

#include "ContraCharacter.h"
#include "ReturnoftheGormGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gun.h"
#include "KillEmAllGameMode.h"
#include "Shotgun.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AContraCharacter::AContraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AContraCharacter::OnHit);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 650.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AContraCharacter::BeginPlay()
{
	Super::BeginPlay();
	// setting(spawning) weapon
	AddWeapon();
}

void AContraCharacter::Tick(float DeltaSeconds)
{
	// Logic is checking "Do player change weapon"
	SwapLogic();
	if (Health <= -10)
	{
		CheckPlayerDead();
	}
}

void AContraCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &AContraCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AContraCharacter::MoveForward);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AContraCharacter::IncreaseSpeed);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AContraCharacter::DecreaseSpeed);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AContraCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &AContraCharacter::StopShooting);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AContraCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("Prone"), EInputEvent::IE_Pressed, this, &AContraCharacter::Prone);
	PlayerInputComponent->BindAction(TEXT("Open"), EInputEvent::IE_Pressed, this, &AContraCharacter::Open);
	PlayerInputComponent->BindAction(TEXT("Pause"), EInputEvent::IE_Pressed, this, &AContraCharacter::Pause);
}

void AContraCharacter::MoveForward(float AxisValue)
{
	// player is going forward but if you hold shift, player will go faster.
	// Not: 1.72/4 for the animation.
	if (!IsInProne() && !IsProneFire())
	{
		AddMovementInput(FVector(-1.f, 0.f, 0.f), AxisValue / (bIsShiftPressed ? 1.72 : 4));
	}
}

void AContraCharacter::MoveRight(float Value)
{
	// add movement in that direction
	if (!IsInProne() && !IsProneFire())
	{
		AddMovementInput(FVector(0.f, -1.f, 0.f), Value / (bIsShiftPressed ? 1.72 : 4));
	}
}

void AContraCharacter::IncreaseSpeed()
{
	bIsShiftPressed = true;
}

void AContraCharacter::DecreaseSpeed()
{
	bIsShiftPressed = false;
}

// Calling pulltrigger function and if ammo isn't enough, it will reload (no animation).
void AContraCharacter::Shoot()
{
	// Shotgun
	if (ShotWithShotgun)
	{
		if (!Agun)
		{
			UE_LOG(LogTemp, Warning, TEXT("Shotgun not found!!"));
			return;
		}
		if (Agun->Ammo != 0)
		{
			GetWorldTimerManager().SetTimer(ShootTimer, this, &AContraCharacter::Shoot, Agun->FireRepeat, true,
			                                Agun->FireRate);
			Agun->PullTrigger();
			if (SetProne)
			{
				SetProneFire = true;
				IsProneFire();
			}
			Agun->Ammo--;
		}
		else if (Agun->Ammo == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(ShootTimer);
			LeftMouseHeld = true;
			SetProneFire = false;
			IsProneFire();
			Reload();
		}
	}
		// Normal weapon
	else
	{
		if (Gun->Ammo != 0)
		{
			GetWorldTimerManager().SetTimer(ShootTimer, this, &AContraCharacter::Shoot, Gun->FireRepeat, true,
			                                Gun->FireRate);
			Gun->PullTrigger();
			Gun->Ammo--;
		}
		else if (Gun->Ammo == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(ShootTimer);
			LeftMouseHeld = true;
			Reload();
		}
	}
}

void AContraCharacter::StopShooting()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootTimer);
	LeftMouseHeld = false;
	SetProneFire = false;
	IsProneFire();
}

// refreshing ammo
void AContraCharacter::Reload()
{
	if (ShotWithShotgun)
	{
	//	UE_LOG(LogTemp, Warning, TEXT("Reloading"));
		Agun->Ammo = Agun->DefaultAmmo;
	}
	else
	{
	//	UE_LOG(LogTemp, Warning, TEXT("Reloading"));
		Gun->Ammo = Gun->DefaultAmmo;
	}
	if (LeftMouseHeld == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootTimer);
	}
	else
	{
		LeftMouseHeld = true;
		GetWorld()->GetTimerManager().ClearTimer(ShootTimer);
		GetWorldTimerManager().SetTimer(ShootTimer, this, &AContraCharacter::Shoot, Gun->FireRepeat, true,
		                                Gun->FireRate);
	}
}

void AContraCharacter::Prone()
{
	// Getting player controller.
	if (SetProne == false)
	{
		SetProne = true;
		IsInProne();
	}
	else
	{
		SetProne = false;
		IsInProne();
	}
}

float AContraCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                   AActor* DamageCauser)
{
	// When player was damaged, it will apply damage to health.
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	// Slow Motion ON
	if (Health < 10)
	{
		SlowLogic = true;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimerValue);
		BacktoNormal();
	}
	// if player dead, it will give information to game mode.
	if (IsDead())
	{
		CheckPlayerDead();
	}

	return DamageToApply;
}

void AContraCharacter::CheckPlayerDead()
{
	AReturnoftheGormGameMode* GameMode = GetWorld()->GetAuthGameMode<AReturnoftheGormGameMode>();
	if (GameMode != nullptr)
	{
		GameMode->PawnKilled(this);
	}
	if (Controller != nullptr && Controller->GetPawn() == this)
	{
		Controller->PawnPendingDestroy(this);
		if (Controller != nullptr)
		{
			Controller->UnPossess();
			Controller = nullptr;
		}
	}
	StopShooting();
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AContraCharacter::IsDead()
{
	return Health <= 0;
}

// for the animation
bool AContraCharacter::IsInProne()
{
	return SetProne;
}

bool AContraCharacter::IsProneFire()
{
	return SetProneFire;
}

// it is hiding main skeletal mesh and activating other skeletal mesh.
void AContraCharacter::SwapWeapon()
{
	Agun->SetActorHiddenInGame(true);
	Gun->SetActorHiddenInGame(false);
	ShotWithShotgun = false;
	GetWorldTimerManager().ClearTimer(SwapWeaponTimer);
}

// shotgun is avaible for a limited time.
void AContraCharacter::SwapLogic()
{
	if (IsCanChange)
	{
		Agun->SetActorHiddenInGame(false);
		Gun->SetActorHiddenInGame(true);
		GetWorldTimerManager().SetTimer(SwapWeaponTimer, this, &AContraCharacter::SwapWeapon, 1, false, WeaponSwapTime);
		ShotWithShotgun = true;
		IsCanChange = false;
	}
}

// It is adding gun on socket.
void AContraCharacter::AddWeapon()
{
	// Spawn the gun and hide character default rifle.
	// Add new socket for weapon and attach to gun actor.
	Gun = GetWorld()->SpawnActor<AGun>(NormalGunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("WeaponSocket")
	);
	Gun->SetOwner(this);
	Health = MaxHealth;
	Gun->Ammo = Gun->DefaultAmmo;
	// for shotgun
	Agun = GetWorld()->SpawnActor<AShotgun>(ShotgunClass);
	Agun->SetActorHiddenInGame(true);
	Agun->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("WeaponSocket")
	);
	Agun->SetOwner(this);
	Agun->Ammo = Agun->DefaultAmmo;
}

// When it enemy to you
void AContraCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                             FVector NormalImpulse, const FHitResult& Hit)
{
	AContraCharacter* ContraCharacter = Cast<AContraCharacter>(OtherActor);
	if (!ContraCharacter) { return; }
	if (ContraCharacter && OtherActor != this && OtherActor->ActorHasTag("Enemy"))
	{
		// when hit the player, It will destroy other player (contra character)
		AReturnoftheGormGameMode* GameMode = GetWorld()->GetAuthGameMode<AReturnoftheGormGameMode>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		Destroy();
	}
}

// Open Door
void AContraCharacter::Open()
{
	CanOpen = !CanOpen;
}

// Slow Time
void AContraCharacter::BacktoNormal()
{
	// setting timer slow motion
	GetWorldTimerManager().SetTimer(BacktoNormalTimer, this, &AContraCharacter::BacktoNormal, 2, true, 0.1f);
	if (SlowLogic)
	{
		// when player was dead and slow time is enough, it will back to the normal and destroy himself.
		if (TimerValue >= 0.5f && Health <= 0)
		{
			TimerValue = 1.0f;
			UGameplayStatics::SetGlobalTimeDilation(this, TimerValue);
			GetWorldTimerManager().ClearTimer(BacktoNormalTimer);
			Destroy();
			Gun->Destroy();
		}
		else
		{
			// if player doesn't dead and slow motion enough, slow time returning normal but slowly.
			TimerValue += 0.1;
			UGameplayStatics::SetGlobalTimeDilation(this, TimerValue);
		}
	}
}

// it's calling to pause screen.
void AContraCharacter::Pause()
{
	KillEmAllGameMode = GetWorld()->GetAuthGameMode<AKillEmAllGameMode>();
	if (KillEmAllGameMode)
	{
		KillEmAllGameMode->CallPause();
	}
}
