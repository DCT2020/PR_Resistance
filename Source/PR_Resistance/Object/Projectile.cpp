// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Engine/StaticMesh.h"
#include "Engine/public/DrawDebugHelpers.h"
#include "PR_Resistance/Chara/PR_ResistanceCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile_mesh"));
	mMesh->SetupAttachment(RootComponent);

	mCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	mCollider->SetupAttachment(mMesh);

	mCollider->BodyInstance.SetCollisionProfileName("Projectile");
}

void AProjectile::Init(FVector direction, float damage, float speed, float lifeTime)
{
	mDamage			= damage;
	mSpeed			= speed;
	mDirection		= direction;
	mLifeTime		= lifeTime;
	
	mMoveDistance	= mDirection * mSpeed;
	mMesh->SetWorldRotation(direction.Rotation());
}

void AProjectile::UpdatePosition(float DeltaTime)
{
	auto curLocation = GetActorLocation();
	curLocation += mMoveDistance * DeltaTime;
	SetActorLocation(curLocation);
}

void AProjectile::OnProjectileBeginOverlap_inheritance(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Hit"));
	UGameplayStatics::ApplyDamage(OtherActor, 10.0f, NULL, this, NULL);

	Destroy();
}

void AProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag(TEXT("Player")))
	{
		OnProjectileBeginOverlap_inheritance(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	mStartPosition = GetActorLocation();

	// 반드시 begin play에서 해주어야함
	{
		mCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileBeginOverlap);
		mCollider->SetGenerateOverlapEvents(true);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mElapsedTime < mLifeTime)
	{
		mElapsedTime += DeltaTime;
		UpdatePosition(DeltaTime);
	}
	else
	{
		mElapsedTime = 0.0f;
		// TODO: 서버 코드로 가야 할 것
		Destroy();
	}

#if WITH_EDITOR
	if (bIsDebug)
	{
		DrawDebugLine(GetWorld(), mStartPosition, mStartPosition + mMoveDistance * mLifeTime, FColor::Green);
	}
#endif
}

