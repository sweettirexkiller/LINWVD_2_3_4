#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <periphery/gpio.h>

#define LED_PIN1 24
#define LED_PIN2 23
#define LED_PIN3 27

#define BUTTON_PIN1 10
#define BUTTON_PIN2 17
#define BUTTON_PIN3 18

void wait_for_button_press(gpio_t *button_gpio, struct timeval *start, struct timeval *end) {
    bool value;

    // Czekaj na naciśnięcie przycisku
    while (true) {
        gpio_read(button_gpio, &value);
        if (value == false) { // Przycisk naciśnięty (założenie: stan niski)
            gettimeofday(end, NULL);
            break;
        }
    }
}

int main(void) {
    gpio_t *gpio_led1, *gpio_led2, *gpio_led3;
    gpio_t *gpio_button1, *gpio_button2, *gpio_button3;
    struct timeval start, end;

    // Inicjalizacja GPIO dla LED
    gpio_led1 = gpio_new();
    gpio_led2 = gpio_new();
    gpio_led3 = gpio_new();

    // Inicjalizacja GPIO dla przycisków
    gpio_button1 = gpio_new();
    gpio_button2 = gpio_new();
    gpio_button3 = gpio_new();

    // Otwarcie pinów GPIO dla LED jako wyjście
    if (gpio_open(gpio_led1, "/dev/gpiochip0", LED_PIN1, GPIO_DIR_OUT) < 0) {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(gpio_led1));
        exit(1);
    }
    if (gpio_open(gpio_led2, "/dev/gpiochip0", LED_PIN2, GPIO_DIR_OUT) < 0) {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(gpio_led2));
        exit(1);
    }
    if (gpio_open(gpio_led3, "/dev/gpiochip0", LED_PIN3, GPIO_DIR_OUT) < 0) {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(gpio_led3));
        exit(1);
    }

    // Otwarcie pinów GPIO dla przycisków jako wejście
    if (gpio_open(gpio_button1, "/dev/gpiochip0", BUTTON_PIN1, GPIO_DIR_IN) < 0) {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(gpio_button1));
        exit(1);
    }
    if (gpio_open(gpio_button2, "/dev/gpiochip0", BUTTON_PIN2, GPIO_DIR_IN) < 0) {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(gpio_button2));
        exit(1);
    }
    if (gpio_open(gpio_button3, "/dev/gpiochip0", BUTTON_PIN3, GPIO_DIR_IN) < 0) {
        fprintf(stderr, "gpio_open(): %s\n", gpio_errmsg(gpio_button3));
        exit(1);
    }

    // Mierzenie czasu reakcji dla przycisku 1
    printf("Naciśnij przycisk odpowiadający diodzie LED1\n");
    gpio_write(gpio_led1, true);
    gettimeofday(&start, NULL);
    wait_for_button_press(gpio_button1, &start, &end);
    gpio_write(gpio_led1, false);

    long reaction_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Czas reakcji dla przycisku 1: %ld ms\n", reaction_time);

    // Mierzenie czasu reakcji dla przycisku 2
    printf("Naciśnij przycisk odpowiadający diodzie LED2\n");
    gpio_write(gpio_led2, true);
    gettimeofday(&start, NULL);
    wait_for_button_press(gpio_button2, &start, &end);
    gpio_write(gpio_led2, false);

    reaction_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Czas reakcji dla przycisku 2: %ld ms\n", reaction_time);

    // Mierzenie czasu reakcji dla przycisku 3
    printf("Naciśnij przycisk odpowiadający diodzie LED3\n");
    gpio_write(gpio_led3, true);
    gettimeofday(&start, NULL);
    wait_for_button_press(gpio_button3, &start, &end);
    gpio_write(gpio_led3, false);

    reaction_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Czas reakcji dla przycisku 3: %ld ms\n", reaction_time);

    // Zamknięcie pinów GPIO
    gpio_close(gpio_led1);
    gpio_close(gpio_led2);
    gpio_close(gpio_led3);
    gpio_close(gpio_button1);
    gpio_close(gpio_button2);
    gpio_close(gpio_button3);

    // Zwolnienie zasobów GPIO
    gpio_free(gpio_led1);
    gpio_free(gpio_led2);
    gpio_free(gpio_led3);
    gpio_free(gpio_button1);
    gpio_free(gpio_button2);
    gpio_free(gpio_button3);

    return 0;
}
