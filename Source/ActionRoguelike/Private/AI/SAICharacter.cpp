// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include <string>
#include <wrl/event.h>

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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

		//被攻击时候，设置instigator为新的target
		if(InstigatorActor!=this)
		{
			SetTarget(InstigatorActor);
		}
		//如果instigator是 character的话，
		USAttributeComponent* Comp = USAttributeComponent::GetAttributes(InstigatorActor);
		if(Comp)
		{
			Comp->ApplyRageChange(this,-Delta/5);
		}
		
		//spawn health bar
		if(ActiveHealthBar ==nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(),HealthBarWidgetClass);
			if(ActiveHealthBar)
			{

				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport(10);
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

AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject("TargetACtor"));
		 
	}
	return nullptr;
}


void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	if(GetTargetActor() != Pawn)
	{
		SetTarget(Pawn);

		
		MulticastPawnSeen();
	}
}

void ASAICharacter::MulticastPawnSeen_Implementation()
{
	if (!ensure(SpottedWidgetClass))
	{
		return;
	}
	NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(),SpottedWidgetClass);
	if(NewWidget)
	{
		NewWidget->AttachedActor =this;
		NewWidget->AddToViewport(10);
		FTimerHandle FTimerHandle_Destroy;
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_Destroy,[&]()
		{
			this->NewWidget->RemoveFromParent();
			GetWorld()->GetTimerManager().ClearTimer(FTimerHandle_Destroy);

		},3,false);
	}
}
