#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// for a flash and nvs
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/bluetooth/hci_vs.h>

#include <zephyr/drivers/gpio.h>

// add a ble related header file
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/types.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>
#include <string.h>
#include <zephyr/sys/reboot.h>

// Removed uint24_t struct and helpers, using byte manipulation directly

// #define DRIFT_FACTOR 1.003191f
#define DRIFT_FACTOR 1.000040f

#define DUPLICAT_INTRUPT_IN_TIME_M 1 //
#define DATA_BUFER_SIZE 200
#define BLE_DATA_SIZE 200 // 200 elements * 3 bytes + 4 bytes UUID = 604 bytes buffer
// ble telated data
char CONFIG_BT_DEVICE_NAME_1[] = "ExtAdvDemo1";

/** @brief Local MAC address buffer (6 bytes). */
static uint8_t mac_data[6] __attribute__((unused));

uint8_t TRANSMIT_POWER_dBm = RADIO_TXPOWER_TXPOWER_0dBm;

#define ISC_BEACON_UUID BT_UUID_128_ENCODE(0x1A957D0F, 0x78C2, 0x4C95, 0xBFDF, 0xBF483E7B67AC)

const float FULL_VOLT = 2.899; // Full voltage is 3V
const float LOW_VOLT = 1.999;  // Low voltage is 2V
float ISC_VOLT;

// #include <zephyr/sys/reboot.h>

uint8_t battery_percentage[3] = {
    0x0F, // UUID part 1
    0x18, // UUID part 2
    0x00, // Placeholder for the percentage value (will be updated later)
};

#define BLE_PART1_ELEMENTS 83
#define BLE_PART2_ELEMENTS 83
#define BLE_PART3_ELEMENTS 34

#define BLE_PART1_BYTE_SIZE (2 + BLE_PART1_ELEMENTS * 3)
#define BLE_PART2_BYTE_SIZE (2 + BLE_PART2_ELEMENTS * 3)
#define BLE_PART3_BYTE_SIZE (2 + BLE_PART3_ELEMENTS * 3)

uint8_t timestamp_adv_payload_buffer1[BLE_PART1_BYTE_SIZE];
uint8_t timestamp_adv_payload_buffer2[BLE_PART2_BYTE_SIZE];
uint8_t timestamp_adv_payload_buffer3[BLE_PART3_BYTE_SIZE];

static inline void set_timestamp_data_element(int idx, uint32_t val) {
    uint8_t *dest;
    if (idx < BLE_PART1_ELEMENTS) {
        dest = &timestamp_adv_payload_buffer1[2 + idx * 3];
    } else if (idx < BLE_PART1_ELEMENTS + BLE_PART2_ELEMENTS) {
        dest = &timestamp_adv_payload_buffer2[2 + (idx - BLE_PART1_ELEMENTS) * 3];
    } else {
        dest = &timestamp_adv_payload_buffer3[2 + (idx - BLE_PART1_ELEMENTS - BLE_PART2_ELEMENTS) * 3];
    }
    dest[0] = (val >> 16) & 0xFF;
    dest[1] = (val >> 8) & 0xFF;
    dest[2] = val & 0xFF;
}

static inline uint32_t get_timestamp_data_element(int idx) {
    const uint8_t *src;
    if (idx < BLE_PART1_ELEMENTS) {
        src = &timestamp_adv_payload_buffer1[2 + idx * 3];
    } else if (idx < BLE_PART1_ELEMENTS + BLE_PART2_ELEMENTS) {
        src = &timestamp_adv_payload_buffer2[2 + (idx - BLE_PART1_ELEMENTS) * 3];
    } else {
        src = &timestamp_adv_payload_buffer3[2 + (idx - BLE_PART1_ELEMENTS - BLE_PART2_ELEMENTS) * 3];
    }
    return (src[0] << 16) | (src[1] << 8) | src[2];
}

