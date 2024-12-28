// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGMiniMapWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/PGIconWidget.h"
#include "PortGame/PortGame.h"
#include "Engine/LevelScriptActor.h"
#include "Interface/FieldManagerInterface.h"
#include "Field/FieldManager.h"
#include "Field/PGField.h"
#include "Field/ObjectPoolManager.h"
#include "Interface/ObjectPoolingInterface.h"
#include "Character/PGNpcCharacter.h"

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

    static ConstructorHelpers::FClassFinder<UPGIconWidget> NPCiconc(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_EnemyIcon.BP_EnemyIcon_C'"));
    if (NPCiconc.Class)
    {
        NPCIconClass = NPCiconc.Class;
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

   /* if (Players.Num() > 0)
    {
        UpdateNPCIconPosition();
    }*/
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

    //�ʵ嵵 setup
    SetUpFieldArray();
   
    //NPC setup
    SetUpNPCCharacter();
}

void UPGMiniMapWidget::SetUpFieldArray()
{
    if (GetWorld())
    {
        IFieldManagerInterface* fieldmanager =Cast<IFieldManagerInterface>(GetWorld()->GetLevelScriptActor());
        if (fieldmanager)
        {

            if (fieldmanager->GetFieldManager())
            {
                Fields = fieldmanager->GetFieldManager()->GetFields();
              
                SetUpFieldIcon();

                fieldmanager->GetFieldManager()->OnfieldIndexchanged.AddUObject(this, &ThisClass::ChangedField);
            }
           

        }
    }
}

void UPGMiniMapWidget::SetUpFieldIcon()
{
    if (Fields.Num() > 0)
    {
        for (APGField* field : Fields)
        {
            if (CanvasPanel_minimap && CharacterIconClass)
            {
                FVector2D fieldiconlocation = ConvertWorldToMiniMap(field->GetActorLocation());
                UPGIconWidget* fieldIcon = CreateWidget<UPGIconWidget>(GetWorld(), FieldIconClass);
                
                if (fieldIcon)
                {
                    int8 teamcolor = field->GetGenericTeamId();
                    fieldIcon->ChangeFieldColor(teamcolor);

                    UCanvasPanelSlot* CanvasSlot = CanvasPanel_minimap->AddChildToCanvas(fieldIcon);
                    if (CanvasSlot)
                    {
                        CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f));

                        CanvasSlot->SetPosition(fieldiconlocation);
                        CanvasSlot->SetZOrder(0);

                        SLOG(TEXT("%s : %f , %f"), *field->GetActorNameOrLabel(), field->GetActorScale().X, field->GetActorScale().Y);
                        FVector2D fieldSize = ConvertFieldSizeToMiniMap(field->GetActorScale());

                        CanvasSlot->SetSize(fieldSize);

                    }

                    FieldIcons.Add(fieldIcon);
                }

            }
        }
    }
}

void UPGMiniMapWidget::GetNPCToIbjectPool()
{
    if (GetWorld())
    {
        IObjectPoolingInterface* pooling = Cast<IObjectPoolingInterface>(GetWorld()->GetLevelScriptActor());
        if (pooling)
        {

            if (pooling->GetObjectPoolManager())
            {
                NpcCharacters.Empty();

                NpcCharacters = pooling->GetObjectPoolManager()->GetAttackNPCCharacter();

            }
        }
    }
}


