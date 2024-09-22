ENTRYPOINT = ota_updates.ino
FQBN_ARG = --fqbn esp32:esp32:esp32
DEVICE = ttyUSB0
PORT = /dev/$(DEVICE)
OTA_IP := $(shell ping -c 1 esp32-chessboard.local | grep 'PING' | awk -F'[()]' '{print $$2}')
BAUD = 115200
LOGFILE = /tmp/$(DEVICE)_output.txt

install_deps:
	sudo apt install inotify-tools socat

compile:
	arduino-cli core list
	arduino-cli compile $(FQBN_ARG) $(ENTRYPOINT)
  
upload:
	arduino-cli upload -p $(PORT) $(FQBN_ARG) $(ENTRYPOINT)

ota_upload:
	arduino-cli upload -p $(OTA_IP) $(FQBN_ARG) $(ENTRYPOINT) --upload-field password=

flash: compile upload

ota_flash: compile ota_upload

monitor:
	# arduino-cli monitor -p $(PORT) -c baudrate=115200
	# screen $(PORT) 115200 > /tmp/serial_log.txt
	# socat -u -b $(BAUD) $(PORT) stdout > $(LOGFILE)
	python3 monitor.py $(PORT) $(BAUD)

monitor_entrypoint:
	@echo "Monitoring changes to $(ENTRYPOINT)..."
	@while true; do \
		make monitor & \
		inotifywait -e modify $(ENTRYPOINT); \
		make stop_monitor; \
		make flash; \
	done; \
	make stop_monitor

stop_monitor:
	# @pkill -f "arduino-cli monitor" || true
	# @pkill -f "screen $(PORT)" || true
	@pkill -f "python3 monitor.py" || true
