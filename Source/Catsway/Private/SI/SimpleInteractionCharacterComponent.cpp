// Fill out your copyright notice in the Description page of Project Settings.


#include "SI/SimpleInteractionCharacterComponent.h"

#include "SI/SimpleInteraction.h"
#include "Camera/CameraComponent.h"


// Sets default values for this component's properties
USimpleInteractionCharacterComponent::USimpleInteractionCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimpleInteractionCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USimpleInteractionCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USimpleInteractionCharacterComponent::TraceAndFocusOnInteractableActor()
{
	APawn* OwningPawn = GetOwner<APawn>();
	if (!OwningPawn) return;

	UCameraComponent* Camera = OwningPawn->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	FVector CameraForwardVector = Camera->GetForwardVector();
	FVector Start = Camera->GetComponentLocation() + (CameraForwardVector * TraceDistanceOffset) + TraceStartOffset;
	FVector End = Start + (CameraForwardVector * TraceDistance) + TraceEndOffset;
	
	// Draw debug line
	if (bDebug) DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f, 0, 0.6);

	// Trace using a sphere
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwningPawn);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(TraceSphereRadius),
		CollisionParams
	);

	if (bHit)
	{
		// Draw debug sphere
		if (bDebug) DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceSphereRadius, 8, FColor::Green, false, 1.f, 0, 0.6);
		
		AActor* HitActor = HitResult.GetActor();
		
		if (HitActor && HitActor->Implements<USimpleInteraction>() && ISimpleInteraction::Execute_CanPlayerInteract(HitActor))
		{
			FocusInteractableActor(HitActor);
		}
		else
		{
			// TODO: Keep current interactable actor if distance is less than a certain threshold
			UnfocusCurrentInteractableActor();
		}
	}
	else
	{
		// Draw debug sphere
		if (bDebug) DrawDebugSphere(GetWorld(), End, TraceSphereRadius, 8, FColor::Red, false, 1.f, 0, 0.1);
		
		UnfocusCurrentInteractableActor();
	}
}

void USimpleInteractionCharacterComponent::FocusInteractableActor(AActor* Interactable)
{
	if (FocusedInteractableActor == Interactable) return;
	if (FocusedInteractableActor) UnfocusCurrentInteractableActor();

	FocusedInteractableActor = Interactable;
	ISimpleInteraction::Execute_PlayerFocuses(FocusedInteractableActor);
}

void USimpleInteractionCharacterComponent::UnfocusCurrentInteractableActor()
{
	if (!FocusedInteractableActor) return;

	ISimpleInteraction::Execute_PlayerUnfocuses(FocusedInteractableActor);
	FocusedInteractableActor = nullptr;
}

void USimpleInteractionCharacterComponent::InteractWithFocusedActor()
{
	if (!FocusedInteractableActor) return;

	ISimpleInteraction::Execute_PlayerInteracts(FocusedInteractableActor);
}


