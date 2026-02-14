#include "scanner.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ctime>

namespace fs = std::filesystem;

void logToFile(const std::string& message) {
    std::ofstream logFile("scanner.log", std::ios::app);
    if (logFile.is_open()) {
        auto now = std::time(nullptr);
        auto tm = std::localtime(&now);
        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
        logFile << "[" << buffer << "] " << message << "\n";
        logFile.close();
    }
}

bool isDangerous(const std::string& line) {
    // Функции выполнения команд
    if (line.find("system(") != std::string::npos ||
        line.find("popen(") != std::string::npos ||
        line.find("execv(") != std::string::npos ||
        line.find("execl(") != std::string::npos ||
        line.find("exec(") != std::string::npos ||
        line.find("ShellExecute(") != std::string::npos) {
        return true;
    }

    // Уязвимые функции для работы со строками
    if (line.find("strcpy(") != std::string::npos ||
        line.find("strcat(") != std::string::npos ||
        line.find("gets(") != std::string::npos ||
        line.find("scanf(") != std::string::npos ||
        line.find("sprintf(") != std::string::npos ||
        line.find("sscanf(") != std::string::npos ||
        line.find("vsprintf(") != std::string::npos ||
        line.find("strdup(") != std::string::npos) {
        return true;
    }

    // Path Traversal атаки
    if (line.find("../") != std::string::npos ||
        line.find("..\\") != std::string::npos ||
        line.find("..%") != std::string::npos) {
        return true;
    }

    // Небезопасная работа с памятью
    if (line.find("malloc(") != std::string::npos ||
        line.find("memcpy(") != std::string::npos ||
        line.find("memmove(") != std::string::npos) {
        // Проверяем без проверок границ
        if (line.find("sizeof(") == std::string::npos) {
            return true;
        }
    }

    // SQL Injection паттерны
    if (line.find("SELECT") != std::string::npos ||
        line.find("INSERT") != std::string::npos ||
        line.find("UPDATE") != std::string::npos ||
        line.find("DELETE") != std::string::npos) {
        // Если используется конкатенация строк с пользовательским вводом
        if (line.find("+") != std::string::npos ||
            line.find("<<") != std::string::npos) {
            return true;
        }
    }

    // Обращение к нулевому указателю без проверки
    if (line.find("->") != std::string::npos ||
        line.find("[") != std::string::npos) {
        // Если нет проверки != nullptr или != NULL
        if (line.find("!= nullptr") == std::string::npos &&
            line.find("!= NULL") == std::string::npos &&
            line.find("if (") == std::string::npos) {
            // Возможный null pointer dereference
            if (line.find("delete ") != std::string::npos ||
                line.find("free(") != std::string::npos) {
                return true;
            }
        }
    }

    // Глобальные переменные и статические указатели
    if (line.find("static") != std::string::npos &&
        line.find("*") != std::string::npos) {
        return true;
    }

    // Использование goto (часто приводит к ошибкам)
    if (line.find("goto ") != std::string::npos) {
        return true;
    }

    // ========== ПРОВЕРКИ СЛЕЖКИ И МОНИТОРИНГА ==========

    // Сетевые соединения и экспорт данных
    if (line.find("socket(") != std::string::npos ||
        line.find("curl_easy_perform(") != std::string::npos ||
        line.find("HttpSendRequest(") != std::string::npos ||
        line.find("WinHttpSendRequest(") != std::string::npos ||
        line.find("URLDownloadToFile(") != std::string::npos ||
        line.find("InternetOpen(") != std::string::npos) {
        return true;
    }

    // Скрытые сетевые операции
    if ((line.find("send(") != std::string::npos ||
         line.find("connect(") != std::string::npos ||
         line.find("WSAConnect(") != std::string::npos) &&
        line.find("error") == std::string::npos) { // Игнорируем обработку ошибок
        return true;
    }

    // Мониторинг процессов и системы
    if (line.find("CreateProcess(") != std::string::npos ||
        line.find("GetProcessList(") != std::string::npos ||
        line.find("EnumProcesses(") != std::string::npos ||
        line.find("CreateToolhelp32Snapshot(") != std::string::npos ||
        line.find("GetWindowText(") != std::string::npos ||
        line.find("GetForegroundWindow(") != std::string::npos) {
        return true;
    }

    // Перехват клавиатуры и мыши
    if (line.find("SetWindowsHookEx(") != std::string::npos ||
        line.find("WH_KEYBOARD") != std::string::npos ||
        line.find("WH_MOUSE") != std::string::npos ||
        line.find("GetAsyncKeyState(") != std::string::npos) {
        return true;
    }

    // Чтение данных пользователя (скрытый файловый доступ)
    if ((line.find("CreateFile(") != std::string::npos ||
         line.find("ReadFile(") != std::string::npos ||
         line.find("fopen(") != std::string::npos ||
         line.find("ifstream(") != std::string::npos) &&
        (line.find("\\Users\\") != std::string::npos ||
         line.find("\\Documents\\") != std::string::npos ||
         line.find("\\Desktop\\") != std::string::npos ||
         line.find("\\AppData\\") != std::string::npos ||
         line.find("/home/") != std::string::npos ||
         line.find("/.ssh/") != std::string::npos)) {
        return true;
    }

    // Логирование и отправка данных
    if ((line.find("fwrite(") != std::string::npos ||
         line.find("WriteFile(") != std::string::npos ||
         line.find("ofstream(") != std::string::npos) &&
        (line.find("log") != std::string::npos ||
         line.find("data") != std::string::npos ||
         line.find("track") != std::string::npos)) {
        return true;
    }

    // Скрытые потоки и асинхронные операции
    if (line.find("CreateThread(") != std::string::npos ||
        line.find("std::thread(") != std::string::npos) {
        // Проверяем, не передается ли конфиденциальная функция
        if (line.find("send") != std::string::npos ||
            line.find("upload") != std::string::npos ||
            line.find("exfiltrate") != std::string::npos) {
            return true;
        }
    }

    // Реестр Windows (скрытые конфигурации)
    if (line.find("RegOpenKey(") != std::string::npos ||
        line.find("RegSetValueEx(") != std::string::npos ||
        line.find("RegQueryValueEx(") != std::string::npos) {
        return true;
    }

    // Криптография без явной проверки целей
    if (line.find("CryptEncrypt(") != std::string::npos ||
        line.find("CryptDecrypt(") != std::string::npos ||
        line.find("EVP_") != std::string::npos) {
        return true;
    }

    // Скрытие процесса или окна
    if (line.find("ShowWindow(") != std::string::npos ||
        line.find("SW_HIDE") != std::string::npos) {
        return true;
    }

    // Стиранию логов или следов
    if (line.find("DeleteFile(") != std::string::npos ||
        line.find("remove(") != std::string::npos ||
        line.find("unlink(") != std::string::npos ||
        line.find("ClearEventLog(") != std::string::npos) {
        return true;
    }

    // Инъекция кода в другие процессы
    if (line.find("VirtualAllocEx(") != std::string::npos ||
        line.find("WriteProcessMemory(") != std::string::npos ||
        line.find("CreateRemoteThread(") != std::string::npos) {
        return true;
    }

    // ========== PYTHON ПРОВЕРКИ ==========
    if (line.find("eval(") != std::string::npos ||
        line.find("exec(") != std::string::npos ||
        line.find("__import__(") != std::string::npos ||
        line.find("compile(") != std::string::npos ||
        line.find("pickle.loads(") != std::string::npos) {
        return true;
    }

    // Python - сетевые операции
    if (line.find("socket.socket(") != std::string::npos ||
        line.find("urllib.request.urlopen(") != std::string::npos ||
        line.find("requests.get(") != std::string::npos ||
        line.find("requests.post(") != std::string::npos) {
        return true;
    }

    // Python - команды ОС
    if (line.find("os.system(") != std::string::npos ||
        line.find("subprocess.call(") != std::string::npos ||
        line.find("subprocess.Popen(") != std::string::npos ||
        line.find("os.popen(") != std::string::npos) {
        return true;
    }

    // ========== JAVASCRIPT/TYPESCRIPT ПРОВЕРКИ ==========
    if (line.find("eval(") != std::string::npos ||
        line.find("Function(") != std::string::npos ||
        line.find("setTimeout(") != std::string::npos ||
        line.find("setInterval(") != std::string::npos) {
        // Опасно, если содержит строки с кодом
        if (line.find("\"") != std::string::npos || 
            line.find("'") != std::string::npos) {
            return true;
        }
    }

    // JavaScript - требесты данных
    if (line.find("fetch(") != std::string::npos ||
        line.find("XMLHttpRequest(") != std::string::npos ||
        line.find("navigator.sendBeacon(") != std::string::npos) {
        return true;
    }

    // ========== SHELL SCRIPT ПРОВЕРКИ ==========
    if (line.find("wget ") != std::string::npos ||
        line.find("curl ") != std::string::npos ||
        line.find("`") != std::string::npos ||
        line.find("$(") != std::string::npos) {
        // Загрузка и выполнение из интернета
        if (line.find("| bash") != std::string::npos ||
            line.find("| sh") != std::string::npos ||
            line.find("| python") != std::string::npos) {
            return true;
        }
    }

    // Bash - команды ОС
    if (line.find("nc ") != std::string::npos ||       // netcat
        line.find("ncat ") != std::string::npos ||
        line.find("socat ") != std::string::npos ||
        line.find("telnet ") != std::string::npos) {
        return true;
    }

    // ========== POWERSHELL ПРОВЕРКИ ==========
    if (line.find("IEX(") != std::string::npos ||
        line.find("Invoke-Expression") != std::string::npos ||
        line.find("DownloadString(") != std::string::npos ||
        line.find("DownloadFile(") != std::string::npos) {
        return true;
    }

    // ========== ОБЩИЕ СЕТЕВЫЕ ПАТТЕРНЫ ==========
    if (line.find("http://") != std::string::npos ||
        line.find("https://") != std::string::npos) {
        // Проверяем на подозрительные домены или IP
        if (line.find("127.0.0.1") != std::string::npos ||
            line.find("192.168.") != std::string::npos ||
            line.find("10.0.") != std::string::npos ||
            line.find("localhost") != std::string::npos ||
            line.find("exfiltrate") != std::string::npos ||
            line.find("c2.") != std::string::npos ||
            line.find("botnet") != std::string::npos) {
            return true;
        }
    }

    // ========== ПРОВЕРКА НА ОБФУСКАЦИЮ ==========
    // Base64 кодирование часто используется для скрытия кода
    if (line.find("base64") != std::string::npos ||
        line.find("Base64") != std::string::npos) {
        // Если вместе с eval или exec
        if (line.find("eval") != std::string::npos ||
            line.find("exec") != std::string::npos ||
            line.find("decode") != std::string::npos) {
            return true;
        }
    }

    // Hex кодирование с eval
    if ((line.find("hex") != std::string::npos ||
         line.find("0x") != std::string::npos) &&
        (line.find("eval") != std::string::npos ||
         line.find("exec") != std::string::npos)) {
        return true;
    }

    return false;
}

