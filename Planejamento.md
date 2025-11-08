1 - Cena Combate

1.1 - Fazer o personagem e o inimigo aparecem na tela;

1.2 - Fazer os ataques do personagem aparecem;

1.3 - Habilitar a seleção de um ataque;

1.4 - Fazer o inimigo escolher um ataque também; (tópico 2)

1.4.1 - Heuristica/Logica para o inimigo escolher um ataque (fazer escolha aleatória no início para facilitar)

1.5 - Logica para definir quem ganhou (quem vai atacar/quem será atacado)

1.5.1 - Olhar ataque selecionado do jogador e do inimigo e definir quem ganhou;

1.6 - Fazer animação de ataque;

1.7 - Atualizar vida de quem sofreu o ataque;

1.8 - Fazer um loop de tudo isso (turnos) até alguém perder

1.9 - Ao finalizar um combate:

1.9.1 - Em caso de vitória -> transição para o mapa

1.9.2 - Em caso de derrota -> transição para o menu

\


2 - Fazer Ataques

2.1 - Definir nome, tipo, custo e dano de cada ataque;

3 - Definir os Assets

3.1 - Selecionar assets do personagem

3.2 - Selecionar assets dos inimigos

3.3 - Selecionar assets do boss final

3.4 - Selecionar assets do background (cenário combate)

3.5 - Selecionar assets do mapa (fundo + ícones)

3.6 - Selecionar assets para o menu inicial

4 - Fazer o Mapa

4.1 - Fazer o mapa aparecer na tela junto com os ícones

4.2 - Habilitar a escolha do jogador da próxima casa

4.3 - Transição de tela entre mapa e nível

5 - Fazer o menu inicial

5.1 - Fazer o menu carregar ao abrir o jogo

5.2 - Implementar opção NOVO JOGO -> transição para o mapa

5.3 - Implementar opção SAIR DO JOGO -> fecha o jogo

5.4 - (OPCIONAL) Implementar opções do jogo (volume...)

\
\
\
\
\
\
\
\
\
\



## **DEFINIÇÃO DE SPRINTS**

### **Sprint 1 (CRÍTICO - Vertical Slice)**

1. Sistema de Cenas (0.2) - Lott

2. Menu Principal básico (5.1) - Kayque

3. Combate básico (1.1, 1.2, 1.3 básico) - Facundo

4. Assets temporários para testar - Rubens

5. Mapa básico - Rubens

**META: Conseguir iniciar jogo, ir para um combate e jogar 1 turno**


### **Sprint 2 (Core Gameplay)**

1. Completar sistema de combate (1.3 completo)

2. Sistema de cartas (2.1, 2.2)

3. Inimigos básicos (3.1 parcial, 3.2 básico)

**META: Completar um combate do início ao fim**


### **Sprint 3 (Loop do Jogo)**

1. Sistema de mapa (4.1, 4.2, 4.3 básico)

2. Fluxo completo (5.2, 5.3 básico)

3. Múltiplos inimigos

**META: Jogar uma run completa do início ao fim**


### **Sprint 4 (Conteúdo)**

1. Mais inimigos e balanceamento (3.1 completo, 3.2 completo)

2. Mais cartas (2.1 completo)

3. Boss e eventos (4.3 completo)

**META: Jogo completo e jogável**


### **Sprint 5 (Arte & Áudio)**

1. Substituir assets temporários (6.1, 6.2, 6.3)

2. Adicionar música e SFX (7.1, 7.2)

**META: Jogo com identidade visual/sonora**


### **Sprint 6 (Polimento)**

1. Balanceamento final (8.1)

2. Juice e feedback (8.2)

3. QoL features (8.3)

4. Testes e bugs

**META: Jogo pronto para apresentação**


## **DEFINIÇÃO DO PROJETO**

### **0.1 Arquitetura Core (COMPLETO)**

1. ✅ Sistema de Game Loop

2. ✅ Sistema de Atores e Componentes

3. ✅ Sistema de Renderização (OpenGL/SDL)

4. ✅ Sistema de Input

5. ✅ Sistema de UI (UIScreen, UIButton, UIText, UIImage)


### **0.2 Sistema de Cenas (PENDENTE)**

