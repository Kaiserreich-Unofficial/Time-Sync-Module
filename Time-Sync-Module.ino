#include <STM32FreeRTOS.h>
#include <HardwareTimer.h>
#include <cmd.h>

#define version_str "v1.0.0" // 版本号

// 定义串口对象
HardwareSerial Serial2(PA3, PA2);
HardwareSerial Serial3(PB11, PB10);

// 定义Timer对象
HardwareTimer *timer1 = new HardwareTimer(TIM1);
HardwareTimer *timer2 = new HardwareTimer(TIM2);
HardwareTimer *timer3 = new HardwareTimer(TIM3);
HardwareTimer *timer4 = new HardwareTimer(TIM4);

unsigned long utcTime = 0; // 初始UTC时间

void setup()
{
  Serial3.begin(BANDRATE(default_uart3_bandrate)); // 第三个串口
  while (!Serial3)
    ;

  delay(200);

  Serial3.print("Start Emulated EEPROM on ");
  Serial3.println(BOARD_NAME);
  Serial3.println(FLASH_STORAGE_STM32_VERSION); // 打印FLASH_STORAGE版本

  Serial3.print("EEPROM length: ");
  Serial3.println(EEPROM.length()); // 打印EEPROM长度

  // Check signature at address 0
  int *signature = (int *)malloc(sizeof(int));
  ;
  EEPROM.get(0, *signature);

  // If the EEPROM is empty then no WRITTEN_SIGNATURE
  if (*signature != WRITTEN_SIGNATURE)
  {
    Serial3.println("EEPROM is empty, writing CONFIG DATA...");

    EEPROM.put(0, WRITTEN_SIGNATURE); // EEPROM 签名占用 4bytes

    WRITECONFIG();

    READCONFIG();
  }
  else
  {
    // EEPROM.get(0, signature);
    Serial3.print("EEPROM has been written.Signature = 0x");
    Serial3.println(*signature, HEX);

    Serial3.println("CONFIG DATA:");

    READCONFIG();
  }
  free(signature);

  // 打印版本信息
  Serial3.println(F("Welcome to the STM32 Blue Pill TimeSync Shell!"));
  Serial3.print(F("Version "));
  Serial3.println(version_str);

  if (*gpio1_en)
  {
    // Timer1 Channel1 (PA8)
    timer1->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, PA8);
    timer1->setOverflow(*gpio1_herz, HERTZ_FORMAT);                         // 设置Timer周期为1秒
    timer1->setCaptureCompare(1, *gpio_duty_cycle, PERCENT_COMPARE_FORMAT); // 设置占空比50%
    timer1->resume();                                                       // 启动Timer
    Serial3.println(F("Initializing PWM OUTPUT on GPIO1..."));
  }

  if (*gpio2_en)
  {
    // Timer2 Channel1 (PA0)
    timer2->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, PA0);
    timer2->setOverflow(*gpio2_herz, HERTZ_FORMAT);                         // 设置Timer周期为1秒
    timer2->setCaptureCompare(1, *gpio_duty_cycle, PERCENT_COMPARE_FORMAT); // 设置占空比50%
    timer2->resume();                                                       // 启动Timer
    Serial3.println(F("Initializing PWM OUTPUT on GPIO2..."));
  }

  if (*gpio3_en)
  {
    // Timer3 Channel1 (PA6)
    timer3->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, PA6);
    timer3->setOverflow(*gpio3_herz, HERTZ_FORMAT);                         // 设置Timer周期为50毫秒
    timer3->setCaptureCompare(1, *gpio_duty_cycle, PERCENT_COMPARE_FORMAT); // 设置占空比50%
    timer3->resume();                                                       // 启动Timer
    Serial3.println(F("Initializing PWM OUTPUT on GPIO3..."));
  }

  if (*gpio4_en)
  {
    // Timer4 Channel1 (PB6) - Warning: STM32F103C6T6 Has no Timer4
    timer4->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, PB6);
    timer4->setOverflow(*gpio4_herz, HERTZ_FORMAT);                         // 设置Timer周期为50毫秒
    timer4->setCaptureCompare(1, *gpio_duty_cycle, PERCENT_COMPARE_FORMAT); // 设置占空比50%
    timer4->resume();                                                       // 启动Timer
    Serial3.println(F("Initializing PWM OUTPUT on GPIO4..."));
  }

  // 配置PC13为输出
  pinMode(PC13, OUTPUT);
  digitalWriteFast(PC_13, HIGH);

  // 初始化串口
  if (*uart1_mode)
  {
    Serial.begin(BANDRATE(*uart1_bandrate)); // 第一个串口
    Serial3.println(F("Initializing UART1..."));
  }
  if (*uart2_mode)
  {
    Serial2.begin(BANDRATE(*uart2_bandrate)); // 第二个串口
    Serial3.println(F("Initializing UART2..."));
  }

  Serial3.print(F("SHELL> "));

  // 创建任务
  xTaskCreate(taskBlink, "Blink", 128, NULL, 1, NULL);
  xTaskCreate(taskNMEA_Transfer, "NMEA", 256, NULL, 1, NULL);
  xTaskCreate(taskShell, "Shell", 1024, NULL, 1, NULL);
  xTaskCreate(taskNMEA_Ref, "NMEA_Ref", 256, NULL, 1, NULL);

  // 启动调度器
  vTaskStartScheduler();
}

