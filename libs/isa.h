#ifndef SISA_GIT_HASH
#define SISA_GIT_HASH "<git hash omitted>"
#endif

#ifndef SISA_VERSION
#define SISA_VERSION "1.8_official commit=" SISA_GIT_HASH
#endif

#define k case


#define SET_PCR(val) (program_counter_region = val)
#define GET_PCR() (program_counter_region)
#define GET_LOCAL_ADDR(val) ( (((UU)program_counter_region)<<16) | val  )
#define SET_PC(val) (program_counter = val)
#define ADD_PC(val) (program_counter += val)
#define INCR_PC() (program_counter++)
#define GET_PC() (program_counter)
#define GET_EFF_PC() ( (((UU)program_counter_region)<<16) | program_counter  )
#define GET_EFF_C()  ( (((UU)program_counter_region)<<16) | c  )
#define GET_EFF_A()  ( (((UU)program_counter_region)<<16) | a  )
#define GET_EFF_B()  ( (((UU)program_counter_region)<<16) | b  )
#define GET_EFF_C_PLUS(val) ( 0xffFFff & (GET_EFF_C()+val))
#define GET_EFF_A_PLUS(val) ( 0xffFFff & (GET_EFF_A()+val))
#define GET_EFF_B_PLUS(val) ( 0xffFFff & (GET_EFF_B()+val))
#define GET_EFF_PC_MINUS(val) ( (((UU)program_counter_region)<<16) | ((program_counter-val)&0xffFF))
#define GET_EFF_PC_AND_INCR() ( (((UU)program_counter_region)<<16) | program_counter++)
#define CONSUME_BYTE M[GET_EFF_PC_AND_INCR()]
#define CONSUME_TWO_BYTES (ADD_PC(2),\
						((((U)M[GET_EFF_PC_MINUS(2)]))<<8) |\
						(U)M[GET_EFF_PC_MINUS(1)])
#define CONSUME_FOUR_BYTES (ADD_PC(4),\
						((((UU)M[GET_EFF_PC_MINUS(4)]))<<24) |\
						((((UU)M[GET_EFF_PC_MINUS(3)]))<<16) |\
						((((UU)M[GET_EFF_PC_MINUS(2)]))<<8) |\
						(UU)M[GET_EFF_PC_MINUS(1)])
#define CONSUME_THREE_BYTES (ADD_PC(3),\
						((((UU)M[GET_EFF_PC_MINUS(3)]))<<16) |\
						((((UU)M[GET_EFF_PC_MINUS(2)]))<<8) |\
						(UU)M[GET_EFF_PC_MINUS(1)])
#define Z_READ_TWO_BYTES_THROUGH_C ((((U)M[GET_EFF_C()])<<8) | (U)M[GET_EFF_C_PLUS(1)])
#define Z_READ_TWO_BYTES_THROUGH_A ((((U)M[GET_EFF_A()])<<8) | (U)M[GET_EFF_A_PLUS(1)])
#define Z_READ_TWO_BYTES_THROUGH_B ((((U)M[GET_EFF_B()])<<8) | (U)M[GET_EFF_B_PLUS(1)])

/*
	Note the use of the comma operator.
*/
#define Z_READBYTE(memloc) ((UU)M[(memloc) & 0xffFFff])
#define Z_READ2BYTES(memloc) (Z_READBYTE(memloc)<<8) | (Z_READBYTE(memloc+1))
#define Z_READ4BYTES(memloc) (Z_READBYTE(memloc)<<24) | (Z_READBYTE(memloc+1)<<16) | (Z_READBYTE(memloc+2)<<8) | (Z_READBYTE(memloc+3))
#define Z_FAR_MEMORY_READ_C_HIGH8_B_LOW16 Z_READ2BYTES((((UU)c)<<16)|((UU)b))
#define Z_FAR_MEMORY_READ_C_HIGH8_A_LOW16 Z_READ2BYTES((((UU)c)<<16)|((UU)a))
#define Z_FAR_MEMORY_READ_C_HIGH8_A_LOW16_4 Z_READ4BYTES((((UU)c)<<16)|((UU)a))
#define write_byte(v,d)		M[(d) & 0xffFFff]=v;
#define write_2bytes(v,d)	{UU TEMPORARY_VARIABLE = (d) & 0xffFFff;  U vuv = v; M[TEMPORARY_VARIABLE]=					(vuv)>>8;\
													M[(TEMPORARY_VARIABLE+1)&0xFFffFF]=	vuv;}
#define write_4bytes(v,d)	{UU TEMPORARY_VARIABLE = (d) & 0xffFFff; UU vuv = v; M[(TEMPORARY_VARIABLE)&0xFFffFF]=		(vuv)>>24;\
													M[(TEMPORARY_VARIABLE+1)&0xFFffFF]=	(vuv)>>16;\
													M[(TEMPORARY_VARIABLE+2)&0xFFffFF]=	(vuv)>>8;\
													M[(TEMPORARY_VARIABLE+3)&0xFFffFF]=	(vuv);}

#define STASH_REG(XX)   UU XX##_stash = XX;
#define UNSTASH_REG(XX) XX = XX##_stash;
#define STASH_REGS STASH_REG(a);STASH_REG(b);STASH_REG(c);STASH_REG(stack_pointer);STASH_REG(program_counter);STASH_REG(program_counter_region);\
		STASH_REG(RX0);STASH_REG(RX1);STASH_REG(RX2);STASH_REG(RX3);u* M_STASH = M;STASH_REG(instruction_counter);STASH_REG(EMULATE_DEPTH);
#define UNSTASH_REGS UNSTASH_REG(a);UNSTASH_REG(b);UNSTASH_REG(c);UNSTASH_REG(stack_pointer);UNSTASH_REG(program_counter);UNSTASH_REG(program_counter_region);\
		UNSTASH_REG(RX0);UNSTASH_REG(RX1);UNSTASH_REG(RX2);UNSTASH_REG(RX3);M = M_STASH;UNSTASH_REG(instruction_counter);UNSTASH_REG(EMULATE_DEPTH);

#ifdef SISA_DEBUGGER
void debugger_hook(	unsigned short *a,
					unsigned short *b,
					unsigned short *c,
					unsigned short *stack_pointer,
					unsigned short *program_counter,
					unsigned char *program_counter_region,
					UU *RX0,
					UU *RX1,
					UU *RX2,
					UU *RX3,
					u *EMULATE_DEPTH,
					u *M
);
#else
#define debugger_hook(FBRUH1,FBRUH2,FBRUH3,FBRUH4,FBRUH5,FBRUH6,FBRUH7,FBRUH8,FBRUH9,FBRUH10,FBRUH11,FBRUH12) /*a comment*/
#endif

