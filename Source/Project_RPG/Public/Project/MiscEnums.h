#pragma once


UENUM(BlueprintType)
enum class ESoundNotifyType : uint8
{
	EST_HitReact UMETA(DisplayName = "HitReact"),
	EST_Footstep UMETA(DisplayName = "Footstep"),
	EST_EquipmentMove UMETA(DisplayName = "EquipmentMove"),
	EST_Death UMETA(DisplayName = "Death"),
	EST_Special UMETA(DisplayName = "Special")
};