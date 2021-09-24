// ----------------------------------------
// Communication between server (ESP-32) and host (PC, Laptop)
// ----------------------------------------
#ifndef _SERVER_HOST_PROTOCOL_
#define _SERVER_HOST_PROTOCOL_
#include <Arduino.h>

/** NOTE: Server only keeps total number of clients, not individual address
 * so the host must manage that. If something goes wrong, e.g, client dropout, 
 * host must notify and sync server to correct number of clients
 */

/** Every message must have a "type" field */
enum message_t : uint8_t
{
    /** 
     * The JSON contains no "type" object or unknown type, 
     * this treats as an error
     */
    INVALID_TYPE = 0,
    /** 
     * Client -> Server -> Host
     * @param "mac": Array 
     */
    REG_CLIENT,
    /**
     * Host -> Server -> Client
     * @param "id": Integer,
     * @param "mac": Array
     */
    RESPOND_REG_CLIENT,
    /**
     * Host -> Server
     * @param "answ": Integer, correct answer
     * @param "time": Integer, in seconds
     */
    START_QUIZ,
    /** Host -> Server, stop the quiz early */
    STOP_QUIZ,
    /** Client -> Server -> Host
     * @param "mac": Array,
     * @param "btn": Integer
     */
    CLIENT_ANSWER,
    /** Host -> Server
     * @param "n": Integer
     */
    SET_CLIENT_SIZE,
};

#endif // !_SERVER_HOST_PROTOCOL_