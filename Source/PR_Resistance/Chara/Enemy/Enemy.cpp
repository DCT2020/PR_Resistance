// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Enemy States
#include "PR_Resistance/StatesSystem/EnemyState/EnmyState_Find.h"



// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Init Components
	{
	//////////////////////////////////////// Service Components //////////////////////////////////////////////

		//floats component
		mFloatsComponent = CreateDefaultSubobject<UFloatsComponent>(TEXT("Floats"));

		//state component
		mStateManager = CreateDefaultSubobject<UStateManager>(TEXT("FSM"));
		mStateManager->Init();

	//////////////////////////////////////// Locationable Components //////////////////////////////////////////////

		//RootComponent
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

		//player detector
		mDetecter = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyDetector"));
		mDetecter->SetCollisionProfileName(TEXT("PlayerDetector"));
		mDetecter->SetupAttachment(RootComponent);
	}
#pragma  endregion
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//////////////////////////////////////// Init States //////////////////////////////////////////////
	mStateManager->AddStateData_uint8(0, EEnemyState::ES_FIND, UEnmyState_Find);
	mStateManager->SetDefaultState_uint8(0, EEnemyState::ES_FIND);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::ListenFloat(int index, float newFloat)
{
}

