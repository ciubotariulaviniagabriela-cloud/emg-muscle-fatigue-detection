#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <U8g2lib.h>

// OLED SSD1306 galben-albastru
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 12, /* data=*/ 14, /* reset=*/ U8X8_PIN_NONE);

// WiFi Access Point
const char* ssid = "EMG_Monitor";
const char* password = "12345678";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Pin EMG si buffer grafic
const int emgPin = A0;
const int maxPoints = 128;
float dataPoints[maxPoints];
int pointIndex = 0;

// Buffer istoric pentru tabel web (30 inregistrari)
struct EmgRecord {
  int value;
  String status;
  unsigned long timestamp;
};

const int historySize = 30;
EmgRecord history[historySize];
int historyIndex = 0;
int historyCount = 0;

unsigned long lastUpdate = 0;
const int updateInterval = 50; // 20 Hz pentru OLED si WebSocket

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    sendFullHistory(num);
  }
}

void sendFullHistory(uint8_t clientNum) {
  String json = "[";
  for (int i = 0; i < historyCount; i++) {
    int idx = (historyIndex - historyCount + i + historySize) % historySize;
    unsigned long seconds = history[idx].timestamp / 1000;
    json += "{\"t\":" + String(seconds) +
            ",\"v\":" + String(history[idx].value) +
            ",\"s\":\"" + history[idx].status + "\"}";
    if (i < historyCount - 1) json += ",";
  }
  json += "]";
  webSocket.sendTXT(clientNum, json);
}

void setup() {
  Serial.begin(115200);
  u8g2.begin();

  for (int i = 0; i < maxPoints; i++) {
    dataPoints[i] = 1023;
  }

  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    String html = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Monitorizare EMG</title>
<style>
body { font-family: Arial, sans-serif; margin: 20px; background: #f0f0f0; }
h1 { color: #333; text-align: center; }
#status { font-size: 28px; font-weight: bold; text-align: center; margin: 20px 0; }
canvas { border: 3px solid #333; background: #fff; display: block; margin: 30px auto; width: 100%; max-width: 2800px; }
table { width: 100%; max-width: 1200px; margin: 40px auto; border-collapse: collapse; }
th, td { border: 1px solid #ccc; padding: 10px; text-align: center; }
th { background: #007bff; color: white; font-size: 18px; }
.oboseala { background-color: #ffcccc; font-weight: bold; }
.normal { background-color: #ccffcc; }
h2 { text-align: center; color: #333; }
</style>
</head>
<body>
<h1>Monitorizare EMG in timp real</h1>
<p id="status">Normal</p>
<canvas id="emgChart" width="2800" height="400"></canvas>
<h2>Ultimele 30 de masuratori</h2>
<div style="max-height:600px; overflow-y:auto; margin: 0 auto; max-width:1200px;">
<table id="dataTable">
<thead><tr><th>Timp (s)</th><th>Valoare EMG</th><th>Status</th></tr></thead>
<tbody></tbody>
</table>
</div>
<script>
const canvas = document.getElementById('emgChart');
const ctx = canvas.getContext('2d');
const tableBody = document.querySelector('#dataTable tbody');
const statusEl = document.getElementById('status');
function resizeCanvas() { canvas.width = canvas.offsetWidth; }
window.addEventListener('resize', resizeCanvas);
resizeCanvas();
let dataPoints = [];
let records = [];
function drawGraph() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.beginPath();
  ctx.strokeStyle = '#007bff';
  ctx.lineWidth = 4;
  for (let i = 0; i < dataPoints.length; i++) {
    let x = i * (canvas.width / (dataPoints.length - 1 || 1));
    let y = canvas.height - (dataPoints[i] / 1023 * canvas.height * 0.9) - 20;
    i === 0 ? ctx.moveTo(x, y) : ctx.lineTo(x, y);
  }
  ctx.stroke();
}
const ws = new WebSocket('ws://' + location.hostname + ':81/');
ws.onmessage = function(event) {
  if (event.data.startsWith('[')) {
    records = JSON.parse(event.data);
    dataPoints = records.map(r => r.v);
    tableBody.innerHTML = '';
    records.forEach(r => {
      let row = tableBody.insertRow();
      row.className = r.s.includes("Oboseala") ? "oboseala" : "normal";
      row.insertCell(0).innerText = r.t;
      row.insertCell(1).innerText = r.v;
      row.insertCell(2).innerText = r.s;
    });
  } else {
    let data = JSON.parse(event.data);
    records.unshift(data);
    if (records.length > 30) records.pop();
    dataPoints.push(data.v);
    if (dataPoints.length > 30) dataPoints.shift();
    statusEl.innerText = data.s;
    statusEl.style.color = data.s.includes("Oboseala") ? "red" : "green";
    let row = tableBody.insertRow(0);
    row.className = data.s.includes("Oboseala") ? "oboseala" : "normal";
    row.insertCell(0).innerText = data.t;
    row.insertCell(1).innerText = data.v;
    row.insertCell(2).innerText = data.s;
  }
  drawGraph();
};
</script>
</body>
</html>
)=====";
    server.send(200, "text/html", html);
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  server.handleClient();
  webSocket.loop();

  unsigned long now = millis();

  if (now - lastUpdate >= updateInterval) {
    int rawValue = analogRead(emgPin);
    String currentStatus = (rawValue < 600) ? "O B O S I T" : "N O R M A L";

    dataPoints[pointIndex] = rawValue;
    pointIndex = (pointIndex + 1) % maxPoints;

    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_logisoso16_tr);
      u8g2.drawStr(8, 15, currentStatus.c_str());
      u8g2.drawHLine(0, 18, 128);

      const int graphTop = 22;
      const int graphBottom = 60;
      const int graphHeight = graphBottom - graphTop;

      u8g2.drawHLine(0, graphTop, 128);
      u8g2.drawHLine(0, graphBottom, 128);

      for (int i = 1; i < maxPoints; i++) {
        int idx1 = (pointIndex - maxPoints + i - 1 + maxPoints) % maxPoints;
        int idx2 = (pointIndex - maxPoints + i + maxPoints) % maxPoints;
        int y1 = graphTop + (int)((1023 - dataPoints[idx1]) / 1023.0 * graphHeight);
        int y2 = graphTop + (int)((1023 - dataPoints[idx2]) / 1023.0 * graphHeight);
        y1 = constrain(y1, graphTop + 2, graphBottom - 2);
        y2 = constrain(y2, graphTop + 2, graphBottom - 2);
        u8g2.drawLine(i-1, y1, i, y2);
      }
    } while (u8g2.nextPage());

    static unsigned long lastWebSend = 0;
    if (now - lastWebSend >= 1000) {
      unsigned long seconds = now / 1000;
      String json = "{\"t\":" + String(seconds) +
                    ",\"v\":" + String(rawValue) +
                    ",\"s\":\"" + currentStatus + "\"}";
      webSocket.broadcastTXT(json);

      history[historyIndex].value = rawValue;
      history[historyIndex].status = currentStatus;
      history[historyIndex].timestamp = now;
      historyIndex = (historyIndex + 1) % historySize;
      if (historyCount < historySize) historyCount++;
      lastWebSend = now;
    }

    lastUpdate = now;
  }
}
