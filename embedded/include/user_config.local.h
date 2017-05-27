#ifndef __MQTT_CONFIG_H__
#define __MQTT_CONFIG_H__

#define MQTT_SSL_ENABLE

/*DEFAULT CONFIGURATIONS*/

#define MQTT_HOST     "m12.cloudmqtt.com"
#define MQTT_PORT     11572
#define MQTT_BUF_SIZE   1024
#define MQTT_KEEPALIVE    120  //second

#define MQTT_CLIENT_ID "home_auto"
#define MQTT_USER "mcvwzaab"
#define MQTT_PASS "KNTEblLVGO-6"
#define MQTT_CLEAN_SESSION 1



#define PUBLISHER_TOPIC "home_automation_response"
#define  SUBSCRIBER_TOPIC "home_automation"


#define AP_SSID "home_automation_project"
#define AP_PASS "home_automation"
#define STA_SSID "vacuous"
#define STA_PASS "tech_sam"

#define MQTT_RECONNECT_TIMEOUT  5 /*second*/

#define DEFAULT_SECURITY  0
#define QUEUE_BUFFER_SIZE       2048

#define PROTOCOL_NAMEv31  /*MQTT version 3.1 compatible with Mosquitto v0.15*/
//PROTOCOL_NAMEv311     /*MQTT version 3.11 compatible with https://eclipse.org/paho/clients/testing/*/

#if defined(DEBUG_ON)
#define INFO( format, ... ) os_printf( format, ## __VA_ARGS__ )
#else
#define INFO( format, ... )
#endif

#endif // __MQTT_CONFIG_H__
