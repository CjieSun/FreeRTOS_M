/*
 * FreeRTOS ARM Cortex-M0 Demo
 * Simple multi-task test implementation
 */

#include "FreeRTOS.h"
#include "task.h"

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

int main(void)
{
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

/*
 * Task 1 - Lower priority task
 * Runs every 1000ms (1 second)
 */
static void vTask1(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(1000);

    /* Initialize the xLastWakeTime variable with the current time */
    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        /* Simulate LED1 toggle */
        led1_counter++;
        
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
    const TickType_t xFrequency = pdMS_TO_TICKS(500);

    /* Initialize the xLastWakeTime variable with the current time */
    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        /* Simulate LED2 toggle */
        led2_counter++;
        
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
}