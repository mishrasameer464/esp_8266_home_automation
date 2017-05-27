#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mqtt.h"
#include "wifi.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"
#include "user_config.h"

static volatile os_timer_t high_priority_poll_timer;
static volatile os_timer_t medium_priority_poll_timer;
static volatile os_timer_t low_priority_poll_timer;

MQTT_Client mqttClient;
static void ICACHE_FLASH_ATTR wifiConnectCb(uint8_t status)
{
  if (status == STATION_GOT_IP) 
  {
    MQTT_Connect(&mqttClient);
  } 
  else 
  {
    MQTT_Disconnect(&mqttClient);
  }
}

static void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Connected\r\n");
  MQTT_Subscribe(&mqttClient, SUBSCRIBER_TOPIC, 2);
}

static void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Disconnected\r\n");
}

static void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Published\r\n");
}

static void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
  char *topicBuf = (char*)os_zalloc(topic_len + 1),
        *dataBuf = (char*)os_zalloc(data_len + 1);
  MQTT_Client* client = (MQTT_Client*)args;
  os_memcpy(topicBuf, topic, topic_len);
  topicBuf[topic_len] = 0;
  os_memcpy(dataBuf, data, data_len);
  dataBuf[data_len] = 0;
  INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
  os_free(topicBuf);
  os_free(dataBuf);
}

void ICACHE_FLASH_ATTR print_info()
{
  INFO("\r\n\r\n[INFO] BOOTUP...\r\n");
  INFO("[INFO] SDK: %s\r\n", system_get_sdk_version());
  INFO("[INFO] Chip ID: %08X\r\n", system_get_chip_id());
  INFO("[INFO] Memory info:\r\n");
  system_print_meminfo();
  INFO("[INFO] -------------------------------------------\n");
  INFO("[INFO] Build time: %s\n", BUID_TIME);
  INFO("[INFO] -------------------------------------------\n");
}

void highPriorityTaskPoll(void *arg)
{
 
}

void mediumPriorityTaskPoll(void *arg)
{
    
}

void ICACHE_FLASH_ATTR lowPriorityTaskPoll(void *arg)
{
  
}

void ICACHE_FLASH_ATTR pollingTimerInit()
{  
    os_timer_disarm(&low_priority_poll_timer);
    os_timer_setfn(&high_priority_poll_timer, (os_timer_func_t *)highPriorityTaskPoll, NULL);
    os_timer_setfn(&medium_priority_poll_timer, (os_timer_func_t *)mediumPriorityTaskPoll, NULL);
    os_timer_setfn(&low_priority_poll_timer, (os_timer_func_t *)lowPriorityTaskPoll, NULL);
    os_timer_arm(&high_priority_poll_timer, 1, 1);
    os_timer_arm(&medium_priority_poll_timer, 2000, 1);
    os_timer_arm(&low_priority_poll_timer, 20000, 1);
}


static void ICACHE_FLASH_ATTR app_init(void)
{
  uart_init(BIT_RATE_19200, BIT_RATE_115200);
  print_info();
  MQTT_InitConnection(&mqttClient, MQTT_HOST, MQTT_PORT, DEFAULT_SECURITY);
  MQTT_InitClient(&mqttClient, MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS, MQTT_KEEPALIVE, MQTT_CLEAN_SESSION);
  MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
  MQTT_OnConnected(&mqttClient, mqttConnectedCb);
  MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
  MQTT_OnPublished(&mqttClient, mqttPublishedCb);
  MQTT_OnData(&mqttClient, mqttDataCb);
  WIFI_Connect(STA_SSID, STA_PASS, wifiConnectCb);
  pollingTimerInit();
}

void user_init(void)
{
  system_init_done_cb(app_init);
}