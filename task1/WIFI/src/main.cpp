#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

// SSID & Password
const char* ssid = "ESP32-Access-Point";  // 你的SSID
const char* password = "20051215";       // 你的密码

// DNS服务器和Web服务器的端口
const byte DNS_PORT = 53;
DNSServer dnsServer;
WebServer server(80);

// LED引脚设置为GPIO 15,这里需要改成对应实际使用的gpio端口
const int ledPin = 15;

// 设置HTML页面内容
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
  digitalWrite(ledPin, LOW); // 默认LED关闭

  // 设置ESP32为AP模式
  WiFi.softAP(ssid, password);
  Serial.println("ESP32作为Access Point启动");

  // 启动DNS服务器，将所有域名请求重定向到本地
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // 设置Web服务器路由
  server.on("/", handleRoot);
  server.on("/LED/on", handleLedOn);
  server.on("/LED/off", handleLedOff);

  // 捕获所有未定义的请求并重定向到根目录
  server.onNotFound([]() {
    server.send(200, "text/html", HTML);
  });

  server.begin();
  Serial.println("Web服务器已启动");
}

void loop() {
  dnsServer.processNextRequest(); // 处理DNS重定向
  server.handleClient(); // 处理HTTP请求
}


