// proyecto.ino
#include "WiFiManager.h"
#include "MqttManager.h"
#include "GasSensor.h"
#include "ShadowReporter.h"
#include <WiFiClientSecure.h>
#include <time.h>

// Pines y parámetros
static const int PUMP_PIN   = 23;
static const int SENSOR_PIN = 34;
const char* WIFI_SSID      = "Saul Guizada";
const char* WIFI_PASS      = "iotprueba";
const char* MQTT_BROKER    = "abmisr4l2y1c3-ats.iot.us-east-2.amazonaws.com";
const int   MQTT_PORT      = 8883;
const char* CLIENT_ID      = "ESP-32";
const char* LOCATION       = "Casa-Saul";


// Certificados (omitir contenido por brevedad)

// Amazon Root CA
extern const char AMAZON_ROOT_CA1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";


// Device certificate
extern const char CERTIFICATE[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVALbguJG5eBmh4BhqZEPSVDBh0LOeMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNTA0MjMyMjUy
MTFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDNfa/d5iVMzoJ25WPM
iMFwpE/BTNXLy9JXfGoOurOmH6AQ3AECGxYNHJ0xlL9sdaQjp0YISnDKDZowmzAw
NhKyn4yPlxZOxzAstfqs6T081HpWF4bz2jmV45AqOc6uh6++eNImLmZIT+Dr2Ihi
O4DalHgzZoRyyDASNlChszh49hRjk0dRCylmHfg9pP6vmE3Aow74S2zk1DS9GeDM
5lfinhCeVTNu/3FXziKNsBWGpFH1+FgplehB3fUxlCzGfdevGVc5IAIOZ5I+rcv2
9CfKuI323Bsu4XNMTrh6UiGBGOMhAq4GS4bm5giMqisI1gp3jANo5XOIa1PAeReH
lkkxAgMBAAGjYDBeMB8GA1UdIwQYMBaAFCjt6KITcg+Rqt38OJ7fLpwGcwTzMB0G
A1UdDgQWBBT3+ZhjUKq5tCNM0a5CPlVfaogL6jAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAPXrgU0NMaw7WqEbYQeS0IiJ3
l9QXOyB/3Vjjzx9MTXkPE7ojeUBIpawLzGxfjZp0/jmLq1TYIYF8Oa8b6/S5bKgt
aw866CwtdRHdI+xV3sucTopzvILM6oIziXgu5k/GY7QVUltQuPXSwaOZZj9pIHBE
8o7V6uoPXbb4EcFFV3KEgDsICHJYSSweV3KqLwFDhNM8w3q7tfnYVUXDBMVBlE7D
gDG3mgKmpnr29O3QWuOeGM/MCXi4prufvwPG/3VD1d2Bft4TdzYOsRUiNPcz5sOr
pMfTJSIC+uGWPmzhIRjr84U1E8GKo7YxyrC6VWgR9E03qyRi/Q9q955ro0Qx3w==
-----END CERTIFICATE-----
)KEY";

