
#include <SPI.h>
#include <RHSPIDriver.h>
#include <RHGenericDriver.h>
#include <Omni_RF69.h>

// Singleton instance of the radio driver
Omni_RF69 rf69(33, 12);

void setup() 
{
    Serial.begin(115200);
    while (!Serial) ; // Wait for serial port to be available

    if (!rf69.init())
        Serial.println("init failed");

    Serial.printf("register 0x10 value: %x \n", rf69.spiRead(0x10));
    // set operating mode to stand-by
    rf69.spiWrite(0x01, 0x01);

    // rf69.spiWrite(0x02, 0x00); // set packet mode and FSK with no shaping (gaussian filter bt = 0.0)
    //rf69.spiWrite(0x02, 0b01000000); // set continous packet mode and FSK with no shaping (gaussian filter bt = 0.0)
    rf69.spiWrite(0x02, 0b01000001); // set continous packet mode and FSK with shaping (gaussian filter bt = 0.3)
    //rf69.spiWrite(0x02, 0b01000010); // set continous packet mode and FSK with no shaping (gaussian filter bt = 0.5)
    //rf69.spiWrite(0x02, 0b01000011); // set continous packet mode and FSK with no shaping (gaussian filter bt = 1.0)

    // bitrate MSB & LSB
    // 32e6 / (787 + 11/16) ~= 40625.248
    rf69.spiWrite(0x03, 0x03);
    rf69.spiWrite(0x04, 0x13);

    // freq deviation: 26370
    // fstep = 61Hz
    // 432 steps
    rf69.spiWrite(0x05, 0x01);
    rf69.spiWrite(0x06, 0xb0);

    rf69.setFrequency(433.910); // omnipod center frequency

    rf69.spiWrite(0x0D, 0b10010010); // default resolutions for listen mode
    rf69.spiWrite(0x0E, 0xF5); // idle phase duration
    rf69.spiWrite(0x0F, 0x20); // rx phase duration

    rf69.spiWrite(0x11, 0b1001111); // pa level
    rf69.spiWrite(0x12, 0b00001001); // pa ramp
    rf69.spiWrite(0x13, 0b00011010); // ocp
    
    rf69.spiWrite(0x18, 0b10001000); // lna default AGC
    rf69.spiWrite(0x19, 0b01010101); // default freq cut-off and filter bw
    rf69.spiWrite(0x1A, 0b10001011); // default freq cut-off and filter bw params


    rf69.spiWrite(0x2c, 0x00); // preamble size msb
    rf69.spiWrite(0x2d, 0x08); // preamble size lsb => 8 bytes

    //rf69.spiWrite(0x2e, 0b11001000); // sync enable, size 2 bytes
    // rf69.spiWrite(0x2f, 0x5a); // sync val 1
    // rf69.spiWrite(0x30, 0xa5); // sync val 2

    rf69.spiWrite(0x2e, 0x00); // sync disabled

    rf69.spiWrite(0x37, 0x20); // fixed packet len, crc off, no encoding, no address filter
    rf69.spiWrite(0x38, 0x40); // packet payload 64 bytes

    rf69.spiWrite(0x3b, 0x00); // disable entering intermediate modes
    rf69.spiWrite(0x3d, 0x02); // auto restart rx, no aes
}

void loop()
{
    // uint8_t data[] = "Well hello world!1!";
    // rf69.send(data, sizeof(data));
    // rf69.waitPacketSent();
    // Serial.print(".");

    rf69.spiWrite(0x01, 0x11010000); // force listen mode
    uint8_t buf[Omni_RF69_FIFO_SIZE];
    uint8_t len = sizeof(buf);
    if (rf69.waitAvailableTimeout(2000))
    { 
        if (rf69.recv(buf, &len))
        {
            Serial.println("\ngot packli: ");
            for(int i=0; i<len; i++)
                Serial.printf("%2x", buf[i]);
            Serial.println("");
        }
        else
        {
            Serial.println("\nrecv failed");
        }
    }
    else
    {
        Serial.print(".");
    }
}