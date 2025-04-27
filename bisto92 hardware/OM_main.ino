#include <Wire.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// OLED setup for SH1106 (1.3" OLED, I2C)
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Button pins
#define BTN1 2    // Reset
#define BTN2 4    // Select / Confirm
#define BTN3 16   // Scroll up / Increment
#define BTN4 17   // Scroll down / Decrement
#define BUZZER 15 // Buzzer

// DIP Switch pins
#define DIP1 25
#define DIP2 26
#define DIP3 33
#define DIP4 32

// Wi-Fi credentials
#define WIFI_SSID "Retro"
#define WIFI_PASSWORD "Retro_Maker"

// Firebase credentials
#define API_KEY "AIzaSyCqRrMlG-INP1a0Sj9IKBgBsLjooLJhjMw"
#define DATABASE_URL "https://order-management-system-edb74-default-rtdb.asia-southeast1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

// Menu Data
String menu[] = {"Burger", "Pizza", "Pasta", "Fries", "Drink"};
int menuSize = sizeof(menu) / sizeof(menu[0]);

// State Variables
int menuIndex = 0;
int quantity = 1;
bool inQuantityDialog = false;
int currentTableNo = 1;

struct OrderItem {
  String name;
  int quantity;
};
std::vector<OrderItem> cart;

