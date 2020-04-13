#include "mbed.h"
#include "fsl_port.h"

#include "fsl_gpio.h"


#define UINT14_MAX        16383

// FXOS8700CQ I2C address

#define FXOS8700CQ_SLAVE_ADDR0 (0x1E<<1) // with pins SA0=0, SA1=0

#define FXOS8700CQ_SLAVE_ADDR1 (0x1D<<1) // with pins SA0=1, SA1=0

#define FXOS8700CQ_SLAVE_ADDR2 (0x1C<<1) // with pins SA0=0, SA1=1

#define FXOS8700CQ_SLAVE_ADDR3 (0x1F<<1) // with pins SA0=1, SA1=1

// FXOS8700CQ internal register addresses

#define FXOS8700Q_STATUS 0x00

#define FXOS8700Q_OUT_X_MSB 0x01

#define FXOS8700Q_OUT_Y_MSB 0x03

#define FXOS8700Q_OUT_Z_MSB 0x05

#define FXOS8700Q_M_OUT_X_MSB 0x33

#define FXOS8700Q_M_OUT_Y_MSB 0x35

#define FXOS8700Q_M_OUT_Z_MSB 0x37

#define FXOS8700Q_WHOAMI 0x0D

#define FXOS8700Q_XYZ_DATA_CFG 0x0E

#define FXOS8700Q_CTRL_REG1 0x2A

#define FXOS8700Q_M_CTRL_REG1 0x5B

#define FXOS8700Q_M_CTRL_REG2 0x5C

#define FXOS8700Q_WHOAMI_VAL 0xC7
void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len);
void FXOS8700CQ_writeRegs(uint8_t * data, int len);


I2C i2c( PTD9,PTD8);
Serial pc(USBTX, USBRX);

Timer timer;
Timer tt;
int m_addr = FXOS8700CQ_SLAVE_ADDR1;
uint8_t who_am_i, data[2], res[6];
int16_t acc16;
float tilt[100];    // for storing the time which has tilt
// float t[3];         // for storing accel value for one move
float X[100], Y[100], Z[100];

int i=0;

EventQueue eq;
DigitalOut redLED(LED1);
DigitalIn Switch(SW2);

void log_acc(void);


int main(){

  
    pc.baud(115200);

    // Enable the FXOS8700Q
    FXOS8700CQ_readRegs( FXOS8700Q_CTRL_REG1, &data[1], 1);
    data[1] |= 0x01;
    data[0] = FXOS8700Q_CTRL_REG1;
    FXOS8700CQ_writeRegs(data, 2);

    // Get the slave address
    FXOS8700CQ_readRegs(FXOS8700Q_WHOAMI, &who_am_i, 1);
    //pc.printf("Here is %x\r\n", who_am_i);

    while(1){
        // timer.start();
        // while(timer.read()<2) {
        redLED = !redLED;             //red led blink
        wait(0.2);
        // }
        if(Switch==0){
            eq.call_every(100, log_acc);  //call log function every 0.1 sec
            eq.dispatch(10000);
            break;
        }
    
    }
    for(i=0; i<100; i++){
        // pc.printf("%1.2f\r\n", X[i]);
        pc.printf("%1.2f\r\n %1.2f\r\n %1.2f\r\n", X[i], Y[i], Z[i]);
        pc.printf("%f\r\n", tilt[i]);
    }
    //pc.printf("end\n");

}


void log_acc(void){
    if(i<100){
        FXOS8700CQ_readRegs(FXOS8700Q_OUT_X_MSB, res, 6);

        acc16 = (res[0] << 6) | (res[1] >> 2);
        if (acc16 > UINT14_MAX/2)
            acc16 -= UINT14_MAX;
        X[i] = ((float)acc16) / 4096.0f;


        acc16 = (res[2] << 6) | (res[3] >> 2);
        if (acc16 > UINT14_MAX/2)
            acc16 -= UINT14_MAX;
        Y[i] = ((float)acc16) / 4096.0f;

        acc16 = (res[4] << 6) | (res[5] >> 2);
        if (acc16 > UINT14_MAX/2)
        acc16 -= UINT14_MAX;
        Z[i] = ((float)acc16) / 4096.0f;
 
        
        if(X[i]>0.5 || X[i]<-0.5 || Y[i]>0.5 || Y[i]<-0.5 || Z[i]<0.5){
            tilt[i]= 1;
        }
        else tilt[i]= 0;
        //pc.printf("tilt[%d]= %f\r\n", i, tilt[i]);
        
        i++;
    }

}


void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len) {

   char t = addr;

   i2c.write(m_addr, &t, 1, true);

   i2c.read(m_addr, (char *)data, len);

}


void FXOS8700CQ_writeRegs(uint8_t * data, int len) {

   i2c.write(m_addr, (char *)data, len);

}