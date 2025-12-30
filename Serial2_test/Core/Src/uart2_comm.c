#include "uart2_comm.h"
#include "log.h"
#include "usart.h"
#include "cmsis_os.h"
#include <string.h>
#include "tim.h"

#define UART2_RX_DMA_BUF_SIZE 512
#define UART2_TX_QUEUE_LEN   8

#define FRAME_LEN 22

static uint8_t  rx_buf[256];
static uint16_t rx_len = 0;

static uint8_t rx_dma_buf[UART2_RX_DMA_BUF_SIZE];

typedef struct {
    uint16_t len;
    uint8_t  data[UART2_RX_DMA_BUF_SIZE];
} rx_block_t;

static rx_block_t rx_pool[UART2_TX_QUEUE_LEN];
static uint8_t rx_idx = 0;

static QueueHandle_t rx_queue;
static QueueHandle_t tx_queue;

volatile uint8_t uart2_tx_busy = 0;

// CRC
static uint8_t crc8_xor(const uint8_t *buf, uint16_t len)
{
    uint8_t c = 0;
    for (uint16_t i = 0; i < len; i++)
        c ^= buf[i];
    return c;
}

void set_moters_duty(uint16_t dutys[]);
// open the data
static void parse_uart2_frames(void)
{
    while (rx_len >= FRAME_LEN)
    {
        /* 找帧头 */
        if (rx_buf[0] != 0xAA || rx_buf[1] != 0x55) {
            memmove(rx_buf, rx_buf + 1, --rx_len);
            continue;
        }

        uint8_t type = rx_buf[2];
        uint8_t seq  = rx_buf[3];
        uint8_t len  = rx_buf[4];

        if (len != 16) {
            /* 非法帧，丢头 */
            memmove(rx_buf, rx_buf + 2, rx_len - 2);
            rx_len -= 2;
            continue;
        }

        /* CRC 校验 */
        uint8_t crc = rx_buf[21];
        if (crc != crc8_xor(rx_buf, 21)) {
            memmove(rx_buf, rx_buf + 2, rx_len - 2);
            rx_len -= 2;
            continue;
        }

        /* ===== 成功解析一帧 ===== */
        if (type == 0x01)   // PWM8
        {
            uint16_t pwm[8];
            for (int i = 0; i < 8; i++) {
                pwm[i] = rx_buf[5 + i*2]
                       | (rx_buf[6 + i*2] << 8);
            }

            /* 👉 更新 PWM */
            // pwm_apply(pwm);
            set_moters_duty(pwm);
            /* 👉 可选调试打印（数值！不是字符串） */
        }

        /* 移除已解析帧 */
        memmove(rx_buf, rx_buf + FRAME_LEN, rx_len - FRAME_LEN);
        rx_len -= FRAME_LEN;
    }
}

void set_moters_duty(uint16_t dutys[]) {
       HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
            log_printf("RX PWM: %u %u %u %u %u %u %u %u\r\n",
                       dutys[0], dutys[1], dutys[2], dutys[3],
                       dutys[4], dutys[5], dutys[6], dutys[7]);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, dutys[1]);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, dutys[3]);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, dutys[5]);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, dutys[6]);
    // __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 500);
    // __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 500);
    // __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 500);
    // __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 500);
}
/* ---------------- RX ---------------- */

static rx_block_t *rx_alloc(void)
{
    rx_block_t *b = &rx_pool[rx_idx];
    rx_idx = (rx_idx + 1) % UART2_TX_QUEUE_LEN;
    return b;
}

void uart2_comm_init(void)
{
    rx_queue = xQueueCreate(UART2_TX_QUEUE_LEN, sizeof(rx_block_t *));
    tx_queue = xQueueCreate(UART2_TX_QUEUE_LEN, sizeof(rx_block_t *));

    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart2, rx_dma_buf, UART2_RX_DMA_BUF_SIZE);
}

void uart2_idle_callback(void)
{
    uint16_t recv_len =
        UART2_RX_DMA_BUF_SIZE -
        __HAL_DMA_GET_COUNTER(huart2.hdmarx);

    if (recv_len == 0) return;

    HAL_UART_DMAStop(&huart2);

    rx_block_t *b = rx_alloc();
    b->len = recv_len;
    memcpy(b->data, rx_dma_buf, recv_len);

    BaseType_t hpw = pdFALSE;
    xQueueSendFromISR(rx_queue, &b, &hpw);

    memset(rx_dma_buf, 0, UART2_RX_DMA_BUF_SIZE);
    HAL_UART_Receive_DMA(&huart2, rx_dma_buf, UART2_RX_DMA_BUF_SIZE);

    portYIELD_FROM_ISR(hpw);
}

/* RX 解析任务（这里只是打印示例） */
void uart2_rx_task(void const *arg)
{
    rx_block_t *b;

    for (;;)
    {
        if (xQueueReceive(rx_queue, &b, portMAX_DELAY) == pdTRUE)
        {
            /* TODO：这里做协议解析 */
            /* 示例：原样回发 */
            // uart2_tx_send(b->data, b->len);
            log_printf("[UART2] RX %d bytes: \r\n", b->len);
            /* 1. 累积到解析缓冲 */
            if (rx_len + b->len > sizeof(rx_buf)) {
                rx_len = 0;   // overflow，直接清
                continue;
            }

            memcpy(rx_buf + rx_len, b->data, b->len);
            rx_len += b->len;

            /* 2. 解析协议帧 */
            parse_uart2_frames();
        }
    }
}

/* ---------------- TX ---------------- */

void uart2_tx_send(uint8_t *buf, uint16_t len)
{
    rx_block_t *b = rx_alloc();
    if (len > UART2_RX_DMA_BUF_SIZE) return;

    memcpy(b->data, buf, len);
    b->len = len;
    xQueueSend(tx_queue, &b, 0);
}

void uart2_tx_task(void const *arg)
{
    rx_block_t *b;

    for (;;)
    {
        if (xQueueReceive(tx_queue, &b, portMAX_DELAY) == pdTRUE)
        {
            while (uart2_tx_busy)
                osDelay(1);

            uart2_tx_busy = 1;
            HAL_UART_Transmit_DMA(&huart2, b->data, b->len);
        }
    }
}

// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == USART2)
//         uart2_tx_busy = 0;
// }
