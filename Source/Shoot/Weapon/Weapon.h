// Created by Ashechol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UWidgetComponent;
class USphereComponent;

UENUM(BlueprintType)
// 使用 enum class 而不是 enum 是因为 enum 的枚举名是全局的，容易冲突
enum class EWeaponState: uint8
{
	Initial,
	Equipped,
	Dropped,

	Max UMETA(Hidden)
};

UCLASS()
class SHOOT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	
	AWeapon();

private:

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	TObjectPtr<UWidgetComponent> PickUpWidget;

	UPROPERTY(VisibleAnywhere)
	EWeaponState WeaponState;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);

public:
	
	virtual void Tick(float DeltaTime) override;
};
