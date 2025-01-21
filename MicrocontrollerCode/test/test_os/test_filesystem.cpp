#include <unity.h>
#include <Arduino.h>

#include "common/os/filesystem.h"
#include <vector>


using namespace std;
using namespace Cesium;

class MockStream : public Stream {

public:
    String buffer;

    MockStream() {buffer = "";}

    int available() {return 0;}
    int read() {return 0;}
    int peek() {return 0;}
    
    size_t write(const char *str) {
        buffer += (String)str;
        return 1;
    }

    size_t write(uint8_t ch) {
        buffer += (char)ch;
        return 1;
    }


};

Cesium::FileSystem filesystem;
// bool begin(bool format_filesystem = false);
    
// bool listDir(const char *dirname);
// void fileTree(const char *dirname);
// bool createDir(const char *dirname);
// bool removeDir(const char *dirname);

// bool readFile(const char *path);
// bool writeFile(const char *path, const char *message);
// bool appendFile(const char *path, const char *message);
// bool renameFile(const char *from_path, const char *to_path);
// bool deleteFile(const char *path);

// bool testFileIO(const char *path);

// bool usageBytes(size_t total, size_t used);
// float usageFraction() const;

////////////////////////////////////////////////////////////
//                      Test Begin                        //
////////////////////////////////////////////////////////////

void test_filesystem_begin() {
    FileSystem new_filesystem;

    // If filesystem is not mounted, this should mount and return true
    TEST_ASSERT_TRUE(new_filesystem.begin(true));

    // If filessytem is mounted, return true
    TEST_ASSERT_TRUE(new_filesystem.begin());
}

////////////////////////////////////////////////////////////
//                   Test writeFile                      //
////////////////////////////////////////////////////////////

void test_write_new_file_test_string() {
    String result;
    TEST_ASSERT_TRUE(filesystem.writeFile("/test/newfile.txt", "AAAAAAAA"));
    TEST_ASSERT_TRUE(filesystem.writeFile("/test/newfile.txt", "TEST_STRING"));
    TEST_ASSERT_TRUE(filesystem.readFile("/test/newfile.txt", result));
    TEST_ASSERT_EQUAL_STRING("TEST_STRING", result.c_str());

}

void test_write_new_file_hello_world() {
    TEST_ASSERT_TRUE(filesystem.writeFile("/test/newfile.txt", "Hello World"));
}

////////////////////////////////////////////////////////////
//                   Test appendFile                      //
////////////////////////////////////////////////////////////

void test_append_file() {
    TEST_ASSERT_TRUE(filesystem.appendFile("/test/newfile.txt", "!"));
}

void test_append_create_file() {
    const char* filename = "/test/new_append_file.txt";
    TEST_ASSERT_TRUE(filesystem.appendFile(filename, "TEST"));

    String result;
    TEST_ASSERT_TRUE(filesystem.readFile(filename, result));
    TEST_ASSERT_EQUAL_STRING("TEST", result.c_str());
    filesystem.deleteFile(filename);
}

////////////////////////////////////////////////////////////
//                   Test listDir                      //
////////////////////////////////////////////////////////////

void test_empty_dir() {
    // Should have no files in directory
    auto files = filesystem.listDir("/test");

    TEST_ASSERT_EQUAL_INT(0, files.size());
}

void test_single_file_dir() {
    // Should have 1 file
    auto files = filesystem.listDir("/test");

    TEST_ASSERT_EQUAL_INT(1, files.size());
    TEST_ASSERT_EQUAL_STRING("newfile.txt|12", files[0].c_str());
}

void test_many_files_in_dir() {
    // TODO, create a bunch and test their names
}

////////////////////////////////////////////////////////////
//                   Test deleteFile                      //
////////////////////////////////////////////////////////////

void test_delete_file() {
    TEST_ASSERT_TRUE(filesystem.deleteFile("/test/newfile.txt"));
}

////////////////////////////////////////////////////////////
//                   Test readFile                      //
////////////////////////////////////////////////////////////

void test_read_file_string() {
    String result;
    TEST_ASSERT_TRUE(filesystem.readFile("/test/newfile.txt", result));
    TEST_ASSERT_EQUAL_STRING("Hello World!", result.c_str());
}

