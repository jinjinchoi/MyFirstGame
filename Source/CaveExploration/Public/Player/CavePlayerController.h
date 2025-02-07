// Copyright Jonwoo-Choi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CavePlayerController.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class AMagicCircle;
class UDamageTextWidgetComponent;
class UCaveAbilitySystemComponent;
struct FGameplayTag;
class UCaveInputConfig;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	const AActor* Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsOccluded;
};

/**
 * 
 */
UCLASS()
class CAVEEXPLORATION_API ACavePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACavePlayerController();

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(const float DamageAmount, ACharacter* TargetCharacter, const bool bIsCriticalHit, const FGameplayTag& DamageType);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	
	FVector GetValidMagicCircleLocation() const;
	
protected:
	/* Engine */
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	/* end Engine */


private:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputMappingContext> CaveContext;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UInputAction> MoveAction;
	
	void Move(const FInputActionValue& InputValue);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	TObjectPtr<UCaveInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UCaveAbilitySystemComponent> CaveAbilitySystemComponent;

	UCaveAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidgetComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY(Replicated)
	TObjectPtr<AMagicCircle> MagicCircle;



	
#pragma region make actor translucent
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera Occlusion|Occlusion", meta=(ClampMin="0.1", ClampMax="10.0"))
	float CapsulePercentageForTrace = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera Occlusion|Occlusion")
	bool IsOcclusionEnabled = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera Occlusion|Occlusion")
	bool DebugLineTraces = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera Occlusion|Materials")
	UMaterialInterface* FadeMaterial;

	UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
	TObjectPtr<USpringArmComponent> ActiveSpringArm;

	UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
	TObjectPtr<UCameraComponent> ActiveCameraComponent;

	UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
	TObjectPtr<UCapsuleComponent> ActiveCapsuleComponent;


private:
	UPROPERTY()
	TMap<const AActor*, FCameraOccludedActor> OccludedActors;

	void HideOccludedActor(const AActor* Actor);
	void OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ShowOccludedActor(FCameraOccludedActor& OccludedActor);
	void OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ForceShowOccludedActor();

	FORCEINLINE bool ShouldCheckCameraOcclusion() const
	{
		return IsOcclusionEnabled && FadeMaterial && ActiveCameraComponent && ActiveCapsuleComponent;
	}

#pragma endregion
};
