/*
You are given a timer library API which can schedule the timer callback with max timeout upto the max value represented by 32-bit value. 
Using this API, provide another API which allows the user to schedule the timer callback with max timeout upto the max value represented by 64-bit value.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Given start_timer_32bit() API, implement start_timer_64bit() API 
typedef void (*cb_t) (void*);
void start_timer_32bit(uint32_t timeout, cb_t func, unsigned long data)
{
	// assume this is already implemented!
	
	/* some stub implementation */
	static int callback_count = 0;
	callback_count++;
	printf("%s: callback_count %d timeout 0x%x\n", __func__, callback_count, timeout);

	func((void*)data);
}

typedef struct timer64_ctx {
	cb_t client_cb;
	uint32_t upper_count;
	uint32_t lower_count;
	unsigned long data;
	unsigned long client_data;
} timer64_ctx_t;

#define MAX_32BIT_TIMEOUT ((1ULL << 32) - 1)

static void cb_internal(void *data) {
	timer64_ctx_t *ctx = (timer64_ctx_t *)data;

	ctx->upper_count--;
	if (ctx->upper_count == 0) {
		ctx->client_cb((void*)ctx->client_data);
		free(ctx);
	} else {
		start_timer_32bit(MAX_32BIT_TIMEOUT, cb_internal, (unsigned long)ctx);
	}
}

void start_timer_64bit(uint64_t timeout, cb_t func, unsigned long data)
{
	timer64_ctx_t *ctx = malloc(sizeof(timer64_ctx_t));
	memset(ctx, 0, sizeof(timer64_ctx_t));

	// example: timeout: 0x0000_0002_0000_0010
	ctx->upper_count = timeout >> 32;
	ctx->lower_count = timeout & MAX_32BIT_TIMEOUT;
	ctx->client_cb = func;
	ctx->client_data = data;

	if (ctx->lower_count) {
		start_timer_32bit(ctx->lower_count, cb_internal, (unsigned long)ctx);
	} else if (ctx->upper_count) {
		start_timer_32bit(MAX_32BIT_TIMEOUT, cb_internal, (unsigned long)ctx);
	}
}

// test code!
void my_64bit_callback(void *data)
{
	printf("%s\n", __func__);
}

int main()
{
	uint64_t timeout = 0x200001000;
	start_timer_64bit(timeout, my_64bit_callback, 0);
}
