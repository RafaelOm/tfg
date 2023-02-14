package com.tfg.rom.Exception;

public class DeviceNotFoundException extends RuntimeException{
	
	public DeviceNotFoundException(Integer id) {
		super("The device "+id+" does  not exists");
	}

}
