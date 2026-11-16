#include "bsp.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
static int parse(const char *s, unsigned long limit, unsigned long *value) {
    char *end;
    if(!s[0] || s[0]=='-') return 0;
    errno=0;*value=strtoul(s,&end,10);
    return !errno && !*end && *value<=limit;
}
int main(int argc, char **argv) {
    unsigned long args[]={64,32,5,3,1};
    const unsigned long limits[]={4096,4096,32,4094,UINT32_MAX};
    if(argc!=1 && argc!=6) {
        fprintf(stderr,"Usage: %s [width height depth min-room seed]\n",argv[0]);return 2;
    }
    for(int i=1;i<argc;i++) if(!parse(argv[i],limits[i-1],&args[i-1])) {
        fprintf(stderr,"Invalid numeric argument: %s\n",argv[i]);return 2;
    }
    char *map=bsp_generate((int)args[0],(int)args[1],(unsigned)args[2],(int)args[3],(uint32_t)args[4]);
    if(!map) {fputs("Invalid dimensions, room size, depth, or allocation failure.\n",stderr);return 1;}
    int failed=0;
    for(size_t y=0;y<args[1];y++) {
        if(fwrite(map+y*args[0],1,args[0],stdout)!=args[0] || putchar('\n')==EOF) {failed=1;break;}
    }
    if(fflush(stdout)==EOF) failed=1;
    free(map);return failed;
}