// Advertising data
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, ISC_BEACON_UUID),
    // BT_DATA(BT_DATA_TX_POWER, &TRANSMIT_POWER_dBm, 1),
    // BT_DATA(BT_DATA_SVC_DATA16, battery_percentage, sizeof(battery_percentage)),
    BT_DATA(BT_DATA_SVC_DATA16, timestamp_adv_payload_buffer1, BLE_PART1_BYTE_SIZE),
    BT_DATA(BT_DATA_SVC_DATA16, timestamp_adv_payload_buffer2, BLE_PART2_BYTE_SIZE),
    BT_DATA(BT_DATA_SVC_DATA16, timestamp_adv_payload_buffer3, BLE_PART3_BYTE_SIZE),
};

static struct bt_le_ext_adv *ext_adv;

struct bt_le_adv_param adv_param = {
    // .options = BT_LE_ADV_OPT_SCANNABLE |
    //            BT_LE_ADV_OPT_EXT_ADV |
    //            BT_LE_ADV_OPT_USE_IDENTITY,
    .options = BT_LE_ADV_OPT_EXT_ADV |
               BT_LE_ADV_OPT_USE_IDENTITY,
    .id = BT_ID_DEFAULT,
    // .interval_min = BT_GAP_ADV_FAST_INT_MIN_2,
    // .interval_max = BT_GAP_ADV_FAST_INT_MAX_2,
    .interval_min = (300 / 0.625),
    .interval_max = (350 / 0.625),
};

// for a flash and nvs
static struct nvs_fs fs;
uint16_t flash_data = 0;
uint16_t flash_tx_p_data = 0;
#define NVS_PARTITION storage_partition
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(NVS_PARTITION)

#define ADDRESS_ID 1
#define TX_P_ID 2


#define SW_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW_NODE, gpios);
static struct gpio_callback button_cb;

#define LED_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

// for a temp value
int err, rc, i, j, temp, temp_flsh_up_counter_id;
uint64_t uptime_ms = 0, uptime_s = 0;
uint32_t uptime_m =  0, uptime_m_priv = 0, uptime_m_priv_temp = 0;

uint8_t reset_flag = 1;

// for a buton flag
bool button_pressed = false;
bool long_press_detected = false;

// for flash data
uint16_t flsh_up_counter_id = 0;
uint16_t store_flsh_up_cont_id = DATA_BUFER_SIZE + 1;
uint32_t upcount_bufer[DATA_BUFER_SIZE];
uint16_t store_flsh_up_time_id = DATA_BUFER_SIZE + 2;

/*
 *general purpuse work falag defind
 *0 - idle
 *1 - button pressed
 *2 - timer intrup for up time store in a flash
 *3 - long press detected for a restart device
 */
uint8_t general_purpus_work_flag = 0;

void set_mack_id(void);   // set a mac id in ble
void start_ble_adv(void); // start a ble extended adv

// timer handler for a store up time in a flash
void up_time_store_timer_handler(struct k_timer *timer);
K_TIMER_DEFINE(up_time_store_timer, up_time_store_timer_handler, NULL);

// timer handler for a store up time in a flash
void button_timer_handler(struct k_timer *timer);
K_TIMER_DEFINE(button_timer, button_timer_handler, NULL);

// timer handler for a store up time in a flash
void button_press_timer_handler(struct k_timer *timer);
K_TIMER_DEFINE(button_press_timer, button_press_timer_handler, NULL);

// add for a conection enabel
void genral_purpus_work_handler(struct k_work *work);
K_WORK_DEFINE(dbg_adv_work, genral_purpus_work_handler);