void scanDirectory(const char* path) {
    std::cout << "[SCAN] Directory: " << path << "\n";
    logToFile("Started scanning directory: " + std::string(path));

    // Проверка существования директории
    if (!fs::exists(path)) {
        std::string error = "ERROR: Directory does not exist: " + std::string(path);
        std::cerr << error << "\n";
        logToFile(error);
        return;
    }

    if (!fs::is_directory(path)) {
        std::string error = "ERROR: Path is not a directory: " + std::string(path);
        std::cerr << error << "\n";
        logToFile(error);
        return;
    }

    try {
        for (auto& p : fs::recursive_directory_iterator(path)) {
            std::string ext = p.path().extension().string();
            
            // Поддерживаемые типы файлов
            bool isSupportedFile = 
                ext == ".cpp" || ext == ".h" || ext == ".hpp" ||    // C++
                ext == ".c" || ext == ".cc" ||                       // C
                ext == ".php" || ext == ".phtml" ||                  // PHP
                ext == ".js" || ext == ".ts" ||                      // JavaScript/TypeScript
                ext == ".py" ||                                       // Python
                ext == ".java" ||                                     // Java
                ext == ".cs" ||                                       // C#
                ext == ".go" ||                                       // Go
                ext == ".rb" ||                                       // Ruby
                ext == ".pl" ||                                       // Perl
                ext == ".sh" || ext == ".bash" ||                    // Shell scripts
                ext == ".bat" || ext == ".cmd" ||                    // Batch files
                ext == ".ps1" || ext == ".ps2" ||                    // PowerShell
                ext == ".vbs" ||                                      // VBScript
                ext == ".xml" || ext == ".config" ||                 // XML/Config
                ext == ".json" || ext == ".yml" || ext == ".yaml" || // Config formats
                ext == ".sql" ||                                      // SQL
                ext == ".txt" || ext == ".log" ||                    // Text files
                ext == ".html" || ext == ".htm";                     // HTML

            if (isSupportedFile) {
                std::ifstream file(p.path());
                std::string line;
                int ln = 0;

                while (std::getline(file, line)) {
                    ln++;
                    if (isDangerous(line)) {
                        std::string warning = "WARNING: " + p.path().string() + ":" + std::to_string(ln);
                        std::cout << "[" << warning << "]\n";
                        logToFile(warning);
                    }
                }
            }
        }
        logToFile("Directory scan completed successfully");
    } catch (const std::exception& e) {
        std::string error = "ERROR: " + std::string(e.what());
        std::cerr << error << "\n";
        logToFile(error);
    }
}
