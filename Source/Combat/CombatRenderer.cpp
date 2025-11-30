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
    Texture* iconeTempo,
    bool mostrarInfoCooldown)
{
    if (!carta) return;

    Renderer* renderer = mGame->GetRenderer();
    Texture* texturaCarta = nullptr;

    // --- LÓGICA DE TEXTURA ---
    // Se não formos mostrar info de cooldown (ex: no clash),
    // forçamos o uso da textura ativa para ficar bonito
    bool usarTexturaAtiva = !mostrarInfoCooldown ? true : carta->IsAvailable();

    if (usarTexturaAtiva)
    {
        auto it = texturasAtivas.find(carta->GetType());
        if (it != texturasAtivas.end()) texturaCarta = it->second;
    }
    else
    {
        auto it = texturasCooldown.find(carta->GetType());
        if (it != texturasCooldown.end()) texturaCarta = it->second;
    }

    if (texturaCarta)
    {
        Vector3 tint(brilho, brilho, brilho);
        renderer->DrawTexture(
            posicao, tamanho, 0.0f, tint, texturaCarta,
            Vector4::UnitRect, Vector2::Zero
        );
    }

    // --- LÓGICA DE UI (TEXTOS E ÍCONES) ---

    // Configurações de layout
    bool isLargeCard = (tamanho.x > 100.0f);
    int tamanhoFontePoder = isLargeCard ?
        CombatConstants::FontSizes::POWER_LARGE : CombatConstants::FontSizes::POWER_SMALL;
    float offsetBottom = isLargeCard ?
        CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM_LARGE : CombatConstants::Offsets::POWER_TEXT_FROM_BOTTOM;

    // Se NÃO estamos mostrando cooldown (Situação: Clash no centro da tela)
    // Apenas mostramos o dano, independente do estado da carta
    if (!mostrarInfoCooldown)
    {
        RenderizarTextoGenerico(
            Vector2(posicao.x, posicao.y + tamanho.y / 2.0f - offsetBottom),
            carta->GetDamage(),
            Vector3(1.0f, 1.0f, 1.0f), // Sempre branco no clash
            tamanhoFontePoder
        );
        return; // Sai da função aqui, não desenha mais nada
    }

    // --- Se chegamos aqui, mostrarInfoCooldown é TRUE (Situação: Mão do Jogador) ---

    if (carta->IsAvailable())
    {
        // 1. Poder (Embaixo)
        Vector3 corTexto = selecionada ? Vector3(1.0f, 0.84f, 0.0f) : Vector3(1.0f, 1.0f, 1.0f);
        RenderizarTextoGenerico(
            Vector2(posicao.x, posicao.y + tamanho.y / 2.0f - offsetBottom),
            carta->GetDamage(),
            corTexto,
            tamanhoFontePoder
        );

        // 2. Cooldown Total (Topo)
        if (iconeTempo)
        {
            float iconSizeSmall = CombatConstants::Sizes::COOLDOWN_ICON_SIZE * 0.6f;
            float topY = posicao.y - tamanho.y / 2.0f + offsetBottom;

            renderer->DrawTexture(
                Vector2(posicao.x - 12.0f, topY - 50.0f),
                Vector2(iconSizeSmall, iconSizeSmall),
                0.0f, Vector3(1.0f, 1.0f, 1.0f), iconeTempo,
                Vector4::UnitRect, Vector2::Zero
            );

            RenderizarTextoGenerico(
                Vector2(posicao.x + 12.0f, topY - 50.0f),
                carta->GetCoolDown(),
                Vector3(0.6f, 0.8f, 1.0f),
                tamanhoFontePoder
            );
        }
    }
    else if (iconeTempo) // Carta em Cooldown
    {
        // 1. Ícone Grande (Centro-Baixo)
        Vector2 iconPos = Vector2(posicao.x, posicao.y + tamanho.y / 2.0f - offsetBottom - 4.0f);

        renderer->DrawTexture(
            iconPos,
            Vector2(CombatConstants::Sizes::COOLDOWN_ICON_SIZE, CombatConstants::Sizes::COOLDOWN_ICON_SIZE),
            0.0f, Vector3(1.0f, 1.0f, 1.0f), iconeTempo,
            Vector4::UnitRect, Vector2::Zero
        );

        // 2. Turnos Restantes (Sobre o ícone)
        RenderizarTextoGenerico(
            iconPos - Vector2(0.0f, 100.0f),
            carta->GetCurrentCooldown(),
            Vector3(1.0f, 0.2f, 0.2f),
            tamanhoFontePoder
        );
    }
}

void CombatRenderer::RenderizarTextoGenerico(Vector2 posicao, int valor, Vector3 cor, int tamanhoFonte)
{
    if (!mGame->GetFont()) return;

    char texto[16];
    snprintf(texto, sizeof(texto), "%d", valor);

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

// Mantenha o método antigo como um wrapper se precisar de compatibilidade,
// ou apenas apague o RenderizarTextoPoder antigo e renomeie no .h
void CombatRenderer::RenderizarTextoPoder(Vector2 posicao, int poder, Vector3 cor, int tamanhoFonte)
{
    RenderizarTextoGenerico(posicao, poder, cor, tamanhoFonte);
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

