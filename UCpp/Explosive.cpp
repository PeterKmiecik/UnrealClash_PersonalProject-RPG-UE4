// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Main.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

AExplosive::AExplosive(){

	Damage = 15.f;

}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor) {
		AMain* Main = Cast<AMain>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Main || Enemy) 
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass );

			if (OverlapParticles) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}
			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);


}
