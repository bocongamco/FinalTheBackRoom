// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasingGhost.h"
#include "TheBackRoomCharacter.h"
#include "Components/BoxComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AChasingGhost::AChasingGhost()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sense Config Sigh "));

	SightConfig->SightRadius = 5000.0f;
	SightConfig->LoseSightRadius = 6000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(0.1f); // time reqquire to forget about the event.

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AChasingGhost::OnSensed);

	CurrentVelocity = FVector::ZeroVector;
	MovementSpeed = 500.0f;
;

	DistanceSquared = BIG_NUMBER;
	
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AChasingGhost::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AChasingGhost::BeginPlay()
{
	Super::BeginPlay();

	
	BaseLocation = this->GetActorLocation();


}

void AChasingGhost::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) 
{
	
	
	ATheBackRoomCharacter* Char = Cast<ATheBackRoomCharacter>(OtherActor);

	if(linksPad){
		if (Char && linksPad->getOverlap()) {
			UE_LOG(LogTemp, Warning, TEXT("Colliding.."));
			isCollide = true;
			//Char->DealDamage(isCollide);
			Char->playerDead = true;
	
		}
	}


}


// Called every frame
void AChasingGhost::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	if (!CurrentVelocity.IsZero()) {
		NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
		if (BackToBaseLocation) {
			if ((NewLocation - BaseLocation).SizeSquared2D() < DistanceSquared) {
				DistanceSquared = (NewLocation - BaseLocation).SizeSquared2D();
			}
			else {
				CurrentVelocity = FVector::ZeroVector;
				DistanceSquared = BIG_NUMBER;
				BackToBaseLocation = false;

				SetNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}
		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void AChasingGhost::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChasingGhost::OnSensed(const TArray<AActor*>& UpdatedActors)
{		


	for (int i = 0; i < UpdatedActors.Num(); i++) {
		FActorPerceptionBlueprintInfo Info;
		AIPerComp->GetActorsPerception(UpdatedActors[i], Info);

		if (linksPad) {
			
			if (Info.LastSensedStimuli[0].WasSuccessfullySensed() && linksPad->getOverlap()) {
				//UE_LOG(LogTemp, Warning, TEXT("Sense true"));

				FVector direction = UpdatedActors[i]->GetActorLocation() - GetActorLocation();
				direction.Z = 0.0f;

				CurrentVelocity = direction.GetSafeNormal() * MovementSpeed;

				SetNewRotation(UpdatedActors[i]->GetActorLocation(), GetActorLocation());

			}
			else {
				FVector direction = BaseLocation - GetActorLocation();
				direction.Z = 0.0f;
				if (direction.SizeSquared2D() > 1.0f) {
					CurrentVelocity = direction.GetSafeNormal() * MovementSpeed;
					BackToBaseLocation = true;
					SetNewRotation(BaseLocation, GetActorLocation());
				}

			}
			
		}
		/*else {
			FVector direction = BaseLocation - GetActorLocation();
			direction.Z = 0.0f;
			if (direction.SizeSquared2D() > 1.0f) {
				CurrentVelocity = direction.GetSafeNormal() * MovementSpeed;
				BackToBaseLocation = true;
				SetNewRotation(BaseLocation, GetActorLocation());
			}
		}*/

	}
}

void AChasingGhost::SetNewRotation(FVector TargetPosition, FVector CurrentPosition)
{

	FVector NewDirection = TargetPosition - CurrentPosition;
	NewDirection.Z = 0.0f;


	EnemyRotation = NewDirection.Rotation();

	SetActorRotation(EnemyRotation);
}

