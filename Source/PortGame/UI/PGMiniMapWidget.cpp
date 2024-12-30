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
#include "Components/Image.h"
#include "UI/PGPlayerIconWidget.h"
#include "Character/PGBaseCharacter.h"
#include "AI/PGAIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AI/PGAI.h"
#include "SlateCore.h"

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


   
}



void UPGMiniMapWidget::SetupPlayers(int8 mynum, const TArray<AActor*>& ActorArray, const TArray<APGAIController*>& allaicontrollers)
{
    Players = ActorArray;
    MyNum = mynum;
    PlayersControllers = allaicontrollers;
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
            UPGPlayerIconWidget* playericon = Cast< UPGPlayerIconWidget>(newicon);
            if (playericon)
            {
                //임시
                APGBaseCharacter* playercharacter = Cast<APGBaseCharacter>(ActorArray[i]);
                if (playercharacter)
                {
                    playericon->SetUpPlayerIconImage(playercharacter->GetPlayerCharacterType() , mine, i, this);
                    Icons.Add(newicon);
                }  
            }     
        }
    }

    bIsMiniMap = true;

    //필드도 setup
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

void UPGMiniMapWidget::ChangeMinimapSize(bool bisminimap)
{
    //미니맵 설정
    FLinearColor minimapalpha;

    bIsMiniMap = bisminimap;

    if (bisminimap)
    {
        //선택한 캐릭터 인덱스 초기화
        SelectIndex = -1;

        minimapalpha = FLinearColor(FVector4d(1.0f, 1.0f, 1.0f, 0.7f));
        MiniMapXMax *= 0.5f;
        MiniMapYMax *= 0.5f;
        
    }
    else
    {
        minimapalpha = FLinearColor(FVector4d(1.0f, 1.0f, 1.0f, 1.0f));
        MiniMapXMax *= 2.0f;
        MiniMapYMax *= 2.0f;
       
    }

    UCanvasPanelSlot* ImageCanvasSlot = Cast<UCanvasPanelSlot>(Image_Minimap->Slot);
    if (ImageCanvasSlot)
    {
        Image_Minimap->SetColorAndOpacity(minimapalpha);
        ImageCanvasSlot->SetSize(FVector2D(MiniMapXMax, MiniMapYMax));
    }

    //플레이어 아이콘 설정
    for (UPGIconWidget* icon : Icons)
    {
       
        UPGPlayerIconWidget* playericon = Cast< UPGPlayerIconWidget>(icon);

        UCanvasPanelSlot* playerCanvasSlot = Cast<UCanvasPanelSlot>(playericon->Slot);
        if (playerCanvasSlot)
        {
            if (bisminimap)
            {
                playerCanvasSlot->SetSize(FVector2D(30.0f, 30.0f));
            }
            else
            {
                playerCanvasSlot->SetSize(FVector2D(100.0f, 100.0f));
            }

            playericon->ChangePlayerIcon(bisminimap);

        }
    }

    

    //강제로 한번 Update NPC
    UpdateNPCIconPosition();

    //필드의 위치와 사이즈 다시 변환
    UpdateFieldIconPostition();


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

        // 겹침 방지를 위한 오프셋 계산
        FVector2D TotalOffset = FVector2D(0.0f, 0.0f);
        for (int32 j = 0; j < i; j++) // 이전 아이콘들과 거리 계산
        {
            FVector otherLocation = Players[j]->GetActorLocation();
            FVector2D OtherIconPos = ConvertWorldToMiniMap(otherLocation);

            float Distance = FVector2D::Distance(IconPos, OtherIconPos);
            float DistanceSafe;
            if (bIsMiniMap)
            {
                DistanceSafe = 10.0f;
            }
            else
            {
                DistanceSafe = 50.0f;
            }

            if (Distance < DistanceSafe) // 거리 임계값 (10은 예시값)
            {
                FVector2D Offset = (IconPos - OtherIconPos).GetSafeNormal() * DistanceSafe;
                TotalOffset += Offset;
            }
        }
        IconPos += TotalOffset;


 
        FWidgetTransform Transform;
        if (bIsMiniMap)
        {
            Transform.Angle = playerRotation.Yaw;
        }
        else
        {
            Transform.Angle = 0.0f;
        }
      

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

void UPGMiniMapWidget::UpdateFieldIconPostition()
{
    if (Fields.Num() > 0)
    {
        for (int32 i=0;i< Fields.Num();i++)
        {
            if (CanvasPanel_minimap)
            {
                FVector2D fieldiconlocation = ConvertWorldToMiniMap(Fields[i]->GetActorLocation());
               
                if (FieldIcons.IsValidIndex(i))
                {

                    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(FieldIcons[i]->Slot);
                    if (CanvasSlot)
                    {
                        //CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f));

                        CanvasSlot->SetPosition(fieldiconlocation);
                        CanvasSlot->SetZOrder(0);


                        FVector2D fieldSize = ConvertFieldSizeToMiniMap(Fields[i]->GetActorScale());

                        CanvasSlot->SetSize(fieldSize);

                    }

        
                }

            }
        }
    }
}

UNavigationPath* UPGMiniMapWidget::CalculateNavigationPath(FVector StartLocation, FVector TargetLocation)
{
   
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys) return nullptr;

    return NavSys->FindPathToLocationSynchronously(GetWorld(), StartLocation, TargetLocation);
   
}