1. Implementar GameScene completo (atualmente comentado)

2. Criar enum de estados: MAIN\_MENU, MAP, COMBAT, GAME\_OVER, VICTORY

3. Implementar transições entre cenas

4. Sistema de carregamento/descarregamento de dados por cena


## **FASE 1: SISTEMA DE COMBATE**

### **1.1 Classes Base do Combate**

1. Criar classe `Card` (ataque)

2. Atributos: nome, tipo (Fogo/Água/Planta/Neutro), valorFixo, dado (D4/D6/D8/D12/D20), cooldown

3. Métodos: CalcularDano(), GetTipo(), IsDisponivel()

4. Criar classe `Combatant` (base para Player e Enemy)

5. Atributos: nome, vida, vidaMaxima, deck (4 cartas), cooldowns ativos

6. Métodos: TakeDamage(), Heal(), SelectCard(), GetAvailableCards()

7. Criar classe `Player` herda de `Combatant`

8. Adicionar inventário, moedas, cartas desbloqueadas

9. Criar classe `Enemy` herda de `Combatant`

10. Implementar IA para seleção de cartas

11. Diferentes tipos de inimigos com decks diferentes

12. Criar classe `CombatManager`

13. Gerenciar turnos

14. Resolver combate (comparar cartas, calcular dano)

15. Detectar vitória/derrota


### **1.2 Lógica de Combate**

1. Implementar sistema de tipos (Fogo > Planta > Água > Fogo)

2. Implementar comparação de cartas do mesmo tipo

3. Implementar rolagem de dados para dano variável (opcional)

4. Implementar sistema de cooldown

5. Implementar opção de defesa (escudo)

6. Implementar sistema de turnos

\



### **1.3 Cena de Combate (CombatScene)**

1. Criar classe `CombatScene` herda de `GameScene`

2. Posicionar Player à esquerda

3. Posicionar Enemy à direita

4. Exibir background de combate

5. Exibir 4 cartas do player na parte inferior

6. Exibir barras de vida

7. Implementar seleção de carta com WASD/Setas + Enter

8. Implementar animação de ataque

9. Implementar efeitos visuais (dano, cura, escudo)

10. Implementar feedback visual de vantagem de tipo

11. Implementar tela de vitória do combate

12. Implementar tela de derrota do combate


## **FASE 2: SISTEMA DE CARTAS & PROGRESSÃO**

### **2.1 Biblioteca de Cartas**

1. Definir conjunto inicial de cartas do player (4 básicas)

2. Criar 15-20 cartas diferentes balanceadas

3. Distribuir cartas por tipo: Fogo, Água, Planta, Neutro

4. Criar arquivo JSON com dados das cartas


### **2.2 Deck Building**

1. Criar tela de escolha de carta (após vitória)

2. Implementar sistema de troca/upgrade de cartas

3. Limitar deck a 4 cartas

4. Salvar deck atual do player


## **FASE 3: INIMIGOS & BALANCEAMENTO**

### **3.1 Tipos de Inimigos**

1. Criar 3-5 inimigos normais

2. Criar 1 boss final

3. Definir vida, deck e comportamento de cada


### **3.2 IA dos Inimigos**

1. Implementar seleção aleatória básica

2. Implementar IA intermediária (considera tipo do player) (aleatorio primeiro - para simplificar)

3. Implementar IA avançada para elites/boss (aleatorio primeiro - para simplificar)

4. Balancear dificuldade progressiva

\



## **FASE 4: SISTEMA DE MAPA**

### **4.1 Classes do Mapa**

1. Criar classe `MapNode`

2. Tipos: COMBAT, EVENT, SHOP, REST, BOSS

3. Estado: LOCKED, AVAILABLE, COMPLETED

4. Criar classe `MapPath` (conexão entre nós)

5. Criar classe `MapGenerator`

6. Gerar árvore de nós estilo Slay the Spire

7. 3-4 colunas de nós até boss

8. Garantir pelo menos 3-5 combates


### **4.2 Cena do Mapa (MapScene)**

1. Criar classe `MapScene` herda de `GameScene`

2. Renderizar background (pântano/fantasia)

3. Renderizar todos os nós com ícones