void genral_purpus_work_handler(struct k_work *work)
{
    if (general_purpus_work_flag == 1)
    {
        general_purpus_work_flag = 0;
        ////////////////////////////////////////////////////
        /* Read current button level */
        int val = gpio_pin_get_dt(&button); // button is your gpio_dt_spec

        printk("Button interrupt, level=%d\r\n", val);
        if (!(val))
        {
            button_pressed = true;
            printk("button press button_pressed=%d\n", button_pressed);
            k_timer_start(&button_timer, K_MSEC(5000), K_NO_WAIT);
        }
        else
        {
            button_pressed = false;
            long_press_detected = false;
            printk("button press button_pressed=%d\n", button_pressed);
            k_timer_stop(&button_timer);
            return;
        }
        ///////////////////////////////////////////////
        uptime_ms = k_uptime_get();
        printk("1 Uptime_ms: %lld\n", uptime_ms);
        uptime_ms = (int64_t)(uptime_ms * DRIFT_FACTOR); // drift corrected
        printk("2 Uptime_ms: %lld\n", uptime_ms);
        uptime_s = uptime_ms / 1000;
        printk("Uptime_s: %lld\n", uptime_s);
        uptime_m = ((uptime_s / 60) + uptime_m_priv) & 0xFFFFFF; // this is a reayal
        // uptime_m = ((uptime_s) + uptime_m_priv) & 0xFFFFFF; // for testing purpase
        printk("uptime_m_priv=%d\n", uptime_m_priv);
        printk("Uptime_m: %d, %x\n", uptime_m, uptime_m);

        if (flsh_up_counter_id >= DATA_BUFER_SIZE)
        {
            temp = flsh_up_counter_id - DATA_BUFER_SIZE;
        }
        else
        {
            temp = flsh_up_counter_id;
        }
        printk("upcount_bufer[%d]=%d\n", temp, upcount_bufer[temp]);
        printk("uptime_m - upcount_bufer[temp]=%d\n", uptime_m - upcount_bufer[temp]);

        printk("upcount_bufer[%d-1]=%d\n", temp, upcount_bufer[temp - 1]);
        printk("uptime_m - upcount_bufer[temp]=%d\n", uptime_m - upcount_bufer[temp - 1]);

        uint32_t uptime = (uptime_m - upcount_bufer[temp - 1]) & 0xFFFFFF;
        printk("uptime_m - upcount_bufer[temp]=%d\n", uptime);

        if ((temp > 0) && (uptime < DUPLICAT_INTRUPT_IN_TIME_M))
        {
            // int temp_1;
            if (reset_flag == 1)
            {
                reset_flag = 0;
                printk("falg set\n");
                // for (i = 0; i < DATA_BUFER_SIZE; i++)
                // {
                //     // temp_1 = timestamp_data[i] & 0xff00;
                //     // temp_1 = temp_1 >> 8;
                //     // temp_1 = temp_1 | ((timestamp_data[i] & 0x00ff) << 8);
                //     // timestamp_data[i]=temp_1;

                //     temp_1 = upcount_bufer[i] & 0xff00;
                //     temp_1 = temp_1 >> 8;
                //     temp_1 = temp_1 | ((upcount_bufer[i] & 0x00ff) << 8);
                //     timestamp_data[i] = temp_1 & 0xffff;
                // }

                for (i = temp-1, j = 1; i >= 0 && j <= BLE_DATA_SIZE; i--, j++)
                {
                    printk("ok1 i=%d j=%d\n", i, j);
                    set_timestamp_data_element(j - 1, upcount_bufer[i]);
                    // printk("%x ", get_timestamp_data_element(j - 1));
                    printk("%x ", upcount_bufer[i]);
                    k_msleep(10);
                }
            }

            printk("same uptime_m found no need to store \n retunr\n");
            start_ble_adv(); // start a ble extended adv
            k_msleep(2000);
            bt_le_ext_adv_stop(ext_adv); // stop adv
            printk("extendede adv stoped\n");

            return;
        }
        printk("0 flsh_up_counter_id=%d temp=%d\n", flsh_up_counter_id, temp);
        nvs_write(&fs, temp, &uptime_m, sizeof(uptime_m));
        upcount_bufer[temp] = uptime_m;

        if (flsh_up_counter_id >= ((DATA_BUFER_SIZE * 2) - 1))
        {
            flsh_up_counter_id = DATA_BUFER_SIZE - 1;
        }
        flsh_up_counter_id++;
        nvs_write(&fs, store_flsh_up_cont_id, &flsh_up_counter_id, sizeof(flsh_up_counter_id));
        rc = nvs_write(&fs, store_flsh_up_time_id, &uptime_m, sizeof(uptime_m));
        if (rc < 0)
        {
            printk("error in store uptime_m rc=%d\n", rc);
        }
        else
        {
            printk("uptime store success runtime\n");
        }
        // print_upcount_bufer();

        printk("1 flsh_up_counter_id=%d header pointer=%d\n", flsh_up_counter_id, temp);
        k_msleep(10);
        temp_flsh_up_counter_id = flsh_up_counter_id - 1;
        if (temp_flsh_up_counter_id >= DATA_BUFER_SIZE)
        {
            printk("ble data = ");
            temp = temp_flsh_up_counter_id - DATA_BUFER_SIZE;
            for (i = temp, j = 1; j <= BLE_DATA_SIZE; i--, j++)
            {
                // printk("ok2 i=%d j=%d\n", i, j);
                set_timestamp_data_element(j - 1, upcount_bufer[i]);

                if (i == 0)
                {
                    i = DATA_BUFER_SIZE;
                }
                // printk("%x ", get_timestamp_data_element(j - 1));
                printk("%x ", upcount_bufer[i]);
                k_msleep(10);
            }
            printk("\n");
        }
        else
        {
            printk("ble data = ");
            temp = temp_flsh_up_counter_id;
            for (i = temp, j = 1; i >= 0 && j <= BLE_DATA_SIZE; i--, j++)
            {
                // printk("ok1 i=%d j=%d\n", i, j);
                set_timestamp_data_element(j - 1, upcount_bufer[i]);
                // printk("%x ", get_timestamp_data_element(j - 1));
                printk("%x ", upcount_bufer[i]);
                k_msleep(10);
            }
            printk("\n");
        }
        // rc = bt_le_ext_adv_set_data(ext_adv, ad, ARRAY_SIZE(ad), NULL, 0);
        // if (rc)
        // {
        //     printk("Failed to set extended adv data (err %d)\n", rc);
        //     return;
        // }
        // k_msleep(100);
        // set_mack_id();   // set a mac id in ble
        gpio_pin_set_dt(&led, 1); // led on
        start_ble_adv();          // start a ble extended adv
        k_msleep(2000);
        bt_le_ext_adv_stop(ext_adv); // stop adv
        gpio_pin_set_dt(&led, 0);    // off led
        printk("extendede adv stoped\n");
    }
    else if (general_purpus_work_flag == 2)
    {
        general_purpus_work_flag = 0;
        uptime_ms = k_uptime_get();
        printk("1 Uptime_ms: %lld\n", uptime_ms);
        uptime_ms = (int64_t)(uptime_ms * DRIFT_FACTOR); // drift corrected
        printk("2 Uptime_ms: %lld\n", uptime_ms);
        uptime_s = uptime_ms / 1000;
        printk("Uptime_s: %lld\n", uptime_s);
        uptime_m = ((uptime_s / 60) + uptime_m_priv) & 0xFFFFFF; // this is a reayal
        // uptime_m = ((uptime_s) + uptime_m_priv) & 0xFFFFFF; // for testing purpase
        printk("Uptime_m: %d\n", uptime_m);

        rc = nvs_write(&fs, store_flsh_up_time_id, &uptime_m, sizeof(uptime_m));
        if (rc < 0)
        {
            printk("error in store uptime_m rc=%d\n", rc);
        }
        else
        {
            printk("uptime store success fulle\n");
        }

        rc = nvs_read(&fs, store_flsh_up_time_id, &uptime_m_priv_temp, sizeof(uptime_m_priv_temp));
        if (rc > 0)
        {
            printk("data read uptime_m_priv_temp =%d\n", uptime_m_priv_temp);
        }
        else
        {
            printk("no data found uptime_m_priv_temp\n");
        }
        k_msleep(10);
    }
    else if (general_purpus_work_flag == 3) // long press detected for restart device
    {
        general_purpus_work_flag = 0;

        flsh_up_counter_id = 0; // reset a header pointer and store in a falsh
        nvs_write(&fs, store_flsh_up_cont_id, &flsh_up_counter_id, sizeof(flsh_up_counter_id));

        for (int i = 0; i < BLE_DATA_SIZE; i++)
        {
            set_timestamp_data_element(i, 0); // reset a ble data buffer
        }
        for (int i = 0; i < DATA_BUFER_SIZE; i++)
        {
            upcount_bufer[i] = 0;  // reser a up time buffer
        }
        uptime_m = 0; // reset a up time
        rc = nvs_write(&fs, store_flsh_up_time_id, &uptime_m, sizeof(uptime_m));
        if (rc < 0)
        {
            printk("error in store uptime_m rc=%d\n", rc);
        }
        else
        {
            printk("uptime store success fulle\n");
        }
        k_msleep(500);
        printk("System is restarting...\n");
        sys_reboot(SYS_REBOOT_COLD);
    }
}

