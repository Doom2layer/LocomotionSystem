// Fill out your copyright notice in the Description page of Project Settings.


#include "MainContent/ControlRig/FRigUnit_RotateAroundPoint.h"

FRigUnit_RotateAroundPoint_Execute()
{
	URigHierarchy* Hierarchy = ExecuteContext.Hierarchy;
	if (Hierarchy)
	{
		OutputTransform = FTransform(AmountToRotate * InputTransform.GetRotation(),
		AmountToRotate.RotateVector(InputTransform.GetTranslation() - PointToRotateAround) + PointToRotateAround,
		InputTransform.GetScale3D()
		);
	}
}