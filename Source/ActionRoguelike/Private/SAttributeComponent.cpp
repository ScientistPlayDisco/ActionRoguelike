// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"),1.f,TEXT("Global Damage Modifier fir Attribute Component."),ECVF_Cheat);


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	HealthMax = 100;
	Health = HealthMax;
	RageMax= 100;
	Rage = 0;
	// ...
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigateActor, float Delta)
{
	//???
	if(!GetOwner()->CanBeDamaged() &&Delta<0.f)
	{
		return  false;
	}
		
	if(Delta<0.f)
	{
		float DamageMutiplier =	CVarDamageMultiplier.GetValueOnGameThread();  

		Delta *= DamageMutiplier;
	}
	
	float OldHealth = Health;

	Health = FMath::Clamp(Health +Delta,0.f,HealthMax);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigateActor,this,Health,ActualDelta);

	if(ActualDelta<0.f&& Health ==0.f)
	{
		ASGameModeBase* GM = Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode<ASGameModeBase>());
		if(GM)
		{
			 GM->OnActorKilled(GetOwner(),InstigateActor);
		}
	}

	return ActualDelta !=0;
}

bool USAttributeComponent::IsFullRage() const
{
	return RageMax == Rage;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigateActor, float Delta)
{
	if(IsFullRage())
	{
		return false;
	}
	float OldRage = Rage;
	Rage = FMath::Clamp(Rage +Delta,0.f,RageMax);
	float ActualDelta = Rage-OldRage;
	OnRageChanged.Broadcast(InstigateActor,this,Rage,ActualDelta);

	return true;
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

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return	ApplyHealthChange(InstigatorActor,-GetHealthMax());
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