#ifdef USE_COMPUTED_GOTO
#define D ;PREEMPT();debugger_hook(&a,&b,&c,&stack_pointer,&program_counter,&program_counter_region,&RX0,&RX1,&RX2,&RX3,&EMULATE_DEPTH,M);goto *goto_table[CONSUME_BYTE];
#else
#define D ;PREEMPT();debugger_hook(&a,&b,&c,&stack_pointer,&program_counter,&program_counter_region,&RX0,&RX1,&RX2,&RX3,&EMULATE_DEPTH,M);switch(CONSUME_BYTE){\
k 0:goto G_HALT;k 1:goto G_LDA;k 2:goto G_LA;k 3:goto G_LDB;k 4:goto G_LB;k 5:goto G_SC;k 6:goto G_STA;k 7:goto G_STB;\
k 8:goto G_ADD;k 9:goto G_SUB;k 10:goto G_MUL;k 11:goto G_DIV;k 12:goto G_MOD;k 13:goto G_CMP;k 14:goto G_JMPIFEQ;k 15:goto G_JMPIFNEQ;\
k 16:goto G_GETCHAR;k 17:goto G_PUTCHAR;k 18:goto G_AND;k 19:goto G_OR;k 20:goto G_XOR;k 21:goto G_LSHIFT;k 22:goto G_RSHIFT;k 23:goto G_ILDA;\
k 24:goto G_ILDB;k 25:goto G_CAB;k 26:goto G_AB;k 27:goto G_BA;k 28:goto G_ALC;k 29:goto G_AHC;k 30:goto G_NOP;k 31:goto G_CBA;\
k 32:goto G_LLA;k 33:goto G_ILLDA;k 34:goto G_LLB;k 35:goto G_ILLDB;k 36:goto G_ILLDAA;k 37:goto G_CPCR;k 38:goto G_ILLDAB;k 39:goto G_ILLDBA;\
k 40:goto G_CA;k 41:goto G_CB;k 42:goto G_AC;k 43:goto G_BC;k 44:goto G_ISTA;k 45:goto G_ISTB;k 46:goto G_ISTLA;k 47:goto G_ISTLB;\
k 48:goto G_JMP;k 49:goto G_STLA;k 50:goto G_STLB;k 51:goto G_STC;k 52:goto G_PUSH;k 53:goto G_POP;k 54:goto G_PUSHA;k 55:goto G_POPA;\
k 56:goto G_ASTP;k 57:goto G_BSTP;k 58:goto G_COMPL;k 59:goto G_CPC;k 60:goto G_CALL;k 61:goto G_RET;k 62:goto G_FARILLDA;k 63:goto G_FARISTLA;\
k 64:goto G_FARILLDB;k 65:goto G_FARISTLB;k 66:goto G_FARPAGEL;k 67:goto G_FARPAGEST;k 68:goto G_LFARPC;k 69:goto G_FARCALL;k 70:goto G_FARRET;k 71:goto G_FARILDA;\
k 72:goto G_FARISTA;k 73:goto G_FARILDB;k 74:goto G_FARISTB;\
k 75:goto TB;k 76:goto TC;k 77:goto TD;k 78:goto TE;k 79:goto TF;\
k 80:goto U0;k 81:goto U1;k 82:goto U2;k 83:goto U3;k 84:goto U4;k 85:goto U5;k 86:goto U6;k 87:goto U7;\
k 88:goto G_TASK_SET;k 89:goto U9;k 90:goto UA;\
k 91:goto G_ALPUSH;k 92:goto G_BLPUSH;k 93:goto G_CPUSH;k 94:goto G_APUSH;k 95:goto G_BPUSH;\
k 96:goto G_ALPOP;k 97:goto G_BLPOP;k 98:goto G_CPOP;k 99:goto G_APOP;k 100:goto G_BPOP;\
k 101:goto G_INTERRUPT;k 102:goto G_CLOCK;\
k 103:goto G_ARX0;\
k 104:goto G_BRX0;k 105:goto V9;k 106:goto VA;k 107:goto VB;k 108:goto VC;k 109:goto VD;k 110:goto VE;k 111:goto VF;\
k 112:goto W0;k 113:goto W1;k 114:goto W2;k 115:goto W3;k 116:goto W4;k 117:goto W5;k 118:goto W6;k 119:goto W7;\
k 120:goto W8;k 121:goto W9;k 122:goto WA;k 123:goto WB;k 124:goto WC;k 125:goto WD;k 126:goto WE;\
k 127:goto WF;\
k 128:goto X0;k 129:goto X1;k 130:goto X2;k 131:goto X3;k 132:goto X4;\
k 133:goto X5;k 134:goto X6;k 135:goto X7;k 136:goto X8;k 137:goto X9;\
k 138:goto XA;k 139:goto XB;k 140:goto XC;k 141:goto XD;k 142:goto XE;k 143:goto XF;\
k 144:goto Y0;k 145:goto Y1;k 146:goto Y2;k 147:goto Y3;\
k 148:goto Y4;k 149:goto Y5;k 150:goto Y6;k 151:goto Y7;k 152:goto Y8;k 153:goto Y9;\
k 154:goto YA;k 155:goto YB;k 156:goto YC;k 157:goto YD;\
k 158:goto YE;k 159:goto YF;\
k 160:goto Z0;k 161:goto Z1;k 162:goto Z2;k 163:goto Z3;\
k 164:goto Z4;k 165:goto Z5;k 166:goto Z6;k 167:goto Z7;\
k 168:goto Z8;k 169:goto Z9;k 170:goto ZA;\
k 171:goto ZB;k 172:goto ZC;k 173:goto ZD;\
k 174:goto ZE;k 175:goto ZF;\
k 176:goto G_AA0;k 177:goto G_AA1;\
k 178:goto G_FLTCMP;k 179:goto G_AA3;\
k 180:goto G_AA4;k 181:goto G_AA5;k 182:goto G_AA6;\
k 183:goto G_AA7;k 184:goto G_AA8;k 185:goto G_AA9;\
k 186:goto G_AA10;k 187:goto G_AA11;\
k 188:goto G_AA12;k 189:goto G_AA13;k 190:goto G_AA14;\
k 191:goto G_AA15;k 192:goto G_AA16;k 193:goto G_AA17;\
k 194:goto G_AA18;k 195:goto G_AA19;k 196:goto G_AA20;k 197:goto G_AA21;\
k 198:goto G_AA22;k 199:goto G_AA23;k 200:goto G_AA24;k 201:goto G_AA25;\
k 202:goto G_AA26;k 203:goto G_AINCR;k 204:goto G_ADECR;k 205:goto G_RX0INCR;\
k 206:goto G_RX0DECR;k 207:goto G_EMULATE;\
k 208:goto G_ITOF;k 209:goto G_FTOI;\
k 210:goto G_SEG_GETCONFIG;k 211:goto G_RXICMP;k 212:goto G_LOGOR;k 213:goto G_LOGAND;\
k 214:goto G_BOOLIFY;k 215:goto G_NOTA;k 216:goto G_USER_FARISTA;k 217:goto G_TASK_RIC;\
k 218:goto G_USER_FARPAGEL;k 219:goto G_USER_FARPAGEST;\
k 220:goto G_LLDA;k 221:goto G_LLDB;\
k 222:goto G_LDRX0;k 223:goto G_LDRX1;k 224:goto G_LDRX2;k 225:goto G_LDRX3;k 226:goto G_LDC;\
k 227:goto G_STRX0;k 228:goto G_STRX1;k 229:goto G_STRX2;k 230:goto G_STRX3;\
k 231:k 232:k 233:k 234:k 235:k 236:k 237:k 238:k 239:k 240:k 241:k 242:k 243:k 244:k 245:k 246:k 247:\
k 248:k 249:k 250:k 251:k 252:k 253:k 254:k 255:default:goto G_HALT;}
#endif

