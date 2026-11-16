#include "bsp.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
static void check(int w, int h, unsigned depth, int room, uint32_t seed) {
    char *a = bsp_generate(w,h,depth,room,seed);
    char *b = bsp_generate(w,h,depth,room,seed);
    assert(a && b && memcmp(a,b,(size_t)w*(size_t)h)==0);
    size_t n=(size_t)w*(size_t)h, count=0, head=0, tail=0;
    size_t *queue=malloc(n*sizeof(*queue));
    assert(queue);
    for(int y=0;y<h;y++) for(int x=0;x<w;x++) {
        size_t i=(size_t)y*(size_t)w+(size_t)x;
        assert(a[i]=='#' || a[i]=='.');
        if(x==0 || y==0 || x==w-1 || y==h-1) assert(a[i]=='#');
        if(a[i]=='.') { if(count==0) queue[tail++]=i; count++; }
    }
    assert(count >= (size_t)room*(size_t)room);
    a[queue[0]]='v';
    while(head<tail) {
        size_t i=queue[head++];
        size_t next[]={i-1,i+1,i-(size_t)w,i+(size_t)w};
        for(size_t k=0;k<4;k++) if(a[next[k]]=='.') {a[next[k]]='v';queue[tail++]=next[k];}
    }
    assert(tail==count);
    free(queue);free(a);free(b);
}
int main(void) {
    assert(!bsp_generate(0,20,2,3,1));
    assert(!bsp_generate(20,-1,2,3,1));
    assert(!bsp_generate(20,20,33,3,1));
    assert(!bsp_generate(20,20,2,0,1));
    assert(!bsp_generate(4,4,2,3,1));
    assert(!bsp_generate(4097,20,2,3,1));
    check(3,3,32,1,0);check(20,20,0,18,1);
    for(uint32_t seed=0;seed<200;seed++) {
        check(64,35,8,3,seed);check(7,80,32,2,seed);check(80,7,32,2,seed);
    }
    check(4096,4096,0,4094,1);
    char *a=bsp_generate(64,35,8,3,1), *b=bsp_generate(64,35,8,3,2);
    assert(a && b && memcmp(a,b,64*35)!=0);
    free(a);free(b);
    puts("All BSP behavior tests passed (603 map cases plus seed variation).");
}