void button_pressed_cb(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button Pressed!\r\n");
    general_purpus_work_flag = 1;
    k_work_submit(&dbg_adv_work);
}

void up_time_store_timer_handler(struct k_timer *timer)
{
    printk("up_time_store_timer_handler called\n");
    general_purpus_work_flag = 2;
    k_work_submit(&dbg_adv_work);
}

void button_press_timer_handler(struct k_timer *timer)
{
    printk("Button Pressed! from timer\r\n");
    general_purpus_work_flag = 1;
    k_work_submit(&dbg_adv_work);
}

void button_timer_handler(struct k_timer *timer)
{
    if (button_pressed)
    {
        long_press_detected = true;
        printk("long press detection\n");
        general_purpus_work_flag = 3;
        k_work_submit(&dbg_adv_work);
    }
}

void configure_peripherals(void)
{
    if (!gpio_is_ready_dt(&button))
    {
        // return;
    }
    if (!gpio_is_ready_dt(&led))
    {
        // return;
    }
    err = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (err)
    {
        printk("Button init failed (err %d)\n", err);
        // return;
    }
    err = gpio_pin_interrupt_configure_dt(&button, (GPIO_INT_EDGE_BOTH));
    // err = gpio_pin_interrupt_configure_dt(&button, (GPIO_INT_EDGE_FALLING));
    if (err)
    {
        printk("Button init failed (err %d)\n", err);
        // return;
    }

    gpio_init_callback(&button_cb, button_pressed_cb, BIT(button.pin));

    gpio_add_callback_dt(&button, &button_cb);
    err = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (err)
    {
        printk("LED init failed (err %d)\n", err);
        // return;
    }
    gpio_pin_set_dt(&led, 0); // led is off
    printk("LED and button configured\n");
}

