// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include <string>

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnScenceComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	
}
void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta<0.f)
	{

		if(InstigatorActor!=this)
		{
			SetTarget(InstigatorActor);
		}
		//spawn health bar
		if(ActiveHealtheBar ==nullptr)
		{
			ActiveHealtheBar = CreateWidget<USWorldUserWidget>(GetWorld(),HealthBarWidgetClass);
			if(ActiveHealtheBar)
			{

				ActiveHealtheBar->AttachedActor = this;
				ActiveHealtheBar->AddToViewport();
			}
		}

		//set hit mesh
		
		// if(NewHealth<=40)
		if(NewHealth<=0.f)
		{
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			//	死亡后碰撞消失  禁用运动
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			
			SetLifeSpan(10.f);
			// DrawDebugString(GetWorld(),GetActorLocation(),"hit",nullptr,FColor::White,4.f,true);
		}

		

	}

}
void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this,&ASAICharacter::OnHealthChanged);

}


void ASAICharacter::SetTarget(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetACtor",NewTarget);

	}
	
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTarget(Pawn);
	DrawDebugString(GetWorld(),GetActorLocation(),"PLAYER SPOTTED",nullptr,FColor::White,4.f,true);
}

