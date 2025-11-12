#include "ServerMain.h"
#include <sys/stat.h>
#include <fstream>
#include <iostream>

namespace ServerMainImpl {
    bool CtrlCHit = false;
#ifndef SERVER_CODE
    struct sigaction oldSigTerm, oldSigQuit, oldSigInt, oldSigPipe;
#endif

#ifndef SERVER_CODE
    inline void HandleSig(int Signal) {
        if (Signal == SIGINT) {
            std::cout << std::endl;
            if (!CtrlCHit) {
                std::cout << "Control-C hit, shutting down" << std::endl;
                CtrlCHit = true;
                ProgReady = false;
            } else {
                std::cout << "OK, OK, exiting immediately" << std::endl;
                exit(1);
            }
        }

        if (Signal == SIGTERM || Signal == SIGQUIT) {
            MainConsole.Console("", GAME_MESSAGE_COLOR);
            MainConsole.Console("Signal received, shutting down", GAME_MESSAGE_COLOR);
            ProgReady = false;
        }
    }

    inline void SetSigHooks() {
        struct sigaction newAction;
        newAction.sa_handler = HandleSig;
        sigemptyset(&newAction.sa_mask);
        newAction.sa_flags = 0;

        sigaction(SIGTERM, &newAction, &oldSigTerm);
        sigaction(SIGINT, &newAction, &oldSigInt);
        sigaction(SIGQUIT, &newAction, &oldSigQuit);
        sigaction(SIGPIPE, &newAction, &oldSigPipe);
    }

    inline void ClearSigHooks() {
        sigaction(SIGTERM, &oldSigTerm, nullptr);
        sigaction(SIGINT, &oldSigInt, nullptr);
        sigaction(SIGQUIT, &oldSigQuit, nullptr);
        sigaction(SIGPIPE, &oldSigPipe, nullptr);
    }
#endif

    inline void WritePID() {
        pid_t pid = getpid();
        std::ofstream pidFile("soldatserver.pid");
        if (pidFile.is_open()) {
            pidFile << pid << std::endl;
            pidFile.close();
        }
    }

    inline void RunServer() {
        if (IsRoot()) {
            std::cout << "You are running soldatserver as root! Don't do that! " <<
                "There are not many valid" << std::endl <<
                "reasons for this and it can, in theory, cause great damage!" << std::endl;
            return;
        }

        SetSigHooks();

        try {
#ifndef DEBUG_BUILD
            try {
#endif
                ActivateServer();
                WritePID();

#ifdef SCRIPT_CODE
                if (sc_enable.Value()) {
                    ScrptDispatcher.Prepare();
                }
#endif

                std::cout << 
                    "----------------------------------------------------------------" << std::endl;

                if (ProgReady) {
                    StartServer();
                }
                
                while (ProgReady) {
                    AppOnIdle();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
#ifndef DEBUG_BUILD
            } catch (const std::exception& e) {
                ProgReady = false;
                MainConsole.Console("Server Encountered an error:", GAME_MESSAGE_COLOR);
                MainConsole.Console(e.what(), GAME_MESSAGE_COLOR);
            }
#endif
        } catch (const std::exception& e) {
            ProgReady = false;
            MainConsole.Console("Server Encountered an error:", GAME_MESSAGE_COLOR);
            MainConsole.Console(e.what(), GAME_MESSAGE_COLOR);
        } catch (...) {
            ProgReady = false;
            MainConsole.Console("Server Encountered an unknown error:", GAME_MESSAGE_COLOR);
        }

        // Cleanup
        Shutdown();
        ClearSigHooks();
    }

    inline bool IsRoot() {
#ifdef _WIN32
        return false;  // Ignore for Windows users
#else
        return (geteuid() == 0);
#endif
    }
}