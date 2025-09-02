/*
 * FreeRTOS ARM Cortex-M0 Demo
 * Simple multi-task test implementation
 */

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

/* Task priorities */
#define TASK1_PRIORITY    (tskIDLE_PRIORITY + 1)
#define TASK2_PRIORITY    (tskIDLE_PRIORITY + 2)

/* Task stack sizes */
#define TASK_STACK_SIZE   128

/* Task function prototypes */
static void vTask1(void *pvParameters);
static void vTask2(void *pvParameters);

/* Simple LED toggle simulation variables */
static volatile uint32_t led1_counter = 0;
static volatile uint32_t led2_counter = 0;



#define TRACE (*(volatile unsigned char *)0x40000000)

// Forward declarations for linker script symbols
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

void _init_data(void) {
    volatile uint32_t *src = &_sidata;
    volatile uint32_t *dst = &_sdata;

    // Copy data section from Flash to RAM
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // Zero out the BSS section
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }
}

int _read(int file, char* ptr, int len) {
    return 0;
}

int _close(int fd){
    return 0;
}

int _fstat_r(int fd) {
    return 0;
}

int _lseek_r(struct _reent *ptr, FILE *fp, long offset, int whence){
    return 0;
}

int _isatty_r(struct _reent *ptr, int fd) {
    return 0;
}

int _write(int file, const char *ptr, int len) {
  int x;

  for (x = 0; x < len; x++) {
    TRACE = *ptr++;
  }

  return (len);
}

// Implement puts for printf support
int puts(const char *s) {
    const char *p = s;
    while (*p) {
        TRACE = *p++;
    }
    TRACE = '\n';  // puts automatically adds newline
    return 1;
}

// Implement putchar for printf support
int putchar(int c) {
    TRACE = (char)c;
    return c;
}

int main(void)
{
    //printf("FreeRTOS Main start\r\n");
    /* Create Task 1 */
    xTaskCreate(vTask1,                 /* Task function */
                "Task1",                /* Task name */
                TASK_STACK_SIZE,        /* Stack size */
                NULL,                   /* Parameters */
                TASK1_PRIORITY,         /* Priority */
                NULL);                  /* Task handle */

    /* Create Task 2 */
    xTaskCreate(vTask2,                 /* Task function */
                "Task2",                /* Task name */
                TASK_STACK_SIZE,        /* Stack size */
                NULL,                   /* Parameters */
                TASK2_PRIORITY,         /* Priority */
                NULL);                  /* Task handle */

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    for(;;)
    {
        /* Empty */
    }
}

/* Build and print "counter=<value>" using puts (no snprintf dependency). */
static void print_with_puts(const char *prefix, uint32_t v)
{
    char buf[32];
    int idx = 0;

    /* Prefix */
    while (prefix[idx] != '\0' && idx < (int)(sizeof(buf) - 1))
    {
        buf[idx] = prefix[idx];
        idx++;
    }

    /* Convert number to decimal */
    char num[11];
    int n = 0;
    if (v == 0)
    {
        num[n++] = '0';
    }
    else
    {
        while (v > 0 && n < (int)sizeof(num))
        {
            num[n++] = (char)('0' + (v % 10));
            v /= 10;
        }
    }

    /* Append digits in correct order */
    for (int j = n - 1; j >= 0 && idx < (int)(sizeof(buf) - 1); --j)
    {
        buf[idx++] = num[j];
    }

    buf[idx] = '\0';
    puts(buf);
}

/*
 * Task 1 - Lower priority task
 * Runs every 1000ms (1 second)
 */
static void vTask1(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(1);

    /* Initialize the xLastWakeTime variable with the current time */
    xLastWakeTime = xTaskGetTickCount();
    for(;;)
    {
        /* Simulate LED1 toggle */
        led1_counter++;
        print_with_puts("vTask1 ", led1_counter);

        /* Wait for the next cycle */
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/*
 * Task 2 - Higher priority task
 * Runs every 500ms
 */
static void vTask2(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(5);

    /* Initialize the xLastWakeTime variable with the current time */
    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        /* Simulate LED2 toggle */
        led2_counter++;
        print_with_puts("vTask2 ", led2_counter);

        /* Wait for the next cycle */
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/*
 * FreeRTOS hook functions
 */
void vApplicationMallocFailedHook(void)
{
    /* Called if a call to pvPortMalloc() fails */
    for(;;)
    {
        /* Empty */
    }
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    /* Called if a stack overflow is detected */
    (void) pcTaskName;
    (void) pxTask;
    
    for(;;)
    {
        /* Empty */
    }
}

void vApplicationIdleHook(void)
{
    /* Called on each cycle of the idle task */
    //printf("vApplicationIdleHook\r\n");
}