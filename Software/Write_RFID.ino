#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;     // Reset pin for MFRC522 module
constexpr uint8_t SS_PIN = D4;      // Slave select pin for MFRC522 module

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create an instance of the MFRC522 class
MFRC522::MIFARE_Key key;             // Key used for authentication

// Block number where data will be written
int blockNum = 2;  

// Data to be written to the card (16 bytes)
byte blockData[16] = {"WheelChair_4"};

// Buffer for reading data from the block (18 bytes)
byte bufferLen = 18;
byte readBlockData[18];

MFRC522::StatusCode status;          // Variable to hold the status of operations

void setup() 
{
  Serial.begin(9600);                // Initialize serial communication at 9600 baud rate
  SPI.begin();                       // Initialize SPI bus
  mfrc522.PCD_Init();               // Initialize the MFRC522 module
  Serial.println("Scan a MIFARE 1K Tag to write data...");
}

void loop()
{
  // Prepare the key for authentication (default is FFFFFFFFFFFFh)
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  // Check for new card presence
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return; // Exit if no new card is detected
  }
  
  // Select the card
  if (!mfrc522.PICC_ReadCardSerial()) 
  {
    return; // Exit if card selection fails
  }

  Serial.print("\n**Card Detected**\n");

  // Print the UID of the card
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  
  Serial.print("\nPICC type: ");
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
         
  // Write data to the specified block
  Serial.println("\nWriting to Data Block...");
  WriteDataToBlock(blockNum, blockData);
  
  // Read data from the same block
  Serial.println("\nReading from Data Block...");
  ReadDataFromBlock(blockNum, readBlockData);
  
  // Print the data read from block
  Serial.print("\nData in Block: ");
  Serial.print(blockNum);
  Serial.print(" --> ");
  for (int j = 0; j < 16; j++)
  {
    Serial.write(readBlockData[j]);
  }
  Serial.print("\n");
}

void WriteDataToBlock(int blockNum, byte blockData[]) 
{
  // Authenticate the block for write access using Key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  Serial.println("Authentication success");

  // Write data to the block
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Writing to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  Serial.println("Data was written into Block successfully");
}

void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
  // Authenticate the block for read access using Key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Read: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  Serial.println("Authentication success");

  // Read data from the block
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  Serial.println("Block was read successfully");  
}
