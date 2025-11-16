#include "CombatRenderer.h"
#include "CombatConstants.h"
#include "Card.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Font.h"
#include <cstdio>

CombatRenderer::CombatRenderer(Game* game)
    : mGame(game)
{
}

CombatRenderer::~CombatRenderer()
{
}

void CombatRenderer::RenderizarCarta(
    Vector2 posicao,
    Vector2 tamanho,
    Card* carta,
    bool selecionada,
    float brilho,
    const std::map<AttackType, Texture*>& texturasAtivas,
    const std::map<AttackType, Texture*>& texturasCooldown,
    Texture* iconeTempo)
{
    if (!carta) return;

    Renderer* renderer = mGame->GetRenderer();
    Texture* texturaCarta = nullptr;

    // Obter textura apropriada
    if (carta->IsAvailable())
    {
        auto it = texturasAtivas.find(carta->GetType());
        if (it != texturasAtivas.end())
            texturaCarta = it->second;
    }
    else
    {
        auto it = texturasCooldown.find(carta->GetType());
        if (it != texturasCooldown.end())
            texturaCarta = it->second;
    }

    // Renderizar textura da carta
    if (texturaCarta)
    {
        Vector3 tint(brilho, brilho, brilho);
        renderer->DrawTexture(
            posicao,
            tamanho,
            0.0f,
            tint,
            texturaCarta,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }

    // Renderizar poder ou ícone de cooldown
    if (carta->IsAvailable())
    {
        // Determinar cor do texto
        Vector3 corTexto = selecionada ?
            Vector3(1.0f, 0.84f, 0.0f) :  // Amarelo dourado
            Vector3(1.0f, 1.0f, 1.0f);    // Branco

        // Determinar tamanho da fonte baseado no tamanho da carta
        int tamanhoFonte = (tamanho.x > 100.0f) ?
            CombatConstants::FontSizes::POWER_LARGE :
            CombatConstants::FontSizes::POWER_SMALL;

        // Determinar offset baseado no tamanho da carta
        float offsetBase = (tamanho.x > 100.0f) ?
            CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM_LARGE :
            CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM;

        RenderizarTextoPoder(
            Vector2(posicao.x, posicao.y + tamanho.y / 2.0f - offsetBase),
            carta->GetDamage(),
            corTexto,
            tamanhoFonte
        );
    }
    else if (iconeTempo)
    {
        // Renderizar ícone de cooldown
        float offsetBase = (tamanho.x > 100.0f) ?
            CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM_LARGE :
            CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM;

        renderer->DrawTexture(
            Vector2(posicao.x, posicao.y + tamanho.y / 2.0f - offsetBase - 4.0f),
            Vector2(CombatConstants::Sizes::COOLDOWN_ICON_SIZE,
                    CombatConstants::Sizes::COOLDOWN_ICON_SIZE),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            iconeTempo,
            Vector4::UnitRect,
            Vector2::Zero
        );
    }
}

void CombatRenderer::RenderizarTextoPoder(Vector2 posicao, int poder, Vector3 cor, int tamanhoFonte)
{
    if (!mGame->GetFont()) return;

    char texto[16];
    snprintf(texto, sizeof(texto), "%d", poder);

    Texture* textura = mGame->GetFont()->RenderText(texto, cor, tamanhoFonte);
    if (textura)
    {
        mGame->GetRenderer()->DrawTexture(
            posicao,
            Vector2(textura->GetWidth(), textura->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            textura,
            Vector4::UnitRect,
            Vector2::Zero
        );
        delete textura;
    }
}

void CombatRenderer::RenderizarTexturaSimples(Vector2 posicao, Vector2 tamanho, Texture* textura, float brilho)
{
    if (!textura) return;

    mGame->GetRenderer()->DrawTexture(
        posicao,
        tamanho,
        0.0f,
        Vector3(brilho, brilho, brilho),
        textura,
        Vector4::UnitRect,
        Vector2::Zero
    );
}

void CombatRenderer::RenderizarMolduraVencedor(Vector2 posicao, Vector2 tamanhoCard, Texture* moldura)
{
    if (!moldura) return;

    float frameWidth = tamanhoCard.x + CombatConstants::Offsets::WINNER_FRAME_EXTRA;
    float frameHeight = tamanhoCard.y + CombatConstants::Offsets::WINNER_FRAME_EXTRA;

    mGame->GetRenderer()->DrawTexture(
        Vector2(posicao.x, posicao.y + CombatConstants::Offsets::WINNER_FRAME_Y_OFFSET),
        Vector2(frameWidth, frameHeight),
        0.0f,
        Vector3(1.0f, 1.0f, 1.0f),
        moldura,
        Vector4::UnitRect,
        Vector2::Zero
    );
}

void CombatRenderer::RenderizarIconeCooldown(Vector2 posicao, float alturaCard, Texture* icone)
{
    if (!icone) return;

    float iconY = posicao.y + alturaCard / 2.0f - CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM - 4.0f;

    mGame->GetRenderer()->DrawTexture(
        Vector2(posicao.x, iconY),
        Vector2(CombatConstants::Sizes::COOLDOWN_ICON_SIZE, CombatConstants::Sizes::COOLDOWN_ICON_SIZE),
        0.0f,
        Vector3(1.0f, 1.0f, 1.0f),
        icone,
        Vector4::UnitRect,
        Vector2::Zero
    );
}

void CombatRenderer::RenderizarBarraHP(Vector2 posicao, int hpAtual, int hpMax, bool isInimigo)
{
    float percentual = (float)hpAtual / (float)hpMax;
    float larguraAtual = CombatConstants::Sizes::HP_BAR_WIDTH * percentual;

    Vector3 corHP = CalcularCorHP(percentual);

    RenderizarBarraFundo(posicao, CombatConstants::Sizes::HP_BAR_WIDTH, CombatConstants::Sizes::HP_BAR_HEIGHT);

    if (hpAtual > 0)
    {
        RenderizarBarraPreenchida(
            Vector2(posicao.x - (CombatConstants::Sizes::HP_BAR_WIDTH - larguraAtual) / 2.0f, posicao.y),
            larguraAtual,
            CombatConstants::Sizes::HP_BAR_HEIGHT,
            corHP
        );
    }

    RenderizarBorda(posicao, CombatConstants::Sizes::HP_BAR_WIDTH, CombatConstants::Sizes::HP_BAR_HEIGHT,
                    CombatConstants::Sizes::HP_BORDER_THICKNESS);

    RenderizarTextoHP(posicao, hpAtual, corHP);
}

Vector3 CombatRenderer::CalcularCorHP(float percentual)
{
    if (percentual > 0.6f)
        return Vector3(0.15f, 0.7f, 0.15f);  // Verde escuro
    else if (percentual > 0.3f)
        return Vector3(0.8f, 0.7f, 0.0f);    // Amarelo escuro
    else
        return Vector3(0.8f, 0.15f, 0.15f);  // Vermelho escuro
}

void CombatRenderer::RenderizarBarraFundo(Vector2 posicao, float largura, float altura)
{
    mGame->GetRenderer()->DrawRect(
        posicao,
        Vector2(largura, altura),
        0.0f,
        Vector3(0.15f, 0.15f, 0.15f),
        Vector2::Zero,
        RendererMode::TRIANGLES
    );
}

void CombatRenderer::RenderizarBarraPreenchida(Vector2 posicao, float largura, float altura, Vector3 cor)
{
    mGame->GetRenderer()->DrawRect(
        posicao,
        Vector2(largura, altura),
        0.0f,
        cor,
        Vector2::Zero,
        RendererMode::TRIANGLES
    );
}

void CombatRenderer::RenderizarBorda(Vector2 posicao, float largura, float altura, float espessura)
{
    Renderer* renderer = mGame->GetRenderer();
    Vector3 corBorda(1.0f, 1.0f, 1.0f);

    // Top
    renderer->DrawRect(
        Vector2(posicao.x, posicao.y - (altura + espessura) / 2.0f),
        Vector2(largura + espessura * 2, espessura),
        0.0f, corBorda, Vector2::Zero, RendererMode::TRIANGLES
    );

    // Bottom
    renderer->DrawRect(
        Vector2(posicao.x, posicao.y + (altura + espessura) / 2.0f),
        Vector2(largura + espessura * 2, espessura),
        0.0f, corBorda, Vector2::Zero, RendererMode::TRIANGLES
    );

    // Left
    renderer->DrawRect(
        Vector2(posicao.x - (largura + espessura) / 2.0f, posicao.y),
        Vector2(espessura, altura + espessura * 2),
        0.0f, corBorda, Vector2::Zero, RendererMode::TRIANGLES
    );

    // Right
    renderer->DrawRect(
        Vector2(posicao.x + (largura + espessura) / 2.0f, posicao.y),
        Vector2(espessura, altura + espessura * 2),
        0.0f, corBorda, Vector2::Zero, RendererMode::TRIANGLES
    );
}

void CombatRenderer::RenderizarTextoHP(Vector2 posicao, int hp, Vector3 cor)
{
    if (!mGame->GetFont()) return;

    char texto[32];
    snprintf(texto, sizeof(texto), "%d", hp);

    Texture* textura = mGame->GetFont()->RenderText(
        texto,
        cor,
        CombatConstants::FontSizes::HP_TEXT
    );

    if (textura)
    {
        float textY = posicao.y - (CombatConstants::Sizes::HP_BAR_HEIGHT / 2.0f) -
                     CombatConstants::Offsets::HP_TEXT_FROM_BAR;

        mGame->GetRenderer()->DrawTexture(
            Vector2(posicao.x, textY),
            Vector2(textura->GetWidth(), textura->GetHeight()),
            0.0f,
            Vector3(1.0f, 1.0f, 1.0f),
            textura,
            Vector4::UnitRect,
            Vector2::Zero
        );
        delete textura;
    }
}

