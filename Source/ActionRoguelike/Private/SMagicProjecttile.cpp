// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjecttile.h"

#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);

	DamageAmount = 30.f;

	SetReplicates(true);
}


void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                       UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(Actor&& Actor !=GetInstigator())
	{
		// USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Actor->GetComponentByClass(USAttributeComponent::StaticClass()));
		// if(AttributeComp)
		// {
		// 	AttributeComp->ApplyHealthChange(GetInstigator(),-DamageAmount);
		// 	Explode();
		// }
		USActionComponent* ActionComp = Cast<USActionComponent>(Actor->GetComponentByClass(USActionComponent::StaticClass()));
		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),Actor,DamageAmount,HitResult))
		{
			Explode();

			//确权
			if(ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(),BurningActionClass);
			}
		}
	}
}



// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



