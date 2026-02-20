    // Fill out your copyright notice in the Description page of Project Settings.

    #include "MainContent/ControlRig/FRigUnit_SetFinalLegIK.h"

    #include "Math/ControlRigMathLibrary.h"

    FRigUnit_SetFinalLegIK_Execute()
    {
        URigHierarchy* Hierarchy = ExecuteContext.Hierarchy;

        if (Hierarchy)
        {
            for (int32 i = 0; i < FootItems.Num(); ++i)
            {
                FRigElementKey EffectorKey = FootItems[i];
               
                // Get parent (ItemB)
                FRigElementKey ItemBKey = Hierarchy->GetFirstParent(EffectorKey);
                if (!ItemBKey.IsValid())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Parent (ItemB) not found for %s"), *EffectorKey.ToString());
                    continue;
                }

                // Get grandparent (ItemA)
                FRigElementKey ItemAKey = Hierarchy->GetFirstParent(ItemBKey);
                if (!ItemAKey.IsValid())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Grandparent (ItemA) not found for %s"), *ItemBKey.ToString());
                    continue;
                }
                
	            FVector PoleVector = FVector(0.0f, 5000.0f, 0.0f);
                FVector PrimaryAxis = i == 0 ? FVector(-1.0, 0.0, 0.0) : FVector(1.0, 0.0, 0.0);
                FVector SecondaryAxis = i == 0 ? FVector(0.0, 1.0, 0.0) : FVector(0.0, -1.0, 0.0);
                
                FTransform EffectorTransform = Hierarchy->GetGlobalTransform(EffectorKey);
                FTransform ItemBTransform    = Hierarchy->GetGlobalTransform(ItemBKey);
                FTransform ItemATransform    = Hierarchy->GetGlobalTransform(ItemAKey);

                // --- Compute bone lengths from initial/bind-pose transforms ---
                FTransform InitialItemATransform        = Hierarchy->GetInitialGlobalTransform(ItemAKey);
                FTransform InitialItemBTransform        = Hierarchy->GetInitialGlobalTransform(ItemBKey);
                FTransform InitialEffectorTransform     = Hierarchy->GetInitialGlobalTransform(EffectorKey);

                // Scale-aware length computation (mirrors engine source exactly)
                FVector ScaleA = FVector::OneVector;
                if (InitialItemATransform.GetScale3D().SizeSquared() > SMALL_NUMBER)
                {
                    ScaleA = ItemATransform.GetScale3D() / InitialItemATransform.GetScale3D();
                }
                float LengthA = ((InitialItemATransform.GetLocation() - InitialItemBTransform.GetLocation()) * ScaleA).Size();

                FVector ScaleB = FVector::OneVector;
                if (InitialItemBTransform.GetScale3D().SizeSquared() > SMALL_NUMBER)
                {
                    ScaleB = ItemBTransform.GetScale3D() / InitialItemBTransform.GetScale3D();
                }
                float LengthB = ((InitialItemBTransform.GetLocation() - InitialEffectorTransform.GetLocation()) * ScaleB).Size();

                if (LengthA < SMALL_NUMBER || LengthB < SMALL_NUMBER)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Computed zero bone length for index %d, check bind pose"), i);
                    continue;
                }

                FControlRigMathLibrary::SolveBasicTwoBoneIK(
                    ItemATransform,
                    ItemBTransform,
                    EffectorTransform,
                    PoleVector,
                    PrimaryAxis,
                    SecondaryAxis,
                    1.0f,        
                    LengthA,     
                    LengthB,     
                    false,
                    0.75f,
                    1.25f
                );
                
                // Update transforms
                Hierarchy->SetGlobalTransform(ItemAKey, ItemATransform, bPropagateToChildren);
                Hierarchy->SetGlobalTransform(ItemBKey, ItemBTransform, bPropagateToChildren);
                // Note: Depending on your rig setup, you may want to update the effector transform as well
                // Hierarchy->SetGlobalTransform(EffectorKey, EffectorTransform);
            }
        }
    }