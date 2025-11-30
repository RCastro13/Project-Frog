#include "MenuTextureHelper.h"
#include "../Renderer/Font.h"
#include "../Renderer/Texture.h"
#include "../Math.h"

const float MenuTextureHelper::GOLD_R = 1.0f;
const float MenuTextureHelper::GOLD_G = 0.84f;
const float MenuTextureHelper::GOLD_B = 0.0f;

const float MenuTextureHelper::WHITE_R = 1.0f;
const float MenuTextureHelper::WHITE_G = 1.0f;
const float MenuTextureHelper::WHITE_B = 1.0f;

Texture* MenuTextureHelper::CreateMenuOption(Font* font, const std::string& text, bool isSelected, int fontSize, int maxWidth)
{
    if (!font) return nullptr;

    Vector3 color = isSelected ?
        Vector3(GOLD_R, GOLD_G, GOLD_B) :
        Vector3(WHITE_R, WHITE_G, WHITE_B);

    return font->RenderText(text, color, fontSize, maxWidth);
}

