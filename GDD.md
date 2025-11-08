Trabalho Final - “Project Frog”

# Desenvolvimento de Jogos Digitais

## Facundo Sanchez - 2025551180

## Kayque Meira Siqueira - 2022043850

## Marcos Lott de Araújo - 2021039786

## Rubens da Cunha Castro - 2022422532

## Victor Vieira B. A. Pessoa - 2018054346

Departamento de Ciência da Computação - Universidade Federal de Minas Gerais (UFMG)

Belo Horizonte - MG - Brasil

# 1. Introdução

O seguinte documento apresenta o design e o processo de ideação do jogo “Project Frog”, ainda sem nome oficial mas que será apresentado como produto do trabalho prático final da disciplina de “Desenvolvimento de Jogos Digitais” da UFMG (Universidade Federal de Minas Gerais).

O jogo está sendo criado inspirado em diversos títulos dos gêneros de combate por turnos e dungeon crawler e suas referências principais são “Slay the Spire” (jogo de combate por turnos para um único jogador, baseado no uso de cartas), a saga “Shin Megami Tensei” (séries de jogos de combate por turnos que utilizam um “party” de personagens com habilidades) e também o mini-game “Card Jitsu” do jogo “Club Penguim” (jogo com combate de turnos utilizando cartas com elementos no formato “pedra, papel e tesoura”).

O jogo será um “Roguelike”, gênero caracterizado pela exploração de níveis gerados aleatoriamente com a morte permanente do jogador, ou seja, após o “Game Over”, o jogo deverá ser recomeçado do início novamente. Com relação ao número de jogadores, a ideia é que seja um jogo “single-player” com o progresso tendo como foco a correção de erros passados e o entendimento do mundo.

O que foi pensado com a produção desse jogo é, mesmo dentro do escopo pequeno de possibilidade de produção e tempo limitado, produzir um jogo que seja ao mesmo tempo completo, divertido e desafiador, para levar a sentimentos de vitória e êxtase durante sua jogatina. 2. Gameplay

Para jogar o jogo será necessário apenas o controle do botão “Enter” e das setas do teclado ou o conjunto de teclas “WASD”. Essas serão necessárias para realizar escolhas dentro do mapa e também para selecionar os ataques dentro do combate.

Uma “run” completa do jogo deve durar aproximadamente entre 10 e 20 minutos, com:

● 3-5 encontros de combate;

● Cada combate durando 2-4 minutos;

● Tempo entre combates para decisões de ataque;

O objetivo é não prolongar muito cada tentativa para permitir sessões de jogo curtas e engajantes, ideais para múltiplas tentativas e experimentação com diferentes estratégias. A seguir serão melhor descritos algumas características do jogo, bem como sua estrutura geral.

# 2.1 Narrativa e Ambientação:

A ideia inicial de narrativa para o jogo é a aventura de um sapo pelo seu mundo cheio de inimigos que o impedem de alcançar a vitória-régia lendária, que, segundo as lendas, é capaz de conceder um desejo à quem alcançá-la. O jogador assume o papel do animal aventureiro e deve prosseguir sua jornada em etapas que apresentam encontros únicos com criaturas que deve combater, tesouros, itens de magia e outros segredos. Ao vencer cada combate, o jogador progride no mapa até alcançar o chefe final.

A narrativa é minimalista e focada na progressão, permitindo que o jogador crie sua própria interpretação da jornada enquanto toma decisões estratégicas sobre qual caminho seguir.

# 2.2 Mecânicas Principais:

De maneira geral, o jogo vai possuir as seguintes mecânicas principais:

2.2.1 - Combate por Turnos com Cartas

O sistema de combate é baseado em turnos onde o jogador e o oponente jogam cartas simultaneamente. Inspirado no Card Jitsu do Club Penguin, o jogo utiliza um sistema de ataques que possuem um dano fixo somado a um valor aleatório que deverá ser obtido via rolagem de dados, então, por exemplo, um ataque com valor 4 + D6 pode ter ser dano variando entre 5 e 10. Além disso, os ataques vão assumir tipos diferentes que deverão interagir entre si para anular danos.

Inicialmente os tipos pensados são: → Fogo: As cartas assumem a cor vermelha, anulam cartas verdes e são anuladas pelas azuis;

→ Água: As cartas assumem a cor azul, anulam cartas vermelhas e são anuladas pelas verdes;

→ Planta: As cartas assumem a cor verde, anulam cartas azuis e são anuladas pelas vermelhas;

→ Neutra: As cartas assumem a cor branca e não anulam e nem são anuladas por nenhuma outra;

Cada carta possui um valor numérico que determina o vencedor quando ambos jogam cartas da mesma cor. A carta de maior valor vence e a diferença entre os valores será o dano causado ao inimigo ou recebido naquele turno. Alguns exemplos de interação do combate estão na Figura 1.

Para além dos ataques, o jogador também poderá escolher se defender, que resulta em receber o ataque total do inimigo caso ele ataque com a redução de um dado valor fixo referente ao escudo do dano final.

> Figura 1: Exemplo de Interação do combate

