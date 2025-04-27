# Bistro 92 Smart Ordering System

## 📋 Project Overview

This project was built for the **Techathon** competition under the **MIE Robolution 2025** event.  
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
- **OLED Display** (1.3 inch for better visibility)
- **Soft Tactile Push Buttons** (4 Buttons: R, S, +, -)
- **Buzzer** (for button press feedback)

---
### Big Idea Features

#### 1. Augmented Reality (AR) 360° Food View

We implemented an AR feature where users can view a realistic 360° model of the food item using a HIRO marker.  
Currently, the HIRO marker (hiro_box) is used as a reference point placed on the table. In the future, the implementation will move to markerless AR directly on the dining table for a smoother experience.

#### 2. Interactive 3D Food Model (Move and Zoom)

Alongside AR, we also developed a 3D view system where users can freely rotate, move, and zoom into the food model.  
This allows customers to inspect the dishes from different angles, enhancing their decision-making experience before placing an order.

---

- **Circuit:**  
  ```markdown
  ![Alt Text](./bisto92%20hardware/Circuit%20Diagram.png)
---

## 🎯 Why This Solution?

- ✅ **Cost-effective**: One device can serve up to 16 tables.
- ✅ **User-friendly**: Simple button navigation.
- ✅ **Real-time updates**: Orders pushed instantly to the cloud.
- ✅ **Faster service**: Reduces wait time and manual errors.
- ✅ **Scalable and Smart**:

---

> Developed for Techathon 2025 |  By Team Alpha
