#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "bsp/board.h"
#include "lwip/apps/mqtt.h"
#include <string.h>


// Declração Protótipo da função 
float func_leitura_temp();

// Declaração de variáveis
float dados_temp;


struct mqtt_connect_client_info_t info_cliente=
{
    "Camargo", /* identificador do cliente */
    NULL, /* Usuário */
    NULL, /*Senha*/
    0, /*keep alive*/
    NULL, /*Tópico do último desejo*/
    NULL, /*Mensagem do Último Desejo*/
    0, /*Qualidade do Serviço do Último Desejo*/
    0 /*Último Desejo retentivo*/
};


static void mqtt_dados_recebidos_cb(void *arg, const u8_t *dados, u16_t comprimento, u8_t flags){
    char led[30];
    memset(led, '\0', 30);
    strncpy(led, (char *) dados, comprimento);
    printf("Dados recebidos %s\n", led);
    if (strncmp(led, "on", 2) == 0) 
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    } else if (strncmp(led, "off", 3) == 0) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
    }
}

static void mqtt_chegando_publicacao_cb(void *arg, const char *topico, u32_t tamanho){

}

static void mqtt_req_cb(void *arg, err_t erro) {
    printf("Requisição MQTT recebida com erro: %d\n", erro);
}

static void mqtt_conectado_cb(mqtt_client_t *cliente, void *arg, mqtt_connection_status_t status) {
    printf("cliente MQTT conectado com o status: %d\n", status);

    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("Conexão MQTT Aceita!\n");

        err_t erro_ret = mqtt_sub_unsub(cliente, "aula_mqtt/led", 0, mqtt_req_cb, arg, 1);
        
        if(erro_ret == ERR_OK) {
            printf("Inscrito com Sucesso!\n");
        }
    }
}
int main()
{
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    // Enable wifi station
    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms("", "", CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }

        /*
     * Configura o Endereço do Broker MQTT
     */
    ip_addr_t endereco_broker;
    ip4addr_aton("35.172.255.228", &endereco_broker);

        mqtt_client_t* mqtt_cliente = mqtt_client_new();
    
    mqtt_set_inpub_callback(mqtt_cliente, &mqtt_chegando_publicacao_cb, mqtt_dados_recebidos_cb, NULL);

       err_t houve_erro = mqtt_client_connect(mqtt_cliente, &endereco_broker, 1883, &mqtt_conectado_cb, NULL, &info_cliente);
    if (houve_erro != ERR_OK) {
        printf("Falha na requisição de conexão MQTT\n");
        return 0;
    }
    bool enviado = false;
    char buffer[20];
    while (true) 
    {
        dados_temp = func_leitura_temp();

        if (board_button_read()) {

            sprintf(buffer,"%.2f",dados_temp);
            mqtt_publish(mqtt_cliente,"Temperatura", buffer, 5, 0, 0, &mqtt_req_cb, NULL);
            enviado = false;
            while(board_button_read());
       } 
       cyw43_arch_poll();
       sleep_ms(10);
    }
}

float func_leitura_temp()
{
    // Leitura do ADC
    float adc_value = adc_read();
    float leitura_adc = adc_value * (3.3f / (1 << 12)); // 12-bit ADC
    float temperature = (27.0f - ((leitura_adc - 0.706f) /0.001721f));
    
    return temperature;
}