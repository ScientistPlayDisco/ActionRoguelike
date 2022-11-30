// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "SMagicProjecttile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);//Character的根组件RootComponent·
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	bUseControllerRotationYaw= false;


	TimeToHitParamName = "TimeToHit";
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//绑定OnHealthChanged

	AttributeComp->OnHealthChanged.AddDynamic(this,&ASCharacter::OnHealthChanged);
}


void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(),value);
	// AddMovementInput(GetActorForwardVector(),value);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;


	FVector RightVector= FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector,value);
	
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this,"PrimaryAttack");
}
void ASCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this,"BlackHoleAttack");
}
void ASCharacter::DashAttack()
{
	ActionComp->StartActionByName(this,"DashAttack");
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this,"Sprint");
}

void ASCharacter::SpringStop()
{
	ActionComp->StopActionByName(this,"Sprint");

}

void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);


	PlayerInputComponent->BindAction("PrimaryAttack",IE_Pressed,this,&ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract",IE_Pressed,this,&ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("BlackHoleAttack",IE_Pressed,this,&ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("DashAttack",IE_Pressed,this,&ASCharacter::DashAttack);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&ASCharacter::SpringStop);

}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ASCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this,Amount);
}


void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{

	if(Delta<0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,GetWorld()->TimeSeconds);

		//
		OwningComp->ApplyRageChange(InstigatorActor,-Delta/5);
	}
	if(NewHealth<=0.f&&Delta<0.f)
	{
		APlayerController* PC =Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.f);
	}
	
}

