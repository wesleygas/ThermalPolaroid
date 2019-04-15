/**
 * \mainpage SD/MMC/SDIO Card with FatFs Example
 *
 * \section Purpose
 *
 * This example shows how to implement the SD/MMC stack with the FatFS.
 * It will mount the file system and write a file in the card.
 *
 * The example outputs the information through the standard output (stdio).
 * To know the output used on the board, look in the conf_example.h file
 * and connect a terminal to the correct port.
 *
 * While using Xplained Pro evaluation kits, please attach I/O1 Xplained Pro
 * extension board to EXT1.
 *
 * \section Usage
 *
 * -# Build the program and download it into the board.
 * -# On the computer, open and configure a terminal application.
 * Refert to conf_example.h file.
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
	-- SD/MMC/SDIO Card Example on FatFs --
	-- Compiled: xxx xx xxxx xx:xx:xx --
	Please plug an SD, MMC or SDIO card in slot.
\endcode
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "conf_example.h"
#include <string.h>
#include "manu.h"

/************************************************************************/
/* PIN DEFS                                                             */
/************************************************************************/

//BLuetooth USART
#define BT_UART USART0
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


/************************************************************************/
/* Global variables                                                     */
/************************************************************************/
volatile long g_systimer = 0;


/************************************************************************/
/* Funtions                                                             */
/************************************************************************/
void SysTick_Handler() {
	g_systimer++;
}
void debug_uart_config(void){
	const usart_serial_options_t usart_serial_options = {
		.baudrate   = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits   = CONF_TEST_STOPBITS,
	};
	
	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);
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

void usart_send_command(Usart *usart, char buffer_rx[], int bufferlen, char buffer_tx[], int timeout) {
	usart_put_string(usart, buffer_tx);
	usart_get_string(usart, buffer_rx, bufferlen, timeout);
}

