#pragma once

// AUTHOR: Colin Skinner / Cesium FSW
// VERSION: X.X.X
// PURPOSE: Wrapper code for LittleFS

#include <Arduino.h>
#include <LittleFS.h>

namespace Cesium {

class FileSystem {

public:

    FileSystem();

    bool begin(bool format_filesystem = false);
    
    bool listDir(const char *dirname);
    void fileTree(const char *dirname);
    bool createDir(const char *dirname);
    bool removeDir(const char *dirname);

    bool readFile(const char *path);
    bool writeFile(const char *path, const char *message);
    bool appendFile(const char *path, const char *message);
    bool renameFile(const char *from_path, const char *to_path);
    bool deleteFile(const char *path);

    bool testFileIO(const char *path);

    bool usageBytes(size_t total, size_t used);
    float usageFraction() const;

private:
    const char* partition_label = "spiffs";
    void fileTreeHelper(File root, uint8_t tabs = 0);
};

}


// filesystem.begin();


// filesystem.fileTree("/");
// filesystem.createDir("/mydir");

// filesystem.appendFile("/mydir/hello2.txt", "Hello2");
// filesystem.fileTree("/");
// filesystem.listDir("/");
// filesystem.deleteFile("/mydir/hello2.txt");
// filesystem.removeDir("/mydir");
// filesystem.listDir("/");
// filesystem.writeFile("/hello.txt", "Hello ");
// filesystem.writeFile("/hello.txt", "World!\r\n");
// filesystem.readFile("/hello.txt");
// filesystem.renameFile("/hello.txt", "/foo.txt");
// filesystem.readFile("/foo.txt");
// filesystem.deleteFile("/foo.txt");
// filesystem.testFileIO("/test.txt");
// size_t a, b;
// filesystem.usageBytes(a, b);
// Serial.println(filesystem.usageFraction());
// filesystem.deleteFile("/test.txt");
// filesystem.usageBytes(a, b);
// Serial.println(filesystem.usageFraction());
// Serial.println("Test complete");