int DONT_WANT_TO_INLINE_THIS e()
{
	
#ifdef SISA_DEBUGGER
	u program_counter_region=0;
	U a=0,b=0,c=0,program_counter=0,stack_pointer=0;
	UU RX0=0,RX1=0,RX2=0,RX3=0;
	u EMULATE_DEPTH=0;
	u current_task=1;
	register u *M=M_SAVER[0];
#else
	register u program_counter_region=0;
	register U a=0,
				b=0,
				c=0,
				program_counter=0,
				stack_pointer=0;
	register UU RX0=0,
				RX1=0,
				RX2=0,
				RX3=0;
	register u EMULATE_DEPTH=0;
	register u *M=M_SAVER[0];
	u current_task=1;
#endif
	sisa_regfile REG_SAVER[1 + SISA_MAX_TASKS] = {0};
	UU seg_access_mask = (SEGMENT_PAGES-1);
	UU seg_access_offset = 0;
#ifndef PREEMPT_TIMER
#define PREEMPT_TIMER 0x100000
#endif
#define EXTREME_HIGH_INSN_COST 700
#define HIGH_INSN_COST 50
#define MED_INSN_COST 10

#ifndef NO_PREEMPT

#ifndef PREEMPT
register UU instruction_counter = 0;
#define PREEMPT() if(EMULATE_DEPTH){\
	instruction_counter++;if(instruction_counter > PREEMPT_TIMER) {R=0xFF;goto G_HALT;}\
}
#endif

#else
#define PREEMPT() /*a comment*/
#endif


#ifdef USE_COMPUTED_GOTO
const void* const goto_table[256] = {
&&G_HALT,&&G_LDA,&&G_LA,&&G_LDB,&&G_LB,&&G_SC,&&G_STA,&&G_STB,
&&G_ADD,&&G_SUB,&&G_MUL,&&G_DIV,&&G_MOD,&&G_CMP,&&G_JMPIFEQ,&&G_JMPIFNEQ,
&&G_GETCHAR,&&G_PUTCHAR,&&G_AND,&&G_OR,&&G_XOR,
&&G_LSHIFT,&&G_RSHIFT,&&G_ILDA,&&G_ILDB,
&&G_CAB,&&G_AB,&&G_BA,
&&G_ALC,&&G_AHC,
&&G_NOP,&&G_CBA,&&G_LLA,&&G_ILLDA,&&G_LLB,
&&G_ILLDB,
&&G_ILLDAA,&&G_CPCR,&&G_ILLDAB,&&G_ILLDBA,
&&G_CA,&&G_CB,&&G_AC,&&G_BC,
&&G_ISTA,&&G_ISTB,&&G_ISTLA,&&G_ISTLB,&&G_JMP,&&G_STLA,&&G_STLB,
&&G_STC,&&G_PUSH,&&G_POP,&&G_PUSHA,&&G_POPA,&&G_ASTP,
&&G_BSTP,&&G_COMPL,&&G_CPC,&&G_CALL,&&G_RET,&&G_FARILLDA,
&&G_FARISTLA,&&G_FARILLDB,&&G_FARISTLB,
&&G_FARPAGEL,&&G_FARPAGEST,
&&G_LFARPC,&&G_FARCALL,&&G_FARRET,
&&G_FARILDA,&&G_FARISTA,
&&G_FARILDB,&&G_FARISTB,
/*FREE SLOTS!*/
&&TB,&&TC,&&TD,&&TE,
&&TF,&&U0,&&U1,&&U2,
&&U3,&&U4,&&U5,&&U6,
&&U7,&&G_TASK_SET,&&U9,&&UA,

&&G_ALPUSH,&&G_BLPUSH,
&&G_CPUSH,&&G_APUSH,&&G_BPUSH,&&G_ALPOP,
&&G_BLPOP,&&G_CPOP,&&G_APOP,
&&G_BPOP, /*100 dec, 0x64*/
&&G_INTERRUPT,&&G_CLOCK,
/*32 bit extensions*/
&&G_ARX0,&&G_BRX0,
&&V9,&&VA,&&VB,&&VC,
&&VD,&&VE,&&VF,&&W0,
&&W1,&&W2,&&W3,&&W4,
&&W5,&&W6,&&W7,&&W8,
&&W9,&&WA,&&WB,&&WC,
&&WD,&&WE,&&WF,&&X0,
&&X1,&&X2,&&X3,&&X4,
&&X5,&&X6,&&X7,&&X8,
&&X9,&&XA,&&XB,&&XC,
&&XD,&&XE,&&XF,&&Y0,
&&Y1,&&Y2,&&Y3,&&Y4,
&&Y5,&&Y6,&&Y7,&&Y8,
&&Y9,&&YA,&&YB,&&YC,
&&YD,&&YE,&&YF,&&Z0,
&&Z1,&&Z2,&&Z3,&&Z4,
&&Z5,&&Z6,&&Z7,&&Z8,
&&Z9,&&ZA,&&ZB,&&ZC,
&&ZD,&&ZE,&&ZF,
&&G_AA0,
&&G_AA1,
&&G_FLTCMP,
&&G_AA3,
&&G_AA4,
&&G_AA5,
&&G_AA6,
&&G_AA7,
&&G_AA8,
&&G_AA9,
&&G_AA10,
&&G_AA11,
&&G_AA12,
&&G_AA13,
&&G_AA14,
&&G_AA15,
&&G_AA16,
&&G_AA17,
&&G_AA18,
&&G_AA19,
&&G_AA20,
&&G_AA21,
&&G_AA22,
&&G_AA23,
&&G_AA24,
&&G_AA25,
&&G_AA26,
&&G_AINCR,
&&G_ADECR,
&&G_RX0INCR,
&&G_RX0DECR,
&&G_EMULATE,
&&G_ITOF,
&&G_FTOI,
&&G_SEG_GETCONFIG,
&&G_RXICMP,
&&G_LOGOR,&&G_LOGAND,
&&G_BOOLIFY,&&G_NOTA,&&G_USER_FARISTA,&&G_TASK_RIC,
&&G_USER_FARPAGEL,&&G_USER_FARPAGEST,
&&G_LLDA,&&G_LLDB,
&&G_LDRX0,
&&G_LDRX1,
&&G_LDRX2,
&&G_LDRX3,
&&G_LDC,
&&G_STRX0,&&G_STRX1,
&&G_STRX2,&&G_STRX3,
&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,
&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,&&G_HALT,
&&G_HALT,
&&G_HALT,
&&G_HALT,
&&G_HALT,
&&G_HALT,
&&G_HALT,

&&G_HALT
};
#endif

