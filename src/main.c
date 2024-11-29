#include "stdio.h"
#include "stdbool.h"
#include "pico/stdlib.h"
#include "pico/stdio_rtt.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/http_client.h"
#include "jsmn.h"

#define STEPPER_1_A 15
#define STEPPER_1_B 14
#define STEPPER_1_COM 13

#define STEPPER_2_A 12
#define STEPPER_2_B 11
#define STEPPER_2_COM 10

// ============================================================================
// NETWORKING
// ============================================================================

#define WIFI_SSID "Tarallini"
#define WIFI_PSK "itarallisonobuoni"
#define SERVER_URL "192.168.1.102"
#define SERVER_PATH "/getdevdata.cgi?device=2&sn=SZ00126022270436"
#define SERVER_PORT 8484

static err_t altcp_recv_callback(void *arg, struct altcp_pcb *pcb, struct pbuf *p, err_t err)
{
    if (err != ERR_OK || p == NULL)
    {
        if (p != NULL)
        {
            pbuf_free(p);
        }
        altcp_close(pcb);
        printf("Error or no data received, err: %d\n", err);
        return ERR_OK;
    }

    // Process the received data
    printf("Received data: %.*s\n", p->len, (char *)p->payload);

    // Free the received pbuf
    pbuf_free(p);

    // Close the connection
    altcp_close(pcb);

    return ERR_OK;
}

void start_data_fetching()
{
    ip_addr_t server_ip;
    server_ip.addr = ipaddr_addr(SERVER_URL);

    err_t err = httpc_get_file(&server_ip, SERVER_PORT, SERVER_PATH, NULL, altcp_recv_callback, NULL, NULL);
    if (err != ERR_OK)
    {
        printf("Failed to init a HTTP GET, err: %d\n", err);
        return;
    }
    printf("HTTP GET started\n");
}

// ============================================================================
// STEPPER
// ============================================================================

typedef struct
{
    int a;
    int b;
    int com;
    int phase;
} stepper_t;

void init_stepper_pin(int pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_12MA);
}

void init_stepper(stepper_t *stepper, int a, int b, int com)
{
    stepper->a = a;
    init_stepper_pin(a);
    stepper->b = b;
    init_stepper_pin(b);
    stepper->com = com;
    init_stepper_pin(com);
    stepper->phase = 0;
}

void apply_phase(stepper_t *stepper)
{
    printf("Executing phase: %d\n", stepper->phase);

    switch (stepper->phase)
    {
    case 0:
        gpio_put(stepper->a, 1);
        gpio_put(stepper->b, 1);
        gpio_put(stepper->com, 0);
        break;
    case 1:
        gpio_put(stepper->a, 0);
        gpio_put(stepper->b, 1);
        gpio_put(stepper->com, 0);
        break;
    case 2:
        gpio_put(stepper->a, 0);
        gpio_put(stepper->b, 1);
        gpio_put(stepper->com, 1);
        break;
    case 3:
        gpio_put(stepper->a, 0);
        gpio_put(stepper->b, 0);
        gpio_put(stepper->com, 1);
        break;
    case 4:
        gpio_put(stepper->a, 1);
        gpio_put(stepper->b, 0);
        gpio_put(stepper->com, 1);
        break;
    case 5:
        gpio_put(stepper->a, 1);
        gpio_put(stepper->b, 0);
        gpio_put(stepper->com, 0);
        break;
    default:
        panic("Invalid stepper phase");
    }

    // Wait for the stepper to move
    sleep_ms(10);
    // Release the GPIO pins
    gpio_put(stepper->a, 0);
    gpio_put(stepper->b, 0);
    gpio_put(stepper->com, 0);
}

void step_up(stepper_t *stepper)
{
    apply_phase(stepper);
    stepper->phase = (stepper->phase + 1) % 6;
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    stdio_uart_init();
    sleep_ms(1000);

    printf("Initializing Wi-Fi\n");
    if (cyw43_arch_init())
    {
        printf("Failed to initialise\n");
        return 1;
    }
    printf("Enabling STA mode\n");
    cyw43_arch_enable_sta_mode();
    printf("Connecting to SSID: " WIFI_SSID "\n");

    while (cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PSK, CYW43_AUTH_WPA3_WPA2_AES_PSK))
    {
        printf("Failed to connect to WiFI\n");
        sleep_ms(1000);
    }
    printf("Connected to Wi-Fi\n");
    cyw43_arch_lwip_begin();

    start_data_fetching();

    // stepper_t stepper_a;
    // init_stepper(&stepper_a, STEPPER_1_A, STEPPER_1_B, STEPPER_1_COM);
    // stepper_t stepper_b;
    // init_stepper(&stepper_b, STEPPER_2_A, STEPPER_2_B, STEPPER_2_COM);
    while (1)
    {
        cyw43_arch_poll();
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
    }

    cyw43_arch_deinit();
    return 0;
}