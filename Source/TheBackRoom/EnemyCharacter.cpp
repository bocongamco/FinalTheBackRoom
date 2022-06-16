// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,5,0);


	DmgCollide = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DmgCollide->SetupAttachment(RootComponent);
	DmgCollide->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OverlapWithPlayerActor);

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::OverlapWithPlayerActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATheBackRoomCharacter* Char = Cast<ATheBackRoomCharacter>(OtherActor);
	if (Char ) {
		UE_LOG(LogTemp, Warning, TEXT("Colliding.."));
		//Char->DealDamage(isCollide);
		Char->playerDead = true;

	}
}

