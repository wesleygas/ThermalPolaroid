/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <string.h>
#include <manu.h>

//BLuetooth USART
#define BT_NAME USART2
#define BT_ID ID_USART2
//RX - PD16 ||||| TX - PD15
//Bluetooth RX
#define BT_RX_PIO PIOD
#define BT_RX_PERIPH PIO_PERIPH_B
#define BT_RX_PIO 15u
#define BT_RX_PIO_MASK (1 << BT_RX_PIO)

//Bluetooth TX
#define BT_TX_PIO PIOD
#define BT_TX_PERIPH PIO_PERIPH_B
#define BT_TX_PIO 16u
#define BT_TX_PIO_MASK (1 << BT_TX_PIO)

volatile long g_systimer = 0;

void SysTick_Handler() {
	g_systimer++;	
}

void usart_put_string(Usart *usart, char str[]) {
	usart_serial_write_packet(usart, str, strlen(str));
}

int usart_get_string(Usart *usart, char buffer[], int bufferlen, int timeout_ms) {
	long timestart = g_systimer;
	uint32_t rx;
	uint32_t counter = 0;
	
	while(g_systimer - timestart < timeout_ms && counter < bufferlen - 1) {
		if(usart_read(usart, &rx) == 0) {
			//timestart = g_systimer; // reset timeout
			buffer[counter++] = rx;
		}
	}
	buffer[counter] = 0x00;
	return counter;
}

void usart_put_array_char(Usart *usart, char str[]) {
  usart_serial_write_packet(usart, str, strlen(str));
}

void usart_put_char(Usart *usart, unsigned char n) {
  usart_serial_putchar(usart, n);
}

void printer_config(void) {
  usart_serial_options_t config;
  config.baudrate = 9600;
  config.charlength = US_MR_CHRL_8_BIT;
  config.paritytype = US_MR_PAR_NO;
  config.stopbits = false;
  usart_serial_init(USART0, &config);
  usart_enable_tx(USART0);
  usart_enable_rx(USART0);

  // RX - PB0  TX - PB1
  pio_configure(PIOB, PIO_PERIPH_C, (1 << 0), PIO_DEFAULT);
  pio_configure(PIOB, PIO_PERIPH_C, (1 << 1), PIO_DEFAULT);
}

void usart_send_command(Usart *usart, char buffer_rx[], int bufferlen, char buffer_tx[], int timeout) {
	usart_put_string(usart, buffer_tx);
	usart_get_string(usart, buffer_rx, bufferlen, timeout);
}

void usart_log(char* name, char* log) {
	usart_put_string(USART1, "[");
	usart_put_string(USART1, name);
	usart_put_string(USART1, "] ");
	usart_put_string(USART1, log);
	usart_put_string(USART1, "\r\n");
}

void config_console(void) {
	usart_serial_options_t config;
	config.baudrate = 115200;
	config.charlength = US_MR_CHRL_8_BIT;
	config.paritytype = US_MR_PAR_NO;
	config.stopbits = false;
	usart_serial_init(USART1, &config);
	usart_enable_tx(USART1);
	usart_enable_rx(USART1);
}

void hm10_config_server(void) {
	usart_serial_options_t config;
	config.baudrate = 9600;
	config.charlength = US_MR_CHRL_8_BIT;
	config.paritytype = US_MR_PAR_NO;
	config.stopbits = false;
	usart_serial_init(BT_NAME, &config);
	usart_enable_tx(BT_NAME);
	usart_enable_rx(BT_NAME);
	 
	 pio_configure(BT_TX_PIO, BT_TX_PERIPH, BT_TX_PIO_MASK, PIO_DEFAULT);
	 pio_configure(BT_RX_PIO, BT_RX_PERIPH, BT_RX_PIO_MASK, PIO_DEFAULT);
}

void hm10_config_client(void) {
	usart_serial_options_t config;
	config.baudrate = 9600;
	config.charlength = US_MR_CHRL_8_BIT;
	config.paritytype = US_MR_PAR_NO;
	config.stopbits = false;
	usart_serial_init(UART3, &config);
	usart_enable_tx(UART3);
	usart_enable_rx(UART3);
	
	// RX - PD28 TX - PD30
	pio_configure(PIOD, PIO_PERIPH_A, (1 << 28), PIO_DEFAULT);
	pio_configure(PIOD, PIO_PERIPH_A, (1 << 30), PIO_DEFAULT);
}

