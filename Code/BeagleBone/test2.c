/*
 ============================================================================
 Name        : test2.c
 Author      : Tommy Tasch
 Description : Receives commands over UDP and then applies these changes to
 certain files in the BeagleBone.
 ============================================================================
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void mode_set(char pin[11], char mode[1]);
void set_duty(char pin_num, char percent[3]);
void set_freq(char pin_num, char freq[5]);
void set_run(char pin_num, char run[1]);
void set_pwms();

#define BUFLEN 512  //Max length of buffer
#define PORT 60200   //The port on which to listen for incoming data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void) {
	// UDP Creation

    struct sockaddr_in si_me, si_other;

    int s, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];
    char motor1_2, motor1_1,motor2_2, motor2_1,motor3_2, motor3_1,motor4_2, motor4_1,motor5_2, motor5_1,motor6_2, motor6_1;
    char motor1[3], motor2[3], motor3[3], motor4[3], motor5[3], motor6[3];

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }

    //Set up pwms
    char freq[5];
    int i, motor_num;
	set_pwms();
	//Ask the user what the frequency should be
	printf("What should the frequency be? ");
	fflush(stdout);
	scanf("%s", freq);
	//Set all frequencies to the user input
	for(i='1';i<'7';i++){
			set_freq(i,freq);
	}
	//Activate the run file in each pwm folder
	for(i='1';i<'7';i++){
			set_run(i,"1");
	}
	//Set the duty percent to 50 for all pwms
	for(i='1';i<'7';i++){
			set_duty(i,"50");
	}
	while(1){

        //try to receive some data
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }

        // Figure out the number of motors from first three bytes of the array received from UDP packet
        motor_num = buf[3];
        int motors[motor_num];
        // Create an array that has the duty percent for each motor in the elements
        for(i=0; i<motor_num; i++){
        	motors[i] = buf[i+4];
        	if(motors[i] > 100) motors[i] = motors[i] - 256; //Conversion for numbers that should be negative
        }
        fflush(stdout);

        //Change the percentages from -100-100 to 0-100 because that's the percentages the BeagleBone can do
        for(i=0; i<6; i++){
        	motors[i] = (motors[i] + 100)/2;
        }

        // The next bunch of if else statements convert the integer duty percent number into strings that the
        // Beaglebone can understand.  Each statement will go through the first and second digit of the integer
        // and then change them to ASCII code and put them into an array.
        if(motors[0] == 100){
        	motor1[0] = '1';
        	motor1[1] = '0';
        	motor1[2] = '0';
        	motor1[3] = '\0';
        }
        else {
            motor1_2 = motors[0]%10 + 0x30;
            motor1_1 = (motors[0]/10)%10 + 0x30;
            motor1[0] = '0';
            motor1[1] = motor1_1;
            motor1[2] = motor1_2;
            motor1[3] = '\0';
        }
        if(motors[1] == 100){
        	motor2[0] = '1';
        	motor2[1] = '0';
        	motor2[2] = '0';
            motor2[3] = '\0';
        }
        else {
            motor2_2 = motors[1]%10 + 0x30;
            motor2_1 = (motors[1]/10)%10 + 0x30;
            motor2[0] = '0';
            motor2[1] = motor2_1;
            motor2[2] = motor2_2;
            motor2[3] = '\0';
        }
        if(motors[2] == 100){
        	motor3[0] = '1';
        	motor3[1] = '0';
        	motor3[2] = '0';
            motor3[3] = '\0';
        }
        else {
            motor3_2 = motors[2]%10 + 0x30;
            motor3_1 = (motors[2]/10)%10 + 0x30;
            motor3[0] = '0';
            motor3[1] = motor3_1;
            motor3[2] = motor3_2;
            motor3[3] = '\0';
        }
        if(motors[3] == 100){
        	motor4[0] = '1';
        	motor4[1] = '0';
        	motor4[2] = '0';
        	motor4[3] = '\0';
        }
        else {
            motor4_2 = motors[3]%10 + 0x30;
            motor4_1 = (motors[3]/10)%10 + 0x30;
            motor4[0] = '0';
            motor4[1] = motor4_1;
            motor4[2] = motor4_2;
            motor4[3] = '\0';
        }
        if(motors[4] == 100){
        	motor5[0] = '1';
        	motor5[1] = '0';
        	motor5[2] = '0';
            motor5[3] = '\0';
        }
        else {
            motor5_2 = motors[4]%10 + 0x30;
            motor5_1 = (motors[4]/10)%10 + 0x30;
            motor5[0] = '0';
            motor5[1] = motor5_1;
            motor5[2] = motor5_2;
            motor5[3] = '\0';
        }
        if(motors[5] == 100){
        	motor6[0] = '1';
        	motor6[1] = '0';
        	motor6[2] = '0';
            motor6[3] = '\0';
        }
        else {
            motor6_2 = motors[5]%10 + 0x30;
            motor6_1 = (motors[5]/10)%10 + 0x30;
            motor6[0] = '0';
            motor6[1] = motor6_1;
            motor6[2] = motor6_2;
            motor6[3] = '\0';
        }

        // Print all the duty percents that were calculated
        printf("First Motor: %s\n", motor1);
        printf("Second Motor: %s\n", motor2);
        printf("Third Motor: %s\n", motor3);
        printf("Fourth Motor: %s\n", motor4);
        printf("Fifth Motor: %s\n", motor5);
        printf("Sixth Motor: %s\n\n", motor6);

        // Set the duty percent of each motor
    	set_duty('1', motor1);
    	set_duty('2', motor2);
    	set_duty('3', motor3);
    	set_duty('4', motor4);
    	set_duty('5', motor5);
    	set_duty('6', motor6);

	}

	close(s);
	return 0;
}

// This function intakes a mode and pin as characters and sets that pin to that mode
void mode_set(char pin[11], char mode[1]){

	// Create file handler and start space to be looked at
	FILE *PWMHandle = NULL;
	char ModeSpace[40] = "/sys/kernel/debug/omap_mux/";

	// Finish file name from parameter
	strcat(ModeSpace,pin);

	// Go into the file and change it
	if ((PWMHandle = fopen(ModeSpace, "r+")) != NULL){
		fwrite(mode,sizeof(char),1,PWMHandle);
		fclose(PWMHandle);
	}

}

// This function will set all the pwms that will be used on the Beaglebone
void set_pwms(){
	mode_set("mcasp0_fsx", "1"); // Pin9 29 , 0B
	mode_set("mcasp0_aclkx","1"); // Pin9 31, 0A
	mode_set("gpmc_a2", "6"); // Pin9 14, 1A
	mode_set("gpmc_a3","6"); // Pin9 16, 1B
	mode_set("gpmc_ad8","4"); // Pin8 19, 2A
	mode_set("gpmc_ad9","4"); // Pin8 13, 2B
}

// This function takes in a percentage and pin number as characters and then sets that percentage to the pin number
void set_duty(char pin_num, char percent[3]){

	// Create file handle and start the file name it will go to
	FILE *PWMHandle = NULL;
	char PercentSpace[40] = "/sys/class/pwm/";
	char pin[11];

	//Based on the pin number given in the parameter figure out what the file name should be
	switch(pin_num){

	case '1':
		strcpy(pin,"ehrpwm.0:0");
		break;
	case '2':
		strcpy(pin,"ehrpwm.0:1");
		break;
	case '3':
		strcpy(pin,"ehrpwm.1:0");
		break;
	case '4':
		strcpy(pin,"ehrpwm.1:1");
		break;
	case '5':
		strcpy(pin,"ehrpwm.2:0");
		break;
	case '6':
		strcpy(pin,"ehrpwm.2:1");
		break;
	}

	// Complete the file name
	strcat(PercentSpace,pin);
	strcat(PercentSpace, "/duty_percent");

	// Go into file and change duty percent
	if ((PWMHandle = fopen(PercentSpace, "r+")) != NULL){
		fwrite(percent,sizeof(char),3,PWMHandle);
		fclose(PWMHandle);
	}
}

// This function intakes frequency as a character and pin number and then sets the pin number to that frequency
void set_freq(char pin_num, char freq[5]){

	// Create file handler and start the file name
	FILE *PWMHandle = NULL;
	char FreqSpace[40] = "/sys/class/pwm/";
	char pin[11];

	// Based on the pin number given in the parameter figure out what the file name should be
	switch(pin_num){

	case '1':
		strcpy(pin,"ehrpwm.0:0");
		break;
	case '2':
		strcpy(pin,"ehrpwm.0:1");
		break;
	case '3':
		strcpy(pin,"ehrpwm.1:0");
		break;
	case '4':
		strcpy(pin,"ehrpwm.1:1");
		break;
	case '5':
		strcpy(pin,"ehrpwm.2:0");
		break;
	case '6':
		strcpy(pin,"ehrpwm.2:1");
		break;
	}

	// Complete the file name
	strcat(FreqSpace,pin);
	strcat(FreqSpace, "/period_freq");

	// Go into file and change frequency
	if ((PWMHandle = fopen(FreqSpace, "r+")) != NULL){
		fwrite(freq,sizeof(char),5,PWMHandle);
		fclose(PWMHandle);
	}
}

// This function will take in the pin number and a 1 or 0 as run to turn the pwm on or off
void set_run(char pin_num, char run[1]){

	// Create file handler and start file name
	FILE *PWMHandle = NULL;
	char RunSpace[40] = "/sys/class/pwm/";
	char pin[11];

	// Based on the pin number given in the parameter figure out what the file name should be
	switch(pin_num){

	case '1':
		strcpy(pin,"ehrpwm.0:0");
		break;
	case '2':
		strcpy(pin,"ehrpwm.0:1");
		break;
	case '3':
		strcpy(pin,"ehrpwm.1:0");
		break;
	case '4':
		strcpy(pin,"ehrpwm.1:1");
		break;
	case '5':
		strcpy(pin,"ehrpwm.2:0");
		break;
	case '6':
		strcpy(pin,"ehrpwm.2:1");
		break;
	}

	// Complete the file name
	strcat(RunSpace,pin);
	strcat(RunSpace, "/run");

	// Go into file and change the run value
	if ((PWMHandle = fopen(RunSpace, "r+")) != NULL){
		fwrite(run,sizeof(char),1,PWMHandle);
		fclose(PWMHandle);
	}
}
