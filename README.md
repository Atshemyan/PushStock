---

## Introduction
This program allows users to work with virtual stocks and cryptocurrencies. It supports four custom cryptocurrencies: VHUK, HKE, VOYL, and NARSMOKE. The value of these cryptocurrencies is updated every second and is stored on a server. This program has a graphical user interface (GUI) that lets users register, login, view graphs of cryptocurrency values, manage their balance, buy/sell cryptocurrencies, and more.

---

### 📌 Requirements

To successfully build and run the program, ensure you have the following requirements:

- Qt version 3.5 or higher.
- QChart module.
- Qt Designer for building the user interface.


## Features

1. **User Registration and Login**: New users must first register to create an account. Once registered, they can login to access the main functionalities of the app.
  
2. **Real-time Value Graph**: Upon logging in, users are greeted with a graph displaying the value of one of the cryptocurrencies. Users can switch between different cryptocurrencies using the change button.

3. **Buy/Sell Cryptocurrencies**: Users can buy or sell the currently displayed cryptocurrency. Transactions are logged and displayed in the interface.

4. **Balance Management**: Users can check their current balance and deposit funds using the deposit button.

5. **Transaction Log**: The application displays all transactions made by the users.

6. **Logout Feature**: Users can safely logout of the application using the exit button.

## How to Build and Run

1. **Setting up the environment**: 
   - Ensure you have the required version of Qt, QChart, and Qt Designer installed.
   - Clone the repository or download the source code.

2. **Building with Qt Designer**:
   - Open the main project file with Qt Designer.
   - Go to `Build` > `Build Project`.
   - Once the build is successful, you can run the application.

3. **Using the application**:
   - Start the program.
   - If you're a new user, click on `register` to create an account. If you're an existing user, click on `login`.
   - After logging in, use the GUI to navigate through the various features like viewing the graph, buying/selling cryptocurrencies, depositing funds, etc.

## Disclaimer

This program is for educational and emulation purposes only. The cryptocurrencies mentioned are virtual and have no real-world value. Please use responsibly.



### 🚀 Getting Started

1. **Initial Landing Page**:
    - As a first-time user, you will be presented with two choices: *Login* or *Register*.

2. **Registration**:
    - Click on *Register* to start your journey.
    - Ensure you fill out all the mandatory fields.
    - Password must contain at least 6 characters.
    - The phone number should start with '0' (for instance, 033099099) and be a minimum of 9 characters long.
    - Upon filling out the details, hit the 'Register' button situated at the bottom.
    - Correct entries will redirect you back to the landing page. Incorrect entries will result in red-highlighted fields, indicating that you need to correct them.

3. **Login**:
    - Post successful registration, choose *Login*.
    - Enter the credentials you provided during the registration process.
    - If verified successfully, you will be redirected to the main Stock page.

---

### 📈 Stock Management

- **Accounts & Balances**:
    - View your balance in USD as well as in specific coins like VHUK, HKE, NARSMOKE, and VOYL.

- **Deposit**:
    - Want to increase your USD balance? Click on the orange *Deposit* button.
    - You'll be taken to a page where you can specify the amount you wish to add. Confirming will update your balance.

- **Interface Overview**:
    - The *Exit* button allows you to safely log out from your account.
    - Centrally located, you'll notice a dynamic chart displaying price-time relations for your chosen coin. By default, the VHUK coin chart is displayed.
    - Coin prices are represented numerically and will be colored either red (if the price dropped) or green (if the price increased).

- **Trading Options**:
    - Below the chart on the right, you have two options: *BUY* and *SELL*.
    - Clicking on *BUY* will pop up a window prompting you to input the dollar amount you want to invest.
    - *SELL* opens up a window where you need to specify the number of coins you wish to sell.
    - The actions of *BUY* or *SELL* will be based on the coin currently selected.

- **Transactions Feed**:
    - On the bottom left, you can track the latest transactions executed by you and other users of PushStock.
    - The feed has space for up to 20 transactions. As new transactions come in, the oldest will be replaced, ensuring you always have the most recent data without an overwhelming list.

---

Thank you for choosing **PushStock** for your stock management needs. Happy trading! 📊🚀
