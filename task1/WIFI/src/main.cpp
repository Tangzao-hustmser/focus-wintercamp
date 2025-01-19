#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

// SSID & Password
const char* ssid = "ESP32-Access-Point";  // ���SSID
const char* password = "20051215";       // �������

// DNS��������Web�������Ķ˿�
const byte DNS_PORT = 53;
DNSServer dnsServer;
WebServer server(80);

// LED��������ΪGPIO 15,������Ҫ�ĳɶ�Ӧʵ��ʹ�õ�gpio�˿�
const int ledPin = 15;

// ����HTMLҳ������
String HTML = "<!DOCTYPE html><html><body>"
              "<h1>ESP32 LED Control plan</h1>"
              "<p><a href=\"/LED/on\">open LED</a></p>"
              "<p><a href=\"/LED/off\">close LED</a></p>"
              "</body></html>";

void handleRoot() {
  server.send(200, "text/html", HTML);
}

void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/html", "<p>LED Opend</p><a href=\"/\">return</a>");
}

void handleLedOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/html", "<p>LED Closed</p><a href=\"/\">return</a>");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Ĭ��LED�ر�

  // ����ESP32ΪAPģʽ
  WiFi.softAP(ssid, password);
  Serial.println("ESP32��ΪAccess Point����");

  // ����DNS�����������������������ض��򵽱���
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // ����Web������·��
  server.on("/", handleRoot);
  server.on("/LED/on", handleLedOn);
  server.on("/LED/off", handleLedOff);

  // ��������δ����������ض��򵽸�Ŀ¼
  server.onNotFound([]() {
    server.send(200, "text/html", HTML);
  });

  server.begin();
  Serial.println("Web������������");
}

void loop() {
  dnsServer.processNextRequest(); // ����DNS�ض���
  server.handleClient(); // ����HTTP����
}