void UPGMiniMapWidget::SetUpNPCCharacter()
{
    GetNPCToIbjectPool();

    if (NpcCharacters.Num() > 0)
    { 
        for (APGNpcCharacter* NPC : NpcCharacters)
        {

            FVector2D minmaplocation = ConvertWorldToMiniMap(NPC->GetActorLocation());
            UPGIconWidget* newicon = AddNPCIcon(NPC->GetGenericTeamId(), minmaplocation);
            if (newicon!=nullptr)
                NPCIcons.Add(newicon);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(NPCIconUpdateHandler, this, &UPGMiniMapWidget::UpdateNPCIconPosition, NPCUpdateTime, true);
   
}

void UPGMiniMapWidget::ChangedField(int8 index, bool lock)
{
    if (Fields.Num() > 0 && FieldIcons.Num() > 0)
    {
        int8 teamcolor = Fields[index]->GetGenericTeamId();
        FieldIcons[index]->ChangeFieldColor(teamcolor);
        FieldIcons[index]->VisibleLockImage(lock);
    }

}

FVector2D UPGMiniMapWidget::ConvertWorldToMiniMap(FVector WorldLocation)
{
	
    float MiniMapX = ((WorldLocation.Y - WorldYMin) / (WorldYMax - WorldYMin)) * (MiniMapXMax - MiniMapXMin) + MiniMapXMin;
    float MiniMapY = ((WorldLocation.X - WorldXMin) / (WorldXMax - WorldXMin)) * (MiniMapYMax - MiniMapYMin) + MiniMapYMin;
	
	return FVector2D(MiniMapX, -MiniMapY);

}

FVector2D UPGMiniMapWidget::ConvertFieldSizeToMiniMap(FVector fieldScale)
{
       float sizeX = (MiniMapYMax - MiniMapYMin) / (WorldYMax - WorldYMin) * fieldScale.Y * 50.0f * 1.45f;
       float sizeY = (MiniMapYMax - MiniMapYMin) / (WorldYMax - WorldYMin) * fieldScale.X * 50.0f * 1.55f;

    return FVector2D(sizeX, sizeY);
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

UPGIconWidget* UPGMiniMapWidget::AddNPCIcon(int8 teamid, FVector2D IconPosition)
{
    if (CanvasPanel_minimap && NPCIconClass)
    {

        UPGIconWidget* NPCIcon = CreateWidget<UPGIconWidget>(GetWorld(), NPCIconClass);
        if (NPCIcon)
        {
            UCanvasPanelSlot* CanvasSlot = CanvasPanel_minimap->AddChildToCanvas(NPCIcon);
            if (CanvasSlot)
            {
                CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f));

                CanvasSlot->SetPosition(IconPosition);

                NPCIcon->SetNPCIconImage(teamid);

                
                CanvasSlot->SetSize(FVector2D(15.0f, 15.0f));


                return NPCIcon;
            }
        }
    }

    return nullptr;
}

void UPGMiniMapWidget::RecycleNPCIcon(int32 index,int8 teamid, FVector2D IconPosition)
{
    if (CanvasPanel_minimap)
    {
        if (NPCIcons.IsValidIndex(index))
        {
            if (NPCIcons[index])
            {
                UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NPCIcons[index]->Slot);
                if (CanvasSlot)
                {
                    NPCIcons[index]->SetVisibility(ESlateVisibility::Visible);
                    
                    CanvasSlot->SetPosition(IconPosition);

                    NPCIcons[index]->SetNPCIconImage(teamid);


                    CanvasSlot->SetSize(FVector2D(15.0f, 15.0f));
                }
            }
        }
        else
        {
            UPGIconWidget* newicon = AddNPCIcon(teamid, IconPosition);
            if (newicon != nullptr)
                NPCIcons.Add(newicon);
        }

    }
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

void UPGMiniMapWidget::UpdateNPCIconPosition()
{
    GetNPCToIbjectPool();

    for (UPGIconWidget* npcicon : NPCIcons)
    {
        npcicon->SetVisibility(ESlateVisibility::Collapsed);
    }


    if (NpcCharacters.Num() > 0)
    {
        for (int32 i=0;i< NpcCharacters.Num();i++)
        {

            FVector2D minmaplocation = ConvertWorldToMiniMap(NpcCharacters[i]->GetActorLocation());
            RecycleNPCIcon(i, NpcCharacters[i]->GetGenericTeamId(), minmaplocation);

        }
    }
}
