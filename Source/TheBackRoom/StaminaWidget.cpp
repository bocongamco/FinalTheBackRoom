// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaWidget.h"

void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<ATheBackRoomCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}


void UStaminaWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Player) {
		StaminaBar->SetPercent(Player->currentStamina / 100);

	}

}
