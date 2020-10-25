// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Projectile.generated.h"

UCLASS(Blueprintable)
class PR_RESISTANCE_API AProjectile : public AActor
{
	GENERATED_BODY()
protected:
	// properties
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		float mSpeed = 100.0f;
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		float mLifeTime = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		float mElapsedTime = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		FVector mDirection;
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		float mDamage = 1.0f;

	// mDirection * mSpeed
	UPROPERTY(VisibleAnywhere)
	FVector mMoveDistance;

	FVector mStartPosition;
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = Debug, meta = (AllowPrivateAccess = "true"))
	bool bIsDebug;
#endif

public:
	UPROPERTY(Category = Projectile, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mMesh;
	UPROPERTY(Category = Projectile, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* mCollider;
	UPROPERTY(Category = Projectile, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", DisplayNAame = "Hit Effect"))
        UParticleSystem* mHitEffect;
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION(BlueprintCallable)
	void Init(FVector direction, float damage, float speed, float lifeTime);
	
	
	virtual void UpdatePosition(float DeltaTime);
	virtual void OnProjectileBeginOverlap_inheritance(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
