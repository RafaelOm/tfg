package com.tfg.rom.repository;

import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import com.tfg.rom.entity.Device;

@Repository
public interface DeviceRepository extends JpaRepository<Device, Integer> {
	
	Optional<Device> findByRoute(String route);
	Optional<Device> findByControlledDevice(Device controlled_device);

}