R=0;
di();
#ifdef SISA_DEBUGGER
debugger_hook(&a,&b,&c,&stack_pointer,&program_counter,&program_counter_region,&RX0,&RX1,&RX2,&RX3,&EMULATE_DEPTH,M);
#endif



/*Free slots!*/

G_NOP:D
G_AND:a&=b;D
G_OR:a|=b;D
G_XOR:a^=b;D
G_GETCHAR:{
	STASH_REGS;
#ifndef NO_DEVICE_PRIVILEGE
	if(EMULATE_DEPTH){R = 16; goto G_HALT;}
#endif
	a_stash=gch();
	UNSTASH_REGS;
}D
G_PUTCHAR:{
	STASH_REGS;
#ifndef NO_DEVICE_PRIVILEGE
	if(EMULATE_DEPTH){R = 17; goto G_HALT;}
#endif
	pch(a_stash);
	UNSTASH_REGS;
}D
G_LSHIFT:a<<=b;D
G_RSHIFT:a>>=b;D
G_ILDA:a=M[GET_EFF_C()]D
G_ILDB:b=M[GET_EFF_C()]D
G_CAB:c=(a<<8)|(b&255)D
G_AB:a=b;D
G_BA:b=a;D
G_ALC:a=c&0xff;D
G_AHC:a=(c>>8);D
G_CBA:c=(b<<8)|(a&255)D
G_LLA:a=CONSUME_TWO_BYTES;D
G_ILLDA:a=Z_READ_TWO_BYTES_THROUGH_C;D
G_LLB:b=CONSUME_TWO_BYTES;D
G_ILLDB:b=Z_READ_TWO_BYTES_THROUGH_C;D
G_ILLDAA:a=Z_READ_TWO_BYTES_THROUGH_A;D
G_CPCR:c=GET_PCR();D
G_ILLDAB:a=Z_READ_TWO_BYTES_THROUGH_B;D
G_ILLDBA:b=Z_READ_TWO_BYTES_THROUGH_A;D
G_CA:c=a;D
G_CB:c=b;D
G_AC:a=c;D
G_BC:b=c;D
G_ISTA:write_byte(a,GET_EFF_C())D
G_ISTB:write_byte(b,GET_EFF_C())D
G_ISTLA:write_2bytes(a,GET_EFF_C())D
G_ISTLB:write_2bytes(b,GET_EFF_C())D
G_JMP:program_counter=c;D
G_STLA:{
	UU temp = (CONSUME_TWO_BYTES);
	temp = GET_LOCAL_ADDR(temp);
	write_2bytes(a,temp);
}D
G_STLB:{
	UU temp = (CONSUME_TWO_BYTES);
	temp = GET_LOCAL_ADDR(temp);
	write_2bytes(b,temp);
}D
G_STC:{
	UU temp = (CONSUME_TWO_BYTES);
	temp = GET_LOCAL_ADDR(temp);
	write_2bytes(c,temp);
}D
G_PUSH:stack_pointer+=CONSUME_TWO_BYTES;D
G_POP:stack_pointer-=CONSUME_TWO_BYTES;D
G_PUSHA:stack_pointer+=a;D
G_POPA:stack_pointer-=a;D
G_ASTP:a=stack_pointer;D
G_BSTP:b=stack_pointer;D
G_COMPL:a=~a;D
G_CPC:c=GET_PC();D
G_LDA:{
	UU tmp = CONSUME_TWO_BYTES;
	tmp = GET_LOCAL_ADDR(tmp);
	a=M[ tmp ];
}D
G_LA:a=CONSUME_BYTE;D
G_LDB:{
	UU tmp = CONSUME_TWO_BYTES;
	tmp = GET_LOCAL_ADDR(tmp);
	b=M[ tmp ];
}
	D
G_LB:b=CONSUME_BYTE;D
G_SC:c=CONSUME_TWO_BYTES;D
G_STA:{
	UU tmp = CONSUME_TWO_BYTES;
	tmp = GET_LOCAL_ADDR(tmp);
	write_byte(a,tmp);
}D
G_STB:{
	UU tmp = CONSUME_TWO_BYTES;
	tmp = GET_LOCAL_ADDR(tmp);
	write_byte(b,tmp);
}D
G_JMPIFEQ:if(a==1){SET_PC(c);}D
G_JMPIFNEQ:if(a!=1){SET_PC(c);}D
G_ADD:a+=b; D
G_SUB:a-=b; D
G_MUL:a*=b; D
G_DIV:{if(b!=0)a/=b;else{R=1;goto G_HALT;}}D
G_MOD:{if(b!=0)a%=b;else{R=2;goto G_HALT;}}D
G_CMP:
if(a<b)a=0;
else if(a>b) a=2;
else a=1;
D
G_FARILLDA:a=Z_FAR_MEMORY_READ_C_HIGH8_B_LOW16;D
G_FARISTLA:write_2bytes(a,  ((((UU)c)<<16)|((UU)b))  )D
G_FARILLDB:b=Z_FAR_MEMORY_READ_C_HIGH8_A_LOW16;D
G_FARISTLB:write_2bytes(b,  ((((UU)c)<<16)|((UU)a))  )D
G_FARPAGEL:
{
	STASH_REGS;
	memmove(M+(((UU)a_stash)<<8),M+(((UU)c_stash)<<8),256);
	UNSTASH_REGS;
#ifndef NO_PREEMPT
	if(EMULATE_DEPTH) instruction_counter += HIGH_INSN_COST; /*This is a very expensive instruction.*/
#endif
}
D
G_FARPAGEST:{
	STASH_REGS;
	memmove(M+(((UU)c_stash)<<8),M+(((UU)a_stash)<<8),256);
	UNSTASH_REGS;
#ifndef NO_PREEMPT
	if(EMULATE_DEPTH) instruction_counter += HIGH_INSN_COST; /*This is a very expensive instruction.*/
#endif
}D
G_LFARPC:
SET_PCR(a);
SET_PC(0);
D
G_CALL:
M[stack_pointer] = GET_PC()>>8; stack_pointer++;
M[stack_pointer] = GET_PC(); stack_pointer++;
SET_PC(c);D
G_RET:
{
	U tmp;
	stack_pointer--;tmp =M[stack_pointer];
	stack_pointer--;tmp|=(((UU)M[stack_pointer])<<8);
	SET_PC(tmp);
}
D
G_FARCALL:
	M[stack_pointer] = GET_PC()>>8; stack_pointer++;
	M[stack_pointer] = GET_PC(); stack_pointer++;
	M[stack_pointer] = GET_PCR(); stack_pointer++;
	SET_PCR(a);
	SET_PC(c);
