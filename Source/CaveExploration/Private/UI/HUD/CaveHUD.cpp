// Copyright Jonwoo-Choi


#include "UI/HUD/CaveHUD.h"

#include "UI/Widget/CaveUserWidget.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* ACaveHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	
	
	return OverlayWidgetController;
}

UAttributeWidgetController* ACaveHUD::GetAttributeWidgetController(const FWidgetControllerParams& Params)
{
	if (AttributeWidgetController == nullptr)
	{
		AttributeWidgetController = NewObject<UAttributeWidgetController>(this, AttributeWidgetControllerClas);
		AttributeWidgetController->SetWidgetControllerParams(Params);
		AttributeWidgetController->BindCallbacksToDependencies();
	}


	return AttributeWidgetController;
}

USpellMenuWidgetController* ACaveHUD::GetSpellMenuController(const FWidgetControllerParams& Params)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(Params);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}

	return SpellMenuWidgetController;
}

void ACaveHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	check(OverlayWidgetClass);
	
	OverlayWidget = CreateWidget<UCaveUserWidget>(GetWorld(), OverlayWidgetClass);
	
	const FWidgetControllerParams WidgetParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetParams);
	
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadCastInitialValues();
	
	OverlayWidget->AddToViewport();
	
	
}
