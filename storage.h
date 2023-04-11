#ifndef STORAGE_H_
#define STORAGE_H_

#include <FS.h>
#include <SD.h>

class Storage {

  private:
    File file;
    const char* idpath ="/id.dat";
    const char* idxpath ="/idx.dat";
    char datasetpath[40];
    char datasetname[40];
    char timestamp[40];

    int flag;
    int id;

  public:
    void begin(void);
    void new_dataset(const char* datetime);
    void add_row(char* row);
    void close_dataset();
    void list(const char* filename) ;
    void list_ID();
    void list_IDX();
    const char* get_datasetname();

    void cleanup();
};

#endif
  
