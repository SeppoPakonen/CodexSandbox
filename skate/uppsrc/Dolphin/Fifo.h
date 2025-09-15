#ifndef _Dolphin_Fifo_h_
#define _Dolphin_Fifo_h_


template <class T>
struct FifoReg {
	T value;
	
	void operator=(T v) {value = v;}
	
};

struct FifoBase {
	FifoReg<char> s8;
	FifoReg<short> s16;
	FifoReg<int> s32;
	FifoReg<unsigned char> u8;
	FifoReg<unsigned short> u16;
	FifoReg<unsigned int> u32;
	FifoReg<float> f32;
	FifoReg<size_t> ptr;
	
};

extern FifoBase GXWGFifo;


#endif
