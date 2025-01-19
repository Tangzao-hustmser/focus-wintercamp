#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

// ���崮������
const int serialPin = 1;  // ����ʵ������޸�

// ����������
TaskHandle_t task1Handle, task2Handle;
//�������֣���һ��ʼ��ÿ�μ���
void task1(void *pvParameters) {
  int value = 1;
  while (1) {
      Serial.print(value);
      Serial.println();
      vTaskDelay(pdMS_TO_TICKS(1000));  // ÿ�뷢��һ��
      value += 3;
  }
}
//�������֣��Ӷ���ʼ��ÿ�μ�һ
void task2(void *pvParameters) {
  int value = 2;
  while (1) {
      Serial.print(value);
      Serial.println();
      vTaskDelay(pdMS_TO_TICKS(1000));  // ÿ�뷢��һ��
      value += 1;
  }
}

void setup() {
  Serial.begin(115200);
  
  // ��������
  xTaskCreate(task1, "Task1", 10000, NULL, 1, &task1Handle);
  xTaskCreate(task2, "Task2", 10000, NULL, 1, &task2Handle);
}

void loop() {
  // ��ѭ���������Arduino�����Ҫ��loop����
}