void read_flsh_data(void)
{
    rc = nvs_read(&fs, store_flsh_up_cont_id, &flsh_up_counter_id, sizeof(flsh_up_counter_id));
    if (rc > 0)
    {
        printk("data read flsh_up_counter_id = %d\n", flsh_up_counter_id);
        if (flsh_up_counter_id >= DATA_BUFER_SIZE)
        {
            temp = DATA_BUFER_SIZE;
        }
        else
        {
            temp = flsh_up_counter_id;
        }

        for (i = 0; i < temp; i++)
        {
            rc = nvs_read(&fs, i, &uptime_m, sizeof(uptime_m));
            if (rc > 0)
            {
                printk("data read from flash i=%d value=%d\n", i, uptime_m);
                upcount_bufer[i] = uptime_m;
            }
            else
            {
                upcount_bufer[i] = 0;
            }
            k_msleep(10);
        }
    }
    else
    {
        printk("no data found flsh_up_counter_id\n");
    }
    k_msleep(10);
    rc = nvs_read(&fs, store_flsh_up_time_id, &uptime_m_priv, sizeof(uptime_m_priv));
    if (rc > 0)
    {
        printk("data read uptime_m_priv =%d\n", uptime_m_priv);
    }
    else
    {
        printk("no data found uptime_m_priv\n");
        // uptime_m_priv = 16777213 ;
    }
    k_msleep(10);
}
void print_upcount_bufer(void)
{
    printk("flsh_up_counter_id=%d\n", flsh_up_counter_id);
    if (flsh_up_counter_id >= DATA_BUFER_SIZE)
    {
        temp = DATA_BUFER_SIZE;
    }
    else
    {
        temp = flsh_up_counter_id;
    }
    for (i = 0; i < temp; i++)
    {
        printk("upcount_bufer[%d]=%d hex=%x\n", i, upcount_bufer[i], upcount_bufer[i]);
        k_msleep(10);
    }
}

