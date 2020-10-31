#pragma once

#include "../Globals.h"

class HTMLLogger
{
    public:
        HTMLLogger()
        {
            logFile = nullptr;
        }

        ~HTMLLogger()
        {
            CloseLogFile();
        }

        void OpenLogFile(std::string logFilePath)
        {
            CloseLogFile();

            this->logFilePath = logFilePath;

            //Create and truncate (empty) the file for writing.
            std::wcout << L"Opening " << std::wstring(logFilePath.begin(), logFilePath.end()) << L" for logging.\n\n";
            logFile = new std::wfstream(logFilePath, std::ios::trunc | std::ios::out);

            //Log file setup.
            time_t logStartTime = time(nullptr);
            *logFile << L"<script src=\"LogFrameScript.js\"></script>";
            *logFile << L"<script>\nlogStartTime = " << std::to_wstring(logStartTime) << L";\n";

            //First output.
            LogWStringInsert(L"<p>Logging started (" + TimeToWString(&logStartTime) + L").</p><br/>");
        }

        void LogWStringOverwrite(std::wstring logMe)
        {
            *logFile << L"output.push('" << logMe << L"');\n";
        }

        void LogWStringInsert(std::wstring logMe)
        {
            LogWStringOverwrite(logMe);

            nextInsertPosition = logFile->tellp();
            RestoreScriptEnd();
            logFile->seekp(nextInsertPosition);
        }

        void RestoreScriptEnd()
        {
            *logFile << L"</script>";
        }

        void CloseLogFile()
        {
            if (logFile == nullptr)
            {
                return;
            }

            //Final output.
            time_t logEndTime = time(nullptr);
            LogWStringOverwrite(L"<br/><p>Logging finished (" + TimeToWString(&logEndTime) + L").</p>");
            *logFile << L"logEndTime = " << std::to_wstring(logEndTime) << L";\n";
            *logFile << L"loggingFinished = true;\n";
            RestoreScriptEnd();

            //Close the file.
            std::wcout << L"\n\nClosing logger file " << std::wstring(logFilePath.begin(), logFilePath.end()) << L".";
            logFile->close();
            delete logFile;
            logFile = nullptr;
        }

    private:
        std::string logFilePath;
        std::wfstream* logFile;
        std::wstreampos nextInsertPosition;
};