#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <gpiod.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO27 27

int main(void)
{
    struct gpiod_chip *chip;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_config;
    struct gpiod_line_request *request_gpio;

    unsigned int offset = GPIO27;

    /* NEW:
       Variable used to hold the value read from GPIO 27 */
    enum gpiod_line_value value;


    /* SAME AS PREVIOUS GPIO PROGRAMS:
       Open the GPIO chip */
    chip = gpiod_chip_open(GPIO_CHIP);

    if (!chip)
    {
        perror("gpiod_chip_open");
        return EXIT_FAILURE;
    }


    /* SAME AS PREVIOUS GPIO PROGRAMS:
       Create line settings */
    settings = gpiod_line_settings_new();

    if (!settings)
    {
        perror("gpiod_line_settings_new");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }


    /* NEW:
       Configure GPIO 27 as an INPUT */
    gpiod_line_settings_set_direction(
        settings,
        GPIOD_LINE_DIRECTION_INPUT
    );


    /* NEW:
       Enable the Raspberry Pi's internal pull-up resistor */
    gpiod_line_settings_set_bias(
        settings,
        GPIOD_LINE_BIAS_PULL_UP
    );


    /* SAME AS PREVIOUS GPIO PROGRAMS:
       Create the line configuration */
    line_config = gpiod_line_config_new();

    if (!line_config)
    {
        perror("gpiod_line_config_new");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }


    /* SAME AS PREVIOUS GPIO PROGRAMS:
       Add GPIO 27 to the configuration */
    if (gpiod_line_config_add_line_settings(
            line_config,
            &offset,
            1,
            settings) < 0)
    {
        perror("gpiod_line_config_add_line_settings");
        gpiod_line_config_free(line_config);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }


    /* SAME AS PREVIOUS GPIO PROGRAMS:
       Request control of GPIO 27 */
    request_gpio = gpiod_chip_request_lines(
        chip,
        NULL,
        line_config
    );

    if (!request_gpio)
    {
        perror("gpiod_chip_request_lines");
        gpiod_line_config_free(line_config);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }


    printf("GPIO 27 input test\n");
    printf("Open switch   = HIGH\n");
    printf("Closed switch = LOW\n");
    printf("Press Ctrl-C to stop.\n\n");


    /* NEW:
       Continuously read the GPIO 27 input */
    while (1)
    {
        value = gpiod_line_request_get_value(
            request_gpio,
            GPIO27
        );


        /* NEW:
           Determine whether the input is HIGH or LOW */
        if (value == GPIOD_LINE_VALUE_ACTIVE)
        {
            printf("GPIO 27 = HIGH   Switch OPEN\n");
        }
        else if (value == GPIOD_LINE_VALUE_INACTIVE)
        {
            printf("GPIO 27 = LOW    Switch CLOSED\n");
        }
        else
        {
            printf("Error reading GPIO 27\n");
        }

        sleep(1);
    }


    return EXIT_SUCCESS;
}