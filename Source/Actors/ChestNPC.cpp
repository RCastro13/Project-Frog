//
// Created by kayqu on 20/11/2025.
//

#include "ChestNPC.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"

ChestNPC::ChestNPC(Game* game)
    : NPCActor(game,
               "../Assets/Chest/chest.png",
               "../Assets/Chest/chest.json",
               {0, 1, 2, 3, 4, 5, 6},
               100,
               130)
{
    if (mAnimatorIdle) {
        mAnimatorIdle->SetLooping(false);
        mAnimatorIdle->SetIsPaused(true);
        mAnimatorIdle->SetAnimation("idle");
        mAnimatorIdle->SetFrameTime(0.6f); //0.6 segundos cada frame pois o audio de abrir bau tem 4 segundos e a animação do bau tem 7 frames -> 4/7 ~= 0.6

        //aplicando barulho de som
        mGame->GetAudio()->PlaySound("ChestOpening.mp3", false);
    }
}

ChestNPC::~ChestNPC()
{
    // O destrutor fica vazio, pois o NPCActor e o Actor
    // já cuidam de limpar os componentes.
}

void ChestNPC::Open() {
    if (mAnimatorIdle) {
        // Ativa e despausa para rodar a animação uma vez
        mAnimatorIdle->SetEnabled(true);
        mAnimatorIdle->SetLooping(false);
        mAnimatorIdle->SetIsPaused(false);
        mAnimatorIdle->SetAnimation("idle");
    }
}

bool ChestNPC::IsOpenFinished() const {
    if (mAnimatorIdle) {
        return mAnimatorIdle->IsFinished();
    }
    return false;
}