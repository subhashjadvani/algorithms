/*
You are given a timer library API which can schedule the timer callback with max timeout upto the max value represented by 32-bit value. 
Using this API, provide another API which allows the user to schedule the timer callback with max timeout upto the max value represented by 64-bit value.
*/

#define MAX_32BIT_TIMEOUT 0xffffffff

// given API 
handle_t start_timer_32(uint32_t timeout, cb_t func, unsigned long data);

// 0x3_0000_0000 seconds

typedef struct 32bit_data {
	unsigned long data;
	cb_t client_cb;
	unsigned long client_data;
	unsigned long low;
} data32;

void cb_internal(void *data) {
	data32 *t = (data32 *)data;

	if (t->count == 0) {
		//if (t->low) {
		//	start_timer_32(t->low, cb_internal, (unsigned long)&temp);
		//	t->low = 0;
		//} else {
			t->client_cb(client_data);
			free(t);
			return;
		//}
	}

	t->count--;
	start_timer_32(MAX_32BIT_TIMEOUT, cb_internal, (unsigned long)t);
}

// val: 0x0000_0002_0000_0010
// big 
// little
handle_t start_timer_64(uint64_t timeout, cb_t func, unsigned long data)
{
	uint32_t count = timeout >> 32;
	uint32_t remain = timeout & (MAX_32BIT_TIMEOUT);
	
	struct data32 *d = malloc(sizeof (struct data32));
	memset(d, sizeof (struct data32), 0);

	d->count = count;
	d->client_cb = func;
	d->client_data = data;

	if (count > 0) {
		if (t->low) {
			start_timer_32(t->low, cb_internal, (unsigned long)&temp);
		//data32.low = remain;
		//start_timer_32(MAX_32BIT_TIMEOUT, cb_internal, (unsigned long)&data32);
	} else
		start_timer_32(remain, cb_internal, (unsigned long)&data32);
}

