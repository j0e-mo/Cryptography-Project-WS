/******************************************************************************
 *
 * Module: UART1
 *
 * File Name: uart1.c
 *
 * Description: Source file for the UART1 TM4C123GH6PM driver
 *
 * Author: Yousouf Soliman
 *
 *******************************************************************************/
/*******************************************************************************
 *                             Inclusions                                      *
 *******************************************************************************/
#include <UART/uart.h>
#include "Service_Layer/tm4c123gh6pm_registers.h"

/*******************************************************************************
 *                   Static Functions Prototypes                               *
 *******************************************************************************/
//static void GPIO_SetupUART1Pins(void);
/*******************************************************************************
 *                        Function Definitions                                 *
 *******************************************************************************/

void UART1_Init(UART_ConfigType *Config_Ptr)
{
    volatile uint32 delay;
    uint32 integer_div;
    uint32 fractional_div;
    uint32 divisor;

    if (Config_Ptr == NULL)
    {
        return; /* DET: Null pointer passed */
    }

    /* 1. Enable UART1 and GPIOB peripheral clocks */
    SYSCTL_RCGCUART_REG |= (1U << 1); /* UART1 clock enable */
    SYSCTL_RCGCGPIO_REG |= (1U << 1); /* GPIOB clock enable */
    delay = SYSCTL_RCGCGPIO_REG; /* Allow clocks to stabilize */

    /* 2. Disable UART1 before configuration */
    UART1_CTL_REG &= ~(1U << 0); /* UARTEN = 0 */

    /* 3. Select clock divisor based on High-Speed Mode */
    if (Config_Ptr->bit_data == HSM_on)
    {
        divisor = 8U;
        UART1_CTL_REG |= (1U << 5); /* Enable High-Speed mode (HSE bit) */
    }
    else
    {
        divisor = 16U;
        UART1_CTL_REG &= ~(1U << 5); /* Disable High-Speed mode */
    }

    /* 4. Calculate baud-rate divisor for system clock = 16 MHz */
    integer_div = (F_CPU) / (divisor * Config_Ptr->baud_rate);
    fractional_div = (uint32) (((F_CPU % (divisor * Config_Ptr->baud_rate))
            * 64U + (divisor * Config_Ptr->baud_rate) / 2U)
            / (divisor * Config_Ptr->baud_rate));
    UART1_IBRD_REG = integer_div;
    UART1_FBRD_REG = fractional_div;

    /* 5. Line control: word length, parity, stop bits, FIFOs */
    UART1_LCRH_REG = 0;
    /* Word length: bits 5-8 */
    UART1_LCRH_REG |= ((uint32) Config_Ptr->bit_data << 5);
    /* Enable FIFO */
    UART1_LCRH_REG |= (1U << 4);
    /* Stop bits */
    if (Config_Ptr->stop_bit == StopBit_2)
    {
        UART1_LCRH_REG |= (1U << 3);
    }
    /* Parity */
    if (Config_Ptr->parity != Parity_Disabled)
    {
        UART1_LCRH_REG |= (1U << 1); /* PEN */
        if (Config_Ptr->parity == Parity_Even)
        {
            UART1_LCRH_REG |= (1U << 2); /* EPS */
        }
    }

    /* 6. Configure GPIOB pins PB0 (U1RX) and PB1 (U1TX) */
    GPIO_PORTB_AFSEL_REG |= (1U << 0) | (1U << 1);
    GPIO_PORTB_PCTL_REG &= ~0x000000FF;
    GPIO_PORTB_PCTL_REG |= 0x00000011;
    GPIO_PORTB_DEN_REG |= (1U << 0) | (1U << 1);
    GPIO_PORTB_AMSEL_REG &= ~((1U << 0) | (1U << 1));

    /* 7. Enable UART1, Tx and Rx */
    UART1_CTL_REG |= (1U << 0) | (1U << 8) | (1U << 9);
}

void UART1_SendByte(uint8 data)
{
    while (!(UART1_FR_REG & 0x80))
        ;
    UART1_DR_REG = data;

}

uint8 UART1_ReceiveByte(void)
{
    while (UART1_FR_REG & 0x10)
        ;
    return UART1_DR_REG;
}

/**
 * Check if data is available in UART1 receive FIFO
 * @return: true if data is available, false if receive FIFO is empty
 */
uint8 UART1_DataAvailable(void)
{
    // Check RXFE bit in Flag Register
    // RXFE = 0 means data is available
    // RXFE = 1 means receive FIFO is empty
    uint8 flag = UART1_FR & 0x10;
    if(!flag)
    {
        return TRUE;
    }
    return FALSE;
}
/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART1_SendString(const uint8 *pData)
{
    uint8 i = 0;
    while (pData[i] != '\0')
    {
        UART1_SendByte(pData[i]);
        i++;
    }
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART1_ReceiveString(uint8 *pData)
{
    uint8 i = 0;

    /* Receive the first byte */
    pData[i] = UART1_ReceiveByte();

    /* Receive the whole string until the '#' */
    while (pData[i] != '#')
    {
        i++;
        pData[i] = UART1_ReceiveByte();
    }

    /* After receiving the whole string plus the '#', replace the '#' with '\0' */
    pData[i] = '\0';
}

/*
 * Description :
 * Send the required number of bytes through UART to the other UART device.
 */
void UART1_SendData(const uint8 *pData, uint32 uSize)
{
    uint8 i = 0;
    for (i = 0; i < uSize; i++)
    {
        UART1_SendByte(pData[i]);
    }
}

/*
 * Description :
 * receive the required number of bytes through UART from the other UART device.
 */
void UART1_ReceiveData(uint8 *pData, uint32 uSize)
{
    uint8 i = 0;
    pData[i] = UART1_ReceiveByte();
    for (i = 1; i < uSize; i++)
    {
        pData[i] = UART1_ReceiveByte();
    }
}
/*******************************************************************************
 *                            End of File                                      *
 *******************************************************************************/
