#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timer.h"

TaskHandle_t task1_handle;
TaskHandle_t task2_handle;
TaskHandle_t task3_handle;

TimerHandle_t timer_handle;

void Timer_callback(TimerHandle_t timer_handle)
{
    printf("oneshot callback\n");
}

void task_1(void *arg)
{
    int count = 0;
    while (1)
    {
        count += 1;
        if (count > 3)
        {
            printf("Task 2 Resumed\n");
            vTaskResume(task2_handle);
            vTaskSuspend(task1_handle);
            count = 0;
        }
        printf("This is the First Task");
        vTaskDelay(1000);
    }
}

void task_2(void *arg)
{
    int count = 0;
    while (1)
    {
        count += 1;
        if (count > 4)
        {
            printf("Task 3 Resumed\n");
            vTaskResume(task3_handle);
            vTaskSuspend(task2_handle);
            count = 0;
        }
        printf("This is the Second Task");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void task_3(void *arg)
{
    timer_handle=xTimerCreate("oneshot",pdns_TO_TICKS(10000),pdFALSE,NULL,Timer_callback);
    printf("one shot call back\n");
    xTimerStart(timer_handle, 0);
    printf("task 3 is deleted\n");
    xTaskDelete(task3_handle);
}

void main()
{
    BaseType_t res1, res2, res3 , res4 ,res5;
    res1 = xTaskCreate(task_1, "First Task", 2048, NULL, 1, &task1_handle);
    res2 = xTaskCreate(task_2, "second Task", 2048, NULL, 2,&task2_handle);
    res3 = xTaskCreate(task_3, "third Task", 2048, NULL, 3, &task3_handle);
}