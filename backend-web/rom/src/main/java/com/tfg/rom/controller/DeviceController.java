package com.tfg.rom.controller;

import java.util.LinkedList;
import java.util.List;

import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.tfg.rom.entity.Device;
import com.tfg.rom.service.DeviceService;

@RestController
@CrossOrigin(origins = "*")
@RequestMapping(path="domoApi/devices")
public class DeviceController {
	
	private final DeviceService deviceService;
	
	public DeviceController(DeviceService deviceService) {
		this.deviceService=deviceService;
	}
	
	@GetMapping
	public List<Device> getAllDevices() {
		return deviceService.getDevices();
	}
	@GetMapping("/{id}")
	public Device getOneDevices(@PathVariable Integer id) throws Exception {
		return deviceService.getOneDevice(id);
	}
	
	@PostMapping
	public void addDevice(@RequestBody Device device ,
			@RequestParam(required = false)Integer controlledDeviceId) {
		deviceService.addDevice(device,controlledDeviceId);
	}
	
	
	
	@PostMapping("/setStatus/{id}/{status}")
	public void setStatus(@PathVariable Integer id, @PathVariable String status) throws Exception {
		deviceService.setStatusAndSend(id, status);
		
	}
	@PostMapping("/setMoveShutter/{id}/{moveDirection}")
	public void setMoveShutter(@PathVariable Integer id, @PathVariable String moveDirection) throws Exception {
		
		switch (moveDirection) {
		case "Up":
			moveDirection="1";
			break;
		case "Down":
			moveDirection="0";
			break;
		case "Stop":
			moveDirection="00";
			break;

		default:
			break;
		}
		deviceService.setMovementShutter(id, moveDirection);
		
	}
	
	@GetMapping("/findNewDevices")
	public List<Device> findNewDevices() {
		return deviceService.findNewDevices();
	}
	@PostMapping("/addAutoDevices/{id}")
	public void findNewDevices(@PathVariable String id) {
		 deviceService.saveAutoDevice(id);
	}
	
	@PostMapping("/addSensor/{sensorName}")
	public void addSensor(@PathVariable String sensorName,
							@RequestParam Integer controlledDeviceId, 
							@RequestParam(required = false) Integer lowerLimit,
							@RequestParam(required = false) Integer upperLimit) {
		deviceService.addSensor(sensorName,controlledDeviceId,lowerLimit,upperLimit);
	}
	
	
	@PutMapping("/{id}")
	public void modifyDevice(@PathVariable Integer id,
								@RequestParam(required = false) String name,
								@RequestParam(required = false) String type,
								@RequestParam(required = false) String route,
								@RequestParam(required = false) String shutterMoveRoute
								) {
		deviceService.updateDevice(id,name,type,route,shutterMoveRoute);
		
	}
	
	@DeleteMapping("/{id}")
	public void deleteDevice(@PathVariable Integer id) {
		deviceService.deleteDevice(id);
	}

}