// set a mac id in ble
void set_mack_id(void)
{
    // read a resitar valu
    uint32_t addr_low = NRF_FICR->DEVICEADDR[0];
    uint32_t addr_high = NRF_FICR->DEVICEADDR[1];

    // DEVICEADDR is stored as 48-bit address (addr_high:addr_low)
    mac_data[0] = (addr_low >> 0) & 0xFF;
    mac_data[1] = (addr_low >> 8) & 0xFF;
    mac_data[2] = (addr_low >> 16) & 0xFF;
    mac_data[3] = (addr_low >> 24) & 0xFF;
    mac_data[4] = (addr_high >> 0) & 0xFF;
    mac_data[5] = ((addr_high >> 8) & 0xFF);

    err = bt_ctlr_set_public_addr(mac_data); // set a mac id
    printk("bt_ctlr_set_public_addr err=%d\n", err);

    printk("mac id=%x %x %X %x %x %x\n", mac_data[5], mac_data[4], mac_data[3], mac_data[2], mac_data[1], mac_data[0]);
}

void start_ble_adv(void)
{
    // err = bt_enable(NULL);
    // if (err)
    // {
    //     printk("Bluetooth init failed (err %d)\n", err);
    //     return;
    // }
    // else
    // {
    //     printk("Bluetooth initialized\n");
    // }

    // err = bt_le_ext_adv_create(&adv_param, NULL, &ext_adv);
    // if (err)
    // {
    //     printk("Failed to create extended advertiser (err %d)\n", err);
    //     return;
    // }
    // printk("Extended advertiser created\n");
    err = bt_le_ext_adv_set_data(ext_adv, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err)
    {
        printk("Failed to set extended adv data (err %d)\n", err);
        return;
    }
    printk("Extended adv data set\n");

    err = bt_le_ext_adv_start(ext_adv, BT_LE_EXT_ADV_START_DEFAULT);
    if (err)
    {
        printk("Failed to start extended adv (err %d)\n", err);
        return;
    }
    printk("Extended advertising started\n");
}

void inin_flash(void)
{
    struct flash_pages_info info;
    // int rc = 0;

    fs.flash_device = NVS_PARTITION_DEVICE;
    if (!device_is_ready(fs.flash_device))
    {
        printk("Flash device %s is not ready\n", fs.flash_device->name);
        return;
    }
    fs.offset = NVS_PARTITION_OFFSET;
    rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
    if (rc)
    {
        printk("Unable to get page info\n");
        return;
    }
    fs.sector_size = info.size;
    fs.sector_count = 3U;

    rc = nvs_mount(&fs);
    if (rc)
    {
        printk("Flash Init failed\n");
        return;
    }
    printk("flash init success\n");
}

