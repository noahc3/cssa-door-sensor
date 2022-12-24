#pragma once

#include <Arduino.h>

class WebhookService {
  private:
    void sendMessage(String message);
    void sendDoorMessage(bool doorState);
    

  public:
    WebhookService();

    void init(bool initialState);
    void trySendMessage(bool doorState);
};

extern WebhookService WebhookSvc;