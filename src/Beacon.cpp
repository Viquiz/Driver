#include "Beacon.hpp"

void beaconUpdatePacket(BeaconHandle *bh)
{
    // Update unanswered
    xQueueReceive(bh->packetUpdateQueue, (void *)&bh->packet, 0);
    // Update time remain
    // TODO: timeLimit + timeStarted - millis()
}

void beaconCallback(TimerHandle_t timer)
{
    BeaconHandle *bh = (BeaconHandle *)pvTimerGetTimerID(timer);
    beaconUpdatePacket(bh);
    esp_now_send(bh->peer.peer_addr,
                 (uint8_t *)&bh->packet,
                 sizeof(bh->packet));
}

BeaconHandle *beaconCreate(BeaconHandle *bh,
                           const char *timerName,
                           TickType_t timerPeriodInTicks,
                           bool autoReload,
                           UBaseType_t queueLength,
                           UBaseType_t queueItemSize)
{
    bh->timer = xTimerCreate(timerName,
                             timerPeriodInTicks,
                             autoReload,
                             (void *)&bh,
                             beaconCallback);
    bh->packetUpdateQueue = xQueueCreate(queueLength, queueItemSize);
    return bh;
}

bool beaconAddPeer(BeaconHandle *bh, bool ignorePeerExisted)
{
    esp_err_t err = esp_now_add_peer(&bh->peer);
    if (err != ESP_OK ||
        (err == ESP_ERR_ESPNOW_EXIST && !ignorePeerExisted))
    {
        Log.errorln("Add beacon peer failed. Code 0x%x", err);
        return false;
    }
    return true;
}

bool beaconDelPeer(BeaconHandle *bh, bool ignorePeerNotFound)
{
    esp_err_t err = esp_now_del_peer(bh->peer.peer_addr);
    if (err != ESP_OK ||
        (err == ESP_ERR_ESPNOW_NOT_FOUND && !ignorePeerNotFound))
    {
        Log.errorln("Delete beacon peer failed. Code 0x%x", err);
        return false;
    }
    return true;
}

bool beaconStart(BeaconHandle *bh,
                 TickType_t ticksToWait,
                 bool addPeerToList,
                 bool ignorePeerExisted)
{
    if (addPeerToList && !beaconAddPeer(bh, ignorePeerExisted))
        return false;
    return xTimerStart(bh->timer, ticksToWait);
}

bool beaconStartFromISR(BeaconHandle *bh,
                        bool higherPriorityTaskWoken,
                        bool addPeerToList,
                        bool ignorePeerExisted)
{
    if (addPeerToList && !beaconAddPeer(bh, ignorePeerExisted))
        return false;
    return xTimerStartFromISR(bh->timer,
                              (BaseType_t *)&higherPriorityTaskWoken);
}

bool beaconStop(BeaconHandle *bh,
                TickType_t ticksToWait,
                bool delPeerFromList,
                bool ignorePeerNotFound)
{
    if (delPeerFromList && !beaconDelPeer(bh, ignorePeerNotFound))
        return false;
    return xTimerStop(bh->timer, ticksToWait);
}