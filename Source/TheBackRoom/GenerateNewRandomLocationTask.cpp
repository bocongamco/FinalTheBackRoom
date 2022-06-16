// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateNewRandomLocationTask.h"

EBTNodeResult::Type UGenerateNewRandomLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;
	if (!Component)
		return EBTNodeResult::Failed;
	AEnemyAiController* MyController = Cast<AEnemyAiController>(Component->GetOwner());
	if (!MyController)
		return EBTNodeResult::Failed;
	MyController->GenerateNewRandomLocation();
	return EBTNodeResult::Succeeded;
}