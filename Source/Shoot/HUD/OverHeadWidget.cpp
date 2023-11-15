// Created by Ashechol


#include "OverHeadWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverHeadWidget::SetDisplayText(FString Text)
{
	if (!DisplayText) return;

	DisplayText.Get()->SetText(FText::FromString(Text));
}

void UOverHeadWidget::ShowPlayerNetRole(APawn* Pawn)
{
	// ENetRole LocalRole = Pawn->GetLocalRole();
	// FString Role;
	//
	// switch (LocalRole)
	// {
	// 	case ROLE_Authority:
	// 		Role = FString("Authority");
	// 		break;
	//
	// 	case ROLE_AutonomousProxy:
	// 		Role = FString("Autonomous Proxy");
	// 		break;
	//
	// 	case ROLE_SimulatedProxy:
	// 		Role = FString("Simulated Proxy");
	// 		break;
	//
	// 	default:
	// 		break;
	// }

	if (auto PlayerState = Pawn->GetPlayerState())
	{
		SetDisplayText(PlayerState->GetName());
	}

	// SetDisplayText(FString::Printf(TEXT("Local Role: %s"), *Role));
}

void UOverHeadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
