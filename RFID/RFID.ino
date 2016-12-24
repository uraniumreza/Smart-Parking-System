/*
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program showing how to read data from a PICC to serial.
   --------------------------------------------------------------------------------------------------------------------
   This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

   Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
   Reader on the Arduino SPI interface.

   When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
   then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
   you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
   will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
   when removing the PICC from reading distance too early.

   If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
   So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
   details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
   keep the PICCs at reading distance until complete.

   @license Released into the public domain.

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

   The circuit for LCD:
   LCD RS pin to digital pin 7 // 12
   LCD Enable pin to digital pin 6 //11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)
*/

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

/*
   F7 4B C8 46
   35 68 2B 7B
   F5 90 2C 7B
   17 DC 4B DD
*/

String card[] = {"DUMMY", "35 68 2B 7B", "F5 90 2C 7B", "17 DC 4B DD", "F7 4B C8 46", "35 AE E9 E5"};
char signals[] = {'X','A','B','C','D','E','P','Q','R','S','T'};
int carParked[20];

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Welcome to SPS!");

  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  //Serial.println("Approximate your card to the reader...");
  //Serial.println();
  digitalWrite(8, carParked);
  delay(1500);

  for (int i = 0; i < 10; i++) carParked[i] = 0;

  lcd.clear();
}


void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  //Show UID on serial monitor
  //Serial.print("UID tag :");
  String content = "";
  byte letter;
  //Serial.print("///");
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  //Serial.println();
  content.toUpperCase();
  //Serial.println(content);


  //Serial.print("Message : ");
  for (int i = 1; i <= 5; i++)
  {
    if (content.substring(1) == card[i] && carParked[i] == 0) //change here the UID of the card/cards that you want to give access
    {
      lcd.setCursor(0, 0);
      lcd.clear();
      //Serial.print("Car Parking Access Granted");
      Serial.print(signals[i]);
      lcd.print("Car Parking-");
      lcd.setCursor(0, 1);
      lcd.print("Access Granted!");
      delay(3000);
      lcd.clear();
      carParked[i] = 1;
    }

    else if (content.substring(1) == card[i] && carParked[i] == 1) //change here the UID of the card/cards that you want to give access
    {
      lcd.setCursor(0, 0);
      lcd.clear();
      //Serial.print("Car Removed from Parking Lot");
      Serial.print(signals[i+5]);
      lcd.print("Car Removed-");
      lcd.setCursor(0, 1);
      lcd.print("From Parking Lot");
      delay(3000);
      lcd.clear();
      carParked[i] = 0;
    }
  }

//  mfrc522.flush();
  
  //Serial.println("\n\n");

  lcd.print("Welcome to SPS!");
}
