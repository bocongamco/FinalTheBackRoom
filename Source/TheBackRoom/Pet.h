// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TheBackRoomCharacter.h"
#include "Pet.generated.h"

UCLASS()
class THEBACKROOM_API APet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Material")
	UStaticMeshComponent* BottomMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Material")
	UStaticMeshComponent* TopMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* material;
	
	ATheBackRoomCharacter* player;
	UMaterialInstanceDynamic* matInstance;


};
