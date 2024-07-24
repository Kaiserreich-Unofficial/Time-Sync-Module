// 计算NMEA校验和
char calculateChecksum(const String &nmea)
{
  char checksum = 0;
  for (int i = 1; i < nmea.length() - 1; i++)
  {
    checksum ^= nmea[i];
  }
  return checksum;
}

// 更新UTC时间
unsigned long updateUTCTime(const unsigned long &utcTime)
{
  // 提取当前的小时、分钟和秒
  unsigned long hours = utcTime / 10000;
  unsigned long minutes = (utcTime % 10000) / 100;
  unsigned long seconds = utcTime % 100;

  // 增加一秒
  seconds++;

  // 处理进位
  if (seconds >= 60)
  {
    seconds = 0;
    minutes++;
  }
  if (minutes >= 60)
  {
    minutes = 0;
    hours++;
  }
  if (hours >= 24)
  {
    hours = 0;
  }

  // 重新组合成UTC时间格式
  return hours * 10000 + minutes * 100 + seconds;
}

// 解析NMEA数据
uint32_t parseNMEA(const String &input, void (*RefreshPWM)())
{
  if (input.startsWith("$GPRMC") || input.startsWith("$GNRMC"))
  {
    // 校验和验证
    char receivedChecksum = input[input.length() - 3];
    char calculatedChecksum = calculateChecksum(input);
    if (receivedChecksum == calculatedChecksum)
    {
      // 提取UTC时间
      int commaIndex1 = input.indexOf(',');
      int commaIndex2 = input.indexOf(',', commaIndex1 + 1);
      String utcTimeStr = input.substring(commaIndex1 + 1, commaIndex2);
      if (utcTimeStr.length() >= 6)
      {
        return utcTimeStr.toInt();
        RefreshPWM();
      }
    }
  }
}

uint32_t nmea_ref(HardwareSerial Serial, void (*RefreshPWM)())
{
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n');
    return parseNMEA(input, RefreshPWM);
  }
}
