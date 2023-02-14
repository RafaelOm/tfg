package com.tfg.rom.configuration;

import org.springframework.integration.annotation.MessagingGateway;
import org.springframework.integration.mqtt.support.MqttHeaders;
import org.springframework.messaging.handler.annotation.Header;

@MessagingGateway(defaultRequestChannel = "mqttOutboundChannel")
public interface mqttGateway {

	void sendToMqtt(String data, @Header(MqttHeaders.TOPIC) String topic);
}
