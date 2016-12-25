//
// Created by 孙桢波 on 2016/12/22.
//

#include "chip8.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

unsigned char chip8_fontset[80] =
        {
                0xF0, 0x90, 0x90, 0x90, 0xF0, //0
                0x20, 0x60, 0x20, 0x20, 0x70, //1
                0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
                0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
                0x90, 0x90, 0xF0, 0x10, 0x10, //4
                0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
                0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
                0xF0, 0x10, 0x20, 0x40, 0x40, //7
                0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
                0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
                0xF0, 0x90, 0xF0, 0x90, 0x90, //A
                0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
                0xF0, 0x80, 0x80, 0x80, 0xF0, //C
                0xE0, 0x90, 0x90, 0x90, 0xE0, //D
                0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
                0xF0, 0x80, 0xF0, 0x80, 0x80  //F
        };

chip8::chip8 ()
{
    initialize ();
}

chip8::~chip8 ()
{
    
}

void chip8::initialize ()
{
    opcode=0;
    I=0;
    pc=0x200;
    sp=0;
    for (int i=0;i<4096;i++)
        _mem[i]=0;
    for (int i=0;i<16;i++)
        _reg[i]=0;
    for (int i=0;i<16;i++)
        stack[i]=0;
    
    delay_timer=0;
    sound_timer=0;
    for (int i=0;i<16;i++)
        key[i]=0;
    for (int i=0;i<64*32;i++)
        gfx[i]=0;
    drawFlag=true;
   srand(unsigned(time(NULL))) ;
}

void chip8::getopcode ()
{
    opcode=_mem[pc]<<8 | _mem[pc+1];
}

