#include "s4396122_os_mqtt.h"

Client client;
int connected = 0;

void mqtt_messagehandler(MessageData *data);

void s4396122_os_mqtt_init() {
    xTaskCreate(&MQTT_Task, "MQTT", configMINIMAL_STACK_SIZE * 8, NULL, mqttTask_PRIORITY, NULL);
}

void connect_network(Network *n, char *addr, int port) {
    int addrSize;
    /*int sock;*/
    struct sockaddr_in sAddr;

    /* Create a TCP Socket */
    if ((n->my_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        debug_printf("can not create socket");
        return;
    }

    /*mqtt_socket = n->my_socket;*/

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons((unsigned short) port);
    sAddr.sin_addr.s_addr = inet_addr(addr);

    if (connect(n->my_socket, (struct sockaddr *)(&sAddr), sizeof (sAddr)) < 0) {
        debug_printf("MQTT ERROR connecting");
        return;
    }
}

int network_read(Network *n, char *buffer, int len, int timeout) {
    read(n->my_socket, buffer, len);
}

void MQTT_Task(void) {

    char buff[100];
    char readBuff[100];
    
    Network network;
    NewNetwork(&network);
    connect_network(&network, "192.168.0.1", 1883);
    debug_printf("Socket Connected\n");

    MQTTClient(&client, &network, 1000, buff, 100, readBuff, 100);
    debug_printf("Created Client\n");
    client.ipstack->mqttread = network_read;
    client.defaultMessageHandler = mqtt_messagehandler;
    debug_printf("Waiting\n");
    /*vTaskDelay(5000);*/

    MQTTConnect(&client, NULL);
    debug_printf("Connected\n");
    connected = 1;

    s4396122_os_mqtt_publish("debug", "MQTT Init");
    s4396122_os_mqtt_subscribe("cmd");
    s4396122_os_mqtt_publish("debug", "MQTT Subbed");

    while(1) {

        Timer timer;
        InitTimer(&timer);
        countdown_ms(&timer, 1000);
        cycle(&client, &timer);

        s4396122_os_mqtt_publish("loop", "Hello");
        vTaskDelay(100);
    }
}

void s4396122_os_mqtt_publish(char *inTopic, char *message) {
    char topic[50];
    memset(topic, 0, sizeof(topic));
    sprintf(topic, "np2/%s", inTopic);
    MQTTMessage new_message;
    new_message.payloadlen = strlen(message);
    new_message.payload = message;
    new_message.qos = QOS0;
    MQTTPublish(&client, topic, &new_message);
}

/**
 * @brief Message handler for incoming (published) messages.
 *
 * @param data MessageData struct pointer. Contains Qos info and payload
 */
void mqtt_messagehandler(MessageData *data) {
    char buffer[data->message->payloadlen];
    strncpy(buffer, data->message->payload, data->message->payloadlen);
    debug_printf("Got: %s\n", data->topicName->lenstring.data, buffer);
}

void s4396122_os_mqtt_subscribe(char *inTopic) {
    if (!connected) {
        debug_printf("Not initialized yet\n");
        return;
    }
    char topic[50];
    memset(topic, 0, sizeof(topic));
    sprintf(topic, "np2/%s", inTopic);
    if (MQTTSubscribe(&client, topic, QOS0, mqtt_messagehandler) == FAILURE) {
        debug_printf("Error\n");
    }
}
