// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	HealthMax = 100;
	Health = HealthMax;
	// ...
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
	
}

bool USAttributeComponent::IsAlive() const
{
	return Health>0.f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
	return  HealthMax;
}

bool USAttributeComponent::IsLowHealth()
{
	return 30.f>=Health;
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigateActor, float Delta)
{
	float OldHealth = Health;

	Health = FMath::Clamp(Health +Delta,0.f,HealthMax);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigateActor,this,Health,ActualDelta);


	return ActualDelta !=0;
}


// USAttributeComponent*  USAttributeComponent::GetAttributes(APawn* Pawn)  
// {
// 	USAttributeComponent* attr =  Cast<USAttributeComponent>(Pawn->GetComponentByClass(USAttributeComponent::StaticClass()));
// 	if(attr)
// 		return attr;
// 	return nullptr;
// }




