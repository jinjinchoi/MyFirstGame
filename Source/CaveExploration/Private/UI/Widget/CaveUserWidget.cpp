// Copyright Jonwoo-Choi


#include "UI/Widget/CaveUserWidget.h"

void UCaveUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
