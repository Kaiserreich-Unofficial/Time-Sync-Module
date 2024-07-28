#include <FlashStorage_STM32.h>
#include <eeprom.h>
#include <nmea.h>

void ClearScreen()
{
  Serial3.write(27);    // ESC ASCII code
  Serial3.print("[2J"); // clear screen
  Serial3.write(27);    // ESC ASCII code
  Serial3.print("[H");  // move cursor to top left corner
}

void vTaskConfigLED(void *pvParameters)
{
  (void)pvParameters;
  // 任务执行
  for (int i = 0; i < 3; i++)
  {
    digitalWriteFast(PA_1, HIGH);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    digitalWriteFast(PA_1, LOW);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  // 任务执行完毕后删除自身
  vTaskDelete(NULL);
}

void Exec(const String &cmd, void (*RefreshPWM)())
{
  if (cmd.startsWith("set")) // 设置参数
  {
    int commaIndex1 = cmd.indexOf(' ');
    int commaIndex2 = cmd.indexOf(' ', commaIndex1 + 1);
    char *name = (char *)malloc(16 * sizeof(char));
    cmd.substring(commaIndex1 + 1, commaIndex2).toCharArray(name, 16);
    uint8_t *value = new uint8_t(cmd.substring(commaIndex2 + 1).toInt());

    if (strcmp(name, "gpio_duty_cycle") == 0)
    {
      gpio_duty_cycle = value;
      Serial3.print("GPIO Duty Cycle:");
      Serial3.print(*gpio_duty_cycle);
      Serial3.println("%");
      EEPROM.write(sizeof(WRITTEN_SIGNATURE), *gpio_duty_cycle);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "gpio1_en") == 0)
    {
      gpio1_en = value;
      Serial3.print("GPIO1_EN:");
      Serial3.println(EN_STATUS(*gpio1_en));
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 1, *gpio1_en);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "gpio1_herz") == 0)
    {
      gpio1_herz = value;
      Serial3.print("GPIO1_Herz:");
      Serial3.print(*gpio1_herz);
      Serial3.println("Hz");
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 2, *gpio1_herz);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "gpio2_en") == 0)
    {
      gpio2_en = value;
      Serial3.print("GPIO2_EN:");
      Serial3.println(EN_STATUS(*gpio2_en));
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 3, *gpio2_en);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "gpio2_herz") == 0)
    {
      gpio2_herz = value;
      Serial3.print("GPIO2_Herz:");
      Serial3.print(*gpio2_herz);
      Serial3.println("Hz");
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 4, *gpio2_herz);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "gpio3_en") == 0)
    {
      gpio3_en = value;
      Serial3.print("GPIO3_EN:");
      Serial3.println(EN_STATUS(*gpio3_en));
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 5, *gpio3_en);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "gpio3_herz") == 0)
    {
      gpio3_herz = value;
      Serial3.print("GPIO3_Herz:");
      Serial3.print(*gpio3_herz);
      Serial3.println("Hz");
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 6, *gpio3_herz);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "gpio4_en") == 0)
    {
      gpio4_en = value;
      Serial3.print("GPIO4_EN:");
      Serial3.println(EN_STATUS(*gpio4_en));
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 7, *gpio4_en);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "gpio4_herz") == 0)
    {
      gpio4_herz = value;
      Serial3.print("GPIO4_Herz:");
      Serial3.print(*gpio4_herz);
      Serial3.println("Hz");
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 8, *gpio4_herz);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "blink_period") == 0)
    {
      blink_period = value;
      Serial3.print("Blink Period:");
      Serial3.print(*blink_period);
      Serial3.println("s");
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 9, *blink_period);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "nmea_period") == 0)
    {
      nmea_period = value;
      Serial3.print("NMEA Period:");
      Serial3.print(*nmea_period);
      Serial3.println("s");
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 10, *nmea_period);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "uart1_mode") == 0)
    {
      uart1_mode = value;
      Serial3.print("UART1 MODE:");
      Serial3.println(UART_MODE(*uart1_mode));
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 11, *uart1_mode);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "uart1_bandrate") == 0)
    {
      uart1_bandrate = value;
      Serial3.print("UART1 BANDRATE:");
      Serial3.println(BANDRATE(*uart1_bandrate));
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 12, *uart1_bandrate);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "uart2_mode") == 0)
    {
      uart2_mode = value;
      Serial3.print("UART2 MODE:");
      Serial3.println(UART_MODE(*uart2_mode));
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 13, *uart2_mode);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else if (strcmp(name, "uart2_bandrate") == 0)
    {
      uart2_bandrate = value;
      Serial3.print("UART2 BANDRATE:");
      Serial3.println(BANDRATE(*uart2_bandrate));
      EEPROM.write(sizeof(WRITTEN_SIGNATURE) + 14, *uart2_bandrate);
      EEPROM.commit();
      xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    }
    else
    {
      Serial3.println(F("Invalid parameter name"));
    }

    free(name);
  }
  else if (cmd.startsWith("read"))
  { // 读取所有参数
    READCONFIG();
  }
  else if (cmd.startsWith("reset"))
  { // 读取所有参数
    Serial3.println(F("Resetting Configuration"));
    xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    WRITECONFIG();
    ClearScreen();
    NVIC_SystemReset();
  }
  else if (cmd.startsWith("help"))
  { // 帮助信息
    Serial3.println(F("set <parameter_name> <value> - Change Value of Parameter"));
    Serial3.println(F("read - Read All CONFIG DATA from EEPROM"));
    Serial3.println(F("help - Show this help message"));
    Serial3.println(F("reset - Reset Configuration to Default"));
    Serial3.println(F("reboot - Reboot System"));
    Serial3.println(F("clear - Clear Screen"));
  }
  else if (cmd.startsWith("reboot"))
  { // 重启系统
    Serial3.println(F("Rebooting System"));
    // xTaskCreate(vTaskConfigLED, "vTaskConfigLED", 128, NULL, 1, NULL);
    ClearScreen();
    NVIC_SystemReset();
  }
  else if (cmd.startsWith("clear"))
  { // 清除屏幕
    ClearScreen();
  }
  else if (cmd.startsWith("$GPRMC") || cmd.startsWith("$GNRMC"))
  {
    parseNMEA(cmd, RefreshPWM);
  }
  else
  { // 无效命令
    Serial3.println(F("Invalid command"));
  }
}
