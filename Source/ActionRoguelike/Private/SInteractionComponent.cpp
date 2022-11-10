// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"),false,TEXT("Enable Debug Lines for Interact Component."),ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius =30.f;
	TraceDistance =500.f;
	CollisionChannel = ECC_WorldDynamic;
	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
	// ...
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();
	FVector Start;

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
	FVector End = EyeLocation +(EyeRotation.Vector()*TraceDistance);
	
	TArray<FHitResult> Hits;


	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End,FQuat::Identity,ObjectQueryParams,Shape);

	FColor LineColor = bBlockingHit?FColor::Green:FColor::Red;

	//clear ref before trying to fill.
	FocuseActor = nullptr;
	for (FHitResult Hit:Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(),Hit.ImpactPoint,TraceRadius,32,LineColor,false,2.0f);
		}
		AActor* HitActor = Hit.GetActor();
		if(HitActor->Implements<USGameplayInterface>())
		{
			FocuseActor = HitActor;
			break;
		}

	}

	if (FocuseActor)
	{
		if(DefaultWidgetInstance ==nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance =CreateWidget<USWorldUserWidget>(GetWorld(),DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocuseActor;
			if(!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(),EyeLocation,End,LineColor,false,2.0f,0,2.0f);
	}
}

void USInteractionComponent::PrimaryInteract()
{
	if (FocuseActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,"No FocuseActor to interact.");
		return;
	}
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocuseActor,MyPawn);
}


