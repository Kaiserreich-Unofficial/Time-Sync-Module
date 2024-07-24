#include <FlashStorage_STM32.h>
// 默认模块配置
#define default_gpio_duty_cycle 50 // 占空比(%)
#define default_gpio1_en 1
#define default_gpio1_herz 1
#define default_gpio2_en 1
#define default_gpio2_herz 1
#define default_gpio3_en 1
#define default_gpio3_herz 20
#define default_gpio4_en 0
#define default_gpio4_herz 1
#define default_blink_period 1 // 闪灯周期
#define default_nmea_period 1  // NMEA 发送周期
#define default_uart1_mode 1   // 0:Closed 1:Output 2:Input
#define default_uart1_bandrate 6
#define default_uart2_mode 1 // 0:Closed 1:Output 2:Input
#define default_uart2_bandrate 6
#define default_uart3_bandrate 10 // UART3 CANT BE CLOSED
// Flash签名
const int WRITTEN_SIGNATURE = 0xBEEFDEFC;
u_char *gpio_duty_cycle = (u_char *)malloc(sizeof(u_char)); // 占空比(%)
u_char *gpio1_en = (u_char *)malloc(sizeof(u_char));        // GPIO1 使能
u_char *gpio1_herz = (u_char *)malloc(sizeof(u_char));      // GPIO1 频率
u_char *gpio2_en = (u_char *)malloc(sizeof(u_char));        // GPIO2 使能
u_char *gpio2_herz = (u_char *)malloc(sizeof(u_char));      // GPIO2 频率
u_char *gpio3_en = (u_char *)malloc(sizeof(u_char));        // GPIO3 使能
u_char *gpio3_herz = (u_char *)malloc(sizeof(u_char));      // GPIO3 频率
u_char *gpio4_en = (u_char *)malloc(sizeof(u_char));        // GPIO4 使能
u_char *gpio4_herz = (u_char *)malloc(sizeof(u_char));      // GPIO4 频率
u_char *blink_period = (u_char *)malloc(sizeof(u_char));    // 闪灯周期
u_char *nmea_period = (u_char *)malloc(sizeof(u_char));     // NMEA 发送周期
u_char *uart1_mode = (u_char *)malloc(sizeof(u_char));      // UART1 模式
u_char *uart1_bandrate = (u_char *)malloc(sizeof(u_char));  // UART1 波特率
u_char *uart2_mode = (u_char *)malloc(sizeof(u_char));      // UART2 模式
u_char *uart2_bandrate = (u_char *)malloc(sizeof(u_char));  // UART2 波特率

String EN_STATUS(const u_char &en)
{
  if (en)
  {
    return "True";
  }
  else
  {
    return "False";
  }
}

String UART_MODE(const u_char &mode)
{
  switch (mode)
  {
  case 0:
    return "Closed";
  case 1:
    return "Output";
  case 2:
    return "Input";
  }
}

uint32_t BANDRATE(const u_char &bandrate)
{
  switch (bandrate)
  {
  case 0:
    return 110;
  case 1:
    return 150;
  case 2:
    return 300;
  case 3:
    return 1200;
  case 4:
    return 2400;
  case 5:
    return 4800;
  case 6:
    return 9600;
  case 7:
    return 19200;
  case 8:
    return 38400;
  case 9:
    return 57600;
  case 10:
    return 115200;
  case 11:
    return 230400;
  case 12:
    return 460800;
  case 13:
    return 921600;
  case 14:
    return 1500000;
  }
}

void WRITECONFIG()
{
  EEPROM.write(sizeof(WRITTEN_SIGNATURE), default_gpio_duty_cycle);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 1, default_gpio1_en);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 2, default_gpio1_herz);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 3, default_gpio2_en);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 4, default_gpio2_herz);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 5, default_gpio3_en);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 6, default_gpio3_herz);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 7, default_gpio4_en);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 8, default_gpio4_herz);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 9, default_blink_period);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 10, default_nmea_period);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 11, default_uart1_mode);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 12, default_uart1_bandrate);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 13, default_uart2_mode);
  EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 14, default_uart2_bandrate);
  EEPROM.commit();
  Serial3.println("Done Writing CONFIG DATA...");
}

void READCONFIG()
{
  Serial3.println("=================================================================================================");
  *gpio_duty_cycle = EEPROM.read(sizeof(WRITTEN_SIGNATURE));
  Serial3.print("GPIO Duty Cycle:");
  Serial3.print(*gpio_duty_cycle);
  Serial3.println("%");

  *gpio1_en = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 1);
  Serial3.print("GPIO1_EN:");
  Serial3.println(EN_STATUS(*gpio1_en));

  *gpio1_herz = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 2);
  Serial3.print("GPIO1_Herz:");
  Serial3.print(*gpio1_herz);
  Serial3.println("Hz");

  *gpio2_en = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 3);
  Serial3.print("GPIO2_EN:");
  Serial3.println(EN_STATUS(*gpio2_en));

  *gpio2_herz = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 4);
  Serial3.print("GPIO2_Herz:");
  Serial3.print(*gpio2_herz);
  Serial3.println("Hz");

  *gpio3_en = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 5);
  Serial3.print("GPIO3_EN:");
  Serial3.println(EN_STATUS(*gpio3_en));

  *gpio3_herz = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 6);
  Serial3.print("GPIO3_Herz:");
  Serial3.print(*gpio3_herz);
  Serial3.println("Hz");

  *gpio4_en = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 7);
  Serial3.print("GPIO4_EN:");
  Serial3.println(EN_STATUS(*gpio4_en));

  *gpio4_herz = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 8);
  Serial3.print("GPIO4_Herz:");
  Serial3.print(*gpio4_herz);
  Serial3.println("Hz");

  *blink_period = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 9);
  Serial3.print("Blink Period:");
  Serial3.print(*blink_period);
  Serial3.println("s");

  *nmea_period = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 10);
  Serial3.print("NMEA Period:");
  Serial3.print(*nmea_period);
  Serial3.println("s");

  *uart1_mode = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 11);
  Serial3.print("UART1 MODE:");
  Serial3.println(UART_MODE(*uart1_mode));

  *uart1_bandrate = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 12);
  Serial3.print("UART1 BANDRATE:");
  Serial3.println(BANDRATE(*uart1_bandrate));

  *uart2_mode = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 13);
  Serial3.print("UART2 MODE:");
  Serial3.println(UART_MODE(*uart2_mode));

  *uart2_bandrate = EEPROM.read(sizeof(WRITTEN_SIGNATURE) + 14);
  Serial3.print("UART2 BANDRATE:");
  Serial3.println(BANDRATE(*uart2_bandrate));

  Serial3.print("UART3 MODE:");
  Serial3.println("Input(Necessary for Debug)");

  Serial3.print("UART3 BANDRATE:");
  Serial3.println(BANDRATE(default_uart3_bandrate));

  Serial3.println("=================================================================================================");
}