D
G_FARRET:
{
	U tmp;
	stack_pointer--;SET_PCR(M[stack_pointer]);
	stack_pointer--;tmp =M[stack_pointer];
	stack_pointer--;tmp|=(((UU)M[stack_pointer])<<8);
	SET_PC(tmp);
}
D
G_FARILDA:a=M[ (((UU)c&255)<<16) |  ((UU)b)]D
G_FARISTA:write_byte(a,((((UU)c)<<16)|((UU)b)))D
G_FARILDB:b=M[(((UU)c&255)<<16)|((UU)a)]D
G_FARISTB:write_byte(b,((((UU)c)<<16)|((UU)a)))D
TB: /**/
{
		if(EMULATE_DEPTH > 0) {R=15; goto G_HALT;}
		SAVE_REGISTER(a, 0);
		SAVE_REGISTER(b, 0);
		SAVE_REGISTER(c, 0);
		SAVE_REGISTER(program_counter, 0);
		SAVE_REGISTER(stack_pointer, 0);
		SAVE_REGISTER(program_counter_region, 0);
		SAVE_REGISTER(RX0, 0);
		SAVE_REGISTER(RX1, 0);
		SAVE_REGISTER(RX2, 0);
		SAVE_REGISTER(RX3, 0);
		EMULATE_DEPTH = 1;M=M_SAVER[current_task];
		/*Load on up again! We're continuing where we left off!*/
		LOAD_REGISTER(a, current_task);
		LOAD_REGISTER(b, current_task);
		LOAD_REGISTER(c, current_task);
		LOAD_REGISTER(program_counter, current_task);
		LOAD_REGISTER(program_counter_region, current_task);
		LOAD_REGISTER(stack_pointer, current_task);
		LOAD_REGISTER(RX0, current_task);
		LOAD_REGISTER(RX1, current_task);
		LOAD_REGISTER(RX2, current_task);
		LOAD_REGISTER(RX3, current_task);
#ifndef NO_PREEMPT
		LOAD_REGISTER(instruction_counter, current_task);
#endif
}D

TC:if(EMULATE_DEPTH){R=15; goto G_HALT;}a=REG_SAVER[current_task].a;D
TD:if(EMULATE_DEPTH){R=15; goto G_HALT;}a=REG_SAVER[current_task].b;D
TE:if(EMULATE_DEPTH){R=15; goto G_HALT;}a=REG_SAVER[current_task].c;D
TF:if(EMULATE_DEPTH){R=15; goto G_HALT;}RX0=REG_SAVER[current_task].RX0;D
U0:if(EMULATE_DEPTH){R=15; goto G_HALT;}RX0=REG_SAVER[current_task].RX1;D
U1:if(EMULATE_DEPTH){R=15; goto G_HALT;}RX0=REG_SAVER[current_task].RX2;D
U2:if(EMULATE_DEPTH){R=15; goto G_HALT;}RX0=REG_SAVER[current_task].RX3;D
U3:if(EMULATE_DEPTH){R=15; goto G_HALT;}a=REG_SAVER[current_task].stack_pointer;D
U4:if(EMULATE_DEPTH){R=15; goto G_HALT;}a=REG_SAVER[current_task].program_counter;D
U5:if(EMULATE_DEPTH){R=15; goto G_HALT;}a=REG_SAVER[current_task].program_counter_region;D
U6:if(EMULATE_DEPTH){R=15; goto G_HALT;}a=M_SAVER[current_task][ (((UU)c&255)<<16) | (UU)b]D
U7:if(EMULATE_DEPTH){R=15; goto G_HALT;}REG_SAVER[current_task].a=a;D
G_TASK_SET: /*task_set*/
	if(EMULATE_DEPTH){R=15; goto G_HALT;}
	current_task = a%SISA_MAX_TASKS + 1;
D
U9:
	if(EMULATE_DEPTH){R=15; goto G_HALT;}
	REG_SAVER[current_task].program_counter_region = 0;
	REG_SAVER[current_task].program_counter = 0;
	REG_SAVER[current_task].stack_pointer = 0;
#ifndef NO_PREEMPT
	REG_SAVER[current_task].instruction_counter = 0; /*So that if we drop back in, the IC doesnt immediately kick in.*/
#endif
D
UA:R=19;goto G_HALT;
G_ALPUSH:	
	M[stack_pointer] = a>>8;	stack_pointer++;
	M[stack_pointer] = a;		stack_pointer++;
D
G_BLPUSH:	
	M[stack_pointer] = b>>8;	stack_pointer++;
	M[stack_pointer] = b;		stack_pointer++;
D
G_CPUSH:	
	M[stack_pointer] = c>>8;	stack_pointer++;
	M[stack_pointer] = c;		stack_pointer++;
D
G_APUSH:	
	M[stack_pointer] = a;		stack_pointer++;
D
G_BPUSH:
	M[stack_pointer] = b;		stack_pointer++;
D
G_ALPOP:
	stack_pointer--;
	a=M[stack_pointer];
	stack_pointer--;
	a|= (((UU)M[stack_pointer])<<8);
D
G_BLPOP:
	stack_pointer--;
	b=M[stack_pointer];
	stack_pointer--;
	b|= (((UU)M[stack_pointer])<<8);
D
G_CPOP:
	stack_pointer--;
	c=   M[stack_pointer];
	stack_pointer--;
	c|= (((UU)M[stack_pointer])<<8);
D
G_APOP:
	stack_pointer-=1;a=M[stack_pointer];
D
G_BPOP:
	stack_pointer-=1;b=M[stack_pointer];
D

