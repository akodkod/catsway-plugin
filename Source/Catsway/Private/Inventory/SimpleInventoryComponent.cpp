// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/SimpleInventoryComponent.h"

// Sets default values for this component's properties
USimpleInventoryComponent::USimpleInventoryComponent() {
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = false;

  // ...
}

// Called when the game starts
void USimpleInventoryComponent::BeginPlay() {
  Super::BeginPlay();

  // ...
  
}

// Called every frame
void USimpleInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

void USimpleInventoryComponent::AddItem(USimpleInventoryItem* Item) {
  FInventoryItem NewInventoryItem;
  NewInventoryItem.Item = Item;
  NewInventoryItem.Quantity = 1;
  Items.Add(NewInventoryItem);
}

void USimpleInventoryComponent::AddItemQuantity(USimpleInventoryItem* Item, int32 Quantity) {
  if (Item == nullptr || Quantity <= 0) {
    throw std::invalid_argument("Item is null or quantity is less than or equal to 0");
  }
  
  if (!Item->bStackable && Quantity > 1) {
    throw std::invalid_argument("Item is not stackable");
  }

  int32 RemainingQuantity = Quantity;

  while (RemainingQuantity > 0) {
    for (FInventoryItem& InventoryItem: Items) {
      if (InventoryItem.Item == Item) {
        const int32 QuantityToAdd = Item->MaxStackSize == 0
          ? RemainingQuantity
          : FMath::Min(
            RemainingQuantity,
            Item->MaxStackSize - InventoryItem.Quantity
          );
        
        InventoryItem.Quantity += QuantityToAdd;
        RemainingQuantity -= QuantityToAdd;
      }
    }

    if (RemainingQuantity > 0) {
      FInventoryItem NewInventoryItem;
      NewInventoryItem.Item = Item;
      NewInventoryItem.Quantity = FMath::Min(RemainingQuantity, Item->MaxStackSize);
      Items.Add(NewInventoryItem);
      
      RemainingQuantity -= NewInventoryItem.Quantity;
    }
  }
}

void USimpleInventoryComponent::RemoveItem(USimpleInventoryItem* Item) {
  if (Item == nullptr) {
    throw std::invalid_argument("Item is null");
  }

  for (auto i = 0; i < Items.Num(); ++i) {
    if (Items[i].Item == Item) {
      Items.RemoveAt(i);
      return;
    }
  }
}

void USimpleInventoryComponent::RemoveItemQuantity(USimpleInventoryItem* Item, int32 Quantity) {
  if (Item == nullptr || Quantity <= 0) {
    throw std::invalid_argument("Item is null or quantity is less than or equal to 0");
  }

  if (!Item->bStackable) {
    throw std::invalid_argument("Item is not stackable");
  }

  int32 RemainingQuantity = Quantity;
  TArray<int32> IndexesToRemove;

  while (RemainingQuantity > 0) {
    for (auto i = 0; i < Items.Num(); ++i) {
      if (Items[i].Item == Item) {
        const int32 QuantityToRemove = FMath::Min(RemainingQuantity, Items[i].Quantity);
        Items[i].Quantity -= QuantityToRemove;
        RemainingQuantity -= QuantityToRemove;

        if (Items[i].Quantity == 0) {
          IndexesToRemove.Add(i);
        }
      }
    }
  }

  for (int32 i = IndexesToRemove.Num() - 1; i >= 0; --i) {
    Items.RemoveAt(IndexesToRemove[i]);
  }
}

bool USimpleInventoryComponent::HasItem(USimpleInventoryItem* Item) const {
  for (const FInventoryItem& InventoryItem: Items) {
    if (InventoryItem.Item == Item) {
      return true;
    }
  }

  return false;
}

int32 USimpleInventoryComponent::GetItemQuantity(USimpleInventoryItem* Item) const {
  for (const FInventoryItem& InventoryItem: Items) {
    if (InventoryItem.Item == Item) {
      return InventoryItem.Quantity;
    }
  }

  return 0;
}
