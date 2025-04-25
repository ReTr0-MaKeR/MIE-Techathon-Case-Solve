#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button pins
#define BTN1 15  // Reset
#define BTN2 2   // Select / Confirm
#define BTN3 4   // Scroll up / Increment
#define BTN4 16  // Scroll down / Decrement

// Menu data
String menu[] = {"Burger", "Pizza", "Pasta", "Fries", "Drink"};
int menuSize = 5;

// State vars
int menuIndex = 0;
int quantity = 1;
bool inQuantityDialog = false;

struct OrderItem {
  String name;
  int quantity;
};
std::vector<OrderItem> cart;

void setup() {
  Serial.begin(115200);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  showMenu();
}

void loop() {
  static bool btn2Pressed = false;
  static unsigned long btn2StartTime = 0;

  // Reset (BTN1)
  if (digitalRead(BTN1) == LOW) {
    resetOrder();
    delay(300);
  }

  // BTN2 Press Logic
  if (digitalRead(BTN2) == LOW && !btn2Pressed) {
    btn2Pressed = true;
    btn2StartTime = millis();
  }

  if (btn2Pressed && digitalRead(BTN2) == HIGH) {
    unsigned long pressDuration = millis() - btn2StartTime;
    btn2Pressed = false;

    if (pressDuration > 1000) {
      simulateOrderPlacement(); // Long press: place order
    } else {
      if (inQuantityDialog) {
        addToCart();
      } else {
        inQuantityDialog = true;
        quantity = 1;
        showQuantityDialog();
      }
    }
    delay(300);
  }

  // BTN3 - Scroll Up or Increase
  if (digitalRead(BTN3) == LOW) {
    if (inQuantityDialog) {
      quantity++;
      showQuantityDialog();
    } else {
      menuIndex = (menuIndex - 1 + menuSize) % menuSize;
      showMenu();
    }
    delay(200);
  }

  // BTN4 - Scroll Down or Decrease
  if (digitalRead(BTN4) == LOW) {
    if (inQuantityDialog) {
      quantity = max(1, quantity - 1);
      showQuantityDialog();
    } else {
      menuIndex = (menuIndex + 1) % menuSize;
      showMenu();
    }
    delay(200);
  }
}

void showMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Select Item:");
  for (int i = 0; i < menuSize; i++) {
    if (i == menuIndex) display.print("> ");
    else display.print("  ");
    display.println(menu[i]);
  }
  display.setCursor(0, 56);  // Bottom line
  display.print("Hold Select to order");
  display.display();
}

void showQuantityDialog() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Qty for:");
  display.println(menu[menuIndex]);
  display.print("Quantity: ");
  display.println(quantity);
  display.println("Press Btn2 to confirm");
  display.display();
}

void addToCart() {
  OrderItem item = { menu[menuIndex], quantity };
  cart.push_back(item);
  inQuantityDialog = false;
  showMenu();
}

void resetOrder() {
  cart.clear();
  inQuantityDialog = false;
  menuIndex = 0;
  quantity = 1;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Order Reset");
  display.display();
  delay(1000);
  showMenu();
}

void simulateOrderPlacement() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Order Summary:");

  for (int i = 0; i < cart.size(); i++) {
    display.print(cart[i].name);
    display.print(" x");
    display.println(cart[i].quantity);
    if (i == 3) break; // Limit to 4 lines
  }

  if (cart.size() > 4) {
    display.println("...more items");
  }

  display.println("Order Sent (offline)");
  display.display();

  delay(3000);
  cart.clear();
  showMenu();
}
