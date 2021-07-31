#pragma once

#include <iostream>

enum class WeaponType {
	Knife = 0,
	Pistol,
	SubMachinegun,
	Rifle,
	Shotgun,
	SniperRifle,
	Machinegun,
	C4,
	Placeholder,
	Grenade,
	Unknown,
	StackableItem,
	Fists,
	BreachCharge,
	BumpMine,
	Tablet,
	Melee
};

struct WeaponInfo {
	std::byte pad_0000[20];
	int maxClip;
	std::byte pad_0001[112];
	const char* name;
	std::byte pad_0002[60];
	WeaponType type;
	std::byte pad_0003[4];
	int price;
	std::byte pad_0004[8];
	float cycletime;
	std::byte pad_0005[12];
	bool fullAuto;
	std::byte pad_0006[3];
	int damage;
	float armorRatio;
	int bullets;
	float penetration;
	std::byte pad_0007[8];
	float range;
	float rangeModifier;
	std::byte pad_0008[16];
	bool silencer;
	std::byte pad_0009[15];
	float maxSpeed;
	float maxSpeedAlt;
	std::byte pad_000A[100];
	float recoilMagnitude;
	float recoilMagnitudeAlt;
	std::byte pad_000B[16];
	float recoveryTimeStand;
};