// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "SimpleInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class CATSWAY_API USimpleInventoryItem : public UDataAsset {
  GENERATED_BODY()

public:

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simple Inventory")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simple Inventory")
  FString Description;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simple Inventory", Meta = (InlineEditConditionToggle))
  bool bStackable = false;

  // Set value to 0 to make it infinite
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simple Inventory", Meta = (EditCondition = "bStackable"))
  int32 MaxStackSize = 1;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simple Inventory")
  FGameplayTag Tag;
  
  // Implement == operator for this class
  bool operator==(const USimpleInventoryItem& Other) const {
    return this == &Other;
  }
};
