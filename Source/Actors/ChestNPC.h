//
// Created by kayqu on 20/11/2025.
//

#ifndef PROJECT_FROG_CHESTACTOR_H
#define PROJECT_FROG_CHESTACTOR_H

#pragma once
#include "NPCActor.h"

class ChestNPC : public NPCActor {
public:
    ChestNPC(Game* game);
    ~ChestNPC();

    void Open();
    bool IsOpenFinished() const;
};


#endif //PROJECT_FROG_CHESTACTOR_H