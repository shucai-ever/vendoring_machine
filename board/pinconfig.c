#include "pinconfig.h"
#include "stm32f10x_cfg.h"
#include <string.h>

/* pin configure structure */
typedef struct 
{
    char name[16];
    GPIO_Group group;
    GPIO_Config config;
}PIN_CONFIG;


typedef enum
{
    AHB,
    APB1,
    APB2,
}PIN_BUS;

typedef struct
{
    PIN_BUS bus;
    uint32_t reset_reg;
    uint32_t enable_reg;
}PIN_CLOCK;


/* pin arrays */
PIN_CONFIG pins[] = 
{
    {"power", GPIOA, 8, GPIO_Speed_2MHz, GPIO_Mode_Out_PP},
    {"usart1_tx", GPIOA, 9, GPIO_Speed_50MHz, GPIO_Mode_AF_PP},
    {"usart1_rx", GPIOA, 10, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING},
    {"pms_reset", GPIOA, 11, GPIO_Speed_2MHz, GPIO_Mode_Out_PP},
    {"pms_set", GPIOA, 12, GPIO_Speed_2MHz, GPIO_Mode_Out_PP},
    {"sound", GPIOC, 1, GPIO_Speed_2MHz, GPIO_Mode_AIN},
    {"gp2y1050", GPIOC, 3, GPIO_Speed_2MHz, GPIO_Mode_AIN},
    {"voca", GPIOC, 2, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING},
    {"vocb", GPIOC, 6, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING},
    {"spi1_nss", GPIOA, 4, GPIO_Speed_50MHz, GPIO_Mode_Out_PP},
    {"spi1_sck", GPIOA, 5, GPIO_Speed_50MHz, GPIO_Mode_AF_PP},
    {"spi1_miso", GPIOA, 6, GPIO_Speed_50MHz, GPIO_Mode_AF_PP},
    {"spi1_mosi", GPIOA, 7, GPIO_Speed_50MHz, GPIO_Mode_AF_PP},
    {"lcd_bkl", GPIOB, 0, GPIO_Speed_2MHz, GPIO_Mode_Out_PP},
    {"lcd_dc", GPIOB, 1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP},
    {"lcd_rst", GPIOB, 2, GPIO_Speed_2MHz, GPIO_Mode_Out_PP},
    {"am2302", GPIOC, 0, GPIO_Speed_2MHz, GPIO_Mode_Out_PP},
#ifdef __I2C_HARDWARE
    {"i2c1_scl", GPIOB, 6, GPIO_Speed_50MHz, GPIO_Mode_AF_OD},
    {"i2c1_sda", GPIOB, 7, GPIO_Speed_50MHz, GPIO_Mode_AF_OD},
    {"i2c2_scl", GPIOB, 10, GPIO_Speed_50MHz, GPIO_Mode_AF_OD},
    {"i2c2_sda", GPIOB, 11, GPIO_Speed_50MHz, GPIO_Mode_AF_OD},
#else
    {"i2c1_scl", GPIOB, 6, GPIO_Speed_2MHz, GPIO_Mode_Out_OD},
    {"i2c1_sda", GPIOB, 7, GPIO_Speed_2MHz, GPIO_Mode_Out_OD},
    {"i2c2_scl", GPIOB, 10, GPIO_Speed_2MHz, GPIO_Mode_Out_OD},
    {"i2c2_sda", GPIOB, 11, GPIO_Speed_2MHz, GPIO_Mode_Out_OD},
#endif
};

/* clock arrays */
PIN_CLOCK pin_clocks[] = 
{
    {AHB, RCC_AHB_ENABLE_CRC, RCC_AHB_ENABLE_CRC},
    {APB2, RCC_APB2_RESET_IOPA, RCC_APB2_ENABLE_IOPA},
    {APB2, RCC_APB2_RESET_IOPB, RCC_APB2_ENABLE_IOPB},
    {APB2, RCC_APB2_RESET_IOPC, RCC_APB2_ENABLE_IOPC},
    {APB2, RCC_APB2_RESET_IOPD, RCC_APB2_ENABLE_IOPD},
    {APB2, RCC_APB2_RESET_USART1, RCC_APB2_ENABLE_USART1},
    {APB2, RCC_APB2_RESET_ADC1, RCC_APB2_ENABLE_ADC1},
    {APB2, RCC_APB2_RESET_SPI1, RCC_APB2_ENABLE_SPI1},
    {APB1, RCC_APB1_RESET_I2C1, RCC_APB1_ENABLE_I2C1},
    {APB1, RCC_APB1_RESET_I2C2, RCC_APB1_ENABLE_I2C2},
};

/**
 * @brief get pin configuration by name
 * @param pin name
 * @return pin configuration
 */
static const PIN_CONFIG *get_pinconfig(const char *name)
{
    uint32_t len = sizeof(pins) / sizeof(PIN_CONFIG);
    for(uint32_t i = 0; i < len; ++i)
    {
        if(strcmp(name, pins[i].name) == 0)
            return &pins[i];
    }
    
    return NULL;
}

/**
 * @brief init pins
 */
void pin_init(void)
{
    //config pin clocks
    uint32_t len = sizeof(pin_clocks) / sizeof(PIN_CLOCK);
    for(uint32_t i = 0; i < len; ++i)
    {
        switch(pin_clocks[i].bus)
        {
        case AHB:
            RCC_AHBPeripClockEnable(pin_clocks[i].enable_reg, TRUE);
            break;
        case APB1:
            RCC_APB1PeriphReset(pin_clocks[i].reset_reg, TRUE);
            RCC_APB1PeriphReset(pin_clocks[i].reset_reg, FALSE);
            RCC_APB1PeripClockEnable(pin_clocks[i].enable_reg, TRUE);
            break;
        case APB2:
            RCC_APB2PeriphReset(pin_clocks[i].reset_reg, TRUE);
            RCC_APB2PeriphReset(pin_clocks[i].reset_reg, FALSE);
            RCC_APB2PeripClockEnable(pin_clocks[i].enable_reg, TRUE);
            break;
        default:
            break;
        }
    }
    
    /* config pins */
    len = sizeof(pins) / sizeof(PIN_CONFIG);
    for(uint32_t i = 0; i < len; ++i)
    {
        GPIO_Setup(pins[i].group, &pins[i].config);
    }
}

/**
 * @brief set pin
 * @param pin name
 */
void pin_set(const char *name)
{
    const PIN_CONFIG *config = get_pinconfig(name);
    assert_param(config != NULL);
    GPIO_SetPin(config->group, config->config.pin);

}

/**
 * @brief reset pin
 * @param pin name
 */
void pin_reset(const char *name)
{
    const PIN_CONFIG *config = get_pinconfig(name);
    assert_param(config != NULL);
    GPIO_ResetPin(config->group, config->config.pin);
}

/**
 * @brief check if pin is set
 * @param pin name
 */
bool is_pinset(const char *name)
{
    const PIN_CONFIG *config = get_pinconfig(name);
    assert_param(config != NULL);
    if(GPIO_ReadPin(config->group, config->config.pin) != 0)
        return TRUE;
    else
        return FALSE;
}

/**
 * @brief get pin information
 * @param pin name
 * @param pin group
 * @param pin number
 */
void get_pininfo(const char *name, uint8_t *group, uint8_t *num)
{
    assert_param(name != NULL);
    assert_param(group != NULL);
    assert_param(num != NULL);
    const PIN_CONFIG *config = get_pinconfig(name);
    assert_param(config != NULL);
    *group = config->group;
    *num = config->config.pin;
}

