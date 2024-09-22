#include <stdio.h>
#include "pico/stdlib.h"
extern "C" {
#include "can2040/can2040.h"
}
#include "mcp2515/mcp2515.h"
static struct can2040 cbus;


struct can2040_msg spark1_pwm;
struct can2040_msg spark1_enable;
struct can2040_msg spark2_enable;
struct can2040_msg spark2_pwm;
float pwm_speed;
repeating_timer_t timer;
int motor_tick_count;
MCP2515 robotcan;
/*static void
can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // Add message processing code here...
}*/

void robotcan_setup() {
    robotcan.reset();
    robotcan.setBitrate(CAN_100KBPS, MCP_8MHZ);
    robotcan.setNormalMode();
}
static void
PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}
void sparkbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 1000000;
    uint32_t gpio_rx = 6, gpio_tx = 7;

    spark1_pwm.id = 0;
    spark2_pwm.id = 0;
    spark1_pwm.dlc = 8;
    spark2_pwm.dlc = 8;
    spark1_pwm.dataf[1] = 0;
    spark2_pwm.dataf[1] = 0;

    spark1_enable.id = 0;
    spark2_enable.id = 0;
    spark1_enable.dlc = 8;
    spark2_enable.dlc = 8;

    for (int i = 1; i<=7; i++) {
        spark1_enable.data[i] = 0;
        spark2_enable.data[i] = 0;
    }



    // Setup canbus
    can2040_setup(&cbus, pio_num);
    //can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);

}

bool drive_motors(struct repeating_timer *t) {

    motor_tick_count++;
    spark1_enable.data[0] = 0;
    spark2_enable.data[0] = 0;
    if (motor_tick_count == 3) {
        spark1_pwm.dataf[0] = pwm_speed;
        spark2_pwm.dataf[0] = pwm_speed;
        can2040_transmit(&cbus, &spark1_pwm);
        can2040_transmit(&cbus, &spark2_pwm);
        motor_tick_count = 0;
    }

    can2040_transmit(&cbus, &spark1_enable);
    can2040_transmit(&cbus, &spark2_enable);
    return true;
}
int main()
{   
    stdio_init_all();
    sparkbus_setup();
    robotcan_setup();
    add_repeating_timer_ms(10, drive_motors, NULL, &timer);

    

}
