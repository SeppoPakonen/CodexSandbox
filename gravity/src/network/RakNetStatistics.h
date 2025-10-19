/*
 * Stub for RakNetStatistics.h
 * This is a minimal stub to allow compilation
 */

#ifndef RakNetStatistics_h
#define RakNetStatistics_h

// Simple stub structure for RakNetStatisticsStruct
struct RakNetStatisticsStruct {
    unsigned int messageSendBuffer[256];
    unsigned int messagesSent[256];
    unsigned int messageDataBitsSent[256];
    unsigned int messageTotalBitsSent[256];
    unsigned int messagesReceived[256];
    unsigned int messageDataBitsReceived[256];
    unsigned int messageTotalBitsReceived[256];
    unsigned int bitsPerSecondSent;
    unsigned int bitsPerSecondReceived;
    unsigned int connectionStartTime;
    unsigned int connectionEndTime;
    unsigned int packetlossLastSecond;
    unsigned int packetsSent;
    unsigned int packetsReceived;
    unsigned int duplicateMessagesReceived;
    unsigned int duplicateAcknowlegementsReceived;
    unsigned int acknowlegementsSent;
    unsigned int acknowlegementsReceived;
    unsigned int delayedAcks;
    unsigned int bytesSent;
    unsigned int bytesReceived;
    unsigned int packetlossTotal;
    unsigned int messagesTotalRequested;
    unsigned int messagesOnResendQueue;
};

// Forward declaration
class BitStream;

// Function to convert statistics to string
void StatisticsToString(RakNetStatisticsStruct *stats, char *buffer, int verbosityLevel);

// Implementation
void StatisticsToString(RakNetStatisticsStruct *stats, char *buffer, int verbosityLevel) {
    if (buffer && stats) {
        // Just write a simple message
        const char* msg = "Stub statistics";
        for (int i = 0; i < 15 && msg[i]; i++) {
            buffer[i] = msg[i];
        }
        buffer[15] = '\0';
    }
}

#endif