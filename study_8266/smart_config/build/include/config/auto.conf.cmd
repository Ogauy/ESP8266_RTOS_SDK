deps_config := \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/app_update/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/aws_iot/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/console/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/esp8266/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/esp_http_client/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/esp_http_server/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/freertos/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/libsodium/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/log/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/lwip/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/mdns/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/mqtt/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/newlib/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/pthread/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/spiffs/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/ssl/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/tcpip_adapter/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/util/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/vfs/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/wifi_provisioning/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/wpa_supplicant/Kconfig \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/bootloader/Kconfig.projbuild \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/esptool_py/Kconfig.projbuild \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/components/partition_table/Kconfig.projbuild \
	/home/xingxing/software/eclipse/project/esp/ESP8266_RTOS_SDK/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
