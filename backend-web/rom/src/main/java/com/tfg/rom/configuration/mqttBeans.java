package com.tfg.rom.configuration;

import java.io.Serial;
import java.util.ArrayList;
import java.util.Optional;

import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.integration.channel.DirectChannel;
import org.springframework.integration.core.MessageProducer;
import org.springframework.integration.mqtt.core.DefaultMqttPahoClientFactory;
import org.springframework.integration.mqtt.core.MqttPahoClientFactory;
import org.springframework.integration.mqtt.inbound.MqttPahoMessageDrivenChannelAdapter;
import org.springframework.integration.mqtt.outbound.MqttPahoMessageHandler;
import org.springframework.integration.mqtt.support.DefaultPahoMessageConverter;
import org.springframework.integration.mqtt.support.MqttHeaders;
import org.springframework.messaging.Message;
import org.springframework.messaging.MessageChannel;
import org.springframework.messaging.MessageHandler;
import org.springframework.messaging.MessagingException;

import com.fasterxml.jackson.core.sym.Name;
import com.tfg.rom.Exception.DeviceNotFoundException;
import com.tfg.rom.entity.Device;
import com.tfg.rom.repository.DeviceRepository;
import com.tfg.rom.service.DeviceService;
import com.tfg.rom.utils.utils;



@Configuration
public class mqttBeans {
	
	@Autowired
	private DeviceService deviceService ;
	
	@Value("${mqttUrl}")
	private String mqttUrl;
	
	@Bean
	public MqttPahoClientFactory mqttClientFactory() {
		DefaultMqttPahoClientFactory factory = new DefaultMqttPahoClientFactory();
		MqttConnectOptions options = new MqttConnectOptions();
		
		//System.out.println(mqttUrl);
		options.setServerURIs(new String[] { "tcp://"+mqttUrl+":1883" });
		options.setCleanSession(true);

		factory.setConnectionOptions(options);
		
		return factory;
	}
	@Bean
	public MessageChannel mqttInputChannel() {
		return new DirectChannel();
	}
	
	//Para suscribirse
	@Bean(name ="inBound")
	public MessageProducer inbound() {
		MqttPahoMessageDrivenChannelAdapter adapter = new MqttPahoMessageDrivenChannelAdapter("serverIn",
				mqttClientFactory(), "server/status");
		

		adapter.setCompletionTimeout(5000);
		adapter.setConverter(new DefaultPahoMessageConverter());
		adapter.setQos(2);
		adapter.setOutputChannel(mqttInputChannel());
		return adapter;
	}
	
	
	@Bean
	@ServiceActivator(inputChannel = "mqttInputChannel")
	public MessageHandler handler() {
		return new MessageHandler() {

			@Override
			public void handleMessage(Message<?> message) throws MessagingException {
				String topic = message.getHeaders().get(MqttHeaders.RECEIVED_TOPIC).toString();
				if(topic.equals("autoConfig")) {
					
					Device auxDevice = utils.getNewDevice(message.getPayload().toString());
					
					if(!deviceService.findNewDevices().contains(auxDevice)) {
						deviceService.setNewDevice(auxDevice);
						System.out.println("AGREGAMOS NUEVO DEVICE A LISTA");
						System.out.println(deviceService.findNewDevices().toString());
					}
					
				}else {
					try {
						Device device = deviceService.getByRoute(topic);
						deviceService.setStatus(deviceService.getByRoute(topic).getId(),message.getPayload().toString());
						//System.out.println(message.getPayload().toString());
					} catch (Exception e1) {
						System.out.println(e1);
					}
				}
				
			}
				
			

		};
	}
	
	
	@Bean
    public MessageChannel mqttOutboundChannel() {
        return new DirectChannel();
    }
	@Bean
    @ServiceActivator(inputChannel = "mqttOutboundChannel")
    public MessageHandler mqttOutbound() {
        //clientId is generated using a random number
        MqttPahoMessageHandler messageHandler = new MqttPahoMessageHandler("serverOut", mqttClientFactory());
        messageHandler.setAsync(true);
        messageHandler.setDefaultTopic("#");
        messageHandler.setDefaultRetained(false);
        return messageHandler;
    }

}
