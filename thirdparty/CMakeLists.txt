
# libpigpio.(so|a)
add_library(pigpio 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/pigpio.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/command.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/custom.cext)

# libpigpiod_if.(so|a)
add_library(pigpiod_if 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/pigpiod_if.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/command.c)

# libpigpiod_if2.(so|a)
add_library(pigpiod_if2
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/pigpiod_if2.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/pigpio/command.c)

    add_compile_definitions("-DMG_ARCH=MG_ARCH_UNIX")

add_library(mongoose
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/mongoose.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/base64.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/device_dummy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/dns.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/event.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/fmt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/fs_fat.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/fs_packed.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/fs_posix.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/fs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/http.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/iobuf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/json.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/md5.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/mqtt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/net_builtin.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/net.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/ota_dummy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/ota_esp32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/ota_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/printf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/rpc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/sha1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/sha256.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/sntp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/sock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/ssi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/str.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/timer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/tls_aes128.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/tls_builtin.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/tls_chacha20.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/tls_dummy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/tls_mbed.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/tls_openssl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/tls_uecc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/tls_x25519.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/url.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/util.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose/src/ws.c
)