// Private key
extern const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAzX2v3eYlTM6CduVjzIjBcKRPwUzVy8vSV3xqDrqzph+gENwB
AhsWDRydMZS/bHWkI6dGCEpwyg2aMJswMDYSsp+Mj5cWTscwLLX6rOk9PNR6VheG
89o5leOQKjnOroevvnjSJi5mSE/g69iIYjuA2pR4M2aEcsgwEjZQobM4ePYUY5NH
UQspZh34PaT+r5hNwKMO+Ets5NQ0vRngzOZX4p4QnlUzbv9xV84ijbAVhqRR9fhY
KZXoQd31MZQsxn3XrxlXOSACDmeSPq3L9vQnyriN9twbLuFzTE64elIhgRjjIQKu
BkuG5uYIjKorCNYKd4wDaOVziGtTwHkXh5ZJMQIDAQABAoIBADglto8IGnsBFi7M
gNdbKpfPnacwil5UA9Y5vXTm2L10eLUCmBfuNLlCb0Uj4qYNAttnEkCjrmvXCaFs
Vd9mdvdtlueJHeRKksQ0uRjT5hKf8AlkBlDM+N0Wv8Z54xpXItMFik46cWIiyU9H
7AQDRDL1TF6nDVubjV78jk2Qfca24GpzOFsIRlLK7r1854LgnM2MadLCK8JPPJ8M
nQ9B8czElpo0KxMukpisUMewFcRMMiroReMdA/XsbpmbtFNqzcFdt+jkwc2DCLz2
v3plPxY41Jr2jVi7y98OXYyjChDuXaxSKrwiTtSesTXzokzj8vmjM3+elo3pt5Ul
PxrwYdECgYEA/RtzbgzAJqIFjKGQ4liJmUnIxj7UHpvxEGBTV67HT71JbQ/DvqHG
gs+r2Mo7lJZ4ZN+S8+UjPoIalpmGc1B0s2XbKKwjpj3k6d3tccba64uQQoaOjTPT
Gw54WxSew/lZgReWSM/bS3esBwrdTNUwFb2bJUub1DUbO/NqCMNqyn0CgYEAz9br
PewBc83eyQOopaSOBCJRG5qD3Vpiv/igytpX6kVnzf2b8hK/0YkqHaygTOUbYP+o
s1kJINnDGs0lGXjLAixX/yLbPSyUWkrxYa1OwMPE4dYht+gyjEuzgJgdUWEHEbYq
UWwtQWlJRsp72TR1psLqk044yxtwnAcIKdN9A8UCgYBZNaKyqCXVuAeSsKN7U89z
ytDMJvrmqvoFVp4pan3vefz2RrcsaqKoGoAsUeM8FOjsTOzjr+CDOKIccJI19Kad
UIo1RK7v+iLD1yfIkVk56h/3gqud0yVWBPygxZzL4AgO+vxKP5DAgbaNaOPlH8Lp
9sFHHOYYnnsx3zKBUjdClQKBgGVGxgS7fw2uENSE6uYkZGuRSVKZHY58X8hJ4BhZ
AyQORAH1hcsU5dG19AbXupkdvxSqH8udJDyUwGTL+DsMSGg7sXmKw6hwk63X/P1q
1QhLnSGrFW0nQZJpxQNxnD+udyJjUQWyH+9S3bkYK/dwf7AOJP44zKX0LhBpwMmY
2789AoGBAKafCoZy835wDTzENZFKLqPkxdTKfhqbB0iXkjzT98g/M9xnF5lXe7gn
CatdqQ5H/BRXcBrmkouMFbQAClLdFZiYFM7CNHboJQKrZPssNo3dbDKoIdfx9Geg
WdtuyWp1AaR5iPE+RBD1Y9qKKHwk99DMRyjHh4l7olRQoRZ0EPZx
-----END RSA PRIVATE KEY-----
)KEY";

WiFiClientSecure secureClient;
WiFiManager wifi(WIFI_SSID, WIFI_PASS);
MqttManager mqttManager(secureClient, MQTT_BROKER, MQTT_PORT, CLIENT_ID);
GasSensor sensor(SENSOR_PIN);
ShadowReporter reporter(mqttManager.client());

unsigned long lastRead = 0;
const unsigned long interval = 3000;
// Estado latched de la bomba
bool pumpState = false;

void setup() {
    Serial.begin(115200);
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, LOW);

    wifi.connect();
    secureClient.setCACert(AMAZON_ROOT_CA1);
    secureClient.setCertificate(CERTIFICATE);
    secureClient.setPrivateKey(PRIVATE_KEY);

    // Sincronizar NTP
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    time_t now;
    do { delay(500); now = time(nullptr); } while (now < 100000);

    mqttManager.begin();
    // Callback delta Shadow: cambia pumpState según desired
    mqttManager.setCallback([](char* topic, byte* payload, unsigned int len) {
        StaticJsonDocument<128> doc;
        if (!deserializeJson(doc, payload)) {
            JsonVariant d = doc["state"]["desired"]["bomba"];
            if (!d.isNull()) {
                pumpState = d.as<bool>();
                digitalWrite(PUMP_PIN, pumpState ? HIGH : LOW);
            }
        }
    });
    while (!mqttManager.ensureConnection());
    mqttManager.subscribe("$aws/things/DameTuCosita/shadow/update/delta");

    // Primera lectura y reporte
    sensor.read();
    // Prende localmente si crítico
    if (sensor.isFireEvent()) {
        pumpState = true;
        digitalWrite(PUMP_PIN, HIGH);
    }
    reporter.report(pumpState, (int)sensor.getPPM(), sensor.isFireEvent(), LOCATION);
}

void loop() {
    if (!mqttManager.ensureConnection()) return;
    mqttManager.loop();

    unsigned long nowMillis = millis();
    if (nowMillis - lastRead < interval) return;
    lastRead = nowMillis;

    sensor.read();

    // Local on-event: prende si crítico, apaga si bajo
    if (sensor.isFireEvent()) {
        if (!pumpState) {
            pumpState = true;
            digitalWrite(PUMP_PIN, HIGH);
        }
    } else {
        if (pumpState) {
            pumpState = false;
            digitalWrite(PUMP_PIN, LOW);
        }
    }
    reporter.report(pumpState, (int)sensor.getPPM(), sensor.isFireEvent(), LOCATION);
}
