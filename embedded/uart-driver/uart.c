/*
Given the hardware description in the code sample,
complete the interrupt service routine,
character read and character write routines
*/
.
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

///////////////////////////////////////////////////////
// External routines
///////////////////////////////////////////////////////
// FIXME: Add any mutual exclusion functions you need, ie:
// void IRQ_DISABLE(void);
// void IRQ_ENABLE(void);
// void spinlock_acquire(bool *spin);
// void spinlock_release(bool *spin);
#define __interrupt __attribute__((__interrupt__))
#define __packed __attribute__((__packed__))

///////////////////////////////////////////////////////
// UART FIFO Hardware description
///////////////////////////////////////////////////////
#define UART_FIFO_ADDRESS 0x12000000 // 0x0 - Rx, 0x01- TX
#define UART_FIFO_SIZE  8   // Depth of the FIFO (RX and TX depths are the same)
#define UART_FIFO_FLAG_RX_NOT_EMPTY     0x01    // Set when the RX fifo has at least 1 character in it.
#define UART_FIFO_FLAG_RX_FULL          0x02    // Set when the RX fifo is full.
#define UART_FIFO_FLAG_RX_DROPPED       0x04    // Set if the RX fifo has dropped one or more characters.
#define UART_FIFO_FLAG_TX_EMPTY         0x10    // Set when the TX fifo is empty.
#define UART_FIFO_FLAG_TX_FULL          0x20    // Set when the TX fifo is full.

// This the memory layout of the UART FIFO hardware.
typedef struct {
    volatile uint8_t fifo_rx;  // Read to get the next RX fifo character.
    volatile uint8_t fifo_tx;  // Write to add a chracter to the TX fifo.
    volatile uint8_t flags; //readonly    // Current status of the FIFO
    volatile uint8_t flags_ie; //r+ w Flags that, when set, will cause an interrupt to fire.
/*
0x1200_0000 -> Rx
0x1200_0001 -> Tx
0x1200_0002 -> flags
0x1200_0003 -> flags_ie
*/
} __packed hw_uart_fifo_t;

/*
typedef struct {
	hw_uart_fifo_t *fifo_mmio;
	spinlock_t lock;
	semaphore_t tx;
	semaphore_t rx;
} uart_t;
*/

///////////////////////////////////////////////////////
// UART FIFO software API
///////////////////////////////////////////////////////
// UART software

// This function is called by the CPU when an interrupt occurs on the UART FIFO.
void __interrupt ISR_uart_fifo(uart_t *uart)
{
 	// FIXME: Add your code here to service the UART FIFO for TX and RX.
	if (uart->fifo_mmio->flags & UART_FIFO_FLAG_TX_FULL != UART_FIFO_FLAG_TX_FULL)
		uart->tx.signal();

	if (uart->fifo_mmio->flags & UART_FIFO_FLAG_RX_NOT_EMPTY != UART_FIFO_FLAG_RX_NOT_EMPTY)
		uart->rx.signal();
}

spinlock_t singleton_lock;

// This function returns an empty uart_t (singleton).
uart_t *uart_alloc(void)
{
	// FIXME: Add your code here to set up the uart_t singleton.

	static uart_t *u = NULL;
	if (u == NULL) {
		spinlock_acquire(&singleton_lock);
		if (u == NULL) {
		
			u = (uart_t *) malloc(sizeof(uart_t));
			// add null check

			u->fifo_mmio = (hw_uart_fifo_t *) UART_FIFO_ADDRESS;
			spinlock_init(&u->lock);
			// TODO: initialize the semaphores
			semaphore_init(&u->tx, 0);
			semaphore_init(&u->rx, 0);
		}
		spinlock_release(&singleton_lock);
	}

	return u;
}

// This function attempts to send up to 256 bytes, and returns the amount sent.
size_t uart_tx(uart_t *uart, const uint8_t *buffer, size_t size)
{
	// FIXME: Send some characters.
	for (int i = 0; i < size; i++) {
		while (1) {
			if (uart->fifo_mmio->flags & UART_FIFO_FLAG_TX_FULL != UART_FIFO_FLAG_TX_FULL) {
				*uart->fifo_mmio->fifo_tx = buffer[i];	
				break;
			} else {
				uart->tx.wait();
			}
		}
	}

	return size;
}

// This function attempts to read up to 256 bytes, and returns the amount read.
size_t uart_rx(uart_t *uart, uint8_t *buffer, size_t size)
{
	// FIXME: Read some characters
	for (int i = 0; i < size; i++) {
		while (1) {
			if (uart->fifo_mmio->flags & UART_FIFO_FLAG_RX_NOT_EMPTY != UART_FIFO_FLAG_RX_NOT_EMPTY) {
				buffer[i] = *uart->fifo_mmio->fifo_rx;	
			} else {
				rx.wait();
			}
		}
	}

}
