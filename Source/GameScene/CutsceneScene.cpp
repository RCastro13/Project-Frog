#include "CutsceneScene.h"
#include "MapScene.h"
#include "../Game.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Font.h"
#include "../Math.h"
#include <SDL.h>

CutsceneScene::CutsceneScene(Game* game)
    : GameScene(game)
    , mBackgroundTexture(nullptr)
    , mScrollOffset(0.0f)
    , mScrollSpeed(20.0f)
    , mSoundTimer(0.0f)
    , mKeyWasPressed(false)
    , mConfirming(false)
    , mSkipTexture(nullptr)
    , mTotalTextHeight(0.0f)
{
}

CutsceneScene::~CutsceneScene()
{
    for (auto* texture : mTextTextures)
    {
        if (texture)
        {
            delete texture;
        }
    }
    mTextTextures.clear();

    if (mSkipTexture)
    {
        delete mSkipTexture;
        mSkipTexture = nullptr;
    }
}

void CutsceneScene::Enter()
{
    mStateTime = 0.0f;
    mScrollOffset = 448.0f - 150.0f; // Começa abaixo da margem inferior (448 - 150 = 298)
    mConfirming = false;

    SDL_SetWindowTitle(mGame->GetWindow(), "Project Frog");

    if (mGame->GetAudio())
    {
        mCutsceneMusic = mGame->GetAudio()->PlaySound("CutsceneMusic.mp3", true);
    }

    // Carregar background da cutscene
    mBackgroundTexture = mGame->GetRenderer()->GetTexture("../Assets/Background/Menu/cutscene.png");

    // Criar todas as linhas de texto
    CreateTextLines();

    // Criar textura de skip
    if (mGame->GetFont())
    {
        mSkipTexture = mGame->GetFont()->RenderText(
            "[ Pressione ESC ou ENTER para pular ]",
            Vector3(1.0f, 0.84f, 0.0f), // Amarelo/dourado
            16,
            600
        );
    }
}

void CutsceneScene::CreateTextLines()
{
    // Definir todas as linhas do texto narrativo
    mNarrativeLines = {
        "Em uma floresta antiga e esquecida,",
        "existe uma lenda sussurrada pelos ventos...",
        "",
        "A 'Vitória-Régia Lendária',",
        "uma flor mística capaz de conceder",
        "um desejo a quem a alcançar.",
        "",
        "Muitos tentaram.",
        "Poucos sobreviveram.",
        "",
        "Você é um sapo aventureiro,",
        "armado apenas com sua coragem",
        "e um pequeno grimório de magias elementais.",
        "",
        "Seu caminho será perigoso:",
        "criaturas selvagens guardam os segredos da floresta,",
        "tesouros escondidos aguardam os corajosos,",
        "e cada escolha pode ser a diferença",
        "entre a glória e o esquecimento.",
        "",
        "Fogo queima Planta.",
        "Planta domina Água.",
        "Água apaga Fogo.",
        "",
        "Use sua sabedoria,",
        "colete novas magias,",
        "e prepare-se para o desafio final...",
        "",
        "A Vitória-Régia o espera.",
        "",
        "",
        "O pergaminho finaliza dizendo:",
        "=== COMANDOS PARA NÃO SE PERDER NESSE MUNDO ===",
        "",
        "Setas/WASD - Navegar",
        "ENTER - Confirmar",
        "ESC - Sair (quando possível)"
    };

    // Marcar quais linhas são especiais (douradas)
    mLineIsSpecial.resize(mNarrativeLines.size(), false);
    mLineIsSpecial[3] = true;  // "A Victoria-Regia Lendaria,"
    mLineIsSpecial[28] = true; // "A Victoria-Regia espera."
    mLineIsSpecial[32] = true; // "=== COMANDOS ==="

    Font* font = mGame->GetFont();
    if (!font) return;

    float lineHeight = 30.0f;
    mTotalTextHeight = 0.0f;

    // Criar texturas para cada linha
    for (size_t i = 0; i < mNarrativeLines.size(); i++)
    {
        const std::string& line = mNarrativeLines[i];

        if (line.empty())
        {
            mTextTextures.push_back(nullptr);
            mTotalTextHeight += lineHeight;
        }
        else
        {
            Vector3 color = mLineIsSpecial[i] ?
                Vector3(1.0f, 0.84f, 0.0f) :  // Dourado para linhas especiais
                Vector3(1.0f, 1.0f, 1.0f);     // Branco para texto normal

            Texture* texture = font->RenderText(line, color, 10, 600);
            mTextTextures.push_back(texture);
            mTotalTextHeight += lineHeight;
        }
    }
}

