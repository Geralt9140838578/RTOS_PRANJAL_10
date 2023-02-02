#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

TaskHandle_t task1_handle;
TaskHandle_t task2_handle;
TaskHandle_t task3_handle;
TaskHandle_t task4_handle;
TaskHandle_t task5_handle;

QueueHandle_t queue;

void task_1(void *arg)
{
    int count = 0;
    UBaseType_t prio_1;
    prio_1 = uxTaskPriorityGet(task1_handle);
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
        printf("This is the First Task & Priority is %d\n", prio_1);
        vTaskDelay(1000);
    }
}

void task_2(void *arg)
{
    vTaskSuspend(task2_handle);
    int count = 0;
    UBaseType_t prio_2;
    prio_2 = uxTaskPriorityGet(task2_handle);
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
        printf("This is the Second Task & Priority is %d\n", prio_2);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void task_3(void *arg)
{
    vTaskSuspend(task3_handle);
    int count = 0;
    UBaseType_t prio_3;
    prio_3 = uxTaskPriorityGet(task3_handle);
    while (1)
    {
        count += 1;
        if (count > 5)
        {
            printf("Task 1 Resumed\n");
            vTaskResume(task4_handle);
            vTaskSuspend(task3_handle);
            count = 0;
        }
        printf("This is the Third Task & Priority is %d\n", prio_3);
        vTaskDelay(5000/ portTICK_PERIOD_MS);
    }
}


void task_4(void *arg)
{
   char txbuffer[50];
   printf("task forth is ready \n");
   queue = xQueueCreate(10,sizeof(txbuffer));
   if(queue==0)
   {
    printf("fail to make a queue\n");
   }
   sprintf(txbuffer,"hello from task 4");
   xQueueSend(queue,(void*)txbuffer,(TickType_t)0);
   sprintf(txbuffer,"i am from Cdac");
   xQueueSend(queue,(void*)txbuffer,(TickType_t)0);
   vTaskDelay(4000);
}

void task_5(void *arg)
{
    char rxbuffre[50];
    while(1)
    {
        if(xQueueReceive(queue,&rxbuffer,(TickType_t)10));
        {
        printf("RECEIVED MSG IS --> %s",rxbuffer);
        vTaskDelay(4000)
        }
    }
}

void main()
{
    BaseType_t res1, res2, res3 , res4 ,res5;
    res1 = xTaskCreate(task_1, "First Task", 2048, NULL, 1, &task1_handle);
    res2 = xTaskCreate(task_2, "second Task", 2048, NULL, 2,&task2_handle);
    res3 = xTaskCreate(task_3, "third Task", 2048, NULL, 3, &task3_handle);
    res4 = xTaskCreate(task_4, "forth Task", 2048, NULL, 4, &task4_handle);
    res5 = xTaskCreate(task_5, "Fifth Task", 2048, NULL, 5, &task5_handle);

}