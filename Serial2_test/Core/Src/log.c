#include "log.h"
#include "usart.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define LOG_BUF_SIZE   256
#define LOG_QUEUE_LEN  8

typedef struct {
    uint16_t len;
    char buf[LOG_BUF_SIZE];
} log_msg_t;

static QueueHandle_t log_queue;
static SemaphoreHandle_t log_mutex;

static log_msg_t log_pool[LOG_QUEUE_LEN];
static uint8_t log_idx = 0;
volatile uint8_t uart1_busy = 0;

void log_init(void)
{
    log_queue = xQueueCreate(LOG_QUEUE_LEN, sizeof(log_msg_t *));
    log_mutex = xSemaphoreCreateMutex();
}

static log_msg_t *log_alloc(void)
{
    log_msg_t *m = &log_pool[log_idx];
    log_idx = (log_idx + 1) % LOG_QUEUE_LEN;
    return m;
}

void log_printf(const char *fmt, ...)
{
    if (!log_queue) return;

    if (xSemaphoreTake(log_mutex, 0) != pdTRUE)
        return;

    log_msg_t *m = log_alloc();
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(m->buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);

    if (n < 0) n = 0;
    if (n > LOG_BUF_SIZE) n = LOG_BUF_SIZE;
    m->len = (uint16_t)n;

    xQueueSend(log_queue, &m, 0);
    xSemaphoreGive(log_mutex);
}

void log_task(void const *arg)
{
    log_msg_t *m;

    for (;;)
    {
        if (xQueueReceive(log_queue, &m, portMAX_DELAY) == pdTRUE)
        {
            while (uart1_busy)
                osDelay(1);

            uart1_busy = 1;
            HAL_UART_Transmit_DMA(&huart1, (uint8_t *)m->buf, m->len);
        }
    }
}

// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart->Instance == USART1)
//         uart1_busy = 0;
// }
