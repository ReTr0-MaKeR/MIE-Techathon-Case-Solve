# Bistro 92 Smart Ordering System

## 📋 Project Overview

This project is built for the **Techathon 2025** competition to solve the **Bistro 92** case study challenge.  
We created an efficient, low-cost smart ordering system that transforms the chaotic dining experience into a smooth, modern one.

---

## 🚀 Key Features

- **Single Device, Multiple Tables:**  
  Instead of one device per table, we use **one device to simulate multiple tables (1–16)**.  
  Before booting, users **input a 4-bit binary key** representing the table number (e.g., `00010` = Table 2).

- **Startup Screen:**  
  Displays **Bistro 92**, **Table Number**, and **Wi-Fi connection status**.

- **Main Menu:**  
  - Right side: **Food Menu**.  
  - Left side: **Welcome message with Table No**.  
  - **Button 3 ("+")**: Scroll to next menu item.  
  - **Button 4 ("-")**: Scroll to previous menu item.  
  - **Button 2 ("S")**: Select highlighted menu item.

- **Ordering Process:**  
  - After item selection, users enter the **Quantity Selection** screen.  
  - Use **"+"** (Button 3) to increase or **"-"** (Button 4) to decrease quantity.  
  - **Button 2 ("S")** confirms quantity and adds the item to the cart.  
  - Multiple items can be added.

- **Placing the Order:**  
  - **Long press Button 2 ("S")** until a long beep is heard.  
  - Displays **Order Summary**.  
  - Sends **Order Data** (Table No, Items, Quantity, Timestamp) to **Firebase Database**.

- **Resetting the Order:**  
  - **Button 1 ("R")** clears the cart and returns to the main menu.

- **User Feedback:**  
  - **Buzzer feedback** on every button press for better interaction.

---

## 🛠 Hardware Used

- **ESP32 Microcontroller** (with Wi-Fi)
- **OLED/LCD Display**
- **Push Buttons** (4 Buttons: R, S, +, -)
- **Buzzer** (for button press feedback)

---

## 🎯 Why This Solution?

- ✅ **Cost-effective**: One device can serve up to 16 tables.
- ✅ **User-friendly**: Simple binary input and button navigation.
- ✅ **Real-time updates**: Orders pushed instantly to the cloud.
- ✅ **Faster service**: Reduces wait time and manual errors.
- ✅ **Scalable and Smart**: Firebase database integration for real-time order management.

---

> Developed for Techathon 2025 |  By Team Alpha
