
/*
	Integer modulo for C
*/

static int modi(int a, int b){return (a%b+b)%b;}
static short mods(short a, short b){return (a%b+b)%b;}
static long modl(long a, long b){return (a%b+b)%b;}

#define MAKE_MOD_FUNCTION(int_type,name) static int_type name(int_type a, int_type b)     {return (a%b+b)%b;}
