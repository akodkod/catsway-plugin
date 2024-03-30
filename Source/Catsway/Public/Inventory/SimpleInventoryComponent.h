// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "SimpleInventoryComponent.generated.h"

// Create a struct for item data table
USTRUCT(BlueprintType)
struct FInventoryItemDataTable : public FTableRowBase {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString Description;
};

// Create a struct for inventory item
USTRUCT(BlueprintType)
struct FInventoryItem {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName RowName;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CATSWAY_API USimpleInventoryComponent : public UActorComponent {
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  USimpleInventoryComponent();

  // Inventory items
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Simple Inventory")
  TArray<FInventoryItem> Items;

  // Items data table
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simple Inventory")
  UDataTable* ItemsDataTable;

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  // Add item to inventory
  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  void AddItem(FName RowName);

  // Remove item from inventory
  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  void RemoveItem(FName RowName);
  
  // Check if item is in inventory
  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  bool HasItem(FName RowName);

  // Item exists in data table
  UFUNCTION(BlueprintCallable, Category = "Simple Inventory")
  bool ItemExistsInDataTable(FName RowName);
};
