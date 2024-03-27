// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleInteractionCharacterComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CATSWAY_API USimpleInteractionCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleInteractionCharacterComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	float TraceSphereRadius = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	float TraceDistance = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	float TraceDistanceOffset = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	FVector TraceStartOffset = FVector(0.f, 0.f, 20.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	FVector TraceEndOffset = FVector(0.f, 0.f, 50.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Interaction")
	bool bDebug = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Store the interactable object that is currently being focused
	AActor* FocusedInteractableActor = nullptr;

	void FocusInteractableActor(AActor* Interactable);
	void UnfocusCurrentInteractableActor();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Simple Interaction")
	void TraceAndFocusOnInteractableActor();
	
	UFUNCTION(BlueprintCallable, Category = "Simple Interaction")
	void InteractWithFocusedActor();
	
	UFUNCTION(BlueprintCallable, Category = "Simple Interaction")
	AActor* GetFocusedInteractableActor() const { return FocusedInteractableActor; }
};