2.2.2 - “Deck” Building

O jogador contará sempre com 4 cartas fixas que ele poderá usar no início de cada combate. Essas cartas podem ser modificadas ao receber uma nova, seja via item, seja via compra na loja. É importante sempre se preparar antes de entrar em cada combate.

Todo ataque possui os seguintes atributos:

→ Valor fixo: valor fixo garantido do ataque; → Valor variável: valor variável que será somado ao valor fixo para gerar o valor final do ataque. Esse valor é representado por um dado de N lados, como por exemplo D4, D6, D8, D12 e D20;

→ Cooldown: número que representa a quantidade de turnos que o jogador deverá esperar o mesmo ataque novamente após utilizá-lo uma vez. Por exemplo, se o cooldown for 2 e o ataque é utilizado no turno 2, ele só poderá ser utilizado novamente no turno 5;

2.2.3 - Sistema de Vida

Com relação aos pontos de vida do jogador e como eles vão determinar sua vitória ou derrota nos combates foi determinado o seguinte:

● Jogador e inimigos começam com seus respectivos totais de pontos de vida (HP);

● Perder uma rodada causa dano baseado no valor da carta vencedora;

● Empates não causam dano;

● O combate termina quando um dos participantes chega a 0 HP;

# 2.3 Objetivo do Jogador:

O objetivo principal é alcançar a última vitória-régia, vencendo todos os encontros pelo caminho. Para o jogador sempre ser relembrado, o mapa será claro com as representações de cada caminho e com desenhos que identificam bem o que será encontrado ao longo de cada trilha. Além disso, será bem evidente que todos os caminhos vão convergir para avitória-régia lendária. Um exemplo do mapa será mostrado na seção de arte do jogo.

# 2.4 Início e Fim do Jogo:

Com relação ao funcionamento ao iniciar e finalizar, o jogo, assim que aberto, deverá exibir para o jogador um menu inicial com as opções de iniciar jogo e sair do jogo. Ao iniciar o jogo, o jogador vai visualizar o mapa com as possibilidades de caminho e deve fazer sua escolha inicial para onde ir. Ao fim de cada encontro, o jogador voltará para a tela com o mapa e deverá fazer sua próxima escolha, mesmo que exista apenas uma possibilidade para onde ir. O fluxo de telas do jogo está descrito na Figura 2.

Todos os caminhos levarão a apenas um chefe final, localizado na última casa do mapa e ao completar esse encontro, o jogador finaliza o jogo e será redirecionado ao menu inicial. Além disso, caso perca durante algum combate intermediário, isto é, ter seus pontos de vida zerados, o jogador vai se deparar com uma tela de “Game Over” e também será redirecionado para a tela inicial. Figura 2: Fluxo de Telas

# 2.5 Sistema de Progressão

2.5.1 - Progressão no Mapa

O mapa é apresentado como uma árvore de nós conectados, similar ao jogo “Slay the Spire”, que é a principal inspiração para o mapa e a progressão pensadas. No jogo o mapa vai conter as seguintes possibilidades:

● Nós de Combate (maioria): Enfrenta um inimigo;

● Nós de Evento: Situações especiais que oferecem recompensas ou desafios;

● Nós de Elite: Combates mais difíceis com melhores recompensas;

● Nós de Loja: Jogador consegue acessar a loja de itens;

● Nó de Chefe: Confronto final do jogo;

O jogador escolhe seu caminho, criando diferentes experiências a cada partida. Ao longo do mapa, quanto mais perto o jogador chegar do chefe final, mais difíceis serão os combates. Após cada luta, caso o jogador vença, ele receberá uma recompensa por vencer.

2.5.2 - Habilidades Aprimoradas ao longo do jogo

Como o jogo é proposto no formato de um “ Roguelike” , essencialmente a cada tentativa do jogador de vencer o jogo ele ganhará mais conhecimento sobre o escopo do mundo do jogo, e isso envolve: detalhes sobre os inimigos e seus ataques, entendimento do melhor sequenciamento de suas habilidades, capacidade de escolher o melhor caminho no mapa para maximizar sua chance de vitória, entre outros. De modo geral, a principal habilidade a ser entendida pelo jogador para vencer o jogo é a capacidade de gerenciar seus recursos e habilidades e manejo de escolhas para manipular a probabilidade de vitória a seu favor.

# 3. Arte

# 3.1. Visão Geral

O Jogo será desenvolvido em 2D, a câmera estará em uma perspectiva frontal lateral, semelhante à utilizada em “ Slay the Spire” e nos minigames de duelo do Dojo do Club Penguin. Durante os combates, o personagem principal ficará posicionado à esquerda da tela, enquanto os inimigos aparecerão à direita. A interface exibirá as quatro habilidades principais do jogador na parte inferior central da tela, representadas por cards estilizados.

A inspiração principal para os personagens e elementos de jogo é a clareza e a simplicidade do Club Penguin: Desafio Ninja (Card-Jitsu), enquanto a composição da cena de batalha e a estrutura do mapa seguirão a funcionalidade e a perspectiva de Slay the Spire .A arte do jogo irá ser ser, acima de tudo, legível, garantindo que o jogador possa entender rapidamente as informações na tela para tomar decisões estratégicas.

