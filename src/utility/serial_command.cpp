#include "utility/serial_command.h"
#include "utility/helper.h"
#include "globals.h"
#include <LittleFS.h>
#include <Arduino.h>

String passwordFilePath = "/meta_data.ps";
namespace SerialCommand
{
  void passwordAdd(const String& command)
  {
    int firstSpace = command.indexOf(' ');
    int secondSpace = command.indexOf(' ', firstSpace + 1);
    if (firstSpace == -1 || secondSpace == -1) {
      Serial.println("[passwordAdd] Usage: passwordAdd <label> <password>");
      return;
    }
    String label = command.substring(firstSpace + 1, secondSpace);
    String password = command.substring(secondSpace + 1);
    if (label.length() == 0 || password.length() == 0) {
      Serial.println("[passwordAdd] Usage: passwordAdd <label> <password>");
      return;
    }

    String xorString = label + ":" + password;
    String encrypted = Helper::xorEncrypt(xorString, xorKey);
    
    File file = LittleFS.open(passwordFilePath, "a");
    if (!file) file = LittleFS.open(passwordFilePath, "w");
    if (file) {
      file.println(encrypted);
      file.close();
      Serial.println("[passwordAdd] Password saved to LittleFS.");
    } else {
      Serial.println("[passwordAdd] Failed to open or create file for writing.");
    }
  }

  void passwordDelete(const String& command) {
    String label = command.substring(13);
    label.trim();
    if (label.length() == 0) {
      Serial.println("[del_password] Usage: del_password <label>");
      return;
    }

    // Read all lines except the one to delete
    File file = LittleFS.open(passwordFilePath, "r");
    if (!file) {
      Serial.println("[del_password] No password file found.");
      return;
    }
    bool deleted = false;
    String newContent = "";
    while (file.available()) {
      String line = file.readStringUntil('\n');
      line.trim(); // Remove any leading/trailing whitespace
      if (line.length() == 0) continue; // Skip empty lines
      String decrpted = Helper::xorEncrypt(line, xorKey);
      if (!decrpted.startsWith(label + ":")) {
        newContent += line + "\n";
      } else {
        deleted = true;
      }
    }
    file.close();

    if (!deleted) {
      Serial.println("[del_password] No entry found with that label.");
      return;
    }
    // Write back filtered content
    file = LittleFS.open(passwordFilePath, "w");
    if (file) {
      file.print(newContent);
      file.close();
      Serial.println("[del_password] Entry deleted (if it existed).");
    } else {
      Serial.println("[del_password] Failed to write file.");
    }
  }

  void listen()
  {
    if (Serial.available() <= 0) return;

    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("add_password ")) {
      passwordAdd(command);
    } else if (command.startsWith("del_password ")) {
      passwordDelete(command);
    } else if (command == "clr_password") {
      File file = LittleFS.open(passwordFilePath, "w");
      if (file) {
        file.close();
        Serial.println("[clr_password] All passwords cleared.");
      } else {
        Serial.println("[clr_password] Failed to clear passwords.");
      }
    } else if (command == "help") {
      Serial.println("Available commands:");
      Serial.println("add_password <label> <password> - Add password to list");
      Serial.println("del_password <label> - Delete password by label");
      Serial.println("clr_password - Delete all passwords");
    } else {
      Serial.println("Unknown command. Type 'help' for available commands.");
    }
  }
}