// Created by Ashechol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadWidget.generated.h"


UCLASS()
class SHOOT_API UOverHeadWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DisplayText;

	void SetDisplayText(FString Text);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* Pawn);

protected:

	virtual void NativeDestruct() override;
};
