// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "TheBackRoomCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAiController.generated.h"

/**
 * 
 */
UCLASS()
class THEBACKROOM_API AEnemyAiController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAiController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	


	void GenerateNewRandomLocation();

	UFUNCTION()
	void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= AI)
		float SightRadius = 50000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= AI)
		float SightAge = 3.5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= AI)
		float LoseSightRadius = SightRadius + 1000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= AI)
		float FieldOfView = 45;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= AI)
		UAISenseConfig_Sight* SightConfiguration;
	UNavigationSystemV1* NavigationSystem;
	APawn* TargetPlayer;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardData* AIBlackboard;
	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBehaviorTree* BehaviorTree;
	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardComponent* BlackboardComponent;

	
};
