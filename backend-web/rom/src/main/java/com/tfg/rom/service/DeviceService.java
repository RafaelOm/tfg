package com.tfg.rom.service;

import org.apache.logging.log4j.message.StringFormattedMessage;
import org.apache.logging.log4j.util.StringBuilderFormattable;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.context.event.ApplicationReadyEvent;
import org.springframework.context.event.EventListener;
import org.springframework.integration.annotation.Publisher;
import org.springframework.integration.mqtt.inbound.MqttPahoMessageDrivenChannelAdapter;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Optional;

import javax.transaction.Transactional;

import com.tfg.rom.Exception.DeviceNotFoundException;
import com.tfg.rom.configuration.mqttGateway;
import com.tfg.rom.entity.Device;
import com.tfg.rom.repository.DeviceRepository;
import com.tfg.rom.utils.Constants;



@Service
public class DeviceService {

	private final DeviceRepository deviceRepository ;
	
	
	private List<Device> newDevices = new ArrayList<Device>();
	
	@Autowired
	mqttGateway mqttGateway;
	
	@Autowired
	private BeanFactory beanFactory;
	
	@Autowired
	public DeviceService(DeviceRepository deviceRepository) {
		this.deviceRepository =deviceRepository;
	}
	
	
	
	public List<Device> getDevices(){
		
		return deviceRepository.findAll();
	}
	
	public Device getOneDevice(Integer id) throws Exception{
		
	Optional<Device> deviceOptional=deviceRepository.findById(id);
	if(!deviceOptional.isPresent()) {
		throw new DeviceNotFoundException(id);
	}
		
		return deviceOptional.get();
	}
	public void addDevice(Device device, Integer controlledDeviceId){
		
		/*Optional<Device> deviceOptional =deviceRepository.findById(device.getId());
		if(deviceOptional.isPresent()) {
			throw new IllegalStateException("The device already exists!");
		}*/
		
		//AGREGAR TOPIC
		MqttPahoMessageDrivenChannelAdapter adapter=(MqttPahoMessageDrivenChannelAdapter)beanFactory.getBean("inBound");
		adapter.addTopic(device.getroute());
		
		if(controlledDeviceId != null) {
			device.setControlledDevice(deviceRepository.findById(controlledDeviceId).orElseThrow(() -> 
						new IllegalStateException("The device "+ controlledDeviceId+ " does not exists!")));
		}
			
		 deviceRepository.save(device);
	}
	
	public Device setStatus(Integer id, String status) throws Exception {
		Device device = getOneDevice(id);
		device.setStatus(status);
		deviceRepository.save(device);
		
		return device;
	}
	public Device setStatusAndSend (Integer id, String status) throws Exception {
		Device device = getOneDevice(id);
		device.setStatus(status);
		deviceRepository.save(device);
		mqttGateway.sendToMqtt(status, device.getroute());
		
		return device;
	}
	
	public void setMovementShutter(Integer id, String movement) throws Exception {
		Device device = getOneDevice(id);
		mqttGateway.sendToMqtt(movement, device.getShutterMoveRoute());
		
		
	}
	
	public Device getByRoute(String route) throws Exception{
		
		Optional<Device> deviceOptional=deviceRepository.findByRoute(route);
		if(!deviceOptional.isPresent()) {
			throw new DeviceNotFoundException(0);
		}
			
			return deviceOptional.get();
		}
	
	public List<Device> findNewDevices(){
		
		return newDevices;
	}
	public void setNewDevice(Device dev) {
			newDevices.add(dev);
	}
	
	public void saveAutoDevice(String id) {
		Device auxDevice = new Device();
		for(Device device : newDevices) {
			if(device.getName().equals(id)) {
				auxDevice =device;
				deviceRepository.save(auxDevice);
				newDevices.remove(device);
				System.out.println("GUARDAMOS DEVICE EN EL SISTEMA");
				System.out.println(newDevices.toString());
				break;
			}
		}
		mqttGateway.sendToMqtt("addDevice", auxDevice.getName());
		MqttPahoMessageDrivenChannelAdapter adapter=(MqttPahoMessageDrivenChannelAdapter)beanFactory.getBean("inBound");
		adapter.addTopic(auxDevice.getroute());
		
		
		
	}
	
