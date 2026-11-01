<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.10.0" doxygen_gitid="ebc57c6dd303a980bd19dd74b8b61c8f3f5180ca">
  <compound kind="file">
    <name>esp32_bus_setup.hpp</name>
    <path>examples/esp32/main/</path>
    <filename>esp32__bus__setup_8hpp.html</filename>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <member kind="function">
      <type>EspI2cBus *</type>
      <name>get_shared_i2c_bus</name>
      <anchorfile>esp32__bus__setup_8hpp.html</anchorfile>
      <anchor>a34120f53f38b468eb02c353cd79d5b0b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>BaseI2c *</type>
      <name>get_i2c_device</name>
      <anchorfile>esp32__bus__setup_8hpp.html</anchorfile>
      <anchor>a7024544c8de144283c563c05320b05bf</anchor>
      <arglist>(uint8_t address) noexcept</arglist>
    </member>
    <member kind="function">
      <type>EspSpiBus *</type>
      <name>get_shared_spi_bus</name>
      <anchorfile>esp32__bus__setup_8hpp.html</anchorfile>
      <anchor>a8357d890d1a32152038dc6c7b5baf360</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::unique_ptr&lt; EspGpio &gt;</type>
      <name>create_gpio</name>
      <anchorfile>esp32__bus__setup_8hpp.html</anchorfile>
      <anchor>ac35786de67943f429d4ead245b81c664</anchor>
      <arglist>(int pin_num, hf_gpio_direction_t direction=hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT, hf_gpio_active_state_t active_state=hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH) noexcept</arglist>
    </member>
    <member kind="function">
      <type>EspAdc *</type>
      <name>get_shared_adc</name>
      <anchorfile>esp32__bus__setup_8hpp.html</anchorfile>
      <anchor>aeb7b6a567a9cbf97eceef16473bbf5ea</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>scan_i2c_bus</name>
      <anchorfile>esp32__bus__setup_8hpp.html</anchorfile>
      <anchor>a821a151721a0a19f59e6177184b68d50</anchor>
      <arglist>(EspI2cBus *bus) noexcept</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>BUS_TAG</name>
      <anchorfile>esp32__bus__setup_8hpp.html</anchorfile>
      <anchor>a73a911be261c628a3ee57cedbafb89a1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>esp32_test_config.hpp</name>
    <path>examples/esp32/main/</path>
    <filename>esp32__test__config_8hpp.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>PIN_I2C_SDA</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>afd84f048502b749115c9e05262ed6753</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_I2C_SCL</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a6b5cc0023af0923f5e57d107bdd8f985</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>I2C_PORT_NUM</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a09996f09e16ea78e133dea56a72955af</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>I2C_CLOCK_HZ</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a072752feab1cd0eeb8c2c0b3e71a52db</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_SPI_MOSI</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a78530c2d4dd5656190162d062b1fed5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_SPI_MISO</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a5a7c039615f027ce1f975f625e9b15ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_SPI_SCLK</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>afdf5870bb1b9d7d2bbd243dc9a11c107</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SPI_HOST_ID</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a906fc067289d6f5d4bd4ba065f5076f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_AS5047U_CS</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a4b30deced476079c4b99a027e331b5ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>AS5047U_SPI_CLOCK_HZ</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a01a2e7b8fa7e5d5508dec9ab7b56892b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BNO08X_I2C_ADDR</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>ae1efef7f5775f2273ec71672bbfcab1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_BNO08X_INT</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a7d924b600eaa8b106093c7aa719af6a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_BNO08X_RST</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>aaf6d8f6941d5766db736be006a37e2d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PCA9685_I2C_ADDR</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a7d596b1d6807872117bf0ea7167cc26f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SE050_I2C_ADDR</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a81f760ab76bbe8522f0d54d977085321</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PCAL95555_I2C_ADDR</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a6742ea041bf54af0bcf7b8f7c1ae4bbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_PCAL95555_INT</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>afe53210edc20c3ac9d8f2c19a2639750</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NTC_ADC_CHANNEL</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>ad50f7a00a9490c1d82531a641107323a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NTC_ADC_UNIT</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>afd02ed4557b678c3d05edc812c08caef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TMC9660_CS</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>aaa0e53e29cf7c8ad43b7b4b57ee2031e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TMC9660_RST</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a38c2ebd294686ddb6528d664da7ba1e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TMC9660_DRV_EN</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a0c18e9185035fe4f951e84904f9b5e1a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TMC9660_FAULTN</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a1ff73a99b2a4e8fdd9885c5a8e757bc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TMC9660_WAKE</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a28704089a036b8d3619f1e1d2a9117ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TMC9660_DEVICE_ADDR</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a2522abade5d2bba1dfbf4feb2171dd22</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TMC9660_SPI_CLOCK_HZ</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a95ad56bdf457fa500e38ce447875f353</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TMC5160_CS</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a51d52526ec02e2a5bde80bf093223d09</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TMC5160_DRV_ENN</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a4e3310c1127e70263d2286cc3c08b491</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TMC5160_DIAG0</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a569f3de7eda5c543544366e61f9e2065</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TMC5160_DIAG1</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a700041d2d1284e5fb4b98f03980e69b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TMC5160_SPI_CLOCK_HZ</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a0c15add35db378329f6c3e27c03a876d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TLE92466ED_CS</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a187bde5d24f26b73ed96277ed6f5463d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TLE92466ED_RESN</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>adb8875b25bd7832beaed776de438b640</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TLE92466ED_EN</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a69dd42df865e63f3942fe0289de8b7f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_TLE92466ED_FAULTN</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a847a6c7bb6a2fd076089de51c3674d07</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TLE92466ED_SPI_CLOCK_HZ</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a9ef94505e68c6bd244f5c04361e9392b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_MAX22200_CS</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>aa1f5bc6e4fc91d2a50198ece152bd7ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_MAX22200_ENABLE</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a9cd49df81957793aea8e18b42638beec</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_MAX22200_CMD</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a12b818de8ba20a6aab9559b98a33c949</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_MAX22200_FAULT</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>ad79101ada0a57ce92a21de005bd3c3c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX22200_SPI_CLOCK_HZ</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a42b3ef2726e0ebe6595009ab120373c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_ADS9324_CS</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a718de5b361709f06c7bee59b24ab142c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_ADS9324_CONVST</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a97283d023c8881a3600dd46eaa26daa9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_ADS9324_DRDY</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>af042d6f0103500690a9993af20072cc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADS9324_SPI_CLOCK_HZ</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a30c420ec6af4c244c8f72e89a59cb039</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_ISF15ACP4_SPI_DUMMY_CS</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a4c8c95f93a26ed566eec93bc1990b4a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_ISF15ACP4_CS</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a657b6dda3fa19b2b729e24b6c27ac00f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_ISF15ACP4_DC</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a5503569571b4327e67f7be0a84f51eb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_ISF15ACP4_RES</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>ac179a9a9ef45c3e825c6c7596b40b8c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_ISF15ACP4_VCC_EN</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a0ed34ebf4708b502bd2849d8f0d9df27</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_ISF15ACP4_SWITCH</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a215a301219376e5e81c51d9d2e9417d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISF15ACP4_SPI_CLOCK_HZ</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a756d41a4ac6510fa92c73003a9215240</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PIN_WS2812_DATA</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a6e70c494b43ea9b014d2acdcf3fe4701</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WS2812_NUM_LEDS</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>a2064736e92a062f362ef988ab6cd1265</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WS2812_RMT_CHANNEL</name>
      <anchorfile>esp32__test__config_8hpp.html</anchorfile>
      <anchor>aad656bcac55800262adb2404b2e930d0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ads9324_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>ads9324__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Ads9324Handler_8h" name="Ads9324Handler.h" local="yes" import="no" module="no" objc="no">handlers/ads9324/Ads9324Handler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_construction</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac5de30a9290d751035ea1495fef9cd6d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_initialized</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1cb3b24c961b118f52be141f4f6c8e3b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_max_channels</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>afef6ead47fbc038e5693d23f06ec0abc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_channel_available</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af8b05658ba5295fe5ae850a6ff197911</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_configure_channel</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a627d8f21005fc89e3f0aa2293af688ef</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_offset_gain</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac2e6ec0e0b546faadea5e9273722a6bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_snapshot</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab05efeed27d06f5d0e573017ab73eb4f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_channel_base_adc</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6df4c4eda103261e1b355df4cda1790c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_invalid_channel</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>acf1ac3d34067d6aa8fc275b4e453961a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_visit_driver</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a515900eefde60c41d19ec27480bd93c8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_driver</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6cd03c8da610595be5157e4d0507c3cc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_dump_diagnostics</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae9aa597545f9f6529d499cc52fd62832</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_missing_convst</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1799899f8c3aee1c3a165132449bbe33</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_deinitialize</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a474bb530f591331a31711724a2e45a54</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONSTRUCTION_TESTS</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a67ae550a9bdc50865266b9277403d095</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CHANNEL_MAP_TESTS</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a156d3c6b42d76764c6ab74b3227df2a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_PGA_TESTS</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8291a061499c4e4cedb000f9a83f96db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_READ_TESTS</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8ddecc324813c99eb6c1c7a97af454bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DRIVER_ACCESS_TESTS</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af1dc429c05da4a4b5be51c809eb2b4b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DIAGNOSTICS_TESTS</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2e723b1283d9cf5fb62f391f378447d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_convst_gpio</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5d1544c0d2b8b70f29c9d74435d914f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_drdy_gpio</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a31a97f7e6541a0d847d47a17e69211a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static BaseSpi *</type>
      <name>g_spi_device</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a425dddf639ccb19b44f7657fe7045389</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Ads9324Handler &gt;</type>
      <name>g_handler</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5f31b39e03ffeed9fcd87bd4593ec39d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>g_hw_present</name>
      <anchorfile>ads9324__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a41c7713ad13517741b17ec422ca5b273</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>as5047u_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>as5047u__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="As5047uHandler_8h" name="As5047uHandler.h" local="yes" import="no" module="no" objc="no">handlers/as5047u/As5047uHandler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_handler_construction</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5844cf1754ee73d31a05039be52231b3</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_initialized</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1cb3b24c961b118f52be141f4f6c8e3b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_angle_raw</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aba85d793687893b19bb9fedc2e925625</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_angle_degrees</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae748208ee2c6327a34a5041ad68af5a0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_angle_consistency</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad3a7324df7618ad71a625be571cfc342</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_velocity</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa512d2571c5fe3ca326494a8b511bcec</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_daec_enable_disable</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a447c85f6ab1ed95fd4385c26b7526568</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_zero_position</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a153219dbdbf2b72e8928235f1197942d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_dump_diagnostics</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae9aa597545f9f6529d499cc52fd62832</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_sensor</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad3875a6423df168d57cc565d9f9f4f3c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_operations_before_init</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac83f97b7754666b5e7733941b37c5630</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>concurrent_reader_task</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaa3a69b37bf8fcd8431b8495e9b311ca</anchor>
      <arglist>(void *param)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_concurrent_access</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae9b39916d49066b59641c5ae10376c84</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ANGLE_TESTS</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6125450dbd324bc686228e0c630b57bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_VELOCITY_TESTS</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa755fec9e5b07c6ef25ffe517f1cde86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DAEC_TESTS</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a20c6c0b48af30b22cdd2679642e70a9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ZERO_POSITION_TESTS</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a26a1d953e6ff3f52b1c7126a28cf612d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DIAGNOSTICS_TESTS</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2e723b1283d9cf5fb62f391f378447d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_THREAD_SAFETY_TESTS</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a37c454ffbe978dc87a61bb2638fa4f5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static BaseSpi *</type>
      <name>g_spi_device</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a425dddf639ccb19b44f7657fe7045389</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; As5047uHandler &gt;</type>
      <name>g_handler</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7f55b2fd79f3fa5937e400314e3c2c0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile bool</type>
      <name>g_thread_test_pass</name>
      <anchorfile>as5047u__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2ee3082da9d64bba843b5a64815e0493</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>bno08x_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>bno08x__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Bno08xHandler_8h" name="Bno08xHandler.h" local="yes" import="no" module="no" objc="no">handlers/bno08x/Bno08xHandler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_construction</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac5de30a9290d751035ea1495fef9cd6d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_initialized</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1cb3b24c961b118f52be141f4f6c8e3b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_accelerometer</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1a0cf4facd447884b468da5a46e75410</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_disable_sensor</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7ffe94c82a5f0a7175b47dd201f314bc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_acceleration</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a81d7e86e5aeb378cf288e8ee565e6237</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_imu_data</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac9e8563a88a220775c0ee6f8b5569168</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_freshness_gating</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a9cbf1babc8e669aebf6b00a2554c5516</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_full_config</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a974fe8f2d7df88d4c1938eb6c0a08aa1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_hardware_reset</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a99a50cdde879dc28c0115746f601e183</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_operations_before_init</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac83f97b7754666b5e7733941b37c5630</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_SENSOR_CONTROL_TESTS</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a51a7cd7e8d43831dc0f0ad3b876c42de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DATA_READING_TESTS</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad3eea42c9079264e7fba3ecb98d3ec61</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_FRESHNESS_TESTS</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7074ef833654cea7c339d779d36866f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONFIG_TESTS</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a54625a8336bef3cfd1209481343c5654</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CALLBACK_TESTS</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a0a8d4d070d9a30498e4b478604a72dfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_HARDWARE_CTRL_TESTS</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a912b468e91300727338ff193c0333f20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_THREAD_SAFETY_TESTS</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a37c454ffbe978dc87a61bb2638fa4f5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_int_gpio</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abc244f8c9ad68acdfdf962ad21e18df9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_rst_gpio</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a88cdd37ab80a0e197d32dff99108b32e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Bno08xHandler &gt;</type>
      <name>g_handler</name>
      <anchorfile>bno08x__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a206de7f2592ea9ebbbb29d589fc455d9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>isf15acp4_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>isf15acp4__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Isf15acp4Handler_8h" name="Isf15acp4Handler.h" local="yes" import="no" module="no" objc="no">handlers/isf15acp4/Isf15acp4Handler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_construction</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac5de30a9290d751035ea1495fef9cd6d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_initialized</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1cb3b24c961b118f52be141f4f6c8e3b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_display_on_off</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad69511270866f4e7b1ba02c66161e700</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_fill_and_present</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa161c3843a99354688293d6889d48bf4</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_graphics_context</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab303323307fdd52cd7669781db50d33b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_button_ui</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab24e5381cb570445a63c71de1e1036e9</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_visit_driver</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a515900eefde60c41d19ec27480bd93c8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_driver</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6cd03c8da610595be5157e4d0507c3cc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_error_flags</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af80454392b48c25f2c9bddf71400f65f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_dump_diagnostics</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae9aa597545f9f6529d499cc52fd62832</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_operations_before_init</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac83f97b7754666b5e7733941b37c5630</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_deinitialize</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a474bb530f591331a31711724a2e45a54</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONSTRUCTION_TESTS</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a67ae550a9bdc50865266b9277403d095</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DISPLAY_TESTS</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a24435a7fa937158e19d5d43c9fbe5bcb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_GRAPHICS_TESTS</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af74f965b551cc8c7e2772d24bf86f807</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_BUTTON_TESTS</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af483bf0f0695092c0e7a681ec9a068d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DRIVER_ACCESS_TESTS</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af1dc429c05da4a4b5be51c809eb2b4b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DIAGNOSTICS_TESTS</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2e723b1283d9cf5fb62f391f378447d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_cs_gpio</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a4b0bb8739444d750543850fec65a26a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_dc_gpio</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a33203ff30049c1de8fe6dbb7f1a2650e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_res_gpio</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a824ebc401c8e545bb03fe8f449241aff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_vcc_gpio</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6ac80ede1212dd74681d825fe67f5f87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_switch_gpio</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3a27a74f97ccd79aff72c2d10ad15975</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static BaseSpi *</type>
      <name>g_spi_device</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a425dddf639ccb19b44f7657fe7045389</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Isf15acp4Handler &gt;</type>
      <name>g_handler</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af6357c1dd633e1f45f1ffd8b284ff74e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>g_hw_present</name>
      <anchorfile>isf15acp4__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a41c7713ad13517741b17ec422ca5b273</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>max22200_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>max22200__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Max22200Handler_8h" name="Max22200Handler.h" local="yes" import="no" module="no" objc="no">handlers/max22200/Max22200Handler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_construction</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac5de30a9290d751035ea1495fef9cd6d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_initialized</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1cb3b24c961b118f52be141f4f6c8e3b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_channel</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8c43350e03fa558a29318ea7fba63060</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_disable_channel</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a29c418f053ca988d587a9b6591dcc1e6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_all_channels</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>afee7768b8b35ea709bb0a2dd90c24287</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_disable_all_channels</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a030fb84f940dc80de1baa1613667ab6e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_channel_enabled</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aac1d4d55d72bbf78c3581872f267f99e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_channels_mask</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>afa13c761b08b8c746e506677ee356613</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_channel_range_guard</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a509aacdf5107f88835f48ba190140097</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_setup_cdr_channel</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab8b3a547fdfe48d80821cf78d919e792</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_setup_vdr_channel</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a506009ebc7bbaaebdb5294d43c63078b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_status</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3b01b348ce01a9f0fdd75cfba6f61c95</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_channel_faults</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a52bc89b10178c9c2d61749f059a3a7bc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_has_fault</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6e6ecb8e8113d98ef060fcf3eade5667</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_clear_faults</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a164f095cc2877928d5b5703c782ae77e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_fault_register</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a85106f7c195fdb5012e5fd5f8db900bf</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_driver_access</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>adbea5cf1f275c2a8c2f6de84c76d8908</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_disable_device</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a72a13813426327d3392bf6d1e4b87499</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_dump_diagnostics</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae9aa597545f9f6529d499cc52fd62832</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_operations_before_init</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac83f97b7754666b5e7733941b37c5630</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_deinitialize</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a474bb530f591331a31711724a2e45a54</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONSTRUCTION_TESTS</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a67ae550a9bdc50865266b9277403d095</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CHANNEL_CONTROL_TESTS</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a997aef295e4e15924ee11fdafc5218b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CHANNEL_SETUP_TESTS</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae254ae69281341238e17e6c8c95237a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_STATUS_TESTS</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a16479e08a1abef7505428fcbdf49eaed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DEVICE_CONTROL_TESTS</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6a14b40da782119f5f308557ce999e0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DIAGNOSTICS_TESTS</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2e723b1283d9cf5fb62f391f378447d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_enable_gpio</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a25eef147dc6ddb84f4695c9f227ff967</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_cmd_gpio</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>acc98a737ff6a4ea57324da995974262b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_fault_gpio</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7fd71f12f093be7e486b30ba7d726248</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static BaseSpi *</type>
      <name>g_spi_device</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a425dddf639ccb19b44f7657fe7045389</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Max22200Handler &gt;</type>
      <name>g_handler</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a52da828667c3b85a71039aedc8439066</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>g_hw_present</name>
      <anchorfile>max22200__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a41c7713ad13517741b17ec422ca5b273</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ntc_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>ntc__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="NtcTemperatureHandler_8h" name="NtcTemperatureHandler.h" local="yes" import="no" module="no" objc="no">handlers/ntc/NtcTemperatureHandler.h</includes>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>threshold_callback</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a336a78abf87ab8f51ccb1440d6094f39</anchor>
      <arglist>(BaseTemperature *sensor, float temperature, hf_u32_t type, void *user_data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sensor_info</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae362c848b61b117cca208af59f043c0a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_capabilities</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aab36b899c5160fa882f7657d4e0830cb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_temperature</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a67108face05ac1e7942a8f3ee042609a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_temperature_consistency</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a60b12f8c5a62ed010779e3911e5b3c22</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_calibration_offset</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a96017c98b6b0882c0580e85c97769f80</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_filtering</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a28f4caf68acee0b2917bf39e1dcf2104</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_threshold_monitoring</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab2fdff7f9c163c09cb3b7838092cf829</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>continuous_callback</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8f2e9dd101d42a48dd6774e898538ed8</anchor>
      <arglist>(BaseTemperature *sensor, const hf_temp_reading_t *reading, void *user_data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_continuous_monitoring</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>acab9ecfbf4add92193d2deb9bc247aac</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_statistics</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aebe87eea1c9b8f6f244eac9ef3b44db8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_diagnostics</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac7a22007aa641ea5a476dcaf3e28da11</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sleep_mode</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1224add7f718c77de85a482e3196102f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_self_test</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a020b75bfe677ae1c19539bc79534fcf1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_health_check</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a624d5ce016d2c1a0c05175def6e8a4db</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_TEMPERATURE_READING_TESTS</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aae32ec38c68aec0d46f93c02bfdd50cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CALIBRATION_TESTS</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a89dc2a130b732cd450304538c4a58641</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_FILTERING_TESTS</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abeeb81d2d6260428003e7a0bc0ad7181</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_THRESHOLD_TESTS</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abe5917af47dc9860dc3835e8e71fc882</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONTINUOUS_MONITORING_TESTS</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7b9ef4d0344c0da67881e38fb5db17e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_STATISTICS_TESTS</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8ff0dd66c1fa6dd49fe61862618d523d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_SLEEP_MODE_TESTS</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>adbe75c30ea2350916ff21f071afb1531</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; NtcTemperatureHandler &gt;</type>
      <name>g_handler</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8875c2063548e95ed495dd3a2d7660ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile bool</type>
      <name>g_threshold_triggered</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7b7f7f165c1004e17e04cf7b6be7ce5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile float</type>
      <name>g_threshold_temperature</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab4a368e1a2587e08729cc38a982f9522</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static volatile int</type>
      <name>g_continuous_count</name>
      <anchorfile>ntc__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a46f3d4f29ca67e1734903ce0f5f1757b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>pca9685_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>pca9685__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Pca9685Handler_8h" name="Pca9685Handler.h" local="yes" import="no" module="no" objc="no">handlers/pca9685/Pca9685Handler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_duty_cycle</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a053a21f14b39acf63bca3167f7176c32</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_duty_raw</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aefb68bb4ff41dd7a88ac2154b18c5808</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_frequency</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa32e8e33a0b7160679e7f2cae4095f5a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sleep_wake</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a685fd757b6e9dd6ac2c939437faf40f1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_gpio_pin_wrapper</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7e6b7146abe2137802965534db305ee5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_error_invalid_channel</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a32fb0c3e356d50769fce00bb9149f7ca</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_PWM_DUTY_TESTS</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a240da635bf6dd898757a70c8c35fd3d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_FREQUENCY_TESTS</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1b1b4bd2393c7244f343aa5816f5019f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_SLEEP_WAKE_TESTS</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a28ad4cc00158e4311c2285901fb3d6e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_PWM_ADAPTER_TESTS</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a0365c60d23633170ac5e725b2cc2723d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_GPIO_PIN_TESTS</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a9ef3369ce053e4110d29f1a9240c09de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_PHASE_OFFSET_TESTS</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1392b17416c64f232b79cf145190b04</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Pca9685Handler &gt;</type>
      <name>g_handler</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa4bfeda8b834ed631e9776476f1bb5df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::shared_ptr&lt; BasePwm &gt;</type>
      <name>g_pwm</name>
      <anchorfile>pca9685__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a567676674d98e79afb9b749c89028f3a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>pcal95555_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>pcal95555__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Pcal95555Handler_8h" name="Pcal95555Handler.h" local="yes" import="no" module="no" objc="no">handlers/pcal95555/Pcal95555Handler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_chip_variant</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac1fc796a73f92613647b3a6fc858b9be</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_pin_direction</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ada313ca9b895bc3cf34aa3b00e36120f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_pin_write_read</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a4b446e42a69606c88fe99bcc37a3d8d7</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_toggle_pin</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a98a25cfeccd8f1dd34e85bf05eb80410</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_batch_write</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a98a52694751cec6c795b36ed77691dca</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_supports_interrupts</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ada5cad67f031a489f355164890b1a17d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_drain_pending_interrupts</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a54f2b0eea2928f4714144199d02f83b9</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_gpio_pin_wrapper</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7e6b7146abe2137802965534db305ee5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_error_invalid_pin</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab29542f82c12dce6320c9674a36e69a2</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_PIN_DIRECTION_TESTS</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a77b0813e86a62740b18b1ff2edd62e6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_PIN_READ_WRITE_TESTS</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af17ce57fc1fa9cbec0b5383f0fa14867</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_BATCH_OPERATIONS_TESTS</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>adfcd43676649090aed8d27ec52afd2b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INTERRUPT_TESTS</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa31e51812d9df745787a7576b087adba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CHIP_VARIANT_TESTS</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3dbb7e3498d2b05a8d9413b07f3e945c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_AGILE_IO_TESTS</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8cda428a05d0f6ef13e0e30d39b27512</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_GPIO_PIN_WRAPPER_TESTS</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a144f6cd917f89a23bdc8e3f9d490c9d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_int_gpio</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abc244f8c9ad68acdfdf962ad21e18df9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Pcal95555Handler &gt;</type>
      <name>g_handler</name>
      <anchorfile>pcal95555__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad9d4d3429ee01b1dde01170ace776910</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>se050_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>se050__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Se050Handler_8h" name="Se050Handler.h" local="yes" import="no" module="no" objc="no">handlers/se050/Se050Handler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_version</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8098c2380efecf35c9521090a8dcd2e0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_random</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae43b2eb6f2fe4cb248048db169dd0a8f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INIT_TESTS</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab5b0c2bb9ce717836b0e59a1e21b1d87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_APPLET_TESTS</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a24a0b302d75546dad881aa4a44037c84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Se050Handler &gt;</type>
      <name>g_handler</name>
      <anchorfile>se050__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ade90f504fe44e588b7a9e9cedb4c2c9b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tle92466ed_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>tle92466ed__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Tle92466edHandler_8h" name="Tle92466edHandler.h" local="yes" import="no" module="no" objc="no">handlers/tle92466ed/Tle92466edHandler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_construction</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac5de30a9290d751035ea1495fef9cd6d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_initialized</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1cb3b24c961b118f52be141f4f6c8e3b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_channel</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8c43350e03fa558a29318ea7fba63060</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_disable_channel</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a29c418f053ca988d587a9b6591dcc1e6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_all_channels</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>afee7768b8b35ea709bb0a2dd90c24287</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_disable_all_channels</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a030fb84f940dc80de1baa1613667ab6e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_channel_current</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3e01856ec38d0f65987f51ca4c4f8a31</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_configure_pwm_raw</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaa6b78b1267743e3de00c5fdcc349020</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_channel_range_guard</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a509aacdf5107f88835f48ba190140097</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_status</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3b01b348ce01a9f0fdd75cfba6f61c95</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_channel_diagnostics</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5902c6a437a5e8eb286f4e5abe37e431</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_fault_report</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a4624327debd6c70d3b0fdd3ad66f827c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_has_fault</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6e6ecb8e8113d98ef060fcf3eade5667</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_clear_faults</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a164f095cc2877928d5b5703c782ae77e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_kick_watchdog</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a0b8f00efedd15a6cb364c402b261c4a8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enter_mission_mode</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a74680e40ac52a407195575e5aa2ecc21</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_output_stage</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a52f62b8008105d3506a7d123333e3760</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_feedback_updates</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a0805bb6c2c4976c993d41e7c33804242</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_configure_dither_via_driver</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad512602b84848b5c9d3e24f782b968ce</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_average_current_after_dither</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a9cd3a0bc781ed25df0ff93145168b9dc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_chip_id</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2a96a1df5f80b91525617128f36538d5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_driver_access</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>adbea5cf1f275c2a8c2f6de84c76d8908</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_dump_diagnostics</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae9aa597545f9f6529d499cc52fd62832</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_operations_before_init</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac83f97b7754666b5e7733941b37c5630</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_deinitialize</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a474bb530f591331a31711724a2e45a54</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONSTRUCTION_TESTS</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a67ae550a9bdc50865266b9277403d095</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CHANNEL_CONTROL_TESTS</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a997aef295e4e15924ee11fdafc5218b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_STATUS_TESTS</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a16479e08a1abef7505428fcbdf49eaed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_WATCHDOG_TESTS</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>afd752f1cd065b10fc0c00db78e2ab9c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_FEEDBACK_PATH_TESTS</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a4da797042f132b985acb98622960201d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DEVICE_INFO_TESTS</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a0de488147ec88d63781a468ed8b1b707</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DIAGNOSTICS_TESTS</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2e723b1283d9cf5fb62f391f378447d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_resn_gpio</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a28e2c1ccf1bd855f31b187fadc5acac1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_en_gpio</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab0c7246b9018df9930545f2241180d2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_faultn_gpio</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3ef2ed9b127b43bfe097db5a19c02d64</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static BaseSpi *</type>
      <name>g_spi_device</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a425dddf639ccb19b44f7657fe7045389</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Tle92466edHandler &gt;</type>
      <name>g_handler</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3a90ad4389a9d7da93fe8c62c4aede90</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>g_hw_present</name>
      <anchorfile>tle92466ed__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a41c7713ad13517741b17ec422ca5b273</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tmc5160_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>tmc5160__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Tmc5160Handler_8h" name="Tmc5160Handler.h" local="yes" import="no" module="no" objc="no">handlers/tmc5160/Tmc5160Handler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_construction</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac5de30a9290d751035ea1495fef9cd6d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_initialized</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1cb3b24c961b118f52be141f4f6c8e3b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_driver_config_snapshot</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8aed4a317f42e8f61531d68184a0e2f1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_motor</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a60e6ef5f353caffdc65edbc6a6a90f2b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_motor_enabled</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab5121620dd7ea4d6096b43f5db4cf42d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_disable_motor</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a816885246954955d485e69d6f15e56a6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_target_velocity</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af243b35fd1b7eb085a4bd078330cfdeb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_target_position</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>adff9ef69982bdc6b1ca673397f1a32b8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_current_position</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5d4eaf43b40e55d17af3f2aa1a145e1a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_current_velocity</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3c97d3cb990700b6fa475f6cc6ccef86</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_stop</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a29b579498d8acaa9576c4a02a83ec959</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_current</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abe86715c9d633a129f40eaf244ac8576</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_standstill</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a466595c8cf306fa57c8eef0535b1ed02</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_target_reached</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a0d6f0a3e0cbcb3d356f59a6046114bf0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_overtemperature</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae89bbfc14731765a6a1cb97e75ef9333</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_stall_detected</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abbdd021a038e602cab6524fea4509b84</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_stallguard_result</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>adef39a94f19a7c69c1d45c1f5c26b855</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_chip_version</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abff4c024b896676b0f3650d37bf18aeb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_visit_driver_ramp_control</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab0d1d93af0a472cf0212be223cb6733f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_visit_driver_motor_control</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a027fc32513c84cb715eaa971fe791bc0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_visit_driver_encoder</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a15956a153e964d345dba14cff6022898</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_visit_driver_communication</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a9f31157d310a1ae6784b23819ff6d296</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_dump_diagnostics</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae9aa597545f9f6529d499cc52fd62832</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_operations_before_init</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac83f97b7754666b5e7733941b37c5630</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_deinitialize</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a474bb530f591331a31711724a2e45a54</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONSTRUCTION_TESTS</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a67ae550a9bdc50865266b9277403d095</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_MOTOR_ENABLE_TESTS</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aecfe95088d34034167af0c58d8514ef6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_MOTION_CONTROL_TESTS</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a95a5b4d87733c91cb2bad28dad6884b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_STATUS_TESTS</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a16479e08a1abef7505428fcbdf49eaed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_VISIT_DRIVER_TESTS</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae64e39768afd4980efce2ab40ba519b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DIAGNOSTICS_TESTS</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2e723b1283d9cf5fb62f391f378447d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_enable_gpio</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a25eef147dc6ddb84f4695c9f227ff967</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_diag0_gpio</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a406d9ba85477a1d171ed60ed2b2375fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_diag1_gpio</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a53847f8bdcd172280ac39830f3fb6c44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static BaseSpi *</type>
      <name>g_spi_device</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a425dddf639ccb19b44f7657fe7045389</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Tmc5160Handler &gt;</type>
      <name>g_handler</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a94ee56e8ec581e54088777649e3a6607</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>g_hw_present</name>
      <anchorfile>tmc5160__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a41c7713ad13517741b17ec422ca5b273</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>tmc9660_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>tmc9660__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Tmc9660Handler_8h" name="Tmc9660Handler.h" local="yes" import="no" module="no" objc="no">handlers/tmc9660/Tmc9660Handler.h</includes>
    <includes id="Tmc9660AdcWrapper_8h" name="Tmc9660AdcWrapper.h" local="yes" import="no" module="no" objc="no">handlers/tmc9660/Tmc9660AdcWrapper.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_construction</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac5de30a9290d751035ea1495fef9cd6d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_ready</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac092626386f689be5f5dcad0c96f6a87</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_read_parameter</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6df3949c50a59c457ffb64939e8d54b2</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_write_parameter</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6ff136907cff449951bb54de5585db32</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_target_velocity</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af243b35fd1b7eb085a4bd078330cfdeb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_target_position</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>adff9ef69982bdc6b1ca673397f1a32b8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_supply_voltage</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a034c4dfd17b352183f5a53a5ff752c55</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_chip_temperature</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a264c973dd34d22326162254c2e027bbd</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_motor_current</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a91a34f465b54030dcb95f508010a06be</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_status_flags</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab211e869f762bcf4e00d4e41f2c51cd5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_error_flags</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af80454392b48c25f2c9bddf71400f65f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_gpio17_wrapper</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1b5aee0d0a3683b28ff6764bdaf545a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_gpio18_wrapper</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a80fe64d072aba66360d6d4480c8079a5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_adc_wrapper</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6a50fad2edc142e731565b05193140b3</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_adc_wrapper_delegation</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5ea41d64190cf4629b350e006b1d4446</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_temperature_wrapper</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac46f8e35782292d0bae9d62ccd1691a9</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_driver_enable</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a643e4815b7fc692fb1ba87152e14636e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_operations_before_init</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac83f97b7754666b5e7733941b37c5630</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONSTRUCTION_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a67ae550a9bdc50865266b9277403d095</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_PARAMETER_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa2426cd2dfabbaf507101f87f03d4618</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_MOTOR_CONTROL_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a76c29de1b8c6ad360aa84fc460b45e5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_TELEMETRY_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac488b91750859680faf2cf1783c851ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_GPIO_WRAPPER_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a95410ca8457f6d2bdcb84c55e6ae570b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ADC_WRAPPER_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad38f0beda9c237223aa42f62678ecbcb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_TEMP_WRAPPER_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8c39ca58a726dd7d7438c872cf8f0cdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DRIVER_ENABLE_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5ce25643dd2b18025e6ada2b2f773bcc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_rst_gpio</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a88cdd37ab80a0e197d32dff99108b32e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_drv_en_gpio</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a69c3b56889a2a06c419d9b581ef6ced7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_faultn_gpio</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3ef2ed9b127b43bfe097db5a19c02d64</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; EspGpio &gt;</type>
      <name>g_wake_gpio</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a905e86e9c557f394cc07081ae22da787</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static BaseSpi *</type>
      <name>g_spi_device</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a425dddf639ccb19b44f7657fe7045389</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Tmc9660Handler &gt;</type>
      <name>g_handler</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6af30925d80ff889d82f4c4cc93d5c5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>g_hw_present</name>
      <anchorfile>tmc9660__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a41c7713ad13517741b17ec422ca5b273</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ws2812_handler_comprehensive_test.cpp</name>
    <path>examples/esp32/main/handler_tests/</path>
    <filename>ws2812__handler__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="Ws2812Handler_8h" name="Ws2812Handler.h" local="yes" import="no" module="no" objc="no">handlers/ws2812/Ws2812Handler.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>create_handler</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad7178bfd7bed244bfbb8045f77e478bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_construction</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac5de30a9290d751035ea1495fef9cd6d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_is_initialized</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1cb3b24c961b118f52be141f4f6c8e3b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_pixel</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a65a5274d52fb5c6299cd3ebe5c6536f7</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_all_pixels</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa2475ae2149fd5a775c3ca401c9f2359</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_show</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7a1578baf97edfb56443c3275a4cc8b9</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_clear</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae283ab37194ca25012e620e7c76e5631</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_pixel_out_of_range</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa0af5593dbdd9d80fd0f1f6eea942d5c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_color_sequence</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a729e1054a9c79312a9be4fdb2dd83e2c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_brightness</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a73b6cfbc6286b7f0055c760309048ac9</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_brightness_range</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2a25814ec633b3839dacb859a535d5ac</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_effect_rainbow</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a9d4c7e34b6470c9fb04d21f205875c05</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_tick</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ada61bd0022e34ae43e4da2b67cf69a4e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_step</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a09960f9f457dce51c427ac7cc2c9fc46</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_strip</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac465911477d50831e37192dfcd24d574</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_animator</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abc2e7813383bb95bbdf9e64b10e25ebf</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_get_num_leds</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a91df361ae268ee8fca4547214b2eadbc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_dump_diagnostics</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae9aa597545f9f6529d499cc52fd62832</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_operations_before_init</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac83f97b7754666b5e7733941b37c5630</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_deinitialize</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a474bb530f591331a31711724a2e45a54</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONSTRUCTION_TESTS</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a67ae550a9bdc50865266b9277403d095</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INITIALIZATION_TESTS</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab7772e69feb610f76ed793f35074a592</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_PIXEL_CONTROL_TESTS</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a277390543e74e46c54ad772bfc21c15c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_BRIGHTNESS_TESTS</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a28044d39e39f85e3d8ec0990eab7d9be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ANIMATION_TESTS</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1ed5cac59b1eba827380e432f0e2db1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DIRECT_ACCESS_TESTS</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a45e12ee17b2e9a3c055a158b480e08f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DIAGNOSTICS_TESTS</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2e723b1283d9cf5fb62f391f378447d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ERROR_HANDLING_TESTS</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab1eea92e5b15a7f4cf1f02166c6851c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::unique_ptr&lt; Ws2812Handler &gt;</type>
      <name>g_handler</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae04d5f55ff533f60012172cbe9d3f49d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>g_hw_present</name>
      <anchorfile>ws2812__handler__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a41c7713ad13517741b17ec422ca5b273</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>full_system_integration_test.cpp</name>
    <path>examples/esp32/main/integration_tests/</path>
    <filename>full__system__integration__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="esp32__bus__setup_8hpp" name="esp32_bus_setup.hpp" local="yes" import="no" module="no" objc="no">esp32_bus_setup.hpp</includes>
    <includes id="esp32__test__config_8hpp" name="esp32_test_config.hpp" local="yes" import="no" module="no" objc="no">esp32_test_config.hpp</includes>
    <includes id="As5047uHandler_8h" name="As5047uHandler.h" local="yes" import="no" module="no" objc="no">handlers/as5047u/As5047uHandler.h</includes>
    <includes id="Bno08xHandler_8h" name="Bno08xHandler.h" local="yes" import="no" module="no" objc="no">handlers/bno08x/Bno08xHandler.h</includes>
    <includes id="Pca9685Handler_8h" name="Pca9685Handler.h" local="yes" import="no" module="no" objc="no">handlers/pca9685/Pca9685Handler.h</includes>
    <includes id="Pcal95555Handler_8h" name="Pcal95555Handler.h" local="yes" import="no" module="no" objc="no">handlers/pcal95555/Pcal95555Handler.h</includes>
    <includes id="NtcTemperatureHandler_8h" name="NtcTemperatureHandler.h" local="yes" import="no" module="no" objc="no">handlers/ntc/NtcTemperatureHandler.h</includes>
    <includes id="Tmc9660Handler_8h" name="Tmc9660Handler.h" local="yes" import="no" module="no" objc="no">handlers/tmc9660/Tmc9660Handler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
    <class kind="struct">HandlerTaskArgs</class>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_i2c_bus_scan</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a6a25aa0fccf0625f8e3cae773904ab38</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_i2c_multi_device_init</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a5a65d07819438b3ffa159258ea42a9ea</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_logger_singleton_across_handlers</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a279ac169ccc62ae95a142d81c10cdebe</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>i2c_handler_task</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a0ffcd10e1f0034357e4d75e4e19504be</anchor>
      <arglist>(void *arg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_concurrent_i2c_access</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>ab02326456d9f2a9466cae859c06f9f1a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_mutex_protected_handler</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>ada81d900b56a67770ac7e61fc64ac706</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_BUS_SCAN_TESTS</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a9012d37d93a390cc6e313df3dc2bce4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_I2C_MULTI_DEVICE_TESTS</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a349753e330c06aade63e4427785c0382</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_LOGGER_INTEGRATION_TESTS</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a9d7c18aba973dcff7eca48d3831a442e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONCURRENT_HANDLER_TESTS</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>aad1ecec7fb1bc8f955b30cb7ce42a120</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_RTOS_INTEGRATION_TESTS</name>
      <anchorfile>full__system__integration__test_8cpp.html</anchorfile>
      <anchor>a5c014c5a50a7846877360ccd67987d0d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>TestFramework.h</name>
    <path>examples/esp32/main/</path>
    <filename>TestFramework_8h.html</filename>
    <class kind="struct">TestResults</class>
    <class kind="struct">TestTaskContext</class>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST_1</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a0a4ef26bff61f2a0fd6a44aacf08c00d</anchor>
      <arglist>(test_func)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST_2</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a2ed54564762f7490f6e746c6e78faaf5</anchor>
      <arglist>(test_name, test_func)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST_GET_MACRO</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a6285c3e06d157a6fe57e3900c5be8ed8</anchor>
      <arglist>(_1, _2, NAME,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a61137c5e3d198031f4040cc96c78528a</anchor>
      <arglist>(...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST_IN_TASK</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a214e38e25939e45e31ae73a073fca47f</anchor>
      <arglist>(name, func, stack_size_bytes, priority)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST_SECTION_IF_ENABLED</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a742c0e25111e9ee95def05464cab957b</anchor>
      <arglist>(define_name, section_name,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_SINGLE_TEST_IF_ENABLED</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a7bc8e45ec7ef683e0b1692c05328b784</anchor>
      <arglist>(define_name, test_name, test_func, stack_size, priority)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST_GROUP_IF_ENABLED</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a069d023efb0dd03f8c2304a56b4a9564</anchor>
      <arglist>(define_name, section_name,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST_SECTION_IF_ENABLED_WITH_PROGRESS</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a75c45174f9dde97eb43339fcd0ac79c8</anchor>
      <arglist>(define_name, section_name, progress_func,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST_SECTION_IF_ENABLED_AUTO_PROGRESS</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a42013656a31f033788afe39c84e96bec</anchor>
      <arglist>(define_name, section_name,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RUN_TEST_SECTION_IF_ENABLED_WITH_PATTERN</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>ab72cc2ae0fb724218e81479185ab38c6</anchor>
      <arglist>(define_name, section_name, blink_count,...)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>init_test_progress_indicator</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a3967179ac699a5b02ef9787b549409fe</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>flip_test_progress_indicator</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a7bb80345e5681c19686df292fa897daf</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cleanup_test_progress_indicator</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a5c86314094f10701c597cf07a95a4bae</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>output_section_indicator</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a0b263a245b65a247ab95a28992b8cc4c</anchor>
      <arglist>(uint8_t blink_count=5) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ensure_gpio14_initialized</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a57a48f91a141d98e45f82937581e1a97</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test_task_trampoline</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>ae0c6ad85cd439fc16572fe4b495e5479</anchor>
      <arglist>(void *param)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print_test_summary</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>ab7e4c076755ecdf2ad6212cb8f6578ab</anchor>
      <arglist>(const TestResults &amp;test_results, const char *test_suite_name, const char *tag) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print_test_section_status</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a200bb35a3a48c625b9cb8d2511d6bd87</anchor>
      <arglist>(const char *tag, const char *test_suite_name) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print_test_section_header</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a5c46ab8dc59b071f0843bc287145d5f1</anchor>
      <arglist>(const char *tag, const char *section_name, bool enabled=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print_test_section_footer</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a5fe4d7658c49a1f3999cd09c0db952c0</anchor>
      <arglist>(const char *tag, const char *section_name, bool enabled=true) noexcept</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>g_test_progress_initialized</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a9bf612ec5a7eaad74cbd8fce98033f70</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>g_test_progress_state</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a09ea58b300726075945c5f209f7d1aeb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr gpio_num_t</type>
      <name>TEST_PROGRESS_PIN</name>
      <anchorfile>TestFramework_8h.html</anchorfile>
      <anchor>a0897f1535fcbb73063a032f07550b12e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>canopen_utils_comprehensive_test.cpp</name>
    <path>examples/esp32/main/utils_tests/</path>
    <filename>canopen__utils__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_can_frame_default</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2c98085b98b167dadb5b05090c3a32f0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_can_frame_data</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abd2eb1e5223777158fe12e1e6037ab00</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sdo_download_frame</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7c1e2aa52b7a25cadbff224d8acc79bd</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_nmt_commands</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa62f517842c51a8999b9f0482c275c02</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CAN_FRAME_TESTS</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a922dfecea8cec7d74a08c8791c069653</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_SDO_TESTS</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a875b4034eb7db2927fde79858e9609b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_NMT_TESTS</name>
      <anchorfile>canopen__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6ad2fa16e1e7a830e5ff3c62a9b4467e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>general_utils_comprehensive_test.cpp</name>
    <path>examples/esp32/main/utils_tests/</path>
    <filename>general__utils__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <class kind="struct">sm_test::Owner</class>
    <namespace>sm_test</namespace>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_circular_buffer_basic</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aafcef82a0e68a723e26435341873f876</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_circular_buffer_overflow</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abd3c580181f051685a9cddbc485a5fbd</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_ring_buffer_basic</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a2cd4954d9b6b37f42ac6380c7ddd750e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_averaging_filter</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1d0dc30e375c19898331999a0322eaf9</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_averaging_filter_partial</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a22f3e317a5261b210e082417a99b2835</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_crc_functions</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5d69708f3e8b09be7f61201d5799a42d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_action_timer</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a32d55bb08563856f410c7c653a262e24</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_software_version</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a0000f541150318542252cbea9b5080a2</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sm_finalize_requires_full_registration</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a19c7a79e3b4cbe763eda9bfff5d8b07f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sm_entry_loop_runs_on_first_tick</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a27fd3b03be28dd31ff379d9a8299b242</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sm_owner_request_transition_runs_exit_then_entry</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a034bf5258503be263649c2b35147e651</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sm_illegal_transition_rejected</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a75efa4d87abe9d2442589fc40ecfdd47</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sm_external_intent_drained_on_update</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a41d00c3ec700a18d505dc86f04196bf5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sm_entry_failure_retries_next_tick</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae5fff781f466e12c266bce721a434db3</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sm_exit_failure_keeps_state_with_intent_pending</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af91b6159f73a8f937cf1f30d4ae7206c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_sm_max_dwell_watchdog_fires_once_per_state</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab656d072395dc9629b54a3545addcca4</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CIRCULAR_BUFFER_TESTS</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5ba5fbf336cf6f68f57f8e00fe75a1d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_RING_BUFFER_TESTS</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae29b50cf2e93306eaf1267df16a266cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_AVERAGING_FILTER_TESTS</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae671a48495a1fa48901e40bc3f7418ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CRC_TESTS</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a06ae68872c355a0545d92dd9501bb4be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_TIMER_TESTS</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a47fc2c0c2aa5a48d172bb6cf295f724d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_VERSION_TESTS</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a777c850679c895b2649737d6218a93c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_STATE_MACHINE_TESTS</name>
      <anchorfile>general__utils__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a157244eae831ac5221412e68d474caf8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>logger_comprehensive_test.cpp</name>
    <path>examples/esp32/main/utils_tests/</path>
    <filename>logger__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_singleton_access</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1cf5761a09d135571a5b64c07c4fd94f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_initialize</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ac314755c1abd2475ff52f823949f3122</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_reinitialize</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a3d0de398e5efe6c520def499e4ef8148</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_log_all_levels</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a0ef301d3e05cd1ad419b3960e1cf8e64</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_set_log_level</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>abb683f84ebb2997730a8f0a9971ab81e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_formatted_output</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa3eec9edd2cb6d63a4a0e25e755ec9b7</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_ascii_art</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a86a1ffe58d3d4a46e5aa2c86d864e098</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_enable_disable_features</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad415f93657d99418910b1e209291efdd</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_concurrent_logging</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a121a66db63fc436e57c4e23cabd3a6a8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_SINGLETON_TESTS</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a73dbc67003d217960638d86afb905f69</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_INIT_TESTS</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ab5b0c2bb9ce717836b0e59a1e21b1d87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_LOG_LEVEL_TESTS</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa71717d38673c8c6f0a710c66ffcd3f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_FORMATTED_OUTPUT_TESTS</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a9b574642318732afa08d5a9bde7097b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_ASCII_ART_TESTS</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a08d24e6de7a2c861320057208e6527fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_CONFIG_TESTS</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a54625a8336bef3cfd1209481343c5654</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_THREAD_SAFETY_TESTS</name>
      <anchorfile>logger__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a37c454ffbe978dc87a61bb2638fa4f5f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>rtos_wrap_comprehensive_test.cpp</name>
    <path>examples/esp32/main/utils_tests/</path>
    <filename>rtos__wrap__comprehensive__test_8cpp.html</filename>
    <includes id="TestFramework_8h" name="TestFramework.h" local="yes" import="no" module="no" objc="no">TestFramework.h</includes>
    <class kind="class">TestThread</class>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_mutex_create</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a6837ed30db223e098f0582c3293b5b08</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_mutex_lock_unlock</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>af92fc7dcfd97fec84aa8eabd35d219d1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_mutex_contention</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a8c28c1672b5db30f5ddb5130f51bfc2f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_lock_guard_scoped</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a0cad4a323900a254f70629fc4cf67a13</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>timer_callback</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>acb6c6f5283e6ee3f412844244a4bcce1</anchor>
      <arglist>(uint32_t arg)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_periodic_timer</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a33699a69fbdc140bcba863f2c3e8ed45</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_base_thread</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a54c79eeaab880238a8bfc12cb9a81efe</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_os_queue</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a57849262fcbeffefbaf4ff2fd03e0da4</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_os_queue_full</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a7659cd90d8187cce7e493081d04f94c5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_event_flags</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ae4f8a05065889b733246e73add616434</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_semaphore</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a313bf4c7ccc37386be3d53191255aab4</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>test_os_delay</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a758da32a1dd5dbb8d4d379d0bf8fb615</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>app_main</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a630544a7f0a2cc40d8a7fefab7e2fe70</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static TestResults</type>
      <name>g_test_results</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aaff89257c54366921eebb8ec1348b1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_MUTEX_TESTS</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a39df2ed1bd32df2d8910cbb8c7e89a27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_LOCK_GUARD_TESTS</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>adef08b43a3f45feb45adcc26b23b09c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_PERIODIC_TIMER_TESTS</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>aa6bd0d7177c0f426f20cb9e607d57b29</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_THREAD_TESTS</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a9910aa5d9f68d8b46929abd23b5d2392</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_QUEUE_TESTS</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>ad4aa2fdfcc6db354d6f60a815ef825a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_EVENT_FLAGS_TESTS</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5d86f1149bc172936333f50642da6793</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_SEMAPHORE_TESTS</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1317903822d4649bf259e38749418e95</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr bool</type>
      <name>ENABLE_DELAY_TESTS</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a5a44b6d9ee7c71c014674e5bbec0b8e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::atomic&lt; int &gt;</type>
      <name>g_timer_count</name>
      <anchorfile>rtos__wrap__comprehensive__test_8cpp.html</anchorfile>
      <anchor>a1ef3f25205f06645880035cb0fbe17b3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Ads7952Handler.cpp</name>
    <path>handlers/ads7952/</path>
    <filename>Ads7952Handler_8cpp.html</filename>
    <includes id="Ads7952Handler_8h" name="Ads7952Handler.h" local="yes" import="no" module="no" objc="no">Ads7952Handler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>LOG_BANNER</name>
      <anchorfile>Ads7952Handler_8cpp.html</anchorfile>
      <anchor>a3f8ce72359938b8bedeba7a51bbba110</anchor>
      <arglist>(tag, art, format)</arglist>
    </member>
    <member kind="function">
      <type>std::unique_ptr&lt; Ads7952Handler &gt;</type>
      <name>CreateAds7952Handler</name>
      <anchorfile>Ads7952Handler_8cpp.html</anchorfile>
      <anchor>a7a60835cd8ffec8fe03075e3567fec7b</anchor>
      <arglist>(BaseSpi &amp;spi_interface, const Ads7952HandlerConfig &amp;config) noexcept</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr const char *</type>
      <name>TAG</name>
      <anchorfile>Ads7952Handler_8cpp.html</anchorfile>
      <anchor>ae7d9c64644585526560e9e6824fd84dc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Ads7952Handler.h</name>
    <path>handlers/ads7952/</path>
    <filename>Ads7952Handler_8h.html</filename>
    <class kind="class">Ads7952SpiAdapter</class>
    <class kind="struct">Ads7952HandlerConfig</class>
    <class kind="struct">Ads7952Diagnostics</class>
    <class kind="class">Ads7952Handler</class>
    <member kind="function">
      <type>Ads7952HandlerConfig</type>
      <name>GetDefaultAds7952Config</name>
      <anchorfile>Ads7952Handler_8h.html</anchorfile>
      <anchor>ad16c327738b31a420b2e42c5a7acb663</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::unique_ptr&lt; Ads7952Handler &gt;</type>
      <name>CreateAds7952Handler</name>
      <anchorfile>Ads7952Handler_8h.html</anchorfile>
      <anchor>a0422e1d432065e66df70e927a30b49e4</anchor>
      <arglist>(BaseSpi &amp;spi_interface, const Ads7952HandlerConfig &amp;config=GetDefaultAds7952Config()) noexcept</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Ads9324Handler.cpp</name>
    <path>handlers/ads9324/</path>
    <filename>Ads9324Handler_8cpp.html</filename>
    <includes id="Ads9324Handler_8h" name="Ads9324Handler.h" local="yes" import="no" module="no" objc="no">Ads9324Handler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
    <member kind="function">
      <type>std::unique_ptr&lt; Ads9324Handler &gt;</type>
      <name>CreateAds9324Handler</name>
      <anchorfile>Ads9324Handler_8cpp.html</anchorfile>
      <anchor>af2745be54e37b7f1d0d9835dd9c40747</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio *convst, BaseGpio *drdy, const Ads9324HandlerConfig &amp;config) noexcept</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr const char *</type>
      <name>TAG</name>
      <anchorfile>Ads9324Handler_8cpp.html</anchorfile>
      <anchor>ae7d9c64644585526560e9e6824fd84dc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Ads9324Handler.h</name>
    <path>handlers/ads9324/</path>
    <filename>Ads9324Handler_8h.html</filename>
    <class kind="class">Ads9324SpiHostAdapter</class>
    <class kind="struct">Ads9324HandlerConfig</class>
    <class kind="struct">Ads9324Diagnostics</class>
    <class kind="class">Ads9324Handler</class>
    <member kind="function">
      <type>Ads9324HandlerConfig</type>
      <name>GetDefaultAds9324Config</name>
      <anchorfile>Ads9324Handler_8h.html</anchorfile>
      <anchor>a799c213ad1fbbe826d5ec2f433c8f5e5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::unique_ptr&lt; Ads9324Handler &gt;</type>
      <name>CreateAds9324Handler</name>
      <anchorfile>Ads9324Handler_8h.html</anchorfile>
      <anchor>ac2cc17b45979fd2189b6dd46d6a8acd8</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio *convst, BaseGpio *drdy=nullptr, const Ads9324HandlerConfig &amp;config=GetDefaultAds9324Config()) noexcept</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>AlicatBasis2Handler.cpp</name>
    <path>handlers/alicat_basis2/</path>
    <filename>AlicatBasis2Handler_8cpp.html</filename>
    <includes id="AlicatBasis2Handler_8h" name="AlicatBasis2Handler.h" local="yes" import="no" module="no" objc="no">AlicatBasis2Handler.h</includes>
    <namespace>anonymous_namespace{AlicatBasis2Handler.cpp}</namespace>
    <member kind="define">
      <type>#define</type>
      <name>BASIS2_FORWARD_VOID</name>
      <anchorfile>AlicatBasis2Handler_8cpp.html</anchorfile>
      <anchor>a7b42b5406b54e3f42b59c2445cd7af03</anchor>
      <arglist>(method,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BASIS2_FORWARD_T</name>
      <anchorfile>AlicatBasis2Handler_8cpp.html</anchorfile>
      <anchor>acd10a16d6e5acbfd6b5acff40fb0dee8</anchor>
      <arglist>(T, method,...)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>AlicatBasis2Handler.h</name>
    <path>handlers/alicat_basis2/</path>
    <filename>AlicatBasis2Handler_8h.html</filename>
    <class kind="class">HalUartAlicatBasis2Comm</class>
    <class kind="struct">AlicatBasis2HandlerConfig</class>
    <class kind="class">AlicatBasis2Handler</class>
    <class kind="struct">AlicatBasis2Handler::DiscoveredDevice</class>
    <member kind="function">
      <type>constexpr AlicatBasis2HandlerConfig</type>
      <name>MakeDefaultBasis2Config</name>
      <anchorfile>AlicatBasis2Handler_8h.html</anchorfile>
      <anchor>a77ca2d9f1d7cbce3c3a7d1350579e7b2</anchor>
      <arglist>(std::uint8_t addr, std::uint8_t idx) noexcept</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>As5047uHandler.cpp</name>
    <path>handlers/as5047u/</path>
    <filename>As5047uHandler_8cpp.html</filename>
    <includes id="As5047uHandler_8h" name="As5047uHandler.h" local="yes" import="no" module="no" objc="no">As5047uHandler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
    <member kind="function">
      <type>std::unique_ptr&lt; As5047uHandler &gt;</type>
      <name>CreateAs5047uHandler</name>
      <anchorfile>As5047uHandler_8cpp.html</anchorfile>
      <anchor>aeadd110d3ac98bd1fd029a8f2f6d5d99</anchor>
      <arglist>(BaseSpi &amp;spi_interface, const As5047uConfig &amp;config) noexcept</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>As5047uHandler.h</name>
    <path>handlers/as5047u/</path>
    <filename>As5047uHandler_8h.html</filename>
    <class kind="class">As5047uSpiAdapter</class>
    <class kind="struct">As5047uMeasurement</class>
    <class kind="struct">As5047uDiagnostics</class>
    <class kind="struct">As5047uConfig</class>
    <class kind="class">As5047uHandler</class>
    <member kind="function">
      <type>std::unique_ptr&lt; As5047uHandler &gt;</type>
      <name>CreateAs5047uHandler</name>
      <anchorfile>As5047uHandler_8h.html</anchorfile>
      <anchor>af8c2baad0f8b9cede2fdb8dadc584c89</anchor>
      <arglist>(BaseSpi &amp;spi_interface, const As5047uConfig &amp;config=As5047uHandler::GetDefaultConfig()) noexcept</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>At25040Handler.cpp</name>
    <path>handlers/at25040/</path>
    <filename>At25040Handler_8cpp.html</filename>
    <includes id="At25040Handler_8h" name="At25040Handler.h" local="yes" import="no" module="no" objc="no">At25040Handler.h</includes>
  </compound>
  <compound kind="file">
    <name>At25040Handler.h</name>
    <path>handlers/at25040/</path>
    <filename>At25040Handler_8h.html</filename>
    <class kind="class">At25040SpiAdapter</class>
    <class kind="class">At25040Handler</class>
  </compound>
  <compound kind="file">
    <name>Bmp581Handler.cpp</name>
    <path>handlers/bmp581/</path>
    <filename>Bmp581Handler_8cpp.html</filename>
    <includes id="Bmp581Handler_8h" name="Bmp581Handler.h" local="yes" import="no" module="no" objc="no">Bmp581Handler.h</includes>
    <namespace>anonymous_namespace{Bmp581Handler.cpp}</namespace>
  </compound>
  <compound kind="file">
    <name>Bmp581Handler.h</name>
    <path>handlers/bmp581/</path>
    <filename>Bmp581Handler_8h.html</filename>
    <class kind="class">HalI2cBmp581Comm</class>
    <class kind="struct">Bmp581HandlerConfig</class>
    <class kind="class">Bmp581Handler</class>
    <class kind="struct">Bmp581Handler::Counters</class>
  </compound>
  <compound kind="file">
    <name>Bno08xHandler.cpp</name>
    <path>handlers/bno08x/</path>
    <filename>Bno08xHandler_8cpp.html</filename>
    <includes id="Bno08xHandler_8h" name="Bno08xHandler.h" local="yes" import="no" module="no" objc="no">Bno08xHandler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
  </compound>
  <compound kind="file">
    <name>Bno08xHandler.h</name>
    <path>handlers/bno08x/</path>
    <filename>Bno08xHandler_8h.html</filename>
    <class kind="class">HalI2cBno08xComm</class>
    <class kind="class">HalSpiBno08xComm</class>
    <class kind="class">IBno08xDriverOps</class>
    <class kind="class">Bno08xDriverImpl</class>
    <class kind="struct">Bno08xVector3</class>
    <class kind="struct">Bno08xQuaternion</class>
    <class kind="struct">Bno08xEulerAngles</class>
    <class kind="struct">Bno08xImuData</class>
    <class kind="struct">Bno08xActivityData</class>
    <class kind="struct">Bno08xCalibrationStatus</class>
    <class kind="struct">Bno08xConfig</class>
    <class kind="class">Bno08xHandler</class>
    <member kind="enumeration">
      <type></type>
      <name>Bno08xError</name>
      <anchorfile>Bno08xHandler_8h.html</anchorfile>
      <anchor>a93eba6be08e06b6dcc1d9800dcc4a832</anchor>
      <arglist></arglist>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832ad0749aaba8b833466dfcbb0428e4f89c">SUCCESS</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832a02de9649e258d1dd94056e676061e969">NOT_INITIALIZED</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832afdd2c3274e42cbf32d77a90e764f1faf">INITIALIZATION_FAILED</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832afeb1a30cb74c35aaf4ea067736109e30">INVALID_PARAMETER</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832a4f66ca42362b83f362b57f7f0aec26d2">COMMUNICATION_FAILED</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832aefa317621a7a5d359894d432ec948b31">SENSOR_NOT_RESPONDING</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832a9cb6ce703e04781843950948b1f80f0f">CALIBRATION_FAILED</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832aac29fd5ddf8878cb5d667746a83acbff">FIRMWARE_UPDATE_FAILED</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832a070a0fb40f6c308ab544b227660aadff">TIMEOUT</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832a13d0255d66b368703af734c8ca5ae227">MUTEX_LOCK_FAILED</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832a2eca914413e779cbfd9655005a0128c8">INVALID_INTERFACE</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832adefa7c21c79061d81f201f471ac484b9">SENSOR_NOT_ENABLED</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832ae86293582656c525e100c3c7628f607f">DATA_NOT_AVAILABLE</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832a89fe56ed105248667fe799031276ab23">HARDWARE_ERROR</enumvalue>
      <enumvalue file="Bno08xHandler_8h.html" anchor="a93eba6be08e06b6dcc1d9800dcc4a832a5a2ff7a2f2c3b40268e57995408f8e9d">UNSUPPORTED_OPERATION</enumvalue>
    </member>
    <member kind="function">
      <type>constexpr const char *</type>
      <name>Bno08xErrorToString</name>
      <anchorfile>Bno08xHandler_8h.html</anchorfile>
      <anchor>a929bff75c00c4ce529177b0640cdf58c</anchor>
      <arglist>(Bno08xError error) noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::unique_ptr&lt; Bno08xHandler &gt;</type>
      <name>CreateBno08xHandlerI2c</name>
      <anchorfile>Bno08xHandler_8h.html</anchorfile>
      <anchor>a00118ff1c99ae958c092ed6d21b59135</anchor>
      <arglist>(BaseI2c &amp;i2c_device, const Bno08xConfig &amp;config=Bno08xConfig{}, BaseGpio *reset_gpio=nullptr, BaseGpio *int_gpio=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::unique_ptr&lt; Bno08xHandler &gt;</type>
      <name>CreateBno08xHandlerSpi</name>
      <anchorfile>Bno08xHandler_8h.html</anchorfile>
      <anchor>a1bd813a8294732698f36232c4b9ed0af</anchor>
      <arglist>(BaseSpi &amp;spi_device, const Bno08xConfig &amp;config=Bno08xConfig{}, BaseGpio *reset_gpio=nullptr, BaseGpio *int_gpio=nullptr, BaseGpio *wake_gpio=nullptr) noexcept</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>CanOpenBaseCanLink.hpp</name>
    <path>handlers/common/canopen/</path>
    <filename>CanOpenBaseCanLink_8hpp.html</filename>
    <includes id="HfUtilsCanOpenTransport_8hpp" name="HfUtilsCanOpenTransport.hpp" local="yes" import="no" module="no" objc="no">HfUtilsCanOpenTransport.hpp</includes>
    <class kind="class">CanOpenBaseCanLink</class>
  </compound>
  <compound kind="file">
    <name>CO_driver_target.h</name>
    <path>handlers/common/canopen/</path>
    <filename>CO__driver__target_8h.html</filename>
    <class kind="struct">CO_CANrxMsg_t</class>
    <class kind="struct">CO_CANrx_t</class>
    <class kind="struct">CO_CANtx_t</class>
    <class kind="struct">CO_CANmodule_t</class>
    <class kind="struct">CO_storage_entry_t</class>
    <member kind="define">
      <type>#define</type>
      <name>CO_LITTLE_ENDIAN</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>aed3e1bffaf912485092fc20193705f35</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_SWAP_16</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a1717fcaabbe2cd6cd9fc0bc0cb917a6c</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_SWAP_32</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>adf87da54942e3a0ff159688c5e0e267b</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_SWAP_64</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>aec0fc209357883f42d66cd2cdaa7236f</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_CANrxMsg_readIdent</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a572468541fe0a18ec027c45652a8740c</anchor>
      <arglist>(msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_CANrxMsg_readDLC</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a9d981bd3dcfbcecb9456e374571d9336</anchor>
      <arglist>(msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_CANrxMsg_readData</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>ab0395143e8caa1a835a2aac5c2080736</anchor>
      <arglist>(msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_LOCK_CAN_SEND</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a908c5d3fe5d6f79db4c6d72212f1f020</anchor>
      <arglist>(CAN_MODULE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_UNLOCK_CAN_SEND</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a5758ee7be17d758e3b55e3c9184d3873</anchor>
      <arglist>(CAN_MODULE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_LOCK_EMCY</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a1bfd1de6faa39a23b04ecd1fd5db7dbd</anchor>
      <arglist>(CAN_MODULE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_UNLOCK_EMCY</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a1d7a22feece8448f985a608f83ea67a6</anchor>
      <arglist>(CAN_MODULE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_LOCK_OD</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>ae91e416633b4df1800e97a2bc02bbeb4</anchor>
      <arglist>(CAN_MODULE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_UNLOCK_OD</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a37d2c1496e5304faf5f0670ef33a4704</anchor>
      <arglist>(CAN_MODULE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_MemoryBarrier</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a839e27ac4c9fc82f717d48292995bd4c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_FLAG_READ</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a577a6ebcf246087f084c75d9ae25eeb7</anchor>
      <arglist>(rxNew)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_FLAG_SET</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>ac54b5e4f680aa8b0177f0df5d5be2e88</anchor>
      <arglist>(rxNew)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CO_FLAG_CLEAR</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a044da4253aeed15c3e0bb7fce13664af</anchor>
      <arglist>(rxNew)</arglist>
    </member>
    <member kind="typedef">
      <type>uint_fast8_t</type>
      <name>bool_t</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a490ee355f57f2815cb1aa4626cf525f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>float</type>
      <name>float32_t</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a4611b605e45ab401f02cab15c5e38715</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>double</type>
      <name>float64_t</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>ac55f3ae81b5bc9053760baacf57e47f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hf_co_driver_lock</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a2298662a35e5d967dfa87873e50325b6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hf_co_driver_unlock</name>
      <anchorfile>CO__driver__target_8h.html</anchorfile>
      <anchor>a484cc4d2849afb59ec99bc4320abd28f</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>CO_driver_vortex.c</name>
    <path>handlers/common/canopen/</path>
    <filename>CO__driver__vortex_8c.html</filename>
    <includes id="hf__co__driver__iface_8h" name="hf_co_driver_iface.h" local="yes" import="no" module="no" objc="no">hf_co_driver_iface.h</includes>
    <member kind="function">
      <type>void</type>
      <name>hf_co_driver_lock</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a2298662a35e5d967dfa87873e50325b6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hf_co_driver_unlock</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a484cc4d2849afb59ec99bc4320abd28f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hf_co_driver_register_tx</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>ad78d9e9a3a8960a02466ab2b286aae77</anchor>
      <arglist>(int(*tx_fn)(uint32_t std_id, uint8_t dlc, const uint8_t *data, void *ctx), void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CO_CANsetConfigurationMode</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a88ef52baae169a80e4c2f2cb93b33747</anchor>
      <arglist>(void *CANptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CO_CANsetNormalMode</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>ad654edfa651bf7b68263913786697200</anchor>
      <arglist>(CO_CANmodule_t *CANmodule)</arglist>
    </member>
    <member kind="function">
      <type>CO_ReturnError_t</type>
      <name>CO_CANmodule_init</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a3a1131813110529494cee5e27c0bf28d</anchor>
      <arglist>(CO_CANmodule_t *CANmodule, void *CANptr, CO_CANrx_t rxArray[], uint16_t rxSize, CO_CANtx_t txArray[], uint16_t txSize, uint16_t CANbitRate)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CO_CANmodule_disable</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>ac6f60f9da27dda0c9b3950c4e96bd687</anchor>
      <arglist>(CO_CANmodule_t *CANmodule)</arglist>
    </member>
    <member kind="function">
      <type>CO_ReturnError_t</type>
      <name>CO_CANrxBufferInit</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a25ee22cd2e3a2f3bb49990e8bc3076b0</anchor>
      <arglist>(CO_CANmodule_t *CANmodule, uint16_t index, uint16_t ident, uint16_t mask, bool_t rtr, void *object, void(*CANrx_callback)(void *object, void *message))</arglist>
    </member>
    <member kind="function">
      <type>CO_CANtx_t *</type>
      <name>CO_CANtxBufferInit</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a8be0e9c60676aaf1f7da6403cff959bb</anchor>
      <arglist>(CO_CANmodule_t *CANmodule, uint16_t index, uint16_t ident, bool_t rtr, uint8_t noOfBytes, bool_t syncFlag)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>decode_tx_ident</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a5ff78dd77aed030750bac47b040730c0</anchor>
      <arglist>(const CO_CANtx_t *buffer, uint16_t *out_id, uint8_t *out_dlc, bool_t *out_rtr)</arglist>
    </member>
    <member kind="function">
      <type>CO_ReturnError_t</type>
      <name>CO_CANsend</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a4664a9f5d547cb0605a9e929fb079f2e</anchor>
      <arglist>(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CO_CANclearPendingSyncPDOs</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>abbeac85cbf513162b11fc3d0717b0753</anchor>
      <arglist>(CO_CANmodule_t *CANmodule)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CO_CANmodule_process</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a066c6742f75b2daac585735ffd6c9928</anchor>
      <arglist>(CO_CANmodule_t *CANmodule)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>dispatch_rx</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a834549a0cd6a8a115e23f2f1be38458e</anchor>
      <arglist>(CO_CANmodule_t *CANmodule, CO_CANrxMsg_t *rcvMsg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hf_co_driver_process_rx</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a11342ac2a7122e6366facafba7623df4</anchor>
      <arglist>(void *CANmodule_void, uint16_t std_id, uint8_t dlc, const uint8_t *data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>CO_CANinterrupt</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>ae5e1af799bd59ed0a39495241ce6f5e3</anchor>
      <arglist>(CO_CANmodule_t *CANmodule)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static portMUX_TYPE</type>
      <name>s_co_mux</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a76bcb8c43cf9040ac966e4c52fd79327</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int(*</type>
      <name>s_tx_fn</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>a6dca7609c7ab728edc9c202e79cc0349</anchor>
      <arglist>)(uint32_t std_id, uint8_t dlc, const uint8_t *data, void *ctx)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static void *</type>
      <name>s_tx_ctx</name>
      <anchorfile>CO__driver__vortex_8c.html</anchorfile>
      <anchor>affd0f1c5673f00f5bd598bbbe76aed50</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>hf_co_driver_iface.h</name>
    <path>handlers/common/canopen/</path>
    <filename>hf__co__driver__iface_8h.html</filename>
    <member kind="function">
      <type>void</type>
      <name>hf_co_driver_register_tx</name>
      <anchorfile>hf__co__driver__iface_8h.html</anchorfile>
      <anchor>ad78d9e9a3a8960a02466ab2b286aae77</anchor>
      <arglist>(int(*tx_fn)(uint32_t std_id, uint8_t dlc, const uint8_t *data, void *ctx), void *ctx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>hf_co_driver_process_rx</name>
      <anchorfile>hf__co__driver__iface_8h.html</anchorfile>
      <anchor>a2f1195054a0601d024b059e8b293b800</anchor>
      <arglist>(void *CANmodule, uint16_t std_id, uint8_t dlc, const uint8_t *data)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>HfUtilsCanOpenBridge.hpp</name>
    <path>handlers/common/canopen/</path>
    <filename>HfUtilsCanOpenBridge_8hpp.html</filename>
    <member kind="function">
      <type>void</type>
      <name>HfUtilsCanFrameToMessage</name>
      <anchorfile>HfUtilsCanOpenBridge_8hpp.html</anchorfile>
      <anchor>a10b9ba06573aa03ca1e8b5f722ba0d37</anchor>
      <arglist>(const CanOpen::CanFrame &amp;c, hf_can_message_t &amp;m) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HfUtilsMessageToCanFrame</name>
      <anchorfile>HfUtilsCanOpenBridge_8hpp.html</anchorfile>
      <anchor>a3c53d9214a389deca928775a8509b5bd</anchor>
      <arglist>(const hf_can_message_t &amp;m, CanOpen::CanFrame &amp;c) noexcept</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>HfUtilsCanOpenTransport.hpp</name>
    <path>handlers/common/canopen/</path>
    <filename>HfUtilsCanOpenTransport_8hpp.html</filename>
    <includes id="HfUtilsCanOpenBridge_8hpp" name="HfUtilsCanOpenBridge.hpp" local="yes" import="no" module="no" objc="no">HfUtilsCanOpenBridge.hpp</includes>
    <class kind="class">HfUtilsCanOpenTransport</class>
  </compound>
  <compound kind="file">
    <name>HandlerCommon.h</name>
    <path>handlers/common/</path>
    <filename>HandlerCommon_8h.html</filename>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">Logger.h</includes>
    <namespace>handler_utils</namespace>
  </compound>
  <compound kind="file">
    <name>PlatformMutexBackend.h</name>
    <path>handlers/common/</path>
    <filename>PlatformMutexBackend_8h.html</filename>
  </compound>
  <compound kind="file">
    <name>DynamixelHandler.cpp</name>
    <path>handlers/dynamixel/</path>
    <filename>DynamixelHandler_8cpp.html</filename>
    <includes id="DynamixelHandler_8h" name="DynamixelHandler.h" local="yes" import="no" module="no" objc="no">DynamixelHandler.h</includes>
  </compound>
  <compound kind="file">
    <name>DynamixelHandler.h</name>
    <path>handlers/dynamixel/</path>
    <filename>DynamixelHandler_8h.html</filename>
    <class kind="class">HalUartDynamixelComm</class>
    <class kind="struct">DynamixelHandlerConfig</class>
    <class kind="class">DynamixelHandler</class>
  </compound>
  <compound kind="file">
    <name>Fdo2Handler.cpp</name>
    <path>handlers/fdo2/</path>
    <filename>Fdo2Handler_8cpp.html</filename>
    <includes id="Fdo2Handler_8h" name="Fdo2Handler.h" local="yes" import="no" module="no" objc="no">Fdo2Handler.h</includes>
  </compound>
  <compound kind="file">
    <name>Fdo2Handler.h</name>
    <path>handlers/fdo2/</path>
    <filename>Fdo2Handler_8h.html</filename>
    <class kind="class">HalUartFdo2Comm</class>
    <class kind="struct">Fdo2HandlerConfig</class>
    <class kind="class">Fdo2Handler</class>
  </compound>
  <compound kind="file">
    <name>Isf15acp4Handler.cpp</name>
    <path>handlers/isf15acp4/</path>
    <filename>Isf15acp4Handler_8cpp.html</filename>
    <includes id="Isf15acp4Handler_8h" name="Isf15acp4Handler.h" local="yes" import="no" module="no" objc="no">Isf15acp4Handler.h</includes>
    <includes id="HandlerCommon_8h" name="HandlerCommon.h" local="yes" import="no" module="no" objc="no">handlers/common/HandlerCommon.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
  </compound>
  <compound kind="file">
    <name>Isf15acp4Handler.h</name>
    <path>handlers/isf15acp4/</path>
    <filename>Isf15acp4Handler_8h.html</filename>
    <class kind="class">HalIsf15acp4SpiAdapter</class>
    <class kind="struct">Isf15acp4HandlerConfig</class>
    <class kind="class">Isf15acp4Handler</class>
  </compound>
  <compound kind="file">
    <name>EspLoggerFactory.cpp</name>
    <path>handlers/logger/factory/</path>
    <filename>EspLoggerFactory_8cpp.html</filename>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">../Logger.h</includes>
  </compound>
  <compound kind="file">
    <name>LogDecorators.h</name>
    <path>handlers/logger/</path>
    <filename>LogDecorators_8h.html</filename>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">Logger.h</includes>
    <namespace>logger_decor</namespace>
  </compound>
  <compound kind="file">
    <name>Logger.cpp</name>
    <path>handlers/logger/</path>
    <filename>Logger_8cpp.html</filename>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">Logger.h</includes>
    <namespace>anonymous_namespace{Logger.cpp}</namespace>
  </compound>
  <compound kind="file">
    <name>Logger.h</name>
    <path>handlers/logger/</path>
    <filename>Logger_8h.html</filename>
    <class kind="struct">LogConfig</class>
    <class kind="struct">AsciiArtFormat</class>
    <class kind="class">Logger</class>
    <class kind="struct">Logger::TagLevel</class>
    <member kind="define">
      <type>#define</type>
      <name>LOG_ERROR</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a53c554c9625bc6ee2727f52091346c17</anchor>
      <arglist>(tag,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_WARN</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a63330e5e329bed8646b5421a8ef25f75</anchor>
      <arglist>(tag,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_INFO</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a4ec0b91afc0eca2e85c60b528628b8cc</anchor>
      <arglist>(tag,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_DEBUG</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a86952bc1b765abd64f032f9f6fce9fe3</anchor>
      <arglist>(tag,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_VERBOSE</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a179efad4f6d6a1e6d899882b8bb7af19</anchor>
      <arglist>(tag,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_ERROR_FORMATTED</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a5afe2155299d50acbbe8edd1a578b0ce</anchor>
      <arglist>(tag, color, style,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_WARN_FORMATTED</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a47c4af298091a89b5dae199d14d020dc</anchor>
      <arglist>(tag, color, style,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_INFO_FORMATTED</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>ae66f739662bd47f7e2db78099b173d15</anchor>
      <arglist>(tag, color, style,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_DEBUG_FORMATTED</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a3b282a96665d912c411a9209e2896746</anchor>
      <arglist>(tag, color, style,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_VERBOSE_FORMATTED</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a396263b7a31e013cf7debbcaa008ebf8</anchor>
      <arglist>(tag, color, style,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_ASCII_ART</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>ab29585be7729d8a20e2da2c3f1793205</anchor>
      <arglist>(tag, art, format)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_BANNER</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a3f8ce72359938b8bedeba7a51bbba110</anchor>
      <arglist>(tag, art, format)</arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>LogStyle</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>aeeb2493a0c1386f85084889cc8bb7f52</anchor>
      <arglist></arglist>
      <enumvalue file="Logger_8h.html" anchor="aeeb2493a0c1386f85084889cc8bb7f52a1e23852820b9154316c7c06e2b7ba051">NORMAL</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="aeeb2493a0c1386f85084889cc8bb7f52a2909dd0e0336f10b6da9735b859a3d19">BOLD</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="aeeb2493a0c1386f85084889cc8bb7f52a25093123b8943308104129e1d762f3b7">ITALIC</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="aeeb2493a0c1386f85084889cc8bb7f52a0086c73c90c261db630b4dc8aea09bcf">UNDERLINE</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="aeeb2493a0c1386f85084889cc8bb7f52aad166989fa2b7da7d238b3e7105c64f8">STRIKETHROUGH</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="aeeb2493a0c1386f85084889cc8bb7f52a2cbaca391380e64473b1302e72a32347">DOUBLE_UNDERLINE</enumvalue>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>LogColor</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>abeb0b5a8d3424ad4a83592f8580b8a2e</anchor>
      <arglist></arglist>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea5b39c8b553c821e7cddc6da64b5bd2ee">DEFAULT</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea08d0012388564e95c3b4a7407cf04965">BLACK</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2eaa2d9547b5d3dd9f05984475f7c926da0">RED</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea9de0e5dd94e861317e74964bed179fa0">GREEN</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea8a568e5f41b7e4da88fe5c4a00aad34e">YELLOW</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea1b3e1ee9bff86431dea6b181365ba65f">BLUE</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2eac634ffea7195608364671ac52ee59a61">MAGENTA</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea344dd8cd533280795b9db82ef0c92749">CYAN</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2eab5bf627e448384cf3a4c35121ca6008d">WHITE</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2eaae7e8e8f5ddf114057b47f5f799347b1">BRIGHT_BLACK</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea01fd5046e2769cae7b662d3dfbb64045">BRIGHT_RED</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea7ca9c0eccf9c3fa7fe85fea8404d50a9">BRIGHT_GREEN</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea2373edc9b8eb3a49710b95c6f7906434">BRIGHT_YELLOW</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ead187a715c27ec1e774325b6715f4da67">BRIGHT_BLUE</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2eacdbf2435d34544195dd18a6b31953951">BRIGHT_MAGENTA</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2eaa74a6a719de5de03a46f89ce260fa4b7">BRIGHT_CYAN</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="abeb0b5a8d3424ad4a83592f8580b8a2ea376520e57593580aa5c7ef8cf5136946">BRIGHT_WHITE</enumvalue>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>LogBackground</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a96718db9ec196b0ccd6f8d9517b45ea1</anchor>
      <arglist></arglist>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a5b39c8b553c821e7cddc6da64b5bd2ee">DEFAULT</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a08d0012388564e95c3b4a7407cf04965">BLACK</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1aa2d9547b5d3dd9f05984475f7c926da0">RED</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a9de0e5dd94e861317e74964bed179fa0">GREEN</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a8a568e5f41b7e4da88fe5c4a00aad34e">YELLOW</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a1b3e1ee9bff86431dea6b181365ba65f">BLUE</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1ac634ffea7195608364671ac52ee59a61">MAGENTA</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a344dd8cd533280795b9db82ef0c92749">CYAN</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1ab5bf627e448384cf3a4c35121ca6008d">WHITE</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1aae7e8e8f5ddf114057b47f5f799347b1">BRIGHT_BLACK</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a01fd5046e2769cae7b662d3dfbb64045">BRIGHT_RED</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a7ca9c0eccf9c3fa7fe85fea8404d50a9">BRIGHT_GREEN</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a2373edc9b8eb3a49710b95c6f7906434">BRIGHT_YELLOW</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1ad187a715c27ec1e774325b6715f4da67">BRIGHT_BLUE</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1acdbf2435d34544195dd18a6b31953951">BRIGHT_MAGENTA</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1aa74a6a719de5de03a46f89ce260fa4b7">BRIGHT_CYAN</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a96718db9ec196b0ccd6f8d9517b45ea1a376520e57593580aa5c7ef8cf5136946">BRIGHT_WHITE</enumvalue>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>LogLevel</name>
      <anchorfile>Logger_8h.html</anchorfile>
      <anchor>a3d8d85bddc47594db84f7f2f84022e86</anchor>
      <arglist></arglist>
      <enumvalue file="Logger_8h.html" anchor="a3d8d85bddc47594db84f7f2f84022e86abb1ca97ec761fc37101737ba0aa2e7c5">ERROR</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a3d8d85bddc47594db84f7f2f84022e86a32bd8a1db2275458673903bdb84cb277">WARN</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a3d8d85bddc47594db84f7f2f84022e86a551b723eafd6a31d444fcb2f5920fbd3">INFO</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a3d8d85bddc47594db84f7f2f84022e86adc30ec20708ef7b0f641ef78b7880a15">DEBUG</enumvalue>
      <enumvalue file="Logger_8h.html" anchor="a3d8d85bddc47594db84f7f2f84022e86aec1f06e9fb39c4ef0729b3c7c9c8e8cc">VERBOSE</enumvalue>
    </member>
  </compound>
  <compound kind="file">
    <name>Max22200Handler.cpp</name>
    <path>handlers/max22200/</path>
    <filename>Max22200Handler_8cpp.html</filename>
    <includes id="Max22200Handler_8h" name="Max22200Handler.h" local="yes" import="no" module="no" objc="no">Max22200Handler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">Logger.h</includes>
    <includes id="HandlerCommon_8h" name="HandlerCommon.h" local="yes" import="no" module="no" objc="no">HandlerCommon.h</includes>
    <namespace>anonymous_namespace{Max22200Handler.cpp}</namespace>
    <member kind="variable" static="yes">
      <type>static constexpr const char *</type>
      <name>TAG</name>
      <anchorfile>Max22200Handler_8cpp.html</anchorfile>
      <anchor>ae7d9c64644585526560e9e6824fd84dc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Max22200Handler.h</name>
    <path>handlers/max22200/</path>
    <filename>Max22200Handler_8h.html</filename>
    <class kind="class">HalSpiMax22200Comm</class>
    <class kind="class">Max22200Handler</class>
  </compound>
  <compound kind="file">
    <name>Mcp9700TemperatureHandler.cpp</name>
    <path>handlers/mcp9700/</path>
    <filename>Mcp9700TemperatureHandler_8cpp.html</filename>
    <includes id="Mcp9700TemperatureHandler_8h" name="Mcp9700TemperatureHandler.h" local="yes" import="no" module="no" objc="no">Mcp9700TemperatureHandler.h</includes>
  </compound>
  <compound kind="file">
    <name>Mcp9700TemperatureHandler.h</name>
    <path>handlers/mcp9700/</path>
    <filename>Mcp9700TemperatureHandler_8h.html</filename>
    <class kind="class">Mcp9700AdcAdapter</class>
    <class kind="class">Mcp9700TemperatureHandler</class>
    <member kind="typedef">
      <type>hf::mcp9700::Mcp9700Thermistor&lt; Mcp9700AdcAdapter &gt;</type>
      <name>Mcp9700ThermistorConcrete</name>
      <anchorfile>Mcp9700TemperatureHandler_8h.html</anchorfile>
      <anchor>a34b1fdd2365cf2120cb3ffa6fc17d0f6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>NtcTemperatureHandler.cpp</name>
    <path>handlers/ntc/</path>
    <filename>NtcTemperatureHandler_8cpp.html</filename>
    <includes id="NtcTemperatureHandler_8h" name="NtcTemperatureHandler.h" local="yes" import="no" module="no" objc="no">NtcTemperatureHandler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>TAG</name>
      <anchorfile>NtcTemperatureHandler_8cpp.html</anchorfile>
      <anchor>a5a85b9c772bbeb480b209a3e6ea92b4c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>NtcTemperatureHandler.h</name>
    <path>handlers/ntc/</path>
    <filename>NtcTemperatureHandler_8h.html</filename>
    <class kind="class">NtcAdcAdapter</class>
    <class kind="struct">ntc_temp_handler_config_t</class>
    <class kind="class">NtcTemperatureHandler</class>
    <member kind="define">
      <type>#define</type>
      <name>NTC_TEMP_HANDLER_CONFIG_DEFAULT</name>
      <anchorfile>NtcTemperatureHandler_8h.html</anchorfile>
      <anchor>abe12c3ac819c298fa6f9dab1b6a84a8a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="typedef">
      <type>NtcThermistor&lt; NtcAdcAdapter &gt;</type>
      <name>NtcThermistorConcrete</name>
      <anchorfile>NtcTemperatureHandler_8h.html</anchorfile>
      <anchor>a0434d5464a8bfd4e789724be31b3db13</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Pca9685Handler.cpp</name>
    <path>handlers/pca9685/</path>
    <filename>Pca9685Handler_8cpp.html</filename>
    <includes id="Pca9685Handler_8h" name="Pca9685Handler.h" local="yes" import="no" module="no" objc="no">Pca9685Handler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
  </compound>
  <compound kind="file">
    <name>Pca9685Handler.h</name>
    <path>handlers/pca9685/</path>
    <filename>Pca9685Handler_8h.html</filename>
    <class kind="class">HalI2cPca9685Comm</class>
    <class kind="class">Pca9685PwmAdapter</class>
    <class kind="class">Pca9685GpioPin</class>
    <class kind="class">Pca9685Handler</class>
  </compound>
  <compound kind="file">
    <name>Pcal95555Handler.cpp</name>
    <path>handlers/pcal95555/</path>
    <filename>Pcal95555Handler_8cpp.html</filename>
    <includes id="Pcal95555Handler_8h" name="Pcal95555Handler.h" local="yes" import="no" module="no" objc="no">Pcal95555Handler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">handlers/logger/Logger.h</includes>
    <class kind="struct">anonymous_namespace{Pcal95555Handler.cpp}::NonOwningGpioDeleter</class>
    <namespace>anonymous_namespace{Pcal95555Handler.cpp}</namespace>
  </compound>
  <compound kind="file">
    <name>Pcal95555Handler.h</name>
    <path>handlers/pcal95555/</path>
    <filename>Pcal95555Handler_8h.html</filename>
    <class kind="class">HalI2cPcal95555Comm</class>
    <class kind="class">Pcal95555GpioPin</class>
    <class kind="class">Pcal95555Handler</class>
  </compound>
  <compound kind="file">
    <name>Pf1550Handler.cpp</name>
    <path>handlers/pf1550/</path>
    <filename>Pf1550Handler_8cpp.html</filename>
    <includes id="Pf1550Handler_8h" name="Pf1550Handler.h" local="yes" import="no" module="no" objc="no">Pf1550Handler.h</includes>
    <namespace>anonymous_namespace{Pf1550Handler.cpp}</namespace>
  </compound>
  <compound kind="file">
    <name>Pf1550Handler.h</name>
    <path>handlers/pf1550/</path>
    <filename>Pf1550Handler_8h.html</filename>
    <class kind="class">HalPf1550Comm</class>
    <class kind="class">Pf1550Handler</class>
  </compound>
  <compound kind="file">
    <name>Se050Handler.cpp</name>
    <path>handlers/se050/</path>
    <filename>Se050Handler_8cpp.html</filename>
    <includes id="Se050Handler_8h" name="Se050Handler.h" local="yes" import="no" module="no" objc="no">Se050Handler.h</includes>
    <namespace>anonymous_namespace{Se050Handler.cpp}</namespace>
    <member kind="define">
      <type>#define</type>
      <name>PW_SE050_RAW_HAL</name>
      <anchorfile>Se050Handler_8cpp.html</anchorfile>
      <anchor>a267c18869529f6a7c76a2fb0ad771559</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Se050Handler.h</name>
    <path>handlers/se050/</path>
    <filename>Se050Handler_8h.html</filename>
    <class kind="class">HalI2cSe050Comm</class>
    <class kind="struct">Se050HandlerConfig</class>
    <class kind="class">Se050Handler</class>
  </compound>
  <compound kind="file">
    <name>SfmHandler.cpp</name>
    <path>handlers/sfm/</path>
    <filename>SfmHandler_8cpp.html</filename>
    <includes id="SfmHandler_8h" name="SfmHandler.h" local="yes" import="no" module="no" objc="no">SfmHandler.h</includes>
    <namespace>anonymous_namespace{SfmHandler.cpp}</namespace>
  </compound>
  <compound kind="file">
    <name>SfmHandler.h</name>
    <path>handlers/sfm/</path>
    <filename>SfmHandler_8h.html</filename>
    <class kind="class">HalI2cSfmComm</class>
    <class kind="struct">SfmHandlerConfig</class>
    <class kind="class">SfmHandler</class>
    <class kind="struct">SfmHandler::Counters</class>
  </compound>
  <compound kind="file">
    <name>Tle92466edHandler.cpp</name>
    <path>handlers/tle92466ed/</path>
    <filename>Tle92466edHandler_8cpp.html</filename>
    <includes id="Tle92466edHandler_8h" name="Tle92466edHandler.h" local="yes" import="no" module="no" objc="no">Tle92466edHandler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">Logger.h</includes>
    <includes id="HandlerCommon_8h" name="HandlerCommon.h" local="yes" import="no" module="no" objc="no">HandlerCommon.h</includes>
    <namespace>anonymous_namespace{Tle92466edHandler.cpp}</namespace>
    <member kind="function" static="yes">
      <type>static tle92466ed::Channel</type>
      <name>toChannel</name>
      <anchorfile>Tle92466edHandler_8cpp.html</anchorfile>
      <anchor>a5c86099f760a9845d5fa0e65f8b57fb5</anchor>
      <arglist>(uint8_t ch)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr const char *</type>
      <name>TAG</name>
      <anchorfile>Tle92466edHandler_8cpp.html</anchorfile>
      <anchor>ae7d9c64644585526560e9e6824fd84dc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Tle92466edHandler.h</name>
    <path>handlers/tle92466ed/</path>
    <filename>Tle92466edHandler_8h.html</filename>
    <class kind="class">HalSpiTle92466edComm</class>
    <class kind="class">Tle92466edHandler</class>
    <class kind="struct">Tle92466edHandler::is_driver_result</class>
    <class kind="struct">Tle92466edHandler::is_driver_result&lt; tle92466ed::DriverResult&lt; T &gt; &gt;</class>
  </compound>
  <compound kind="file">
    <name>Tmc5160Handler.cpp</name>
    <path>handlers/tmc5160/</path>
    <filename>Tmc5160Handler_8cpp.html</filename>
    <includes id="Tmc5160Handler_8h" name="Tmc5160Handler.h" local="yes" import="no" module="no" objc="no">Tmc5160Handler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">Logger.h</includes>
    <includes id="HandlerCommon_8h" name="HandlerCommon.h" local="yes" import="no" module="no" objc="no">HandlerCommon.h</includes>
    <member kind="variable" static="yes">
      <type>static constexpr const char *</type>
      <name>TAG</name>
      <anchorfile>Tmc5160Handler_8cpp.html</anchorfile>
      <anchor>ae7d9c64644585526560e9e6824fd84dc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Tmc5160Handler.h</name>
    <path>handlers/tmc5160/</path>
    <filename>Tmc5160Handler_8h.html</filename>
    <class kind="struct">Tmc5160CtrlPins</class>
    <class kind="class">HalSpiTmc5160Comm</class>
    <class kind="class">HalUartTmc5160Comm</class>
    <class kind="class">Tmc5160Handler</class>
  </compound>
  <compound kind="file">
    <name>Tmc9660AdcWrapper.cpp</name>
    <path>handlers/tmc9660/</path>
    <filename>Tmc9660AdcWrapper_8cpp.html</filename>
    <includes id="Tmc9660AdcWrapper_8h" name="Tmc9660AdcWrapper.h" local="yes" import="no" module="no" objc="no">Tmc9660AdcWrapper.h</includes>
    <includes id="Tmc9660Handler_8h" name="Tmc9660Handler.h" local="yes" import="no" module="no" objc="no">Tmc9660Handler.h</includes>
  </compound>
  <compound kind="file">
    <name>Tmc9660AdcWrapper.h</name>
    <path>handlers/tmc9660/</path>
    <filename>Tmc9660AdcWrapper_8h.html</filename>
    <class kind="class">Tmc9660AdcWrapper</class>
  </compound>
  <compound kind="file">
    <name>Tmc9660Handler.cpp</name>
    <path>handlers/tmc9660/</path>
    <filename>Tmc9660Handler_8cpp.html</filename>
    <includes id="Tmc9660Handler_8h" name="Tmc9660Handler.h" local="yes" import="no" module="no" objc="no">Tmc9660Handler.h</includes>
    <includes id="HandlerCommon_8h" name="HandlerCommon.h" local="yes" import="no" module="no" objc="no">HandlerCommon.h</includes>
    <namespace>anonymous_namespace{Tmc9660Handler.cpp}</namespace>
  </compound>
  <compound kind="file">
    <name>Tmc9660Handler.h</name>
    <path>handlers/tmc9660/</path>
    <filename>Tmc9660Handler_8h.html</filename>
    <class kind="struct">Tmc9660CtrlPins</class>
    <class kind="class">HalSpiTmc9660Comm</class>
    <class kind="class">HalUartTmc9660Comm</class>
    <class kind="class">Tmc9660Handler</class>
    <class kind="class">Tmc9660Handler::Gpio</class>
    <class kind="class">Tmc9660Handler::Adc</class>
    <class kind="class">Tmc9660Handler::Temperature</class>
  </compound>
  <compound kind="file">
    <name>Ws2812Handler.cpp</name>
    <path>handlers/ws2812/</path>
    <filename>Ws2812Handler_8cpp.html</filename>
    <includes id="Ws2812Handler_8h" name="Ws2812Handler.h" local="yes" import="no" module="no" objc="no">Ws2812Handler.h</includes>
    <includes id="Logger_8h" name="Logger.h" local="yes" import="no" module="no" objc="no">Logger.h</includes>
    <member kind="variable" static="yes">
      <type>static constexpr const char *</type>
      <name>TAG</name>
      <anchorfile>Ws2812Handler_8cpp.html</anchorfile>
      <anchor>ae7d9c64644585526560e9e6824fd84dc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Ws2812Handler.h</name>
    <path>handlers/ws2812/</path>
    <filename>Ws2812Handler_8h.html</filename>
    <class kind="class">Ws2812Handler</class>
    <class kind="struct">Ws2812Handler::Config</class>
    <member kind="typedef">
      <type>int</type>
      <name>gpio_num_t</name>
      <anchorfile>Ws2812Handler_8h.html</anchorfile>
      <anchor>afed64f3fd254b6fa1d77cfc0026d1b3f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>README.md</name>
    <path></path>
    <filename>README_8md.html</filename>
  </compound>
  <compound kind="class">
    <name>Tmc9660Handler::Adc</name>
    <filename>classTmc9660Handler_1_1Adc.html</filename>
    <member kind="function">
      <type></type>
      <name>Adc</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a6f5e9eb8f504a04682523c5097ae713e</anchor>
      <arglist>(Tmc9660Handler &amp;parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Adc</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a9fd14f78ed8a833efb605a3359141b8f</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a3b750314ba12d924c747518c3949bc1f</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>afde209e0b01c869413c927cca08bf4a2</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxChannels</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a54138fdf832c471ce109a7c19521d27b</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelAvailable</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>acce9d9a737359a4a483fea8c03d3993e</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelV</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a4252bc585fc61b87dd2fbd3ba3e5bbe1</anchor>
      <arglist>(hf_channel_id_t channel_id, float &amp;channel_reading_v, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelCount</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a11c9f89f749cb7e1fb9af7043d1accf9</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t &amp;channel_reading_count, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a1abd9e2f3575e82c6679befeeba32b89</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t &amp;channel_reading_count, float &amp;channel_reading_v, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadMultipleChannels</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a9428171625483d715d95c4d2bf04e87c</anchor>
      <arglist>(const hf_channel_id_t *channel_ids, hf_u8_t num_channels, hf_u32_t *readings, float *voltages) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>GetStatistics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a72495dbc11ec0797172f3da4798d3b80</anchor>
      <arglist>(hf_adc_statistics_t &amp;statistics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>GetDiagnostics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>ab4b818f999a30d044ad2ef780d3bcae4</anchor>
      <arglist>(hf_adc_diagnostics_t &amp;diagnostics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ResetStatistics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>aeb392d8c266f3183dbce1f31f7bff568</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ResetDiagnostics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a4ac45d019ddc392ca9e04194e81cc3ae</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadAinChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a7b53c42cd28f39699d34fc180a3df8f6</anchor>
      <arglist>(uint8_t ain_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadCurrentSenseChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a9ea843420c0b1d68314d9699d38a4c10</anchor>
      <arglist>(uint8_t current_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadVoltageChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>ade0515c993fff08763b61988923df9cb</anchor>
      <arglist>(uint8_t voltage_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadTemperatureChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a44c5d06aa9c79fc2a38fdc53ffd69069</anchor>
      <arglist>(uint8_t temp_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadMotorDataChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>aa940fa772a9c668c698d95a47a23d67a</anchor>
      <arglist>(uint8_t motor_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_adc_err_t</type>
      <name>ValidateChannelId</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a5f9cfbbb92986ee7368b8301ff39525e</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_adc_err_t</type>
      <name>ReadChannelLocked</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a1ff51dc674661709a946c9622da98751</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t &amp;raw, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_adc_err_t</type>
      <name>UpdateStatistics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a8f7aa9ea8e16d94e1a4166af1e3821d0</anchor>
      <arglist>(hf_adc_err_t result, uint64_t start_time_us) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>uint64_t</type>
      <name>GetCurrentTimeUs</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>aa0cecbf9aacc9b985945c73343c324e8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>UpdateDiagnostics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a95192f5ef76ba77cc2abce2581f44801</anchor>
      <arglist>(hf_adc_err_t error) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>const char *</type>
      <name>GetChannelTypeString</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a65034322c01356bb594c24214577bc03</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Tmc9660Handler &amp;</type>
      <name>parent_</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a73e49be8277b954ef0a57ee319f841c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>mutex_</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>afd6a0c0ecf16ede28863879da6826c03</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_adc_statistics_t</type>
      <name>statistics_</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a528e613422995ab34e5d923ed01c3dc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_adc_diagnostics_t</type>
      <name>diagnostics_</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>ada0fe00d5c9b9d99bc40437a61eebb4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; hf_adc_err_t &gt;</type>
      <name>last_error_</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a038a19dedbc96effaa9e41a43c5d85f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a3b750314ba12d924c747518c3949bc1f</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>afde209e0b01c869413c927cca08bf4a2</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxChannels</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a54138fdf832c471ce109a7c19521d27b</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelAvailable</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>acce9d9a737359a4a483fea8c03d3993e</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelV</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a4252bc585fc61b87dd2fbd3ba3e5bbe1</anchor>
      <arglist>(hf_channel_id_t channel_id, float &amp;channel_reading_v, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelCount</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a11c9f89f749cb7e1fb9af7043d1accf9</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t &amp;channel_reading_count, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a1abd9e2f3575e82c6679befeeba32b89</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t &amp;channel_reading_count, float &amp;channel_reading_v, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadMultipleChannels</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a9428171625483d715d95c4d2bf04e87c</anchor>
      <arglist>(const hf_channel_id_t *channel_ids, hf_u8_t num_channels, hf_u32_t *readings, float *voltages) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>GetStatistics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a72495dbc11ec0797172f3da4798d3b80</anchor>
      <arglist>(hf_adc_statistics_t &amp;statistics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>GetDiagnostics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>ab4b818f999a30d044ad2ef780d3bcae4</anchor>
      <arglist>(hf_adc_diagnostics_t &amp;diagnostics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ResetStatistics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>aeb392d8c266f3183dbce1f31f7bff568</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ResetDiagnostics</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a4ac45d019ddc392ca9e04194e81cc3ae</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadAinChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a7b53c42cd28f39699d34fc180a3df8f6</anchor>
      <arglist>(uint8_t ain_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadCurrentSenseChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a9ea843420c0b1d68314d9699d38a4c10</anchor>
      <arglist>(uint8_t current_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadVoltageChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>ade0515c993fff08763b61988923df9cb</anchor>
      <arglist>(uint8_t voltage_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadTemperatureChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>a44c5d06aa9c79fc2a38fdc53ffd69069</anchor>
      <arglist>(uint8_t temp_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadMotorDataChannel</name>
      <anchorfile>classTmc9660Handler_1_1Adc.html</anchorfile>
      <anchor>aa940fa772a9c668c698d95a47a23d67a</anchor>
      <arglist>(uint8_t motor_channel, hf_u32_t &amp;raw_value, float &amp;voltage) noexcept</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Ads7952Diagnostics</name>
    <filename>structAds7952Diagnostics.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>initialized</name>
      <anchorfile>structAds7952Diagnostics.html</anchorfile>
      <anchor>a330bcdfe7a6e32374d441b178547f85b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>driver_ready</name>
      <anchorfile>structAds7952Diagnostics.html</anchorfile>
      <anchor>a770783766b6f482b545f07d06beae865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ads7952::Mode</type>
      <name>current_mode</name>
      <anchorfile>structAds7952Diagnostics.html</anchorfile>
      <anchor>a909ef51163cdd4bb49e6bcdd323eacb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ads7952::Range</type>
      <name>current_range</name>
      <anchorfile>structAds7952Diagnostics.html</anchorfile>
      <anchor>a2dd09f9ce8fd131638c2132eae9d39fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>vref</name>
      <anchorfile>structAds7952Diagnostics.html</anchorfile>
      <anchor>ab57ef49d134f6dd382896e67c86acaba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>active_vref</name>
      <anchorfile>structAds7952Diagnostics.html</anchorfile>
      <anchor>a9825d914e0233c1c4c74e7acad37ecef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>total_reads</name>
      <anchorfile>structAds7952Diagnostics.html</anchorfile>
      <anchor>ae63b7d77f408be06c65dd7e2d7207bef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>error_count</name>
      <anchorfile>structAds7952Diagnostics.html</anchorfile>
      <anchor>a7b89100e37bacd9b01d325b115c965bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>device_index</name>
      <anchorfile>structAds7952Diagnostics.html</anchorfile>
      <anchor>a6c497ddf14c09f1cf42fcdd933e02d42</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ads7952Handler</name>
    <filename>classAds7952Handler.html</filename>
    <member kind="function">
      <type></type>
      <name>Ads7952Handler</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a33dc51f5cf79be9730b4ad89010160a8</anchor>
      <arglist>(BaseSpi &amp;spi_interface, const Ads7952HandlerConfig &amp;config=GetDefaultAds7952Config()) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Ads7952Handler</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a13ed8f1aaf11c19d5670f9c32ac89cf2</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Ads7952Handler</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a88d8ac328ed230d828148eecc9552cc4</anchor>
      <arglist>(const Ads7952Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Ads7952Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>ae677b9a01f88f0bb83b312ea34c6b4d7</anchor>
      <arglist>(const Ads7952Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Ads7952Handler</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a1f4a7527384227733fcd55932d1f1838</anchor>
      <arglist>(Ads7952Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Ads7952Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a1ff4d6d7cc4a98462add32fa5f0c9cd0</anchor>
      <arglist>(Ads7952Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a0644e9b1c82bc200591155b36a9ca4dd</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>afbc0202e5ff46d90c5bcbef8618ff94d</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxChannels</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>aabec13f63b24c69229d0930fc7751707</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelAvailable</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a3ee435d534550023462c51e24b146aa5</anchor>
      <arglist>(hf_channel_id_t channel) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelV</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a7887cb224ec4d97b198f4fb45a1775b2</anchor>
      <arglist>(hf_channel_id_t channel, float &amp;voltage, hf_u8_t samples=1, hf_time_t timeout_ms=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelCount</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>ac43a66dc6bb9495501fe1b6f52d6b9f9</anchor>
      <arglist>(hf_channel_id_t channel, hf_u32_t &amp;count, hf_u8_t samples=1, hf_time_t timeout_ms=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannel</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a4796fe85c52cfd13d55f550063bf18af</anchor>
      <arglist>(hf_channel_id_t channel, hf_u32_t &amp;count, float &amp;voltage, hf_u8_t samples=1, hf_time_t timeout_ms=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadMultipleChannels</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a04d47ae11d8f143379fc62f580010de6</anchor>
      <arglist>(const hf_channel_id_t *channels, hf_u8_t num_channels, hf_u32_t *counts, float *voltages) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ReadAllChannels</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>adf5fd6ce6e060862d6d0f83b896b403f</anchor>
      <arglist>(ads7952::ChannelReadings &amp;readings) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ReadAuto1Channels</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>afbb86a7af87f258fdadf5e436800a6be</anchor>
      <arglist>(uint16_t mask, ads7952::ChannelReadings &amp;readings) noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>RawManualFrames</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a5a2b66bed2f15d3e9c3ab2c2b5f06d2d</anchor>
      <arglist>(uint8_t channel, uint16_t *out, uint8_t n) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ProgramAlarm</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a0efbed6e9ec52986720b088d74310743</anchor>
      <arglist>(uint8_t channel, ads7952::AlarmBound bound, uint16_t threshold_12bit) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ProgramAlarmVoltage</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a1cda2d2ebe4ce49e2c600cae260cb9ea</anchor>
      <arglist>(uint8_t channel, ads7952::AlarmBound bound, float voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>SetRange</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a163bcf0a7f0b0a9c62d3d716ba34e8ba</anchor>
      <arglist>(ads7952::Range range) noexcept</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>GetActiveVref</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a90f8a58167a9d70ce28b96c106ec7f37</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>ads7952::ADS7952&lt; Ads7952SpiAdapter &gt; *</type>
      <name>GetDriver</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a7703915b68bb1ef88fbbd980bf938d90</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const ads7952::ADS7952&lt; Ads7952SpiAdapter &gt; *</type>
      <name>GetDriver</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>ae85d3136bffa914057733aacd43ecd65</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a69677cd8a49a9d456c739755d3bd2943</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept -&gt; decltype(fn(std::declval&lt; ads7952::ADS7952&lt; Ads7952SpiAdapter &gt; &amp; &gt;()))</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>abd4eadbc944d59aa00165ea60a3bff3a</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>GetHandlerDiagnostics</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a54f86046573eed687229f577a62cfd2f</anchor>
      <arglist>(Ads7952Diagnostics &amp;diag) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a993769ffc1a79c7253cc7ebddcb7f622</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>GetDeviceIndex</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a538d1600f17a4b48c175d55144bbd0ec</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a4541d57be2e20b35d9270a62642fb862</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>ApplyConfiguration</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a3cc3a33955e1e0b218f801f1b98192f2</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>ads7952::ReadResult</type>
      <name>ReadChannelLocked</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>aba1840f09961e541562bb2bfd646a53d</anchor>
      <arglist>(uint8_t channel) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_ref_</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a1e504e9bea722ce507b7fcdc6f258a5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Ads7952SpiAdapter &gt;</type>
      <name>spi_adapter_</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a0fd2e299b6d37bd1e7e0ac4842877650</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; ads7952::ADS7952&lt; Ads7952SpiAdapter &gt; &gt;</type>
      <name>adc_driver_</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a774221748d6d89b3db751dff6c94e5e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Ads7952HandlerConfig</type>
      <name>config_</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a2703e257a893f7bd4b14c2de006ade9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a6594e53c4f1ab256d4888d41fe0116af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a7f09602e3748f03bcd3e419f812bb6c9</anchor>
      <arglist>[64]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint32_t</type>
      <name>total_reads_</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>a22605a5709c4b52434d13325edc726de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint32_t</type>
      <name>error_count_</name>
      <anchorfile>classAds7952Handler.html</anchorfile>
      <anchor>acc134b2af544d0d3066d2edf4c50b8e7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Ads7952HandlerConfig</name>
    <filename>structAds7952HandlerConfig.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>vref</name>
      <anchorfile>structAds7952HandlerConfig.html</anchorfile>
      <anchor>a14d559024176320ca47e66b5587af8a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>va</name>
      <anchorfile>structAds7952HandlerConfig.html</anchorfile>
      <anchor>a81f827c994d14cf5dd6061b625ca17c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ads7952::Range</type>
      <name>range</name>
      <anchorfile>structAds7952HandlerConfig.html</anchorfile>
      <anchor>a412f1a1bd62d08b4ce5139ac34e9a469</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ads7952::Mode</type>
      <name>initial_mode</name>
      <anchorfile>structAds7952HandlerConfig.html</anchorfile>
      <anchor>a2557c74a127c8f19803e2d2164604fdc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>auto1_channel_mask</name>
      <anchorfile>structAds7952HandlerConfig.html</anchorfile>
      <anchor>a434e6731b486eb967ceca6014c046160</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>auto2_last_channel</name>
      <anchorfile>structAds7952HandlerConfig.html</anchorfile>
      <anchor>acea2eba3f86da993afa473f3f2ddf351</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>device_index</name>
      <anchorfile>structAds7952HandlerConfig.html</anchorfile>
      <anchor>a38e2c68ee93ae27b4061c323d3e7a5f4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ads7952SpiAdapter</name>
    <filename>classAds7952SpiAdapter.html</filename>
    <member kind="function">
      <type></type>
      <name>Ads7952SpiAdapter</name>
      <anchorfile>classAds7952SpiAdapter.html</anchorfile>
      <anchor>a0d1892fcd00563ac8f09ec0c956d971b</anchor>
      <arglist>(BaseSpi &amp;spi_interface) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>transfer</name>
      <anchorfile>classAds7952SpiAdapter.html</anchorfile>
      <anchor>a58116a4703d7b30c2c215ca1539317c5</anchor>
      <arglist>(const uint8_t *tx, uint8_t *rx, std::size_t len) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_interface_</name>
      <anchorfile>classAds7952SpiAdapter.html</anchorfile>
      <anchor>a03c0c191aa96b79a65fad3c8a2fc864f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Ads9324Diagnostics</name>
    <filename>structAds9324Diagnostics.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>initialized</name>
      <anchorfile>structAds9324Diagnostics.html</anchorfile>
      <anchor>a13b7a47cb924e79c7c3da8ab7277f16b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>driver_ready</name>
      <anchorfile>structAds9324Diagnostics.html</anchorfile>
      <anchor>a6e2479374729ad31f471e6595a8bfec1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>device_id</name>
      <anchorfile>structAds9324Diagnostics.html</anchorfile>
      <anchor>a8322d92fdb5e54db1ddfda11cdcad2d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>total_reads</name>
      <anchorfile>structAds9324Diagnostics.html</anchorfile>
      <anchor>ae87a95cf7040c24fa820c2109d9bfabd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>error_count</name>
      <anchorfile>structAds9324Diagnostics.html</anchorfile>
      <anchor>a0c5a2b154b2f91c6bffffad14f07e100</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>device_index</name>
      <anchorfile>structAds9324Diagnostics.html</anchorfile>
      <anchor>ac12d4a53a0b70c8d89705368bd55cce4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ads9324Handler</name>
    <filename>classAds9324Handler.html</filename>
    <member kind="function">
      <type></type>
      <name>Ads9324Handler</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a152b9925167b95d7da40e66c6aa81d69</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio *convst, BaseGpio *drdy=nullptr, const Ads9324HandlerConfig &amp;config=GetDefaultAds9324Config()) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Ads9324Handler</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a6a54c3513c2207b06bb11c62a6dda8ca</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Ads9324Handler</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>af5d7d6b0d676974450a23c52fbd44be1</anchor>
      <arglist>(const Ads9324Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Ads9324Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a8761d65858a19a5cf0fb2abb1166b4b8</anchor>
      <arglist>(const Ads9324Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Ads9324Handler</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>ab4d999d7603b7b818c79e73ed8b1cf12</anchor>
      <arglist>(Ads9324Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Ads9324Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a33e01f2bb18ba57af97b11ee453b96c4</anchor>
      <arglist>(Ads9324Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>adbbb0527b10e0ed286ab9296d618b120</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a5a6a01cee4fe26870a33bcea96dcaaff</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxChannels</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a6c70dfc02fa5b8f7abbb73464de5c09a</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelAvailable</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a1222cae22149c62b6364ba172c9e25cd</anchor>
      <arglist>(hf_channel_id_t channel) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelV</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a2b619080fedca5b47785c85511d1457d</anchor>
      <arglist>(hf_channel_id_t channel, float &amp;voltage, hf_u8_t samples=1, hf_time_t timeout_ms=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelCount</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a3be56627a87e3a0b243af4d5acd1fd84</anchor>
      <arglist>(hf_channel_id_t channel, hf_u32_t &amp;count, hf_u8_t samples=1, hf_time_t timeout_ms=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannel</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a38883752ee6121f8cd4311980cdc9e77</anchor>
      <arglist>(hf_channel_id_t channel, hf_u32_t &amp;count, float &amp;voltage, hf_u8_t samples=1, hf_time_t timeout_ms=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadMultipleChannels</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a49ce15cdc2d6a6c30f1d23d77234b618</anchor>
      <arglist>(const hf_channel_id_t *channels, hf_u8_t num_channels, hf_u32_t *counts, float *voltages) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ConfigureChannel</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a14ced79eeea0e079ca343fa43172fdc9</anchor>
      <arglist>(uint8_t channel, const ads9324::ChannelConfig &amp;cfg) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>SetChannelOffset</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>afa906309c1e2145d2f4c8f9ce3861fa8</anchor>
      <arglist>(uint8_t channel, int16_t ofs_10bit) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>SetChannelGain</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a4f6a0225529a2ed222093224604e491e</anchor>
      <arglist>(uint8_t channel, uint16_t gan_14bit) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ReadSnapshot</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>aeeb32661714529229825e30be65be92a</anchor>
      <arglist>(ads9324::Snapshot &amp;snap) noexcept</arglist>
    </member>
    <member kind="function">
      <type>ads9324::ADS9324&lt; Ads9324SpiHostAdapter, Ads9324SpiHostAdapter &gt; *</type>
      <name>GetDriver</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a7b64ce0d4b8024108ca6967f531aafdf</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const ads9324::ADS9324&lt; Ads9324SpiHostAdapter, Ads9324SpiHostAdapter &gt; *</type>
      <name>GetDriver</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a1bba25086fc79a3c8078f91a6c378033</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a6bebc5c00b2334608502e23dd8e6044e</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept -&gt; decltype(fn(std::declval&lt; ads9324::ADS9324&lt; Ads9324SpiHostAdapter, Ads9324SpiHostAdapter &gt; &amp; &gt;()))</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>af1813c34164dc6a9ba4ea5130a9597ee</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>GetHandlerDiagnostics</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a3ebb3e41cee3693204512a6389c5683d</anchor>
      <arglist>(Ads9324Diagnostics &amp;diag) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a3517fca0e15619a1733b7e76b2954e6d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>GetDeviceIndex</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a5291f6c944c95dc8247749ece9277468</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a4fa9be5165c8fd545b9b3d80b3c7cb4b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_ref_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a6c9a8a259968245f90b01eda95534dc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>convst_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a32b754b43864360828c245619ef8c1d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>drdy_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a27894f7dc33965bad691040a5de3608a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Ads9324SpiHostAdapter &gt;</type>
      <name>adapter_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a490d2e0294859b800a9584ebf325c5ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; ads9324::ADS9324&lt; Ads9324SpiHostAdapter, Ads9324SpiHostAdapter &gt; &gt;</type>
      <name>adc_driver_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>aeec4debdd779646d645c7cb8feafd516</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Ads9324HandlerConfig</type>
      <name>config_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>aa8fb1394e3c4c8742700fe87dd7f3ee4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a081c73eea399e48df337186a72388a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>ad27fffbb14d106f244be03b96f06c762</anchor>
      <arglist>[64]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint32_t</type>
      <name>total_reads_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a40ab700c7b3a71987ae2d3da3202a14d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint32_t</type>
      <name>error_count_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>a038c65dd5cdc7293c89e30adb220ee79</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>device_id_</name>
      <anchorfile>classAds9324Handler.html</anchorfile>
      <anchor>aa26750f4d554e9609a7581d41462023f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Ads9324HandlerConfig</name>
    <filename>structAds9324HandlerConfig.html</filename>
    <member kind="variable">
      <type>ads9324::ChannelConfig</type>
      <name>default_channel</name>
      <anchorfile>structAds9324HandlerConfig.html</anchorfile>
      <anchor>ac95e185b86189d4aca449b399673cea9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ads9324::DataFormat</type>
      <name>format</name>
      <anchorfile>structAds9324HandlerConfig.html</anchorfile>
      <anchor>a1c2770f491b91ac259e1b1aa6a34c2c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>device_index</name>
      <anchorfile>structAds9324HandlerConfig.html</anchorfile>
      <anchor>aeea219262d2eb66a3f4cb8b84d818fc3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ads9324SpiHostAdapter</name>
    <filename>classAds9324SpiHostAdapter.html</filename>
    <member kind="function">
      <type></type>
      <name>Ads9324SpiHostAdapter</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>aae0fd99bfab3856beebd84b419f40425</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio *convst, BaseGpio *drdy) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>transfer</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>ae17f2d3e0349963cc5c2f170126ddbff</anchor>
      <arglist>(const uint8_t *tx, uint8_t *rx, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ConvstWrite</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>a8682264600adf850726af4d463e9cc68</anchor>
      <arglist>(bool high) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>DrdyRead</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>a9ad2b988e738267267298f7f72bc5e4b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HasDrdy</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>a6cb520495e1db843a29029569ac07a36</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>a4f5db16cfd1980f10e44e2c6023fcc1f</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayMs</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>a67e99ac13311db109801ae23bd7345e8</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>ab435657583a6e98b58f95cb934ba2aca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>convst_</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>ad29ef2c6c2062ff17f6da696d94505c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>drdy_</name>
      <anchorfile>classAds9324SpiHostAdapter.html</anchorfile>
      <anchor>a9e8e678c02c0d28598830c4154af73f9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>AlicatBasis2Handler</name>
    <filename>classAlicatBasis2Handler.html</filename>
    <class kind="struct">AlicatBasis2Handler::DiscoveredDevice</class>
    <member kind="typedef">
      <type>alicat_basis2::Driver&lt; HalUartAlicatBasis2Comm &gt;</type>
      <name>DriverType</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a9d598dbc4dee826db750abd1d6735d4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::function&lt; bool(std::uint32_t bps)&gt;</type>
      <name>HostBaudSetter</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>ad5e74b2aaf285a5292b5e7fe2a46aae3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AlicatBasis2Handler</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a0da54c2220e86de2922c77bcd28b852a</anchor>
      <arglist>(BaseUart &amp;uart, const AlicatBasis2HandlerConfig &amp;config, RtosMutex *bus_mutex=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AlicatBasis2Handler</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>ad5bc3de022e93f0ae548054bafcabb79</anchor>
      <arglist>(const AlicatBasis2Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>AlicatBasis2Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a3a5eeba10bc6d23a3ae84dd7353be1c3</anchor>
      <arglist>(const AlicatBasis2Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>aded4fa6cc765ede4a6a3c04247a6f32d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a04e625d6bac3ae8926566637c1c13caf</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::uint8_t</type>
      <name>Address</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a543989ebb5cd06901dcdf8f008cdef6d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::uint8_t</type>
      <name>DeviceIndex</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a41bc72d284a6de1b84f8b37484655987</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const alicat_basis2::InstrumentIdentity &amp;</type>
      <name>Identity</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a50fd0dc280bd18d3039a85e82902ca9f</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::uint8_t</type>
      <name>FlowDecimals</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a74f17434770f929c9499f47f90000ab4</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; alicat_basis2::InstantaneousData &gt;</type>
      <name>ReadInstantaneous</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a130e3b961f990b0223cd54aad6c75e12</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; alicat_basis2::InstrumentIdentity &gt;</type>
      <name>RereadIdentity</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a951bb57d22a226307b5dca483276ffc2</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>Tare</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>ad1beb9c75aaec1b7620fb02b6ecf590e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetGas</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>af8d01a125e5a78f2a47b92b8213482e1</anchor>
      <arglist>(alicat_basis2::Gas g) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetSetpoint</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>aa077175c8ca4c4c9dae658cd7d1b6583</anchor>
      <arglist>(float user_units) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetSetpointSource</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a6e1a7abc9a81d948c0320ad4d0252757</anchor>
      <arglist>(alicat_basis2::SetpointSource s) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetCommWatchdogMs</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a30400e5ff5010a062ea18c0e2e12f310</anchor>
      <arglist>(std::uint16_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetMaxSetpointRamp</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a9f1efadddf640c602887f1d05d8cae92</anchor>
      <arglist>(std::uint32_t pct_per_ms_x_10e7) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetAutotareEnabled</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a4db8e96ba1db999aa1cc0676dd279c27</anchor>
      <arglist>(bool enabled) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>ResetTotalizer</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>ae0ba62eb3d3e0e966836f51bb068a8da</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetTotalizerLimitMode</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>acda55975494409ca7294be62ad0422c2</anchor>
      <arglist>(alicat_basis2::TotalizerLimitMode m) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetTotalizerBatch</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a2f1959c3fa989762a5c9948af60bbeb4</anchor>
      <arglist>(std::uint32_t value_scaled) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetFlowAveragingMs</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a0fddcc97075af74f60cbbf48869b228d</anchor>
      <arglist>(std::uint16_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetReferenceTemperatureC</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>ae2eb92dbed17d08bb62f4fa14f858027</anchor>
      <arglist>(float c) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>ConfigureMeasurementTrigger</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a725295638daba95f3c577367af4d0a95</anchor>
      <arglist>(std::uint16_t bits) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>StartMeasurementSamples</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a5ffb42c0cef940f78d04c26fe8f61c51</anchor>
      <arglist>(std::uint16_t n) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; alicat_basis2::MeasurementData &gt;</type>
      <name>ReadMeasurement</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a484df6ff00c4ef3c24fe4c9c76e9d8bd</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetModbusAddress</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a001984561e44eb0723ae1029521f69ec</anchor>
      <arglist>(std::uint8_t addr) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetAsciiUnitId</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a929b084b2ca994947f04fc5744c18c7c</anchor>
      <arglist>(char id) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>SetBaudRate</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a38b458591fe58f4115e9b4f5f8b9eb6a</anchor>
      <arglist>(alicat_basis2::BaudRate br) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; void &gt;</type>
      <name>FactoryRestore</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a898466d43a57b153814e922dbc428727</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; std::uint8_t &gt;</type>
      <name>Discover</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a9bdba31a2f80ef97a6b2c98dc83d90a0</anchor>
      <arglist>(std::uint8_t *present_bitmap, std::size_t bitmap_bytes, std::uint16_t probe_timeout_ms=30) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; std::size_t &gt;</type>
      <name>DiscoverAcrossBauds</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a4400773ddc1c5ede15d453b1d926120c</anchor>
      <arglist>(DiscoveredDevice *out, std::size_t max_devices, const HostBaudSetter &amp;set_host_baud, std::uint16_t probe_timeout_ms=30, std::uint32_t settle_ms=25, const std::uint32_t *baud_list_bps=nullptr, std::size_t baud_list_count=0) noexcept</arglist>
    </member>
    <member kind="function">
      <type>alicat_basis2::DriverResult&lt; std::size_t &gt;</type>
      <name>NormalizeBusBaud</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>acd0909cb398b2b50a90f161bd3b805d0</anchor>
      <arglist>(std::uint32_t target_bps, const DiscoveredDevice *devices, std::size_t device_count, const HostBaudSetter &amp;set_host_baud, std::uint8_t *failed_addresses_bitmap=nullptr, std::size_t failed_bitmap_bytes=0, std::uint16_t verify_timeout_ms=100) noexcept</arglist>
    </member>
    <member kind="function">
      <type>DriverType *</type>
      <name>GetDriver</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>add62849756ef0690f6ce7eca4109d356</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a2f319111cee27d138e90a7c5b6e3b46b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>AlicatBasis2HandlerConfig</type>
      <name>config_</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a339335d6ddb889898121979b0a7e428f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>HalUartAlicatBasis2Comm</type>
      <name>comm_</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a53967f68c415f044eb51cca36d0a6d0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; DriverType &gt;</type>
      <name>driver_</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>adc2e8208d97da3c109ed1bc355ae0169</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>alicat_basis2::InstrumentIdentity</type>
      <name>identity_</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>adea6cf7e6cc96e756c4349ed53a6557e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::uint8_t</type>
      <name>flow_decimals_</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>ad8339393101f5c6357f300675b53f751</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; bool &gt;</type>
      <name>initialized_</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a650a683c3ea6c86ae54d3419a0f749fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>private_mutex_</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>aff9d1e09053403078971df23240870f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex *</type>
      <name>bus_mutex_</name>
      <anchorfile>classAlicatBasis2Handler.html</anchorfile>
      <anchor>a028a9a4a19a368c9b48813410aa44255</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AlicatBasis2HandlerConfig</name>
    <filename>structAlicatBasis2HandlerConfig.html</filename>
    <member kind="variable">
      <type>std::uint8_t</type>
      <name>modbus_address</name>
      <anchorfile>structAlicatBasis2HandlerConfig.html</anchorfile>
      <anchor>a4dfea5755769cc94caf856f3dc14d20a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint16_t</type>
      <name>timeout_ms</name>
      <anchorfile>structAlicatBasis2HandlerConfig.html</anchorfile>
      <anchor>a25fbef9f46be0b67fdb49be60ecbebc7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint8_t</type>
      <name>device_index</name>
      <anchorfile>structAlicatBasis2HandlerConfig.html</anchorfile>
      <anchor>acb67eacf9bbb74a394c0195d813a4f6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint8_t</type>
      <name>flow_decimals_default</name>
      <anchorfile>structAlicatBasis2HandlerConfig.html</anchorfile>
      <anchor>aaa1795d23555157dadf8a4ba2633826e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>As5047uConfig</name>
    <filename>structAs5047uConfig.html</filename>
    <member kind="variable">
      <type>FrameFormat</type>
      <name>frame_format</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>af7982d833742164706c12d4e7c5e7739</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>crc_retries</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>ad4039ffbfdad7f1fda749ff1ad8f8394</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_daec</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>ae86f40b2989be31fee7b397ecaad0e03</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_adaptive_filter</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>a970e4eba11928e338870a65df6897a94</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>zero_position</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>a40b03f2b1943a55386666bbc048a5612</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_abi_output</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>a7434166ac0ecdf07a1c1d4aabad6372a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_uvw_output</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>a492273d284a915045db486fd9893b789</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_pwm_output</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>a100ebc08f150771ad11c9b624551818c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>abi_resolution_bits</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>a078c980d2b3827e7686a5b39baea4162</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>uvw_pole_pairs</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>a5938deaf6c6b4684bd829bfa47792357</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>high_temperature_mode</name>
      <anchorfile>structAs5047uConfig.html</anchorfile>
      <anchor>ac67cfd3cfd1942ee97e5a5aacfcaf24b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>As5047uDiagnostics</name>
    <filename>structAs5047uDiagnostics.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>magnetic_field_ok</name>
      <anchorfile>structAs5047uDiagnostics.html</anchorfile>
      <anchor>ab6d5b5b629f8c8a47607276b4e56ee55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>agc_warning</name>
      <anchorfile>structAs5047uDiagnostics.html</anchorfile>
      <anchor>a8b59af0c7aebe5939e36098a0b7a7fd8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>cordic_overflow</name>
      <anchorfile>structAs5047uDiagnostics.html</anchorfile>
      <anchor>a4ad9860e18065baf503da55c440d77e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>offset_compensation_ok</name>
      <anchorfile>structAs5047uDiagnostics.html</anchorfile>
      <anchor>ac5bc5f03da7686a4db947cec93b2cd44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>communication_ok</name>
      <anchorfile>structAs5047uDiagnostics.html</anchorfile>
      <anchor>ae9f8a494782de68b41318d8621c2a3ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>last_error_flags</name>
      <anchorfile>structAs5047uDiagnostics.html</anchorfile>
      <anchor>a81510ba051729ca9e90821b030eac5ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>communication_errors</name>
      <anchorfile>structAs5047uDiagnostics.html</anchorfile>
      <anchor>a44c0983e85ef16a245011892369471a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>total_measurements</name>
      <anchorfile>structAs5047uDiagnostics.html</anchorfile>
      <anchor>a15245dd4b501df7cc4d5c1d10a297771</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>As5047uHandler</name>
    <filename>classAs5047uHandler.html</filename>
    <member kind="function">
      <type></type>
      <name>As5047uHandler</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a9db3ada6bdf71212d36b8e2d12ef6113</anchor>
      <arglist>(BaseSpi &amp;spi_interface, const As5047uConfig &amp;config=GetDefaultConfig()) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~As5047uHandler</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>aec5cc306958b6915f3324526fefe8403</anchor>
      <arglist>() noexcept=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>As5047uHandler</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a9aee9ec6fbcdc2074a32659c97307bf4</anchor>
      <arglist>(const As5047uHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>As5047uHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a9e503ad72ffc736d9d84950f951c80f2</anchor>
      <arglist>(const As5047uHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>As5047uHandler</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>aed1b1a02d2cebaed9d882476c946760a</anchor>
      <arglist>(As5047uHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>As5047uHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a557b92db81b7f6e125d6410c9720fb7c</anchor>
      <arglist>(As5047uHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a157826010a85f4279b7a13791fcf2003</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a7f16efc74198279ee63c46d2b55825fa</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a7b178fdb83d16cee568f7e52b2b3f85a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>ab0ff2f9e31a7cbc1e1a21eff75ff5188</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsSensorReady</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>abe61e6ac904d2cb3ab8b6d679aacfb97</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>as5047u::AS5047U&lt; As5047uSpiAdapter &gt; *</type>
      <name>GetSensor</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a7935a51fb2973e3a1793d378fc50118a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>as5047u::AS5047U&lt; As5047uSpiAdapter &gt; *</type>
      <name>GetDriver</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a10e4de42743f7ceb1f2fef7cadbfc000</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const as5047u::AS5047U&lt; As5047uSpiAdapter &gt; *</type>
      <name>GetDriver</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>aff42824ec0c4048211b432a35946ce19</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a60c746a9c38be3c77de2c6148fbfbe54</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept -&gt; decltype(fn(std::declval&lt; as5047u::AS5047U&lt; As5047uSpiAdapter &gt; &amp; &gt;()))</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a9c4b15673544646987fce22ca789e597</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>AS5047U_Error</type>
      <name>GetLastError</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a4cf122135165cde95680fe70ca55efcc</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a4b24bfde6fe7f29e66b61828f44d2dff</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static As5047uConfig</type>
      <name>GetDefaultConfig</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a5cfe5bb285cb087d078e5f248ccd0d82</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>ValidateSensor</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a10eeb015c791ff484935168e3683c053</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a040ba453eb7892ab3189631d73a2f683</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>HandleSensorErrors</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a6b29b6418f7b2d19cbdcc65aa6eb9756</anchor>
      <arglist>(uint16_t sensor_errors) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>UpdateDiagnostics</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a6854be00365afbb2eeddfb3177939456</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>ApplyConfiguration</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a0e7c20d4fd883c55ff9091041ec04c39</anchor>
      <arglist>(const As5047uConfig &amp;config) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_ref_</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>afa515ec63a07cc2a19020e53673fc0cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; As5047uSpiAdapter &gt;</type>
      <name>spi_adapter_</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a345768a88cb50d8751d7818a99d78991</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; as5047u::AS5047U&lt; As5047uSpiAdapter &gt; &gt;</type>
      <name>as5047u_sensor_</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>adf87c4d40df713678dfae26dc4585ad2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>As5047uConfig</type>
      <name>config_</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>afe844da69eb5d41abf3f5a7a4ed7d5b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a6a3896d3b872ad10d3f7b64d074a81ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a06e319b13ee69597297a881236b6464f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>AS5047U_Error</type>
      <name>last_error_</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>aa61a5e00eb613efc5cec7b31ae86d2b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>As5047uDiagnostics</type>
      <name>diagnostics_</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a5af0b45c93a36c7c97030b546eb6fed8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classAs5047uHandler.html</anchorfile>
      <anchor>a087d0d651eedf85bfb5ff373ba5e7961</anchor>
      <arglist>[64]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>As5047uMeasurement</name>
    <filename>structAs5047uMeasurement.html</filename>
    <member kind="variable">
      <type>uint16_t</type>
      <name>angle_raw</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>a90d827bdb1b61fe2defee715c92be2f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>angle_compensated</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>ab1abcdd7a68fbaa6560058774e94122a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int16_t</type>
      <name>velocity_raw</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>a4556c5d147003c5ad22d364350569a09</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>velocity_deg_per_sec</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>ad225b2b6862308ff0e6c99f7214afd99</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>velocity_rad_per_sec</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>a5ac093437fbd32c0779631f3264efab2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>velocity_rpm</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>a6a357d1e546fb4cf5d0dfb98a28375c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>agc_value</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>ac4ca209ff2462bcbbee2d2c14ab40f11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>magnitude</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>a6f233a35bd987a4053c3c89ce9f2fd26</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>error_flags</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>a51e4df549e1fdae289d49bb9f9db7cd3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>structAs5047uMeasurement.html</anchorfile>
      <anchor>ac1ec2bc694b2feb47c0a3a2abb172f90</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>As5047uSpiAdapter</name>
    <filename>classAs5047uSpiAdapter.html</filename>
    <member kind="function">
      <type></type>
      <name>As5047uSpiAdapter</name>
      <anchorfile>classAs5047uSpiAdapter.html</anchorfile>
      <anchor>a42035583f8ce18bacc85e87708aeb297</anchor>
      <arglist>(BaseSpi &amp;spi_interface) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>transfer</name>
      <anchorfile>classAs5047uSpiAdapter.html</anchorfile>
      <anchor>a5c90fccf33a360042821dcd5f94aa941</anchor>
      <arglist>(const uint8_t *tx, uint8_t *rx, std::size_t len) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_interface_</name>
      <anchorfile>classAs5047uSpiAdapter.html</anchorfile>
      <anchor>a37cd11da2a833789f8880fc2e3dfe55b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AsciiArtFormat</name>
    <filename>structAsciiArtFormat.html</filename>
    <member kind="variable">
      <type>LogColor</type>
      <name>color</name>
      <anchorfile>structAsciiArtFormat.html</anchorfile>
      <anchor>a572b21fc865079a7a9d52ce9d693880c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>LogBackground</type>
      <name>background</name>
      <anchorfile>structAsciiArtFormat.html</anchorfile>
      <anchor>a91b637360df475a056a6d4dd8367aa37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>LogStyle</type>
      <name>style</name>
      <anchorfile>structAsciiArtFormat.html</anchorfile>
      <anchor>adeeb1aebe6628af741fa7a24b140ddc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>center_art</name>
      <anchorfile>structAsciiArtFormat.html</anchorfile>
      <anchor>ae06514377c3e9b3fda97c96d03192902</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>add_border</name>
      <anchorfile>structAsciiArtFormat.html</anchorfile>
      <anchor>a03474ec42a1baad458fc1d6b7b5e2a9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>border_char</name>
      <anchorfile>structAsciiArtFormat.html</anchorfile>
      <anchor>adf9e00bc2854bfa8598a058d388510b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>border_padding</name>
      <anchorfile>structAsciiArtFormat.html</anchorfile>
      <anchor>a37ed73c07ea928261da4900d0766ad9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>max_width</name>
      <anchorfile>structAsciiArtFormat.html</anchorfile>
      <anchor>ab73c6d828b65d8d31c0f50a89582234d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>At25040Handler</name>
    <filename>classAt25040Handler.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>ProbeResult</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>a1e9c45fb86ca885b999afc9da9c2113b</anchor>
      <arglist></arglist>
      <enumvalue file="classAt25040Handler.html" anchor="a1e9c45fb86ca885b999afc9da9c2113ba018ef9aed10602379367b66bcfdc68b3">NotBound</enumvalue>
      <enumvalue file="classAt25040Handler.html" anchor="a1e9c45fb86ca885b999afc9da9c2113bae384f071109188762a0f4349e8a38a88">Absent</enumvalue>
      <enumvalue file="classAt25040Handler.html" anchor="a1e9c45fb86ca885b999afc9da9c2113badd058df87f5c88e3285a28ad7406a3c6">Present</enumvalue>
    </member>
    <member kind="function">
      <type></type>
      <name>At25040Handler</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>a323c5cc513d3b9b1f4ec2be48d1c7cf9</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio *wp=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type>ProbeResult</type>
      <name>Probe</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>a4e484ec3fc161003669105c97a3e54ed</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>LastRdsr</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>ac537e02650383ae9d8bbe6db5ffb8420</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Read</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>a896f0674cb1e585b54b3ff1b2bfb4c65</anchor>
      <arglist>(uint16_t addr, uint8_t *dst, uint16_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Write</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>adacab835d239d5e7e064203ba37a80bd</anchor>
      <arglist>(uint16_t addr, const uint8_t *src, uint16_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ReleaseWriteProtect</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>a8a4d5d20e572558e8f8bd269182ba904</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ParkWriteProtect</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>a477e2648be5bf82bd63fac10df1205ce</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>At25040SpiAdapter</type>
      <name>adapter_</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>af2dff8321e18958bdbdbbb0162c302dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>at25040::Device&lt; At25040SpiAdapter &gt;</type>
      <name>device_</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>a269fbccb881206df93e069f0dbad823f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>wp_</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>a4a460cac738701db1b21e34900d5dd91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>mutex_</name>
      <anchorfile>classAt25040Handler.html</anchorfile>
      <anchor>a5e548139f0497efc98f3aa52ce4db71c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>At25040SpiAdapter</name>
    <filename>classAt25040SpiAdapter.html</filename>
    <member kind="function">
      <type></type>
      <name>At25040SpiAdapter</name>
      <anchorfile>classAt25040SpiAdapter.html</anchorfile>
      <anchor>a0963d2b9ce03f67b7dc063f4b551f5d2</anchor>
      <arglist>(BaseSpi &amp;spi) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>transfer</name>
      <anchorfile>classAt25040SpiAdapter.html</anchorfile>
      <anchor>aead8069f1a1130d982f7182b67e4035d</anchor>
      <arglist>(const uint8_t *tx, uint8_t *rx, std::size_t len, uint32_t timeout_ms)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_</name>
      <anchorfile>classAt25040SpiAdapter.html</anchorfile>
      <anchor>ae13e11f5acf0a278105704fa79460f4f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Bmp581Handler</name>
    <filename>classBmp581Handler.html</filename>
    <class kind="struct">Bmp581Handler::Counters</class>
    <member kind="typedef">
      <type>bmp581::Driver&lt; HalI2cBmp581Comm &gt;</type>
      <name>DriverType</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a180671d596aa2e44c63d3ac189fee647</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Bmp581Handler</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a5dd617fce8c7ab099796915b732f85a6</anchor>
      <arglist>(BaseI2c &amp;i2c_device, const Bmp581HandlerConfig &amp;config=Bmp581HandlerConfig{}, RtosMutex *bus_mutex=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Bmp581Handler</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a1995e6951aaac833d1e8b81345e0f199</anchor>
      <arglist>(const Bmp581Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Bmp581Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a903f03b7032d5991bc60cf5e6152bdd9</anchor>
      <arglist>(const Bmp581Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Bmp581Handler</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>aaf7203160d5c4d6f136c93b961c948d0</anchor>
      <arglist>(Bmp581Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Bmp581Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a282c477c65f925dad7fcb6aab608d93e</anchor>
      <arglist>(Bmp581Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a10e540b5919ce9acd2dd26d6dadc2e3c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPresent</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>abfb530dbe365f35f8d06e72fc2c9f6c0</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::uint8_t</type>
      <name>ChipId</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a200f8a76037be353ba1bd4a5d32d99f8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const bmp581::EffectiveOsr &amp;</type>
      <name>EffectiveOsr</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>ad7f70986e23ccbfddcda3f316a8bb939</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bmp581::DriverResult&lt; bool &gt;</type>
      <name>DataReady</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a028408cddf1032615dea2360a17077c9</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bmp581::DriverResult&lt; bmp581::Sample &gt;</type>
      <name>ReadSample</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>ae2e1cbd247caa1534d166895d6773b8d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bmp581::DriverResult&lt; void &gt;</type>
      <name>Configure</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a83f83d6a3aa3a3f556067632753b65c8</anchor>
      <arglist>(const bmp581::Config &amp;cfg) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bmp581::DriverResult&lt; void &gt;</type>
      <name>Reset</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a44247674d6ae56480d2c6aad81c362c0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>Counters</type>
      <name>GetCounters</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>ac6eb0f2bcec58254dfc9c6d15991d455</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a019e9a1b2ee7981b98eae2c281592837</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Bmp581HandlerConfig</type>
      <name>config_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a2e2146a442579f4783a225968111dac9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>HalI2cBmp581Comm</type>
      <name>comm_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>aa40022daa6755155db79eb12af30aa3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; DriverType &gt;</type>
      <name>driver_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a8515143e52b59a05b4dfb7cb3ceb6c27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::uint8_t</type>
      <name>chip_id_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>ad35951d9a7a216590397f729d31f9db5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bmp581::EffectiveOsr</type>
      <name>effective_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a2df12883b37ae19ecfe31a28e3622c8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; bool &gt;</type>
      <name>initialized_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a987536aaa20597d578981549999cf871</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; std::uint32_t &gt;</type>
      <name>samples_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a1d65775d6ed6196a09e105f1072c5bfd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; std::uint32_t &gt;</type>
      <name>bus_errors_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>aa5b96ad2d44bd8090f983bd52799cfa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>private_mutex_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a2210f284140ff45a4af3080582b215f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex *</type>
      <name>bus_mutex_</name>
      <anchorfile>classBmp581Handler.html</anchorfile>
      <anchor>a13ed6424cdd6d34f175c30ea74ce4750</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Bmp581HandlerConfig</name>
    <filename>structBmp581HandlerConfig.html</filename>
    <member kind="variable">
      <type>bmp581::Config</type>
      <name>config</name>
      <anchorfile>structBmp581HandlerConfig.html</anchorfile>
      <anchor>aac5b5d48f3f11939f5cb916ce8890baa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>soft_reset_on_init</name>
      <anchorfile>structBmp581HandlerConfig.html</anchorfile>
      <anchor>aacb36b33a181ec2819c249efec03563b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint8_t</type>
      <name>device_index</name>
      <anchorfile>structBmp581HandlerConfig.html</anchorfile>
      <anchor>aa0c1a1f817fba5ab054ad64f646d8408</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Bno08xActivityData</name>
    <filename>structBno08xActivityData.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>tap_detected</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>a20ff948551c4991bfafdf5057360c734</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>double_tap</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>a80f5838ee8748e4b3f6de39cebd990b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>tap_direction</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>a22e19399a211fca7fd5594e7f8398d2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>step_count</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>a7272bb2b981d3ead3f826cf7eb2c0dc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>step_detected</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>a554f7cf1ce6d617a05e6357f34760825</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>shake_detected</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>ae6cb112a5271167eecb90f73e75f2a1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>pickup_detected</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>a508cd74ccb7e95beddd7634143841593</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>significant_motion</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>a4622038469ea55dfadafb46a302df001</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>stability_class</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>aa1d976419c7241f5c47be2bf71f4413e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>timestamp_us</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>ab3246a8a87f53e5392b8db45f3f8d62e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>structBno08xActivityData.html</anchorfile>
      <anchor>ab826f581678b48deb188caffc9924a12</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Bno08xCalibrationStatus</name>
    <filename>structBno08xCalibrationStatus.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>accelerometer_accuracy</name>
      <anchorfile>structBno08xCalibrationStatus.html</anchorfile>
      <anchor>a115a17c08837ca2ac2512c3e7816c5e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>gyroscope_accuracy</name>
      <anchorfile>structBno08xCalibrationStatus.html</anchorfile>
      <anchor>ad8fbed7b8273a998f3ce83c4f61e65bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>magnetometer_accuracy</name>
      <anchorfile>structBno08xCalibrationStatus.html</anchorfile>
      <anchor>a3d84d0ced6d22135f0654d2ba2a3bbe6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>rotation_accuracy</name>
      <anchorfile>structBno08xCalibrationStatus.html</anchorfile>
      <anchor>a377e8ca2f2ce5d05dc035906ace9be47</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>fully_calibrated</name>
      <anchorfile>structBno08xCalibrationStatus.html</anchorfile>
      <anchor>a1682b52f8db214f1d43c3a249843f765</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Bno08xConfig</name>
    <filename>structBno08xConfig.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>enable_accelerometer</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>ac70b68e23d71e1d57a0159a90b4cf025</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_gyroscope</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a288386b0400202469a3657b3d7c9b38e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_magnetometer</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>af600304829cef3d3e2607651e06317af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_rotation_vector</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a29ea05f211972184d28f639041890406</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_linear_acceleration</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a57bec720b6335b24433f012fbf1ee8cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_gravity</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a39f6c40ee3eb9da203c955ed49a5029f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_game_rotation</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>aabfdb780ca1451ad524e393d3cb003a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_tap_detector</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>af7f0fc07e1c3159c3732dec4ac388169</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_step_counter</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>af8347445c58adc10a534ececc771e491</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_step_detector</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>ade52090a60a126df87ea8ee684c10308</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_shake_detector</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>aae96a9cf2ef13c48c81753610b740432</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_pickup_detector</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a1cd7b744e9d407cd6e0af992f5fe3d05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_significant_motion</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a0cd624ccac722809d72a994bc808747b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_stability_classifier</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a67da8bbff3cb8be3f80a7802cec1ad8b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>accelerometer_interval_ms</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>adf9ae4c85ad6ea0296023947c0611248</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>gyroscope_interval_ms</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a927d6555fa3e3c4418afae4f3b3df3cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>magnetometer_interval_ms</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>acc63542b675482d61aa2709337e300ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>rotation_interval_ms</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>aa4a54d0b30c6b17e16d9a0e455a3cd13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>linear_accel_interval_ms</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>abf99eb8ed74e78b66605a56d1be674b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>gravity_interval_ms</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a83422ed8164bc618ec23072faf5ac31a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>game_rotation_interval_ms</name>
      <anchorfile>structBno08xConfig.html</anchorfile>
      <anchor>a1e6923fc6542b050c2ab4fdc587d55bb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Bno08xDriverImpl</name>
    <filename>classBno08xDriverImpl.html</filename>
    <templarg>typename CommType</templarg>
    <base>IBno08xDriverOps</base>
    <member kind="function">
      <type></type>
      <name>Bno08xDriverImpl</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a9be86b521fabd5745ccd12f0eb69919a</anchor>
      <arglist>(CommType &amp;&amp;comm) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Begin</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>ac89c44db7975c4683f896b85f519ea62</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Update</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a6ad40347e415a1524b585498c87b4233</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnableSensor</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a8af2b41d7ac18e7fc1b3cf6c777959c2</anchor>
      <arglist>(BNO085Sensor sensor, uint32_t interval_ms, float sensitivity) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>DisableSensor</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a92a939a92cd8823c217526efa265d032</anchor>
      <arglist>(BNO085Sensor sensor) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetCallback</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a4385462cf9a4d719cb8fe2f03ab54255</anchor>
      <arglist>(SensorCallback cb) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HasNewData</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a3a097f73569d05c185c2d1d76bc7039b</anchor>
      <arglist>(BNO085Sensor sensor) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>SensorEvent</type>
      <name>GetLatest</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a7fd357181fef0ce4c845df160ae92f7a</anchor>
      <arglist>(BNO085Sensor sensor) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>GetLastError</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>aa4da595e6a38a1f0aa3ce652672ca574</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>HardwareReset</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a438c41d8c26b4e4dccf60079d8861043</anchor>
      <arglist>(uint32_t lowMs) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetBootPin</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a6f6bedc4fd3fb4553ce90cf297ca1f6a</anchor>
      <arglist>(bool state) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetWakePin</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a5f185e438bb8ffed4249253427883ce8</anchor>
      <arglist>(bool state) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SelectInterface</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>adc64b928a7b03b38602c755bfc03bd37</anchor>
      <arglist>(BNO085Interface iface) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>BNO085Interface</type>
      <name>GetInterfaceType</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>aefa8b67017e373c9fbdc5d00a882a58f</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>CommType</type>
      <name>comm_</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>aeb6cd5bfef51dbe8df177de30ab1dec2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BNO085&lt; CommType &gt;</type>
      <name>driver_</name>
      <anchorfile>classBno08xDriverImpl.html</anchorfile>
      <anchor>a43baa8985ed295129a8793e08681d372</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Bno08xEulerAngles</name>
    <filename>structBno08xEulerAngles.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>roll</name>
      <anchorfile>structBno08xEulerAngles.html</anchorfile>
      <anchor>a8f2e2420f6aaed647d9107741e59fe7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>pitch</name>
      <anchorfile>structBno08xEulerAngles.html</anchorfile>
      <anchor>ae2dba92a3a192db0b30c290fed76934d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>yaw</name>
      <anchorfile>structBno08xEulerAngles.html</anchorfile>
      <anchor>a1de95942c1e2a98e063d697dc558c774</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>accuracy</name>
      <anchorfile>structBno08xEulerAngles.html</anchorfile>
      <anchor>a4152f00eb49e3dd98271efa1a52028a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>timestamp_us</name>
      <anchorfile>structBno08xEulerAngles.html</anchorfile>
      <anchor>ad70e7cd8eff9a25f037edc0b33280a2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>structBno08xEulerAngles.html</anchorfile>
      <anchor>a8241a46cf562622226d491e1c1ac62bd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Bno08xHandler</name>
    <filename>classBno08xHandler.html</filename>
    <member kind="function">
      <type></type>
      <name>Bno08xHandler</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a23386de466024b4809f2bfe42fa76c67</anchor>
      <arglist>(BaseI2c &amp;i2c_device, const Bno08xConfig &amp;config=Bno08xConfig{}, BaseGpio *reset_gpio=nullptr, BaseGpio *int_gpio=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Bno08xHandler</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a801b02378a8bccc4a20a9af3697e8222</anchor>
      <arglist>(BaseSpi &amp;spi_device, const Bno08xConfig &amp;config=Bno08xConfig{}, BaseGpio *reset_gpio=nullptr, BaseGpio *int_gpio=nullptr, BaseGpio *wake_gpio=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Bno08xHandler</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a4bdec4a51e6c90b2354db27fbc8b3251</anchor>
      <arglist>() noexcept=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Bno08xHandler</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a6b2e5e14222d04f15591b31b319bb605</anchor>
      <arglist>(const Bno08xHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Bno08xHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>ab8262b5c735887b095013d053acf7cc8</anchor>
      <arglist>(const Bno08xHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Bno08xHandler</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>acf9772bf644ddfeff83371740dd6216b</anchor>
      <arglist>(Bno08xHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Bno08xHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a3116a61b5546a29048810a6849542438</anchor>
      <arglist>(Bno08xHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Bno08xError</type>
      <name>Initialize</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>ac1a1ff0d56324dcc09d9b48ca2e132e6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>afab341345c8ed8fed1a218e0df18c4aa</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>Bno08xError</type>
      <name>Deinitialize</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a2019941b7d9fa00ffe08630fee9ef340</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>abdc987c8440f4a2394ae861a40766e10</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>Bno08xError</type>
      <name>Update</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a74cdf0328224c1a3fad944bbf1312e7e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetSensorCallback</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>ac6c4372bc30b608206070bcf96723b2d</anchor>
      <arglist>(SensorCallback callback) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ClearSensorCallback</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a86c30e163a55db1e1dd3f1ce30635239</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>BNO085Interface</type>
      <name>GetInterfaceType</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a88213b7fef4820d06b5972fac3702715</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>IBno08xDriverOps *</type>
      <name>GetSensor</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a57e2952c0438e800c091cd0e3ebcdaf5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const IBno08xDriverOps *</type>
      <name>GetSensor</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>abf3a81e646ebd6dd0a10890e5be21d6d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>IBno08xDriverOps *</type>
      <name>GetDriver</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a7872446e9f4256f1d606b79dde170170</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const IBno08xDriverOps *</type>
      <name>GetDriver</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a61a4b3f8422778536f2c23e07cadc60b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>ac1692884d36e285a1f4e4cf2968f46bc</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept -&gt; decltype(fn(std::declval&lt; IBno08xDriverOps &amp; &gt;()))</arglist>
    </member>
    <member kind="function">
      <type>Bno08xError</type>
      <name>GetLastError</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>abf7206fa6caf5e8cf5132b02cd7a62bc</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>GetLastDriverError</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a0f3b0686f5cbab0f7af7690d188c89b8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>ab349a1edff7b5d84230704274bb3d966</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a58384d1dfa272ba616140abb9fa55c2b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>QuaternionToEuler</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>ac08bd345f853529de4448205be64cafb</anchor>
      <arglist>(const Bno08xQuaternion &amp;quaternion, Bno08xEulerAngles &amp;euler_angles) noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Bno08xConfig</type>
      <name>GetDefaultConfig</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a7ab2e3756b7a6e17879bc95ba2125e8b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>ensureInitializedLocked</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a0479abc034d5417ba7dc524468a5ce82</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>applyConfigLocked</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a5669d8d3146a450430e5644b8d297de5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static Bno08xError</type>
      <name>mapDriverError</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a6e91ad7cdb2e1bb9a57eb797cf7d8e96</anchor>
      <arglist>(int sh2_error) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; IBno08xDriverOps &gt;</type>
      <name>driver_ops_</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>af43f44ede1cc408c5141588a4b965fea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Bno08xConfig</type>
      <name>config_</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a04876261e622b918bd759f042ce025fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>aa17448183bf8a9205830538ce86a3e1a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a4f2e5fdd78f9f3fbb737bed060478287</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Bno08xError</type>
      <name>last_error_</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>ae17c70536ddb4a4ae8931807347c9001</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BNO085Interface</type>
      <name>interface_type_</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a6a6864399d79e0363da35bd3e684c123</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SensorCallback</type>
      <name>user_callback_</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>ab4be91e6bd9492c1824aa747db733c0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classBno08xHandler.html</anchorfile>
      <anchor>a8a2f0bb1bca336b49c6292c10475f64d</anchor>
      <arglist>[64]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Bno08xImuData</name>
    <filename>structBno08xImuData.html</filename>
    <member kind="variable">
      <type>Bno08xVector3</type>
      <name>acceleration</name>
      <anchorfile>structBno08xImuData.html</anchorfile>
      <anchor>a5ed6cd044cac92af0fc8b116a0781379</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bno08xVector3</type>
      <name>gyroscope</name>
      <anchorfile>structBno08xImuData.html</anchorfile>
      <anchor>a2dc11996fa80be3f66f41574312a59a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bno08xVector3</type>
      <name>magnetometer</name>
      <anchorfile>structBno08xImuData.html</anchorfile>
      <anchor>a615841bc51167785bedcc6fac397fbee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bno08xVector3</type>
      <name>linear_acceleration</name>
      <anchorfile>structBno08xImuData.html</anchorfile>
      <anchor>ae1ffba7f21e424be299561439af362a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bno08xVector3</type>
      <name>gravity</name>
      <anchorfile>structBno08xImuData.html</anchorfile>
      <anchor>a29e8f558918db6c974797c3c25c2926f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bno08xQuaternion</type>
      <name>rotation</name>
      <anchorfile>structBno08xImuData.html</anchorfile>
      <anchor>adad533d11218564196c5183f128f1dfc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Bno08xEulerAngles</type>
      <name>euler</name>
      <anchorfile>structBno08xImuData.html</anchorfile>
      <anchor>ae8c416d04df52796b7fcc9843d81cb49</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>timestamp_us</name>
      <anchorfile>structBno08xImuData.html</anchorfile>
      <anchor>a6195cdab3f19277e84307ab33e1558d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>structBno08xImuData.html</anchorfile>
      <anchor>ad8851df86152f1bc6fbbb206dd125237</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Bno08xQuaternion</name>
    <filename>structBno08xQuaternion.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>w</name>
      <anchorfile>structBno08xQuaternion.html</anchorfile>
      <anchor>aeecd4137ac0047a98dfbd2269e5838d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>x</name>
      <anchorfile>structBno08xQuaternion.html</anchorfile>
      <anchor>a412bb1d189f6cdff590a6bc5b2cfc065</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>y</name>
      <anchorfile>structBno08xQuaternion.html</anchorfile>
      <anchor>aab7982e2134475343faea970b1f6afa2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>z</name>
      <anchorfile>structBno08xQuaternion.html</anchorfile>
      <anchor>aec9e624e59e4cdcd0e768f14da2ecafd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>accuracy</name>
      <anchorfile>structBno08xQuaternion.html</anchorfile>
      <anchor>a9ad791e995d643d781c6e395fa5f9dab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>timestamp_us</name>
      <anchorfile>structBno08xQuaternion.html</anchorfile>
      <anchor>aa7e26a46c3faff01aa13e1ebe2e0ec3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>structBno08xQuaternion.html</anchorfile>
      <anchor>a50e1eb9efb42df59e11b88f8170ed78d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Bno08xVector3</name>
    <filename>structBno08xVector3.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>x</name>
      <anchorfile>structBno08xVector3.html</anchorfile>
      <anchor>a0d0ccabd855cb598686661948b764128</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>y</name>
      <anchorfile>structBno08xVector3.html</anchorfile>
      <anchor>aa93979daeda26945dd822e1c0aa89168</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>z</name>
      <anchorfile>structBno08xVector3.html</anchorfile>
      <anchor>aed0b778674a5081eb4d5fb5b0847a07f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>accuracy</name>
      <anchorfile>structBno08xVector3.html</anchorfile>
      <anchor>aea9641493afa37905bf389ce60b510ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>timestamp_us</name>
      <anchorfile>structBno08xVector3.html</anchorfile>
      <anchor>a163c181287c91298c912e15cd0ea5f80</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>structBno08xVector3.html</anchorfile>
      <anchor>a91e7e778b15e406c496171f661bd74fb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>CanOpenBaseCanLink</name>
    <filename>classCanOpenBaseCanLink.html</filename>
    <member kind="function">
      <type></type>
      <name>CanOpenBaseCanLink</name>
      <anchorfile>classCanOpenBaseCanLink.html</anchorfile>
      <anchor>a38a339f8db663e7aaa7a93a065298eef</anchor>
      <arglist>(BaseCan &amp;can) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Open</name>
      <anchorfile>classCanOpenBaseCanLink.html</anchorfile>
      <anchor>a1b6f8d617bf69586c623ce08187d842b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Close</name>
      <anchorfile>classCanOpenBaseCanLink.html</anchorfile>
      <anchor>a68a3847e8acd0e66884487b625204d3f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Write</name>
      <anchorfile>classCanOpenBaseCanLink.html</anchorfile>
      <anchor>a4c2332c81264e6dc52e9043ce62519f6</anchor>
      <arglist>(const CanOpen::CanFrame &amp;f) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Read</name>
      <anchorfile>classCanOpenBaseCanLink.html</anchorfile>
      <anchor>a3f41c2bb8730aebd02ce0671e9455344</anchor>
      <arglist>(CanOpen::CanFrame &amp;f, int timeoutMs) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>HfUtilsCanOpenTransport</type>
      <name>transport_</name>
      <anchorfile>classCanOpenBaseCanLink.html</anchorfile>
      <anchor>af2aea9c52f9c8ac9fb65b184aacb2fa4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CO_CANmodule_t</name>
    <filename>structCO__CANmodule__t.html</filename>
    <member kind="variable">
      <type>void *</type>
      <name>CANptr</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>aad1c18f6d47621e5a611333dc57cb349</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CO_CANrx_t *</type>
      <name>rxArray</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>a747e694e15cca5a5579c2c4397a6da39</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>rxSize</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>a88edfd32c2bff5b9f29a2bccd1ac96f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CO_CANtx_t *</type>
      <name>txArray</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>af623f2a045716af77e906b28deee3d3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>txSize</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>a6a53d3d12efbe30d1ee08e821d1b4139</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>CANerrorStatus</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>a5757052c57726cb4fb2ac5cdd8ff744d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile bool_t</type>
      <name>CANnormal</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>a5555f7d10e09da5815526d7c8b10901d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile bool_t</type>
      <name>useCANrxFilters</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>a9b28f7a6f02d398b3a0ea6cf70fa64f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile bool_t</type>
      <name>bufferInhibitFlag</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>ad7af19de0bf39c8927c926b095cb5292</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile bool_t</type>
      <name>firstCANtxMessage</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>ac12bd37971b89b5796fec0de71be0f07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint16_t</type>
      <name>CANtxCount</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>a269294aa6a15ba56f92a460fae0536ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>errOld</name>
      <anchorfile>structCO__CANmodule__t.html</anchorfile>
      <anchor>aa0627988ddedc3a4ec3bbfcf3b818d06</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CO_CANrx_t</name>
    <filename>structCO__CANrx__t.html</filename>
    <member kind="variable">
      <type>uint16_t</type>
      <name>ident</name>
      <anchorfile>structCO__CANrx__t.html</anchorfile>
      <anchor>a8595c238cf0364bde995dee97d321909</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>mask</name>
      <anchorfile>structCO__CANrx__t.html</anchorfile>
      <anchor>af7a48dd4ac895a19c4031038e2c1222d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>object</name>
      <anchorfile>structCO__CANrx__t.html</anchorfile>
      <anchor>a957a1ce67cd1d9010889d557bf0c5770</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void(*</type>
      <name>CANrx_callback</name>
      <anchorfile>structCO__CANrx__t.html</anchorfile>
      <anchor>ab2f19f6143d96c3235a4d07077e0db14</anchor>
      <arglist>)(void *object, void *message)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CO_CANrxMsg_t</name>
    <filename>structCO__CANrxMsg__t.html</filename>
    <member kind="variable">
      <type>uint16_t</type>
      <name>ident</name>
      <anchorfile>structCO__CANrxMsg__t.html</anchorfile>
      <anchor>aea295e1f7c718732dbcff3cb1b6f414d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>dlc</name>
      <anchorfile>structCO__CANrxMsg__t.html</anchorfile>
      <anchor>a5f20f2612ff37d9d71e8600157ae9855</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>data</name>
      <anchorfile>structCO__CANrxMsg__t.html</anchorfile>
      <anchor>a5703337fe56f58d56caf9a6c4ab400c8</anchor>
      <arglist>[8]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CO_CANtx_t</name>
    <filename>structCO__CANtx__t.html</filename>
    <member kind="variable">
      <type>uint32_t</type>
      <name>ident</name>
      <anchorfile>structCO__CANtx__t.html</anchorfile>
      <anchor>a9cc2687eb11da14d4c0aa167352c635c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>DLC</name>
      <anchorfile>structCO__CANtx__t.html</anchorfile>
      <anchor>a9bb96d60314283061f7619e36d870fa0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>data</name>
      <anchorfile>structCO__CANtx__t.html</anchorfile>
      <anchor>aae5bcdc2296a5d1d53a3105e86dbb66d</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable">
      <type>volatile bool_t</type>
      <name>bufferFull</name>
      <anchorfile>structCO__CANtx__t.html</anchorfile>
      <anchor>a305f0687a4ed7cd533e7937d6ff7d31b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile bool_t</type>
      <name>syncFlag</name>
      <anchorfile>structCO__CANtx__t.html</anchorfile>
      <anchor>a79c19597a51351b9d6ed1c9bdfd051b3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CO_storage_entry_t</name>
    <filename>structCO__storage__entry__t.html</filename>
    <member kind="variable">
      <type>void *</type>
      <name>addr</name>
      <anchorfile>structCO__storage__entry__t.html</anchorfile>
      <anchor>a44ebb3251846942e6da56b5e6f973ee0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>len</name>
      <anchorfile>structCO__storage__entry__t.html</anchorfile>
      <anchor>a7c4b9052f381bf5c1a7f6ffea3ce551c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>subIndexOD</name>
      <anchorfile>structCO__storage__entry__t.html</anchorfile>
      <anchor>a454596a1e6f6ebbbb31b5207ea2f3bbc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>attr</name>
      <anchorfile>structCO__storage__entry__t.html</anchorfile>
      <anchor>a0275cc193406d0ec3a931d953edf5f1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>addrNV</name>
      <anchorfile>structCO__storage__entry__t.html</anchorfile>
      <anchor>aa11430a8cb523612951d9219a891f26e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Ws2812Handler::Config</name>
    <filename>structWs2812Handler_1_1Config.html</filename>
    <member kind="variable">
      <type>gpio_num_t</type>
      <name>gpio_pin</name>
      <anchorfile>structWs2812Handler_1_1Config.html</anchorfile>
      <anchor>acfd7c925abeb4af12fbddd986082d66f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>num_leds</name>
      <anchorfile>structWs2812Handler_1_1Config.html</anchorfile>
      <anchor>a7506ba4e8c14cf26a114594cb77357bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>LedType</type>
      <name>led_type</name>
      <anchorfile>structWs2812Handler_1_1Config.html</anchorfile>
      <anchor>a241ed81954221fa96164b11099439d44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>brightness</name>
      <anchorfile>structWs2812Handler_1_1Config.html</anchorfile>
      <anchor>a34b3e52bebb8a93793f35f0cd5b551c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>rmt_channel</name>
      <anchorfile>structWs2812Handler_1_1Config.html</anchorfile>
      <anchor>a1f59004470961fc7f42f5d416f02654d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>t0h</name>
      <anchorfile>structWs2812Handler_1_1Config.html</anchorfile>
      <anchor>abbb51d05f423a9b8ccb820c8f416b5c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>t1h</name>
      <anchorfile>structWs2812Handler_1_1Config.html</anchorfile>
      <anchor>aa0a0d32c028be876a8da5c962c519d71</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>t0l</name>
      <anchorfile>structWs2812Handler_1_1Config.html</anchorfile>
      <anchor>ab38c01cedf6037e1d99df6e002dbbd0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>t1l</name>
      <anchorfile>structWs2812Handler_1_1Config.html</anchorfile>
      <anchor>a15159e3080965a04ac08325c18ddd435</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Bmp581Handler::Counters</name>
    <filename>structBmp581Handler_1_1Counters.html</filename>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>samples</name>
      <anchorfile>structBmp581Handler_1_1Counters.html</anchorfile>
      <anchor>abf2bdf861e59099f97bb94e3653f285a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>bus_errors</name>
      <anchorfile>structBmp581Handler_1_1Counters.html</anchorfile>
      <anchor>a599076957c50611e9b6b89b33f982324</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SfmHandler::Counters</name>
    <filename>structSfmHandler_1_1Counters.html</filename>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>frames</name>
      <anchorfile>structSfmHandler_1_1Counters.html</anchorfile>
      <anchor>a5cd6ab0b01ba68efc4f06e68f87a883a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>no_data</name>
      <anchorfile>structSfmHandler_1_1Counters.html</anchorfile>
      <anchor>a657999182e9a1425229b95dea735548c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>crc_errors</name>
      <anchorfile>structSfmHandler_1_1Counters.html</anchorfile>
      <anchor>a7d6b66f4db6c27864b63b3bd95594319</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>bus_errors</name>
      <anchorfile>structSfmHandler_1_1Counters.html</anchorfile>
      <anchor>ab5319ffce72dabd69baf0dcc67955d73</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AlicatBasis2Handler::DiscoveredDevice</name>
    <filename>structAlicatBasis2Handler_1_1DiscoveredDevice.html</filename>
    <member kind="variable">
      <type>std::uint8_t</type>
      <name>address</name>
      <anchorfile>structAlicatBasis2Handler_1_1DiscoveredDevice.html</anchorfile>
      <anchor>a642f39531c659a5ba56bd33ca93947fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>baud_bps</name>
      <anchorfile>structAlicatBasis2Handler_1_1DiscoveredDevice.html</anchorfile>
      <anchor>a7f7b5b675e207b5d3f2604b73ad5c49a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DynamixelHandler</name>
    <filename>classDynamixelHandler.html</filename>
    <member kind="function">
      <type></type>
      <name>DynamixelHandler</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a080a43ba3044087a9d86f1064cd835fa</anchor>
      <arglist>(BaseUart &amp;uart, const DynamixelHandlerConfig &amp;config=DynamixelHandlerConfig{}, RtosMutex *bus_mutex=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DynamixelHandler</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a7604c2c7d772633ec5356bde3b44a323</anchor>
      <arglist>(const DynamixelHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>DynamixelHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>af1dfd79268a8bb89d02ad020e86e0532</anchor>
      <arglist>(const DynamixelHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DynamixelHandler</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a51c0fe23283a8902fcf3de4e76b9b4a5</anchor>
      <arglist>(DynamixelHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>DynamixelHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a5bc152eb52010f399a213a4abd5d72db</anchor>
      <arglist>(DynamixelHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>ab84a7dc316d9601f9fdb3ea3234f340f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPresent</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a2b03b112f2d9d03b693491e25de93cfa</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const dynamixel::PingInfo &amp;</type>
      <name>Identity</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a368ed2489c5ba7fa145af77ef651ffd8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const dynamixel::ModelDescriptor *</type>
      <name>Model</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>aa208470b755c2182f5cea1728a4e4ccc</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>BindModel</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a26e2da409698d0fcfaaf8f679bd21aa8</anchor>
      <arglist>(const dynamixel::ModelDescriptor &amp;desc) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>BindXSeriesFallback</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a9d5db8ea2b452dc900339357b22fb401</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverResult&lt; dynamixel::PingInfo &gt;</type>
      <name>Ping</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a66dce8a1b2cec2f079881cd124dcfbbe</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverResult&lt; uint8_t &gt;</type>
      <name>Scan</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a2128f421198dccaa0aafa4728636d6a2</anchor>
      <arglist>(uint8_t *out_ids, uint8_t max_ids, uint8_t first_id=1, uint8_t last_id=dynamixel::kMaxDeviceId) noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverResult&lt; void &gt;</type>
      <name>SetTorqueEnabled</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a8609e29b92308c2bfdab08f5917f7a9d</anchor>
      <arglist>(bool enabled) noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverResult&lt; void &gt;</type>
      <name>SetOperatingMode</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>ab596bcf7270cc394d4ae8d78f7a4cc0d</anchor>
      <arglist>(dynamixel::OperatingMode mode) noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverResult&lt; void &gt;</type>
      <name>SetMotionProfile</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>adb410602a15f67dfeb49183e50465c21</anchor>
      <arglist>(uint32_t profile_acceleration, uint32_t profile_velocity) noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverResult&lt; void &gt;</type>
      <name>SetGoalPosition</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a24723a575ba957e9e12c4019e47c5f45</anchor>
      <arglist>(int32_t ticks) noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverResult&lt; void &gt;</type>
      <name>SetGoalVelocity</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a94593d7f8581341e8e7624f1a23e2a89</anchor>
      <arglist>(int32_t ticks_per_sec) noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverResult&lt; int32_t &gt;</type>
      <name>ReadPosition</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a8bfe87ccbb85c268c054f7e71d461256</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverResult&lt; dynamixel::Telemetry &gt;</type>
      <name>ReadTelemetry</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a03c3e5420dc51a9ac2c5e2c0246de693</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::Bus &amp;</type>
      <name>bus</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>ad7b887647e795ff9b462646586e3562d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::Device &amp;</type>
      <name>device</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a0e0e02b158a4b75c4bdc0b335f9ab326</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDevice</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a27d8d94c66ced1cf9d0102f55cb7884b</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>acf7ca14bc4aa7bfa8de08921c45426da</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>DynamixelHandlerConfig</type>
      <name>config_</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a995917590eed30c3dd6b2ec8c8963f45</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>HalUartDynamixelComm</type>
      <name>comm_</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>aea14d2524ec961ba16e2083935e8638a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>dynamixel::Bus</type>
      <name>bus_</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a7ea2989e2a12d5507fad2d1d167c49b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>dynamixel::Device</type>
      <name>device_</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a72508fb118d8a60ebc4e1fc042f4a299</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>dynamixel::PingInfo</type>
      <name>identity_</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>aec674e26b5be35f52f79e3dfe066f0b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; bool &gt;</type>
      <name>initialized_</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a175df5b281204750b1f97c3e84262a9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>private_mutex_</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a897f38c972dce7ae04a62f74314f5f05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex *</type>
      <name>bus_mutex_</name>
      <anchorfile>classDynamixelHandler.html</anchorfile>
      <anchor>a264cfd1ac8bc6711e0cf6e9158895841</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>DynamixelHandlerConfig</name>
    <filename>structDynamixelHandlerConfig.html</filename>
    <member kind="variable">
      <type>std::uint8_t</type>
      <name>servo_id</name>
      <anchorfile>structDynamixelHandlerConfig.html</anchorfile>
      <anchor>acc41a309aa781143630c835e999511fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>dynamixel::TransportConfig</type>
      <name>transport</name>
      <anchorfile>structDynamixelHandlerConfig.html</anchorfile>
      <anchor>a5259101cfa8bf681c79122eafa80434c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>bind_xseries_fallback</name>
      <anchorfile>structDynamixelHandlerConfig.html</anchorfile>
      <anchor>a5574732ac2ae297955aaa2bd154cb764</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint8_t</type>
      <name>device_index</name>
      <anchorfile>structDynamixelHandlerConfig.html</anchorfile>
      <anchor>a1163572c3a838985c3297d6d73b5ecd8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Fdo2Handler</name>
    <filename>classFdo2Handler.html</filename>
    <member kind="typedef">
      <type>fdo2::Driver&lt; HalUartFdo2Comm &gt;</type>
      <name>DriverType</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a32c49b5c3217e4b30c4da75a41a0c566</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Fdo2Handler</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a0492d72319d63ebb9524a1fbb5e88b50</anchor>
      <arglist>(BaseUart &amp;uart, const Fdo2HandlerConfig &amp;config=Fdo2HandlerConfig{}, RtosMutex *bus_mutex=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Fdo2Handler</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>ac3790762d884902d4581a9edf907f879</anchor>
      <arglist>(const Fdo2Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Fdo2Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a45471e3b27d0d35ae10bd34b7f084437</anchor>
      <arglist>(const Fdo2Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Fdo2Handler</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>ad3089dec29c8bda0eaef563ddfa47c04</anchor>
      <arglist>(Fdo2Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Fdo2Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a3947b997be086680a99e00635f577ed2</anchor>
      <arglist>(Fdo2Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a36795f24ed679bf45cc5de809fb9a9d8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPresent</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>af7e63db9b8d3336349865b7feb26db91</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const fdo2::VersionInfo &amp;</type>
      <name>Identity</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a7098532a55876168243f0b7c9872e031</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>fdo2::DriverResult&lt; fdo2::MoxyReading &gt;</type>
      <name>MeasureMoxy</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a99bba6021370bbd0dcf7b86f8f4efb4e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>fdo2::DriverResult&lt; fdo2::MrawReading &gt;</type>
      <name>MeasureMraw</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a8d1c48731fcc63071c675fae4e566985</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>fdo2::DriverResult&lt; fdo2::VersionInfo &gt;</type>
      <name>ReadVersion</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>acab9a5af909302da9e5268687d8a8975</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>fdo2::DriverResult&lt; std::uint64_t &gt;</type>
      <name>ReadUniqueId</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a9ca814fc66a1d02cb0b3bfb7879e9f8c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a78beeff055bbfb7035037778f87998fa</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Fdo2HandlerConfig</type>
      <name>config_</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>ac95137ebeff168ccdfab3b56aa672621</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>HalUartFdo2Comm</type>
      <name>comm_</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a542606a8d3bfd958d8d200aa20a66e2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; DriverType &gt;</type>
      <name>driver_</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a49c5e0ccf60eeb58f96ebdb601a3e478</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>fdo2::VersionInfo</type>
      <name>identity_</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>abde00731254cb4558cad3583cee1544e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; bool &gt;</type>
      <name>initialized_</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a4ed746405c7f06faeb3063184a2cb8f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>private_mutex_</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>ad6d2ab49fcccb2d307112253707715ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex *</type>
      <name>bus_mutex_</name>
      <anchorfile>classFdo2Handler.html</anchorfile>
      <anchor>a2dfe74675ad87436a6c4a8ee96a70697</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Fdo2HandlerConfig</name>
    <filename>structFdo2HandlerConfig.html</filename>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>line_timeout_ms</name>
      <anchorfile>structFdo2HandlerConfig.html</anchorfile>
      <anchor>ae571afa8fbba96f6afecced1c8bc896f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>measure_timeout_ms</name>
      <anchorfile>structFdo2HandlerConfig.html</anchorfile>
      <anchor>aa4fce7d95248bc4425e9634171184b15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint8_t</type>
      <name>device_index</name>
      <anchorfile>structFdo2HandlerConfig.html</anchorfile>
      <anchor>a2ff5357b0ff4f1a8e2730b51aaabd0b5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tmc9660Handler::Gpio</name>
    <filename>classTmc9660Handler_1_1Gpio.html</filename>
    <member kind="function">
      <type></type>
      <name>Gpio</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a819b8d57d9ed8f80145a1c88e216ee62</anchor>
      <arglist>(Tmc9660Handler &amp;parent, uint8_t gpioNumber)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Gpio</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>af9012cdaeb1678b7b82087ece4fc494f</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a94de04e5fdade00c7f131b8b910bca80</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>ab51d1d37aef210699b8918ea78ede699</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPinAvailable</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a1bad24b764946fc93876d36fcc9b64dc</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxPins</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>afe970436a85c7fa61a7e74eef8ddb7fc</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a0370b84c0356793d8fc2529ba419bb41</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetDirectionImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>abb373809db45c7971b18cf0881b29976</anchor>
      <arglist>(hf_gpio_direction_t direction) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetOutputModeImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a4b370365fb2aa8c2cb8d4bdb2ff07caf</anchor>
      <arglist>(hf_gpio_output_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPullModeImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a9ce2b675e2bfacb82b957f4f2ec39e4a</anchor>
      <arglist>(hf_gpio_pull_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPinLevelImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a3484e3f2b1591d88d58f1e1a008bfbbc</anchor>
      <arglist>(hf_gpio_level_t level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetPinLevelImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a70c6aaec5983e4096c27e7d4e8767ef2</anchor>
      <arglist>(hf_gpio_level_t &amp;level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_pull_mode_t</type>
      <name>GetPullModeImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a025e514beea05768e2cd24b58e0fd7b0</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetDirectionImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>aaf312c1bbf7a2d2475909a8bde80a22c</anchor>
      <arglist>(hf_gpio_direction_t &amp;direction) const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetOutputModeImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>ab1a51599cf36c0f5680550d5afbcd785</anchor>
      <arglist>(hf_gpio_output_mode_t &amp;mode) const noexcept override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Tmc9660Handler &amp;</type>
      <name>parent_</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>ae9beb3e242baa96c117e3f3fb37f0bc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>gpioNumber_</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a4314495a5f96eca9ac7d9bb8134f8102</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>afb5f965e4632755cea9ce47937604244</anchor>
      <arglist>[32]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_gpio_direction_t</type>
      <name>direction_</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>aa3afa9d50a63ea40986a24e4cab6f7f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a94de04e5fdade00c7f131b8b910bca80</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>ab51d1d37aef210699b8918ea78ede699</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPinAvailable</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a1bad24b764946fc93876d36fcc9b64dc</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxPins</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>afe970436a85c7fa61a7e74eef8ddb7fc</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a0370b84c0356793d8fc2529ba419bb41</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetDirectionImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>abb373809db45c7971b18cf0881b29976</anchor>
      <arglist>(hf_gpio_direction_t direction) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetOutputModeImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a4b370365fb2aa8c2cb8d4bdb2ff07caf</anchor>
      <arglist>(hf_gpio_output_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPullModeImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a9ce2b675e2bfacb82b957f4f2ec39e4a</anchor>
      <arglist>(hf_gpio_pull_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPinLevelImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a3484e3f2b1591d88d58f1e1a008bfbbc</anchor>
      <arglist>(hf_gpio_level_t level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetPinLevelImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a70c6aaec5983e4096c27e7d4e8767ef2</anchor>
      <arglist>(hf_gpio_level_t &amp;level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_pull_mode_t</type>
      <name>GetPullModeImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>a025e514beea05768e2cd24b58e0fd7b0</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetDirectionImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>aaf312c1bbf7a2d2475909a8bde80a22c</anchor>
      <arglist>(hf_gpio_direction_t &amp;direction) const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetOutputModeImpl</name>
      <anchorfile>classTmc9660Handler_1_1Gpio.html</anchorfile>
      <anchor>ab1a51599cf36c0f5680550d5afbcd785</anchor>
      <arglist>(hf_gpio_output_mode_t &amp;mode) const noexcept override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalI2cBmp581Comm</name>
    <filename>classHalI2cBmp581Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalI2cBmp581Comm</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a3baa8e941c0723a369cecf70a3a05f29</anchor>
      <arglist>(BaseI2c &amp;i2c_device) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>WriteRegs</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a93aa7b7c9ed447edba78df07f9f45032</anchor>
      <arglist>(std::uint8_t reg, const std::uint8_t *data, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ReadRegs</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>abbbc0de308776bbad3b51442a9a89b10</anchor>
      <arglist>(std::uint8_t reg, std::uint8_t *out, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a585b589fb2e9242277fa5173072fc6c8</anchor>
      <arglist>(std::uint32_t us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a90c281269e06a8c959cc844d51757f25</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_device_</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a93d64fb960bcb37f1d2eb0f6eae6ed3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::uint8_t</type>
      <name>tx_scratch_</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a4c55f62949733ec25b2290c001894fc4</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::uint8_t</type>
      <name>rx_scratch_</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a9e2af899fbf4be6269798eb986ee11d2</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::uint8_t</type>
      <name>reg_scratch_</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>ae10b5cb31da93d9220df20694e839fa5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>WriteRegs</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a93aa7b7c9ed447edba78df07f9f45032</anchor>
      <arglist>(std::uint8_t reg, const std::uint8_t *data, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ReadRegs</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>abbbc0de308776bbad3b51442a9a89b10</anchor>
      <arglist>(std::uint8_t reg, std::uint8_t *out, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a585b589fb2e9242277fa5173072fc6c8</anchor>
      <arglist>(std::uint32_t us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalI2cBmp581Comm.html</anchorfile>
      <anchor>a90c281269e06a8c959cc844d51757f25</anchor>
      <arglist>() noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalI2cBno08xComm</name>
    <filename>classHalI2cBno08xComm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalI2cBno08xComm</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>afde4e38c7ab3e98c0bffc236629bc67a</anchor>
      <arglist>(BaseI2c &amp;i2c, BaseGpio *reset_gpio=nullptr, BaseGpio *int_gpio=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type>BNO085Interface</type>
      <name>GetInterfaceType</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>af286cccb3eb839e869f5dcb076e0ae07</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Open</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>acda12b075ae326ec9dcddcc6ec49b1d0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Close</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>a50726bdd6b82c76210b139bb60695dd6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>Write</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>af54938cbc82a718aeb463cad9183cbd3</anchor>
      <arglist>(const uint8_t *data, uint32_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>Read</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>abcc28793c108a7f216b6e09247c5d849</anchor>
      <arglist>(uint8_t *data, uint32_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>DataAvailable</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>aa9648740128765f62c24229e7b616c81</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Delay</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>a9e31c7ef335dbf10270c642f38ecdc72</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>GetTimeUs</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>a28e5cc1f58852b5ce443c067b2a475ba</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GpioSet</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>a2b036e336f3f439a0a158fac5fdf1421</anchor>
      <arglist>(bno08x::CtrlPin pin, bno08x::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>a42c98cd92dd35ea1cb000c4f98324b24</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>reset_gpio_</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>a47e4d758c94792c03f2b062f152180a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>int_gpio_</name>
      <anchorfile>classHalI2cBno08xComm.html</anchorfile>
      <anchor>a2a9ae5ca94b8d5ce08d9dab1c04c6fc4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalI2cPca9685Comm</name>
    <filename>classHalI2cPca9685Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalI2cPca9685Comm</name>
      <anchorfile>classHalI2cPca9685Comm.html</anchorfile>
      <anchor>a919aa48efbc89683f066a23056b4eb78</anchor>
      <arglist>(BaseI2c &amp;i2c_device) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Write</name>
      <anchorfile>classHalI2cPca9685Comm.html</anchorfile>
      <anchor>a9ed8d1fd0839a7e5620a7ebee0f62140</anchor>
      <arglist>(uint8_t addr, uint8_t reg, const uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Read</name>
      <anchorfile>classHalI2cPca9685Comm.html</anchorfile>
      <anchor>a527a3e92d1b7e9ee2e4d188f179b6929</anchor>
      <arglist>(uint8_t addr, uint8_t reg, uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalI2cPca9685Comm.html</anchorfile>
      <anchor>a72bb476ca0e88352093a37dd94bd480e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_device_</name>
      <anchorfile>classHalI2cPca9685Comm.html</anchorfile>
      <anchor>a65a901f0c59d18120055de429bb9ec9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>i2c_mutex_</name>
      <anchorfile>classHalI2cPca9685Comm.html</anchorfile>
      <anchor>a184349f391b935720c32d6ec729aae70</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Write</name>
      <anchorfile>classHalI2cPca9685Comm.html</anchorfile>
      <anchor>a9ed8d1fd0839a7e5620a7ebee0f62140</anchor>
      <arglist>(uint8_t addr, uint8_t reg, const uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Read</name>
      <anchorfile>classHalI2cPca9685Comm.html</anchorfile>
      <anchor>a527a3e92d1b7e9ee2e4d188f179b6929</anchor>
      <arglist>(uint8_t addr, uint8_t reg, uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalI2cPca9685Comm.html</anchorfile>
      <anchor>a72bb476ca0e88352093a37dd94bd480e</anchor>
      <arglist>() noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalI2cPcal95555Comm</name>
    <filename>classHalI2cPcal95555Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalI2cPcal95555Comm</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a954c492c422c732ff1da3a7a9715b4ed</anchor>
      <arglist>(BaseI2c &amp;i2c_device) noexcept</arglist>
    </member>
    <member kind="function">
      <type>const std::function&lt; void()&gt; &amp;</type>
      <name>GetInterruptHandler</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>aff0221cc91b57b20ba76435d0d99d170</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Write</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a4f6987588b44889b656c627cdb6f9523</anchor>
      <arglist>(uint8_t addr, uint8_t reg, const uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Read</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a4a772bc05d784a0ef807a4c3c099beb0</anchor>
      <arglist>(uint8_t addr, uint8_t reg, uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>ab5f4b653944ba3a9470c1e684662dc1f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>RegisterInterruptHandler</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a8a5517910e3a771091471f2843ea26ed</anchor>
      <arglist>(std::function&lt; void()&gt; handler) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_device_</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>aaab0f2ead8fb5c16cd047a24bc8139f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>i2c_mutex_</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a8f693605b5546ee054ee647f1d681b17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::function&lt; void()&gt;</type>
      <name>interrupt_handler_</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a5f821ea69c3532d8cc758b444e1d83cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>cmd_scratch_</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a1725f4bcf6c8775f42772131486b8a8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>tx_scratch_</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>aea7b83e5acbd6808e63b96b61ea6940e</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>rx_scratch_</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a9bad3cf07822e4dd367a725aac48c03a</anchor>
      <arglist>[2]</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Write</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a4f6987588b44889b656c627cdb6f9523</anchor>
      <arglist>(uint8_t addr, uint8_t reg, const uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Read</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a4a772bc05d784a0ef807a4c3c099beb0</anchor>
      <arglist>(uint8_t addr, uint8_t reg, uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>ab5f4b653944ba3a9470c1e684662dc1f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>RegisterInterruptHandler</name>
      <anchorfile>classHalI2cPcal95555Comm.html</anchorfile>
      <anchor>a8a5517910e3a771091471f2843ea26ed</anchor>
      <arglist>(std::function&lt; void()&gt; handler) noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalI2cSe050Comm</name>
    <filename>classHalI2cSe050Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalI2cSe050Comm</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>a79546790eb1ee4897c2cbace6bf95330</anchor>
      <arglist>(BaseI2c &amp;i2c, BaseGpio *reset_gpio) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>aafe10210e9cfb60c100ebfcf4803983d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>se050::Error</type>
      <name>I2cWrite</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>a70e1aefefc05f67353ba8cc5e2a66fdb</anchor>
      <arglist>(const std::uint8_t *tx, std::size_t tx_len, std::uint32_t timeout_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>se050::Error</type>
      <name>I2cRead</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>a961f84f8344a2612fc5eaf765ea760a9</anchor>
      <arglist>(std::uint8_t *rx, std::size_t rx_len, std::uint32_t timeout_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>se050::Error</type>
      <name>Transceive</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>a6709bf21c025c3520a0b975d96f8e3db</anchor>
      <arglist>(const std::uint8_t *tx, std::size_t tx_len, std::uint8_t *rx, std::size_t rx_cap, std::size_t *rx_len_out, std::uint32_t timeout_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>se050::Error</type>
      <name>HardwareReset</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>a8078b2b9ac2e5ac4df40519b0aaea8b5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delay_ms_impl</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>ac5a6e9ff0bfe08f99111fe4f43ee51d3</anchor>
      <arglist>(std::uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HoldBus</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>a7563d8499622691cf7c0eab4dbbcbaab</anchor>
      <arglist>(std::uint32_t timeout_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ReleaseBus</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>a3b713deeb54cd11eb1b2e2c050db794c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>BusHeld</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>a969bb59f3f10bccb688721df908004b0</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>ac13616ce7150a4f1dade01af631339c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>reset_gpio_</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>ad295dc332511e15e87f488fab44ce56a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>bus_held_</name>
      <anchorfile>classHalI2cSe050Comm.html</anchorfile>
      <anchor>ada5e0dda64f64ce4bcc75f68436e1c14</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalI2cSfmComm</name>
    <filename>classHalI2cSfmComm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalI2cSfmComm</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>a47fe5490e479ae1efc092cbc6e657e20</anchor>
      <arglist>(BaseI2c &amp;i2c_device) noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::uint8_t</type>
      <name>DeviceAddress</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>af49be0159ff59aa0aaab69332c8cff1c</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Write</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>a3e5352e342d19794507f780b25bb8e84</anchor>
      <arglist>(std::uint8_t addr7, const std::uint8_t *data, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Read</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>ab08cb52164e81c9619e5aa6e90e2dcf8</anchor>
      <arglist>(std::uint8_t addr7, std::uint8_t *out, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayMs</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>a66f6f7a75226130ea30e25a4b4479450</anchor>
      <arglist>(std::uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>a7a2dab73ae437d30f0720b8575b7611a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_device_</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>ae318b2a56c067ce60a39d4365185ed6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::uint8_t</type>
      <name>tx_scratch_</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>a6790afafd1594ee6535c7fe543321a98</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::uint8_t</type>
      <name>rx_scratch_</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>aace63c7c14fc2ab36b1490975ea36c98</anchor>
      <arglist>[18]</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Write</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>a3e5352e342d19794507f780b25bb8e84</anchor>
      <arglist>(std::uint8_t addr7, const std::uint8_t *data, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Read</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>ab08cb52164e81c9619e5aa6e90e2dcf8</anchor>
      <arglist>(std::uint8_t addr7, std::uint8_t *out, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayMs</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>a66f6f7a75226130ea30e25a4b4479450</anchor>
      <arglist>(std::uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalI2cSfmComm.html</anchorfile>
      <anchor>a7a2dab73ae437d30f0720b8575b7611a</anchor>
      <arglist>() noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalIsf15acp4SpiAdapter</name>
    <filename>classHalIsf15acp4SpiAdapter.html</filename>
    <member kind="function">
      <type></type>
      <name>HalIsf15acp4SpiAdapter</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a85f66343fe9329eec031abdad21e53a1</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;cs, BaseGpio &amp;dc, BaseGpio &amp;reset, BaseGpio *vcc_enable=nullptr, BaseGpio *switch_in=nullptr, bool switch_active_low=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a269fc420c5fbfb62c488db05c9d575f6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>BeginCommand</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>ae7f333f61d9921892f902db8e2c5a8c6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>BeginData</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>af435f9d17abbc0bb8940eced0c4071dc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EndTransaction</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a0bdd4b535b88f1d51e81f08f4d438247</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>WriteBytes</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a49c60f30b886c64f425cc7f30b00dc2f</anchor>
      <arglist>(const uint8_t *data, std::size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GpioSet</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a37154768e8d31f2e292412f83a02b519</anchor>
      <arglist>(isf15acp4::CtrlPin pin, isf15acp4::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>GpioRead</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a4a78f216161ac2371c86254940052673</anchor>
      <arglist>(isf15acp4::CtrlPin pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a7a60fc76c2ada54b883d10c527567dbb</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>setGpio</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a1a92b202e3a1a2b93dfc5ac65a8590d8</anchor>
      <arglist>(BaseGpio &amp;gpio, bool electrical_high) noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static bool</type>
      <name>readGpio</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a09fff052bc7e69b1f9d589c5f33eff94</anchor>
      <arglist>(BaseGpio &amp;gpio, bool active_low) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a1ad81ae6144bb1f03a4eea3d059dd3c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>cs_</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>abca34c40bc403ce3bcd6259324d74351</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>dc_</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a6fc5d96537d91c75d458b1507a116330</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>reset_</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>ab073acf001f2152f98994c5f981b3a54</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>vcc_enable_</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>acc741fa32d438ebc0b238af9fc69650d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>switch_in_</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>af3fcf01c7a18b4c072e1e5ecd82f30ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>switch_active_low_</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>ab199b902642093c680a8f91a59064477</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>ready_</name>
      <anchorfile>classHalIsf15acp4SpiAdapter.html</anchorfile>
      <anchor>a6bc5436fdd9633a5460e45876803a5a8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalPf1550Comm</name>
    <filename>classHalPf1550Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalPf1550Comm</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>a8a0ff7b976bc350256452923b42f116c</anchor>
      <arglist>(BaseI2c &amp;i2c, BaseGpio *standby_gpio, BaseGpio *usb_vbus_en_gpio, BaseGpio *usb_otg_en_gpio) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Write</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>a62771ef9a0c4dd34afbdd9065992bb0f</anchor>
      <arglist>(uint8_t addr, uint8_t reg, const uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Read</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>a0ddaa3f66fb44a34ff58e7f4f0e3e5da</anchor>
      <arglist>(uint8_t addr, uint8_t reg, uint8_t *data, size_t len) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>a19bd8def2de5ac4f3d028331986f3b39</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GpioSet</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>a0e13772d7e106e042fef525f0c85ca27</anchor>
      <arglist>(pf1550::CtrlPin pin, pf1550::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>ab3992fa50cd643d6a5056e1b34f1ff68</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>a47eb11b80e22649ba7cfd1d8c35e9b65</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>standby_gpio_</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>ae32b1fa53a9642a19da7156ff2d625de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>usb_vbus_en_gpio_</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>a124509a85d483f0c4267bc7f987d1f7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>usb_otg_en_gpio_</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>aaddf66e3517fe75f6385a1687129b671</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>i2c_mutex_</name>
      <anchorfile>classHalPf1550Comm.html</anchorfile>
      <anchor>ae89851e864bfe4cf75f2b1678bda9532</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalSpiBno08xComm</name>
    <filename>classHalSpiBno08xComm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalSpiBno08xComm</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>a880384796f34e09d390176735ffe6650</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio *reset_gpio=nullptr, BaseGpio *int_gpio=nullptr, BaseGpio *wake_gpio=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type>BNO085Interface</type>
      <name>GetInterfaceType</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>afb07a9f1a7eb1084e9c7f65fdcf50207</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Open</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>ae098ae79401bc270da7c0673846c30f0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Close</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>ab5d623448eeb3d1f48468c77f258ad10</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>Write</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>aade85e3da1785bf3c5f1835702903f51</anchor>
      <arglist>(const uint8_t *data, uint32_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>Read</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>adcd09b77763bed245b9573e6f743f2a0</anchor>
      <arglist>(uint8_t *data, uint32_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>DataAvailable</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>a89bd1ea79d01322d41ee2195c7ffdc19</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Delay</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>a66125153a6719f71942be7b1fd925f3a</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>GetTimeUs</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>a4248142269520b3bd7fdc77867ff6cab</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GpioSet</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>aaf74e0d4b9091ec6019be5d2f3b09e12</anchor>
      <arglist>(bno08x::CtrlPin pin, bno08x::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>a216981250a3ea8a94b4df383610353b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>reset_gpio_</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>ad01c70218c8d062eb9ebd6d638e28ac9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>int_gpio_</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>a34555f644ea717a68e1d6a7e7fa2bd1a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>wake_gpio_</name>
      <anchorfile>classHalSpiBno08xComm.html</anchorfile>
      <anchor>a676f2c503ebc07e6ac6ba0a3f44f099f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalSpiMax22200Comm</name>
    <filename>classHalSpiMax22200Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalSpiMax22200Comm</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a7c44007d24a7415b551f3478f26275e9</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;enable, BaseGpio &amp;cmd, BaseGpio *fault=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>acea1390361f74d13563126f81dd4da1e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Transfer</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>ab69a5625afd94e62e4f973eea89979a2</anchor>
      <arglist>(const uint8_t *tx_data, uint8_t *rx_data, size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>SetChipSelect</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a5c5923eecc5266b4ecadab4220cde6d5</anchor>
      <arglist>(bool state) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Configure</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a86c5a54b0e6cc0b84122e13936a88a27</anchor>
      <arglist>(uint32_t speed_hz, uint8_t mode, bool msb_first=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsReady</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a6217c4c3006d026b9bdbe2155fcb16b2</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a485c7077edcd4c3c7d5bafc6a221ca37</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GpioSet</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>afa72d731643fff15ec9f448e62401b43</anchor>
      <arglist>(max22200::CtrlPin pin, max22200::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>GpioRead</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a765cf2ee51ea49c7a222da0755549951</anchor>
      <arglist>(max22200::CtrlPin pin, max22200::GpioSignal &amp;signal) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>ab995505356aaba21bef34c994bf5c78a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>enable_</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a4ef640e44a008a64265ca12158d1a3c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>cmd_</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a50e2b2810862b78f2c23bab7a4c0f402</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>fault_</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a33b760fc9d485515fb533afe58a12d82</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a0a4545e17540af9452cc8c0ae1fd743d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>acea1390361f74d13563126f81dd4da1e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Transfer</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>ab69a5625afd94e62e4f973eea89979a2</anchor>
      <arglist>(const uint8_t *tx_data, uint8_t *rx_data, size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>SetChipSelect</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a5c5923eecc5266b4ecadab4220cde6d5</anchor>
      <arglist>(bool state) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Configure</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a86c5a54b0e6cc0b84122e13936a88a27</anchor>
      <arglist>(uint32_t speed_hz, uint8_t mode, bool msb_first=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsReady</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a6217c4c3006d026b9bdbe2155fcb16b2</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a485c7077edcd4c3c7d5bafc6a221ca37</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>GpioSet</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>afa72d731643fff15ec9f448e62401b43</anchor>
      <arglist>(max22200::CtrlPin pin, max22200::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>GpioRead</name>
      <anchorfile>classHalSpiMax22200Comm.html</anchorfile>
      <anchor>a765cf2ee51ea49c7a222da0755549951</anchor>
      <arglist>(max22200::CtrlPin pin, max22200::GpioSignal &amp;signal) noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalSpiTle92466edComm</name>
    <filename>classHalSpiTle92466edComm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalSpiTle92466edComm</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>ae5ebff246df187621d975c09b4d32585</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;resn, BaseGpio &amp;en, BaseGpio *faultn=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>Init</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a2f62649da77db8dcc44340f1bcf31973</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>Deinit</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a25410af1693bc75af0fcb40d7cce22ab</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; uint32_t &gt;</type>
      <name>Transfer32</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a63f84b0db1a338f4c02e76e8d01c8b65</anchor>
      <arglist>(uint32_t tx_data) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>TransferMulti</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a058b8f150542b2e746f08a1f3b84208b</anchor>
      <arglist>(std::span&lt; const uint32_t &gt; tx_data, std::span&lt; uint32_t &gt; rx_data) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>TransferMulti</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a3abfd791065fcca6d44884f420bc1ced</anchor>
      <arglist>(std::span&lt; const uint32_t &gt; tx_data, std::span&lt; uint32_t &gt; rx_data, uint32_t gap_us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>Delay</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>aca4238d0c15c631b7fca413fdf7420ab</anchor>
      <arglist>(uint32_t microseconds) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>Configure</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a9aca0311d71b2663db79601720b3de07</anchor>
      <arglist>(const tle92466ed::SPIConfig &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsReady</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a74ccbe3aa8f726c7b9ca9096636dcf14</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommError</type>
      <name>GetLastError</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a69ba2d992118332b715f6ab8e1fa0a56</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>ClearErrors</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>ae6284f4ee27f3afb48b9f7fb005d5e6c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>GpioSet</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a324a1156fd81d3aaf3c6d6f07296ae3b</anchor>
      <arglist>(tle92466ed::CtrlPin pin, tle92466ed::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; tle92466ed::GpioSignal &gt;</type>
      <name>GpioRead</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a082f6fda7c85de00474f24f2bae90691</anchor>
      <arglist>(tle92466ed::CtrlPin pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Log</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>ada2e34106a72dd56b1513f83a0cce63e</anchor>
      <arglist>(tle92466ed::LogLevel level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>LastRxWord</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a4a40e8c8cdb9dea21d7f828b1186a8ea</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>LastRxWordFirst</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a0f7a1432092e8109ab3d7532eb56a9bf</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>acfa2ec7048c1041eabd9d904d0adddf8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>resn_</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a1866f3c39835beae58426fdfd47d7435</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>en_</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>ad1352b0117d174b50f28bd32cdc0a65c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>faultn_</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a50b2039ea26470371f6e75af11ff7b17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a3e4ab75d1716561d08697a47917f56a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>tle92466ed::CommError</type>
      <name>last_error_</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a2155df77a576e4c58d2365bc3ae39951</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint32_t</type>
      <name>last_rx_</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>ae59b8ba0ff0b0e44531ac6eff17ad248</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint32_t</type>
      <name>last_rx_first_</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a15788acd3867684e0418a24a07af59a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>Init</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a2f62649da77db8dcc44340f1bcf31973</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>Deinit</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a25410af1693bc75af0fcb40d7cce22ab</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; uint32_t &gt;</type>
      <name>Transfer32</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a63f84b0db1a338f4c02e76e8d01c8b65</anchor>
      <arglist>(uint32_t tx_data) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>TransferMulti</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a058b8f150542b2e746f08a1f3b84208b</anchor>
      <arglist>(std::span&lt; const uint32_t &gt; tx_data, std::span&lt; uint32_t &gt; rx_data) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>TransferMulti</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a3abfd791065fcca6d44884f420bc1ced</anchor>
      <arglist>(std::span&lt; const uint32_t &gt; tx_data, std::span&lt; uint32_t &gt; rx_data, uint32_t gap_us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>Delay</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>aca4238d0c15c631b7fca413fdf7420ab</anchor>
      <arglist>(uint32_t microseconds) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>Configure</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a9aca0311d71b2663db79601720b3de07</anchor>
      <arglist>(const tle92466ed::SPIConfig &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsReady</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a74ccbe3aa8f726c7b9ca9096636dcf14</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommError</type>
      <name>GetLastError</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a69ba2d992118332b715f6ab8e1fa0a56</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>ClearErrors</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>ae6284f4ee27f3afb48b9f7fb005d5e6c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; void &gt;</type>
      <name>GpioSet</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a324a1156fd81d3aaf3c6d6f07296ae3b</anchor>
      <arglist>(tle92466ed::CtrlPin pin, tle92466ed::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::CommResult&lt; tle92466ed::GpioSignal &gt;</type>
      <name>GpioRead</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a082f6fda7c85de00474f24f2bae90691</anchor>
      <arglist>(tle92466ed::CtrlPin pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Log</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>ada2e34106a72dd56b1513f83a0cce63e</anchor>
      <arglist>(tle92466ed::LogLevel level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>LastRxWord</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a4a40e8c8cdb9dea21d7f828b1186a8ea</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>LastRxWordFirst</name>
      <anchorfile>classHalSpiTle92466edComm.html</anchorfile>
      <anchor>a0f7a1432092e8109ab3d7532eb56a9bf</anchor>
      <arglist>() const noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalSpiTmc5160Comm</name>
    <filename>classHalSpiTmc5160Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalSpiTmc5160Comm</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a460b9fe3ce0b0001cfed4cb1f818d49f</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;enable, BaseGpio *diag0=nullptr, BaseGpio *diag1=nullptr, const tmc51x0::PinActiveLevels &amp;active_levels={}) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>SpiTransfer</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>ad2c09e34483d07e2a11ee5438ef6c3c7</anchor>
      <arglist>(const uint8_t *tx, uint8_t *rx, size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>GpioSet</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>ad40d6f955903195dde76310f9b22e43d</anchor>
      <arglist>(tmc51x0::TMC51x0CtrlPin pin, tmc51x0::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; tmc51x0::GpioSignal &gt;</type>
      <name>GpioRead</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a12d4631ae0dd1a28c71ea7926dc4cc57</anchor>
      <arglist>(tmc51x0::TMC51x0CtrlPin pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DebugLog</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a9be312a17a2d104eff165383eadb37c7</anchor>
      <arglist>(int level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayMs</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a68134d1ddb4ce782d8aa4d367475f054</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a48c631e0d24f7b4b0c33c83b1ffa91ad</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::CommMode</type>
      <name>GetMode</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>ad8fab19c9f7d162f7d1dd2550b6d723b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a1c7479ef6289f49ac4cc4056f53fe2e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Tmc5160CtrlPins</type>
      <name>ctrl_pins_</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>aa9c18495a245e766e9b7204fb81e5818</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>tmc51x0::PinActiveLevels</type>
      <name>active_levels_</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>abdb1653393e8b07cf57671e818eedc1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>SpiTransfer</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>ad2c09e34483d07e2a11ee5438ef6c3c7</anchor>
      <arglist>(const uint8_t *tx, uint8_t *rx, size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>GpioSet</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>ad40d6f955903195dde76310f9b22e43d</anchor>
      <arglist>(tmc51x0::TMC51x0CtrlPin pin, tmc51x0::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; tmc51x0::GpioSignal &gt;</type>
      <name>GpioRead</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a12d4631ae0dd1a28c71ea7926dc4cc57</anchor>
      <arglist>(tmc51x0::TMC51x0CtrlPin pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DebugLog</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a9be312a17a2d104eff165383eadb37c7</anchor>
      <arglist>(int level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayMs</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a68134d1ddb4ce782d8aa4d367475f054</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>a48c631e0d24f7b4b0c33c83b1ffa91ad</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::CommMode</type>
      <name>GetMode</name>
      <anchorfile>classHalSpiTmc5160Comm.html</anchorfile>
      <anchor>ad8fab19c9f7d162f7d1dd2550b6d723b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalSpiTmc9660Comm</name>
    <filename>classHalSpiTmc9660Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalSpiTmc9660Comm</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>af1a0f32616c37bc82b428e3ebd141f54</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;rst, BaseGpio &amp;drv_en, BaseGpio &amp;faultn, BaseGpio &amp;wake, bool rst_active_high=true, bool drv_en_active_high=true, bool faultn_active_low=false, bool wake_active_low=false) noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>transportDiag</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a2569902a00690043e386003f413b853b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>rawTmclProbe</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>ae364a929ba09c27d675dde8613063f3b</anchor>
      <arglist>(std::array&lt; uint8_t, 8 &gt; &amp;tx, std::array&lt; uint8_t, 8 &gt; &amp;rx, uint32_t timeout_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>spiTransferTMCL</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a84b3ab081c7b9cab86432945ca9e3f6e</anchor>
      <arglist>(std::array&lt; uint8_t, 8 &gt; &amp;tx, std::array&lt; uint8_t, 8 &gt; &amp;rx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hostSpiTransferTMCLPair</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a0bab91ce7d731fa96bf50cfa9b5495fa</anchor>
      <arglist>(std::array&lt; uint8_t, 8 &gt; &amp;tx1, std::array&lt; uint8_t, 8 &gt; &amp;rx1, std::array&lt; uint8_t, 8 &gt; &amp;tx2, std::array&lt; uint8_t, 8 &gt; &amp;rx2, uint32_t gap_us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>spiTransferBootloader</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a691d8a1d66cf606952d11afc73bffda0</anchor>
      <arglist>(std::array&lt; uint8_t, 5 &gt; &amp;tx, std::array&lt; uint8_t, 5 &gt; &amp;rx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gpioSet</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>ad62042c7cc3d39dd901ecf1f6a3ccb6c</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gpioRead</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>aa7c3ebc1dfe70f3012ce352c0e881ca3</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal &amp;signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugLog</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>ac7145aa4216147b19418a4b23cccda75</anchor>
      <arglist>(int level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delayMs</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a1f4360b2fe821d797fc4a8d771150656</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delayUs</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>aeed51d5504933aa294d940f0799c279a</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a79247e058a5250b805f60de3c2e58ced</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Tmc9660CtrlPins</type>
      <name>ctrl_pins_</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a57a03a0afc398cb7af11919a42aff141</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint32_t</type>
      <name>last_spi_err_</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>abe9a38c16d2446dc0cb6d1b3d6e1f0fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>spiTransferTMCL</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a84b3ab081c7b9cab86432945ca9e3f6e</anchor>
      <arglist>(std::array&lt; uint8_t, 8 &gt; &amp;tx, std::array&lt; uint8_t, 8 &gt; &amp;rx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hostSpiTransferTMCLPair</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a0bab91ce7d731fa96bf50cfa9b5495fa</anchor>
      <arglist>(std::array&lt; uint8_t, 8 &gt; &amp;tx1, std::array&lt; uint8_t, 8 &gt; &amp;rx1, std::array&lt; uint8_t, 8 &gt; &amp;tx2, std::array&lt; uint8_t, 8 &gt; &amp;rx2, uint32_t gap_us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>spiTransferBootloader</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a691d8a1d66cf606952d11afc73bffda0</anchor>
      <arglist>(std::array&lt; uint8_t, 5 &gt; &amp;tx, std::array&lt; uint8_t, 5 &gt; &amp;rx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gpioSet</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>ad62042c7cc3d39dd901ecf1f6a3ccb6c</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gpioRead</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>aa7c3ebc1dfe70f3012ce352c0e881ca3</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal &amp;signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugLog</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>ac7145aa4216147b19418a4b23cccda75</anchor>
      <arglist>(int level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delayMs</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>a1f4360b2fe821d797fc4a8d771150656</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delayUs</name>
      <anchorfile>classHalSpiTmc9660Comm.html</anchorfile>
      <anchor>aeed51d5504933aa294d940f0799c279a</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalUartAlicatBasis2Comm</name>
    <filename>classHalUartAlicatBasis2Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalUartAlicatBasis2Comm</name>
      <anchorfile>classHalUartAlicatBasis2Comm.html</anchorfile>
      <anchor>af6e931410893120b6bec6c8311141cc2</anchor>
      <arglist>(BaseUart &amp;uart) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classHalUartAlicatBasis2Comm.html</anchorfile>
      <anchor>aee601a75f437a026d19fb9213287ab83</anchor>
      <arglist>(const std::uint8_t *data, std::size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>read</name>
      <anchorfile>classHalUartAlicatBasis2Comm.html</anchorfile>
      <anchor>a9a370f8aee1dc9745cbb5c3ff8dc1ba9</anchor>
      <arglist>(std::uint8_t *out, std::size_t max, std::uint32_t timeout_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>flush_rx</name>
      <anchorfile>classHalUartAlicatBasis2Comm.html</anchorfile>
      <anchor>aa663736de5ebf3cba233d5135728b27d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delay_ms_impl</name>
      <anchorfile>classHalUartAlicatBasis2Comm.html</anchorfile>
      <anchor>a17ce7a1f509bc27b6db957f00a8af17f</anchor>
      <arglist>(std::uint32_t ms) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseUart &amp;</type>
      <name>uart_</name>
      <anchorfile>classHalUartAlicatBasis2Comm.html</anchorfile>
      <anchor>af09d5596a6c8e79330c722e0d6d8c68e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalUartDynamixelComm</name>
    <filename>classHalUartDynamixelComm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalUartDynamixelComm</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>a7f62d5bd041868d9624798c5a328f1a7</anchor>
      <arglist>(BaseUart &amp;uart) noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverError</type>
      <name>open</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>a790e4fb2b3011e92073758e503716570</anchor>
      <arglist>(const dynamixel::TransportConfig &amp;) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>close</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>a6756b6358757df2b4c548b285f96474d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverError</type>
      <name>set_baud_rate</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>aec3efb8af3f072606dbd5046903e3639</anchor>
      <arglist>(std::uint32_t baud) noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::TransportIo</type>
      <name>write_some</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>a04b89fd05332e5b708f0ac96d8cf94e3</anchor>
      <arglist>(const std::uint8_t *data, std::size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::DriverError</type>
      <name>finish_transmit</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>a693ebb11d45379e210b3d85b50816844</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>dynamixel::TransportIo</type>
      <name>read_some</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>a8dbe013021d2d31bc2dae045ec1aab68</anchor>
      <arglist>(std::uint8_t *out, std::size_t max) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>discard_stale_input</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>a94077c4c9a98fb16d9fa8b68d4074967</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::uint64_t</type>
      <name>now_us</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>ac86b581a82cb610c9612d34ac2610e01</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delay_ms_impl</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>a6188c5b3eb0182bc1d15ed6d5697ea44</anchor>
      <arglist>(std::uint32_t ms) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseUart &amp;</type>
      <name>uart_</name>
      <anchorfile>classHalUartDynamixelComm.html</anchorfile>
      <anchor>a1e69a6199cd44fa23b04fa3f28383d6e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalUartFdo2Comm</name>
    <filename>classHalUartFdo2Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalUartFdo2Comm</name>
      <anchorfile>classHalUartFdo2Comm.html</anchorfile>
      <anchor>a1b843d32bc23259cb9574c40a4dfddcd</anchor>
      <arglist>(BaseUart &amp;uart) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classHalUartFdo2Comm.html</anchorfile>
      <anchor>acb8749482c7ef94e99d33bafdb0d981b</anchor>
      <arglist>(const std::uint8_t *data, std::size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>read</name>
      <anchorfile>classHalUartFdo2Comm.html</anchorfile>
      <anchor>af967fa684e34d0c0bd33728669eea72e</anchor>
      <arglist>(std::uint8_t *out, std::size_t max, std::uint32_t timeout_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>flush_rx</name>
      <anchorfile>classHalUartFdo2Comm.html</anchorfile>
      <anchor>ae35e65fd7d755ae9179b93af1e295e29</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delay_ms_impl</name>
      <anchorfile>classHalUartFdo2Comm.html</anchorfile>
      <anchor>a5589d7088dcae478e66825f63c6da124</anchor>
      <arglist>(std::uint32_t ms) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseUart &amp;</type>
      <name>uart_</name>
      <anchorfile>classHalUartFdo2Comm.html</anchorfile>
      <anchor>a8a3645bde68b04552d2a0a3b45baed78</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalUartTmc5160Comm</name>
    <filename>classHalUartTmc5160Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalUartTmc5160Comm</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a439031723f57353182a54d6a2e6b2c48</anchor>
      <arglist>(BaseUart &amp;uart, BaseGpio &amp;enable, BaseGpio *diag0=nullptr, BaseGpio *diag1=nullptr, const tmc51x0::PinActiveLevels &amp;active_levels={}) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>UartSend</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>abeca3028fb6b1007c2457b08d3dfe113</anchor>
      <arglist>(const uint8_t *data, size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>UartReceive</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a8549a4cd0f5427fcba59c0b41c0ac9e6</anchor>
      <arglist>(uint8_t *data, size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>GpioSet</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a36a213182c6897a47b986b020f60b127</anchor>
      <arglist>(tmc51x0::TMC51x0CtrlPin pin, tmc51x0::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; tmc51x0::GpioSignal &gt;</type>
      <name>GpioRead</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a3b5ee138e1facbf7a078d76af7138e67</anchor>
      <arglist>(tmc51x0::TMC51x0CtrlPin pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DebugLog</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a52758af74d4bb38d2d44c1111f13a94e</anchor>
      <arglist>(int level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayMs</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a2ad82076cd779eee19a69de5274f9b70</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a87ed8334c9dcbd2f8c028da910f5a968</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::CommMode</type>
      <name>GetMode</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a706e06d0266fe0ceb07095c4a2645517</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseUart &amp;</type>
      <name>uart_</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a26bfc4fc96f6bd0f453873729ab4d1c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Tmc5160CtrlPins</type>
      <name>ctrl_pins_</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>aa814d4f11e76e735658a00e84d9b8895</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>tmc51x0::PinActiveLevels</type>
      <name>active_levels_</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a9206f8e8b25f0dd1d78467e49d8f5b4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>UartSend</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>abeca3028fb6b1007c2457b08d3dfe113</anchor>
      <arglist>(const uint8_t *data, size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>UartReceive</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a8549a4cd0f5427fcba59c0b41c0ac9e6</anchor>
      <arglist>(uint8_t *data, size_t length) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; void &gt;</type>
      <name>GpioSet</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a36a213182c6897a47b986b020f60b127</anchor>
      <arglist>(tmc51x0::TMC51x0CtrlPin pin, tmc51x0::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::Result&lt; tmc51x0::GpioSignal &gt;</type>
      <name>GpioRead</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a3b5ee138e1facbf7a078d76af7138e67</anchor>
      <arglist>(tmc51x0::TMC51x0CtrlPin pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DebugLog</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a52758af74d4bb38d2d44c1111f13a94e</anchor>
      <arglist>(int level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayMs</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a2ad82076cd779eee19a69de5274f9b70</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a87ed8334c9dcbd2f8c028da910f5a968</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::CommMode</type>
      <name>GetMode</name>
      <anchorfile>classHalUartTmc5160Comm.html</anchorfile>
      <anchor>a706e06d0266fe0ceb07095c4a2645517</anchor>
      <arglist>() const noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HalUartTmc9660Comm</name>
    <filename>classHalUartTmc9660Comm.html</filename>
    <member kind="function">
      <type></type>
      <name>HalUartTmc9660Comm</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a144c7a0f50237dff1fa25c235084c3e1</anchor>
      <arglist>(BaseUart &amp;uart, BaseGpio &amp;rst, BaseGpio &amp;drv_en, BaseGpio &amp;faultn, BaseGpio &amp;wake, bool rst_active_high=true, bool drv_en_active_high=true, bool faultn_active_low=false, bool wake_active_low=false) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setReplyTimeoutMs</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>ab2591ae09ecaaecc8816abed53467422</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>replyTimeoutMs</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a610647df8d601e5eb5892d924fa1fde6</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>uartSendTMCL</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a213dfdf5041d1d98372413b9cd3a9b53</anchor>
      <arglist>(const std::array&lt; uint8_t, 9 &gt; &amp;data) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>uartReceiveTMCL</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a0df8ee2b226b44eb2e21728e4b7c44d7</anchor>
      <arglist>(std::array&lt; uint8_t, 9 &gt; &amp;data) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>uartTransferBootloader</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a044f4af1242b3fb2a9412523e575447a</anchor>
      <arglist>(const std::array&lt; uint8_t, 8 &gt; &amp;tx, std::array&lt; uint8_t, 8 &gt; &amp;rx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gpioSet</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a85aa3e262d86857902e11e818e2e818e</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gpioRead</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>af2f8bd1ec3ad0a3c3d59b97e13babff2</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal &amp;signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugLog</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a1a992dfbd1c60452f19f38dff2754403</anchor>
      <arglist>(int level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delayMs</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>ac4e3272567e3486a74e62d53ccd814d9</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delayUs</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a03e458d1dca85da1149a9040d8458c06</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseUart &amp;</type>
      <name>uart_</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>ad5eebb3f68eb20fe923a9370d6120ece</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Tmc9660CtrlPins</type>
      <name>ctrl_pins_</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a7d842bccb7b871b4fa3367935b3c4024</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint32_t</type>
      <name>reply_timeout_ms_</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a5017fad0cce1e963730dff7dbd1609d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>uartSendTMCL</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a213dfdf5041d1d98372413b9cd3a9b53</anchor>
      <arglist>(const std::array&lt; uint8_t, 9 &gt; &amp;data) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>uartReceiveTMCL</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a0df8ee2b226b44eb2e21728e4b7c44d7</anchor>
      <arglist>(std::array&lt; uint8_t, 9 &gt; &amp;data) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>uartTransferBootloader</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a044f4af1242b3fb2a9412523e575447a</anchor>
      <arglist>(const std::array&lt; uint8_t, 8 &gt; &amp;tx, std::array&lt; uint8_t, 8 &gt; &amp;rx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gpioSet</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a85aa3e262d86857902e11e818e2e818e</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>gpioRead</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>af2f8bd1ec3ad0a3c3d59b97e13babff2</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal &amp;signal) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugLog</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a1a992dfbd1c60452f19f38dff2754403</anchor>
      <arglist>(int level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delayMs</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>ac4e3272567e3486a74e62d53ccd814d9</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delayUs</name>
      <anchorfile>classHalUartTmc9660Comm.html</anchorfile>
      <anchor>a03e458d1dca85da1149a9040d8458c06</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>HandlerTaskArgs</name>
    <filename>structHandlerTaskArgs.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>name</name>
      <anchorfile>structHandlerTaskArgs.html</anchorfile>
      <anchor>a4946ac1e075c203883f24d7d78b10afc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::atomic&lt; int &gt; *</type>
      <name>success_count</name>
      <anchorfile>structHandlerTaskArgs.html</anchorfile>
      <anchor>a23a9a22a9e3346fc09227f7dd3410aae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>iterations</name>
      <anchorfile>structHandlerTaskArgs.html</anchorfile>
      <anchor>abca62a9df4baab74ed4369b05b2cb2e2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HfUtilsCanOpenTransport</name>
    <filename>classHfUtilsCanOpenTransport.html</filename>
    <member kind="function">
      <type></type>
      <name>HfUtilsCanOpenTransport</name>
      <anchorfile>classHfUtilsCanOpenTransport.html</anchorfile>
      <anchor>a693bf70cdece51659cb0b4c2e5625762</anchor>
      <arglist>(BaseCan &amp;can) noexcept</arglist>
    </member>
    <member kind="function">
      <type>BaseCan &amp;</type>
      <name>can</name>
      <anchorfile>classHfUtilsCanOpenTransport.html</anchorfile>
      <anchor>ae8907dea8d44d39407877ecd58fb01d1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>send</name>
      <anchorfile>classHfUtilsCanOpenTransport.html</anchorfile>
      <anchor>abb816b2ae179800b733f16a34cc024a8</anchor>
      <arglist>(const CanOpen::CanFrame &amp;f, hf_u32_t timeout_ms=50U) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>receive</name>
      <anchorfile>classHfUtilsCanOpenTransport.html</anchorfile>
      <anchor>aa7ca3b9e5bda184de10956e775976de4</anchor>
      <arglist>(CanOpen::CanFrame &amp;f, hf_u32_t timeout_ms) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseCan &amp;</type>
      <name>can_</name>
      <anchorfile>classHfUtilsCanOpenTransport.html</anchorfile>
      <anchor>a0813e37429f6126a20c4b586d1cabba4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>IBno08xDriverOps</name>
    <filename>classIBno08xDriverOps.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~IBno08xDriverOps</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>af4304b60a6c224809a080144789ede9f</anchor>
      <arglist>() noexcept=default</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>Begin</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>a0ca90c1a5664ab3eb83f691aff34faa0</anchor>
      <arglist>() noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>Update</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>acf715744d1f45d38c44b6cbe6a2f56a4</anchor>
      <arglist>() noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>EnableSensor</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>a1da5bdfd4c8d7fbd42475ae7ee37fb98</anchor>
      <arglist>(BNO085Sensor sensor, uint32_t interval_ms, float sensitivity) noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>DisableSensor</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>ab62ac5137487fe6f6ab3386684681857</anchor>
      <arglist>(BNO085Sensor sensor) noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>SetCallback</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>a04816a487d80027e6d6fe435f651b265</anchor>
      <arglist>(SensorCallback cb) noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>HasNewData</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>a1529def7c6fc77eed5dbc39aa16f3cc9</anchor>
      <arglist>(BNO085Sensor sensor) const noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual SensorEvent</type>
      <name>GetLatest</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>ab2a749539196e1ba1362837595b048d7</anchor>
      <arglist>(BNO085Sensor sensor) const noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual int</type>
      <name>GetLastError</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>ade5ca399da28b49b2b9b1851cd189ede</anchor>
      <arglist>() const noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>HardwareReset</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>a66418ccb85cd8eb8eea327fa979a2b6a</anchor>
      <arglist>(uint32_t lowMs) noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>SetBootPin</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>a35854a70ef94b79c9193d40459341864</anchor>
      <arglist>(bool state) noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>SetWakePin</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>af376c488e7690106f52551157d362184</anchor>
      <arglist>(bool state) noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>SelectInterface</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>a70716b25e2722af65540942e5db8d4d1</anchor>
      <arglist>(BNO085Interface iface) noexcept=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual BNO085Interface</type>
      <name>GetInterfaceType</name>
      <anchorfile>classIBno08xDriverOps.html</anchorfile>
      <anchor>a4833242c3c81925b8e05a9dff1deaa5f</anchor>
      <arglist>() noexcept=0</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tle92466edHandler::is_driver_result</name>
    <filename>structTle92466edHandler_1_1is__driver__result.html</filename>
    <templarg>typename T</templarg>
  </compound>
  <compound kind="struct">
    <name>Tle92466edHandler::is_driver_result&lt; tle92466ed::DriverResult&lt; T &gt; &gt;</name>
    <filename>structTle92466edHandler_1_1is__driver__result_3_01tle92466ed_1_1DriverResult_3_01T_01_4_01_4.html</filename>
    <templarg>typename T</templarg>
  </compound>
  <compound kind="class">
    <name>Isf15acp4Handler</name>
    <filename>classIsf15acp4Handler.html</filename>
    <member kind="function">
      <type></type>
      <name>Isf15acp4Handler</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>aa9e4794456037d32eb5f9ca0aa90f325</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;cs, BaseGpio &amp;dc, BaseGpio &amp;reset, const Isf15acp4HandlerConfig &amp;config={}, BaseGpio *vcc_enable=nullptr, BaseGpio *switch_in=nullptr, bool switch_active_low=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Isf15acp4Handler</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a386781ecd5b7397fbe62d0035a206286</anchor>
      <arglist>(const Isf15acp4Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Isf15acp4Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a0ee9ba3c8cb652e001f0f5b36cd87215</anchor>
      <arglist>(const Isf15acp4Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Isf15acp4Handler</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>aa39b7fc8f3555c9f460189607dcac11a</anchor>
      <arglist>(Isf15acp4Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Isf15acp4Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a7554f723e86f0943d396a31c937d26f0</anchor>
      <arglist>(Isf15acp4Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>aa8ae72cc9cceaa9de40592174b1d3617</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureDeinitialized</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>ab1f8a44ad63d176733b1fc2a77f49075</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>ad116826f1444d11b2a24ca72290b69bc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>ae95eb9bcb702e9944198eee38322856d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>DisplayOn</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a89a62b51d21e222f325d6beef1b97f70</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>DisplayOff</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>aca17a0e980345f32f642e9fc191739f1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>FillScreen</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>adb2b49881c18f817f273e7162507910c</anchor>
      <arglist>(isf15acp4::Color565 color) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Present</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>ad5e21b9498546d51df7a018b48ab066d</anchor>
      <arglist>(const isf15acp4::GraphicsContext &amp;gfx) noexcept</arglist>
    </member>
    <member kind="function">
      <type>isf15acp4::GraphicsContext</type>
      <name>CreateGraphicsContext</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>af6982e9774cf67b36c717b0a2e8f9933</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPressed</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>aac8657c879eab461ad4d3a2f6d47ace9</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>visitDriver</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a2805feb4c0fa354eab088fc2c5570f73</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept</arglist>
    </member>
    <member kind="function">
      <type>isf15acp4::SmartDisplay&lt; HalIsf15acp4SpiAdapter &gt; *</type>
      <name>GetDriver</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a4516cd9767f58ea06281f4fe45c89d7b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const isf15acp4::SmartDisplay&lt; HalIsf15acp4SpiAdapter &gt; *</type>
      <name>GetDriver</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a4404a9c735ddc6b8283d221440444e9c</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>isf15acp4::SmartDisplay&lt; HalIsf15acp4SpiAdapter &gt; *</type>
      <name>GetDisplay</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>ad819b256485215a82400257ebdea6cab</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>GetErrorFlags</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a3ebe4d164acca466559fff122a27d571</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ClearErrorFlags</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a0ec305b0d923d390d02e564128e29406</anchor>
      <arglist>(uint16_t mask=0xFFFF) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a63b4eacec756f432bbeff6ca50834c17</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>ensureInitializedUnlocked</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a8dd1e89148ea18ca2c33a26efd563d52</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>deinitializeUnlocked</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a8b5cd314c4b987a8c13e8b80ef7d8422</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseSpi &amp;</type>
      <name>spi_ref_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a35bc51aebf66db73ae80583658d2a465</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>cs_ref_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a5c9c442bc72199d51bb19f9a43e7b445</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>dc_ref_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>ac94a3bd70e7977abecdcd6b673bf7a76</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio &amp;</type>
      <name>reset_ref_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>af9a2867952241bf2821511803242a60f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>vcc_enable_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a37eeb131f02276a7f58eb597e1cf57f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>switch_in_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a241af480a72e94a372e08a62e0edfba1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>switch_active_low_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a3f65d1da2261940ea84519b1718a7d8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Isf15acp4HandlerConfig</type>
      <name>config_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a1ff14601a47467dbf80180e8028dcaea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalIsf15acp4SpiAdapter &gt;</type>
      <name>adapter_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a367cefde13375b1aac0de76a990e4da3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; isf15acp4::SmartDisplay&lt; HalIsf15acp4SpiAdapter &gt; &gt;</type>
      <name>display_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>a1349ce57c648b43cee9392eb93bef981</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>abcde0f81c19a1a9838cfe6de108ea771</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classIsf15acp4Handler.html</anchorfile>
      <anchor>acaa3c9fb20e55ce042631755706b75f4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Isf15acp4HandlerConfig</name>
    <filename>structIsf15acp4HandlerConfig.html</filename>
    <member kind="variable">
      <type>isf15acp4::ProductVariant</type>
      <name>variant</name>
      <anchorfile>structIsf15acp4HandlerConfig.html</anchorfile>
      <anchor>a74c911c8509d5c3788c51c3fa8115a4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>isf15acp4::GraphicsBackend</type>
      <name>graphics</name>
      <anchorfile>structIsf15acp4HandlerConfig.html</anchorfile>
      <anchor>ad88c8d9002d37a1d03ecd829a3413fdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_vcc_on_init</name>
      <anchorfile>structIsf15acp4HandlerConfig.html</anchorfile>
      <anchor>ad66188aea8fe63a65dfc8adec7e2fced</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LogConfig</name>
    <filename>structLogConfig.html</filename>
    <member kind="variable">
      <type>LogLevel</type>
      <name>level</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>a514266f193b4fc2d8a8c4c2601e8ac1a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>LogColor</type>
      <name>color</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>a855f3f5df85dce55ceb7b591f19d5b92</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>LogBackground</type>
      <name>background</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>a71a3b458ea8826514bec6d8bcb9eec6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>LogStyle</type>
      <name>style</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>aed58684ab39633b59b907f6b3ab58b84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_colors</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>a66996cfd3e1bae906e081d7f14f24e02</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_effects</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>ac71d8aefc25d70efb4484bd352d8290d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>max_width</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>a1b6aafe5e326ebeeb603560b0e6a8a6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>center_text</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>abd0066ddfb5a4e514177b5d6edc2d1f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>add_border</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>ac55e8067844a376dd7016a092ce19f73</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>border_char</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>a8d3c4c711a7e1bf65115b878b1959d83</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>border_padding</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>a3d2ecc60bd064b16857e25eff432d9bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_ascii_art</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>a78b904caf7c610787ef1c3d9c7fbb16c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>format_ascii_art</name>
      <anchorfile>structLogConfig.html</anchorfile>
      <anchor>ab2bec83242fdfe9d6bd2cfc05ade0824</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Logger</name>
    <filename>classLogger.html</filename>
    <class kind="struct">Logger::TagLevel</class>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>adcdc1901fd35d1e06b5c8cd47005daad</anchor>
      <arglist>(const LogConfig &amp;config=LogConfig{}) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a92b1c55e7bcac05a7a518717e220216e</anchor>
      <arglist>(const LogConfig &amp;config, std::unique_ptr&lt; BaseLogger &gt; backend) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Deinitialize</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a6730678df56c64dbb626c1b739f64b72</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>ab8af01e14cae7cb4bc0f82f369f7757c</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetLogLevel</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a0e83e1b37ca2c702f8828f38d662cc23</anchor>
      <arglist>(const char *tag, LogLevel level) noexcept</arglist>
    </member>
    <member kind="function">
      <type>LogLevel</type>
      <name>GetLogLevel</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>ae436878411ce3f0a080ffdf5140cd58e</anchor>
      <arglist>(const char *tag) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetTagLevel</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>aff32d6ea1ead65ddf069aa8c609570c9</anchor>
      <arglist>(const char *tag, LogLevel level) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Error</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>adce17848a5844e053bfd314863dc1404</anchor>
      <arglist>(const char *tag, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Warn</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>aaa95ffa14d07a965d4a85bdec82cba26</anchor>
      <arglist>(const char *tag, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Info</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a4681229c521795685fc14f6b55c82300</anchor>
      <arglist>(const char *tag, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Debug</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>abd2f9efde67ebd50c458b996a15c8417</anchor>
      <arglist>(const char *tag, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Verbose</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>acde0013df4b5a021affaee1b35e487bc</anchor>
      <arglist>(const char *tag, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Error</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>adf27e2ee35e622c18cf7eb92f47b134c</anchor>
      <arglist>(const char *tag, LogColor color, LogStyle style, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Warn</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a2a10a88618a574494c90b8431a986f0b</anchor>
      <arglist>(const char *tag, LogColor color, LogStyle style, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Info</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a8c8e9fac8521c28b5b1346d1a4d1ab8a</anchor>
      <arglist>(const char *tag, LogColor color, LogStyle style, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Debug</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a77448fdfa976956c22198cb038819b66</anchor>
      <arglist>(const char *tag, LogColor color, LogStyle style, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Verbose</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a0469274fd372e5a6c4b786dd8729b246</anchor>
      <arglist>(const char *tag, LogColor color, LogStyle style, const char *format,...) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>LogAsciiArt</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a95d97b7713dad6000fbf87a95b266af9</anchor>
      <arglist>(const char *tag, const char *ascii_art, const AsciiArtFormat &amp;format=AsciiArtFormat{}) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>LogAsciiArt</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a4985924d10370f1f8dbf7af2ce930fce</anchor>
      <arglist>(LogLevel level, const char *tag, const char *ascii_art, const AsciiArtFormat &amp;format=AsciiArtFormat{}) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>LogBanner</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a2403e1d9b67029c159e0f9f6e27419dd</anchor>
      <arglist>(const char *tag, const char *ascii_art, const AsciiArtFormat &amp;format=AsciiArtFormat{}) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetConfig</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>ac78253f1f745833880231e9f3dc0ad00</anchor>
      <arglist>(const LogConfig &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type>LogConfig</type>
      <name>GetConfig</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a362dfbfeeb211dedd7e234e769b4aaa3</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EnableColors</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a18d970d2f45661f20decd61c4c7283fd</anchor>
      <arglist>(bool enable) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EnableEffects</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a218ded86af8f2fe90dcd027bda3e3aae</anchor>
      <arglist>(bool enable) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>EnableAsciiArt</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>aac24f377445cbd2597aeb9c72fb6327b</anchor>
      <arglist>(bool enable) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Flush</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a366cd04b097aee22e68ca8668dad1c35</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Logger &amp;</type>
      <name>GetInstance</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a5789d11f8f1356e2192c0023a089cbc4</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>Logger</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>af4376d745e05cf7af3db5921e6d6adeb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>~Logger</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a843a878e056f9c2c835d9bddffb6692a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>Logger</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>add0f34d4c753c86af8074840a72366b3</anchor>
      <arglist>(const Logger &amp;)=delete</arglist>
    </member>
    <member kind="function" protection="private">
      <type>Logger &amp;</type>
      <name>operator=</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a8c0ca770cdc91ca9fc3015286568e1ea</anchor>
      <arglist>(const Logger &amp;)=delete</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>Logger</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a92cabd934eeaf6a83a0ebf40236eb529</anchor>
      <arglist>(Logger &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function" protection="private">
      <type>Logger &amp;</type>
      <name>operator=</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>aea05eaeaa020b687d8ec39ec6e72b0c9</anchor>
      <arglist>(Logger &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>LogInternal</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a6adb205db597526fb91ad20067400394</anchor>
      <arglist>(LogLevel level, const char *tag, LogColor color, LogStyle style, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>size_t</type>
      <name>WriteColorPrefix</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a51cd2efad577caf4ef6472bc2479dc2e</anchor>
      <arglist>(char *buf, size_t buf_size, LogColor color, LogBackground background, LogStyle style) const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>FormatAndLogAsciiArt</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a866b8fb309235b82816e6c7ffa3b3879</anchor>
      <arglist>(const char *tag, LogLevel level, const char *ascii_art, const AsciiArtFormat &amp;format) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>IsLevelEnabled</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>af9e84e0ff6bd538e4f9ed8dcebc3e2f0</anchor>
      <arglist>(LogLevel level, const char *tag) const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>DumpStatistics</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>ade1c385c9e613ee1cc72865048858b22</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static size_t</type>
      <name>WriteResetSequence</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a7cac3fc4eb37a54b9e8af19d0450b935</anchor>
      <arglist>(char *buf, size_t buf_size) noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static std::unique_ptr&lt; BaseLogger &gt;</type>
      <name>CreateDefaultBaseLogger</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a1523d9809debb29550c21c8c8f3dee8b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; bool &gt;</type>
      <name>initialized_</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a607eecea0ac44ec8bbe052c00949a9a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LogConfig</type>
      <name>config_</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a14d456263913f3ae7f05e30326796836</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>TagLevel</type>
      <name>tag_levels_</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>aa51410b7a39c658d24127f87c0e38636</anchor>
      <arglist>[kMaxTagLevels]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; BaseLogger &gt;</type>
      <name>base_logger_</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a6f694e77741e4abe06e49ef7dd379acd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static constexpr size_t</type>
      <name>kMaxTagLevels</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a1d59aa839504e8d05f08927c2617fb67</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static constexpr size_t</type>
      <name>kMaxTagLength</name>
      <anchorfile>classLogger.html</anchorfile>
      <anchor>a3d7ee6286d92d880a680743d8a1af8f6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Max22200Handler</name>
    <filename>classMax22200Handler.html</filename>
    <member kind="typedef">
      <type>max22200::MAX22200&lt; HalSpiMax22200Comm &gt;</type>
      <name>DriverType</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a9c3881de11e1af4a850fc81ecb98f932</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Max22200Handler</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a980ab91fe8af480657541343c4f1eb90</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;enable, BaseGpio &amp;cmd, BaseGpio *fault=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Max22200Handler</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>ac4714672494c6253e2b9e95f9b5d32a6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Max22200Handler</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a2e66dfcba97f15e640743125f640b0d9</anchor>
      <arglist>(const Max22200Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Max22200Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>abf7d8a8e79978ac0554f802bd154a0f9</anchor>
      <arglist>(const Max22200Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Max22200Handler</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>aa15b316c8fd5d5d0f0e87ca8d67f01eb</anchor>
      <arglist>(Max22200Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Max22200Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>ac7ff1b8ec100de179014d9e651dc6e20</anchor>
      <arglist>(Max22200Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>Initialize</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a0dce05cd6639a10e52c0f669b72baf52</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a803b60663755d7850857626b58efc084</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>Initialize</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>aca940fd8213ecb633bafb7c3e143a361</anchor>
      <arglist>(const max22200::BoardConfig &amp;board_config) noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>Deinitialize</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a96276218d9af520726b43745bcd0f0f5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a84534ddd9d77b88a861f093bd5a587de</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>GetLastFaultByte</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>ac60a7cd413fc94d0eff7529aa626643e</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>ConfigureChannel</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a7dbfc381539a08e3bfab394db5739145</anchor>
      <arglist>(uint8_t channel, const max22200::ChannelConfig &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>SetupCdrChannel</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>ad723710c3ee6c0519b6bde20925b1bf8</anchor>
      <arglist>(uint8_t channel, uint16_t hit_ma, uint16_t hold_ma, float hit_time_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>SetupVdrChannel</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a6892c6f6c6efdef080d9a485e4de2223</anchor>
      <arglist>(uint8_t channel, float hit_duty_pct, float hold_duty_pct, float hit_time_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>EnableChannel</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a7f49cdd0e0e2686f560a1d26221e44de</anchor>
      <arglist>(uint8_t channel) noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>DisableChannel</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a250cfb6dbfcff2b145d6356b35b52560</anchor>
      <arglist>(uint8_t channel) noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>EnableAllChannels</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>af7aba4f68825068e774badca04c49a07</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>DisableAllChannels</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a9e9684818c7efddd45697f28051cbca2</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelEnabled</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a07c395b7496c7da567fb6a8d58d9d992</anchor>
      <arglist>(uint8_t channel) noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>SetChannelsMask</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a6417f65fa49d539915fa5ca712fffc2f</anchor>
      <arglist>(uint8_t mask) noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>GetStatus</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a650e6b9666b785a5b3eadc729e187556</anchor>
      <arglist>(max22200::StatusConfig &amp;status) noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>GetChannelFaults</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>ac593ec1cad7b098494a53b327521cbb6</anchor>
      <arglist>(uint8_t channel, max22200::FaultStatus &amp;faults) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HasFault</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>ac5b60b117fec404c55597929134fa1a1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>ClearFaults</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a963d65edae16cca4fd153b3af0c1e68e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>max22200::DriverStatus</type>
      <name>ReadFaultRegister</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a9a09340fc22f485e50feacbacc25318b</anchor>
      <arglist>(max22200::FaultStatus &amp;faults) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>WithDriver</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a0bb0a70cc02384071ce92a83ad2acbc9</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept</arglist>
    </member>
    <member kind="function">
      <type>DriverType *</type>
      <name>GetDriver</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a001e00b9d9a02611b51951f16bcb78d1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const DriverType *</type>
      <name>GetDriver</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>aee42537a3d2f0955a1479709b2d4b4b8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>afd07a624dc736a1452c0f6190499adc0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint8_t</type>
      <name>kNumChannels</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a8505ffd513d2ebedcd107bc452407da1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>max22200::DriverStatus</type>
      <name>InitializeLocked</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>abad786f185502af5b31b94835e424b15</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>aa00edacb247660349767fa4521121104</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>WaitForActiveAndDrainFaults</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a46a1ed5c96b117192f0cae05c4f9627b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>auto</type>
      <name>withDriver</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>abcdd9746a0df9c28bb36508d52d80ef5</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a7551cc6aef14367b839f97c1dc096b1a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>mutex_</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a08079ccf08dd0068f39e2a4a208de9a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalSpiMax22200Comm &gt;</type>
      <name>comm_</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a24eb0c32f2cf5360a3882523e7eeb08f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; DriverType &gt;</type>
      <name>driver_</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>a748af00c00ba68752bf7590f04b588e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>last_fault_byte_</name>
      <anchorfile>classMax22200Handler.html</anchorfile>
      <anchor>ae92b7d84d748444d428ad487da488d7c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Mcp9700AdcAdapter</name>
    <filename>classMcp9700AdcAdapter.html</filename>
    <member kind="function">
      <type></type>
      <name>Mcp9700AdcAdapter</name>
      <anchorfile>classMcp9700AdcAdapter.html</anchorfile>
      <anchor>a41d01bd35afe352ceb3795bd2b222e1a</anchor>
      <arglist>(BaseAdc *adc) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classMcp9700AdcAdapter.html</anchorfile>
      <anchor>a856744a834009968357b3bd4d3624f77</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ReadChannelV</name>
      <anchorfile>classMcp9700AdcAdapter.html</anchorfile>
      <anchor>ae7afa72701b078c5d31f96e5df09d48c</anchor>
      <arglist>(uint8_t channel, float *voltage_v) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseAdc *</type>
      <name>adc_</name>
      <anchorfile>classMcp9700AdcAdapter.html</anchorfile>
      <anchor>a03e1b3f8d08c2784d96f8910dc0ecd58</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Mcp9700TemperatureHandler</name>
    <filename>classMcp9700TemperatureHandler.html</filename>
    <member kind="function">
      <type></type>
      <name>Mcp9700TemperatureHandler</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>ab1e1b3c4030c574bb3706bced614b9d7</anchor>
      <arglist>(BaseAdc *adc_interface, uint8_t adc_channel, const char *sensor_name=&quot;MCP9700&quot;) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Mcp9700TemperatureHandler</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>af85f241a3eb2bb63593721db57c72745</anchor>
      <arglist>(const Mcp9700TemperatureHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Mcp9700TemperatureHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>a8396b4c67d6d53ede751624941aca8e9</anchor>
      <arglist>(const Mcp9700TemperatureHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Mcp9700TemperatureHandler</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>ae42e3bcbe2b4451680706b0932f50685</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>a63afda9405fae8a648e668bc33ba75a7</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>a5308ab9a70750306607b6dea72ff9814</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_temp_err_t</type>
      <name>ReadTemperatureCelsiusImpl</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>ade877860a7c76e6e427f8f619bc6d8d6</anchor>
      <arglist>(float *temperature_celsius) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_temp_err_t</type>
      <name>GetSensorInfo</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>a810ee7b9fff07ee32b4ec4f3e189ecc1</anchor>
      <arglist>(hf_temp_sensor_info_t *info) const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_u32_t</type>
      <name>GetCapabilities</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>a51c72b9c6e14b0101b6502f11ba8db71</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>mutex_</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>ac1764c73f551490d2ccfa6513a888626</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseAdc *</type>
      <name>adc_interface_</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>a18909b46c2493dc0523c3631ab9b61bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>adc_channel_</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>abf624e6bb8b67a8c23cd4b4cfe711621</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const char *</type>
      <name>sensor_name_</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>a4b5046aa32c320408dd14e8a40a68e28</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Mcp9700AdcAdapter &gt;</type>
      <name>adc_adapter_</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>a81a580ca2f8787f3ecb9d0a406d6cf3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Mcp9700ThermistorConcrete &gt;</type>
      <name>thermistor_</name>
      <anchorfile>classMcp9700TemperatureHandler.html</anchorfile>
      <anchor>a0d438ea914afac7aadb0adf324f5df8f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>anonymous_namespace{Pcal95555Handler.cpp}::NonOwningGpioDeleter</name>
    <filename>structanonymous__namespace_02Pcal95555Handler_8cpp_03_1_1NonOwningGpioDeleter.html</filename>
    <member kind="function">
      <type>void</type>
      <name>operator()</name>
      <anchorfile>structanonymous__namespace_02Pcal95555Handler_8cpp_03_1_1NonOwningGpioDeleter.html</anchorfile>
      <anchor>a4e7a759b732a66c57d2b3586a1bf1871</anchor>
      <arglist>(BaseGpio *) const noexcept</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ntc_temp_handler_config_t</name>
    <filename>structntc__temp__handler__config__t.html</filename>
    <member kind="variable">
      <type>NtcType</type>
      <name>ntc_type</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a4cd9eaf9faa4e56997d992ef32c8b4f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>adc_channel</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>aa5264b0ce82a4acb21dbff6b97e70269</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>voltage_divider_series_resistance</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a1a0d98402c32fd45d02f0f1563431974</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>voltage_divider_parallel_resistance</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>acace50cf7adfbc10f44a24ddc51206a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>reference_voltage</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a72a484bd74a9566e5ed9f2f725720840</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>calibration_offset</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a47eb8618368565320305658f25ac0ce4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>beta_value</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a7b398b9523e46c323ebdb6bcec52b13f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>NtcConversionMethod</type>
      <name>conversion_method</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>aa83a38e6f657a9eefe13a27dc9ae5728</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>sample_count</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a45b7f94d6946c1a44c1582fa52cf488b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>sample_delay_ms</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a7d7a099da75dfe23c6e2655ff69e2bf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>min_temperature</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a6566ae0df558de5dcf441bd8b80c46d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>max_temperature</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a71caa44219cb7b02284922e7841835ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_filtering</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a2a105fd17b2141df71209860c293e25a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>filter_alpha</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a95bb304af9a3de1f85dc6f271b8a97ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>enable_threshold_monitoring</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>aff249634eb00623eaf5a3937d093fd2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>low_threshold_celsius</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a3610e55ba831ff2f0d989591bcd654bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>high_threshold_celsius</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a6d47bea340cdf6e4f10630984298f016</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>hf_temp_threshold_callback_t</type>
      <name>threshold_callback</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>ab9f8e4c25e67bf7bb1dfef59ec373491</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>threshold_user_data</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>abc3fc2c8c2afd4dc8ee6c5e2343d0f20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>sensor_name</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>ad472c99be0f43c23de2d9e73d2415acd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>sensor_description</name>
      <anchorfile>structntc__temp__handler__config__t.html</anchorfile>
      <anchor>a4368245a90e7dab56f447327930f8e82</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NtcAdcAdapter</name>
    <filename>classNtcAdcAdapter.html</filename>
    <member kind="function">
      <type></type>
      <name>NtcAdcAdapter</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>a967dc70fa9d0da926422801a8b8f8047</anchor>
      <arglist>(BaseAdc *adc, float ref_voltage=3.3f, uint8_t res_bits=12) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>a57cbe8c6f89c0a8544d72144eaff0b07</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>a2304c9e4022fdf6031d4155ea06dc342</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelAvailable</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>a2950ee775d52e680818aa016ec27ca88</anchor>
      <arglist>(uint8_t channel) const</arglist>
    </member>
    <member kind="function">
      <type>ntc::AdcError</type>
      <name>ReadChannelCount</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>a6d3ec827ad427fc3cf214cb7dd441fed</anchor>
      <arglist>(uint8_t channel, uint32_t *count)</arglist>
    </member>
    <member kind="function">
      <type>ntc::AdcError</type>
      <name>ReadChannelV</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>af23fadc3050a19a877f0f3effb89f8a3</anchor>
      <arglist>(uint8_t channel, float *voltage_v)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>GetReferenceVoltage</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>a29f8858c1a9c49beb34f408e397e6ccf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>GetResolutionBits</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>a2711b318c0702e8d89969dc60d2ad222</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseAdc *</type>
      <name>adc_</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>a77d0264a70af77ffdde42459fb8812bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>reference_voltage_</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>a5fda436c5e70119b24fe172a4f0f9b90</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>resolution_bits_</name>
      <anchorfile>classNtcAdcAdapter.html</anchorfile>
      <anchor>af962ac38399e0efcb0d68c41e6316b57</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NtcTemperatureHandler</name>
    <filename>classNtcTemperatureHandler.html</filename>
    <member kind="function">
      <type></type>
      <name>NtcTemperatureHandler</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a02f7492acfb66633e01e0018bf60563b</anchor>
      <arglist>(NtcType ntc_type, BaseAdc *adc_interface, const char *sensor_name=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NtcTemperatureHandler</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a041211e8ef52c17de195fed5b1c74a9f</anchor>
      <arglist>(BaseAdc *adc_interface, const ntc_temp_handler_config_t &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NtcTemperatureHandler</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a0845dd73d1b2678a058022e53ab1438a</anchor>
      <arglist>(const NtcTemperatureHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>NtcTemperatureHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a4255991124dc803676fdfd6923e1232d</anchor>
      <arglist>(const NtcTemperatureHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NtcTemperatureHandler</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a2c3640034987b3d7b27b1d2b96971d56</anchor>
      <arglist>(NtcTemperatureHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>NtcTemperatureHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a03aae04e77e9492e498d7c4a927f1365</anchor>
      <arglist>(NtcTemperatureHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NtcTemperatureHandler</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>aac69a65bcccfecfc2bfca99f7ac2aeb3</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a97fa93b2ed6337e24e056b46fbe331c7</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a8c1949119402a748e1000e473e263703</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>ReadTemperatureCelsiusImpl</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a61337aaf7bb60d2b89a526b1531d773d</anchor>
      <arglist>(float *temperature_celsius) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>GetSensorInfo</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a8d529921d7de8911e42ed5ba0af27762</anchor>
      <arglist>(hf_temp_sensor_info_t *info) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u32_t</type>
      <name>GetCapabilities</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a8eb439565fe06b8e62fb87ad9ee0b0a1</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>SetRange</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a4fa05ac2c5ffc286cf9786a8d551f781</anchor>
      <arglist>(float min_celsius, float max_celsius) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>GetRange</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ae536f854573a886c66b049cfd725ad0b</anchor>
      <arglist>(float *min_celsius, float *max_celsius) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>GetResolution</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ab6887eb8e112ab7711e554c6598bfd79</anchor>
      <arglist>(float *resolution_celsius) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>SetThresholds</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a275ae5ee2756034dcaa99f3877d8e1f0</anchor>
      <arglist>(float low_threshold_celsius, float high_threshold_celsius) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>GetThresholds</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ab8fbb0a6a931a265dd8c4fa1d5822931</anchor>
      <arglist>(float *low_threshold_celsius, float *high_threshold_celsius) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>EnableThresholdMonitoring</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ae3ed02cf4cd05dd73a3e9a42f9219f3e</anchor>
      <arglist>(hf_temp_threshold_callback_t callback, void *user_data) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>DisableThresholdMonitoring</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a3991d34f48052ae90f6bfc1ad0d5c1cb</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>StartContinuousMonitoring</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>aeabac754a1b923c79969657d69b97a72</anchor>
      <arglist>(hf_u32_t sample_rate_hz, hf_temp_reading_callback_t callback, void *user_data) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>StopContinuousMonitoring</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a7cd3aa59989bec0514d6717d345d8466</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsMonitoringActive</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>aa66c83e9176cca65468c7b8483e0b5ee</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>SetCalibrationOffset</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>af69b8450dddaf1ffeece5c586f318875</anchor>
      <arglist>(float offset_celsius) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>GetCalibrationOffset</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>af825904df06c2cf42c1c515257908e49</anchor>
      <arglist>(float *offset_celsius) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>ResetCalibration</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>afbd49ab0a549ce22cada91fcd498b6d7</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>EnterSleepMode</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a059ed62988ed8c4e69ac891b448ff17a</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>ExitSleepMode</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a3bc0497cbb414d6cb346a2a519e8b0b7</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsSleeping</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a4c4dba5256ded055f5b3461f8d435286</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>SelfTest</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>adb887f49bc1fc44e23482aef76a707b7</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>CheckHealth</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a9fe1485d86dd6d2b3f3e5f1d8545d24c</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>GetStatistics</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a2fc99e0c62cd5df47b3e1025019014f0</anchor>
      <arglist>(hf_temp_statistics_t &amp;statistics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>GetDiagnostics</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a553eca244e975cebe3f1f03632130e56</anchor>
      <arglist>(hf_temp_diagnostics_t &amp;diagnostics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>ResetStatistics</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a20cfb11a0f83dda45e22ee4815b2c6d7</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>ResetDiagnostics</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>adb609ac3f90e26a005a28c0d0056f638</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>GetNtcThermistor</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a5cf221687209404add0524dc3022d1b7</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const void *</type>
      <name>GetNtcThermistor</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ad65823c5d4324a1c92445c36cdac3feb</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>GetNtcReading</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a881e82a3cd93b8d3596dddfa5ce921a7</anchor>
      <arglist>(ntc_reading_t *reading) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>GetNtcConfiguration</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a164f6bbba3aa697788ee410c49582937</anchor>
      <arglist>(ntc_config_t *config) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>SetNtcConfiguration</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a7241123186a31ae2bccd66c1e042e789</anchor>
      <arglist>(const ntc_config_t &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>GetResistance</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>af151920d28d327aaf2fa94cde3a9ead6</anchor>
      <arglist>(float *resistance_ohms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>GetVoltage</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a4a76971e79afcda6fcc375f52f08a589</anchor>
      <arglist>(float *voltage_volts) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>GetRawAdcValue</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>aa685b6c62984e1112a1bc87b26a2df7d</anchor>
      <arglist>(uint32_t *adc_value) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>Calibrate</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ab9723f043883e044f077a2627b600fdb</anchor>
      <arglist>(float reference_temperature_celsius) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>SetConversionMethod</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a5aca6936df7c82d48a1e91c8a348c965</anchor>
      <arglist>(NtcConversionMethod method) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>SetVoltageDivider</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a6f587e3c3ec35f09364655144e325a36</anchor>
      <arglist>(float series_resistance) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>SetReferenceVoltage</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ae0c1091e78debcf710e7dc98c370405b</anchor>
      <arglist>(float reference_voltage) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>SetBetaValue</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a528b33b82898bc5b7bfa1cfd5ef88791</anchor>
      <arglist>(float beta_value) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>SetAdcChannel</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>aa9bacb44871dfb3c82f139f7cc55059e</anchor>
      <arglist>(uint8_t adc_channel) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>SetSamplingParameters</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>aaa2a51bfdacb214f281f2562966d4db7</anchor>
      <arglist>(uint32_t sample_count, uint32_t sample_delay_ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcError</type>
      <name>SetFiltering</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a007ed81a3d0a23c01a556682aff1d0d5</anchor>
      <arglist>(bool enable, float alpha=0.1f) noexcept</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetSensorName</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a2d252331ccdbcab2093f8aac6d2fefbb</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetSensorDescription</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a8eb11d54de41d7ce2a1a67c4b2af1188</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>NtcThermistorConcrete *</type>
      <name>GetDriver</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a509435457c33078b831da827c9eff953</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const NtcThermistorConcrete *</type>
      <name>GetDriver</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a38a4e75151e0fc16ad746de8a2cd7144</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>SetLastError</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a5b148f36777ccff9fb633e3f4e99050f</anchor>
      <arglist>(hf_temp_err_t error) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>UpdateStatistics</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a2c8404fac12fa3e3d3655cd4430d2049</anchor>
      <arglist>(bool operation_successful, hf_u32_t operation_time_us) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>UpdateDiagnostics</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ab867e35301fff064ce0e76dd85033f8c</anchor>
      <arglist>(hf_temp_err_t error) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>InitializeNtcThermistor</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a12fdb422ddbde5931a567803c9a8eef6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_temp_err_t</type>
      <name>ConvertNtcError</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>af9018fa033d296d30d6420886ad09701</anchor>
      <arglist>(NtcError ntc_error) const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>CheckThresholds</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>acc84378578731f61a0a368b2913ea034</anchor>
      <arglist>(float temperature_celsius) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>UpdateBaseDiagnostics</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>addc61cbd9e69f84bb6014a92b1c63496</anchor>
      <arglist>(hf_temp_err_t error) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_temp_err_t</type>
      <name>ValidateConfiguration</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a4a54d746f5c2b474bf66e4ad7e3bb37d</anchor>
      <arglist>(const ntc_temp_handler_config_t &amp;config) const noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>ContinuousMonitoringCallback</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>aaaef3aab6d69f967a416db9f858c9d1c</anchor>
      <arglist>(uint32_t arg)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static hf_u32_t</type>
      <name>RegisterMonitoringContext</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ae173c55a63a51d2344798dd785b25ebe</anchor>
      <arglist>(NtcTemperatureHandler *handler) noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>UnregisterMonitoringContext</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a0b3fba99650d0b01b1347eae0c86abba</anchor>
      <arglist>(hf_u32_t context_id) noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static NtcTemperatureHandler *</type>
      <name>ResolveMonitoringContext</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a2a459d108af3fc0c29d518c62cb791fe</anchor>
      <arglist>(hf_u32_t context_id) noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static hf_u64_t</type>
      <name>GetCurrentTimeUs</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>adffe3f391437c3d39a56bd632ff8807e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>mutex_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a13433ca31eb877b27d39f398ee5b0d6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; NtcAdcAdapter &gt;</type>
      <name>ntc_adc_adapter_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a0456ab6b452345e3e406f9941e8deccb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; NtcThermistorConcrete &gt;</type>
      <name>ntc_thermistor_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>acc20655e17adf28accf22244fd30e628</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseAdc *</type>
      <name>adc_interface_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a69d2acbb9fad640b19febc3db8f76773</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>ntc_temp_handler_config_t</type>
      <name>config_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a1c8980812318a321753cd5ba821f35a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_temp_state_t</type>
      <name>current_state_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a74b308efe15b28f81e5c48f23470fba1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_temp_config_t</type>
      <name>base_config_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a5b96b430cfec01f2190e57977e0ca4e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>low_threshold_celsius_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a433bfb281cb15b6c1e1f57224b8bd7a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>high_threshold_celsius_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a1ad76c89cb99d1752b1e8a37e996ef90</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_temp_threshold_callback_t</type>
      <name>threshold_callback_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ac3d4059b519f6a7c5503503a688cec17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>void *</type>
      <name>threshold_user_data_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>aee4337b29ad1d9b34b3a1c8266bb102e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_u32_t</type>
      <name>sample_rate_hz_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a4970e4b901d824cd772fa331e03db063</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_temp_reading_callback_t</type>
      <name>continuous_callback_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>af1eb1c7171b474d8d121b7b443e99067</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>void *</type>
      <name>continuous_user_data_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a83d109b4035a696dac6b1b82bfab5976</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>PeriodicTimer</type>
      <name>monitoring_timer_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ab71fd7339661fe72a7f7760883fd8e38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_u32_t</type>
      <name>monitoring_context_id_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a335b006f6f0c150dd53bf38052067cc5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>calibration_offset_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a00691e05139ae9348465decef54b1a66</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_temp_statistics_t</type>
      <name>statistics_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a3ccd083e07c279322e87a4202edcf09d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_temp_diagnostics_t</type>
      <name>diagnostics_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a6f1cba555ce4adf0309324532dfb60e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a53c8c72f999e93781d7f4d41ef3e1903</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>threshold_monitoring_enabled_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>aa4b3ca60a65dc5da71b7a3d4b9405f09</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>monitoring_active_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>af312fddeecba9fcccef5edf9d05b1c91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static constexpr hf_u32_t</type>
      <name>kMonitoringContextSlots</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>ae2f7041f1b34c60db7afb47f3cb9f6a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static RtosMutex</type>
      <name>callback_registry_mutex_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a7445e75a00f245e9a74ee1d846d3248e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::array&lt; NtcTemperatureHandler *, kMonitoringContextSlots &gt;</type>
      <name>callback_registry_</name>
      <anchorfile>classNtcTemperatureHandler.html</anchorfile>
      <anchor>a9c7c1fa912406565388527569f87cc98</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>sm_test::Owner</name>
    <filename>structsm__test_1_1Owner.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>EnterA</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a32ac9e2d3be9e75d553630f1e018597e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>LoopA</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>ad979b85defb29aa7c54e6677b1103d31</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ExitA</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>ae52690ba46d70038627a5f0e2d712e44</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnterB</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a7bd3578b06104cd2e934a6007f0e6d7d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>LoopB</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a9704ec4f1c2d9832877e2c601477895b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ExitB</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a16ae0e1e824053ce713f26af159d0158</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnterC</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a121adbf5ca46738ecb8e9546f2d05aef</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>LoopC</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>aaa94d40139da7c86e06da38ac60b95ac</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ExitC</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a4e40d560ba9718f4180b78b6b35f0f7b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>entry_a</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a168d5bfadcab7dcc9a9dfbcbcb8900c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>loop_a</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a6f3e82d7f9b063877cdb54c07efc1b5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>exit_a</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a9d67602ebef6ff2edd388ce7c402bf33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>entry_b</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a99453eb2de5572a5b480bbdaef900e05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>loop_b</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a3495fd85d5773d251b3b05aef5ea0a69</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>exit_b</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a580cf548fb6d455e4a503a8487159013</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>entry_c</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a48fec521088acbf6a6ad517621545b81</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>loop_c</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>ae0de1040a7ef725d55c759b51999fc59</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>exit_c</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a22c8f6576091863bb1dd4b239cef4205</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>entry_b_should_fail</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a895859248eebca109c88c270c030f635</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>exit_a_should_fail</name>
      <anchorfile>structsm__test_1_1Owner.html</anchorfile>
      <anchor>a09b8f9509e916ad3e506180ad7e8c5b5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Pca9685GpioPin</name>
    <filename>classPca9685GpioPin.html</filename>
    <member kind="function">
      <type></type>
      <name>Pca9685GpioPin</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a64892b616d817bd04eace3ab2c4ef4a8</anchor>
      <arglist>(hf_pin_num_t pin, Pca9685Handler *parent_handler, hf_gpio_active_state_t active_state=hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Pca9685GpioPin</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ad911e7d226d0b94dcde49c1830567b1b</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>aad888b977f799ca7c4f1f35eb9644311</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>afbbc12ce4cfbaba86fbc39a7274f3905</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPinAvailable</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a5048dc8c0666a6dec81a9730010a3253</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxPins</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ae807550a0353d368df72b46e2e30fa88</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a9c781e39d938496324ce607b0d395fd5</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetDirectionImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ad15fb1c64afe5c1b6e006db5551e9b7b</anchor>
      <arglist>(hf_gpio_direction_t direction) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetDirectionImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>aa1e8393edc0e47ab7b93e0240cc32b46</anchor>
      <arglist>(hf_gpio_direction_t &amp;direction) const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetOutputModeImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ae989f1d52889ef958acf9c5b565d3b99</anchor>
      <arglist>(hf_gpio_output_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetOutputModeImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ac5a7fafa84eb25f9e5991da64d24a1c7</anchor>
      <arglist>(hf_gpio_output_mode_t &amp;mode) const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPullModeImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ae3ee349427e82d8069ac2935ae7cde00</anchor>
      <arglist>(hf_gpio_pull_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_pull_mode_t</type>
      <name>GetPullModeImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a72238f788bf8728fb1dda06c3c4a8d35</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPinLevelImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a2775690e26ad55ca12be078f7a7867de</anchor>
      <arglist>(hf_gpio_level_t level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetPinLevelImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>abd8b1dc6117988dcfe332fa33beca673</anchor>
      <arglist>(hf_gpio_level_t &amp;level) noexcept override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_pin_num_t</type>
      <name>pin_</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a0cee0fcf79159c62d601f391a409b6bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Pca9685Handler *</type>
      <name>parent_handler_</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a30c9e37f305e5df75f0c9d56362351e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a89c70897f0e8a4103617de6959248cc4</anchor>
      <arglist>[32]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_gpio_level_t</type>
      <name>cached_level_</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a2792209cdd313f933bab8f83b8ba3a78</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="protected">
      <type>friend class</type>
      <name>Pca9685Handler</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ab0f74b52dfc8f21be1f8fa9c791f5970</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>aad888b977f799ca7c4f1f35eb9644311</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>afbbc12ce4cfbaba86fbc39a7274f3905</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPinAvailable</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a5048dc8c0666a6dec81a9730010a3253</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxPins</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ae807550a0353d368df72b46e2e30fa88</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a9c781e39d938496324ce607b0d395fd5</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetDirectionImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ad15fb1c64afe5c1b6e006db5551e9b7b</anchor>
      <arglist>(hf_gpio_direction_t direction) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetDirectionImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>aa1e8393edc0e47ab7b93e0240cc32b46</anchor>
      <arglist>(hf_gpio_direction_t &amp;direction) const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetOutputModeImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ae989f1d52889ef958acf9c5b565d3b99</anchor>
      <arglist>(hf_gpio_output_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetOutputModeImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ac5a7fafa84eb25f9e5991da64d24a1c7</anchor>
      <arglist>(hf_gpio_output_mode_t &amp;mode) const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPullModeImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>ae3ee349427e82d8069ac2935ae7cde00</anchor>
      <arglist>(hf_gpio_pull_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_pull_mode_t</type>
      <name>GetPullModeImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a72238f788bf8728fb1dda06c3c4a8d35</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPinLevelImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>a2775690e26ad55ca12be078f7a7867de</anchor>
      <arglist>(hf_gpio_level_t level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetPinLevelImpl</name>
      <anchorfile>classPca9685GpioPin.html</anchorfile>
      <anchor>abd8b1dc6117988dcfe332fa33beca673</anchor>
      <arglist>(hf_gpio_level_t &amp;level) noexcept override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Pca9685Handler</name>
    <filename>classPca9685Handler.html</filename>
    <member kind="typedef">
      <type>pca9685::PCA9685&lt; HalI2cPca9685Comm &gt;</type>
      <name>Pca9685Driver</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a98f3e67474173eeb234d4cc3e4b4d03b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pca9685Handler</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a7dfc66449a73d316e0101e4c539a3e28</anchor>
      <arglist>(BaseI2c &amp;i2c_device) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Pca9685Handler</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ad78af0db8495a5ed739debd52291394b</anchor>
      <arglist>() noexcept=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pca9685Handler</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a2690cb463ddcf97dbb99301295081725</anchor>
      <arglist>(const Pca9685Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Pca9685Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a74ed42bd622f96bf7c25e38551ebb000</anchor>
      <arglist>(const Pca9685Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pca9685Handler</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>aaaccb9942a46b78ecb4ca1e289a34019</anchor>
      <arglist>(Pca9685Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Pca9685Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ada9cbb30d3f4772b200cc7e2b4fe376a</anchor>
      <arglist>(Pca9685Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a088d4c2eecbd5c39480a5dddf73d91d3</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureDeinitialized</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ac6bc3289adec2e6e48a07545023770b0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ae9ec3bcf1629765a26d627e48afc10b1</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>GetErrorFlags</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>aca456553a9db5bca8f474fe7cf456d9d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ClearErrorFlags</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a81164b84390312d4971a435eeb464930</anchor>
      <arglist>(uint16_t mask=0xFFFF) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ae86db8a2e163cd62cfd4459047f1f204</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>Pca9685Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a91f6e14b606d4007797234ec9599a134</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const Pca9685Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a7d3b7d292f41670fcb1b1ecfc803b404</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>ensureInitializedLocked</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>abc72cbb1c0ffd4410143d52c605e0e49</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_pwm_err_t</type>
      <name>initializeInternal</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ab7b5cd2320a6bb69ff6a2869051e9d5e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_pwm_err_t</type>
      <name>deinitializeInternal</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a8fbc5e520828ef7e52dea2fd8497b20c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>validateChannel</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a8d6e386b561fb3ac96b9afb5386ad239</anchor>
      <arglist>(uint8_t channel) const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>SetFrequency</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>adcdc86029bb4028ddcc8a3a099ffe635</anchor>
      <arglist>(float freq_hz) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>SetDuty</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a1d7ea1f902efe9dabc6fea9b97e3e369</anchor>
      <arglist>(uint8_t channel, float duty) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>SetPwm</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>aa40bf3968e55af7c7912870165d47297</anchor>
      <arglist>(uint8_t channel, uint16_t on_time, uint16_t off_time) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>SetAllPwm</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a2dbb42acc6660c88b281e8b96e84e41e</anchor>
      <arglist>(uint16_t on_time, uint16_t off_time) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>SetChannelFullOn</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>addfe3b4c674c0b59eaba2945035ac13d</anchor>
      <arglist>(uint8_t channel) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>SetChannelFullOff</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a6175f653b325713b0c80c5808ccbe45a</anchor>
      <arglist>(uint8_t channel) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>Sleep</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a989c4b1a8260c7af0bf210103d13cc7b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>Wake</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>abfea96039e525e253d0eab9234cdfa5f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>SetOutputInvert</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a6e145fe83cb71c5a5f32fe397ad70e1a</anchor>
      <arglist>(bool invert) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>SetOutputDriverMode</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>aa447ac4404a59220bdf4de022feb89ed</anchor>
      <arglist>(bool totem_pole) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_device_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ae96ede1c1515f2909c6525aee7329880</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalI2cPca9685Comm &gt;</type>
      <name>i2c_adapter_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a3c76cb3a2531c1ce7c69d29b486e17f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Pca9685Driver &gt;</type>
      <name>pca9685_driver_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ab6cf923c2fc823d73aeffb552798960a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a1f185ee5489b94affcc4791f6007a50c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a78ed285da5d0316a7c9c5a171363fbd0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::shared_ptr&lt; Pca9685PwmAdapter &gt;</type>
      <name>pwm_adapter_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a6f6e37a5f910fb63c8d02db3192eb296</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; std::shared_ptr&lt; Pca9685GpioPin &gt;, 16 &gt;</type>
      <name>gpio_registry_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a719fcc60d8e8b0d3b3b075e146b6767d</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>Pca9685PwmAdapter</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>afdbebb557c0701e93df0f183ea1461b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>Pca9685GpioPin</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a7fe3fe37643cadff2b344cd4fbc8ec13</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>pca9685::PCA9685&lt; HalI2cPca9685Comm &gt;</type>
      <name>Pca9685Driver</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a98f3e67474173eeb234d4cc3e4b4d03b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_device_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ae96ede1c1515f2909c6525aee7329880</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalI2cPca9685Comm &gt;</type>
      <name>i2c_adapter_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a3c76cb3a2531c1ce7c69d29b486e17f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Pca9685Driver &gt;</type>
      <name>pca9685_driver_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ab6cf923c2fc823d73aeffb552798960a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a1f185ee5489b94affcc4791f6007a50c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a78ed285da5d0316a7c9c5a171363fbd0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::shared_ptr&lt; Pca9685PwmAdapter &gt;</type>
      <name>pwm_adapter_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a6f6e37a5f910fb63c8d02db3192eb296</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; std::shared_ptr&lt; Pca9685GpioPin &gt;, 16 &gt;</type>
      <name>gpio_registry_</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a719fcc60d8e8b0d3b3b075e146b6767d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pca9685Handler</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a7dfc66449a73d316e0101e4c539a3e28</anchor>
      <arglist>(BaseI2c &amp;i2c_device) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Pca9685Handler</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ad78af0db8495a5ed739debd52291394b</anchor>
      <arglist>() noexcept=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pca9685Handler</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a2690cb463ddcf97dbb99301295081725</anchor>
      <arglist>(const Pca9685Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Pca9685Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a74ed42bd622f96bf7c25e38551ebb000</anchor>
      <arglist>(const Pca9685Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pca9685Handler</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>aaaccb9942a46b78ecb4ca1e289a34019</anchor>
      <arglist>(Pca9685Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Pca9685Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ada9cbb30d3f4772b200cc7e2b4fe376a</anchor>
      <arglist>(Pca9685Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a088d4c2eecbd5c39480a5dddf73d91d3</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureDeinitialized</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ac6bc3289adec2e6e48a07545023770b0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ae9ec3bcf1629765a26d627e48afc10b1</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>GetI2cAddress</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>aba9c3a3673589629f4e20e1dd24c01c8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::shared_ptr&lt; BasePwm &gt;</type>
      <name>GetPwmAdapter</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a3f5b93ead4b85c137c94a0d0e8d9bb12</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::shared_ptr&lt; BaseGpio &gt;</type>
      <name>CreateGpioPin</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a478a14f9dd0ff9c5569bbbebebe7f830</anchor>
      <arglist>(hf_pin_num_t channel, hf_gpio_active_state_t active_state=hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH, bool allow_existing=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::shared_ptr&lt; BaseGpio &gt;</type>
      <name>GetGpioPin</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a7ac083c548ae83b06654ec25571ce4e1</anchor>
      <arglist>(hf_pin_num_t channel) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPinCreated</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>aa6b5dea8e955f60443f8972c478c6018</anchor>
      <arglist>(hf_pin_num_t channel) const noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static constexpr uint8_t</type>
      <name>ChannelCount</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a849d184def0cac7aa1fa9179659318ea</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>GetErrorFlags</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>aca456553a9db5bca8f474fe7cf456d9d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ClearErrorFlags</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a81164b84390312d4971a435eeb464930</anchor>
      <arglist>(uint16_t mask=0xFFFF) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>ae86db8a2e163cd62cfd4459047f1f204</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>Pca9685Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a91f6e14b606d4007797234ec9599a134</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const Pca9685Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPca9685Handler.html</anchorfile>
      <anchor>a7d3b7d292f41670fcb1b1ecfc803b404</anchor>
      <arglist>() const noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Pca9685PwmAdapter</name>
    <filename>classPca9685PwmAdapter.html</filename>
    <member kind="function">
      <type></type>
      <name>Pca9685PwmAdapter</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aff11c1ac9e63fd735a76497582a54bf0</anchor>
      <arglist>(Pca9685Handler *parent_handler) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Pca9685PwmAdapter</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aae4166b69c6c96f3429f2f42a2dba83a</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>Initialize</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a4444ef91c028f448f8fd2d06a4d90992</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>Deinitialize</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aca699f9fffd3ddf238ca3e833e85b919</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>EnableChannel</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a1ae2b30150d427183e7f171daf0af720</anchor>
      <arglist>(hf_channel_id_t channel_id) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>DisableChannel</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>ae561cd717f09691cd9c45961ef739eb9</anchor>
      <arglist>(hf_channel_id_t channel_id) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelEnabled</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a5bdbecb058703d5b5f6f59fac8daa5b2</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetDutyCycle</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a2f565d01eb539c60ef8266688bc3824c</anchor>
      <arglist>(hf_channel_id_t channel_id, float duty_cycle) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetDutyCycleRaw</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aefb37b4910fa3c338f25ddf88b0a1f5f</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t raw_value) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetFrequency</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a849333230ae977939266c075d5830a38</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_frequency_hz_t frequency_hz) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetPhaseShift</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a6cd746620e622a273998807c6a260b7d</anchor>
      <arglist>(hf_channel_id_t channel_id, float phase_shift_degrees) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>StartAll</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aabd9412420647133c10651ec3caed0d4</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>StopAll</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a825a19eb0b16cbf9ebb93a7b6da115d8</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>UpdateAll</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a20b32c0131c0265e0c50b2f7d66677f3</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetComplementaryOutput</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a0442088d54116c40d4644aa59307f5c0</anchor>
      <arglist>(hf_channel_id_t primary_channel, hf_channel_id_t complementary_channel, hf_u32_t deadtime_ns) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>GetDutyCycle</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>ad8f8441e2fc34f1d1331f79e37bc80af</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_frequency_hz_t</type>
      <name>GetFrequency</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aac49436a3981c258dab38f8770d8d827</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint8_t</type>
      <name>kMaxChannels</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>af55760adbbae32af83d3d4f68219f822</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint8_t</type>
      <name>kResolutionBits</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>ab14fef2196431d10dc3c4347ba3a7d67</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint16_t</type>
      <name>kMaxRawValue</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a4fae40d04fe23c1bda49465a724ca260</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint32_t</type>
      <name>kMinFrequencyHz</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a2625228b6dc6f6edb8bebb4f69f668da</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint32_t</type>
      <name>kMaxFrequencyHz</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a99fbcd37f8ec3a7147f09895b6c8d0ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>validateChannel</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a33dc3b0fc7647b69a384329c1d3b1841</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Pca9685Handler *</type>
      <name>parent_handler_</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a813045693ec697af5a3dde89a1b1ba74</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_frequency_hz_t</type>
      <name>current_frequency_hz_</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aee02c1bfca7d8dae6d0c1bf6e662b9a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; float, kMaxChannels &gt;</type>
      <name>duty_cache_</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a5bf85e1725e3bb94fd5ee76d6e71c185</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; uint16_t, kMaxChannels &gt;</type>
      <name>on_time_cache_</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a0938303a73472f97cb89bd38247ec8a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; bool, kMaxChannels &gt;</type>
      <name>channel_enabled_</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>ae66c071510d76dedf3c0eca2eeb6a5f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>Pca9685Handler</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>ab0f74b52dfc8f21be1f8fa9c791f5970</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; float, kMaxChannels &gt;</type>
      <name>duty_cache_</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a5bf85e1725e3bb94fd5ee76d6e71c185</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; uint16_t, kMaxChannels &gt;</type>
      <name>on_time_cache_</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a0938303a73472f97cb89bd38247ec8a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; bool, kMaxChannels &gt;</type>
      <name>channel_enabled_</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>ae66c071510d76dedf3c0eca2eeb6a5f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>Initialize</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a4444ef91c028f448f8fd2d06a4d90992</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>Deinitialize</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aca699f9fffd3ddf238ca3e833e85b919</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>EnableChannel</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a1ae2b30150d427183e7f171daf0af720</anchor>
      <arglist>(hf_channel_id_t channel_id) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>DisableChannel</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>ae561cd717f09691cd9c45961ef739eb9</anchor>
      <arglist>(hf_channel_id_t channel_id) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelEnabled</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a5bdbecb058703d5b5f6f59fac8daa5b2</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetDutyCycle</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a2f565d01eb539c60ef8266688bc3824c</anchor>
      <arglist>(hf_channel_id_t channel_id, float duty_cycle) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetDutyCycleRaw</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aefb37b4910fa3c338f25ddf88b0a1f5f</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t raw_value) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetFrequency</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a849333230ae977939266c075d5830a38</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_frequency_hz_t frequency_hz) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetPhaseShift</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a6cd746620e622a273998807c6a260b7d</anchor>
      <arglist>(hf_channel_id_t channel_id, float phase_shift_degrees) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>StartAll</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aabd9412420647133c10651ec3caed0d4</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>StopAll</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a825a19eb0b16cbf9ebb93a7b6da115d8</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>UpdateAll</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a20b32c0131c0265e0c50b2f7d66677f3</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_pwm_err_t</type>
      <name>SetComplementaryOutput</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>a0442088d54116c40d4644aa59307f5c0</anchor>
      <arglist>(hf_channel_id_t primary_channel, hf_channel_id_t complementary_channel, hf_u32_t deadtime_ns) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>GetDutyCycle</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>ad8f8441e2fc34f1d1331f79e37bc80af</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_frequency_hz_t</type>
      <name>GetFrequency</name>
      <anchorfile>classPca9685PwmAdapter.html</anchorfile>
      <anchor>aac49436a3981c258dab38f8770d8d827</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Pcal95555GpioPin</name>
    <filename>classPcal95555GpioPin.html</filename>
    <member kind="function">
      <type></type>
      <name>Pcal95555GpioPin</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>ac198902065a7c14f6774fa2809fb5e5a</anchor>
      <arglist>(hf_pin_num_t pin, Pcal95555Handler *parent_handler, hf_gpio_direction_t direction=hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT, hf_gpio_active_state_t active_state=hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH, hf_gpio_output_mode_t output_mode=hf_gpio_output_mode_t::HF_GPIO_OUTPUT_MODE_PUSH_PULL, hf_gpio_pull_mode_t pull_mode=hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING, bool configure_hardware=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Pcal95555GpioPin</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a0f4144b4e01a57439b4b1a43f1779ef6</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a49b393914fbb14d0a5f32e4a597babb2</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a18fb43e58cfbe361b98c794fee82ea28</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPinAvailable</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a35e544950f4da17316cb606689b0267c</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxPins</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>af691c00c9d8bdb267615e167619df676</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>ae14f1c39878874431091328573b4ea4b</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SupportsInterrupts</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a4b3f3c93b648f3d6cd3142078cba0575</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ConfigureInterrupt</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>adfadb2f35d756b7660c28fd419120c01</anchor>
      <arglist>(hf_gpio_interrupt_trigger_t trigger, InterruptCallback callback=nullptr, void *user_data=nullptr) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetPolarityInversion</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a0c315eb82ad0bf0c657721e2648e459c</anchor>
      <arglist>(bool invert) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetInterruptMask</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>aa5db6f60106badc53e4e0f73e4ab7cda</anchor>
      <arglist>(bool mask) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>GetInterruptStatus</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a77dc4cf1bc87b1d10e60cc9afc6615cb</anchor>
      <arglist>(bool &amp;status) noexcept</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetDirectionImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>ade4b792a73b556e5779bab9ccfdd5020</anchor>
      <arglist>(hf_gpio_direction_t direction) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetOutputModeImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a22754d3c7f698126efe9cfbc1aab08a5</anchor>
      <arglist>(hf_gpio_output_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPullModeImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a019aa1c94129daff3c817d73499cbb9c</anchor>
      <arglist>(hf_gpio_pull_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_pull_mode_t</type>
      <name>GetPullModeImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a85cd9e01dbca0dda8433f0f2a361f3a6</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPinLevelImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a288b0ae1dab2cd0a26ae1f5e45efc9fb</anchor>
      <arglist>(hf_gpio_level_t level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetPinLevelImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>af4d577512778712ca5b0953890201873</anchor>
      <arglist>(hf_gpio_level_t &amp;level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetDirectionImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>abbb57e223ea40abaee447b37ffc36162</anchor>
      <arglist>(hf_gpio_direction_t &amp;direction) const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetOutputModeImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>af00051e1bc17a2b8c422489afb6ba3b8</anchor>
      <arglist>(hf_gpio_output_mode_t &amp;mode) const noexcept override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_pin_num_t</type>
      <name>pin_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a4ad94123075d15cf74df599d636af2b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Pcal95555Handler *</type>
      <name>parent_handler_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>ac7e0433e0b48c39e7304f651cb8f6a43</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a539b73d425615efef5cdb64ac4a1347c</anchor>
      <arglist>[32]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>configure_hardware_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a026cdfce1af4973beab83aa92fd8e527</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>InterruptCallback</type>
      <name>interrupt_callback_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a336231d16e990c8f59fb96dcf90e89ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>void *</type>
      <name>interrupt_user_data_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a238f1f99f7fbdc64d5bcbeab2f88bf02</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_gpio_interrupt_trigger_t</type>
      <name>interrupt_trigger_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>af2afb724ee06cc95b3488be24eb04b2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>interrupt_enabled_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a8b3bbdf5d209bd8784bbbae6a5e3cb54</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="protected">
      <type>friend class</type>
      <name>Pcal95555Handler</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a227c584070b705ad70832ca30f5d0adf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>InterruptCallback</type>
      <name>interrupt_callback_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a336231d16e990c8f59fb96dcf90e89ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>void *</type>
      <name>interrupt_user_data_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a238f1f99f7fbdc64d5bcbeab2f88bf02</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_gpio_interrupt_trigger_t</type>
      <name>interrupt_trigger_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>af2afb724ee06cc95b3488be24eb04b2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>interrupt_enabled_</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a8b3bbdf5d209bd8784bbbae6a5e3cb54</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a49b393914fbb14d0a5f32e4a597babb2</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a18fb43e58cfbe361b98c794fee82ea28</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPinAvailable</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a35e544950f4da17316cb606689b0267c</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxPins</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>af691c00c9d8bdb267615e167619df676</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>ae14f1c39878874431091328573b4ea4b</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SupportsInterrupts</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a4b3f3c93b648f3d6cd3142078cba0575</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ConfigureInterrupt</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>adfadb2f35d756b7660c28fd419120c01</anchor>
      <arglist>(hf_gpio_interrupt_trigger_t trigger, InterruptCallback callback=nullptr, void *user_data=nullptr) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetPolarityInversion</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a0c315eb82ad0bf0c657721e2648e459c</anchor>
      <arglist>(bool invert) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetInterruptMask</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>aa5db6f60106badc53e4e0f73e4ab7cda</anchor>
      <arglist>(bool mask) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>GetInterruptStatus</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a77dc4cf1bc87b1d10e60cc9afc6615cb</anchor>
      <arglist>(bool &amp;status) noexcept</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetDirectionImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>ade4b792a73b556e5779bab9ccfdd5020</anchor>
      <arglist>(hf_gpio_direction_t direction) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetOutputModeImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a22754d3c7f698126efe9cfbc1aab08a5</anchor>
      <arglist>(hf_gpio_output_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPullModeImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a019aa1c94129daff3c817d73499cbb9c</anchor>
      <arglist>(hf_gpio_pull_mode_t mode) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_pull_mode_t</type>
      <name>GetPullModeImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a85cd9e01dbca0dda8433f0f2a361f3a6</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>SetPinLevelImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>a288b0ae1dab2cd0a26ae1f5e45efc9fb</anchor>
      <arglist>(hf_gpio_level_t level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetPinLevelImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>af4d577512778712ca5b0953890201873</anchor>
      <arglist>(hf_gpio_level_t &amp;level) noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetDirectionImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>abbb57e223ea40abaee447b37ffc36162</anchor>
      <arglist>(hf_gpio_direction_t &amp;direction) const noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>hf_gpio_err_t</type>
      <name>GetOutputModeImpl</name>
      <anchorfile>classPcal95555GpioPin.html</anchorfile>
      <anchor>af00051e1bc17a2b8c422489afb6ba3b8</anchor>
      <arglist>(hf_gpio_output_mode_t &amp;mode) const noexcept override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Pcal95555Handler</name>
    <filename>classPcal95555Handler.html</filename>
    <member kind="typedef">
      <type>pcal95555::PCAL95555&lt; HalI2cPcal95555Comm &gt;</type>
      <name>Pcal95555Driver</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a79a4140f8340810bea7bed218a78df6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pcal95555Handler</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>af60d10d8b4968523231592f915dfa264</anchor>
      <arglist>(BaseI2c &amp;i2c_device, BaseGpio *interrupt_pin=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Pcal95555Handler</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a43bd4812929f7c68e6f63d60911f76dd</anchor>
      <arglist>() noexcept=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pcal95555Handler</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a6079fb16f640bfccf3a2b757ca30e637</anchor>
      <arglist>(const Pcal95555Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Pcal95555Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a441e27219b0d01cc054cff4ca1314a02</anchor>
      <arglist>(const Pcal95555Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pcal95555Handler</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a610f1a2888ff268b4994c9bb7cf878cd</anchor>
      <arglist>(Pcal95555Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Pcal95555Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aab51c31bafd93a83547c4aa80ca562ec</anchor>
      <arglist>(Pcal95555Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a255f8ed3461301b509ff4ae9f6e78c51</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureDeinitialized</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a305b8bc372d595fd0f176daa5bf9c6d1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a691bee4095f01be0e2665f6f764bcc7b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetDirection</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a5ff0ad1ac3c4dcaf45dc4c0d27827da7</anchor>
      <arglist>(uint8_t pin, hf_gpio_direction_t direction) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetOutput</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a4591e7da481679beac69c20bbd554db1</anchor>
      <arglist>(uint8_t pin, bool active) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ReadInput</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a32cb9ad0e9a4cf9e804e30420900edde</anchor>
      <arglist>(uint8_t pin, bool &amp;active) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>Toggle</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>adb6abd365588fa5386e4bf78ac913f05</anchor>
      <arglist>(uint8_t pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetPullMode</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ab1ea2933e065e8a224874ba21cb5bf43</anchor>
      <arglist>(uint8_t pin, hf_gpio_pull_mode_t pull_mode) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>GetPullMode</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a1228e87bf2287b442883106c275a0bda</anchor>
      <arglist>(uint8_t pin, hf_gpio_pull_mode_t &amp;pull_mode) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetDirections</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a6475963048e57f95207bead94c0b46f9</anchor>
      <arglist>(uint16_t pin_mask, hf_gpio_direction_t direction) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetOutputs</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a6ba1e5c9ca60f28a0dad4eae7fc2c4b6</anchor>
      <arglist>(uint16_t pin_mask, bool active) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetOutputsVerified</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aa74e1628bd1f61b6e5c1367dada481dd</anchor>
      <arglist>(uint16_t pin_mask, bool active) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetPullModes</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a72ce8f8a0f79654cfeb2a4ce7e7559d8</anchor>
      <arglist>(uint16_t pin_mask, hf_gpio_pull_mode_t pull_mode) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HasAgileIO</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a997583346dffe6c95e5f8fcfc127c37b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>pcal95555::ChipVariant</type>
      <name>GetChipVariant</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a2087e09275d692c42a3425631dc5dbc8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetPolarityInversion</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a31401008daec249d3a06a98aa9a336e6</anchor>
      <arglist>(hf_pin_num_t pin, bool invert) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetMultiplePolarityInversion</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a3095861c90ba05b81eb89c4ff3813663</anchor>
      <arglist>(uint16_t pin_mask, bool invert) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetInterruptMask</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a693538e569689249d3e6ee19872a4972</anchor>
      <arglist>(hf_pin_num_t pin, bool mask) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>GetInterruptStatus</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a0db4e8fc6e472672e6d11b5f3962b7e4</anchor>
      <arglist>(hf_pin_num_t pin, bool &amp;status) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetDriveStrength</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a3c1cf846d6d3966f3d3b5b724a47a6bb</anchor>
      <arglist>(hf_pin_num_t pin, DriveStrength level) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>EnableInputLatch</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a45a4692f11ec58be6ec754779d4b55d3</anchor>
      <arglist>(hf_pin_num_t pin, bool enable) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetOutputMode</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ab248e8fe39a2772ddfc4777eb322c5ad</anchor>
      <arglist>(bool port0_open_drain, bool port1_open_drain) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ResetToDefault</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ab614d32aa8d2c6bb775ebbbd148875b3</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>GetErrorFlags</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a56cc0f2c707f3e8913fe5eb734dff3df</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ClearErrorFlags</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>afcb849e480d9f660d8f3f6c50b813fc6</anchor>
      <arglist>(uint16_t mask=0xFFFF) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a7fcf65a91f94a68330ee6738f8297b83</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a7d8ed72bf4fc9dfcb0be4877deec99b3</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_gpio_err_t</type>
      <name>Initialize</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a5c2e68d6c9e39e373d40e94f5e951558</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_gpio_err_t</type>
      <name>Deinitialize</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a233b1596b075c64ee9de7ba5d78342be</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>ValidatePin</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a8211811d3e4416232fe534f8c5652740</anchor>
      <arglist>(uint8_t pin) const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_gpio_err_t</type>
      <name>RegisterPinInterrupt</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a59b2dcc6d426c19a487175e8a83ed999</anchor>
      <arglist>(hf_pin_num_t pin, hf_gpio_interrupt_trigger_t trigger, InterruptCallback callback, void *user_data) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_gpio_err_t</type>
      <name>UnregisterPinInterrupt</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a49e5b18fcbd6762701009b564b90d481</anchor>
      <arglist>(hf_pin_num_t pin) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_gpio_err_t</type>
      <name>ConfigureHardwareInterrupt</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>add6bf45bbac47b7d8d579fbd9bc50d2e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>ProcessInterrupts</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a7b9a82a463f57e0394b55a4f3ecead39</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>Pcal95555Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a6715e51d5d09a2466b3b86820b9f5ea6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>const Pcal95555Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a8121efea49cc4be54c4526eb346e5f8d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>HardwareInterruptCallback</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a87e8b50faf5d3d815227068ff26b1a6b</anchor>
      <arglist>(BaseGpio *gpio, hf_gpio_interrupt_trigger_t trigger, void *user_data) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_device_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a3d89af46dc9c4927390e92834ce47242</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalI2cPcal95555Comm &gt;</type>
      <name>i2c_adapter_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a28dc7229076f6b2b3455e337bd296bf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Pcal95555Driver &gt;</type>
      <name>pcal95555_driver_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a19c7623e661a824f5341b08420a3dd69</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>afc2d7b418279e4b3dd42a03e43025252</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ada54239b8d7a1b3429070e8325d2c208</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; Pcal95555GpioPin *, 16 &gt;</type>
      <name>pin_registry_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aec7e78d4ce7e63c8f8d1b336b09e5cae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; std::unique_ptr&lt; Pcal95555GpioPin &gt;, 16 &gt;</type>
      <name>owned_pins_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aca6bd0126a289ef68ea3a846943d153d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; hf_gpio_pull_mode_t, 16 &gt;</type>
      <name>pull_mode_cache_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aa2bb96cbbd906cb83a398e3a8d310bc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>prev_input_state_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a41f16f23becbbc45984e1adfed1320b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>output_shadow_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a11cd669d0c9317619e4eba98e2f43dbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>config_shadow_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a863827b584dd3a129343827039a7190a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>polarity_shadow_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ae7b92323d2fbb92fc1033b124c900599</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>port_shadow_valid_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a651563d023f252c7eb4ef8c241b3f8a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>Pcal95555GpioPin</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ab5ff10bc52cad940896de791a45cc012</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>pcal95555::PCAL95555&lt; HalI2cPcal95555Comm &gt;</type>
      <name>Pcal95555Driver</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a79a4140f8340810bea7bed218a78df6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_device_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a3d89af46dc9c4927390e92834ce47242</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalI2cPcal95555Comm &gt;</type>
      <name>i2c_adapter_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a28dc7229076f6b2b3455e337bd296bf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Pcal95555Driver &gt;</type>
      <name>pcal95555_driver_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a19c7623e661a824f5341b08420a3dd69</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>afc2d7b418279e4b3dd42a03e43025252</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ada54239b8d7a1b3429070e8325d2c208</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; Pcal95555GpioPin *, 16 &gt;</type>
      <name>pin_registry_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aec7e78d4ce7e63c8f8d1b336b09e5cae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; std::unique_ptr&lt; Pcal95555GpioPin &gt;, 16 &gt;</type>
      <name>owned_pins_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aca6bd0126a289ef68ea3a846943d153d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>interrupt_pin_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a620cb1a1051db58b8d6dd7d50b1ef2d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>interrupt_configured_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a3ddb306f8da6122a74f20f234dda0470</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; bool &gt;</type>
      <name>interrupt_pending_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a9f3c4bd5c353cdd988fc9e4d39641ee6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HasInterruptSupport</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a582a8375e50b2df78dfd91aa132955af</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInterruptConfigured</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>adcf9f009e24b393d9ba92a9d3d8e316e</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>DrainPendingInterrupts</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a2e8fc9ad39143e2cc26a2e6a9d711a7e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>GetAllInterruptMasks</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a63e5f4a2b43c2b8dc3f2b5e2532d4277</anchor>
      <arglist>(uint16_t &amp;mask) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>GetAllInterruptStatus</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aee0a26c02aa3a04a4bce58513f9854b3</anchor>
      <arglist>(uint16_t &amp;status) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; hf_gpio_pull_mode_t, 16 &gt;</type>
      <name>pull_mode_cache_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aa2bb96cbbd906cb83a398e3a8d310bc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>prev_input_state_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a41f16f23becbbc45984e1adfed1320b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>output_shadow_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a11cd669d0c9317619e4eba98e2f43dbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>config_shadow_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a863827b584dd3a129343827039a7190a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>polarity_shadow_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ae7b92323d2fbb92fc1033b124c900599</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>port_shadow_valid_</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a651563d023f252c7eb4ef8c241b3f8a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pcal95555Handler</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>af60d10d8b4968523231592f915dfa264</anchor>
      <arglist>(BaseI2c &amp;i2c_device, BaseGpio *interrupt_pin=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Pcal95555Handler</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a43bd4812929f7c68e6f63d60911f76dd</anchor>
      <arglist>() noexcept=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pcal95555Handler</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a6079fb16f640bfccf3a2b757ca30e637</anchor>
      <arglist>(const Pcal95555Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Pcal95555Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a441e27219b0d01cc054cff4ca1314a02</anchor>
      <arglist>(const Pcal95555Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pcal95555Handler</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a610f1a2888ff268b4994c9bb7cf878cd</anchor>
      <arglist>(Pcal95555Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Pcal95555Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aab51c31bafd93a83547c4aa80ca562ec</anchor>
      <arglist>(Pcal95555Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a255f8ed3461301b509ff4ae9f6e78c51</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureDeinitialized</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a305b8bc372d595fd0f176daa5bf9c6d1</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a691bee4095f01be0e2665f6f764bcc7b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetDirection</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a5ff0ad1ac3c4dcaf45dc4c0d27827da7</anchor>
      <arglist>(uint8_t pin, hf_gpio_direction_t direction) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetOutput</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a4591e7da481679beac69c20bbd554db1</anchor>
      <arglist>(uint8_t pin, bool active) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ReadInput</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a32cb9ad0e9a4cf9e804e30420900edde</anchor>
      <arglist>(uint8_t pin, bool &amp;active) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>Toggle</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>adb6abd365588fa5386e4bf78ac913f05</anchor>
      <arglist>(uint8_t pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetPullMode</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ab1ea2933e065e8a224874ba21cb5bf43</anchor>
      <arglist>(uint8_t pin, hf_gpio_pull_mode_t pull_mode) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>GetPullMode</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a1228e87bf2287b442883106c275a0bda</anchor>
      <arglist>(uint8_t pin, hf_gpio_pull_mode_t &amp;pull_mode) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetDirections</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a6475963048e57f95207bead94c0b46f9</anchor>
      <arglist>(uint16_t pin_mask, hf_gpio_direction_t direction) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetOutputs</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a6ba1e5c9ca60f28a0dad4eae7fc2c4b6</anchor>
      <arglist>(uint16_t pin_mask, bool active) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetOutputsVerified</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>aa74e1628bd1f61b6e5c1367dada481dd</anchor>
      <arglist>(uint16_t pin_mask, bool active) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetPullModes</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a72ce8f8a0f79654cfeb2a4ce7e7559d8</anchor>
      <arglist>(uint16_t pin_mask, hf_gpio_pull_mode_t pull_mode) noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>GetI2cAddress</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a0a5df15d919f0d07454f2485ded1aa29</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>AttachStaticPin</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a28d96ee5b65b2019ad925857da3b0fc9</anchor>
      <arglist>(Pcal95555GpioPin &amp;pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::shared_ptr&lt; BaseGpio &gt;</type>
      <name>CreateGpioPin</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a8aaf65b110fcc05a55588abc448c338c</anchor>
      <arglist>(hf_pin_num_t pin, hf_gpio_direction_t direction=hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT, hf_gpio_active_state_t active_state=hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH, hf_gpio_output_mode_t output_mode=hf_gpio_output_mode_t::HF_GPIO_OUTPUT_MODE_PUSH_PULL, hf_gpio_pull_mode_t pull_mode=hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING, bool allow_existing=true, bool configure_hardware=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::shared_ptr&lt; BaseGpio &gt;</type>
      <name>GetGpioPin</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ae2f28f1eaaf4c26798167d1b4c7487f0</anchor>
      <arglist>(hf_pin_num_t pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>Pcal95555GpioPin *</type>
      <name>GetGpioPinRaw</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a7a0768a2f350d9bca3b10e1dbbc029b4</anchor>
      <arglist>(hf_pin_num_t pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ReadAllInputLevels</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a2ff2904be993d89b71dc798c1ec5eb04</anchor>
      <arglist>(uint16_t &amp;levels) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ProgramPortsAbsolute</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ae0767335c4d2594a03df060ed820eefe</anchor>
      <arglist>(uint16_t config, uint16_t output, uint16_t out_check_mask) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ReadConfigAndOutputPorts</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a001163d478f30c49dc3f5afec27ac17c</anchor>
      <arglist>(uint16_t &amp;config, uint16_t &amp;output) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ReadOutputLatchFromWire</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a57bc7b4236fb5cb437c9e31f922a4829</anchor>
      <arglist>(uint16_t &amp;output) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ReadStandardRegisterBank</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a14bddc4daa10a06e8db9213e2fb7f656</anchor>
      <arglist>(uint16_t &amp;input, uint16_t &amp;output, uint16_t &amp;polarity, uint16_t &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>VerifyOutputPinEffect</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a7c9cfff643067cc85279cc373a11d6ae</anchor>
      <arglist>(uint8_t pin, uint16_t &amp;latch_high, uint16_t &amp;input_high, uint16_t &amp;latch_low, uint16_t &amp;input_low) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPinCreated</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a2c3999b62728839cd55ab4f03a54bba1</anchor>
      <arglist>(hf_pin_num_t pin) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>GetCreatedPinMask</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a1d1e49208d5dbed704c4af0ad110ac2b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static constexpr uint8_t</type>
      <name>PinCount</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a8635d65049f9d416d201b47614a4b024</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HasAgileIO</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a997583346dffe6c95e5f8fcfc127c37b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>pcal95555::ChipVariant</type>
      <name>GetChipVariant</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a2087e09275d692c42a3425631dc5dbc8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetPolarityInversion</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a31401008daec249d3a06a98aa9a336e6</anchor>
      <arglist>(hf_pin_num_t pin, bool invert) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetMultiplePolarityInversion</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a3095861c90ba05b81eb89c4ff3813663</anchor>
      <arglist>(uint16_t pin_mask, bool invert) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetInterruptMask</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a693538e569689249d3e6ee19872a4972</anchor>
      <arglist>(hf_pin_num_t pin, bool mask) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>GetInterruptStatus</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a0db4e8fc6e472672e6d11b5f3962b7e4</anchor>
      <arglist>(hf_pin_num_t pin, bool &amp;status) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetDriveStrength</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a3c1cf846d6d3966f3d3b5b724a47a6bb</anchor>
      <arglist>(hf_pin_num_t pin, DriveStrength level) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>EnableInputLatch</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a45a4692f11ec58be6ec754779d4b55d3</anchor>
      <arglist>(hf_pin_num_t pin, bool enable) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>SetOutputMode</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ab248e8fe39a2772ddfc4777eb322c5ad</anchor>
      <arglist>(bool port0_open_drain, bool port1_open_drain) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_gpio_err_t</type>
      <name>ResetToDefault</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>ab614d32aa8d2c6bb775ebbbd148875b3</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>GetErrorFlags</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a56cc0f2c707f3e8913fe5eb734dff3df</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ClearErrorFlags</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>afcb849e480d9f660d8f3f6c50b813fc6</anchor>
      <arglist>(uint16_t mask=0xFFFF) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a7fcf65a91f94a68330ee6738f8297b83</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>Pcal95555Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a6715e51d5d09a2466b3b86820b9f5ea6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>const Pcal95555Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPcal95555Handler.html</anchorfile>
      <anchor>a8121efea49cc4be54c4526eb346e5f8d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Pf1550Handler</name>
    <filename>classPf1550Handler.html</filename>
    <member kind="typedef">
      <type>pf1550::PF1550&lt; HalPf1550Comm &gt;</type>
      <name>Pf1550Driver</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>ab501ac50f2c41743896ef54288fc0754</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pf1550Handler</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>ae30b621d785e845aa326c76e43013ef5</anchor>
      <arglist>(BaseI2c &amp;i2c, BaseGpio *standby_gpio=nullptr, BaseGpio *usb_vbus_en_gpio=nullptr, BaseGpio *usb_otg_en_gpio=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Pf1550Handler</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>adec0348246886793fe1a8ea358352b18</anchor>
      <arglist>(const Pf1550Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Pf1550Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a2ef720c33c7eb432f65073754df95606</anchor>
      <arglist>(const Pf1550Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a962849539c098ed41bf63e57b63f4faf</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a84ab6fa18a77cfe343184cac5224ab63</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ApplyProfile</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>ac40b3c30a9fe57719766385b08dfbfbb</anchor>
      <arglist>(std::span&lt; const pf1550::profiles::RegisterWrite &gt; profile) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ApplyPortentaH7Profile</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>aa108a6c6313e5f61b43541939ce6c1b5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ApplyPortentaH7CarrierProfile</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>aa149ec3df5c29560dd537cf457d526eb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>SetPowerMode</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a7540e1c2eb4822869fc256420f04e9ab</anchor>
      <arglist>(pf1550::PowerMode mode) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>SetUsbRails</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a1755298d07dd03ea83595bd7b283a2c5</anchor>
      <arglist>(bool vbus_en, bool otg_en) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ReadPmicStatus</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>aa1dc27d4ac7487171444335651860052</anchor>
      <arglist>(uint8_t &amp;status) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>RefreshDiagnosticSnapshot</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a8f631a9e024938d05d3b3948b5d31800</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>RefreshStatusSnapshot</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a6066a3dde8757c3d58787274b4c69fc5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ReadDiagnosticSnapshot</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>ab4e75f26a65123710aa09e5efa2100f3</anchor>
      <arglist>(pf1550::DiagnosticSnapshot &amp;out) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>RunPowerSelfTest</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a3deeaf8555d0e6ddfa43fb2b201f230e</anchor>
      <arglist>(pf1550::SelfTestResult &amp;out) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ClearLatchedFaults</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>adcf7cc2b378e93bbf4392157d90167c5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HasMcuAffectingFault</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a8f1a6f6e5f108fdce71fd4db4a5fea70</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>Pf1550Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>aa3f78cf43a2524c363ff1d5f0735d742</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const Pf1550Driver *</type>
      <name>GetDriver</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>aada4b4909bdc43d0ae5864a7d7fd4e98</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>ensureInitializedLocked</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a06c9ab2db30c4daf8f0d7cba72e74b2f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseI2c &amp;</type>
      <name>i2c_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a053b7bf85cb37ed383c6e4f094f04fcb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>standby_gpio_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>aca87e848fa7216245b14418984712a0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>usb_vbus_en_gpio_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a3409393118e43a9847456ac2c6b2a8be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>BaseGpio *</type>
      <name>usb_otg_en_gpio_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a8be5af063016a3c316b418b512b4046e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalPf1550Comm &gt;</type>
      <name>comm_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a30ddf062d7cd0f6cf7b816013220e44b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Pf1550Driver &gt;</type>
      <name>driver_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>ac56cad9375971218936eb48a8a3c4cf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a5244b107501bac9a6e3bf523747e1ce5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>pf1550::DiagnosticSnapshot</type>
      <name>cached_snapshot_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a88bfdc80a9ce2e52303b6d76d85d8df9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>have_full_snapshot_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>a78115fa05596564c829ceeb772ab7303</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classPf1550Handler.html</anchorfile>
      <anchor>abce7f367f016fdd0b83f385ae9c1f065</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Se050Handler</name>
    <filename>classSe050Handler.html</filename>
    <member kind="typedef">
      <type>se050::Device&lt; HalI2cSe050Comm &gt;</type>
      <name>DeviceType</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>a528bb48a4a299b603225ecfa456aaafe</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Se050Handler</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>a5deededd9b4446576d9835955ec5b800</anchor>
      <arglist>(BaseI2c &amp;i2c, const Se050HandlerConfig &amp;config=Se050HandlerConfig{}, BaseGpio *reset_gpio=nullptr, RtosMutex *bus_mutex=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Se050Handler</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>ad0ca869cab38533ee2f3d7a7998bf5a1</anchor>
      <arglist>(const Se050Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Se050Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>a997a5387492a78e17aa4210ef29f2c2c</anchor>
      <arglist>(const Se050Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Se050Handler</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>a45f25145691af177534d1f58ca83396f</anchor>
      <arglist>(Se050Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Se050Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>ae16d0ac0634f4552bf5e142e5b77716c</anchor>
      <arglist>(Se050Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>const Se050HandlerConfig &amp;</type>
      <name>Config</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>aad83014419b10ca06c3c069b6d69f82d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>a11b8c8b73d8c2e1f3a5cd7ef2af5f0c5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPresent</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>a2bc78e74c4c871bf9bc7885f96b7b473</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>DeviceType &amp;</type>
      <name>GetDevice</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>a7827d6df0d76cafdaefadcfb449509cf</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const DeviceType &amp;</type>
      <name>GetDevice</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>ab7f5872346868bea5b42ed1dd5f8cf9e</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>aaee404639b6be1dffc26a4840d2f12b6</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Se050HandlerConfig</type>
      <name>config_</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>ab5a24dfbb6fdb0147eeb31f84be87ba6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>HalI2cSe050Comm</type>
      <name>comm_</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>ae375f28a4835e67961417886f0f22930</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>DeviceType</type>
      <name>device_</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>a8c098e9a9a7921bb950d7bfce6881507</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; bool &gt;</type>
      <name>initialized_</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>ae0455657fc76d72a4b8591e9d2b6f77b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>private_mutex_</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>ab5ce089c45ce53b97d95e944d2df14f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex *</type>
      <name>bus_mutex_</name>
      <anchorfile>classSe050Handler.html</anchorfile>
      <anchor>a3147d01f1592d9cef5f14f8a6f3761ac</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Se050HandlerConfig</name>
    <filename>structSe050HandlerConfig.html</filename>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>apdu_timeout_ms</name>
      <anchorfile>structSe050HandlerConfig.html</anchorfile>
      <anchor>a191005e7be5a525719f0cdf4f54f9bf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>warm_reset_timeout_ms</name>
      <anchorfile>structSe050HandlerConfig.html</anchorfile>
      <anchor>afa1bf0166dbffab7e1d28f77b2c6169f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint32_t</type>
      <name>t1_inter_frame_delay_ms</name>
      <anchorfile>structSe050HandlerConfig.html</anchorfile>
      <anchor>a606129b8490fc78bfd77b25dcb0b313c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>skip_chip_warm_reset</name>
      <anchorfile>structSe050HandlerConfig.html</anchorfile>
      <anchor>a46fd8c97fb384f7e2c88e70e924c7ff7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SfmHandler</name>
    <filename>classSfmHandler.html</filename>
    <class kind="struct">SfmHandler::Counters</class>
    <member kind="typedef">
      <type>sfm::Driver&lt; HalI2cSfmComm &gt;</type>
      <name>DriverType</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a48ddc4242733ab15f5009f7dd35380bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SfmHandler</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a6967d0c2516fe48911203db4663d893c</anchor>
      <arglist>(BaseI2c &amp;i2c_device, const SfmHandlerConfig &amp;config=SfmHandlerConfig{}, RtosMutex *bus_mutex=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SfmHandler</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a4a928f532d913d5a30d8f1a92b90defb</anchor>
      <arglist>(const SfmHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>SfmHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ad0f3a86070f6a49153d646480bd7c301</anchor>
      <arglist>(const SfmHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SfmHandler</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a2047001e909bc277c0d51ac86c90edde</anchor>
      <arglist>(SfmHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>SfmHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a27c405dee112bc7f3f979f231afaf3fb</anchor>
      <arglist>(SfmHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ab62b35fd1401994083eb845d53c77533</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Invalidate</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a2f2299befbbf0917d55b780d6018ec43</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsPresent</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ae3b7fce3e2c16ce69bfd037073c0bd5e</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const sfm::ProductInfo &amp;</type>
      <name>Identity</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a33ac10844e576a970d93231910304967</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>SupportsGas</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ae65e2a363cd0f5c13c49ee423bc55463</anchor>
      <arglist>(sfm::Gas gas) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>FullScaleSlm</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ae57eda39bd8066932094c78044534720</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::DriverResult&lt; void &gt;</type>
      <name>Start</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a7f730b2e19bf47014ae2d0967a4a4159</anchor>
      <arglist>(sfm::Gas gas) noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::DriverResult&lt; void &gt;</type>
      <name>Start</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ab7e2e62f557bb6a6d34e5a4de1394b82</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::DriverResult&lt; void &gt;</type>
      <name>Stop</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ad810a485b2f266f90169f42626b397f0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::DriverResult&lt; sfm::Measurement &gt;</type>
      <name>ReadMeasurement</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a41bd4e16c381ee68e48ff50c4224e08e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::DriverResult&lt; void &gt;</type>
      <name>ConfigureAveraging</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a3b57b8b46a74678a495773736d4ee099</anchor>
      <arglist>(std::uint16_t window) noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::DriverResult&lt; void &gt;</type>
      <name>UpdateConcentration</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ab2f018d52a421507b60724d0562932a9</anchor>
      <arglist>(std::uint16_t o2_permille) noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::DriverResult&lt; float &gt;</type>
      <name>ReadTemperature</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a120c0fbffbec124f26af0e4ccb23cf68</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::DriverResult&lt; sfm::Measurement &gt;</type>
      <name>DecodeFrame</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a618e988d1f6e8605835c12bf98a532c9</anchor>
      <arglist>(const std::uint8_t frame[9]) noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::Family</type>
      <name>Family</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a06d364c79110158148c92c35d8897b3d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::uint8_t</type>
      <name>Address</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a7bd2d5a49de73a0bb96aafb510dc3168</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::Gas</type>
      <name>ActiveGas</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a6b8bef454b0f8399f50126ba05829d24</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>sfm::Scaling</type>
      <name>ActiveScaling</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>adb2b40b46dcc1395f5b646877f89793c</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Measuring</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a621576ea05291d13d6601d4392919d37</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>Counters</type>
      <name>GetCounters</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>aa0ac21a5d1d17da6d8ba483b63089d68</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a96af66640366c681fc21e21b41d53a80</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SfmHandlerConfig</type>
      <name>config_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a11f71a65a31cd6c47a52f9eb0b138348</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>HalI2cSfmComm</type>
      <name>comm_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ae8a216602232eada71ed3d546e1cd70b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; DriverType &gt;</type>
      <name>driver_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>abe98dab4df1bbc38f1deba23f018fd07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>sfm::ProductInfo</type>
      <name>identity_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a41c9a1d139fef5e4cdf87db92c8f1e87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; bool &gt;</type>
      <name>initialized_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a7f4805372d2d89b93d4bdb534b440936</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; std::uint32_t &gt;</type>
      <name>frames_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a8f7b8d16c3208c41a4b9a7bd1d78e298</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; std::uint32_t &gt;</type>
      <name>no_data_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a4ede0bc14cd9328c7dcf96a03a603983</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; std::uint32_t &gt;</type>
      <name>crc_errors_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a79eb909642c78b47241eee2d2bdc4d52</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; std::uint32_t &gt;</type>
      <name>bus_errors_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a3b1a1d69bdd905fcf0fffed713a17671</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>private_mutex_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>ae9ad7b52a212626227e8c6b7f0ecabfd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex *</type>
      <name>bus_mutex_</name>
      <anchorfile>classSfmHandler.html</anchorfile>
      <anchor>a855f0f12da4a5c178e9dd924e17bc1a0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SfmHandlerConfig</name>
    <filename>structSfmHandlerConfig.html</filename>
    <member kind="variable">
      <type>sfm::Gas</type>
      <name>default_gas</name>
      <anchorfile>structSfmHandlerConfig.html</anchorfile>
      <anchor>a9e5d957bca8bd7b63c83ece3a776189e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint16_t</type>
      <name>o2_permille</name>
      <anchorfile>structSfmHandlerConfig.html</anchorfile>
      <anchor>a6654103598d3f98f864fbb181024e9eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint16_t</type>
      <name>averaging_window</name>
      <anchorfile>structSfmHandlerConfig.html</anchorfile>
      <anchor>a19c41b9554eaba3a4180f123312799cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>soft_reset_on_init</name>
      <anchorfile>structSfmHandlerConfig.html</anchorfile>
      <anchor>a64a3684c5275aabe53594ad3126ff209</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::uint8_t</type>
      <name>device_index</name>
      <anchorfile>structSfmHandlerConfig.html</anchorfile>
      <anchor>afddaeb805ab4939a33439632ed067e50</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Logger::TagLevel</name>
    <filename>structLogger_1_1TagLevel.html</filename>
    <member kind="variable">
      <type>char</type>
      <name>tag</name>
      <anchorfile>structLogger_1_1TagLevel.html</anchorfile>
      <anchor>a56f03d5b2d3cfd87033437d44653d0e5</anchor>
      <arglist>[kMaxTagLength]</arglist>
    </member>
    <member kind="variable">
      <type>LogLevel</type>
      <name>level</name>
      <anchorfile>structLogger_1_1TagLevel.html</anchorfile>
      <anchor>af6d7e19cd63be826348cc7b1e5e5c1aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>in_use</name>
      <anchorfile>structLogger_1_1TagLevel.html</anchorfile>
      <anchor>ae0706da5d17901967f36e666f960de42</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tmc9660Handler::Temperature</name>
    <filename>classTmc9660Handler_1_1Temperature.html</filename>
    <member kind="function">
      <type></type>
      <name>Temperature</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a39c2778d7ba493b3c5a620b26289218d</anchor>
      <arglist>(Tmc9660Handler &amp;parent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Temperature</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a47ac469deac03d39024c8332a274d830</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a8019acd874c13ba0e175224a367bf370</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a0666ccf1178ec18b29ff9b04ed982a12</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>ReadTemperatureCelsiusImpl</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a5ebc77b2122ba4473b74a4a0fc78ff09</anchor>
      <arglist>(float *temperature_celsius) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>GetSensorInfo</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a70513525aa4ce3695b66efe61790b9d9</anchor>
      <arglist>(hf_temp_sensor_info_t *info) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u32_t</type>
      <name>GetCapabilities</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a5aaa7a97dba72b8d7ffe3db213ab69b6</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function" protection="private">
      <type>hf_temp_err_t</type>
      <name>UpdateStatistics</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a909be22915a36d0ef677558f3c00c8c4</anchor>
      <arglist>(hf_temp_err_t result, uint64_t start_time_us) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>uint64_t</type>
      <name>GetCurrentTimeUs</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a41a767273451c49bc03ab16243cfd7e0</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>UpdateDiagnostics</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a824232f839a3c46684de17dabaca8b79</anchor>
      <arglist>(hf_temp_err_t error) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Tmc9660Handler &amp;</type>
      <name>parent_</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a6c18ef51fa429deb59617149c377133b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>mutex_</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a7db458c4691ee7be8671d97191531a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_temp_statistics_t</type>
      <name>statistics_</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>ac2902d7ec338a30c9cd641b5b0493884</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>hf_temp_diagnostics_t</type>
      <name>diagnostics_</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a6987bb19fe490be8c8b7b81fecf54f89</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::atomic&lt; hf_temp_err_t &gt;</type>
      <name>last_error_</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a5f25018bc7dfed904873d722a7b2ca90</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a8019acd874c13ba0e175224a367bf370</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a0666ccf1178ec18b29ff9b04ed982a12</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>ReadTemperatureCelsiusImpl</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a5ebc77b2122ba4473b74a4a0fc78ff09</anchor>
      <arglist>(float *temperature_celsius) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_temp_err_t</type>
      <name>GetSensorInfo</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a70513525aa4ce3695b66efe61790b9d9</anchor>
      <arglist>(hf_temp_sensor_info_t *info) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u32_t</type>
      <name>GetCapabilities</name>
      <anchorfile>classTmc9660Handler_1_1Temperature.html</anchorfile>
      <anchor>a5aaa7a97dba72b8d7ffe3db213ab69b6</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>TestResults</name>
    <filename>structTestResults.html</filename>
    <member kind="function">
      <type>void</type>
      <name>add_result</name>
      <anchorfile>structTestResults.html</anchorfile>
      <anchor>a257eba3a9005a5c19c68ae27774948f7</anchor>
      <arglist>(bool passed, uint64_t execution_time) noexcept</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>get_success_percentage</name>
      <anchorfile>structTestResults.html</anchorfile>
      <anchor>ab98f9ad5f994aa31bd7c0cd8ae551fb8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>get_total_time_ms</name>
      <anchorfile>structTestResults.html</anchorfile>
      <anchor>a6d2e718d81a8b4a4dcca515ee858de57</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>total_tests</name>
      <anchorfile>structTestResults.html</anchorfile>
      <anchor>afe20b168a13950ffca7296c0c56d4359</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>passed_tests</name>
      <anchorfile>structTestResults.html</anchorfile>
      <anchor>a55b5f813fe7b5863b46e03f2c7f761ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>failed_tests</name>
      <anchorfile>structTestResults.html</anchorfile>
      <anchor>a0acef46f38f27f0fbb8fa822bb218b3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint64_t</type>
      <name>total_execution_time_us</name>
      <anchorfile>structTestResults.html</anchorfile>
      <anchor>a7eddc258a4288ac5f0a36a1008c9d014</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>TestTaskContext</name>
    <filename>structTestTaskContext.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>test_name</name>
      <anchorfile>structTestTaskContext.html</anchorfile>
      <anchor>abcf19d2762ab98990c3fdc1eacf86130</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool(*</type>
      <name>test_func</name>
      <anchorfile>structTestTaskContext.html</anchorfile>
      <anchor>aa72faa223fd0ff992772b12458f62ec8</anchor>
      <arglist>)() noexcept</arglist>
    </member>
    <member kind="variable">
      <type>TestResults *</type>
      <name>results</name>
      <anchorfile>structTestTaskContext.html</anchorfile>
      <anchor>a1c97680bd267a35637278ea10a3fae0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>tag</name>
      <anchorfile>structTestTaskContext.html</anchorfile>
      <anchor>ae51f49c33ea802bc7f8bdde560a27304</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SemaphoreHandle_t</type>
      <name>completion_semaphore</name>
      <anchorfile>structTestTaskContext.html</anchorfile>
      <anchor>ab82bee11b198be2f2b4d6c908573ddb6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TestThread</name>
    <filename>classTestThread.html</filename>
    <member kind="function">
      <type></type>
      <name>TestThread</name>
      <anchorfile>classTestThread.html</anchorfile>
      <anchor>a6c0eff17bfe9e6ada106bc920b69fe1e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>GetRunCount</name>
      <anchorfile>classTestThread.html</anchorfile>
      <anchor>a1f720b6a4ff2ea619abfe99eaf589058</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTestThread.html</anchorfile>
      <anchor>a9890573d4692389987a3211a7df70145</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>Setup</name>
      <anchorfile>classTestThread.html</anchorfile>
      <anchor>ae3c9089f3b5f8c1338d20f5de976e7f0</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>uint32_t</type>
      <name>Step</name>
      <anchorfile>classTestThread.html</anchorfile>
      <anchor>a72b16472c179086a4185b8fd1da184dd</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>Cleanup</name>
      <anchorfile>classTestThread.html</anchorfile>
      <anchor>a5febe0f3e5a53da420cb7b72ec6aed1f</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>ResetVariables</name>
      <anchorfile>classTestThread.html</anchorfile>
      <anchor>abaf68689b014106872445278fdfcd1fa</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>run_count_</name>
      <anchorfile>classTestThread.html</anchorfile>
      <anchor>a4ecde2fcf972d844206e09b7ae9a48fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>stack_</name>
      <anchorfile>classTestThread.html</anchorfile>
      <anchor>ae65a7026498c48897b80d38a4c8fe4ca</anchor>
      <arglist>[4096]</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tle92466edHandler</name>
    <filename>classTle92466edHandler.html</filename>
    <class kind="struct">Tle92466edHandler::is_driver_result</class>
    <class kind="struct">Tle92466edHandler::is_driver_result&lt; tle92466ed::DriverResult&lt; T &gt; &gt;</class>
    <member kind="typedef">
      <type>tle92466ed::Driver&lt; HalSpiTle92466edComm &gt;</type>
      <name>DriverType</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a3041ad56808ba94846d7f5294222438c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tle92466edHandler</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>aa06da4aafab991668a54478a602c029c</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;resn, BaseGpio &amp;en, BaseGpio *faultn=nullptr) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Tle92466edHandler</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a4ee41e5564077c020e0786f19217fd20</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tle92466edHandler</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a86586728cf443f549417bf7b53256765</anchor>
      <arglist>(const Tle92466edHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tle92466edHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a0058457c10dd450ed9667388fd62c632</anchor>
      <arglist>(const Tle92466edHandler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tle92466edHandler</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a9d4347b488c132af9cb84aeecddf7320</anchor>
      <arglist>(Tle92466edHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tle92466edHandler &amp;</type>
      <name>operator=</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>af3c793d7797adb455ab5d7aefb9d5a55</anchor>
      <arglist>(Tle92466edHandler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>Initialize</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>acc015030fa6b0761fb525d9ad17d293a</anchor>
      <arglist>(bool perform_hardware_reset=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a1a976346516812f7125cdeb2f3aeb327</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>Initialize</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a2f01dd08b168f009db41d658db3484eb</anchor>
      <arglist>(const tle92466ed::GlobalConfig &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>Deinitialize</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a8ec12ada88277464a937b97a606715eb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>aecf5a6f8ff25914f027f11a1c20f97ee</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>ConfigureChannel</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>ac14e1f8cf8996a259428969e237f8675</anchor>
      <arglist>(uint8_t channel, const tle92466ed::ChannelConfig &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>EnableChannel</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a2e7ce0a3fa43f9abdba28e12df2740e6</anchor>
      <arglist>(uint8_t channel) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>DisableChannel</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a33b7dc81c74a87723d9e4f0ad829e41a</anchor>
      <arglist>(uint8_t channel) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>EnableAllChannels</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a1ee30771436827a1499a379aa7e5ab49</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>DisableAllChannels</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>ac37cbbcce53c7a55657ee61cd5015190</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>SetChannelCurrent</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a534dece279e9cbb9a7648e2f95eedd22</anchor>
      <arglist>(uint8_t channel, uint16_t current_ma) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>SetChannelCurrentUa</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>afa3ff77991f3f5e13cd6a03e96d31397</anchor>
      <arglist>(uint8_t channel, uint32_t current_ua) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; uint16_t &gt;</type>
      <name>GetChannelCurrentSetpoint</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a473439bc50403c32720ac7b94f38cb51</anchor>
      <arglist>(uint8_t channel, bool parallel_mode=false) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>ConfigurePwmRaw</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>aa0ad7669bfe2830245b05e46846e6eca</anchor>
      <arglist>(uint8_t channel, uint8_t mantissa, uint8_t exponent, bool low_freq_range=false) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>EnterMissionMode</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>aee10ff4c219a042d260a6b1f58a9219a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>EnterConfigMode</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a19b966bb109b988eb98d319b76908667</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsMissionMode</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a6bcbd117208f3e44f2216d45541eb4fa</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>EnableOutputStage</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>aec30fe1266505d13b73f02c7f92e1618</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>DisableOutputStage</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a7785b8d56d0bd25001e5bdf9b8d557a2</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>EnableFeedbackUpdates</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>ad9d8117d83234cdd7947aeeb40281b36</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>GetStatus</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a0851407035d56168309937254b642491</anchor>
      <arglist>(tle92466ed::DeviceStatus &amp;status) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>GetChannelDiagnostics</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a460b8f8c43510396ad34f8928e3f72f9</anchor>
      <arglist>(uint8_t channel, tle92466ed::ChannelDiagnostics &amp;diag) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>GetFaultReport</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>aff0e4e65ed1f439801204385e83b3472</anchor>
      <arglist>(tle92466ed::FaultReport &amp;report) noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>ClearFaults</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>ab3123678e6df690f5669fd89079e0a4e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>HasFault</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>ab86e6d3eb2dca25154a8fa7c42ddb945</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>KickWatchdog</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a1cc881741f12bec3e5e0042edfb47caf</anchor>
      <arglist>(uint16_t reload_value=1000) noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>GetChipId</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a4c646ee5db271f07517cc15e8a623f0e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>GetIcVersion</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a0c29d4ada1c9247dc90fb64d914dec1f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>LastSpiRxWord</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a051f5db092d3cdc556b32711bcd9e7d8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>LastSpiRxWordFirst</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a570f1956481270726785fc0ce4d05f5e</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>RawSpiChain</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a47d8c9624c814c08152104917559ea1a</anchor>
      <arglist>(std::span&lt; const uint32_t &gt; tx, std::span&lt; uint32_t &gt; rx, uint32_t gap_us=0U) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>WithLock</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>aca1baa18cad87089f57915ad1a024098</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>WithDriver</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>ae4c64bd153132a4787e81ce8f846d618</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept</arglist>
    </member>
    <member kind="function">
      <type>DriverType *</type>
      <name>GetDriver</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>ae83bbf85e50d68c8adcc2160bdaed271</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const DriverType *</type>
      <name>GetDriver</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a56ebe6e509ccd1b6b865728ba14d2533</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>afcec01ba90f6596dcde99387ef41b6e4</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static constexpr uint8_t</type>
      <name>kNumChannels</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>aeb30a570646f9b32aaa499611691487e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>tle92466ed::DriverResult&lt; void &gt;</type>
      <name>InitializeLocked</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a9edc67611c6c32eb369488f04912ea53</anchor>
      <arglist>(bool perform_hardware_reset) noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a59ce7fd71a61c78bba4dc4a43c99e280</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>auto</type>
      <name>withDriver</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>aef42f3c7c1b1ef51b27eb8d178aca6bc</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a79e6d31e7f429f789a2552b3ce8e5a63</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>mutex_</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>af0921ac446c5c577bcd10f56bfe744e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalSpiTle92466edComm &gt;</type>
      <name>comm_</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a804b537caa4f13ae19ae4a2e57982f7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; DriverType &gt;</type>
      <name>driver_</name>
      <anchorfile>classTle92466edHandler.html</anchorfile>
      <anchor>a098cb780362ae98a49c13a23fcd80418</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tmc5160CtrlPins</name>
    <filename>structTmc5160CtrlPins.html</filename>
    <member kind="function">
      <type>BaseGpio *</type>
      <name>get</name>
      <anchorfile>structTmc5160CtrlPins.html</anchorfile>
      <anchor>a5c9f3e281f5d485c015a78ac32ce8145</anchor>
      <arglist>(tmc51x0::TMC51x0CtrlPin pin) noexcept</arglist>
    </member>
    <member kind="variable">
      <type>BaseGpio &amp;</type>
      <name>enable</name>
      <anchorfile>structTmc5160CtrlPins.html</anchorfile>
      <anchor>a0780705d1b4a6eef17a26bb9d7bceecc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BaseGpio *</type>
      <name>diag0</name>
      <anchorfile>structTmc5160CtrlPins.html</anchorfile>
      <anchor>a2bd0a07a547abfef8152abab9dd7d6a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BaseGpio *</type>
      <name>diag1</name>
      <anchorfile>structTmc5160CtrlPins.html</anchorfile>
      <anchor>a4b7a60c1c231cc8000747e658ff387e5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tmc5160Handler</name>
    <filename>classTmc5160Handler.html</filename>
    <member kind="typedef">
      <type>tmc51x0::TMC51x0&lt; HalSpiTmc5160Comm &gt;</type>
      <name>SpiDriver</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a93e7a35d8a8b4dc3ec97bb1a9d26c3a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tmc51x0::TMC51x0&lt; HalUartTmc5160Comm &gt;</type>
      <name>UartDriver</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a31e66827548c966a076877961d3e852f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::variant&lt; std::monostate, SpiDriver *, UartDriver * &gt;</type>
      <name>DriverVariant</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a4b00981028b5ff6cb89eeed105a8eed1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::variant&lt; std::monostate, const SpiDriver *, const UartDriver * &gt;</type>
      <name>ConstDriverVariant</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>ae9ac81d930499aae3a3bb7a6dc632cda</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc5160Handler</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a3734595897a34600986242dbbefd9ed3</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;enable, BaseGpio *diag0=nullptr, BaseGpio *diag1=nullptr, uint8_t daisy_chain_position=0, const tmc51x0::PinActiveLevels &amp;active_levels={}) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc5160Handler</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a2601e350781600541622f65460a24691</anchor>
      <arglist>(BaseUart &amp;uart, BaseGpio &amp;enable, BaseGpio *diag0=nullptr, BaseGpio *diag1=nullptr, uint8_t uart_node_address=0, const tmc51x0::PinActiveLevels &amp;active_levels={}) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Tmc5160Handler</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>aed4f4e077bf616f3a0b7af6f3fafcf74</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc5160Handler</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a134451145d7803bfcfcde6891f3154f2</anchor>
      <arglist>(const Tmc5160Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tmc5160Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a9622a527174fdc5f37e55044652f75b8</anchor>
      <arglist>(const Tmc5160Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc5160Handler</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>aa4ba39d1f580c80119eb34943dd6e7ed</anchor>
      <arglist>(Tmc5160Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tmc5160Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a6dba946e59e91c2d5b16eccc25992b6e</anchor>
      <arglist>(Tmc5160Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>tmc51x0::ErrorCode</type>
      <name>Initialize</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a44ab36857e4e43062f81f7d6f10eab09</anchor>
      <arglist>(const tmc51x0::DriverConfig &amp;config, bool verbose=true) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a5ad276d4fcd81765d0a5af064fdbf7b5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a262b31fb5ccbfe229766a9a4b3ce546b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>ab5b929298611ac61b7b705eb5bd64a61</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsSpi</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a7167cbb3ba74139d12e37e72d778d6b1</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>adf1b6ae5b680756f1b68169789cf0c1f</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept -&gt; decltype(fn(std::declval&lt; SpiDriver &amp; &gt;()))</arglist>
    </member>
    <member kind="function">
      <type>SpiDriver *</type>
      <name>driverViaSpi</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a15dae0f711e2dd4ecea09822b8a3a8bb</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const SpiDriver *</type>
      <name>driverViaSpi</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>aa47c7b0c69a35aa3f9e9c6fc8d2bd95c</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>UartDriver *</type>
      <name>driverViaUart</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>af8cb1c7b5011bd24102d296f22c211c5</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const UartDriver *</type>
      <name>driverViaUart</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a845e1879a4606367f419f3892061ff31</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>DriverVariant</type>
      <name>GetDriver</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a8753f4b21ffb25d294d4ce41951f11f7</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>ConstDriverVariant</type>
      <name>GetDriver</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a755fbb0bd0c5faa03c807522e4ea469a</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>ab4bf5aabef968f6f8ee66d08cf05913a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a28d5365ad7e7ef2af60d599388e9df37</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const tmc51x0::DriverConfig &amp;</type>
      <name>GetDriverConfig</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a4fac52db1a50fc8f24350ac804c5fd4b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static tmc51x0::DriverConfig</type>
      <name>GetDefaultConfig</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>aee9861399d8eee4660d17ba4122bec8a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a6d4fce2433e03bb11b05a4278ef66fd0</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>auto</type>
      <name>visitDriverInternal</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a439a68f9d3f6785e0a493f9ed3821325</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept -&gt; decltype(fn(std::declval&lt; SpiDriver &amp; &gt;()))</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>is_spi_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a8affb73f8819802955910b46d886048f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a9611596b31235838079bb847dbeb0ac6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>mutex_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>ae5cc2c89041026e9627b3f2defe2b75c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalSpiTmc5160Comm &gt;</type>
      <name>spi_comm_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>aa8d3ce7658218be523c82e0abcf83161</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalUartTmc5160Comm &gt;</type>
      <name>uart_comm_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>aeb7ebf11e1b2aaf3cdb6531b23dad290</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; SpiDriver &gt;</type>
      <name>spi_driver_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>ab348f2967109fd94a2d80bc6ac907e17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; UartDriver &gt;</type>
      <name>uart_driver_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>af16b467bb1410def2c8a835f13a8bbe0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>address_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>a43ccc6febdbf53eb85fcec87cc9a8c99</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>tmc51x0::DriverConfig</type>
      <name>config_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>ad7f9b019221efb2adcdc0d21d562a8dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classTmc5160Handler.html</anchorfile>
      <anchor>ac72f7509b90ed545349b9556e28da075</anchor>
      <arglist>[64]</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tmc9660AdcWrapper</name>
    <filename>classTmc9660AdcWrapper.html</filename>
    <member kind="function">
      <type></type>
      <name>Tmc9660AdcWrapper</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>aaa1916aded3e1f9af9ed4c9a0d35af85</anchor>
      <arglist>(Tmc9660Handler &amp;handler) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Tmc9660AdcWrapper</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a6e6b964a668298a31492d8020770bce8</anchor>
      <arglist>() noexcept override=default</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660AdcWrapper</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>aa882341eb3bbf4bc62f53dea344f3297</anchor>
      <arglist>(const Tmc9660AdcWrapper &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tmc9660AdcWrapper &amp;</type>
      <name>operator=</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a3544cbda26cdd1ef273a1592c803905d</anchor>
      <arglist>(const Tmc9660AdcWrapper &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660AdcWrapper</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>adb9d695d17dcd4909fd19024df2fefd9</anchor>
      <arglist>(Tmc9660AdcWrapper &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tmc9660AdcWrapper &amp;</type>
      <name>operator=</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a153c79d5cc9770dafd565aa465b5306c</anchor>
      <arglist>(Tmc9660AdcWrapper &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>aebe0e2cd94c5db2d70ef05c5cffd1913</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a9d61893186aaa1d768cf61038ab285cb</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxChannels</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a7f754c5432b29313fb09f3cc353a4e6f</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelAvailable</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>aee2152f63daf77c1e3cacf5c669a71e2</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelV</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a17adac7645b48565ded0ad022bf9cb1d</anchor>
      <arglist>(hf_channel_id_t channel_id, float &amp;channel_reading_v, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelCount</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a64101f711603386a40be623166b90bc9</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t &amp;channel_reading_count, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannel</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a42eebe0e3090b205a166cf3186bd121e</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t &amp;channel_reading_count, float &amp;channel_reading_v, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadMultipleChannels</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a3703e1432eb017a9936e543854973755</anchor>
      <arglist>(const hf_channel_id_t *channel_ids, hf_u8_t num_channels, hf_u32_t *readings, float *voltages) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>GetStatistics</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a81242df0d4a815840ddd924040a1b499</anchor>
      <arglist>(hf_adc_statistics_t &amp;statistics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>GetDiagnostics</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a40c0a3bfa343ae9664bd71c683b18185</anchor>
      <arglist>(hf_adc_diagnostics_t &amp;diagnostics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ResetStatistics</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>abccc374da45807cc0d2cf7ddedfd88bc</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ResetDiagnostics</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a3a7e9bf27e3cf27497ecbed63209fbdf</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Tmc9660Handler &amp;</type>
      <name>handler_</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a2283d4d09f117a689861411887c9a0be</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>aebe0e2cd94c5db2d70ef05c5cffd1913</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a9d61893186aaa1d768cf61038ab285cb</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_u8_t</type>
      <name>GetMaxChannels</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a7f754c5432b29313fb09f3cc353a4e6f</anchor>
      <arglist>() const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsChannelAvailable</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>aee2152f63daf77c1e3cacf5c669a71e2</anchor>
      <arglist>(hf_channel_id_t channel_id) const noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelV</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a17adac7645b48565ded0ad022bf9cb1d</anchor>
      <arglist>(hf_channel_id_t channel_id, float &amp;channel_reading_v, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannelCount</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a64101f711603386a40be623166b90bc9</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t &amp;channel_reading_count, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadChannel</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a42eebe0e3090b205a166cf3186bd121e</anchor>
      <arglist>(hf_channel_id_t channel_id, hf_u32_t &amp;channel_reading_count, float &amp;channel_reading_v, hf_u8_t numOfSamplesToAvg=1, hf_time_t timeBetweenSamples=0) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ReadMultipleChannels</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a3703e1432eb017a9936e543854973755</anchor>
      <arglist>(const hf_channel_id_t *channel_ids, hf_u8_t num_channels, hf_u32_t *readings, float *voltages) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>GetStatistics</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a81242df0d4a815840ddd924040a1b499</anchor>
      <arglist>(hf_adc_statistics_t &amp;statistics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>GetDiagnostics</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a40c0a3bfa343ae9664bd71c683b18185</anchor>
      <arglist>(hf_adc_diagnostics_t &amp;diagnostics) noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ResetStatistics</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>abccc374da45807cc0d2cf7ddedfd88bc</anchor>
      <arglist>() noexcept override</arglist>
    </member>
    <member kind="function">
      <type>hf_adc_err_t</type>
      <name>ResetDiagnostics</name>
      <anchorfile>classTmc9660AdcWrapper.html</anchorfile>
      <anchor>a3a7e9bf27e3cf27497ecbed63209fbdf</anchor>
      <arglist>() noexcept override</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>Tmc9660CtrlPins</name>
    <filename>structTmc9660CtrlPins.html</filename>
    <member kind="function">
      <type>BaseGpio &amp;</type>
      <name>get</name>
      <anchorfile>structTmc9660CtrlPins.html</anchorfile>
      <anchor>a89d2aea877e8da13a1c26364801630fd</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin) noexcept</arglist>
    </member>
    <member kind="function">
      <type>const BaseGpio &amp;</type>
      <name>get</name>
      <anchorfile>structTmc9660CtrlPins.html</anchorfile>
      <anchor>a25b9b59843a26d8008a7d6c33e13ee5b</anchor>
      <arglist>(tmc9660::TMC9660CtrlPin pin) const noexcept</arglist>
    </member>
    <member kind="variable">
      <type>BaseGpio &amp;</type>
      <name>rst</name>
      <anchorfile>structTmc9660CtrlPins.html</anchorfile>
      <anchor>ace66f983fb92571fe12361a803cad705</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BaseGpio &amp;</type>
      <name>drv_en</name>
      <anchorfile>structTmc9660CtrlPins.html</anchorfile>
      <anchor>ae809d42f3ca4fd1084d1533add34fbe9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BaseGpio &amp;</type>
      <name>faultn</name>
      <anchorfile>structTmc9660CtrlPins.html</anchorfile>
      <anchor>a39589c90d71bbfa3714e204a8a5a8a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BaseGpio &amp;</type>
      <name>wake</name>
      <anchorfile>structTmc9660CtrlPins.html</anchorfile>
      <anchor>a0f1e3e0750a8d6ac1b9d840fd5883d4a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Tmc9660Handler</name>
    <filename>classTmc9660Handler.html</filename>
    <class kind="class">Tmc9660Handler::Adc</class>
    <class kind="class">Tmc9660Handler::Gpio</class>
    <class kind="class">Tmc9660Handler::Temperature</class>
    <member kind="typedef">
      <type>tmc9660::TMC9660&lt; HalSpiTmc9660Comm &gt;</type>
      <name>SpiDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a65d00ffe3eba7aef1cb05ccef5feb619</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tmc9660::TMC9660&lt; HalUartTmc9660Comm &gt;</type>
      <name>UartDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a68a5549c74095516db249e2c2d4afed7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::variant&lt; std::monostate, SpiDriver *, UartDriver * &gt;</type>
      <name>DriverVariant</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a04c51be889ee4365d2eee7ee367180dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::variant&lt; std::monostate, const SpiDriver *, const UartDriver * &gt;</type>
      <name>ConstDriverVariant</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a110c823022b879cd6d2c47cdc4468321</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a33c5f6adfbd116fd717ac96c3e00fb0b</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;rst, BaseGpio &amp;drv_en, BaseGpio &amp;faultn, BaseGpio &amp;wake, uint8_t address=0, const tmc9660::BootloaderConfig *bootCfg=&amp;kDefaultBootConfig)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>aed1c579c5456e85070601f22b3f39a01</anchor>
      <arglist>(BaseUart &amp;uart, BaseGpio &amp;rst, BaseGpio &amp;drv_en, BaseGpio &amp;faultn, BaseGpio &amp;wake, uint8_t address=0, const tmc9660::BootloaderConfig *bootCfg=&amp;kDefaultBootConfig)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>acb01f56d0292ce5e7fcad4df46421fdc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>abcae1e86438604b16a5e92312e7eb8e5</anchor>
      <arglist>(const Tmc9660Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tmc9660Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a15630492c4e23ca9b54e502b5c75ba08</anchor>
      <arglist>(const Tmc9660Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ab69c4d42a99fb746ea79a0c861efd314</anchor>
      <arglist>(Tmc9660Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tmc9660Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>af2e2baaba84789c9a5c89a07d965b415</anchor>
      <arglist>(Tmc9660Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>adc564847f8ae26c47412aaee0758991c</anchor>
      <arglist>(bool performReset=true, bool retrieveBootloaderInfo=true, bool failOnVerifyError=false)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a5a279ae9d4b5a8e0d71cb24577ad88ea</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsDriverReady</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a13db97d201a287d8aa60384e04070b9f</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetTmclReplyTimeoutMs</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a419ea352256ec90536a3107047a6b225</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>Gpio &amp;</type>
      <name>gpio</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>acf7d1583239c9f767f82b42fc03b56cf</anchor>
      <arglist>(uint8_t gpioNumber)</arglist>
    </member>
    <member kind="function">
      <type>Adc &amp;</type>
      <name>adc</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a7fe382c4dbf6bf84ed31586e52ec2e6e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Temperature &amp;</type>
      <name>temperature</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a393382194d0602e7aa02da6a8fd7a20a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tmc9660::CommMode</type>
      <name>GetCommMode</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ac7592c35471c995115e68cc94056b4fb</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const tmc9660::BootloaderConfig &amp;</type>
      <name>bootConfig</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a2381670960d25634731a697f895c19ab</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>adee8a764564554f060b879752048bf74</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ab0196eb3a096c7250014a4f1b65cb25a</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>SpiDriver *</type>
      <name>driverViaSpi</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a85be9cd2e065b83c7abb36cb70fa6c67</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const SpiDriver *</type>
      <name>driverViaSpi</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ad8946c2dd3a3d5e26620c514988fa2df</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>UartDriver *</type>
      <name>driverViaUart</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ac4f303f9e7be9cb737a667dd5d85dd3a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const UartDriver *</type>
      <name>driverViaUart</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a4eb74918e10999d77de25728b19542cf</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>DriverVariant</type>
      <name>GetDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ac29a3cfd55b2186f357c8b2f56d16bce</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>ConstDriverVariant</type>
      <name>GetDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ae1d6f6676180aff28d02dcb954917b79</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a4e219285c12b2ca50075edfaf73b4e2f</anchor>
      <arglist>(Func &amp;&amp;func) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a9a641de6a945a44e7f32cb1491eb5991</anchor>
      <arglist>(Func &amp;&amp;func) const noexcept</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const tmc9660::BootloaderConfig</type>
      <name>kDefaultBootConfig</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a4de763121aff441af52e492f3cff6f1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a7c1d8073576d678b09dac24c9eb12845</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>auto</type>
      <name>visitDriverInternal</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a07fd9e33de7e1f90137626b730bb5d27</anchor>
      <arglist>(Func &amp;&amp;func) noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>use_spi_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a01828de5ad5685ee4baac83943be3440</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>acdf20446a664a0c84b2b2b7cb21f3d36</anchor>
      <arglist>[64]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalSpiTmc9660Comm &gt;</type>
      <name>spi_comm_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>abec7689d1093da18106e90c32f427188</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalUartTmc9660Comm &gt;</type>
      <name>uart_comm_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a45d218c71e268fd627dcdd1f20e37313</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; SpiDriver &gt;</type>
      <name>spi_driver_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a29bbecb3a580ddad61ce19a18f23c77d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; UartDriver &gt;</type>
      <name>uart_driver_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a0bedb4612372da6ec2f5f01cd1b11f8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; std::unique_ptr&lt; Gpio &gt;, 2 &gt;</type>
      <name>gpioWrappers_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a8e454461702fd671f09f2a271908798c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Adc &gt;</type>
      <name>adcWrapper_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>aaf719a53b5dd88722f3f62c9ac16ffe3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Temperature &gt;</type>
      <name>temperatureWrapper_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a68cefac6f96e3f434036901633872ce6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const tmc9660::BootloaderConfig *</type>
      <name>bootCfg_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a1b5307e03e07d57585e7bcdcb0e7ef2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>device_address_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a1aea362e219afe55d37c69b637d48dc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>boot_ok_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a7f9a3d129d2f815114ef7d07e544d3ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a9fe707c03f7c0ea26d83cbd0771780af</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tmc9660::TMC9660&lt; HalSpiTmc9660Comm &gt;</type>
      <name>SpiDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a65d00ffe3eba7aef1cb05ccef5feb619</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>tmc9660::TMC9660&lt; HalUartTmc9660Comm &gt;</type>
      <name>UartDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a68a5549c74095516db249e2c2d4afed7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::variant&lt; std::monostate, SpiDriver *, UartDriver * &gt;</type>
      <name>DriverVariant</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a04c51be889ee4365d2eee7ee367180dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::variant&lt; std::monostate, const SpiDriver *, const UartDriver * &gt;</type>
      <name>ConstDriverVariant</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a110c823022b879cd6d2c47cdc4468321</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const tmc9660::BootloaderConfig</type>
      <name>kDefaultBootConfig</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a4de763121aff441af52e492f3cff6f1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalSpiTmc9660Comm &gt;</type>
      <name>spi_comm_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>abec7689d1093da18106e90c32f427188</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; HalUartTmc9660Comm &gt;</type>
      <name>uart_comm_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a45d218c71e268fd627dcdd1f20e37313</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; SpiDriver &gt;</type>
      <name>spi_driver_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a29bbecb3a580ddad61ce19a18f23c77d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; UartDriver &gt;</type>
      <name>uart_driver_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a0bedb4612372da6ec2f5f01cd1b11f8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::array&lt; std::unique_ptr&lt; Gpio &gt;, 2 &gt;</type>
      <name>gpioWrappers_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a8e454461702fd671f09f2a271908798c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Adc &gt;</type>
      <name>adcWrapper_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>aaf719a53b5dd88722f3f62c9ac16ffe3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; Temperature &gt;</type>
      <name>temperatureWrapper_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a68cefac6f96e3f434036901633872ce6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const tmc9660::BootloaderConfig *</type>
      <name>bootCfg_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a1b5307e03e07d57585e7bcdcb0e7ef2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>device_address_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a1aea362e219afe55d37c69b637d48dc2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>boot_ok_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a7f9a3d129d2f815114ef7d07e544d3ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>handler_mutex_</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a9fe707c03f7c0ea26d83cbd0771780af</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a33c5f6adfbd116fd717ac96c3e00fb0b</anchor>
      <arglist>(BaseSpi &amp;spi, BaseGpio &amp;rst, BaseGpio &amp;drv_en, BaseGpio &amp;faultn, BaseGpio &amp;wake, uint8_t address=0, const tmc9660::BootloaderConfig *bootCfg=&amp;kDefaultBootConfig)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>aed1c579c5456e85070601f22b3f39a01</anchor>
      <arglist>(BaseUart &amp;uart, BaseGpio &amp;rst, BaseGpio &amp;drv_en, BaseGpio &amp;faultn, BaseGpio &amp;wake, uint8_t address=0, const tmc9660::BootloaderConfig *bootCfg=&amp;kDefaultBootConfig)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>acb01f56d0292ce5e7fcad4df46421fdc</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>abcae1e86438604b16a5e92312e7eb8e5</anchor>
      <arglist>(const Tmc9660Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tmc9660Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a15630492c4e23ca9b54e502b5c75ba08</anchor>
      <arglist>(const Tmc9660Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tmc9660Handler</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ab69c4d42a99fb746ea79a0c861efd314</anchor>
      <arglist>(Tmc9660Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Tmc9660Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>af2e2baaba84789c9a5c89a07d965b415</anchor>
      <arglist>(Tmc9660Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>adc564847f8ae26c47412aaee0758991c</anchor>
      <arglist>(bool performReset=true, bool retrieveBootloaderInfo=true, bool failOnVerifyError=false)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a5a279ae9d4b5a8e0d71cb24577ad88ea</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsDriverReady</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a13db97d201a287d8aa60384e04070b9f</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetTmclReplyTimeoutMs</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a419ea352256ec90536a3107047a6b225</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>Gpio &amp;</type>
      <name>gpio</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>acf7d1583239c9f767f82b42fc03b56cf</anchor>
      <arglist>(uint8_t gpioNumber)</arglist>
    </member>
    <member kind="function">
      <type>Adc &amp;</type>
      <name>adc</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a7fe382c4dbf6bf84ed31586e52ec2e6e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Temperature &amp;</type>
      <name>temperature</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a393382194d0602e7aa02da6a8fd7a20a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>tmc9660::CommMode</type>
      <name>GetCommMode</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ac7592c35471c995115e68cc94056b4fb</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>const tmc9660::BootloaderConfig &amp;</type>
      <name>bootConfig</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a2381670960d25634731a697f895c19ab</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>adee8a764564554f060b879752048bf74</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ab0196eb3a096c7250014a4f1b65cb25a</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>SpiDriver *</type>
      <name>driverViaSpi</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a85be9cd2e065b83c7abb36cb70fa6c67</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const SpiDriver *</type>
      <name>driverViaSpi</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ad8946c2dd3a3d5e26620c514988fa2df</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>UartDriver *</type>
      <name>driverViaUart</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ac4f303f9e7be9cb737a667dd5d85dd3a</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const UartDriver *</type>
      <name>driverViaUart</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a4eb74918e10999d77de25728b19542cf</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>DriverVariant</type>
      <name>GetDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ac29a3cfd55b2186f357c8b2f56d16bce</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>ConstDriverVariant</type>
      <name>GetDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>ae1d6f6676180aff28d02dcb954917b79</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a4e219285c12b2ca50075edfaf73b4e2f</anchor>
      <arglist>(Func &amp;&amp;func) noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classTmc9660Handler.html</anchorfile>
      <anchor>a9a641de6a945a44e7f32cb1491eb5991</anchor>
      <arglist>(Func &amp;&amp;func) const noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Ws2812Handler</name>
    <filename>classWs2812Handler.html</filename>
    <class kind="struct">Ws2812Handler::Config</class>
    <member kind="function">
      <type></type>
      <name>Ws2812Handler</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>adffc9bfd3774c28bdd2d9fe72f2e40ad</anchor>
      <arglist>(const Config &amp;config) noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Ws2812Handler</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a681653a0596ee282296caf6a3dc4a070</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Ws2812Handler</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a96c9334a0dbbd5155e83bcb80a393802</anchor>
      <arglist>(const Ws2812Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Ws2812Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>ace55144ab23456289f57c2ff118bd062</anchor>
      <arglist>(const Ws2812Handler &amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Ws2812Handler</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>ab4f3d775827c8cc17f2a5140ebe450f4</anchor>
      <arglist>(Ws2812Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>Ws2812Handler &amp;</type>
      <name>operator=</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a47f4379a948fc0482dbb2e43dc250c46</anchor>
      <arglist>(Ws2812Handler &amp;&amp;)=delete</arglist>
    </member>
    <member kind="function">
      <type>esp_err_t</type>
      <name>Initialize</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>acce4955435f295ba4853b6239090982f</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>EnsureInitialized</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a764cc93c64a81844a284ac7515ea8366</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Deinitialize</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a608af1984272e2924da68109e7985c4e</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsInitialized</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>adcaf3eab4374c5907f332a8892884b44</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>GetNumLeds</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>ae1c94f58ab9ca971f6801c10556192f9</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>WS2812Strip *</type>
      <name>GetStrip</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a7471840392adad3cab496e2dc57dc792</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const WS2812Strip *</type>
      <name>GetStrip</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>af97f5cc26ad1fb0f4722e72346ca7caa</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>WS2812Strip *</type>
      <name>GetDriver</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a2511358625ce2eeb89eae9ca17495b6c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const WS2812Strip *</type>
      <name>GetDriver</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a8180b47ace28b9d2af1e6f610bc46867</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>WS2812Animator *</type>
      <name>GetAnimator</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a3e09ba2c15d5b0e7d6d254e22f634ae8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const WS2812Animator *</type>
      <name>GetAnimator</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>ad564e91a669c0a06f5abde5a3ae8a6a6</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitDriver</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a338df7ac09cd31e3b70d7dd488c1baa6</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept -&gt; decltype(fn(std::declval&lt; WS2812Strip &amp; &gt;()))</arglist>
    </member>
    <member kind="function">
      <type>auto</type>
      <name>visitAnimator</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>ae6bb3e858c86d80d69edc1fced893293</anchor>
      <arglist>(Fn &amp;&amp;fn) noexcept -&gt; decltype(fn(std::declval&lt; WS2812Animator &amp; &gt;()))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DumpDiagnostics</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a586683dcf63f17194b77c026972c7b2b</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetDescription</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a807459e442df75bcfae26a18dcd1622b</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Config</type>
      <name>GetDefaultConfig</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>afaed7a0963afc1bf8288fe17ccacde07</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" protection="private">
      <type>bool</type>
      <name>EnsureInitializedLocked</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>afe5f397b22e9b92cf2fb2cebae6e123d</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Config</type>
      <name>config_</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>ac0a3dc379fcdb7964fe6e6995c37598e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initialized_</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a4a5ea3c76506c449dc37c70d664aa033</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>RtosMutex</type>
      <name>mutex_</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>ab05395f0e7509f80ed86011bddd09d73</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; WS2812Strip &gt;</type>
      <name>strip_</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a4387bbc8038b920f3a2f5851f63307b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>std::unique_ptr&lt; WS2812Animator &gt;</type>
      <name>animator_</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>aba9a27e605659de49f6b8c745cebf050</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>description_</name>
      <anchorfile>classWs2812Handler.html</anchorfile>
      <anchor>a0bc6bf24df5f102cf5c04e1e70eb63a3</anchor>
      <arglist>[64]</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{AlicatBasis2Handler.cpp}</name>
    <filename>namespaceanonymous__namespace_02AlicatBasis2Handler_8cpp_03.html</filename>
    <member kind="function">
      <type>al::BaudRate</type>
      <name>BaudFromBps</name>
      <anchorfile>namespaceanonymous__namespace_02AlicatBasis2Handler_8cpp_03.html</anchorfile>
      <anchor>a50d9ea9c8b3553537fef28c9ee07cb30</anchor>
      <arglist>(std::uint32_t bps) noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsSupportedBaudBps</name>
      <anchorfile>namespaceanonymous__namespace_02AlicatBasis2Handler_8cpp_03.html</anchorfile>
      <anchor>aeb2770bf09308f306ebf0b62df9fab6d</anchor>
      <arglist>(std::uint32_t bps) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetBitmap</name>
      <anchorfile>namespaceanonymous__namespace_02AlicatBasis2Handler_8cpp_03.html</anchorfile>
      <anchor>ac74ab57c3bd5279c52f56c677d73730d</anchor>
      <arglist>(std::uint8_t *bm, std::size_t bytes, std::uint8_t addr) noexcept</arglist>
    </member>
    <member kind="variable">
      <type>constexpr std::uint32_t</type>
      <name>kAllBaudsBps</name>
      <anchorfile>namespaceanonymous__namespace_02AlicatBasis2Handler_8cpp_03.html</anchorfile>
      <anchor>a964fadd70111271f4fac6a475ae1b340</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{Bmp581Handler.cpp}</name>
    <filename>namespaceanonymous__namespace_02Bmp581Handler_8cpp_03.html</filename>
    <member kind="variable">
      <type>constexpr hf_u32_t</type>
      <name>kI2cTimeoutMs</name>
      <anchorfile>namespaceanonymous__namespace_02Bmp581Handler_8cpp_03.html</anchorfile>
      <anchor>a055e48a209faf105c60406736021cc24</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{Logger.cpp}</name>
    <filename>namespaceanonymous__namespace_02Logger_8cpp_03.html</filename>
    <member kind="function">
      <type>hf_log_level_t</type>
      <name>ToBaseLevel</name>
      <anchorfile>namespaceanonymous__namespace_02Logger_8cpp_03.html</anchorfile>
      <anchor>a9d2b496fa1b4f4e28667d14e4b8cb190</anchor>
      <arglist>(LogLevel level) noexcept</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{Max22200Handler.cpp}</name>
    <filename>namespaceanonymous__namespace_02Max22200Handler_8cpp_03.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>g_max_spi_tx</name>
      <anchorfile>namespaceanonymous__namespace_02Max22200Handler_8cpp_03.html</anchorfile>
      <anchor>a25dbc7f7ed5fd79481884a7fab6cd3d5</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>g_max_spi_rx</name>
      <anchorfile>namespaceanonymous__namespace_02Max22200Handler_8cpp_03.html</anchorfile>
      <anchor>a08fe6bda86042b2574e8b17024f89b23</anchor>
      <arglist>[8]</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{Pcal95555Handler.cpp}</name>
    <filename>namespaceanonymous__namespace_02Pcal95555Handler_8cpp_03.html</filename>
    <class kind="struct">anonymous_namespace{Pcal95555Handler.cpp}::NonOwningGpioDeleter</class>
    <member kind="function">
      <type>void</type>
      <name>SyncDeviceAddress</name>
      <anchorfile>namespaceanonymous__namespace_02Pcal95555Handler_8cpp_03.html</anchorfile>
      <anchor>acbb121bbddccbab84878e4fa92fcafed</anchor>
      <arglist>(BaseI2c &amp;i2c, uint8_t addr) noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::shared_ptr&lt; BaseGpio &gt;</type>
      <name>ShareNonOwning</name>
      <anchorfile>namespaceanonymous__namespace_02Pcal95555Handler_8cpp_03.html</anchorfile>
      <anchor>a04966c939c39d4ea76f6fdeacca72c3e</anchor>
      <arglist>(Pcal95555GpioPin *pin) noexcept</arglist>
    </member>
    <member kind="variable">
      <type>volatile uint8_t</type>
      <name>g_pcal_handler_init_stage</name>
      <anchorfile>namespaceanonymous__namespace_02Pcal95555Handler_8cpp_03.html</anchorfile>
      <anchor>aa6180f291805758f9b7e8c3f0159baf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>volatile uint8_t</type>
      <name>g_pcal_last_i2c_cmd</name>
      <anchorfile>namespaceanonymous__namespace_02Pcal95555Handler_8cpp_03.html</anchorfile>
      <anchor>a18ef2f824f03fa130fb6535ed22272a4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{Pf1550Handler.cpp}</name>
    <filename>namespaceanonymous__namespace_02Pf1550Handler_8cpp_03.html</filename>
    <member kind="function">
      <type>void</type>
      <name>driveGpio</name>
      <anchorfile>namespaceanonymous__namespace_02Pf1550Handler_8cpp_03.html</anchorfile>
      <anchor>a0dedac1caced138fdf1d689e1a7d66fb</anchor>
      <arglist>(BaseGpio *pin, pf1550::GpioSignal signal)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{Se050Handler.cpp}</name>
    <filename>namespaceanonymous__namespace_02Se050Handler_8cpp_03.html</filename>
    <member kind="function">
      <type>se050::Error</type>
      <name>MapI2c</name>
      <anchorfile>namespaceanonymous__namespace_02Se050Handler_8cpp_03.html</anchorfile>
      <anchor>a2a6831a1ec2b7c917f283d1788590978</anchor>
      <arglist>(hf_i2c_err_t e) noexcept</arglist>
    </member>
    <member kind="function">
      <type>hf_u16_t</type>
      <name>ClampU16</name>
      <anchorfile>namespaceanonymous__namespace_02Se050Handler_8cpp_03.html</anchorfile>
      <anchor>ac712ade1fe8a1302fb5ee3187909b09e</anchor>
      <arglist>(std::size_t n) noexcept</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{SfmHandler.cpp}</name>
    <filename>namespaceanonymous__namespace_02SfmHandler_8cpp_03.html</filename>
    <member kind="variable">
      <type>constexpr hf_u32_t</type>
      <name>kI2cTimeoutMs</name>
      <anchorfile>namespaceanonymous__namespace_02SfmHandler_8cpp_03.html</anchorfile>
      <anchor>a85399d204f743d67d5b96c84a2220f35</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{Tle92466edHandler.cpp}</name>
    <filename>namespaceanonymous__namespace_02Tle92466edHandler_8cpp_03.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>g_tle_spi_tx</name>
      <anchorfile>namespaceanonymous__namespace_02Tle92466edHandler_8cpp_03.html</anchorfile>
      <anchor>a1cbbee10486d119395b27bee926fa6a3</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>g_tle_spi_rx</name>
      <anchorfile>namespaceanonymous__namespace_02Tle92466edHandler_8cpp_03.html</anchorfile>
      <anchor>aa460383e4f42285b8eac26a9ce530413</anchor>
      <arglist>[4]</arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>kTleChainMaxFrames</name>
      <anchorfile>namespaceanonymous__namespace_02Tle92466edHandler_8cpp_03.html</anchorfile>
      <anchor>a7586fe5a1ed91d08bd2830744b76e5a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint32_t</type>
      <name>kTleChainGapUs</name>
      <anchorfile>namespaceanonymous__namespace_02Tle92466edHandler_8cpp_03.html</anchorfile>
      <anchor>a9cd2ca3aaa82b95d73fddc25256a9011</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>g_tle_chain_tx</name>
      <anchorfile>namespaceanonymous__namespace_02Tle92466edHandler_8cpp_03.html</anchorfile>
      <anchor>a71b823eb16626c98ca26429fc57387cf</anchor>
      <arglist>[kTleChainMaxFrames][4]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>g_tle_chain_rx</name>
      <anchorfile>namespaceanonymous__namespace_02Tle92466edHandler_8cpp_03.html</anchorfile>
      <anchor>a795ef095919fea283c4e35851da542ac</anchor>
      <arglist>[kTleChainMaxFrames][4]</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>anonymous_namespace{Tmc9660Handler.cpp}</name>
    <filename>namespaceanonymous__namespace_02Tmc9660Handler_8cpp_03.html</filename>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>setGpioFromSignal</name>
      <anchorfile>namespaceanonymous__namespace_02Tmc9660Handler_8cpp_03.html</anchorfile>
      <anchor>a747df189a572cdfd86ec4d3f74160078</anchor>
      <arglist>(BaseGpio &amp;gpio_pin, tmc9660::GpioSignal signal, bool) noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>readGpioToSignal</name>
      <anchorfile>namespaceanonymous__namespace_02Tmc9660Handler_8cpp_03.html</anchorfile>
      <anchor>a9dc9d5dcab6377d0a891fcfc400abe10</anchor>
      <arglist>(BaseGpio &amp;gpio_pin, tmc9660::GpioSignal &amp;signal, bool) noexcept</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>handler_utils</name>
    <filename>namespacehandler__utils.html</filename>
    <member kind="function">
      <type>void</type>
      <name>RouteLogToLogger</name>
      <anchorfile>namespacehandler__utils.html</anchorfile>
      <anchor>a71864e1d5801bea0d6d830326b6b183a</anchor>
      <arglist>(int level, const char *tag, const char *format, va_list args) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayMs</name>
      <anchorfile>namespacehandler__utils.html</anchorfile>
      <anchor>afc7fd91265677d0865cbc2a413754ed8</anchor>
      <arglist>(uint32_t ms) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DelayUs</name>
      <anchorfile>namespacehandler__utils.html</anchorfile>
      <anchor>ac2999c5753a6d86504463b742c23dcfe</anchor>
      <arglist>(uint32_t us) noexcept</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>logger_decor</name>
    <filename>namespacelogger__decor.html</filename>
    <member kind="function">
      <type>void</type>
      <name>BannerColored</name>
      <anchorfile>namespacelogger__decor.html</anchorfile>
      <anchor>a242f5384a18a9eee59577d39a6bc6d21</anchor>
      <arglist>(const char *tag, const char *line1, LogColor color) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Banner</name>
      <anchorfile>namespacelogger__decor.html</anchorfile>
      <anchor>a92743352b2c068e87fab4bb2715cfc7d</anchor>
      <arglist>(const char *tag, const char *line1) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Section</name>
      <anchorfile>namespacelogger__decor.html</anchorfile>
      <anchor>ad336dcc7bd41082a5176af0e1c631f8f</anchor>
      <arglist>(const char *tag, const char *title) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Rule</name>
      <anchorfile>namespacelogger__decor.html</anchorfile>
      <anchor>ac5bed90d6e56255709791c8796eb02bf</anchor>
      <arglist>(const char *tag) noexcept</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>sm_test</name>
    <filename>namespacesm__test.html</filename>
    <class kind="struct">sm_test::Owner</class>
    <member kind="typedef">
      <type>hf_utils::StateMachine&lt; Owner, S, static_cast&lt; size_t &gt;(S::COUNT)&gt;</type>
      <name>SM</name>
      <anchorfile>namespacesm__test.html</anchorfile>
      <anchor>a42545b4c565ca63b9f237e3d7cee1216</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>S</name>
      <anchorfile>namespacesm__test.html</anchorfile>
      <anchor>a477b1d6911e837558e72fd69691eb1fc</anchor>
      <arglist></arglist>
      <enumvalue file="namespacesm__test.html" anchor="a477b1d6911e837558e72fd69691eb1fca7fc56270e7a70fa81a5935b72eacbe29">A</enumvalue>
      <enumvalue file="namespacesm__test.html" anchor="a477b1d6911e837558e72fd69691eb1fca9d5ed678fe57bcca610140957afab571">B</enumvalue>
      <enumvalue file="namespacesm__test.html" anchor="a477b1d6911e837558e72fd69691eb1fca0d61f8370cad1d412f80b84d143e1257">C</enumvalue>
      <enumvalue file="namespacesm__test.html" anchor="a477b1d6911e837558e72fd69691eb1fca4905ac9d6a22bdfc1ae096094ce6248d">COUNT</enumvalue>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>register_all</name>
      <anchorfile>namespacesm__test.html</anchorfile>
      <anchor>aa64c8572ce750eeec442c2680079ddd8</anchor>
      <arglist>(SM &amp;sm, Owner &amp;o)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>MAX22200_HAL_CommAdapter</name>
    <title>HAL Communication Adapter</title>
    <filename>group__MAX22200__HAL__CommAdapter.html</filename>
    <class kind="class">HalSpiMax22200Comm</class>
  </compound>
  <compound kind="group">
    <name>MAX22200_Handler</name>
    <title>Main Handler Class</title>
    <filename>group__MAX22200__Handler.html</filename>
    <class kind="class">Max22200Handler</class>
  </compound>
  <compound kind="group">
    <name>PCA9685_HAL_I2CAdapter</name>
    <title>HAL I2C Communication Adapter</title>
    <filename>group__PCA9685__HAL__I2CAdapter.html</filename>
    <class kind="class">HalI2cPca9685Comm</class>
  </compound>
  <compound kind="group">
    <name>PCA9685_HAL_PwmAdapter</name>
    <title>Multi-Channel PWM Adapter</title>
    <filename>group__PCA9685__HAL__PwmAdapter.html</filename>
    <class kind="class">Pca9685PwmAdapter</class>
  </compound>
  <compound kind="group">
    <name>PCA9685_HAL_GpioPin</name>
    <title>Per-Pin GPIO Wrapper</title>
    <filename>group__PCA9685__HAL__GpioPin.html</filename>
    <class kind="class">Pca9685GpioPin</class>
  </compound>
  <compound kind="group">
    <name>PCA9685_HAL_Handler</name>
    <title>PCA9685 Handler</title>
    <filename>group__PCA9685__HAL__Handler.html</filename>
    <class kind="class">Pca9685Handler</class>
  </compound>
  <compound kind="group">
    <name>PCAL95555_HAL_I2CAdapter</name>
    <title>HAL I2C Communication Adapter</title>
    <filename>group__PCAL95555__HAL__I2CAdapter.html</filename>
    <class kind="class">HalI2cPcal95555Comm</class>
  </compound>
  <compound kind="group">
    <name>PCAL95555_HAL_GpioPin</name>
    <title>Per-Pin GPIO Wrapper</title>
    <filename>group__PCAL95555__HAL__GpioPin.html</filename>
    <class kind="class">Pcal95555GpioPin</class>
  </compound>
  <compound kind="group">
    <name>PCAL95555_HAL_Handler</name>
    <title>PCAL95555 Handler</title>
    <filename>group__PCAL95555__HAL__Handler.html</filename>
    <class kind="class">Pcal95555Handler</class>
  </compound>
  <compound kind="group">
    <name>TLE92466ED_HAL_CommAdapter</name>
    <title>HAL Communication Adapter</title>
    <filename>group__TLE92466ED__HAL__CommAdapter.html</filename>
    <class kind="class">HalSpiTle92466edComm</class>
  </compound>
  <compound kind="group">
    <name>TLE92466ED_Handler</name>
    <title>Main Handler Class</title>
    <filename>group__TLE92466ED__Handler.html</filename>
    <class kind="class">Tle92466edHandler</class>
  </compound>
  <compound kind="group">
    <name>TMC5160_HAL_CommAdapters</name>
    <title>HAL Communication Adapters</title>
    <filename>group__TMC5160__HAL__CommAdapters.html</filename>
    <class kind="struct">Tmc5160CtrlPins</class>
    <class kind="class">HalSpiTmc5160Comm</class>
    <class kind="class">HalUartTmc5160Comm</class>
  </compound>
  <compound kind="group">
    <name>TMC5160_Handler</name>
    <title>Main Handler Class</title>
    <filename>group__TMC5160__Handler.html</filename>
    <class kind="class">Tmc5160Handler</class>
  </compound>
  <compound kind="group">
    <name>TMC9660_HAL_CommAdapters</name>
    <title>HAL Communication Adapters</title>
    <filename>group__TMC9660__HAL__CommAdapters.html</filename>
    <class kind="struct">Tmc9660CtrlPins</class>
    <class kind="class">HalSpiTmc9660Comm</class>
    <class kind="class">HalUartTmc9660Comm</class>
  </compound>
  <compound kind="group">
    <name>TMC9660_HAL_Handler</name>
    <title>TMC9660 Handler</title>
    <filename>group__TMC9660__HAL__Handler.html</filename>
    <class kind="class">Tmc9660Handler</class>
  </compound>
  <compound kind="group">
    <name>WS2812_Handler</name>
    <title>WS2812 LED Strip Handler</title>
    <filename>group__WS2812__Handler.html</filename>
    <class kind="class">Ws2812Handler</class>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>HF-Core Platform</title>
    <filename>index.html</filename>
    <docanchor file="index.html" title="HF-Core Platform">md_README</docanchor>
  </compound>
</tagfile>