void update_a_ble_data(void)
{
    temp_flsh_up_counter_id = flsh_up_counter_id - 1;
    if (temp_flsh_up_counter_id >= DATA_BUFER_SIZE)
    {
        temp = temp_flsh_up_counter_id - DATA_BUFER_SIZE;
        for (i = temp, j = 1; j <= BLE_DATA_SIZE; i--, j++)
        {
            printk("ok2 i=%d j=%d\n", i, j);
            set_timestamp_data_element(j - 1, upcount_bufer[i]);
            if (i == 0)
            {
                i = DATA_BUFER_SIZE;
            }
            printk("timestamp_data %d %x\n", get_timestamp_data_element(j - 1), get_timestamp_data_element(j - 1));
            k_msleep(10);
        }
    }
    else
    {
        temp = temp_flsh_up_counter_id;
        for (i = temp, j = 1; i >= 0 && j <= BLE_DATA_SIZE; i--, j++)
        {
            printk("ok1 i=%d j=%d\n", i, j);
            set_timestamp_data_element(j - 1, upcount_bufer[i]);
            printk("timestamp_data %d %x\n", get_timestamp_data_element(j - 1), get_timestamp_data_element(j - 1));
            k_msleep(10);
        }
    }

    if (temp_flsh_up_counter_id >= DATA_BUFER_SIZE)
    {
        printk("ble data = ");
        temp = temp_flsh_up_counter_id - DATA_BUFER_SIZE;
        for (i = temp, j = 1; j <= BLE_DATA_SIZE; i--, j++)
        {
            // printk("ok2 i=%d j=%d\n", i, j);
            set_timestamp_data_element(j - 1, upcount_bufer[i]);

            if (i == 0)
            {
                i = DATA_BUFER_SIZE;
            }
            // printk("%x ", get_timestamp_data_element(j - 1));
            printk("%x ", upcount_bufer[i]);
            k_msleep(10);
        }
        printk("\n");
    }
    else
    {
        printk("ble data = ");
        temp = temp_flsh_up_counter_id;
        for (i = temp, j = 1; i >= 0 && j <= BLE_DATA_SIZE; i--, j++)
        {
            // printk("ok1 i=%d j=%d\n", i, j);
            set_timestamp_data_element(j - 1, upcount_bufer[i]);
            // printk("%x ", get_timestamp_data_element(j - 1));
            printk("%x ", upcount_bufer[i]);
            k_msleep(10);
        }
        printk("\n");
    }
}
void main(void)
{

    inin_flash();

    read_flsh_data(); // data read from flash

    print_upcount_bufer(); // prrint a buffer

    update_a_ble_data();
    k_msleep(500);

    reset_flag = 1; // set flag only on reset

    configure_peripherals(); // configrabel buttorn

    uint16_t uuid = sys_cpu_to_be16(0xaabb);
    memcpy(timestamp_adv_payload_buffer1, &uuid, sizeof(uuid));
    memcpy(timestamp_adv_payload_buffer2, &uuid, sizeof(uuid));
    memcpy(timestamp_adv_payload_buffer3, &uuid, sizeof(uuid));

    set_mack_id(); // set a mac id in ble

    err = bt_enable(NULL);
    if (err)
    {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }
    else
    {
        printk("Bluetooth initialized.\n");
    }
    err = bt_le_ext_adv_create(&adv_param, NULL, &ext_adv);
    if (err)
    {
        printk("Failed to create extended advertiser (err %d)\n", err);
        return;
    }
    printk("Extended advertiser created\n");

    // start a timer for a store a up timr in a flash every 5 minutes
    k_timer_start(&up_time_store_timer, K_MINUTES(5), K_MINUTES(5));
    // k_timer_start(&up_time_store_timer, K_SECONDS(5), K_SECONDS(5)); // for testing purpuse

    // k_msleep(5000);

    // k_timer_start(&button_press_timer, K_MINUTES(15), K_MINUTES(15));
    // // start_ble_adv(); // start a ble extended adv
}