#include "conf_example.h"
#include "manu.h"
#include <asf.h>
#include <string.h>

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
void debug_uart_config(void) {
  const usart_serial_options_t usart_serial_options = {
      .baudrate = CONF_TEST_BAUDRATE,
      .charlength = CONF_TEST_CHARLENGTH,
      .paritytype = CONF_TEST_PARITY,
      .stopbits = CONF_TEST_STOPBITS,
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

  while (g_systimer - timestart < timeout_ms && counter < bufferlen - 1) {
    if (usart_read(usart, &rx) == 0) {
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

void print_something(){
	//limpa o buffer para não imprimir lixo na memória
	usart_put_char(USART0, 27);
	usart_put_char(USART0, 64);

	//envia as configuraçōes da impressora
	usart_put_char(USART0, 27);
	usart_put_char(USART0, 55);
	usart_put_char(USART0, 7);
	usart_put_char(USART0, 220);
	usart_put_char(USART0, 25);

	usart_put_array_char(USART0, "\r\n\r\n");

	for (int j = 0; j < 512; j++) {
		usart_put_char(USART0, 18);
		usart_put_char(USART0, 42);
		usart_put_char(USART0, 1);
		usart_put_char(USART0, 48); // 48 bytes serão necessários enviar
		for (int i = 0; i < (48); i++) {
			usart_put_char(USART0, (unsigned)imagem_manu[j][i]);
			usart_put_char(USART1, (unsigned)imagem_manu[j][i]);
		}
		usart_put_char(USART0, 27);
		usart_put_char(USART0, 74);
		usart_put_char(USART0, 0);
	}
	//manda 3 enters depois da imagem
	usart_put_array_char(USART0, "\r\n\r\n\r\n");

}

//USART0 - Impressora
//USART1 - Console.log
//USART2 - Bluetooth
//
/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void) {
  Ctrl_status status;
  FRESULT res;
  DIR dj;
  FATFS fs;
  FILINFO fno;
  FIL file_object;
  UINT byte_read;
  
  float progress;

  sysclk_init();
  board_init();
  delay_init();
  SysTick_Config(sysclk_get_cpu_hz() / 1000); // 1 ms
  debug_uart_config();

  //irq_initialize_vectors();
  //cpu_irq_enable();
  
  printer_config();
  hm10_config_server();
  //hm10_server_init(); //Hardware ja esta configurado
  
  char file_name[30];
  char image[48];
  int img_index;
  /* Initialize SD MMC stack */
  sd_mmc_init();
  
  usart_put_string(USART1, "Iniciando programa...");
  printf("\x0C\n\r-- SD/MMC/SDIO FatFs on Card Printer --\n\r");
  printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

  while (1) {
	
    printf("Please plug an SD, MMC or SDIO card in slot.\n\r");
    /* Wait card present and ready */

    //Abrir o cartao
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
	
    uint32_t capacity = sd_mmc_get_capacity(0);
    printf("[OK], capacity: %lu\r\n", capacity);
	while(1){
		while (1) {
			memset(file_name, 0, 30);
			//usart_put_string(BT_UART, "USART0-OI\n\r");
			img_index = -1;
			usart_put_string(USART2, "Hello BT");
			usart_get_string(USART2, file_name, 30, 1000);
			if(strlen(file_name)){
				img_index = atoi(file_name);
				printf("Recebi o numero %d!\n\r", img_index);
				if(img_index == 10){
					print_something();
					goto main_end_of_test;
				}
				break;
			}
		}
	
		//Apos abrir o cartao, le o arquivo

		sprintf(file_name, "0:img%d.fa\n\r", img_index);
		printf("Opening file: %s\n\r", file_name);
		res = f_open(&file_object, (char const *)file_name, FA_OPEN_EXISTING | FA_READ);
		if (res != FR_OK) {
		  f_close(&file_object);
		  printf("[FAIL] res: %d\n\r", res);
		  goto main_end_of_test;
		}
		puts("File successfully opened\n\r");
		printf("Printing image of size: %d", file_object.fsize);
	
		//usart_put_array_char(USART0, "hello world is seloko em cachorreira\r\n");
		//limpa o buffer para n�o imprimir lixo na mem�ria
		usart_put_char(USART0, 27);
		usart_put_char(USART0, 64);

		//envia as configura�?es da impressora
		usart_put_char(USART0, 27);
		usart_put_char(USART0, 55);
		usart_put_char(USART0, 7);
		usart_put_char(USART0, 220);
		usart_put_char(USART0, 30);
		//3 new lines
		//usart_put_array_char(USART0, "\r\n\r\n");

		int counter = 0;
		int file_size = file_object.fsize;
		UINT bytes_to_read = 48;
		while (true) {
			progress = (float)counter*100/(float)file_size;
			sprintf(file_name,"Progress: %.3f%%\n\r", progress);
			puts(file_name);
			res = f_read(&file_object, image, 48, &byte_read);
			counter += byte_read;
			if (res != FR_OK) {
				printf("Couldn't read img%d, res: %d. Bytes read: %d, in total: %d\n\r", img_index, res , byte_read, counter);
				break;
			}
			if (byte_read != 48) {
				printf("No final, li %d bytes de %d. Sobrou %d\n\r", counter, file_size, byte_read);
				break;
			}
			//Config new line
			usart_put_char(USART0, 18);
			usart_put_char(USART0, 42);
			usart_put_char(USART0, 1);
			usart_put_char(USART0, 48); // 48 bytes serao necessarios enviar
			for (int j = 0; j < 48; j++) {
				usart_put_char(USART0, image[j]);
				//printf("%d ", image[j]);
			}
			usart_put_char(USART0, 27);
			usart_put_char(USART0, 74);
			usart_put_char(USART0, 0);
		}
		//manda 3 enters depois da imagem
		usart_put_array_char(USART0, "\r\n\r\n\r\n");
	
		f_close(&file_object);
	}

  main_end_of_test:
    printf("Please unplug the card\n\r");
    while (CTRL_NO_PRESENT != sd_mmc_check(0)) {
		
    }
  }
}