void loop()
{
  // 空的loop函数
}

// 中断服务例程
void refreshPWM()
{
  digitalWriteFast(PC_13, HIGH);
  // 刷新Timer1的PWM
  timer1->refresh();
  // 刷新Timer3的PWM
  timer2->refresh();
  // 刷新Timer4的PWM
  timer3->refresh();
}

// 任务1：闪灯
void taskBlink(void *pvParameters)
{
  (void)pvParameters;

  for (;;)
  {
    GPIOC->ODR ^= GPIO_PIN_13;                             // 切换GPIOC引脚13的状态
    vTaskDelay(*blink_period * 1000 / portTICK_PERIOD_MS); // 延时1/gpio1_herz秒
  }
}

// 任务2：构建NMEA语句与串口发送
void taskNMEA_Transfer(void *pvParameters)
{
  (void)pvParameters;

  for (;;)
  {
    // 格式化UTC时间
    char utcTimeStr[11];
    sprintf(utcTimeStr, "%06lu.00", utcTime);

    // 构建NMEA语句
    String nmea = "$GNRMC," + String(utcTimeStr) + ",A,3110.4706987,N,12123.2653375,E,0.604,243.2,300713,0.0,W,A*";
    char checksum = calculateChecksum(nmea);
    char checksumStr[3];
    sprintf(checksumStr, "%02X", checksum); // 转换为大写十六进制字符串
    nmea += String(checksumStr);

    // 发送NMEA语句到第一个串口
    if (*uart1_mode == 1)
      Serial.println(nmea);

    // 发送NMEA语句到第二个串口
    if (*uart2_mode == 1)
      Serial2.println(nmea);

    // 更新UTC时间
    utcTime = updateUTCTime(utcTime);

    vTaskDelay(*nmea_period * 1000 / portTICK_PERIOD_MS); // 延时nmea_period秒
  }
}

// 任务3：处理配置命令
void taskShell(void *pvParameters)
{
  (void)pvParameters;
  String Command = "";
  for (;;)
  {
    if (Serial3.available())
    {
      char input = Serial3.read();
      switch (input)
      {
      case 13: // 回车键
        Serial3.println("");
        Exec(Command, refreshPWM);
        Command = "";
        Serial3.print(F("SHELL> "));
        break;
      case 8: // 退格键
        if (Command.length() > 0)
        {
          Command.remove(Command.length() - 1);
          Serial3.print(F("\b \b"));
        }
        break;
      case 127: // 退格键
        if (Command.length() > 0)
        {
          Command.remove(Command.length() - 1);
          Serial3.print(F("\b \b"));
        }
        break;
      default:
        Serial3.print(input);
        Command += input;
        break;
      }
    }
    vTaskDelay(5 / portTICK_PERIOD_MS); // 延时5毫秒
  }
}

// 任务4：读取参考GPRMC或GNRMC语句并更新UTC时间
void taskNMEA_Ref(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    if (*uart1_mode == 2)
      utcTime = nmea_ref(Serial, refreshPWM);
    if (*uart2_mode == 2)
      utcTime = nmea_ref(Serial2, refreshPWM);

    vTaskDelay(5 / portTICK_PERIOD_MS); // 延时5毫秒
  }
}
