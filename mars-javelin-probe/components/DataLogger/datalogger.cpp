#include "datalogger.h"

void vBeginLogLoop_Task(void *pvParameters)
{
  setup();
  for(;;)
    {
      vTaskDelay(1);
    }

  vTaskDelete(NULL);
}

int BMI085_Message(char* message){
  // Just use String class here. We are in C++ so don't make it hard on yourself
  char ts = [32];
  char ac = [32];
  char gy = [64];

  snprintf(ts, "TS %llu\n", toff_micro);
  snprintf(ac, "AC %d %d %d %d\n", ax, ay, az, temp);
  snprintf(gy, "GY %d %d %d %d %d %d\n\n", gx, gy, gz, pitch, roll, yaw);
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file){
    printf("Failed to open file for appending\n");
    return;
  }
  
  if(file.print(message)){
    printf("Message appended\n");
  }
  else{
    printf("Append failed\n");
  }
}

void testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  }else{
    printf("Failed to open file for reading\n");
  }

  file = fs.open(path, FILE_WRITE);
  if(!file){
    printf("Failed to open file for writing\n");
    return;
  }

  size_t i;
  start = millis();
  for(i = 0; i < 2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

void setup(){
  SPIClass *vspi = new SPIClass(VSPI);
  vspi->begin(SPI_2_SCK, SPI_2_MISO, SPI_2_MOSI, SPI_2_CS);

  if(!SD.begin(SPI_2_CS, *vspi)){
    printf("Card Mount Failed!\n");
    return;
  }

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    printf("No SD card attached\n");
    return;
  }

  printf("SD Card Type:");
  switch(cardType){
    case CARD_MMC:
    printf("MMC\n");
      break;
    case CARD_SD:
    printf("SDSC\n");
      break;
    case CARD_SDHC:
    printf("SDHC\n");
      break;
    default:
    printf("UNKOWN\n");
      break;
  }

  uint64_t cardSize = SD.cardSize() / (1024*1024);
  printf("SD Card Size: %lluMB\n", cardSize);

  testFileIO(SD, "/test.txt");

}