G_INTERRUPT:
{
	STASH_REGS;
#ifndef NO_DEVICE_PRIVILEGE
	if(EMULATE_DEPTH){R = 18; goto G_HALT;}
#endif
	a_stash=interrupt(
		a_stash,
		b_stash,
		c_stash,
		stack_pointer_stash,
		program_counter_stash,
		program_counter_region_stash,
		RX0_stash,
		RX1_stash,
		RX2_stash,
		RX3_stash,
		M_STASH
	);
	UNSTASH_REGS;
}
D
G_CLOCK:{
	size_t q;
	{
		STASH_REGS;
		q=clock();
		UNSTASH_REGS;
	}
	a=((q)/(CLOCKS_PER_SEC/1000));
	b=q/(CLOCKS_PER_SEC);
	c=q;
#ifndef NO_PREEMPT
	if(EMULATE_DEPTH) instruction_counter += EXTREME_HIGH_INSN_COST; /*This is a very VERY expensive instruction.*/
#endif
}D
/*load from RX0*/
G_ARX0:a=RX0;D
G_BRX0:b=RX0;D
V9:c=RX0;D
/*store to RX0*/
VA:RX0=a;D
VB:RX0=b;D
VC:RX0=c;D
/*load from RX1*/
VD:a=RX1;D
VE:b=RX1;D
VF:c=RX1;D
/*Store*/
W0:RX1=a;D
W1:RX1=b;D
W2:RX1=c;D
/*load from RX2*/
W3:a=RX2;D
W4:b=RX2;D
W5:c=RX2;D
/*store to RX2*/
W6:RX2=a;D
W7:RX2=b;D
W8:RX2=c;D
/*load from RX3*/
W9:a=RX3;D
WA:b=RX3;D
WB:c=RX3;D
/*store to RX3*/
WC:RX3=a;D
WD:RX3=b;D
WE:RX3=c;D
/*Register-to-register moves for RX registers.*/
WF:RX0=RX1;D
X0:RX0=RX2;D
X1:RX0=RX3;D

X2:RX1=RX0;D
X3:RX1=RX2;D
X4:RX1=RX3;D

X5:RX2=RX0;D
X6:RX2=RX1;D
X7:RX2=RX3;D

X8:RX3=RX0;D
X9:RX3=RX1;D
XA:RX3=RX2;D

/*loads*/
XB:RX0=CONSUME_FOUR_BYTES;D
XC:RX1=CONSUME_FOUR_BYTES;D
XD:RX2=CONSUME_FOUR_BYTES;D
XE:RX3=CONSUME_FOUR_BYTES;D
/*far indirect loads, through C and A.*/
XF:RX0=Z_FAR_MEMORY_READ_C_HIGH8_A_LOW16_4;D
Y0:RX1=Z_FAR_MEMORY_READ_C_HIGH8_A_LOW16_4;D
Y1:RX2=Z_FAR_MEMORY_READ_C_HIGH8_A_LOW16_4;D
Y2:RX3=Z_FAR_MEMORY_READ_C_HIGH8_A_LOW16_4;D
/*far indirect stores, through C and A*/
Y3:write_4bytes(RX0,((((UU)c)<<16)|((UU)a)))D
Y4:write_4bytes(RX1,((((UU)c)<<16)|((UU)a)))D
Y5:write_4bytes(RX2,((((UU)c)<<16)|((UU)a)))D
Y6:write_4bytes(RX3,((((UU)c)<<16)|((UU)a)))D
/*math*/
Y7:RX0=(RX0+RX1)&0xffFFffFF;D
Y8:RX0=(RX0-RX1)&0xffFFffFF;D
Y9:RX0=(RX0*RX1)&0xffFFffFF;D
YA:if(RX1!=0)RX0=(RX0/RX1)&0xffFFffFF;else{R=3;goto G_HALT;}D
YB:if(RX1!=0)RX0=(RX0%RX1)&0xffFFffFF;else{R=4;goto G_HALT;}D
YC:RX0=(RX0>>(RX1))&0xffFFffFF;D
YD:RX0=(RX0<<(RX1))&0xffFFffFF;D
/*pushes*/
YE:
M[stack_pointer++] = RX0>>24;
M[stack_pointer++] = RX0>>16;
M[stack_pointer++] = RX0>>8;
M[stack_pointer++] = RX0;
D
YF:
M[stack_pointer++] = RX1>>24;
M[stack_pointer++] = RX1>>16;
M[stack_pointer++] = RX1>>8;
M[stack_pointer++] = RX1;
D
Z0:
M[stack_pointer++] = RX2>>24;
M[stack_pointer++] = RX2>>16;
M[stack_pointer++] = RX2>>8;
M[stack_pointer++] = RX2;
D
Z1:
M[stack_pointer++] = RX3>>24;
M[stack_pointer++] = RX3>>16;
M[stack_pointer++] = RX3>>8;
M[stack_pointer++] = RX3;
D
/*pops*/
Z2:
	stack_pointer--; RX0=M[stack_pointer];
	stack_pointer--; RX0 |= (((UU)M[stack_pointer])<<8);
	stack_pointer--; RX0 |= (((UU)M[stack_pointer])<<16);
	stack_pointer--; RX0 |= (((UU)M[stack_pointer])<<24);
D
Z3:
	stack_pointer--; RX1=M[stack_pointer];
	stack_pointer--; RX1 |= (((UU)M[stack_pointer])<<8);
	stack_pointer--; RX1 |= (((UU)M[stack_pointer])<<16);
	stack_pointer--; RX1 |= (((UU)M[stack_pointer])<<24);
D
Z4:
	stack_pointer--; RX2=M[stack_pointer];
	stack_pointer--; RX2 |= (((UU)M[stack_pointer])<<8);
	stack_pointer--; RX2 |= (((UU)M[stack_pointer])<<16);
	stack_pointer--; RX2 |= (((UU)M[stack_pointer])<<24);
D
Z5:
	stack_pointer--; RX3=M[stack_pointer];
	stack_pointer--; RX3 |= (((UU)M[stack_pointer])<<8);
	stack_pointer--; RX3 |= (((UU)M[stack_pointer])<<16);
	stack_pointer--; RX3 |= (((UU)M[stack_pointer])<<24);
D
/*bitwise*/
Z6:RX0=RX0&RX1;D
Z7:RX0=RX0|RX1;D
Z8:RX0=RX0^RX1;D
Z9:RX0=~RX0;D
ZA:
	if(RX0<RX1)a=0;
	else if(RX0>RX1)a=2;
	else a=1;
D
ZB:
#if !defined(NO_SEGMENT)
	if(seg_access_offset==SEGMENT_PAGES){R=5;goto G_HALT;}
	{
		STASH_REGS;
		memcpy(
			M + 0x100 * (RX0&0xffFF),

			SEGMENT + 0x100 * (seg_access_offset +	(RX1 & seg_access_mask)),
			0x100
		);
		UNSTASH_REGS;
#ifndef NO_PREEMPT
		if(EMULATE_DEPTH) instruction_counter += MED_INSN_COST; /*This is a very expensive instruction.*/
#endif
	}
	D
#else
	R=14; goto G_HALT;
