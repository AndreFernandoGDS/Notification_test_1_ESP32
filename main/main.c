#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_1 2
#define LED_2 4

static TaskHandle_t tasksHandler = NULL; // cria o hendler
int counter = 0;

void blink_task1(void * params){

    while(true){

        gpio_set_level(LED_1, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED_1, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        counter ++; //incrementa o counter

        if(counter == 5){
            xTaskNotifyGive(tasksHandler);
            counter = 0;
        }
    }

}

void blink_task2(void * params){

    while(true){

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Esperar pela notificação da Task1

        for(int i = 0; i <= 6; i++ ){
        	if(i ==5){
            gpio_set_level(LED_2, 1);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            gpio_set_level(LED_2, 0);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        	}
        }
    }

}

void app_main(void){

	esp_rom_gpio_pad_select_gpio(LED_1);
	gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);
	esp_rom_gpio_pad_select_gpio(LED_2);
	gpio_set_direction(LED_2, GPIO_MODE_OUTPUT);

    xTaskCreate(&blink_task1,"blink_task1",2048,NULL,2,&tasksHandler);
    xTaskCreate(&blink_task2,"blink_task2",2048,NULL,2,&tasksHandler);
}
