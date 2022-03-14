#pragma once
#include <stdint.h>
#include <memory>

static uint8_t IntIoCharHex(uint8_t ch) {
	if (15 >= ch && ch >= 10) {
		ch = ch - 10 + 'A';
	}
	else if (ch >= 0 && ch < 10) {
		ch += '0';
	}
	else {
		printf("ch = %d, cant change to hex\n", ch);
	}
	return ch;
}

static void CharToHex(const uint8_t& data, uint8_t* buf) {
	buf[1] = (uint8_t)(data & 0x0F);
	buf[0] = (uint8_t)((data & 0xF0) >> 4);

	buf[0] = IntIoCharHex(buf[0]);
	buf[1] = IntIoCharHex(buf[1]);

}

#define print_bytes(bytes, len) do { \
    std::stringstream ss;               \
    ss << "byte: ";                     \
    uint8_t tmp[2] = { 0 };             \
    for (int i = 0; i < len; i++) {     \
        CharToHex(bytes[i], tmp);       \
        ss << tmp[0] << tmp[1] << " ";  \
    }                                   \
    __logger_debug << ss.str() << "\n"; \
} while(0)                              \
//
struct MFCHead {
	uint8_t addr;
	uint8_t stx;		// 0x02

	uint8_t rw;			// 0x80 = r, 0x81 = w
	uint8_t len;		// pkg_len - 6
	uint8_t cmd_class;
	uint8_t instance;
	uint8_t attribute;
	uint8_t datas[0];
};

static void dump_mfch(MFCHead* mfch) {
//	printf("ack             = 0x%x\n", mfch->ack);
	printf("addr            = 0x%x\n", mfch->addr);
	printf("stx             = 0x%x\n", mfch->stx);
	printf("rw              = 0x%x\n", mfch->rw);
	printf("len             = 0x%x\n", mfch->len);
	printf("cmd_class       = 0x%x\n", mfch->cmd_class);
	printf("instance        = 0x%x\n", mfch->instance);
	printf("attribute       = 0x%x\n", mfch->attribute);
	for (int i = 0; i < mfch->len - 1; i++) {
		printf("data[%d]         = 0x%x\n", i, mfch->datas[i]);
	}
}
struct MFCPeer {
	uint8_t fix;		// 0x00
	uint8_t crc8;
};

struct MFC_ACK {
	uint8_t ack;			// 06;
	uint8_t addr;			// 00
	uint8_t stx;			// 02
	uint8_t service_type;	// 80
	uint8_t len;			// 
	uint8_t cmd_class;
	uint8_t instance;
	uint8_t attribute;
	uint8_t datas[0];
};

static void dump_ack(MFC_ACK* ma) {
	printf("ack             = 0x%x\n", ma->ack);
	printf("addr            = 0x%x\n", ma->addr);
	printf("stx             = 0x%x\n", ma->stx);
	printf("service_type    = 0x%x\n", ma->service_type);
	printf("len             = 0x%x\n", ma->len);
	printf("cmd_class       = 0x%x\n", ma->cmd_class);
	printf("instance        = 0x%x\n", ma->instance);
	printf("attribute       = 0x%x\n", ma->attribute);
	for (int i = 0; i < ma->len - 1; i++) {
		printf("data[%d]         = 0x%x\n", i, ma->datas[i]);
	}
}
class HandleMFCFrame {
public:
	using ptr = std::shared_ptr<HandleMFCFrame>;
	void framing();
	void decode();
private:

}; // class HandleFrame
// 100 ms ÄÚÓ¦´ð