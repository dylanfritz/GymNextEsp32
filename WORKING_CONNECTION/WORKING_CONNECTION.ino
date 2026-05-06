#include "BLEDevice.h"

// Service and characteristics
static BLEUUID serviceUUID("028c8db0-fb17-11e4-a322-1697f925ec7b");
static BLEUUID wcharUUID ("028c8db2-fb17-11e4-a322-1697f925ec7b"); // WRITE
static BLEUUID ncharUUID ("028c8db1-fb17-11e4-a322-1697f925ec7b"); // NOTIFY

static bool doConnect   = false;
static bool connected   = false;
static bool doScan      = false;

static BLERemoteCharacteristic* pWriteChar   = nullptr;
static BLERemoteCharacteristic* pNotifyChar  = nullptr;
static BLEAdvertisedDevice*     myDevice     = nullptr;

volatile bool gotInit = false;   // set when first notify arrives
bool sentCommands     = false;   // ensure we only send once

// ---------- NOTIFY CALLBACK ----------
static void notifyCallback(BLERemoteCharacteristic* pChr,
                           uint8_t* pData,
                           size_t length,
                           bool isNotify) {
  Serial.print("Notify from ");
  Serial.print(pChr->getUUID().toString().c_str());
  Serial.print(" len=");
  Serial.println(length);

  Serial.print("data: ");
  for (size_t i = 0; i < length; i++) {
    if (pData[i] < 16) Serial.print("0");
    Serial.print(pData[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  gotInit = true;  // handshake seen
}

// ---------- CLIENT CALLBACKS ----------
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) override {
    Serial.println("Client connected");
  }

  void onDisconnect(BLEClient* pclient) override {
    connected = false;
    Serial.println("onDisconnect");
  }
};

// ---------- CONNECT TO SERVER + HANDSHAKE ----------
bool connectToServer() {
  gotInit = false;   // reset handshake flag

  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient* pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  if (!pClient->connect(myDevice)) {
    Serial.println(" - Failed to connect");
    return false;
  }
  Serial.println(" - Connected to server");

  BLERemoteService* pService = pClient->getService(serviceUUID);
  if (!pService) {
    Serial.println(" - Failed to find service");
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found service");

  // WRITE characteristic
  pWriteChar = pService->getCharacteristic(wcharUUID);
  if (!pWriteChar) {
    Serial.println(" - Failed to find WRITE characteristic");
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found WRITE characteristic");

  // NOTIFY characteristic
  pNotifyChar = pService->getCharacteristic(ncharUUID);
  if (!pNotifyChar) {
    Serial.println(" - Failed to find NOTIFY characteristic");
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found NOTIFY characteristic");

  if (!pNotifyChar->canNotify()) {
    Serial.println(" - NOTIFY characteristic does NOT support notify");
    pClient->disconnect();
    return false;
  }

  Serial.println(" - Subscribing to NOTIFY...");
  pNotifyChar->registerForNotify(notifyCallback);

  Serial.println(" - Waiting briefly for any notify (optional)...");
  unsigned long start = millis();
  while (!gotInit && millis() - start < 500) {  // short, non-critical wait
    delay(10);
  }

  if (gotInit) {
    Serial.println(" - Got a notify packet (status update)");
  } else {
    Serial.println(" - No notify yet (device is just idle, continuing anyway)");
  }

  connected = true;
  return true;
}

// ---------- SCAN CALLBACK ----------
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) override {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.getName().indexOf("FlxTm") != -1 ||
    (advertisedDevice.haveServiceUUID() &&
     advertisedDevice.isAdvertisingService(serviceUUID))) {

      Serial.println(" - Target device found, stopping scan");
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan    = true;
    }
  }
};

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(60, false);
}

// ---------- LOOP ----------
void loop() {
  // First-time connection
  if (doConnect) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("Failed to connect to server.");
    }
    doConnect = false;
  }

  // Once connected and handshake done, send commands ONCE
  if (connected && !sentCommands && pWriteChar != nullptr) {
    sentCommands = true;

    delay(200); // small guard delay after handshake

    // 1) Send FM? (flash message)
    {
      const char* fmCmd = "FM?3,CONNEC;";  // adjust index/text as needed
      Serial.print("Sending FM: ");
      Serial.println(fmCmd);

      pWriteChar->writeValue(
        (uint8_t*)fmCmd,
        strlen(fmCmd),
        false  // Write Without Response
      );
    }

    delay(200);

    // 2) Send WM? (welcome message)
    {
      const char* wmCmd = "WM?HELL0_W0RLD;"; // example from your nRF log
      Serial.print("Sending WM: ");
      Serial.println(wmCmd);

      pWriteChar->writeValue(
        (uint8_t*)wmCmd,
        strlen(wmCmd),
        false  // Write Without Response
      );
    }

    Serial.println("Commands sent. You can now optionally disconnect or leave it.");
  }

  // Optional: restart scan after disconnect
  if (!connected && doScan) {
    BLEDevice::getScan()->start(0);
  }

  delay(100);
}