void hm10_config_server(void) {
	usart_serial_options_t config;
	config.baudrate = 9600;
	config.charlength = US_MR_CHRL_8_BIT;
	config.paritytype = US_MR_PAR_NO;
	config.stopbits = false;
	usart_serial_init(USART2, &config);
	usart_enable_tx(USART2);
	usart_enable_rx(USART2);

	// RX - PB0  TX - PB1
	pio_configure(PIOD, PIO_PERIPH_B, (1 << 15u), PIO_DEFAULT);
	pio_configure(PIOD, PIO_PERIPH_B, (1 << 6u), PIO_DEFAULT);
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

int hm10_server_init(void) {
	char buffer_rx[128];
	usart_send_command(BT_UART, buffer_rx, 1000, "AT", 200);
	usart_send_command(BT_UART, buffer_rx, 1000, "AT", 200);
	usart_send_command(BT_UART, buffer_rx, 1000, "AT", 200);
	usart_send_command(BT_UART, buffer_rx, 1000, "AT+RESET", 400);
	printf("hm10_server_init", buffer_rx);
	usart_send_command(BT_UART, buffer_rx, 1000, "AT+NAMETPolaroid", 400);
	printf("hm10_server_init", buffer_rx);
	usart_send_command(BT_UART, buffer_rx, 1000, "AT+FILT0", 400);
	printf("hm10_server_init", buffer_rx);
	usart_send_command(BT_UART, buffer_rx, 1000, "AT+SHOW1", 400);
	printf("hm10_server_init", buffer_rx);
	usart_send_command(BT_UART, buffer_rx, 1000, "AT+ROLE0", 400);
	printf("hm10_server_init", buffer_rx);
}


void print_something(char image[], int img_len){
	//limpa o buffer para não imprimir lixo na memória
	usart_put_char(USART0, 27);
	usart_put_char(USART0, 64);

	//envia as configuraç?es da impressora
	usart_put_char(USART0, 27);
	usart_put_char(USART0, 55);
	usart_put_char(USART0, 7);
	usart_put_char(USART0, 200);
	usart_put_char(USART0, 15);

	usart_put_array_char(USART0, "\r\n\r\n");
	int counter;
	for (int j=0; j < img_len; j++) {
		if(counter == 0){
			
		usart_put_char(USART0, 18);
		usart_put_char(USART0, 42);
		usart_put_char(USART0, 1);
		usart_put_char(USART0, 48); // 48 bytes serão necessários enviar
	}
	

		usart_put_char(USART0, (unsigned)image[j]);
	
	
	if(counter ==47){
		usart_put_char(USART0, 27);
		usart_put_char(USART0, 74);
		usart_put_char(USART0, 0);
		counter = 0;
	}
	else{
	counter++;
		
	}
	
	}
	//manda 3 enters depois da imagem
	usart_put_array_char(USART0, "\r\n\r\n\r\n");

}



/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	char test_file_name[] = "0:sd_mmc_test.txt";
	Ctrl_status status;
	FRESULT res;
	DIR dj;
	FATFS fs;
	FILINFO fno;
	FIL file_object;
	UINT byte_read;
	
	sysclk_init();
	board_init();
	delay_init();
	SysTick_Config(sysclk_get_cpu_hz() / 1000); // 1 ms
	debug_uart_config();
	
	
	//irq_initialize_vectors();
	//cpu_irq_enable();
	printer_config();
	hm10_config_server();
	
	//hm10_server_init();
	char buffer[300];
	/* Initialize SD MMC stack */
	sd_mmc_init();
	usart_put_string(USART1,"E isso ai cara...");
	printf("\x0C\n\r-- SD/MMC/SDIO Card Example on FatFs --\n\r");
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
	
	while (1) {
		printf("Please plug an SD, MMC or SDIO card in slot.\n\r");
		/* Wait card present and ready */
		do {
			status = sd_mmc_test_unit_ready(0);
			if (CTRL_FAIL == status) {
				printf("Card install FAIL\n\r");
				printf("Please unplug and re-plug the card.\n\r");
				while (CTRL_NO_PRESENT != sd_mmc_check(0)) {
				}
			}
		} while (CTRL_GOOD != status);

		printf("Mount disk (f_mount) of size %d...\r\n", sizeof(FATFS));
		memset(&fs, 0, sizeof(FATFS));
		res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
		if (FR_INVALID_DRIVE == res) {
			printf("[FAIL] res %d\r\n", res);
			goto main_end_of_test;
		}
		printf("[OK]\r\n");

		printf("Create a file (f_open)...\r\n");
		test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
		res = f_open(&file_object,(char const *)test_file_name,
				FA_CREATE_ALWAYS | FA_WRITE);
		if (res != FR_OK) {
			printf("[FAIL] res %d\r\n", res);
			goto main_end_of_test;
		}
		uint32_t capacity = sd_mmc_get_capacity(0);
		printf("[OK], capacity: %lu\r\n", capacity);
		
		
		printf("Write to test file (f_puts)...\r\n");
		if (0 == f_puts("ABCDEFGHIJKLMNOPQRSTUVWXYZ", &file_object)) {
			f_close(&file_object);
			printf("[FAIL]\r\n");
			goto main_end_of_test;
		}
		printf("[OK]\r\n");
		f_close(&file_object);
		
		res = f_open(&file_object, (char const *)test_file_name, FA_OPEN_EXISTING | FA_READ);
		if(res != FR_OK){
			f_close(&file_object);
			printf("[FAIL]\r\n");
			goto main_end_of_test;
		}
				
		printf("Reading info on file(f_gets)...\r\n");
		puts(test_file_name);
		uint8_t buff[4800];
		memset(buff, 0, 2048);
		if(NULL == f_gets(buff,2048,&file_object)){
			f_close(&file_object);
			printf("[FAIL]\r\n");
			goto main_end_of_test;
		}
		
		printf("[OK] Closing file.\n\r");
		f_close(&file_object);
		
		printf("Get files from directory\n\r");
		sprintf(test_file_name,"0:img%d.bytes",1);
		puts(test_file_name);
		res = f_open(&file_object,(char const *)test_file_name, FA_OPEN_EXISTING|FA_READ);
		if(res != FR_OK){
			f_close(&file_object);
			printf("[FAIL]\n\r");
			goto main_end_of_test;
		}puts("File successfully open\n\r");
		memset(buff, 0, 4800);
		UINT bytes_to_read = file_object.fsize;
		printf("Reading %d bytes\n\r", bytes_to_read);
		res = f_read(&file_object, buff, 4800, &byte_read);
		if(res != FR_OK){
			printf("Couldn't read %d. Bytes read: %d\n\r",1, byte_read);
		}
		printf("ae cambada, li %d bytes\n\r", byte_read);
		//for(int i = 0; i < byte_read+5; i++){
		//	printf("%d ",buff[i]);
		//}
		printf("\n\rOKKKKK\n\r");
		
		
		printf("Test is successful.\n\r");
		usart_put_array_char(USART0, "hello world is sn\r\n");
		while(1){
			memset(buffer,0, 30);
			//usart_put_string(BT_UART, "USART0-OI\n\r");
			usart_put_string(USART2, "USART2-OI");
			usart_get_string(USART2, buffer, 1024, 1000);
			if(strcmp(buffer,"oi") == 0){
				printf("Recebi oi!");
				print_something(buff, 4800);
			}
			printf("main %s\n\r", buffer);
		}
		

main_end_of_test:
		printf("Please unplug the card.\n\r");
		while (CTRL_NO_PRESENT != sd_mmc_check(0)) {
		}
	}
}
