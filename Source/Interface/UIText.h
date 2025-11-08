//
// Created by Mi PC on 05/11/2025.
//

#pragma once
#include "../Math.h"
#include "../Renderer/Renderer.h"
#include <string>

class UIText {
public:
    void SetText(const std::string& name);
    void Draw(Renderer* renderer);
protected:
    std::string mText;
    class UIFont* mFont;
    Texture *mTextTexture;
    unsigned int mPointSize;
    unsigned int mWrapLength;
    Vector2 mPosition;
};