void chip8::emulateCycle ()
{
    getopcode ();
    int i;
    
    switch(opcode & 0xf000)
    {
        case 0x0000:
            switch (opcode & 0x000f)
            {
                case  0x0000:
                    for (i=0;i<32*64;i++)
                        gfx[i]=0;
                    drawFlag=true;
                    pc=(unsigned short)(pc+2);
                    break;
                case 0x000E:
                    sp--;
                    pc=stack[sp];
                    pc=(unsigned short)(pc+2);
                    break;
                default:
                    std::cout<<"wrong opcode"<<std::endl;
            }
            break;
        case 0x1000:
            pc= (unsigned short)(opcode & 0x0fff);
            break;
        case 0x2000:
            stack[sp]=pc;
            sp++;
            pc=(unsigned short)(opcode & 0x0fff);
            break;
        case 0x3000:
            if (_reg[(opcode & 0x0f00)>>8]==(opcode & 0x00ff))
                pc=(unsigned short)(pc+4);
            else
                pc=(unsigned short)(pc+2);
            break;
        case 0x4000:
            if (_reg[(opcode & 0x0f00)>>8]!=(opcode & 0x00ff))
                pc=(unsigned short)(pc+4);
            else
                pc=(unsigned short)(pc+2);
            break;
        case 0x5000:
            if(_reg[(opcode & 0x0f00)>>8]==_reg[(opcode & 0x00f0)>>4])
                pc=(unsigned short)(pc+4);
            else
                pc=(unsigned short)(pc+2);
            break;
        case 0x6000:
            _reg[(opcode & 0x0f00)>>8]=(unsigned char)(opcode & 0x00ff);
            pc=(unsigned short)(pc+2);
            break;
        case 0x7000:
            _reg[(opcode & 0x0f00)>>8]=_reg[(opcode & 0x0f00)>>8]+(unsigned char)(opcode & 0x00ff);
            pc=(unsigned short)(pc+2);
            break;
        case 0x8000:
            switch (opcode & 0x000f)
            {
                case 0x0000:
                    _reg[(opcode & 0x0f00) >> 8] = _reg[(opcode & 0x00f0) >> 4];
                    pc=(unsigned short)(pc+2);
                    break;
                case 0x0001:
                    _reg[(opcode & 0x0f00) >> 8] = _reg[(opcode & 0x0f00) >> 8] | _reg[(opcode & 0x00f0) >> 4];
                    pc=(unsigned short)(pc+2);
                    break;
                case 0x0002:
                    _reg[(opcode & 0x0f00) >> 8] = _reg[(opcode & 0x0f00) >> 8] & _reg[(opcode & 0x00f0) >> 4];
                    pc=(unsigned short)(pc+2);
                    break;
                case 0x0003:
                    _reg[(opcode & 0x0f00) >> 8] = _reg[(opcode & 0x0f00) >> 8] ^ _reg[(opcode & 0x00f0) >> 4];
                    pc=(unsigned short)(pc+2);
                    break;
                case 0x0004:
                    if ( _reg[(opcode & 0x0f00) >> 8] > (0xff - _reg[(opcode & 0x00f0) >> 4]))
                        _reg[0xf] = 1;
                    else
                    {
                        _reg[(opcode & 0x0f00) >> 8] = _reg[(opcode & 0x0f00)>>8] + _reg[(opcode & 0x00f0)>>4];
                        _reg[0xf] = 0;
                    }
                    pc=(unsigned short)(pc+2);
                    break;
                case 0x0005:
                    if ( _reg[(opcode & 0x0f00) >> 8] < _reg[(opcode & 0x00f0) >> 4] )
                        _reg[0xf] = 1;
                    else
                    {
                        _reg[(opcode & 0x0f00) >> 8] = _reg[(opcode & 0x0f00) >> 8] - _reg[(opcode & 0x00f0) >> 4];
                        _reg[0xf] = 0;
                    }
                    pc=(unsigned short)(pc+2);
                    break;
                case 0x0006:
                    _reg[0xf] = _reg[(opcode & 0x0f00) >> 8] & (unsigned char) (0x01);
                    _reg[(opcode & 0x0f00) >> 8] = _reg[(opcode & 0x0f00) >> 8] >> 1;
                    pc=(unsigned short)(pc+2);
                    break;
                case 0x0007:
                    if ( _reg[(opcode & 0x0f00) >> 8] > _reg[(opcode & 0x00f0) >> 4] )
                        _reg[0xf] = 1;
                    else
                    {
                        _reg[0xf] = 0;
                        _reg[(opcode & 0x0f00) >> 8] = _reg[(opcode & 0x00f0) >> 4] - _reg[(opcode & 0x0f00) >> 8];
                    }
                    pc=(unsigned short)(pc+2);
                    break;
                case 0x000e:
                    _reg[0xf] = _reg[(opcode & 0x0f00) >> 8] >> 7;
                    _reg[(opcode & 0x0f00)>>8] = _reg[(opcode & 0x0f00) >> 8] << 1;
                    pc=(unsigned short)(pc+2);
                    break;
                default:
                    std::cout<<"wrong opcode"<<std::endl;
            }
            break;
        case 0x9000:
            if (_reg[(opcode & 0x0f00)>>8]!=_reg[(opcode & 0x00f0)>>4])
                pc=(unsigned short)(pc+4);
            else
                pc=(unsigned short)(pc+2);
            break;
        case 0xa000:
            I=(unsigned short)(opcode & 0x0fff);
            pc=(unsigned short)(pc+2);
            break;
        case 0xb000:
            pc=(unsigned char)(_reg[0x0]+(opcode & 0x0fff));
            break;
        case 0xc000:
            _reg[(opcode & 0x0f00)>>8]=(unsigned char) ((rand() & 0xff) & (opcode & 0x00ff));
            pc=(unsigned short)(pc+2);
            break;
        case 0xd000:
        {
            drawspirate ();
            pc = (unsigned short) (pc + 2);
        }
            break;
        case 0xe000:
            switch (opcode & 0x00ff)
            {
                case 0x009e:
                    if (key[_reg[(opcode & 0x0f00)>>8]]!=0)
                        pc=(unsigned short)(pc+4);
                    else
                        pc=(unsigned short)(pc+2);
                    break;
                case 0x00a1:
                    if (key[_reg[(opcode & 0x0f00)>>8]]==0)
                        pc=(unsigned short)(pc+4);
                    else
                        pc=(unsigned short)(pc+2);
                    break;
                default:
                std::cout<<"wrong opcode"<<std::endl;
            }
            break;
        case 0xf000:
            switch(opcode & 0x00ff)
            {
                case 0x0007:
                    _reg[(opcode & 0x0f00) >> 8] = delay_timer;
                    pc = (unsigned short) (pc + 2);
                    break;
                case 0x000A: // FX0A: 如果有按键信息，存入VX
                {
                    bool keyPress = false;
        
                    for( i = 0; i < 16; ++i)
                    {
                        if(key[i] != 0)
                        {
                            _reg[(opcode & 0x0F00) >> 8] =(unsigned char)i;
                            keyPress = true;//这里只存了最后一个按下的按键到VX
                        }
                    }
        
                    //如果没有按键按下，则返回
                    if(!keyPress)
                        return;
        
                    pc += 2;
                }
                    break;
                case 0x0015:
                    delay_timer = _reg[(opcode & 0x0f00) >> 8];
                    pc = (unsigned short) (pc + 2);
                    break;
                case 0x0018:
                    sound_timer = _reg[(opcode & 0x0f00) >> 8];
                    pc = (unsigned short) (pc + 2);
                    break;
                case 0x001e:
                    if(I + _reg[(opcode & 0x0F00) >> 8] > 0xFFF)
                        _reg[0xF] = 1;
                    else
                        _reg[0xF] = 0;
                    I += _reg[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x0029:
                    I = (unsigned short)(_reg[(opcode & 0x0F00) >> 8] * 0x5);
                    pc += 2;
                    break;
                    //对应精灵的地址
                case 0x0033:
                    _mem[I] = (unsigned char) (_reg[(opcode & 0x0f00) >> 8] / 100);
                    _mem[I + 1] = (unsigned char) ((_reg[(opcode & 0x0f00) >> 8] / 10) % 10);
                    _mem[I + 2] = (unsigned char) (_reg[(opcode & 0x0f00) >> 8] % 10);
                    pc = (unsigned short) (pc + 2);
                    break;
                case 0x0055:
                    for ( i = 0; i <= (opcode & 0x0f00) >> 8; i++ )
                        _mem[I + i] = _reg[i];
                    I = (unsigned short) (I + ((opcode & 0x0f00) >> 8 )+ 1);
                    pc = (unsigned short) (pc + 2);
                    break;
                case 0x0065:
                    for ( i = 0; i < ((opcode & 0x0f00) >> 8); i++ )
                        _reg[i] = _mem[I + i];
                    I = (unsigned short) (I + ((opcode & 0x0f00) >> 8) + 1);
                    pc = (unsigned short) (pc + 2);
                    break;
                default:
                    std::cout<<"wrong opcode"<<std::endl;
            }
            break;
        default:
           std::cout<<"wrong opcode"<<std::endl;
    }
    
    if(delay_timer > 0)
        -- delay_timer;
    
    //计时器--
    if(sound_timer > 0)
    {
        //if(sound_timer == 1) printf("beep\n");
        -- sound_timer;
    }
}

void chip8::drawspirate()
{
    unsigned short x=_reg[(opcode & 0x0f00)>>8];
    unsigned short y=_reg[(opcode & 0x00f0)>>4];
    unsigned short height=(unsigned short)(opcode & 0x000f);
    unsigned short pixel;
    _reg[0xf]=0;
    for (int yline=0;yline<height;yline++)
    {
        pixel = _mem[I + yline];
        for ( int xline = 0; xline < 8; xline++ )
        {
            if ((pixel & (unsigned char)(0x80 >> xline)) != 0 )
            {
                if ( gfx[x + xline + (y + yline) * 64] == 1 )
                    _reg[0xf] = 1;
                gfx[x + xline + (y + yline) * 64] = (unsigned char) (gfx[x + xline + (y + yline) * 64] ^ 1);
            }
        
        }
        drawFlag = true;
    
    }
}

bool chip8::loadApplication (const char *filename)
{
    initialize ();
    printf("loading:%s\n",filename);
    FILE * pfile=fopen(filename,"rb");
    if (pfile==NULL)
    {
        fputs("Memory Error",stderr);
        return false;
    }
    fseek(pfile,0,SEEK_END);
    long lsize=ftell(pfile);
    rewind(pfile);
    printf("filesize: %d\n",(int)lsize);
    
    char * buffer =(char *)malloc(sizeof(char) *lsize);
    if (buffer==NULL)
    {
        fputs("Memry Error",stderr);
        return false;
    }
    
    size_t result=fread(buffer,1,lsize,pfile);
    if (result!=lsize)
    {
        fputs("Reading Error",stderr);
        return false;
    }
    if ((4096-512)>lsize)
    {
        for (int i=0;i<lsize;i++)
            _mem[i+512]=(unsigned char)buffer[i];
    }
    else
        printf("Error:ROM too big for memory");
    
    fclose(pfile);
    free(buffer);
    
    return true;
    
}