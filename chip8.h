//
// Created by 孙桢波 on 2016/12/22.
//

#ifndef CHIP_8_CHIP8_H
#define CHIP_8_CHIP8_H


#include <i386/types.h>

class chip8
{
public:
    chip8();
    ~chip8();
    
    bool drawFlag; //是否雪瑶绘画的标识位
    
    bool loadApplication(const char * filename);
    
    void initialize();
    void emulateCycle(); //模拟周期
    
    unsigned char gfx[64*32];
    unsigned char  key[16];//16个按键输入
    
private:
    unsigned short opcode;   //当前opcode
   
    unsigned short I;    //当前寄存索引器
    unsigned short pc;   //程序计数器

    unsigned short sp;    //当前栈指针

    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned char _mem[4096];    //内存
    unsigned char _reg[16];  //寄存器

    
    void getopcode();
    void drawspirate();
};


#endif //CHIP_8_CHIP8_H