// ------ Setup ------
void setup() {
  Serial.begin(115200);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  // DIP Switch pins
  pinMode(DIP1, INPUT_PULLUP);
  pinMode(DIP2, INPUT_PULLUP);
  pinMode(DIP3, INPUT_PULLUP);
  pinMode(DIP4, INPUT_PULLUP);

  display.begin();
  display.setFont(u8g2_font_profont12_mf);

  currentTableNo = readTableNumber();

  showStartupScreen();  // <--- ADD STARTUP SCREEN FIRST

  // Wi-Fi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println("Connected to WiFi");

  // Firebase connection
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase signup OK");
    signupOK = true;
  } else {
    Serial.printf("Firebase signup failed: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  showMenu();
}

// ------ Main Loop ------
void loop() {
  static bool btn2Pressed = false;
  static unsigned long btn2StartTime = 0;

  if (digitalRead(BTN1) == LOW) {
    buzzLong();
    resetOrder();
    delay(300);
  }

  if (digitalRead(BTN2) == LOW && !btn2Pressed) {
    btn2Pressed = true;
    btn2StartTime = millis();
  }

  if (btn2Pressed && digitalRead(BTN2) == HIGH) {
    unsigned long pressDuration = millis() - btn2StartTime;
    btn2Pressed = false;

    if (pressDuration > 1000) {
      buzzLong();
      simulateOrderPlacement();
    } else {
      buzzShort();
      if (inQuantityDialog) {
        updateCart();
      } else {
        inQuantityDialog = true;
        quantity = getPreviousQuantity(menu[menuIndex]);
        showQuantityDialog();
      }
    }
    delay(300);
  }

  if (digitalRead(BTN3) == LOW) {
    buzzShort();
    if (inQuantityDialog) {
      quantity++;
      showQuantityDialog();
    } else {
      menuIndex = (menuIndex + 1) % menuSize;
      showMenu();
    }
    delay(200);
  }

  if (digitalRead(BTN4) == LOW) {
    buzzShort();
    if (inQuantityDialog) {
      quantity = max(0, quantity - 1);
      showQuantityDialog();
    } else {
      menuIndex = (menuIndex - 1 + menuSize) % menuSize;
      showMenu();
    }
    delay(200);
  }
}

// ------ Display Functions ------
void showMenu() {
  display.clearBuffer();
  display.setFont(u8g2_font_profont12_mf);

  int leftWidth = 64;
  int rightStartX = 64;

  display.drawFrame(0, 0, 128, 64);

  display.setFont(u8g2_font_profont10_mf);
  display.setCursor((leftWidth - display.getStrWidth("Welcome")) / 2, 12);
  display.print("Welcome");

  display.drawCircle(leftWidth/2, 40, 18);
  display.setFont(u8g2_font_profont12_mf);

  String tableText = "T" + String(currentTableNo);
  display.setCursor((leftWidth - display.getStrWidth(tableText.c_str())) / 2, 44);
  display.print(tableText);

  display.setFont(u8g2_font_profont12_mf);
  int itemsPerPage = 4;
  int pageStart = (menuIndex / itemsPerPage) * itemsPerPage;

  for (int i = 0; i < itemsPerPage; i++) {
    int itemIndex = pageStart + i;
    if (itemIndex >= menuSize) break;

    int y = 20 + i * 12;

    if (itemIndex == menuIndex) {
      display.drawBox(rightStartX + 5, y - 10, 54, 12);
      display.setDrawColor(0);
      int textWidth = display.getStrWidth(menu[itemIndex].c_str());
      display.setCursor(rightStartX + 5 + (54 - textWidth)/2, y);
      display.print(menu[itemIndex]);
      display.setDrawColor(1);
    } else {
      int textWidth = display.getStrWidth(menu[itemIndex].c_str());
      display.setCursor(rightStartX + 5 + (54 - textWidth)/2, y);
      display.print(menu[itemIndex]);
    }
  }

  display.sendBuffer();
}

void showQuantityDialog() {
  display.clearBuffer();
  display.setFont(u8g2_font_profont12_mf);

  display.drawFrame(0, 0, 128, 64);

  int centerX = 64;

  display.setCursor(centerX - 30, 20);
  display.println("Select Qty:");

  display.setCursor(centerX - (display.getStrWidth(menu[menuIndex].c_str())/2), 35);
  display.print(menu[menuIndex]);

  char qtyText[10];
  sprintf(qtyText, "x %d", quantity);
  display.setCursor(centerX - (display.getStrWidth(qtyText)/2), 50);
  display.print(qtyText);

  display.sendBuffer();
}

void showStartupScreen() {
  display.clearBuffer();
  display.setFont(u8g2_font_profont12_mf);

  display.drawFrame(0, 0, 128, 64);

  int centerX = 64;

  display.setCursor(centerX - (display.getStrWidth("Bistro 92") / 2), 20);
  display.print("Bistro 92");

  String tableInfo = "Table: T" + String(currentTableNo);
  display.setCursor(centerX - (display.getStrWidth(tableInfo.c_str()) / 2), 35);
  display.print(tableInfo);

  display.setCursor(centerX - (display.getStrWidth("Connecting...") / 2), 50);
  display.print("Connecting...");

  display.sendBuffer();
}

// ------ Cart Management ------
void updateCart() {
  bool found = false;
  for (int i = 0; i < cart.size(); i++) {
    if (cart[i].name == menu[menuIndex]) {
      found = true;
      if (quantity == 0) {
        cart.erase(cart.begin() + i);
      } else {
        cart[i].quantity = quantity;
      }
      break;
    }
  }

  if (!found && quantity > 0) {
    OrderItem item = { menu[menuIndex], quantity };
    cart.push_back(item);
  }

  inQuantityDialog = false;
  showMenu();
}

int getPreviousQuantity(String itemName) {
  for (OrderItem item : cart) {
    if (item.name == itemName) {
      return item.quantity;
    }
  }
  return 0;
}

// ------ Firebase Functions ------
void simulateOrderPlacement() {
  display.clearBuffer();
  display.drawFrame(0, 0, 128, 64);
  display.setFont(u8g2_font_profont12_mf);

  int margin = 5;
  display.setCursor(margin, 12);
  display.println("Order Summary:");

  for (int i = 0; i < cart.size(); i++) {
    display.setCursor(margin, 22 + i * 10);
    display.print(cart[i].name);
    display.print(" x");
    display.print(cart[i].quantity);
    if (i == 3) break;
  }

  if (cart.size() > 4) {
    display.setCursor(margin, 60);
    display.println("...more items");
  }

  display.setCursor(margin, 55);
  display.println("Order Sent!");
  display.sendBuffer();
  delay(3000);

  sendOrderToFirebase();
}

void sendOrderToFirebase() {
  if (signupOK) {
    String tableNo = String(currentTableNo);

    for (int i = 0; i < cart.size(); i++) {
      String itemName = cart[i].name;
      int itemQuantity = cart[i].quantity;

      String itemPath = "/Tables/Table_" + tableNo + "/" + itemName;
      Firebase.RTDB.setInt(&fbdo, itemPath, itemQuantity);
    }

    // Send timestamp
    String timestampPath = "/Tables/Table_" + tableNo + "/Timestamp";
    Firebase.RTDB.setTimestamp(&fbdo, timestampPath);

    Serial.println("Order Uploaded to Firebase!");
  }

  cart.clear();
  showMenu();
}

void resetOrder() {
  cart.clear();

  String tableNo = String(currentTableNo);
  String basePath = "/Tables/Table_" + tableNo;

  Firebase.RTDB.deleteNode(&fbdo, basePath);

  inQuantityDialog = false;
  menuIndex = 0;
  quantity = 1;

  display.clearBuffer();
  display.drawFrame(0, 0, 128, 64);
  display.setCursor(30, 32);
  display.println("Order Reset");
  display.sendBuffer();
  delay(1000);

  showMenu();
}

// ------ Buzzer Feedback ------
void buzzShort() {
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
}

void buzzLong() {
  digitalWrite(BUZZER, HIGH);
  delay(300);
  digitalWrite(BUZZER, LOW);
}

// ------ DIP Switch Reading ------
int readTableNumber() {
  int bit1 = digitalRead(DIP1) == LOW ? 1 : 0;
  int bit2 = digitalRead(DIP2) == LOW ? 1 : 0;
  int bit3 = digitalRead(DIP3) == LOW ? 1 : 0;
  int bit4 = digitalRead(DIP4) == LOW ? 1 : 0;

  int table = (bit1 << 3) | (bit2 << 2) | (bit3 << 1) | bit4;
  if (table == 0) table = 16;

  return table;
}