FReply UPGMiniMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        FVector2D ClickedPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
        //SLOG(TEXT("%f , %f "), ClickedPosition.X, ClickedPosition.Y);
        FVector TargetWorldLocation = ConvertMiniMapToWorld(ClickedPosition);
        //SLOG(TEXT("%f , %f "), TargetWorldLocation.X, TargetWorldLocation.Y);

        if (SelectIndex != -1)
        {
            MoveSelectCharacterToLocation(SelectIndex, TargetWorldLocation);
         
        }

    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UPGMiniMapWidget::MoveSelectCharacterToLocation(int32 index, FVector TargetLocation)
{
    if (PlayersControllers.IsValidIndex(index))
    {
        UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(PlayersControllers[index]);
        if (NavSys)
        {
            if (IsLocationReachable(index, TargetLocation))
            {
                PlayersControllers[index]->SetForceMoveVector(TargetLocation);
                SelectIndex = -1;
                MiniMapPathPoints.Empty();
               
            }
           
        }
    }
}

FVector UPGMiniMapWidget::ConvertMiniMapToWorld(FVector2D targetPostion)
{

    float WorldMapX = ((((targetPostion.Y - MiniMapYMax)*-1) - MiniMapYMin) / (MiniMapYMax - MiniMapYMin)) * (WorldXMax - WorldXMin) + WorldXMin;
    float WorldMapY = ((targetPostion.X - MiniMapXMin) / (MiniMapXMax - MiniMapXMin)) * (WorldYMax - WorldYMin) + WorldYMin;
    
    
    FVector CheckLocation = CheckNavigableLocation(FVector(WorldMapX, WorldMapY, 1200.f));

    return CheckLocation;
}

FVector UPGMiniMapWidget::CheckNavigableLocation(FVector TargetLocation)
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSys)
    {
       
        FNavLocation NavLocation;
        FVector QueryExtent = FVector(100.0f, 100.0f, 2000.0f);
        if (NavSys->ProjectPointToNavigation(TargetLocation, NavLocation, QueryExtent))
        {

           
            return NavLocation.Location; 
        }
    }

    
    return FVector::ZeroVector;
}

bool UPGMiniMapWidget::IsLocationReachable(int32 index, FVector TargetLocation)
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Players[index]->GetWorld());
    if (NavSys)
    {
        // AIActor의 현재 위치
        FVector StartLocation = Players[index]->GetActorLocation();

        // 경로 계산
        UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(Players[index]->GetWorld(), StartLocation, TargetLocation);
        if (NavPath && NavPath->IsValid())
        {
            return true; 
        }
    }
    return false; 
   
}

TArray<FVector> UPGMiniMapWidget::CalculateNavPath(FVector StartLocation, FVector EndLocation)
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    TArray<FVector> PathPoints;

    if (NavSys)
    {
        UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(GetWorld(), StartLocation, EndLocation);
        if (NavPath && NavPath->IsValid())
        {
            PathPoints = NavPath->PathPoints;
        }

    
    }

    return PathPoints;
}

FReply UPGMiniMapWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseMove(InGeometry, InMouseEvent);
    if (SelectIndex != -1)
    {
        
        FVector2D MousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
        FVector WorldTargetLocation = ConvertMiniMapToWorld(MousePosition);
        TArray<FVector> Paths = CalculateNavPath(Players[SelectIndex]->GetActorLocation(), WorldTargetLocation);
        MiniMapPathPoints.Empty();
        for (const FVector&  path : Paths)
        {
            FVector2D Convertpath = ConvertWorldToMiniMap(path);
            MiniMapPathPoints.Add(FVector2D(Convertpath.X, Convertpath.Y+ 910.0f));
        }
      
    }
   
    return FReply::Handled();
}

int32 UPGMiniMapWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{

    Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
    
    // 경로를 그릴 준비가 되었는지 확인
    if (MiniMapPathPoints.Num() > 1)
    {
        for (int32 i = 0; i < MiniMapPathPoints.Num() - 1; ++i)
        {
            
           
            /*FVector2D Start = AllottedGeometry.LocalToAbsolute(MiniMapPathPoints[i]);
            FVector2D End = AllottedGeometry.LocalToAbsolute(MiniMapPathPoints[i + 1]);*/
            FVector2D Start = MiniMapPathPoints[i];
            FVector2D End =MiniMapPathPoints[i + 1];

            /*FVector2D ConvertStart = FVector2D(MiniMapYMaxStart.X, MiniMapYMax- Start.Y );
            FVector2D ConvertEnd = FVector2D(End.X, End.Y - MiniMapYMax);*/
            if (i == 0)
            {
                SLOG(TEXT("Start Vector : %f   , %f"), Start.X, Start.Y);
            }
            FSlateDrawElement::MakeLines(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(),
                { Start, End },
                ESlateDrawEffect::None,
                FLinearColor::Green.CopyWithNewOpacity(1.0f), // 완전히 불투명
                true,
                5.0f // 더 두꺼운 선
            );
        }
    }

    return LayerId + 1;
}

