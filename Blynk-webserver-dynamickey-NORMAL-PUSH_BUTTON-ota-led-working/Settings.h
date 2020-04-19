/*
 * General options
 */

#define BOARD_FIRMWARE_VERSION        "1.0.1"
#define BOARD_HARDWARE_VERSION        "1.0.0"

#define BOARD_NAME                    "A-Link"        // Name of your product. Should match App Export request info.
#define BOARD_VENDOR                  "SUDHIR"        // Name of your company. Should match App Export request info.
#define BOARD_TEMPLATE_ID             "A-LINK001A"    // 001a,001b,001c....ID of the Tile Template. Can be found in Tile Template Settings

#define PRODUCT_WIFI_SSID             "A-LINK"         // Name of the device, to be displayed during configuration. Should match export request info.
#define BOARD_CONFIG_AP_URL           "a-link.cc"      // Config page will be available in a browser at 'http://our-product.cc/'

/*
 * Board configuration (see examples below).
 */

#if defined(USE_NODE_MCU_BOARD)

  #warning "NodeMCU board selected"

  // Example configuration for NodeMCU v1.0 Board
  #define BOARD_BUTTON_PIN            0
  #define BOARD_BUTTON_ACTIVE_LOW     true

  #define BOARD_LED_PIN               15
  #define BOARD_LED_INVERSE           false
  #define BOARD_LED_BRIGHTNESS        64
  
//  #define BOARD_LED_PIN_R             15
 // #define BOARD_LED_PIN_G             12
  //#define BOARD_LED_PIN_B             13
#else
  #error "No board selected"
#endif


/*
 * Advanced options
 */

#define BUTTON_HOLD_TIME_INDICATION   3000
#define BUTTON_HOLD_TIME_ACTION       10000

#define BOARD_PWM_MAX                 1023

#define WIFI_NET_CONNECT_TIMEOUT      30000
#define WIFI_CLOUD_CONNECT_TIMEOUT    15000
#define WIFI_AP_CONFIG_PORT           80
#define WIFI_AP_IP                    IPAddress(192, 168, 4, 1)
#define WIFI_AP_Subnet                IPAddress(255, 255, 255, 0)
//#define WIFI_CAPTIVE_PORTAL_ENABLE

#define USE_TICKER
//#define USE_TIMER_ONE
//#define USE_TIMER_THREE

#if defined(APP_DEBUG)
  #define DEBUG_PRINT(...) BLYNK_LOG1(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
#endif
