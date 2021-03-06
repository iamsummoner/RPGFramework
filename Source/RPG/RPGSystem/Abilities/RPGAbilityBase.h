// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "GameplayTagContainer.h"
#include "../Effects/RPGEffectBase.h"
#include "../Structs/RPGSystemSructs.h"
#include "RPGAbilityBase.generated.h"

/**
 * This is base class for all powers in game.
 * Power is an action that can be performed by pawn, if said pawn have PowerManagerComponent.
 * like casting spell, throwing granade, shooting weapon, swinging sword. 
 Well, maybe those be better implemented in RPGWeapon ;)
 * but any special action like whirlwind sword, is essentialy power that trigger special animation, 
 have set duration (how long will character whirlwind ?), cost (how much endurance?)
 * cooldown (how long before I can use it again) and other costs defined.
 * power can also require character to have special weapon equiped, in order to use it.
 * powers are never placed directly in level. The class is data holder as well as contains 
 common function needed to perform power.
 * power logic/behavior should be defined withing blueprint, which will derive 
 from appropriate Power class.
 */
/* TODO
1. Add cast time handling (time between button press by player and triggering action). - semi done
2. Add resource cost handling. - done
3. Add weapon requierment. - semi done
4. Add attribute requirment.
5. Add cooldown handling (time after which power can be used again). - done
6. Add projectile shooting.
7. Add types of power, by enum.
8. Add mele type functions. Or create another power down the line in class hierarchy.
9. 
*/
//weapon that is required to use ability.

/*
Should we take resource cost on cast start or on cast finish ?
Start seems better, for sole reason is less forgiving ;)
*/
UENUM()
enum ECastType
{
	Casted,
	Casted_Charged,
	Channeled,
	Instant
};

UCLASS(BlueprintType, Blueprintable)
class URPGAbilityBase : public UObject, public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;
	virtual bool IsTickable() const OVERRIDE;
	virtual TStatId GetStatId() const OVERRIDE;

	virtual class UWorld* GetWorld() const OVERRIDE;
	/*
		Launch Order:
		1. When player press input button StartAbility() is fired.
		2. StartAbility() call Initialize(). If you override StartAbility()
		Completly, you should make sure to Override Initialize() and add it to StartAbility();
		3. When Initialize() is passed, StartAbility() check is ability is on Cooldown,
		and then check how it should be casted.
	*/
	/*
	*/
	virtual void InputPressed();
	virtual void InputReleased();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Events")
	void OnAbilityStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Events")
	void OnAbilityStop();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Events")
		void OnAbilityInitialized();
	/*
		Call only when ability is equiped for use. Ie. dragged to hotbar, or prepared from spellbook.
		Assign any properties that are needed prior ability can be used. 
		For example check for componenets.
	*/
	virtual void Initialize(APawn* owner, AController* instigator);
	//should it be here or move it game specific ?

protected:
	UPROPERTY(EditAnywhere, Category = "Ability Properties")
		float MaxCastTime;

	UPROPERTY(EditAnywhere, Category = "Ability Properties")
		float ChannelInterval;

	UPROPERTY(EditAnywhere, Category = "Ability Properties")
		float MaxOverCastTime;

	UPROPERTY(EditAnywhere, Category = "Ability Properties")
		float AbilityCooldownTime;

	UPROPERTY(EditAnywhere, Category = "Ability Properties")
		TArray<FModdableAttributes> AttributeCost;



	//class TWeakObjectPtr<class URPGEffectBase> AbilityCost;

private:
	bool CheckAbilityCost();
public:
	/*
		Effect for handling casting (using) ability.
	*/
	FGameplayTagContainer AbilityTags;

private:
	bool isChanneled;
	bool isCasted;
	bool isOnCooldown;
	bool isCharged;
	float currentIntervalTime;
	float currentCooldownTime;
	float currentCastTime;
	float currentChargeTime;
protected:
	bool IsAbilityInitialized;
public:
	UPROPERTY(EditAnywhere, Category = "Ability Properties")
	TEnumAsByte<ECastType> AbilityCastType;
public:
	void FORCEINLINE SetAbilityOwner(APawn* Owner) { AbilityOwner = Owner; };
	void FORCEINLINE SetInstigator(AController* instigator) { Instigator = instigator; };

protected:
	void CacheInstantEffects();

	UFUNCTION(BlueprintPure, Category = "RPG|Ability")
		float GetOwnerAttribute(FName AttributeName);

	/*
		maybe change it to APawn ?
		Most actors are static anyway, and they do not
		posses all the functionality needed for Ability to be 
		correctly used in the first place.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	APawn* AbilityOwner;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	AController* Instigator;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	TWeakObjectPtr<class URPGAttributeComponent> OwnerAttributeComp;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	TWeakObjectPtr<class URPGAttributeComponent> TargetAttributes;

	//UPROPERTY(BlueprintReadOnly, Category = "Ability")
	//	AActor* AbilityTarget;

};
