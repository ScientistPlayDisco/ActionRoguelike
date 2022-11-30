// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAICharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	// Called when the game starts or when spawned

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Actions")
	USActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attribute")
	USAttributeComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="UI")
	USWorldUserWidget* NewWidget;

	UFUNCTION(NetMulticast,Unreliable)
	void MulticastPawnSeen();
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void SetTarget(AActor* NewTarget);

	UFUNCTION()
	AActor* GetTargetActor() const;

};

