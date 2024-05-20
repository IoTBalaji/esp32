#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char *ssid = "ESP32";		   // Enter SSID here
const char *password = "12345678"; // Enter Password here

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(8118);

bool LED1status = LOW;
bool LED2status = LOW;

String SendHTML(bool isNotFound = false);
String SendJSON(bool led1, bool led2);

void handle_OnConnect();
void handle_led1on();
void handle_led1off();
void handle_led2on();
void handle_led2off();
void handle_Status();
void handle_NotFound();

void setup()
{
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);

	WiFi.softAP(ssid, password);
	WiFi.softAPConfig(local_ip, gateway, subnet);
	delay(100);

	server.on("/", handle_OnConnect);
	server.on("/led", handle_Status);
	server.on("/led/1/on", handle_led1on);
	server.on("/led/1/off", handle_led1off);
	server.on("/led/2/on", handle_led2on);
	server.on("/led/2/off", handle_led2off);
	server.onNotFound(handle_NotFound);

	server.begin();
	Serial.println("HTTP server started");
}
void loop()
{
	server.handleClient();
	if (LED1status)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(200);
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
	}
	else
	{
		digitalWrite(LED_BUILTIN, LOW);
	}

	if (LED2status)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(200);
		digitalWrite(LED_BUILTIN, LOW);
		delay(200);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(200);
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
	}
	else
	{
		digitalWrite(LED_BUILTIN, LOW);
	}
}

void handle_OnConnect()
{
	server.send(200, "text/html", SendHTML());
}

void handle_Status()
{
	server.send(200, "text/json", SendJSON(LED1status, LED2status));
}

void handle_led1on()
{
	LED1status = HIGH;
	Serial.println("GPIO4 Status: ON");
	server.send(204);
}

void handle_led1off()
{
	LED1status = LOW;
	Serial.println("GPIO4 Status: OFF");
	server.send(204);
}

void handle_led2on()
{
	LED2status = HIGH;
	Serial.println("GPIO5 Status: ON");
	server.send(204);
}

void handle_led2off()
{
	LED2status = LOW;
	Serial.println("GPIO5 Status: OFF");
	server.send(204);
}

void handle_NotFound()
{
	server.send(404, "text/html", SendHTML(true));
}

String SendHTML(bool isNotFound)
{
	String htmlContent = "<!DOCTYPE html>";
	htmlContent += "<html>";
	htmlContent += "<head>";
		htmlContent += "<meta charset='utf-8'>";
		htmlContent += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
		htmlContent += "<title>Balaji ESP32</title>";
		htmlContent += "<link href='https://cdn.jsdelivr.net/gh/IoTBalaji/esp32@latest/index.css' rel='stylesheet' crossorigin='anonymous'>";
	htmlContent += "</head>";
	htmlContent += "<body>";

	if (isNotFound) {
		htmlContent += "<div id=\"container\">";
			htmlContent += "<h1>Page Not Found</h1>";
			htmlContent += "<a href=\"/\">Go Back</a>";
		htmlContent += "</div>";
	} else {
		htmlContent += "<div id=\"container\">";
			htmlContent += "<h1>Poor Internet Connection!</h1>";
		htmlContent += "</div>";
		htmlContent += "<script src='https://cdn.jsdelivr.net/gh/IoTBalaji/esp32@latest/index.js' crossorigin='anonymous'></script>";
	}
	htmlContent += "</div>";
	htmlContent += "</html>";
	return htmlContent;
}

String SendJSON(bool led1, bool led2) {
	return String("{\"led1\":" + String(led1 == true ? "true" : "false") + ", \"led2\":" + String(led2 == true ? "true" : "false") + "}");
}