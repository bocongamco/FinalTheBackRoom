// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAiController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyAiController::AEnemyAiController() {
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	
	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);
	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAiController::OnPawnDetected);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAiController::OnSensesUpdated);


	TargetPlayer = nullptr;
}

void AEnemyAiController::BeginPlay()
{
	Super::BeginPlay();
	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (!AIBlackboard)
		return;
	if (!ensure(BehaviorTree))
		return;
	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);

}

void AEnemyAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void AEnemyAiController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
}

FRotator AEnemyAiController::GetControlRotation() const
{
	if (GetPawn()){
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}
	return FRotator::ZeroRotator;

}

void AEnemyAiController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	/*if (NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(),
			2000, ReturnLocation);
		MoveToLocation(ReturnLocation.Location);
	}*/
}

//void AEnemyAiController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
//{
//	for (auto TempActor : DetectedPawns)
//	{
//		APawn* TempPawn = Cast<APawn>(TempActor);
//		if (TempPawn)
//		{
//			if (TargetPlayer && TargetPlayer == TempPawn)
//			{
//				UE_LOG(LogTemp, Warning, TEXT("Player is known and is now leaving target space"));
//				TargetPlayer = nullptr;
//			}
//			else
//			{
//				UE_LOG(LogTemp, Warning, TEXT("Player has been spotted. Saving reference"));
//				TargetPlayer = TempPawn;
//			}
//		}
//	}
//}

void AEnemyAiController::GenerateNewRandomLocation()
{
	if (NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolLocation", ReturnLocation.Location);
	}

}

void AEnemyAiController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	APawn* TemporaryPawn = Cast<APawn>(UpdatedActor);
	if (TemporaryPawn && TemporaryPawn->IsPlayerControlled())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Actor Location"));
			TargetPlayer = TemporaryPawn;
			BlackboardComponent->SetValueAsBool("ChasePlayer", true);
			BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
		}
		else
		{
			TargetPlayer = nullptr;
			BlackboardComponent->ClearValue("ChasePlayer");
		}
	}
}
