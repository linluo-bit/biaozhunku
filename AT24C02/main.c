void write_and_display_data() {
    uint8_t data_to_write = 18;
    uint8_t read_data;

    // 初始化AT24C02和OLED
    AT24C02_Init();
    OLED_Init();

    // 向AT24C02写入数据
    AT24C02_WriteOneByte(0x00, data_to_write);

    // 从AT24C02读取数据
    read_data = AT24C02_ReadOneByte(0x00);

    // 在OLED上显示数据
    OLED_ShowNum(0, 0, read_data, 2, 16); // 假设在(0, 0)位置显示2位数，使用字体大小为16

    while (1) {
        // 主循环中的其他代码
    }
}