4. Renderizar caminhos entre nós

5. Destacar nós disponíveis

6. Implementar seleção com WASD/Setas

7. Implementar confirmação com Enter

8. Animação de movimento do player no mapa

9. Transição para combate ao selecionar nó


### **4.3 Tipos de Encontros**

1. Implementar nó de COMBATE normal

2. Implementar nó de LOJA (comprar cartas)

3. Implementar nó de EVENTO (escolhas especiais)

4. (OPCIONAL) Implementar nó de DESCANSO (recuperar vida)

5. Implementar nó de BOSS


## **FASE 5: MENU & FLUXO DO JOGO**

### **5.1 Menu Principal**

1. Criar classe `MainMenuScene` herda de `GameScene`

2. Adicionar background do menu

3. Botão "NOVO JOGO" → inicia MapScene

4. Botão "SAIR" → fecha o jogo

5. (OPCIONAL) Botão "OPÇÕES" → volume, controles

6. (OPCIONAL) Título animado do jogo

\
\



### **5.2 Telas de Fim**

1. Criar tela de GAME OVER

2. Mostrar causa da derrota

3. Botão voltar ao menu

4. Criar tela de VITÓRIA

5. Mensagem de parabenização

6. Estatísticas da run (turnos, dano, etc)

7. Botão voltar ao menu


### **5.3 Sistema de Recompensas**

1. Tela de recompensa pós-combate

2. Escolher nova carta OU

3. Melhorar carta existente OU

4. Ganhar moedas

5. Sistema de moedas

6. Loja de cartas


## **FASE 6: ARTE & ASSETS**

### **6.1 Sprites de Personagens**

1. Sprite do Player (sapo)

2. Sprites de 3-5 inimigos normais

3. Sprite do boss final

4. Animações idle

5. Animações de ataque

6. Animações de dano/morte


### **6.2 Sprites de UI**

1. Design das cartas (4 variações de tipo)

2. Ícones de tipos (Fogo, Água, Planta, Neutro)

3. Ícones do mapa (combate, elite, loja, evento, boss)

4. Barra de vida

5. Indicadores de cooldown

6. Cursor/seleção


### **6.3 Backgrounds**

1. Background do menu principal

2. Background de combate (2-3 variações)

3. Background do mapa

4. Tela de vitória

5. Tela de game over

\



### **6.4 Efeitos Visuais**

1. Efeito de ataque de fogo

2. Efeito de ataque de água

3. Efeito de ataque de planta

4. Efeito de ataque neutro

5. Efeito de escudo/defesa

6. Partículas de dano

7. Transições de cena


## **FASE 7: ÁUDIO**

### **7.1 Música**

1. Música do menu principal

2. Música de combate normal (temática aventuresca)

3. Música de combate com boss (temática épica)

4. Música do mapa (temática misteriosa)

5. Música de vitória

6. Música de game over


### **7.2 Efeitos Sonoros**

1. Som de seleção de UI

2. Som de confirmação

3. Som de carta sendo jogada

4. Som de ataque por tipo (4 sons)

5. Som de dano recebido

6. Som de defesa/escudo

7. Som de vitória de turno

8. Som de derrota de turno

9. Som de morte do inimigo

10. Sons ambiente (pântano, natureza)


## **FASE 8: POLIMENTO & EXTRAS**

### **8.1 Balanceamento**

1. Testar e balancear todas as cartas

2. Ajustar vida dos inimigos

3. Ajustar dificuldade progressiva

4. Testar diferentes estratégias


### **8.2 Feedback & Juice**

1. Screenshake em ataques poderosos

2. Slow motion em momentos críticos

3. Partículas e efeitos visuais extras

4. Animações de transição suaves

5. Tooltips explicativos


### **8.3 Qualidade de Vida**

1. Tutorial/explicação de mecânicas

2. Indicador de vantagem de tipo

3. Calculadora de dano antes de jogar

4. Histórico de turnos

5. Opções de volume

6. Confirmação antes de decisões importantes


### **8.4 (OPCIONAL) Conteúdo Extra**

1. Múltiplos personagens jogáveis

2. Mais cartas e inimigos

3. Sistema de conquistas

4. Estatísticas globais

5. Diferentes finais
