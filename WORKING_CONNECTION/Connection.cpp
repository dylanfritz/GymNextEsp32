#include "Connection.h"

// ── UUIDs ────────────────────────────────────────────────────────────────────
static BLEUUID serviceUUID("028c8db0-fb17-11e4-a322-1697f925ec7b");
static BLEUUID wcharUUID  ("028c8db2-fb17-11e4-a322-1697f925ec7b"); // WRITE
static BLEUUID ncharUUID  ("028c8db1-fb17-11e4-a322-1697f925ec7b"); // NOTIFY

// ── State ────────────────────────────────────────────────────────────────────
static bool doConnect    = false;
static bool connected    = false;
static bool doScan       = false;
static bool sentCommands = false;

static BLERemoteCharacteristic* pWriteChar = nullptr;
static BLERemoteCharacteristic* pNotifyChar = nullptr;
static BLEAdvertisedDevice*     myDevice    = nullptr;

volatile static bool gotInit = false;

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
  void onConnect(BLEClient*) override    { Serial.println("Client connected"); }
  void onDisconnect(BLEClient*) override { connected = false; Serial.println("onDisconnect"); }
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
      myDevice  = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan    = true;
    }
  }
};

// ── Connect + handshake ───────────────────────────────────────────────────────
static bool connectToServer() {
  gotInit = false;

  Serial.print("Connecting to ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient* pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  if (!pClient->connect(myDevice)) {
    Serial.println(" - Failed to connect");
    return false;
  }
  Serial.println(" - Connected");

  BLERemoteService* pService = pClient->getService(serviceUUID);
  if (!pService) { pClient->disconnect(); return false; }

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

  connected    = true;
  sentCommands = false; // allow commands to fire after reconnect too
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

  if (connected && !sentCommands && pWriteChar) {
    sentCommands = true;
    delay(200);

    const char* fmCmd = "FM?3,CONNEC;";
    Serial.print("Sending FM: "); Serial.println(fmCmd);
    pWriteChar->writeValue((uint8_t*)fmCmd, strlen(fmCmd), false);
    delay(200);

    const char* wmCmd = "WM?p00p;";
    Serial.print("Sending WM: "); Serial.println(wmCmd);
    pWriteChar->writeValue((uint8_t*)wmCmd, strlen(wmCmd), false);

    Serial.println("Commands sent.");
  }

  if (!connected && doScan) {
    BLEDevice::getScan()->start(0);
  }
}