// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGMiniMapWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/PGIconWidget.h"
#include "PortGame/PortGame.h"


UPGMiniMapWidget::UPGMiniMapWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UPGIconWidget> iconc(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_CharacterIcon.BP_CharacterIcon_C'"));
    if (iconc.Class)
    {
        CharacterIconClass = iconc.Class;
    }

    static ConstructorHelpers::FClassFinder<UPGIconWidget> FIconc(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_FieldIcon.BP_FieldIcon_C'"));
    if (FIconc.Class)
    {
        FieldIconClass = FIconc.Class;
    }
}

void UPGMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UPGMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    if (Players.Num() > 0)
    {
        UpdateIconPosition();
    }
}



void UPGMiniMapWidget::SetupPlayers(int8 mynum, const TArray<AActor*>& ActorArray)
{
    Players = ActorArray;
    MyNum = mynum;
   
    if (ActorArray.Num() > 0)
    {
        for (int32 i = 0; i < ActorArray.Num(); i++)
        {
            bool mine = false;
            if (i == mynum)
            {
                mine = true;
               
            }
            FVector2D minmaplocation = ConvertWorldToMiniMap(ActorArray[i]->GetActorLocation());
            UPGIconWidget * newicon = AddPlayerIcon(mine, minmaplocation, ActorArray[i]->GetActorRotation().Yaw);
            Icons.Add(newicon);
           
          
        }

    }
   
}

FVector2D UPGMiniMapWidget::ConvertWorldToMiniMap(FVector WorldLocation)
{
	
    float MiniMapX = ((WorldLocation.Y - WorldYMin) / (WorldYMax - WorldYMin)) * (MiniMapXMax - MiniMapXMin) + MiniMapXMin;
    float MiniMapY = ((WorldLocation.X - WorldXMin) / (WorldXMax - WorldXMin)) * (MiniMapYMax - MiniMapYMin) + MiniMapYMin;
	
	return FVector2D(MiniMapX, -MiniMapY);

}

UPGIconWidget* UPGMiniMapWidget::AddPlayerIcon(bool mine,FVector2D IconPosition,float playerRotatiaon)
{
    if (CanvasPanel_minimap && CharacterIconClass)
    {
        
        UPGIconWidget* playerIcon = CreateWidget<UPGIconWidget>(GetWorld(), CharacterIconClass);
        if (playerIcon)
        {
            UCanvasPanelSlot* CanvasSlot = CanvasPanel_minimap->AddChildToCanvas(playerIcon);
            if (CanvasSlot)
            {
                CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f));
               
                CanvasSlot->SetPosition(IconPosition);
                
                if (mine)
                {
                    CanvasSlot->SetZOrder(2);
                    playerIcon->SetPlayerIconImage();
                }
                else
                {
                    CanvasSlot->SetZOrder(1);
                }
               
                FWidgetTransform Transform;
                Transform.Angle = playerRotatiaon; 
               

                playerIcon->SetRenderTransform(Transform);


                CanvasSlot->SetSize(FVector2D(30.0f, 30.0f));

               
                return playerIcon;
            }
        }
    }
    return nullptr;
}

void UPGMiniMapWidget::UpdateIconPosition()
{
    for (int32 i=0; i < Players.Num(); i++)
    {
        FVector playerLocation = Players[i]->GetActorLocation();
        FRotator playerRotation = Players[i]->GetActorRotation();

        FVector2D IconPos = ConvertWorldToMiniMap(playerLocation);


 
        FWidgetTransform Transform;
        Transform.Angle = playerRotation.Yaw;

        Icons[i]->SetRenderTransform(Transform);

        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Icons[i]->Slot);
        if (CanvasSlot)
        {
            CanvasSlot->SetPosition(IconPos);
        }

    }
}
