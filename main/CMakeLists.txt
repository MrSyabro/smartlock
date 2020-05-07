set(COMPONENT_SRCS "app_main.c")
set(COMPONENT_ADD_INCLUDEDIRS "include")
set(COMPONENT_REQUIRES tcpip_adapter nvs_flash lock_hardware lock_connect lock_access)

register_component()
