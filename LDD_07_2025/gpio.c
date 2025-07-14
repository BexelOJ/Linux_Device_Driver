#include <stdio.h>

#define LED_GPIO 21

int main(){
if (!gpio_is_valid(LED_GPIO)) {
    printf("LED: Invalid GPIO %d\n", LED_GPIO);
    //return 1;
}
}