int hm10_server_init(void) {
	char buffer_rx[128];
	usart_send_command(BT_NAME, buffer_rx, 1000, "AT", 200);
	usart_send_command(BT_NAME, buffer_rx, 1000, "AT", 200);
	usart_send_command(BT_NAME, buffer_rx, 1000, "AT", 200);
	usart_send_command(BT_NAME, buffer_rx, 1000, "AT+RESET", 400);
	usart_log("hm10_server_init", buffer_rx);	
	usart_send_command(BT_NAME, buffer_rx, 1000, "AT+NAMETPolaroid", 400);
	usart_log("hm10_server_init", buffer_rx);
	usart_send_command(BT_NAME, buffer_rx, 1000, "AT+FILT0", 400);
	usart_log("hm10_server_init", buffer_rx);
	usart_send_command(BT_NAME, buffer_rx, 1000, "AT+SHOW1", 400);
	usart_log("hm10_server_init", buffer_rx);
	usart_send_command(BT_NAME, buffer_rx, 1000, "AT+ROLE0", 400);
	usart_log("hm10_server_init", buffer_rx);
}

int hm10_client_init(void) {
	char buffer_rx[128];
	usart_send_command(UART3, buffer_rx, 1000, "AT", 200);
	usart_send_command(UART3, buffer_rx, 1000, "AT", 200);
	usart_send_command(UART3, buffer_rx, 1000, "AT", 200);
	usart_send_command(UART3, buffer_rx, 1000, "AT+RESET", 400);
	usart_send_command(UART3, buffer_rx, 1000, "AT+NAMEClient", 400);
	usart_log("hm10_client_init", buffer_rx);
	usart_send_command(UART3, buffer_rx, 1000, "AT+IMME1", 400);
	usart_log("hm10_client_init", buffer_rx);
	usart_send_command(UART3, buffer_rx, 1000, "AT+ROLE1", 400);
	usart_log("hm10_client_init", buffer_rx);
	usart_send_command(UART3, buffer_rx, 1000, "AT+RESET", 800); // http://www.martyncurrey.com/hm-10-bluetooth-4ble-modules/
	usart_log("hm10_client_init", buffer_rx);
	usart_send_command(UART3, buffer_rx, 1000, "AT+DISC?", 10000); 
	usart_log("hm10_client_init", buffer_rx);
	usart_send_command(UART3, buffer_rx, 1000, "AT+CONN0", 1000);
	usart_log("hm10_client_init", buffer_rx);
	
}

void print_something(){
   //limpa o buffer para não imprimir lixo na memória
  usart_put_char(USART0, 27);
  usart_put_char(USART0, 64);

  //envia as configuraçōes da impressora
  usart_put_char(USART0, 27);
  usart_put_char(USART0, 55);
  usart_put_char(USART0, 7);
  usart_put_char(USART0, 200);
  usart_put_char(USART0, 15);

  usart_put_array_char(USART0, "\r\n\r\n");

  for (int j = 0; j < 512; j++) {
    usart_put_char(USART0, 18);
    usart_put_char(USART0, 42);
    usart_put_char(USART0, 1);
    usart_put_char(USART0, 48); // 48 bytes serão necessários enviar
    for (int i = 0; i < (48); i++) {

      usart_put_char(USART0, (unsigned)imagem_manu[j][i]);
    }
    usart_put_char(USART0, 27);
    usart_put_char(USART0, 74);
    usart_put_char(USART0, 0);
  }
  //manda 3 enters depois da imagem
  usart_put_array_char(USART0, "\r\n\r\n\r\n");

}


int main (void)
{
	board_init();
	sysclk_init();
	delay_init();
	SysTick_Config(sysclk_get_cpu_hz() / 1000); // 1 ms
	config_console();
	//config impressora
	printer_config();
	//imprime a imagem que esta no manu.h
	print_something();

	usart_put_string(USART1, "Inicializando...\r\n");
	usart_put_string(USART1, "Config HC05 Server...\r\n");
	hm10_config_server();
	hm10_server_init();
	char buffer[1024];

	
	while(1) {
		usart_put_string(BT_NAME, "HM10-OI");
		usart_get_string(BT_NAME, buffer, 1024, 1000);
		usart_log("main", buffer);
	}
	
	

	/* Insert application code here, after the board has been initialized. */
}
