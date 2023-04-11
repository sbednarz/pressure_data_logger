#include <FS.h>
#include <SD.h>
#include "storage.h"

void Storage::begin(void) {
      Serial.print("# storage: SD card init ...");
      if (!SD.begin()) {
        Serial.println("# storage error: initialization failed.");
            while (1);
        } else {
        Serial.println(" OK");
      }
      flag = 0;
    }

void Storage::new_dataset(const char* t) {
    if (flag==2) {
      Serial.println("# storage error: close dataset first error");
      while(1);      
    }
    flag = 1;
    sprintf(timestamp,"%s",t);
    file = SD.open(idpath);
    if(!file){
      Serial.println("# storage error: Failed to open ID file for reading");
      while(1);
    }
    int lastID = 0;
    while(file.available()){
      lastID = file.parseInt();
      //Serial.println(lastID);
    }
    file.close();
    id = lastID + 1;
    sprintf(datasetpath, "/%d.csv", id);
    sprintf(datasetname, "%d.csv", id);
    Serial.println(String("# storage: new dataset: ")+String(datasetpath));
    char buff[80];
    sprintf(buff,"# %s",datasetname);
    add_row(buff);
  }

void Storage::add_row(char* row){
      if (flag==0) {
        Serial.println("# storage error: addRow error");
        while(1);
      }
      flag = 2;
      File file = SD.open(datasetpath, FILE_APPEND);
      if(!file){
        Serial.println(String("# storage error: failed to open file for writing: ")+String(datasetpath));
        while(1);
      }
      if(file.println(row)){
        //Serial.println("row written");
      } else {
        Serial.println("# storage error: row writting failed");
        while(1);
      }
      file.close();
  }


void Storage::close_dataset(){
      flag = 3;
      File file = SD.open(idxpath, FILE_APPEND);
      if(!file){
        Serial.println("# storage error: failed to open file IDX for writing");
        while(1);
      }
      char row[100];
      
      Serial.println("timestamp in storage - close_dataset");
      Serial.println(timestamp);

      sprintf(row, "/%d.csv %s", id, timestamp);
      if(file.println(row)){
        Serial.println("IDX row written");
        Serial.println(row);
      } else {
        //Serial.println("IDX row writting failed");
        while(1);
      }
      file.close();

    file = SD.open(idpath, FILE_WRITE);
    if(!file){
      Serial.println("# storage error: failed to open ID file for reading");
      while(1);
    }
    
    if(file.print(id)){
      //Serial.println("ID row written");
    } else {
      //Serial.println("ID row writting failed");
      while(1);
    }
    file.close();
  }

void Storage::list(const char* filename) {
  file = SD.open(filename, FILE_READ);
  if(!file){
    Serial.println("Failed to open file for reading:");
    Serial.println(filename);
    while(1);
  }

  char buff[1024];

  while(file.available()){
    file.readBytes(buff,1024);
    Serial.println(buff);
  }

  file.close();
}

  void Storage::list_ID() {
    list("/id.dat");
  }

  void Storage::list_IDX() {
    list("/idx.dat");
  }

  const char* Storage::get_datasetname(){
    return datasetname;
  }
  
void Storage::cleanup() {
    Serial.println("Cleaning up SD card!");
    for (int i = 100; i<1000; i++){
        char buff[40];
        sprintf(buff,"/%d.csv",i);
        Serial.println(buff);
        SD.remove(buff);
    }
    SD.remove("/id.dat");
    SD.remove("/idx.dat");
    
    file = SD.open("/id.dat", FILE_WRITE);
    if(!file){
        Serial.println("# storage error: failed to open ID file for writing");
        while(1);
    }
    if(file.print(100)){
        //Serial.println("ID row written");
    } else {
        Serial.println("ID writting failed");
        while(1);
    }
    file.close();
}