void test_read_file_fail() {
    String result;
    TEST_ASSERT_FALSE(filesystem.readFile("/test/this_is_not_a_file", result));
}

void test_read_file_stream() {
    MockStream stream;
    TEST_ASSERT_TRUE(filesystem.readFile("/test/newfile.txt", stream));

    String result = stream.buffer;
    TEST_ASSERT_EQUAL_STRING("Hello World!", result.c_str());
}

////////////////////////////////////////////////////////////
//                      Test mkdir                        //
////////////////////////////////////////////////////////////

void test_mkdir() {
    filesystem.mkdir("/test/new_folder");

    auto files = filesystem.listDir("/test");

    TEST_ASSERT_EQUAL_INT(1, files.size());
    TEST_ASSERT_EQUAL_STRING("new_folder", files[0].c_str());

    filesystem.rmdir("/test/new_folder");
}

////////////////////////////////////////////////////////////
//                  Test rename file                      //
////////////////////////////////////////////////////////////

void test_rename_file() {
    TEST_ASSERT_TRUE(filesystem.renameFile("/test/newfile.txt", "/test/newfile2.txt"));
    TEST_ASSERT_TRUE(filesystem.renameFile("/test/newfile2.txt", "/test/newfile.txt"));
}

void test_rename_file_fail() {
    TEST_ASSERT_FALSE(filesystem.renameFile("/test/THISISNOTAFILE", "/willnotexecute"));
}

////////////////////////////////////////////////////////////
//                Test rmdir recursive                    //
////////////////////////////////////////////////////////////

void test_rmdir_recursive() {
    filesystem.mkdir("/test/new_folder");
    filesystem.writeFile("/test/new_folder/hello.txt","Hello!"); 
    filesystem.mkdir("/test/new_folder/folder_2"); 
    filesystem.writeFile("/test/new_folder/folder_2/hello.txt","Hello!"); 

    TEST_ASSERT_TRUE(filesystem.rmdir("/test/new_folder", true));

    auto files = filesystem.listDir("/test");
    TEST_ASSERT_EQUAL(0, files.size());
}

////////////////////////////////////////////////////////////
//               Large Test File IO function              //
////////////////////////////////////////////////////////////

void test_file_IO() {
    TEST_ASSERT_TRUE(filesystem.testFileIO("/test/LMAO"));

    TEST_ASSERT_TRUE(filesystem.deleteFile("/test/LMAO"));
}
////////////////////////////////////////////////////////////
//                    Run all tests                       //
////////////////////////////////////////////////////////////

void run_all_filesystem_tests() {

    
    // filesystem should be formatted, but if it is not, then format it
    filesystem.begin(true);
    filesystem.rmdir("/test", true);
    filesystem.mkdir("/test");
    // filesystem.deleteFile("/test/newfile.txt");
    // filesystem.deleteFile("/test/this_is_not_a_file"); // In case this is still here
    // filesystem.rmdir("/test/new_folder");
    
    RUN_TEST(test_filesystem_begin);

    RUN_TEST(test_empty_dir); // Makes sure no files in /test directory

    RUN_TEST(test_mkdir); // Creates directory then deletes it

    RUN_TEST(test_rmdir_recursive);

    // Functions with 1 file
    
    RUN_TEST(test_write_new_file_hello_world); // Adds "Hello World"
    RUN_TEST(test_append_file); // Adds "!"


    RUN_TEST(test_read_file_string);
    RUN_TEST(test_read_file_fail);
    RUN_TEST(test_read_file_stream);

    RUN_TEST(test_single_file_dir);

    RUN_TEST(test_rename_file);
    RUN_TEST(test_rename_file_fail);

    RUN_TEST(test_file_IO);
    

    RUN_TEST(test_delete_file);

    
    RUN_TEST(test_empty_dir); // Makes sure no files in /test directory

    RUN_TEST(test_write_new_file_test_string); // Creates new file from write and overwrites existing data and then deletes it
    RUN_TEST(test_append_create_file); // Creates new file from append and then deletes it
}