void CutsceneScene::Update(float deltaTime)
{
    mStateTime += deltaTime;
    UpdateFade(deltaTime);

    mSoundTimer += deltaTime; // Conta o tempo

    if (mSoundTimer >= 3.5f) // Se passou 2 segundos ou mais
    {
        //mGame->GetAudio()->PlaySound("ChangeOption.wav", false);
        mSoundTimer = 0.0f; // Reseta o timer para contar do zero novamente
    }

    // Scroll do texto para cima
    mScrollOffset -= mScrollSpeed * deltaTime;

    // Se o texto todo já passou, vai para o mapa
    if (mScrollOffset < -mTotalTextHeight - 100.0f && !mConfirming)
    {
        mConfirming = true;
        mGame->SetScene(new MapScene(mGame));
    }
}

void CutsceneScene::ProcessInput(const Uint8* keyState)
{
    if (ShouldBlockInput())
        return;

    // ESC ou ENTER para pular a cutscene
    if ((keyState[SDL_SCANCODE_ESCAPE] || keyState[SDL_SCANCODE_RETURN] || keyState[SDL_SCANCODE_SPACE]) && !mKeyWasPressed && !mConfirming)
    {
        mKeyWasPressed = true;
        mConfirming = true;
        mGame->SetScene(new MapScene(mGame));
        return;
    }

    // Reset do flag
    if (!keyState[SDL_SCANCODE_ESCAPE] && !keyState[SDL_SCANCODE_RETURN] && !keyState[SDL_SCANCODE_SPACE])
    {
        mKeyWasPressed = false;
    }
}

void CutsceneScene::RenderBackground()
{
    if (mBackgroundTexture)
    {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 224.0f),
            Vector2(640.0f, 448.0f),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f), // Cor normal do background
            mBackgroundTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }
    else
    {
        // Fallback: fundo preto
        mGame->GetRenderer()->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void CutsceneScene::Render()
{
    float lineHeight = 30.0f;
    float currentY = mScrollOffset;

    const float TOP_MARGIN = 120.0f;
    const float BOTTOM_MARGIN = 200.0f;
    const float VISIBLE_TOP = TOP_MARGIN;
    const float VISIBLE_BOTTOM = 448.0f - BOTTOM_MARGIN; // 298px

    // Renderizar cada linha de texto
    for (size_t i = 0; i < mTextTextures.size(); i++)
    {
        Texture* texture = mTextTextures[i];

        if (texture)
        {
            // Só renderizar se estiver dentro da área visível (entre as margens)
            // Adicionar um buffer de 50px para transições suaves
            if (currentY >= VISIBLE_TOP - 50.0f && currentY <= VISIBLE_BOTTOM + 50.0f)
            {
                mGame->GetRenderer()->DrawTexture(
                    Vector2(320.0f, currentY),
                    Vector2(texture->GetWidth(), texture->GetHeight()),
                    0.0f,
                    Vector3(1.0f, 1.0f, 1.0f),
                    texture,
                    Vector4::UnitRect,
                    Vector2::Zero
                );
            }
        }

        currentY += lineHeight;
    }

    // Renderizar instrução de skip no topo
    if (mSkipTexture)
    {
        mGame->GetRenderer()->DrawTexture(
            Vector2(320.0f, 30.0f),
            Vector2(mSkipTexture->GetWidth(), mSkipTexture->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            mSkipTexture,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    RenderFade();
}

void CutsceneScene::Exit()
{
    if (mGame->GetAudio())
    {
        mGame->GetAudio()->StopSound(mCutsceneMusic);
    }

    for (auto* texture : mTextTextures)
    {
        if (texture)
        {
            delete texture;
        }
    }
    mTextTextures.clear();

    if (mSkipTexture)
    {
        delete mSkipTexture;
        mSkipTexture = nullptr;
    }

    mBackgroundTexture = nullptr;
}

