// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"
#include "Kismet/GameplayStatics.h"


ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	// MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// // Disable collision, instead we use SphereComp to handle interaction queries
	// MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// MeshComp->SetupAttachment(RootComponent);

	CreditCost = 20;
}


void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) &&AttributeComp->ApplyHealthChange(InstigatorPawn,AttributeComp->GetHealthMax()))
			{
				// Only activate if healed successfully
				HideAndCooldownPowerup();
			}
		}
	}
}

