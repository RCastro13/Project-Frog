//
// Created by Mi PC on 05/11/2025.
//

#pragma once
#include "../UIScreen.h"

class HUD : public UIScreen
{
public:
    HUD(class Game* game);
    ~HUD();
    void Update(float deltaTime);
    void Draw();
private:
    // EXEMPLO
    /*
    class UIImage* mRupeeIcon;
    class UIText* mRupeeCounter;
    class UIImage* mBombIcon;
    class UIText* mBombCounter;
    class UIImage* mArrowIcon;
    class UIText* mArrowCounter;
    */
    };