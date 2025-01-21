
#include <Arduino.h>
#include "common/os/filesystem.h"
#include "common/globals.h"

#define FORMAT_LITTLEFS_IF_FAILED true

using namespace Cesium;
using namespace std;

#define LED 2

FileSystem filesystem;

void setup() {
    Serial.begin(115200);

    filesystem.begin(true);

    filesystem.listDir("/");

    size_t total, size;
    filesystem.usageBytes(total, size);
    Serial.println(total);
    Serial.println(size);

    static uint8_t buf[512];
    for (int i = 0; i < 2048; i++) {
        if ((i & 0x001F) == 0x001F) {
            Serial.print(".");
        }
        filesystem.writeFile("/wow.txt", buf, 512);
    }
    
    filesystem.usageBytes(total, size);
    Serial.println(total);
    Serial.println(size);

    // filesystem.writeFile("/wow2.txt", "hahahaha");
    // filesystem.usageBytes(total, size);

    filesystem.deleteFile("/wow.txt");
    filesystem.usageBytes(total, size);
    Serial.println(total);
    Serial.println(size);

    filesystem.end();


    // filesystem.deleteFile("/wow2.txt");
    // filesystem.usageBytes(total, size);



    // filesystem.deleteFile("/test/wow.txt");

    // filesystem.rmdir("/test", true);
    // filesystem.mkdir("/test");

    // filesystem.writeFile("/test/wow.txt", "ha");

    // filesystem.listDir("/test");

    // filesystem.deleteFile("/test/wow.txt");

    // filesystem.listDir("/test");

    // Serial.println("--------------");

    // filesystem.rmdir("/test", true);
    // filesystem.mkdir("/test");
    
    // filesystem.mkdir("/test/new_folder");
    // filesystem.writeFile("/test/new_folder/hello.txt","Hello!"); 
    // filesystem.mkdir("/test/new_folder/folder_2"); 
    // filesystem.writeFile("/test/new_folder/folder_2/hello.txt","Hello!"); 

    // Serial.println(filesystem.rmdir("/test", true));

    // filesystem.mkdir("/test");
    // auto files = filesystem.listDir("/test");
    // Serial.println(files.size());

}

void loop() {
    // filesystem.createDir("/mydir"); // Create a mydir folder
    // filesystem.writeFile("/mydir/hello1.txt", "Hello1"); // Create a hello1.txt file with the content "Hello1"
    ; // List the directories up to one level beginning at the root directory

    
    // filesystem.deleteFile("/mydir/hello1.txt"); //delete the previously created file
    // filesystem.removeDir("/mydir"); //delete the previously created folder
    // filesystem.listDir("/", 1); // list all directories to make sure they were deleted
        
    // filesystem.writeFile("/hello.txt", "Hello "); //Create and write a new file in the root directory
    // filesystem.appendFile("/hello.txt", "World!\r\n"); //Append some text to the previous file
    // filesystem.readFile("/hello.txt"); // Read the complete file
    // filesystem.renameFile("/hello.txt", "/foo.txt"); //Rename the previous file
    // filesystem.readFile("/foo.txt"); //Read the file with the new name
    // filesystem.deleteFile("/foo.txt"); //Delete the file
    // filesystem.testFileIO("/test.txt"); //Testin
    // filesystem.deleteFile("/test.txt"); //Delete the file
}