#endif
ZC:
#if !defined(NO_SEGMENT)
	if(seg_access_offset==SEGMENT_PAGES){R=5;goto G_HALT;}
	else
	{
		STASH_REGS;
		memcpy(
			SEGMENT + 0x100 *  (seg_access_offset +	(RX1 & seg_access_mask)),
			M + 0x100 * (RX0&0xffFF),
			0x100);
		UNSTASH_REGS;
#ifndef NO_PREEMPT
		if(EMULATE_DEPTH) instruction_counter += MED_INSN_COST; /*This is a very expensive instruction.*/
#endif
	}
	D
#else
	R=14; goto G_HALT;
#endif

ZD: 
#if !defined(NO_SEGMENT)
	if(EMULATE_DEPTH) {R = 15; goto G_HALT;}
	seg_access_offset = RX0;
	seg_access_mask = RX1;
D
#else
	R=14; goto G_HALT;
#endif

#ifdef NO_FP
/*no floating point unit.*/
/*
	TODO: implement software floating point unit.
*/
ZE: R=8; goto G_HALT;
ZF: R=8; goto G_HALT;
G_AA0: R=8; goto G_HALT;
G_AA1: R=8; goto G_HALT;
/*cmp*/
G_FLTCMP: R=8; goto G_HALT;
#else
ZE:{
	float fRX0, fRX1;
	UU RX0_CP = RX0;
	UU RX1_CP = RX1;
	memcpy(&fRX0, &RX0_CP, 4);
	memcpy(&fRX1, &RX1_CP, 4);
	fRX0 = fRX0 + fRX1;
	memcpy(&RX0_CP, &fRX0, 4);
	RX0 = RX0_CP;
}D
ZF: {
	float fRX0, fRX1;
	UU RX0_CP = RX0;
	UU RX1_CP = RX1;
	memcpy(&fRX0, &RX0_CP, 4);
	memcpy(&fRX1, &RX1_CP, 4);
	fRX0 = fRX0 - fRX1;
	memcpy(&RX0_CP, &fRX0, 4);
	RX0 = RX0_CP;
}D
G_AA0:{
		float fRX0, fRX1;
	UU RX0_CP = RX0;
	UU RX1_CP = RX1;
	memcpy(&fRX0, &RX0_CP, 4);
	memcpy(&fRX1, &RX1_CP, 4);
	fRX0 = fRX0 * fRX1;
	memcpy(&RX0_CP, &fRX0, 4);
	RX0 = RX0_CP;
}D
G_AA1: {
	float fRX0, fRX1;
	UU RX0_CP = RX0;
	UU RX1_CP = RX1;
	memcpy(&fRX0, &RX0_CP, 4);
	memcpy(&fRX1, &RX1_CP, 4);
	if(fRX1 == 0.0 || 
		fRX1 == -0.0){
			R=9; goto G_HALT;
		}
	fRX0 = fRX0 / fRX1;
	memcpy(&RX0_CP, &fRX0, 4);
	RX0 = RX0_CP;
}D
/*cmp*/
G_FLTCMP: {
	float fRX0, fRX1;
	UU RX0_CP = RX0;
	UU RX1_CP = RX1;
	memcpy(&fRX0, &RX0_CP, 4);
	memcpy(&fRX1, &RX1_CP, 4);
	if(
		fRX0<fRX1
	)a=0;else if(
		fRX0>fRX1
	)a=2;else a=1;
}D
#endif
G_AA3:RX0=SEGMENT_PAGES;D
G_AA4:RX0=Z_READ4BYTES(RX1)D
G_AA5:RX0=Z_READ4BYTES(RX0)D
G_AA6:SET_PCR(RX0>>16);SET_PC(RX0 & 0xffFF);D
G_AA7:write_4bytes(RX0,RX1)D
G_AA8:write_4bytes(RX1,RX0)D
G_AA9:c=(RX0>>16);b=RX0;D
G_AA10:c=(RX0>>16);a=RX0;D
#if !defined(NO_SIGNED_DIV)
G_AA11:{SUU SRX0, SRX1;
		SRX0 = RX0;
		SRX1 = RX1;
	if(SRX1!=0)RX0=(SRX0/SRX1)&0xffFFffFF;else{R=3;goto G_HALT;}
}D
G_AA12:{SUU SRX0, SRX1;
		SRX0 = RX0;
		SRX1 = RX1;
	if(SRX1!=0)RX0=(SRX0%SRX1)&0xffFFffFF;else{R=4;goto G_HALT;}
}D
#else
	/*
		TODO: Emulate signed integer division for two's complement guaranteed behavior.
	*/
	G_AA11:R=10;goto G_HALT;
	G_AA12:R=10;goto G_HALT;
#endif
	G_AA13:{UU flight;
		flight = CONSUME_THREE_BYTES;
		RX0=(((UU)M[flight])<<24) | 
					(((UU)M[(flight+1)&0xffFFff])<<16) |
					(((UU)M[(flight+2)&0xffFFff])<<8) |
					(((UU)M[(flight+3)&0xffFFff]));
	}D
	G_AA14:{UU flight;
		flight = CONSUME_THREE_BYTES;
		RX1=(((UU)M[flight])<<24) | 
							(((UU)M[(flight+1)&0xffFFff])<<16) |
							(((UU)M[(flight+2)&0xffFFff])<<8) |
							(((UU)M[(flight+3)&0xffFFff]));
	}D
	G_AA15:{UU flight;
		flight = CONSUME_THREE_BYTES;
		RX2=(((UU)M[flight])<<24) | 
							(((UU)M[(flight+1)&0xffFFff])<<16) |
							(((UU)M[(flight+2)&0xffFFff])<<8) |
							(((UU)M[(flight+3)&0xffFFff]));
	}D
	G_AA16:{UU flight;
		flight = CONSUME_THREE_BYTES;
		RX3=(((UU)M[flight])<<24) | 
							(((UU)M[(flight+1)&0xffFFff])<<16) |
							(((UU)M[(flight+2)&0xffFFff])<<8) |
							(((UU)M[(flight+3)&0xffFFff]));
	}D
	G_AA17:{UU flight;
		flight = CONSUME_THREE_BYTES;
		a=(((UU)M[(flight)])<<8) | 
					(((UU)M[(flight+1)&0xffFFff]));
	}D
	G_AA18:{UU flight;
		flight = CONSUME_THREE_BYTES;
		b=(((UU)M[(flight)])<<8) | 
					(((UU)M[(flight+1)&0xffFFff]));
	}D
	G_AA19:{UU flight;
		flight = CONSUME_THREE_BYTES;
		c=(((UU)M[(flight)])<<8) | 
					(((UU)M[(flight+1)&0xffFFff]));
	}D
	G_AA20:{UU flight;
		flight = CONSUME_THREE_BYTES;
		write_4bytes(RX0, flight);
	}D
	G_AA21:{UU flight;
		flight = CONSUME_THREE_BYTES;
		write_4bytes(RX1, flight);
	}D
	G_AA22:{UU flight;
		flight = CONSUME_THREE_BYTES;
		write_4bytes(RX2, flight);
	}D
	G_AA23:{UU flight;
		flight = CONSUME_THREE_BYTES;
		write_4bytes(RX3, flight);
	}D
	G_AA24:{UU flight;
		flight = CONSUME_THREE_BYTES;
		write_2bytes(a, flight);
	}D
	G_AA25:{UU flight;
		flight = CONSUME_THREE_BYTES;
		write_2bytes(b, flight);
	}D
	G_AA26:{UU flight;
		flight = CONSUME_THREE_BYTES;
		write_2bytes(c, flight);
	}D
	G_AINCR:a++;D
	G_ADECR:a--;D
	G_RX0INCR:RX0++;D
	G_RX0DECR:RX0--;D
