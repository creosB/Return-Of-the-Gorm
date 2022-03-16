// Fill out your copyright notice in the Description page of Project Settings.


#include "HoverMini.h"

#include "ReturnoftheGormGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHoverMini::AHoverMini()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HoverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hovermini Mesh"));
	HoverMesh->SetWorldScale3D(FVector(0.5f, 0.75f, 0.75f));
	HoverMesh->SetSimulatePhysics(true);
	HoverMesh->SetNotifyRigidBodyCollision(true);
	HoverMesh->BodyInstance.bLockXRotation = true;
	HoverMesh->BodyInstance.bLockYRotation = true;
	HoverMesh->BodyInstance.bLockZRotation = true;
	HoverMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	HoverMesh->OnComponentHit.AddDynamic(this, &AHoverMini::OnHit);
	RootComponent = HoverMesh;
}

// Called when the game starts or when spawned
void AHoverMini::BeginPlay()
{
	Super::BeginPlay();

	// Call main player
	Player = Cast<AContraCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player) { return; }
	// Set movement values
	SetMovementValue();
	// Set health
	Health = MaxHealth;
}

// Called every frame
void AHoverMini::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FirstLogic)
	{
		FirstMovement(DeltaTime);
	}

	if (SecondLogic)
	{
		SecondMovement(DeltaTime);
	}
	if (ThirdLogic)
	{
		ThirdMovement(DeltaTime);
	}
}

// Movement System
void AHoverMini::SetMovementValue()
{
	/* it will create right side but need to update.
	else
	{
		FirstTarget = GetActorLocation() - FVector(0, -FirstTargetValue, 0);
		SecondTarget = FirstTarget - FVector(0.0f, 0.0f, SecondTargetValue);
		ThirdTarget = SecondTarget - FVector(0.0f, -ThirdTargetValue, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("BBB"),*ThirdTarget.ToString());
	}
	*/
	// Set first movement value
	FirstInitial = GetActorLocation();
	FirstCurrent = FirstInitial;
	FirstCurrent = Player->GetActorLocation();
	FirstTarget = GetActorLocation() - FVector(0, FirstTargetValue, 0);
	FirstMovementHeight = GetActorLocation().Z;

	// Set second movement value
	SecondInitial = FirstTarget;
	SecondCurrent = SecondInitial;
	SecondTarget = FirstTarget - FVector(0.0f, 0.0f, SecondTargetValue);

	// Set third movement value
	ThirdInitial = SecondTarget;
	ThirdCurrent = ThirdInitial;
	ThirdTarget = SecondTarget + FVector(0.0f, ThirdTargetValue, 0.0f);

	Forward = GetActorLocation();
}

void AHoverMini::FirstMovement(float DeltaTime)
{
	HoverMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	FirstCurrent = FMath::VInterpConstantTo(FirstCurrent, FirstTarget, DeltaTime,
	                                        FirstMovementSpeed);
	Forward = FirstCurrent;
	SetActorLocation(FVector(Forward.X, Forward.Y, FirstMovementHeight));
	if (Forward == FirstTarget)
	{
		FirstLogic = false;
		SecondLogic = true;
	}
}

void AHoverMini::SecondMovement(float DeltaTime)
{
	HoverMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	SecondCurrent = FMath::VInterpConstantTo(SecondCurrent, SecondTarget, DeltaTime,
	                                         SecondMovementSpeed);
	Forward = SecondCurrent;
	SetActorLocation(Forward);

	if (Forward == SecondTarget)
	{
		SecondLogic = false;
		ThirdLogic = true;
	}
}

void AHoverMini::ThirdMovement(float DeltaTime)
{
	ThirdCurrent = FMath::VInterpConstantTo(ThirdCurrent, ThirdTarget, DeltaTime,
	                                        FirstMovementSpeed);
	Forward = ThirdCurrent;
	SetActorLocation(Forward);

	if (Forward == ThirdTarget)
	{
		Destroy();
	}
}

// Health - Dead system
bool AHoverMini::IsDead() const
{
	return Health <= 0;
}

float AHoverMini::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser)
{
	// pawn taking damage and applying health.
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);

	Health -= DamageToApply;
	// when health was less than 10, it will set start slow motion value and call function.
	if (Health < 10)
	{
		SlowLogic = true;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimerValue);
		BacktoNormal();
	}
	// when player was dead, it will spawn particle and sound.
	if (IsDead())
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
		// pawn is giving information to gamemode.
		AReturnoftheGormGameMode* GameMode = GetWorld()->GetAuthGameMode<AReturnoftheGormGameMode>();
		if (GameMode != nullptr && ReturnGameModeDead)
		{
			GameMode->PawnKilled(this);
			// returning game mode one! It's protecting sum more score.
			ReturnGameModeDead = false;
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

		//Destroy();
		//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageToApply;
}

void AHoverMini::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                       FVector NormalImpulse, const FHitResult& Hit)
{
	AContraCharacter* ContraCharacter = Cast<AContraCharacter>(OtherActor);
	if (!ContraCharacter) { return; }

	if (ContraCharacter && OtherActor != this)
	{
		ContraCharacter->Health -= DamageValue;
	}
}


// Slow Motion
void AHoverMini::BacktoNormal()
{
	// setting timer slow motion
	GetWorldTimerManager().SetTimer(BacktoNormalTimer, this, &AHoverMini::BacktoNormal, 2, true, 0.1f);
	if (SlowLogic)
	{
		// when player was dead and slow time is enough, it will back to the normal and destroy himself.
		if (TimerValue >= 0.5f && Health <= 0)
		{
			TimerValue = 1.0f;
			UGameplayStatics::SetGlobalTimeDilation(this, TimerValue);
			GetWorldTimerManager().ClearTimer(BacktoNormalTimer);
			Destroy();
		}
		else
		{
			// if player doesn't dead and slow motion enough, slow time returning normal but slowly.
			TimerValue += 0.1;
			UGameplayStatics::SetGlobalTimeDilation(this, TimerValue);
		}
	}
}