# 3.2. Pilares Artísticos

> O

estilo visual do jogo buscará equilibrar simplicidade e carisma, combinando elementos de pixel art estilizados com animações suaves e cores vivas que reforçam a temática de fantasia natural. A escolha da pixel art visa facilitar a produção, sem abrir mão de um estilo visual coeso e reconhecível.

O cenário das batalhas será composto por planos bidimensionais com cenários de estilo não definidos ainda, mas que remeterão à pântanos e ambientes fantasiosos que compactuam com a narrativa do personagem — ambientes que reforçam a identidade do jogo. A interface será minimalista e funcional, mantendo o foco no combate e na progressão do jogador. Durante as batalhas, o HUD exibirá:

● As quatro habilidades do sapo na parte inferior central, representadas por cards no formato de cartas com ícones ilustrativos com indicações de custo, valor e tempo de cooldown.

● As barras de vida do jogador e do inimigo próximas aos personagens.

● Ícones discretos no topo da tela para mostrar moedas, tempo de batalha e nome do cenário. .

O mapa seguirá o estilo roguelike ramificado , com caminhos que o jogador escolhe a cada etapa, inspirado no sistema de progressão de Slay the Spire . Cada nó do mapa representará uma nova vitória-régia, simbolizando o avanço do personagem principal em sua jornada. Usaremos ícones simples para representar os diferentes tipos de encontro (combate, evento, elite, loja e chefe) em cada vitória-régia que serão conectados por linhas, sobre um fundo neutro que remete à temática do jogo. A cada avanço, uma animação que representa a movimentação do personagem nesse mapa será feita.

As animações serão sutis, com foco em expressividade e clareza visual — pequenos movimentos do personagem, efeitos de energia, magia, escudo e ataque durante o uso das habilidades e transições entre cenários. Os efeitos visuais das habilidades seguirão uma estética que representa a função da habilidade, mantendo coerência com a temática anfíbia e mística do personagem.

# 3.3. Referências Visuais

1)  Slay the Spire – para o layout de combate e design do mapa roguelike (Figuras 4, 5 e 6).

2)  Club Penguin: Card-Jitsu Dojo – para o formato de duelos e expressividade dos personagens (Figura 3).

> Figura 3 - Club Penguin: Card-Jitsu Dojo
> Figura 4 - Slay the Spire - Combate Figura 5 - Slay the Spire - Carta selecionada
> Figura 6 - Slay the Spire - Mapa

# 4. Música e Efeitos Sonoros

O estilo musical pensado para o jogo foi o de temática mística e misteriosa inspirada na trilha sonora de “ The Legend of Zelda” , mas também com influência musical de jogos como “ Don’t Starve” e “ Hollow Knight” com aspecto aventureiro e suave. Os efeitos sonoros serão obtidos de forma gratuita e/ou também sintetizados por computador se for cômodo para o desenvolvimento, mas sempre tentando seguir uma ideia de som que remete à natureza e aos animais.

Os cenários do jogo também serão contemplados com músicas diferentes que irão combinar com o seu propósito, por exemplo, ao lutar com o chefe, uma música mais “animada” será empregada, mas ao enfrentar inimigos mais fracos, uma outra música menos impactante seria mais condizente. O mesmo se aplicará para a Loja e o Mapa.

# 5. Tecnologias

Nosso jogo será desenvolvido na linguagem de programação “C++”, em conjunto com a biblioteca SDL. Serão utilizadas as ferramentas e as estruturas de código vistas em aula. O jogo foi pensado para ser jogado exclusivamente em PC (sistemas operacionais Windows, macOS e Linux).

Os recursos visuais (sprites, cenários etc.) serão, em sua maioria, obtidos de terceiros por meio de sites que oferecem assets gratuitos. Caso seja necessário desenvolver recursos visuais mais específicos para o jogo, serão utilizadas as ferramentas vistas em aula para o design de sprites (Aseprite ou Piskel) e para a criação de tilemaps (Tiled).

Os efeitos sonoros e a música serão obtidos, em sua maioria, de sites públicos de assets. Caso seja necessário criar efeitos mais específicos, serão utilizados programas de edição e estações de trabalho de áudio como o Audacity ou o FL Studio.

# 6. Marketing

O jogo foi concebido para um público afim a jogos de luta por turnos e para fãs do gênero roguelike. Ainda assim, por ter uma mecânica, em princípio, simples, o jogo não deve ser difícil para pessoas não tão familiarizadas com o gênero. O jogo se dirige tanto a jogadores que buscam partidas curtas e satisfatórias quanto àqueles que querem experimentar progressão um percurso com caminhos diversos ao longo da história.

Para sua promoção e divulgação, o jogo será apresentado em sua versão final ao final da disciplina de Desenvolvimento de Jogos Digitais e, posteriormente, compartilhado gratuitamente com os demais alunos da UFMG, por meio de um repositório no GitHub. 7. Cronograma