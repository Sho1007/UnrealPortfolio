// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/Magazine.h"

TObjectPtr<ABullet> AMagazine::Pop()
{
	if (IsEmpty()) return NULL;

	TObjectPtr<ABullet> Bullet = Bullets[Bullets.Num() - 1];
	Bullets.RemoveAt(Bullets.Num() - 1);

	return Bullet;
}