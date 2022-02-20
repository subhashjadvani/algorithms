// -------------------------------------------------------
/*
-----------
   Setup
-----------
A small embedded controller is being built.

It will be connected to a PC using a standard UART/RS232 serial
cable.

System constraints require that all code must be implemented to
operate cooperatively in a non-blocking tasking environment. (there is
no os to do this).

The structure of this environment is very simple and can be
generalized to the form.
*/
int main_loop(void)
{
    while (1) {
        // serial input.
        if (check_message_from_pc(...)) {
            process_message(...);
        }
        //  ... other real time tasks ...
    }
}

/*

-----------
Data Format
-----------

We would like to allow the controller to accept configuration from the
PC using a high-level protocol that takes the form of the following
characters

0x02    LENGTH (1-255)    DATA0    [DATA1]    ...    CRC8

The final byte CRC8 must checked against all data bytes including the
length.

-----------
Given API
-----------
The following API is provided.
*/

// checks for any bytes are in the hardware serial buffer
// if ready, returns TRUE, else FALSE
bool serial_ready(void);

// read the next available byte in the hardware serial buffer
// calling this when !serial_ready() will crash..
uint8_t serial_read(void);

// calculates the CRC8 for a data buffer
uint8_t calc_crc8(uint8_t * p_data, size_t length);


// -------------------------------------------------------

/*

The task here is to implement the function

check_message_from_pc(args);

which will be added as shown to the
main while(1) loop to poll the serial port and pass on a complete
message if done.

To keep the controller responsive, the architecture requires that no
function should block.

The resulting function may convey a complete message in any form the
author desires.  Global variables are allowed and so is limited use
of std c lib.
*/

enum msg_states {
	WAIT_FOR_MARKER,
	WAIT_FOR_LENGTH,
	WAIT_FOR_DATA,
	WAIT_FOR_CRC
};

struct msg_t {
	enum msg_states state;
	uint8_t length;
	uint8_t buffer[255];
	uint8_t buf_off;
};

struct msg_t msg_context;

bool check_message_from_pc(...) {
	/*
	// checks for any bytes are in the hardware serial buffer
	// if ready, returns TRUE, else FALSE
	bool serial_ready(void);

	// read the next available byte in the hardware serial buffer
	// calling this when !serial_ready() will crash..
	uint8_t serial_read(void);

	// calculates the CRC8 for a data buffer
	uint8_t calc_crc8(uint8_t * p_data, size_t length);
	*/
	// 0x02    LENGTH (1-255)    DATA0    [DATA1]    ...    CRC8
	// 0x02 0x4 0xAA 0xBB 0xCC 0xDD 0xff

	while (serial_ready()) { // while may not be good too!
		switch (msg_context.state) {
			case WAIT_FOR_MARKER:
				if (serial_read() == 0x2)
					msg_context.state = WAIT_FOR_LENGTH;
				break;
			case WAIT_FOR_LENGTH:
				msg_context.length = serial_read();
				msg_context.buf_off = 0;
				if (msg_context.length != 0)
					msg_context.state = WAIT_FOR_DATA;
				else
					msg_context.state = WAIT_FOR_MARKER;
				break;
			case WAIT_FOR_DATA:
				if (msg_context.buf_off < msg_context.length) {
					msg_context.buffer[msg_context.buf_off] = serial_read();
					msg_context.buf_off++;
				}
				if (msg_context.buf_off == msg_context.length)
					msg_context.state = WAIT_FOR_CRC;
				break;
			case WAIT_FOR_CRC:
				msg_context.state = WAIT_FOR_MARKER;					
				// if whole message is received and crc check passed
				if (serial_read() == calc_crc8(msg_context.buffer, msg_context.length))
					return true;
				break;
		}
	}

	return false;
}

int main_loop(void)
{
	msg_context.state = WAITING_FOR_MARKER;

	while (1) {
		// serial input.
		if (check_message_from_pc(...)) {
			process_message(...);
		}
		//  ... other real time tasks ...
	}
}
