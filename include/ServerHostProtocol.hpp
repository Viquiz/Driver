// ----------------------------------------
// Communication between server (ESP-32) and host (PC, Laptop)
// ----------------------------------------
#ifndef _SERVER_HOST_PROTOCOL_
#define _SERVER_HOST_PROTOCOL_
#include <Arduino.h>

/** NOTE: Server only keeps total number of clients, not individual address
 * so the host must manage that. If something goes wrong, e.g, client dropout, 
 * host must notify and sync the server to correct number of clients
 */

#define JSON_TYPE_KEY "type"
#define JSON_ADDR_KEY "mac"
#define JSON_CLIENT_ID_KEY "id"
#define JSON_ANSW_KEY "btn"
#define JSON_TIME_KEY "time"

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
     * @param "btn": Integer, correct answer
     * @param "time": Integer, in seconds
     */
    START_QUIZ,
    /** Host -> Server, stop the quiz early */
    STOP_QUIZ,
    /** Host -> Server
     * @param "n": Integer
     */
    SET_CLIENT_SIZE,
    /** Client -> Server -> Host
     * @param "mac": Array,
     * @param "btn": Integer
     */
    CLIENT_ANSWER
};

#endif // !_SERVER_HOST_PROTOCOL_