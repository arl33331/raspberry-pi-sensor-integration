#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    const char *chip_path = "/dev/gpiochip0";
    unsigned int offset = 17;

    struct gpiod_chip *chip;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_config;
    struct gpiod_line_request *request;

    chip = gpiod_chip_open(chip_path);
    if (!chip)
    {
        perror("gpiod_chip_open");
        return EXIT_FAILURE;
    }

    settings = gpiod_line_settings_new();
    if (!settings)
    {
        perror("gpiod_line_settings_new");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    gpiod_line_settings_set_direction(
        settings,
        GPIOD_LINE_DIRECTION_OUTPUT
    );

    gpiod_line_settings_set_output_value(
        settings,
        GPIOD_LINE_VALUE_INACTIVE
    );

    line_config = gpiod_line_config_new();
    if (!line_config)
    {
        perror("gpiod_line_config_new");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

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

    request = gpiod_chip_request_lines(
        chip,
        NULL,
        line_config
    );

    if (!request)
    {
        perror("gpiod_chip_request_lines");
        gpiod_line_config_free(line_config);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    /* Blink 10 times */
    for (int i = 0; i < 10; i++)
    {
        gpiod_line_request_set_value(
            request,
            17,
            GPIOD_LINE_VALUE_ACTIVE
        );

        sleep(1);

        gpiod_line_request_set_value(
            request,
            17,
            GPIOD_LINE_VALUE_INACTIVE
        );

        sleep(1);
    }

    gpiod_line_request_release(request);
    gpiod_line_config_free(line_config);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

    return EXIT_SUCCESS;
}