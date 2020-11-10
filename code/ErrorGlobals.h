#pragma once

#include <iostream>
//#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <execinfo.h>

static bool AlreadyBacktracing = false;
static void PrintBacktrace()
{
    if (AlreadyBacktracing)
    {
        std::wcerr << L"\nWARNING: Recursive backtrace detected!\n";
        return;
    }
    AlreadyBacktracing = true;
    void* backtraceBuffer[100];
    int addressCount = backtrace(backtraceBuffer, 100);
    char** backtraceStrings = backtrace_symbols(backtraceBuffer, addressCount);
    std::wcerr << L"Backtrace (" << addressCount << L"):\n";
    for (int backtraceIndex = 0; backtraceIndex < addressCount; ++backtraceIndex)
    {
        std::wcerr << L"\t" << backtraceIndex << L": " << StringToWString(backtraceStrings[backtraceIndex]) << L"\n";
    }
    delete backtraceStrings;

    //For testing recursive backtrace.
    //PrintBacktrace();

    AlreadyBacktracing = false;
}

static bool AlreadyHandlingSignal = false;
static void SignalHandlerFunction(int sig, siginfo_t* info, void* ucontext)
{
    if (AlreadyHandlingSignal)
    {
        std::wcerr << L"\nWARNING: Recursive signal handling detected!\n";
        return;
    }
    AlreadyHandlingSignal = true;
    std::wcerr << L"\n\nSignal error (" << sig << L"): " << StringToWString(strsignal(sig)) << L"\n\n";
    PrintBacktrace();

    //For testing recursive signal handling (assumes SIGABRT is being handled here).
    //abort();

    AlreadyHandlingSignal = false;
    exit(EXIT_FAILURE);
}

//Handle Linux error signals.
static void SetupSignalHandlers()
{
    //https://www.man7.org/linux/man-pages/man2/sigaction.2.html
    //https://www.man7.org/linux/man-pages/man7/signal.7.html
    //"The signals SIGKILL and SIGSTOP cannot be caught, blocked, or ignored."

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;
    sa.sa_sigaction = SignalHandlerFunction;

    //Aborted.
    sigaction(SIGABRT, &sa, nullptr);
    //Bus error.
    sigaction(SIGBUS, &sa, nullptr);
    //Floating-point exception.
    sigaction(SIGFPE, &sa, nullptr);
    //Hangup detected on controlling terminal or death of controlling process.
    sigaction(SIGHUP, &sa, nullptr);
    //Illegal instruction.
    sigaction(SIGILL, &sa, nullptr);
    //Interrupt from keyboard.
    sigaction(SIGINT, &sa, nullptr);
    //Power failure.
    sigaction(SIGPWR, &sa, nullptr);
    //Quit from keyboard.
    sigaction(SIGQUIT, &sa, nullptr);
    //Invalid memory reference. An extremely likely signal to encounter (trying to read/write memory program doesn't
    //have access to, including dereferencing a nullptr).
    sigaction(SIGSEGV, &sa, nullptr);
    //Bad system call.
    sigaction(SIGSYS, &sa, nullptr);
    //Terminated.
    sigaction(SIGTERM, &sa, nullptr);
    //CPU time limit exceeded.
    sigaction(SIGXCPU, &sa, nullptr);
    //File size limit exceeded.
    sigaction(SIGXFSZ, &sa, nullptr);

    //If restoring the stack environment is desired:
    //sigjmp_buf preSignalEnvironment;
    //setjmp(preSignalEnvironment);
    //longjmp(preSignalEnvironment, 1);
}

static int MainWithCatch(std::function<int()> mainWithoutCatch)
{
    //UTF-8 wide character printing.
    setlocale(LC_ALL, "en_US.UTF-8");
    //Other configurations.
    std::wcout << std::boolalpha;

    SetupSignalHandlers();
    std::set_unexpected([]()
    {
        std::wcerr << L"unexpected() called!\n\n";
        PrintBacktrace();
        exit(EXIT_FAILURE);
    });
    std::set_terminate([]()
    {
        std::wcerr << L"terminate() called!\n\n";
        PrintBacktrace();
        exit(EXIT_FAILURE);
    });
    try
    {
        //For testing various errors.
        //abort();
        //int divideByZero = 1 / 0;
        //int* nullPointer = nullptr; *nullPointer = 1;
        //std::unexpected();
        //std::terminate();
        //throw std::runtime_error("my runtime error");
        //throw std::wstring(L"test");
        //throw std::string("test");
        //throw L"test";
        //throw "test";
        //throw true;

        int returnCode = mainWithoutCatch();
        if (returnCode == 0)
        {
            std::wcout << L"\n\nFinished successfully.\n\n";
        }
        return returnCode;
    }
    catch (std::exception const& ex)
    {
        std::wcerr << L"Exception (std::exception) in MainWithCatch(): " << StringToWString(ex.what()) << L"\n\n";
        PrintBacktrace();
        exit(EXIT_FAILURE);
    }
    catch (std::wstring const& wstr)
    {
        std::wcerr << L"Exception (std::wstring) in MainWithCatch(): " << wstr << L"\n\n";
        PrintBacktrace();
        exit(EXIT_FAILURE);
    }
    catch (std::string const& str)
    {
        std::wcerr << L"Exception (std::string) in MainWithCatch(): " << StringToWString(str) << L"\n\n";
        PrintBacktrace();
        exit(EXIT_FAILURE);
    }
    catch (const wchar_t* wstr)
    {
        std::wcerr << L"Exception (const wchar_t*) in MainWithCatch(): " << wstr << L"\n\n";
        PrintBacktrace();
        exit(EXIT_FAILURE);
    }
    catch (const char* str)
    {
        std::wcerr << L"Exception (const char*) in MainWithCatch(): " << StringToWString(str) << L"\n\n";
        PrintBacktrace();
        exit(EXIT_FAILURE);
    }
    catch (...)
    {
        std::wcerr << L"Exception (...) in MainWithCatch()!\n\n";
        PrintBacktrace();
        exit(EXIT_FAILURE);
    }
}