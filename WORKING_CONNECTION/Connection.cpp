#include "Connection.h"
#include <string>
#include <queue>

// ── UUIDs ────────────────────────────────────────────────────────────────────
static BLEUUID serviceUUID("028c8db0-fb17-11e4-a322-1697f925ec7b");
static BLEUUID wcharUUID  ("028c8db2-fb17-11e4-a322-1697f925ec7b"); // WRITE
static BLEUUID ncharUUID  ("028c8db1-fb17-11e4-a322-1697f925ec7b"); // NOTIFY

// ── State ────────────────────────────────────────────────────────────────────
static bool doConnect    = false;
static bool connected    = false;
static bool doScan       = false;

static BLEClient*               pClient     = nullptr;
static BLERemoteCharacteristic* pWriteChar  = nullptr;
static BLERemoteCharacteristic* pNotifyChar = nullptr;
static BLEAdvertisedDevice*     myDevice    = nullptr;

volatile static bool gotInit = false;

struct Command {
    std::string body;
    unsigned long delayMs;

    Command(std::string b) : body(b), delayMs(0) {}
    Command(std::string b, unsigned long d) : body(b), delayMs(d) {}
};

static std::queue<Command> commands;

// ── Notify callback ──────────────────────────────────────────────────────────
static void notifyCallback(BLERemoteCharacteristic* pChr,
                           uint8_t* pData, size_t length, bool isNotify) {
  Serial.print("Notify len=");
  Serial.println(length);
  Serial.print("data: ");
  for (size_t i = 0; i < length; i++) {
    if (pData[i] < 0x10) Serial.print("0");
    Serial.print(pData[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  gotInit = true;
}

// ── Client callbacks ─────────────────────────────────────────────────────────
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient*) override { Serial.println("Client connected"); }
  void onDisconnect(BLEClient*) override {
    connected   = false;
    pWriteChar  = nullptr;
    pNotifyChar = nullptr;
    while (!commands.empty()) commands.pop();
    Serial.println("onDisconnect - will restart scan");
    doScan = true;
  }
};

// ── Scan callback ─────────────────────────────────────────────────────────────
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    Serial.print("Found: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.getName().indexOf("FlxTm") != -1 ||
        (advertisedDevice.haveServiceUUID() &&
         advertisedDevice.isAdvertisingService(serviceUUID))) {

      Serial.println(" - Target found, stopping scan");
      BLEDevice::getScan()->stop();
      delete myDevice;
      myDevice  = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan    = false;
    }
  }
};

// ── Connect + handshake ───────────────────────────────────────────────────────
static bool connectToServer() {
  gotInit = false;

  Serial.print("Connecting to ");
  Serial.println(myDevice->getAddress().toString().c_str());

  if (pClient == nullptr) {
    pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());
  }

  if (!pClient->connect(myDevice)) {
    Serial.println(" - Failed to connect");
    return false;
  }
  Serial.println(" - Connected");

  BLERemoteService* pService = pClient->getService(serviceUUID);
  if (!pService) {
    Serial.println(" - Failed to find service");
    pClient->disconnect();
    return false;
  }

  pWriteChar  = pService->getCharacteristic(wcharUUID);
  pNotifyChar = pService->getCharacteristic(ncharUUID);

  if (!pWriteChar || !pNotifyChar) {
    Serial.println(" - Missing characteristic(s)");
    pClient->disconnect();
    return false;
  }

  if (!pNotifyChar->canNotify()) {
    Serial.println(" - NOTIFY not supported");
    pClient->disconnect();
    return false;
  }

  pNotifyChar->registerForNotify(notifyCallback);

  unsigned long start = millis();
  while (!gotInit && millis() - start < 500) delay(10);

  Serial.println(gotInit ? " - Got notify" : " - No notify yet, continuing");

  connected = true;

  Connection_enqueue("FM?3,CONNEC", 3000);
  Connection_enqueue("ME");
  for (int i = 0; i < 10; i++) {
    Connection_enqueue("XM?0000" + std::to_string(i));
  }
  return true;
}

// ── Public API ────────────────────────────────────────────────────────────────
void Connection_init() {
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(60, false);
}

void Connection_update() {
  if (doConnect) {
    if (connectToServer()) Serial.println("Connected to BLE server.");
    else                   Serial.println("Connection failed.");
    doConnect = false;
  }

  if (connected && pWriteChar && !commands.empty()) {
    Command curr = commands.front();
    commands.pop();

    Serial.print("Sending Command: ");
    Serial.println(curr.body.c_str());
    pWriteChar->writeValue((uint8_t*)curr.body.c_str(), curr.body.length(), false);

    if (curr.delayMs > 0) delay(curr.delayMs);
  }

  if (!connected && doScan) {
    doScan = false;
    BLEDevice::getScan()->clearResults();
    BLEDevice::getScan()->start(0, false);
  }
}

void Connection_enqueue(std::string payload, unsigned long delayMs) {
  std::string body = payload.back() == ';' ? payload : payload + ";";
  size_t len = body.length();
  size_t i = 0;

  while (len > 0) {
    size_t chunkSize = (len >= 20) ? 20 : len;
    std::string chunk = body.substr(i, chunkSize);

    unsigned long chunkDelay = (len <= 20) ? delayMs : 0;
    commands.push(Command(chunk, chunkDelay));

    i += chunkSize;
    len -= chunkSize;
  }
}