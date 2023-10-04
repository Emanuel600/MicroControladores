/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "atraso.h"
#include "defPrincipais.h"
#include "NOKIA5110_fb.h"
#include "figuras.h"
#include "PRNG_LFSR.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .cb_mem = &defaultTaskControlBlock,
    .cb_size = sizeof(defaultTaskControlBlock),
    .stack_mem = &defaultTaskBuffer[0],
    .stack_size = sizeof(defaultTaskBuffer),
    .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

#define max_naves_inimigas	30
#define max_tiros_nave0		10
#define max_tiros_nave2		5

// max_acertos_nave2 deve ser menor que max_pontos_p_nave2
#define max_pontos_p_nave2	30		// maxima pontuacao para surgimento da nave2 (nave mae)
#define max_acertos_nave2	10		// n�mero de acertos para eliminar a nave2

#define flag_mov_nave2		0
#define flag_nave2_ativa	1
#define flag_desce_nave2	2

struct pontos_t nave0p;
struct pontos_t nave2p;
uint32_t FLAGS = 0, cont_tiros0 = 0, cont_naves1 = 0 ;
uint32_t tiros0XY[max_tiros_nave0][2], naves1XY[max_naves_inimigas][2];
uint32_t tiros2XY[max_tiros_nave2][2], cont_tiros2 = 0;
int32_t score = 0, score_nave2 = 0;


uint32_t ADC_buffer[2];
uint32_t valor_ADC[2];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
void StartDefaultTask(void* argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1) {
        valor_ADC[0] = ADC_buffer[0];
        valor_ADC[1] = ADC_buffer[1];
    }
}

