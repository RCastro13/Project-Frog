#pragma once
#include <string>

class Font;
class Texture;
class Vector3;

class MenuTextureHelper
{
public:
    static Texture* CreateMenuOption(Font* font, const std::string& text, bool isSelected, int fontSize = 22, int maxWidth = 400);

private:
    static const float GOLD_R;
    static const float GOLD_G;
    static const float GOLD_B;
    static const float WHITE_R;
    static const float WHITE_G;
    static const float WHITE_B;
};

