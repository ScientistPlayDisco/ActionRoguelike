// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"

#include <Actor.h>

#include "AIController.h"
#include "AI/SAICharacter.h"
#include "AVEncoder/Public/Microsoft/AVEncoderIMFSampleWrapper.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ensure(AIPawn))
	{
		USAttributeComponent* AttributeComp =Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if(AttributeComp)
		{
			UBlackboardComponent* BlackboardComp =  OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsBool(HealthKey.SelectedKeyName,AttributeComp->IsLowHealth());
		}
	}
}
