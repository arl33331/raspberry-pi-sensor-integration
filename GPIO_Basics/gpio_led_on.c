#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const char *chip_path = "/dev/gpiochip0";
    unsigned int offset = 17;

    struct gpiod_chip *chip;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_config;
    struct gpiod_line_request *request;

    /* Open GPIO chip 0 */
    chip = gpiod_chip_open(chip_path);
    if (!chip)
    {
        perror("gpiod_chip_open");
        return EXIT_FAILURE;
    }

    /* Create line settings */
    settings = gpiod_line_settings_new();
    if (!settings)
    {
        perror("gpiod_line_settings_new");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    /* GPIO17 = output */
    if (gpiod_line_settings_set_direction(
            settings,
            GPIOD_LINE_DIRECTION_OUTPUT) < 0)
    {
        perror("gpiod_line_settings_set_direction");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    /* Initial value = HIGH */
    if (gpiod_line_settings_set_output_value(
            settings,
            GPIOD_LINE_VALUE_ACTIVE) < 0)
    {
        perror("gpiod_line_settings_set_output_value");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    /* Create line configuration */
    line_config = gpiod_line_config_new();
    if (!line_config)
    {
        perror("gpiod_line_config_new");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    /* Apply settings to GPIO17 */
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

    /* Request GPIO17 */
    request = gpiod_chip_request_lines(
        chip,
        NULL,
        line_config);

    if (!request)
    {
        perror("gpiod_chip_request_lines");
        gpiod_line_config_free(line_config);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    printf("GPIO17 set HIGH.\n");

    /*
     * Release everything and exit.
     *
     * Whether the physical GPIO remains HIGH after release
     * is something we will observe on the actual Raspberry Pi.
     */
    gpiod_line_request_release(request);
    gpiod_line_config_free(line_config);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

    return EXIT_SUCCESS;
}