//---------------------------------------------------------------------------------------------------
// Tarefa para atualizar periodicamente o LCD
void vTask_LCD_Print(void* pvParameters)
{
    while(1) {
        imprime_LCD();
    }
}
//---------------------------------------------------------------------------------------------------
void vTask_Pontuacao(void* pvParameters)
{
    uint32_t i, j, k;
    int32_t score2nave2 = 0;
    struct pontos_t p;

    p.x1 = 0; p.y1 = 0;
    p.x2 = 0; p.y2 = 0;
    p.x3 = 0; p.y3 = 0;

    while(1) {
        for(i = 0; i < max_tiros_nave0; i++) {
            if(tiros0XY[i][0] != 0) {	// se houver tiro verifica o acerto
                //------------------------------------------------------------------------------------
                for(j = 0; j < max_naves_inimigas; j++) { // teste para as naves filhas
                    if(naves1XY[j][0] != 0xFF) {	// se houver nave inimiga verifica o acerto
                        // verificacao para a nave1 (nave filha)
                        if((naves1XY[j][1] == (tiros0XY[i][1] - 3)) ||
                                (naves1XY[j][1] == (tiros0XY[i][1] - 2))) {	// verifica se esta no mesmo ponto Y (2 pixeis de largura)
                            for(k = 0; k < 8; k++) {	//varredura de teste em toda a extens�o da nave inimiga (8 pixeis)
                                if((naves1XY[j][0] + k) == (tiros0XY[i][0] + 1)) { // se esta no mesmo x, acertou!
                                    if(!tst_bit(FLAGS, flag_nave2_ativa)) { // so pontua com as naves filha se a nave mae nao estiver ativa
                                        score++;
                                    }

                                    // apaga tiro
                                    p.x1 = tiros0XY[i][0];
                                    p.y1 = tiros0XY[i][1];
                                    tiros0XY[i][0] = 0;

                                    if(cont_tiros0 > 0) {
                                        cont_tiros0--;
                                    }

                                    desenha_fig(&p, &apaga_tiro0f);

                                    // apaga nave inimiga
                                    p.x1 = naves1XY[j][0];
                                    p.y1 = naves1XY[j][1];
                                    naves1XY[j][0] = 0xFF;
                                    cont_naves1--;
                                    desenha_fig(&p, &apaga_nave1f);
                                }
                            }
                        }
                    }
                }// for de teste das naves filhas
                //------------------------------------------------------------------------------------
                // verifica se o o tiro atingiu a nave mae
                if(tst_bit(FLAGS, flag_nave2_ativa)) {
                    if(((tiros0XY[i][0] + 1) >= nave2p.x1) && ((tiros0XY[i][0] + 1) <= nave2p.x1 + 15)) { // verifica se estao no mesmo x
                        if((tiros0XY[i][1] >= nave2p.y1) && (tiros0XY[i][1] <= (nave2p.y1 + 8))) { //verifica se estao no mesmo y
                            // apaga tiro
                            p.x1 = tiros0XY[i][0];
                            p.y1 = tiros0XY[i][1];
                            tiros0XY[i][0] = 0;

                            if(cont_tiros0 > 0) {
                                cont_tiros0--;
                            }

                            desenha_fig(&p, &apaga_tiro0f);
                            score_nave2++;
                            if(score_nave2 > max_acertos_nave2) {
                                score_nave2 = 0;
                                clr_bit(FLAGS, flag_nave2_ativa);
                            }
                            if((score_nave2 % 5) == 0) { //a cada 5 tiros soma 1 no score
                                score++;
                            }
                        }
                    }
                }
            }
        }
        //------------------------------------------------------------------------------------
        if(tst_bit(FLAGS, flag_nave2_ativa)) {
            for(j = 0; j < max_tiros_nave2; j++) { // se tiro da nave2 atingiu a nave0
                if(tiros2XY[j][0] != 0) {	// se houver tiro verifica o acerto
                    if(((tiros2XY[j][0] + 2) >= nave0p.x1) && ((tiros2XY[j][0] + 2) <= (nave0p.x1 + 8))) { // se estao no mesmo x
                        if(((tiros2XY[j][1] + 5) >= nave0p.y1) && ((tiros2XY[j][1] + 6) <= (nave0p.y1 + 7))) { // se estao no mesmo y, acertou
                            if(cont_tiros2 > 0) {
                                cont_tiros2--;
                            }

                            p.x1 = tiros2XY[j][0];
                            p.y1 = tiros2XY[j][1];
                            tiros2XY[j][0] = 0;
                            score--;
                            if(score_nave2 > 0) {
                                score_nave2--;
                            }
                            desenha_fig(&p, &apaga_tiro2f);
                        }
                    }
                }
            }
            //---------------------------------------------------------------------------------
            // verifica se houve contato entra a nave2 e a nave0
            if(((nave0p.x1 > nave2p.x1) && (nave0p.x1 < (nave2p.x1 + 16))) || (((nave0p.x1 + 8) > nave2p.x1) &&
                            ((nave0p.x1 + 8) < (nave2p.x1 + 16)))) {
                if(((nave0p.y1 > nave2p.y1) && (nave0p.y1 < (nave2p.y1 + 9))) || (((nave0p.y1 + 7) > nave2p.y1) &&
                                ((nave0p.y1 + 7) < (nave2p.y1 + 9)))) {
                    score--;
                    vTaskDelay(50);
                }
            }

            //---------------------------------------------------------------------------------
        }
        //-------------------------------------------------------------------------------------
        // teste para choque entre nave0 e nave1 (jogador e nave filha)
        for(j = 0; j < max_naves_inimigas; j++) {
            if(naves1XY[j][0] != 0xFF) {	// se houver nave inimiga verifica o acerto
                // verificar se estao no mesmo X

                p.x1 = naves1XY[j][0];
                p.y1 = naves1XY[j][1];

                if(((nave0p.x1 <= p.x1) && (p.x1 <= (nave0p.x1 + 8))) || ((p.x1 <= nave0p.x1) && (nave0p.x1 <= (p.x1 + 6)))) {
                    // verificar o y
                    if(((p.y1 <= nave0p.y1) && (nave0p.y1 <= (p.y1 + 3))) || ((nave0p.y1 <= p.y1) && (p.y1 <= (nave0p.y1 + 7)))) {
                        score--;

                        // apaga nave inimiga
                        p.x1 = naves1XY[j][0];
                        p.y1 = naves1XY[j][1];
                        naves1XY[j][0] = 0xFF;
                        cont_naves1--;
                        desenha_fig(&p, &apaga_nave1f);

                    }
                }
            }
        }
        //---------------------------------------------------------------------------------------
        //a cada max_pontos_p_nave2 gera uma nave mae se esta nao estiver criada
        if((score >= (score2nave2 + max_pontos_p_nave2)) && (!tst_bit(FLAGS, flag_desce_nave2)) &&
                (!tst_bit(FLAGS, flag_nave2_ativa))) {
            set_bit(FLAGS, flag_desce_nave2);
            score2nave2 = score;
        }
        if(score <= (score2nave2 - max_pontos_p_nave2)) {
            score2nave2 = score;
        }
        //-------------------------------------------------------------------------

        escreve_Nr_Peq(60, 0, score, 6); // mostra at� 99999
        vTaskDelay(10);
    }
}
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
void vTask_Desloca_Nave2(void* pvParameters)	// nave m�e
{
    uint32_t i, movimento;

    nave2p.x1 = 0; nave2p.y1 = 0;
    nave2p.x2 = 0; nave2p.y2 = 0;
    nave2p.x3 = 0; nave2p.y3 = 0;

    while(1) {

        vTaskDelay(1);

        if(tst_bit(FLAGS, flag_desce_nave2)) {	//se a nave estiver ativa desce uma vez
            //descida da nave
            nave2p.x1 = 33; nave2p.y1 = 0;
            for(i = 0; i < 10; i++) { // descida
                nave2p.x3 = 17; nave2p.y3 = 1 + i;
                desenha_fig(&nave2p, &nave2f);
                vTaskDelay(100);
            }
            nave2p.x3 = 0; nave2p.y3 = 0;
            set_bit(FLAGS, flag_nave2_ativa);	// ativa o tiro e o movimento da nave 2, sera deastivada pela pontuacao
            clr_bit(FLAGS, flag_desce_nave2);
        } else if(tst_bit(FLAGS, flag_nave2_ativa)) {
            set_bit(FLAGS, flag_mov_nave2);

            movimento = prng_LFSR() % 8; // s�o 8 op��es de movimento

            while(tst_bit(FLAGS, flag_mov_nave2)) {
                switch(movimento) {
                // descida vertical
                case 0: if(nave2p.y1 < 38) {
                        nave2p.y1++;
                    } else {
                        clr_bit(FLAGS, flag_mov_nave2);
                    }
                    break;

                // subida vertical
                case 1: if(nave2p.y1 > 0) {
                        nave2p.y1--;
                    } else {
                        clr_bit(FLAGS, flag_mov_nave2);
                    }
                    break;

                // movimento horizontal para a direita
                case 2: if(nave2p.x1 < 68) {
                        nave2p.x1++;
                    } else {
                        clr_bit(FLAGS, flag_mov_nave2);
                    }
                    break;

                // movimento horizontal para a esquerda
                case 3: if(nave2p.x1 > 0) {
                        nave2p.x1--;
                    } else {
                        clr_bit(FLAGS, flag_mov_nave2);
                    }
                    break;

                // movimento para a direita e para baixo
                case 4: if((nave2p.x1 < 68) && (nave2p.y1 < 38)) {
                        nave2p.x1++;
                        nave2p.y1++;
                    } else {
                        clr_bit(FLAGS, flag_mov_nave2);
                    }
                    break;

                // movimento para a esquerda e para cima
                case 5: if((nave2p.x1 > 0) && (nave2p.y1 > 0)) {
                        nave2p.x1--;
                        nave2p.y1--;
                    } else {
                        clr_bit(FLAGS, flag_mov_nave2);
                    }
                    break;

                // movimento para a esquerda e para baixo
                case 6: if((nave2p.x1 > 0) && (nave2p.y1 < 38)) {
                        nave2p.x1--;
                        nave2p.y1++;
                    } else {
                        clr_bit(FLAGS, flag_mov_nave2);
                    }
                    break;

                // movimento para a direita e para cima
                case 7: if((nave2p.x1 < 68) && (nave2p.y1 < 0)) {
                        nave2p.x1++;
                        nave2p.y1--;
                    } else {
                        clr_bit(FLAGS, flag_mov_nave2);
                    }
                    break;
                }
                if(tst_bit(FLAGS, flag_nave2_ativa)) {
                    desenha_fig(&nave2p, &nave2f);
                    vTaskDelay(80);
                } else {
                    desenha_fig(&nave2p, &apaga_nave2f);
                }

            }// while move nave2
        }//se nave2 ativa

    }// while(1)
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
void vTask_Disparo_Nave2(void* pvParameters)
{
    uint32_t  i, j;
    struct pontos_t tiro2p;
    tiro2p.x1 = 0; tiro2p.y1 = 0;
    tiro2p.x2 = 0; tiro2p.y2 = 0;
    tiro2p.x3 = 0; tiro2p.y3 = 0;

    for(i = 0; i < max_tiros_nave2; i++)
        for(j = 0; j < 2; j++) {
            tiros2XY[i][j] = 0;
        }

    j = 0; // auxilio na impressao correta

    while(1) {
        if(tst_bit(FLAGS, flag_nave2_ativa)) { // se nave mae estiver ativa
            if(j == 0) {	// para evitar a impressao sucessiva de um tiro em cima do outro
                if(cont_tiros2 < max_tiros_nave2) {	// limite m�ximo de 5 tiros ativos
                    tiro2p.x1 = nave2p.x1 + 6;
                    tiro2p.y1 = nave2p.y1 + 7;

                    if(tiro2p.y1 < 38) { // impedir impressao na parte inferior da tela quando a nave esta baixa
                        tiro2p.y1 = nave2p.y1 + 7;

                        for(i = 0; i < max_tiros_nave2; i++) {
                            if(tiros2XY[i][0] == 0) {
                                tiros2XY[i][0] = tiro2p.x1; // salva ponto x do tiro
                                tiros2XY[i][1] = tiro2p.y1; // salva ponto y do tiro
                                cont_tiros2++;
                                break;		// sai do for, ou seja, cria somente um tiro
                            }
                        }
                    }
                }
            }
            j++;
            if(j > 18) {
                j = 0;    // pelo menos 9 pixeis j� estarao impressos
            }
        }
        // imprime e desloca os tiros criados
        for(i = 0; i < max_tiros_nave2; i++) {
            if(tiros2XY[i][0] != 0) {	//se for diferente de zero tem tiro para ser impresso
                tiro2p.x1 = tiros2XY[i][0];
                tiro2p.y1 = tiros2XY[i][1];

                if(tiro2p.y1 < 48) {	// imprime tiro
                    tiro2p.y1 = tiros2XY[i][1];
                    tiro2p.y1++;
                    tiros2XY[i][1] = tiro2p.y1;

                    desenha_fig(&tiro2p, &tiro2f);
                } else { // apaga tiro
                    cont_tiros2--;
                    tiro2p.y1 = tiros2XY[i][1];
                    tiros2XY[i][0] = 0;	// libera vaga para novo tiro
                    desenha_fig(&tiro2p, &apaga_tiro2f);
                }
            }
        }
        vTaskDelay(50);
    }//while
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
void vTask_Desloca_Nave1(void* pvParameters)	// naves em queda livre
{
    uint32_t naves_conjuntas, i, j, posic_inic;
    int32_t  naves_mesma_linha[3];
    uint32_t dif1, dif2, cont_espaco = 0, cont_pixelsY[max_naves_inimigas];

    struct pontos_t nave1p;

    nave1p.x1 = 0; nave1p.y1 = 0;
    nave1p.x2 = 0; nave1p.y2 = 0;
    nave1p.x3 = 0; nave1p.y3 = 0;

    for(i = 0; i < max_naves_inimigas; i++) {
        naves1XY[i][0] = 0xFF;
        naves1XY[i][1] = 0;
        cont_pixelsY[i] = 0;
    }

    while(1) {
        // cria no m�ximo 3 naves ao mesmo tempo, se estiverem em posicao sobreposta n�o cria
        if(cont_espaco > 9) { // deve criar a cada 9 deslocamentos me y
            cont_espaco = 0;
            naves_conjuntas = (prng_LFSR() % 3) + 1;	// no m�ximo 3 naves criadas ao mesmo tempo e pelo menos 1 sempre
            //atraso_us(1);								// no minimo 84 ciclos de clock para novo sorteio aleat�rio

            if(cont_naves1 < max_naves_inimigas) {
                posic_inic = prng_LFSR() % 77;
                //atraso_us(1);
                naves_mesma_linha[0] = posic_inic;		// cria uma nave do conjunto
                cont_naves1++;

                for(i = 1; i < 3; i++) {
                    naves_mesma_linha[i] = 0xFF;    // um valor qualquer (> 76) para excluir a impressao
                }

                if(naves_conjuntas > 1) {
                    if(cont_naves1 < max_naves_inimigas) {
                        posic_inic = prng_LFSR() % 77;
                        //atraso_us(1);
                        dif1 = abs(naves_mesma_linha[0] - posic_inic);

                        if(dif1 > 7) {
                            naves_mesma_linha[1] =  posic_inic;
                            cont_naves1++;

                            if(naves_conjuntas > 2) {
                                if(cont_naves1 < max_naves_inimigas) {
                                    posic_inic = prng_LFSR() % 77;
                                    //atraso_us(1);
                                    dif1 = abs(naves_mesma_linha[1] - posic_inic);
                                    dif2 = abs(naves_mesma_linha[0] - posic_inic);

                                    if((dif1 > 7) && (dif2 > 7)) {
                                        naves_mesma_linha[2] = posic_inic;
                                        cont_naves1++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // carrega as naves criadas no vetor com todas as naves
            j = 0;
            for(i = 0; i < max_naves_inimigas; i++) {
                if(naves1XY[i][0] == 0xFF) { // se tem vaga para salvar a nova nave
                    naves1XY[i][0] = (uint32_t) naves_mesma_linha[j];
                    naves1XY[i][1] = (prng_LFSR() % 5); // alguma aleatoriedade em y
                    //atraso_us(1);
                    j++;
                }
                if(j == naves_conjuntas) {
                    break;
                }
            }
        }
        // -----------------------------------------------------------------------------------
        // Imprime as naves e movimenta
        // -----------------------------------------------------------------------------------
        for(i = 0; i < max_naves_inimigas; i++) {
            if(naves1XY[i][0] != 0xFF) {
                nave1p.y1 = naves1XY[i][1];
                nave1p.x1 = naves1XY[i][0];

                if(cont_pixelsY[i] == 0) {
                    nave1p.x3 = 7; nave1p.y3 = 1;
                    desenha_fig(&nave1p, &nave1f);
                    cont_pixelsY[i]++;
                } else if(cont_pixelsY[i] == 1) {
                    nave1p.x3 = 7; nave1p.y3 = 2;
                    desenha_fig(&nave1p, &nave1f);
                    cont_pixelsY[i]++;
                } else if(cont_pixelsY[i] == 2) {
                    nave1p.x3 = 7; nave1p.y3 = 3;
                    desenha_fig(&nave1p, &nave1f);
                    cont_pixelsY[i]++;
                } else {
                    nave1p.x3 = 0; nave1p.y3 = 0;
                    desenha_fig(&nave1p, &nave1f);

                    nave1p.y1++;
                    naves1XY[i][1] = nave1p.y1;
                    cont_pixelsY[i]++;

                    if(nave1p.y1 > 47) {	//libera espaco para nova nave inimiga
                        cont_pixelsY[i] = 0;
                        naves1XY[i][0] = 0xFF;
                        naves1XY[i][1] = 0;
                        cont_naves1--;
                        score--;
                    }
                }
            }
        }
        cont_espaco++;
        vTaskDelay(150);
    } // while
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
void vTask_Disparo_Nave0(void* pvParameters)
{
    uint32_t i, j;
    struct pontos_t tiro0p;
    tiro0p.x1 = 0; tiro0p.y1 = 0;
    tiro0p.x2 = 0; tiro0p.y2 = 0;
    tiro0p.x3 = 0; tiro0p.y3 = 0;

    for(i = 0; i < max_tiros_nave0; i++)
        for(j = 0; j < 2; j++) {
            tiros0XY[i][j] = 0;
        }

    j = 0; // auxilio na impressao correta

    while(1) {
        if((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15))) { // leitura do botao
            if(j == 0) {	// para evitar a impressao sucessiva de um tiro em cima do outro
                if(cont_tiros0 < max_tiros_nave0) {	// limite m�ximo de 10 tiros ativos
                    tiro0p.x1 = nave0p.x1 + 3;
                    tiro0p.y1 = nave0p.y1;

                    if(tiro0p.y1 > 4) { // impedir impressao no topo da tela, problemas para y = 0
                        tiro0p.y1 = nave0p.y1 - 3;

                        for(i = 0; i < max_tiros_nave0; i++) {
                            //xSemaphoreTake(mutexTiro,portMAX_DELAY);
                            if(tiros0XY[i][0] == 0) {
                                tiros0XY[i][0] = tiro0p.x1; // salva ponto x do tiro
                                tiros0XY[i][1] = tiro0p.y1; // salva ponto y do tiro
                                cont_tiros0++;
                                //	xSemaphoreGive(mutexTiro);
                                break;		// sai do for, ou seja, cria somente um tiro
                            }
                        }
                    }
                }
            }
            j++;
            if(j > 6) {
                j = 0;    // pelo menos 6 pixeis j� estarao impressos
            }
        }
        // imprime e desloca os tiros criados
        for(i = 0; i < max_tiros_nave0; i++) {
            //xSemaphoreTake(mutexTiro,portMAX_DELAY);
            if(tiros0XY[i][0] != 0) {	//se for diferente de zero tem tiro para ser impresso
                tiro0p.x1 = tiros0XY[i][0];
                tiro0p.y1 = tiros0XY[i][1];

                if(tiro0p.y1 > 1) {	// imprime tiro
                    tiro0p.y1 = tiros0XY[i][1];
                    tiro0p.y1--;
                    tiros0XY[i][1] = tiro0p.y1;
                    //	xSemaphoreGive(mutexTiro);
                    desenha_fig(&tiro0p, &tiro0f);
                } else { // apaga tiro
                    //xSemaphoreTake(mutexTiro,portMAX_DELAY);
                    if(cont_tiros0 > 0) {
                        cont_tiros0--;
                    }
                    tiro0p.y1 = tiros0XY[i][1];
                    tiros0XY[i][0] = 0;	// libera vaga para novo tiro
                    //xSemaphoreGive(mutexTiro);
                    desenha_fig(&tiro0p, &apaga_tiro0f);
                }
            }
        }
        vTaskDelay(50);
    }//while
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
void vTask_Desloca_Nave0(void* pvParameters)
{
    static uint32_t x = 38, y = 39;
    static int32_t dif_eixoX, dif_eixoY;

    nave0p.x2 = 0; nave0p.y2 = 0;
    nave0p.x3 = 0; nave0p.y3 = 0;

    while(1) {
        //escreve_Nr_Peq(10,20, media_eixoX, 10);
        //escreve_Nr_Peq(10,30, media_eixoY, 10);

        dif_eixoX = 2048 - valor_ADC[0];// media_eixoX;
        dif_eixoY = 2048 - valor_ADC[1];//media_eixoY;

        if(dif_eixoX < -200) {
            if(x > 0) {
                x--;
            }
        } else if(dif_eixoX > 200) { // 50 = Delta de tolerancia no deslocamento do eixo
            if(x < 75) {
                x++;
            }
        }
        // ------------------------------------------------------------------------
        if(dif_eixoY < -200) {
            if(y < 40) {
                y++;
            }
        }

        else if(dif_eixoY > 200) {
            if(y > 0) {
                y--;
            }
        }
        // ------------------------------------------------------------------------
        nave0p.x1 = x;  nave0p.y1 = y;

        desenha_fig(&nave0p, &nave0f);
        vTaskDelay(30);					// atraso para a nave n�o se movimentar muito r�pido.
        desenha_fig(&nave0p, &apaga_nave0f);
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    uint32_t semente_PRNG = 1;
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    /* USER CODE BEGIN 2 */

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_buffer, 2);
    HAL_ADC_Start_IT(&hadc1);

    // inicializa LCD 5110
    inic_LCD();
    limpa_LCD();

    // --------------------------------------------------------------------------------------
    // inicializa tela do jogo
    escreve2fb((unsigned char*)tela_inicial);
    imprime_LCD();

    HAL_Delay(1000);
    INVERTE_PIXELS();
    HAL_Delay(1000);
    NORMALIZA_PIXELS();
    HAL_Delay(1000);
    limpa_LCD();

    goto_XY(10, 1);
    string_LCD("Pressione o");
    goto_XY(25, 3);
    string_LCD("Botao!");
    imprime_LCD();

    while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15)) { // quando pressionar a tecla come�a o jogo
        semente_PRNG++;
    }


    init_LFSR(semente_PRNG);	// inicializacao para geracao de numeros pseudoaleatorios
    limpa_LCD();

    // -----------------------------------------

    /* USER CODE END 2 */

    /* Init scheduler */
    osKernelInitialize();

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    xTaskCreate(vTask_LCD_Print, "LCD_print", 128, NULL, osPriorityNormal, NULL);
    xTaskCreate(vTask_Pontuacao, "pontuacao", 128, NULL, osPriorityNormal, NULL);
    xTaskCreate(vTask_Disparo_Nave2, "nave mae", 128, NULL, osPriorityNormal, NULL);
    xTaskCreate(vTask_Desloca_Nave2, "nave mae", 128, NULL, osPriorityNormal, NULL);
    xTaskCreate(vTask_Desloca_Nave1, "nave minha", 256, NULL, osPriorityAboveNormal, NULL);
    xTaskCreate(vTask_Disparo_Nave0, "naves filhas", 128, NULL, osPriorityNormal, NULL);
    xTaskCreate(vTask_Desloca_Nave0, "naves filhas", 128, NULL, osPriorityNormal, NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while(1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Common config
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 2;
    if(HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
            | GPIO_PIN_7, GPIO_PIN_RESET);

    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PC15 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PA3 PA4 PA5 PA6
                             PA7 */
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
            | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void* argument)
{
    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    for(;;) {
        osDelay(1);
    }
    /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if(htim->Instance == TIM4) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while(1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