#if defined(NO_FP)
	G_ITOF:G_FTOI:R=8;goto G_HALT;
#else
	G_ITOF:{
		float fRX0;
		SUU lRX0 = RX0;
		fRX0 = (float)lRX0;
		memcpy(&lRX0, &fRX0, 4);
		RX0 = lRX0;
	}D
	G_FTOI:{
		float fRX0;
		SUU lRX0 = RX0;
			memcpy(&fRX0,&lRX0,4);
			lRX0 = fRX0;
			memcpy(&lRX0, &fRX0,4);
		RX0 = lRX0;
	}D
#endif

	G_SEG_GETCONFIG:
#if !defined(NO_SEGMENT)
	{
		RX1 = seg_access_mask;
		RX0 = seg_access_offset;
	}D
#else
	R=14; goto G_HALT;
#endif
#if !defined(NO_EMULATE)
	G_EMULATE:{
		if(EMULATE_DEPTH) {R=11; goto G_HALT;}

		{
			STASH_REGS;
			memcpy(M_SAVER[current_task], M_SAVER[0], 0x1000000);
			UNSTASH_REGS;
		}
		SAVE_REGISTER(a, 0);
		SAVE_REGISTER(b, 0);
		SAVE_REGISTER(c, 0);
		SAVE_REGISTER(program_counter, 0);
		SAVE_REGISTER(stack_pointer, 0);
		SAVE_REGISTER(program_counter_region, 0);
		SAVE_REGISTER(RX0, 0);
		SAVE_REGISTER(RX1, 0);
		SAVE_REGISTER(RX2, 0);
		SAVE_REGISTER(RX3, 0);		
		EMULATE_DEPTH = 1;
		M = M_SAVER[current_task];
		stack_pointer=0;
		SET_PCR(0);
		SET_PC(0);
#ifndef NO_PREEMPT
		instruction_counter = 0;
#endif
		RX0=0;RX1=0;RX2=0;RX3=0;
		a=0;b=0;c=0;
	}D
#else
	G_EMULATE: R=14; goto G_HALT;
#endif
	G_RXICMP:
	{
		SUU RX0I = RX0;
		SUU RX1I = RX1;
		if(RX0I<RX1I)		a=0;
		else if(RX0I>RX1I)	a=2;
		else 				a=1;
	}D
	G_LOGOR: a = a || b; D
	G_LOGAND: a = a && b;D
	G_BOOLIFY: a = (a!=0)D
	G_NOTA: a=(a==0)D
	G_USER_FARISTA:{
		if(EMULATE_DEPTH){R=15; goto G_HALT;}
	 	M_SAVER[current_task][ (((UU)c&255)<<16) | (UU)b]=a;
	}
	D
	/*add more insns here. remember the free slots above!*/
	G_TASK_RIC:
#ifndef NO_PREEMPT
		if(EMULATE_DEPTH){R=15; goto G_HALT;}
		REG_SAVER[current_task].instruction_counter = 0;
#endif
	D
	G_USER_FARPAGEL:
	if(EMULATE_DEPTH){R=15; goto G_HALT;}
	{
		STASH_REGS;
		memcpy(
			M_STASH + (a_stash<<8),
			M_SAVER[current_task] + (c_stash<<8),
			256
		);
		UNSTASH_REGS;
	}D
	G_USER_FARPAGEST:
	if(EMULATE_DEPTH){R=15; goto G_HALT;}
	{
		STASH_REGS;
		memcpy(
			M_SAVER[current_task] + (c_stash<<8),
			M_STASH + (a_stash<<8),
			256
		);
		UNSTASH_REGS;
	}D
	G_LLDA:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		a = Z_READ2BYTES(f);
	}D
	G_LLDB:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		b = Z_READ2BYTES(f);
	}D
	G_LDRX0:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		RX0 = Z_READ4BYTES(f);
	}D
	G_LDRX1:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		RX1 = Z_READ4BYTES(f);
	}D	
	G_LDRX2:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		RX2 = Z_READ4BYTES(f);
	}D
	G_LDRX3:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		RX3 = Z_READ4BYTES(f);
	}D
	G_LDC:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		c = Z_READ2BYTES(f);
	}D
	G_STRX0:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		write_4bytes(RX0, f);
	}D
	G_STRX1:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		write_4bytes(RX1, f);
	}D
	G_STRX2:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		write_4bytes(RX2, f);
	}D
	G_STRX3:{
		UU f = CONSUME_TWO_BYTES;
		f = GET_LOCAL_ADDR(f);
		write_4bytes(RX3, f);
	}D
	G_HALT:
	if(EMULATE_DEPTH == 0){
		dcl();return 0;
	} else {
		SAVE_REGISTER(a, current_task);
		SAVE_REGISTER(b, current_task);
		SAVE_REGISTER(c, current_task);
		SAVE_REGISTER(program_counter, current_task);
		SAVE_REGISTER(stack_pointer, current_task);
		SAVE_REGISTER(program_counter_region, current_task);
		SAVE_REGISTER(RX0, current_task);
		SAVE_REGISTER(RX1, current_task);
		SAVE_REGISTER(RX2, current_task);
		SAVE_REGISTER(RX3, current_task);
#ifndef NO_PREEMPT
		SAVE_REGISTER(instruction_counter, current_task);
#endif
		M=M_SAVER[0];
		EMULATE_DEPTH=0;
		a=R;R=0;
		LOAD_REGISTER(b, 0);
		LOAD_REGISTER(c, 0);
		LOAD_REGISTER(program_counter, 0);
		LOAD_REGISTER(program_counter_region, 0);
		LOAD_REGISTER(stack_pointer, 0);
		LOAD_REGISTER(RX0, 0);
		LOAD_REGISTER(RX1, 0);
		LOAD_REGISTER(RX2, 0);
		LOAD_REGISTER(RX3, 0);
		D
	}
}
#undef D
#undef k

