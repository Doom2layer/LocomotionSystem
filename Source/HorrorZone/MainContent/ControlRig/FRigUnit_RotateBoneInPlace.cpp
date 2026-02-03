// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/ControlRig/FRigUnit_RotateBoneInPlace.h"

FRigUnit_RotateBoneInPlace_Execute()
{
	URigHierarchy* Hierarchy = ExecuteContext.Hierarchy;
	if (Hierarchy)
	{
		const FRigElementKey Key(Bone, ERigElementType::Bone);
		if (!CachedBone.UpdateCache(Key, Hierarchy))
		{
			UE_CONTROLRIG_RIGUNIT_REPORT_WARNING(TEXT("Bone '%s' is not valid."), *Bone.ToString());
		}
		else
		{
			FTransform CurrentTransform = Hierarchy->GetGlobalTransform(CachedBone);

			FQuat CurrentRotation = CurrentTransform.GetRotation();
            
			FQuat NewRotation = AmountToRotate * CurrentRotation;

			CurrentTransform.SetRotation(NewRotation);
			Hierarchy->SetGlobalTransform(CachedBone, CurrentTransform);
		}
	}
}