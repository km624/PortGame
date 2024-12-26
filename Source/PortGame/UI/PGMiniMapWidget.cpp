// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGMiniMapWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/PGIconWidget.h"
#include "PortGame/PortGame.h"


UPGMiniMapWidget::UPGMiniMapWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UPGIconWidget> iconc(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_Icon.BP_Icon_C'"));
    if (iconc.Class)
    {
        iconClass = iconc.Class;
    }
}

void UPGMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UPGMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPGMiniMapWidget::SetUpMinimap()
{
}

void UPGMiniMapWidget::SetupPlayers(int8 mynum, const TArray<AActor*>& ActorArray)
{
    
    if (ActorArray.Num() > 0)
    {
       
        for (int32 i = 0; i < ActorArray.Num(); i++)
        {
            bool mine = false;
            if (i == mynum)
            {
                mine = true;
                FVector2D minmaplocation = ConvertWorldToMiniMap(ActorArray[i]->GetActorLocation());
                //SLOG(TEXT("%d : %f   %f"), i, minmaplocation.X, minmaplocation.Y);
                UPGIconWidget* newicon = AddPlayerIcon(mine, minmaplocation);
                Icons.Add(newicon);
            }
           
          
        }

    }
   
}

FVector2D UPGMiniMapWidget::ConvertWorldToMiniMap(FVector WorldLocation)
{
	/*float MiniMapX = ((WorldLocation.X - WorldXMin) / (WorldXMax - WorldXMin)) * (MiniMapXMax - MiniMapXMin) + MiniMapXMin;
	float MiniMapY = ((WorldLocation.Y - WorldYMin) / (WorldYMax - WorldYMin)) * (MiniMapYMax - MiniMapYMin) + MiniMapYMin;*/

    float MiniMapX = ((WorldLocation.Y - WorldYMin) / (WorldYMax - WorldYMin)) * (MiniMapXMax - MiniMapXMin) + MiniMapXMin;
    float MiniMapY = ((WorldLocation.X - WorldXMin) / (WorldXMax - WorldXMin)) * (MiniMapYMax - MiniMapYMin) + MiniMapYMin;
	
	return FVector2D(MiniMapX, -MiniMapY);

}

UPGIconWidget* UPGMiniMapWidget::AddPlayerIcon(bool mine,FVector2D IconPosition)
{
    if (CanvasPanel_minimap && iconClass)
    {
        
        UPGIconWidget* playerIcon = CreateWidget<UPGIconWidget>(GetWorld(), iconClass);
        if (playerIcon)
        {
            UCanvasPanelSlot* CanvasSlot = CanvasPanel_minimap->AddChildToCanvas(playerIcon);
            if (CanvasSlot)
            {
                CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f));
                SLOG(TEXT(" %f ,  %f"), IconPosition.X, IconPosition.Y);
                CanvasSlot->SetPosition(IconPosition);
                if (mine)
                {
                    CanvasSlot->SetZOrder(2);
                    //playerIcon->ColorAndOpacity = FLinearColor(FColor::Blue);
                }
                else
                {
                    CanvasSlot->SetZOrder(1);
                }
               
                CanvasSlot->SetSize(FVector2D(15.0f, 15.0f));

               
                return playerIcon;
            }
        }
    }
    return nullptr;
}
