// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainContent/LocomotionSystem/ActorProfileEffects/HitReaction.h"
#include "MainContent/LocomotionSystem/Data/SenseRules_Enum.h"
#include "ActorProfileSystem.generated.h"


class AActorProfileEffectsBase;
enum class ESenseRule : uint8;
class UActorProfileContainer;
class UHealth;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdated, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_NineParams(FOnTakeDamageDelegate, AActor*, DamagedActor, float, Damage, class AController*, InstigatedBy, FVector, HitLocation, UPrimitiveComponent*, FHitComponent, FName, BoneName, FVector, ShotFromDirection, const UDamageType*, DamageType, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_NineParams(FOnDeath, AActor*, DamagedActor, float, Damage, class AController*, InstigatedBy, FVector, HitLocation, UPrimitiveComponent*, FHitComponent, FName, BoneName, FVector, ShotFromDirection, const UDamageType*, DamageType, AActor*, DamageCauser);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORRORZONE_API UActorProfileSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorProfileSystem();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE FOnHealthUpdated& GetOnHealthUpdatedDelegate(){return OnHealthUpdated;}
	FORCEINLINE FOnTakeDamageDelegate& GetOnTakeDamageDelegate(){return OnTakeDamageDelegate;}
	FORCEINLINE FOnDeath& GetOnDeathDelegate(){return OnDeathDelegate;}
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE TArray<FName> GetTags() const { return Tags; }
	FORCEINLINE ESenseRule GetSenseRule() const { return SenseRule; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void InitializeHealth();

	void InitializeActorProfileContainer();
	
	void SetActorProfileContainerVisibility(ESlateVisibility Visibility) const;
	
	void UpdateHealth(float Value);
	
	void PerformNPCTrace();
	
	void InitializeEffects();
	
	TArray<AActor*> TraceForNPCs();
	
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

	// Delegates
	FOnHealthUpdated OnHealthUpdated;
	FOnTakeDamageDelegate OnTakeDamageDelegate;
	FOnDeath OnDeathDelegate;
	
	// Variables
	FTimerHandle TraceNPCTimer;
	
	TArray<TWeakObjectPtr<AActor>> PreviouslyVisibleNPCs;
	
	UPROPERTY()
	TArray<FName> Tags = {"NPC"};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	ESenseRule SenseRule = ESenseRule::ActiveOnlyIfPlayerInRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	bool bShowHealthBar = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	TSubclassOf<UActorProfileContainer> ActorProfileContainerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	TSubclassOf<UHealth> HealthWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	TObjectPtr<UActorProfileContainer> ActorProfileContainerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	FName ActorProfileContainerSocket;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	FVector ActorProfileContainerOffset = FVector(40.0f, 0.0f, 100.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	float Health = 100.0f;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	float SenseRange = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	TObjectPtr<AActor> Owner;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	TObjectPtr<ACharacter> Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	TArray<TSubclassOf<AActorProfileEffectsBase>> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Actor Profile")
	bool bIsDead;
};