	@Transactional
	public void updateDevice(Integer deviceId, String name, String type, String route, String shutterMoveRoute) {
		
		Device device = deviceRepository.findById(deviceId).orElseThrow(() -> 
				new IllegalStateException("The device "+ deviceId+ " does not exists!"));
		
		if(name != null) {
			device.setName(name);
		}
		if(type != null) {
			device.setType(type);
		}
		if(route !=null) {
			device.setroute(route);
		}
		if (shutterMoveRoute !=null) {
			device.setShutterMoveRoute(shutterMoveRoute);
		}
	}
	
	public void deleteDevice(Integer id) {
		Device device = deviceRepository.findById(id).orElseThrow(() -> 
		new IllegalStateException("The device "+ id+ " does not exists!"));
		MqttPahoMessageDrivenChannelAdapter adapter=(MqttPahoMessageDrivenChannelAdapter)beanFactory.getBean("inBound");
		if(device.getroute()!=null && device.getroute().length()>1) {
			adapter.removeTopic(device.getroute());
		}
		if(device.getControlledDevice()!=null) {
			device.setControlledDevice(null);
		}
		
		if(deviceRepository.findByControlledDevice(device).isPresent()) {
			Device deviceOfControll = deviceRepository.findByControlledDevice(device).get();
			this.deleteDevice(deviceOfControll.getId());
		}
	
		
		deviceRepository.delete(device);		
		
	}
	
	public void addSensor(String name, Integer controlledDeviceId, Integer lowerLimit, Integer upperLimit) {
		
		Device auxDevice = new Device();
		for(Device device : newDevices) {
			if(device.getName().equals(name)) {
				auxDevice =device;
				Device controlledDevice =deviceRepository.findById(controlledDeviceId).orElseThrow(() -> 
				new IllegalStateException("The device "+ controlledDeviceId+ " does not exists!"));
				auxDevice.setControlledDevice(controlledDevice);
				auxDevice.setroute(null);
				auxDevice.setShutterMoveRoute(null);
				deviceRepository.save(auxDevice);
				newDevices.remove(device);
				setSensorContolledDevice(controlledDevice,auxDevice, lowerLimit,upperLimit);
				
				break;
			}
		}
		
	}
	
	
	
	private void setSensorContolledDevice(Device controlledDevice,Device sensor,Integer lowerLimit,Integer upperLimmit) {
			if(controlledDevice.getType().equals("shutter")) {
				mqttGateway.sendToMqtt(controlledDevice.getShutterMoveRoute(), sensor.getName());
				
			}else if(sensor.getType().equals("Light sensor") && lowerLimit!= null && upperLimmit !=null) {
				mqttGateway.sendToMqtt(controlledDevice.getroute()+" "+lowerLimit+" "+upperLimmit,sensor.getName());
				System.out.println("Enviado: "+controlledDevice.getroute()+" "+lowerLimit+" "+upperLimmit);
			}
			else {
				mqttGateway.sendToMqtt(controlledDevice.getroute(), sensor.getName());
			}
		
	}
	
	@EventListener(ApplicationReadyEvent.class)
	public void runAfterStartup() {
	    System.out.println("*******Connecting devices with MQTT server********");
	    MqttPahoMessageDrivenChannelAdapter adapter=(MqttPahoMessageDrivenChannelAdapter)beanFactory.getBean("inBound");
	    List<Device> devices = this.getDevices();
	  
	    for(Device d: devices) {
	    	if(d.getroute()!=null &&d.getroute().length()>1) {
	    		adapter.addTopic(d.getroute());
	    	}
	    }
	    adapter.addTopic("autoConfig");	   
	}
	
	
	
}
