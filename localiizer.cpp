// local.cpp - Localizer Terminal v1.0
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#define CLEAR_COMMAND "cls"
#else
#include <unistd.h>
#define CLEAR_COMMAND "clear"
#endif

namespace fs = std::filesystem;
std::string currentPath = fs::current_path().string();
std::map<std::string, std::string> config;
std::vector<std::string> history;

void showTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << std::ctime(&time);
}

void clr() {
    system(CLEAR_COMMAND);
}

void echo(const std::string& msg) {
    std::cout << msg << std::endl;
}

void createFile(const std::string& name) {
    std::ofstream(name).close();
    std::cout << "[+] File created: " << name << std::endl;
}

void goTo(const std::string& path) {
    if (fs::exists(path) && fs::is_directory(path)) {
        currentPath = path;
        fs::current_path(path);
        std::cout << "[+] Moved to: " << path << std::endl;
    } else {
        std::cout << "[!] Path not found.\n";
    }
}

void listFiles() {
    for (const auto& entry : fs::directory_iterator(currentPath)) {
        std::cout << entry.path().filename().string() << std::endl;
    }
}

void makeDir(const std::string& name) {
    fs::create_directory(name);
    std::cout << "[+] Directory created: " << name << std::endl;
}

void removeDir(const std::string& name) {
    fs::remove_all(name);
    std::cout << "[+] Directory removed: " << name << std::endl;
}

void removeFile(const std::string& name) {
    fs::remove(name);
    std::cout << "[+] File removed: " << name << std::endl;
}

void renameFile(const std::string& old, const std::string& neu) {
    fs::rename(old, neu);
    std::cout << "[+] Renamed to: " << neu << std::endl;
}

void copyFile(const std::string& from, const std::string& to) {
    fs::copy(from, to);
    std::cout << "[+] Copied to: " << to << std::endl;
}

void moveFile(const std::string& from, const std::string& to) {
    fs::rename(from, to);
    std::cout << "[+] Moved to: " << to << std::endl;
}

void readFile(const std::string& name) {
    std::ifstream file(name);
    std::string line;
    while (getline(file, line)) std::cout << line << std::endl;
}

void writeFile(const std::string& name, const std::string& content) {
    std::ofstream file(name);
    file << content;
}

void appendFile(const std::string& name, const std::string& content) {
    std::ofstream file(name, std::ios::app);
    file << content << std::endl;
}

void countLines(const std::string& name) {
    std::ifstream file(name);
    std::string line;
    int count = 0;
    while (getline(file, line)) count++;
    std::cout << "[+] Line count: " << count << std::endl;
}

void scanExt(const std::string& ext) {
    for (const auto& entry : fs::directory_iterator(currentPath)) {
        if (entry.path().extension() == ext) {
            std::cout << entry.path().filename().string() << std::endl;
        }
    }
}

void showHelp() {
    std::cout << "\n[Command List]\n"
              << "cret -file [name]\nloc [path]\npref [path] in [file]\nlotf [name]\nhype [name]\n"
              << "poll\nset [key] [value]\nget [key]\ndef\nunt\ndot\n"
              << "time\nclr\necho [msg]\ndel [file]\nmv [file] to [dir]\n"
              << "cp [file] to [file]\nren [old] to [new]\nls\ncwd\n"
              << "cat [file]\nwrite [file] [text]\nappend [file] [text]\n"
              << "mkdir [name]\nrmdir [name]\nscan [ext]\ncount [file]\n"
              << "ver\nhelp\nexit\n";
}

void setConfig(const std::string& key, const std::string& val) {
    config[key] = val;
    std::cout << "[+] Set " << key << " to " << val << std::endl;
}

void getConfig(const std::string& key) {
    if (config.find(key) != config.end()) {
        std::cout << "[+] " << key << " = " << config[key] << std::endl;
    } else {
        std::cout << "[!] Key not found.\n";
    }
}

int main() {
    std::string input;
    std::cout << "Localizer Terminal v1.0\n";
    while (true) {
        std::cout << "\\> ";
        getline(std::cin, input);
        history.push_back(input);

        if (input == "dot" || input == "exit") break;
        else if (input.rfind("cret -file ", 0) == 0) createFile(input.substr(11));
        else if (input.rfind("loc ", 0) == 0) goTo(input.substr(4));
        else if (input.rfind("pref ", 0) == 0) std::cout << "[i] 'pref' not yet implemented\n";
        else if (input.rfind("lotf ", 0) == 0) createFile(input.substr(5) + ".secret");
        else if (input.rfind("hype ", 0) == 0) createFile(input.substr(5) + ".hype");
        else if (input == "poll") std::cout << "[i] Debug active\n";
        else if (input == "def") std::cout << "[i] Def not yet implemented\n";
        else if (input == "unt") std::cout << "[i] Unt not yet implemented\n";
        else if (input.rfind("set ", 0) == 0) {
            auto pos = input.find(' ', 4);
            setConfig(input.substr(4, pos - 4), input.substr(pos + 1));
        }
        else if (input.rfind("get ", 0) == 0) getConfig(input.substr(4));
        else if (input == "time") showTime();
        else if (input == "clr") clr();
        else if (input.rfind("echo ", 0) == 0) echo(input.substr(5));
        else if (input.rfind("del ", 0) == 0) removeFile(input.substr(4));
        else if (input.rfind("mv ", 0) == 0) {
            auto pos = input.find(" to ");
            moveFile(input.substr(3, pos - 3), input.substr(pos + 4));
        }
        else if (input.rfind("cp ", 0) == 0) {
            auto pos = input.find(" to ");
            copyFile(input.substr(3, pos - 3), input.substr(pos + 4));
        }
        else if (input.rfind("ren ", 0) == 0) {
            auto pos = input.find(" to ");
            renameFile(input.substr(4, pos - 4), input.substr(pos + 4));
        }
        else if (input == "ls") listFiles();
        else if (input == "cwd") std::cout << currentPath << std::endl;
        else if (input.rfind("cat ", 0) == 0) readFile(input.substr(4));
        else if (input.rfind("write ", 0) == 0) {
            auto pos = input.find(' ', 6);
            writeFile(input.substr(6, pos - 6), input.substr(pos + 1));
        }
        else if (input.rfind("append ", 0) == 0) {
            auto pos = input.find(' ', 7);
            appendFile(input.substr(7, pos - 7), input.substr(pos + 1));
        }
        else if (input.rfind("mkdir ", 0) == 0) makeDir(input.substr(6));
        else if (input.rfind("rmdir ", 0) == 0) removeDir(input.substr(6));
        else if (input.rfind("scan ", 0) == 0) scanExt(input.substr(5));
        else if (input.rfind("count ", 0) == 0) countLines(input.substr(6));
        else if (input == "ver") std::cout << "Localizer Terminal v1.0\n";
        else if (input == "help") showHelp();
        else std::cout << "[!] Unknown command. Type 'help'.\n";
    }
    std::cout << "[x] Session ended.\n";
    return 0;
}
