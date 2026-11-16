#include "bsp.h"
#include <stdlib.h>
#include <string.h>
typedef struct { int x,y,w,h; } Rect;
typedef struct { int x,y; } Point;
typedef struct { char *cells; int width, room; uint32_t rng; } Context;
static int random_between(Context *c, int low, int high) {
    c->rng=c->rng*UINT32_C(1664525)+UINT32_C(1013904223);
    return low+(int)(c->rng%(uint32_t)(high-low+1));
}
static void carve(Context *c, int x, int y) {
    c->cells[(size_t)y*(size_t)c->width+(size_t)x]='.';
}
static Point room(Context *c, Rect r) {
    int w=random_between(c,c->room,r.w), h=random_between(c,c->room,r.h);
    int x=random_between(c,r.x,r.x+r.w-w), y=random_between(c,r.y,r.y+r.h-h);
    for(int j=y;j<y+h;j++) for(int i=x;i<x+w;i++) carve(c,i,j);
    return (Point){x+w/2,y+h/2};
}
static void connect(Context *c, Point a, Point b) {
    while(a.x!=b.x) {carve(c,a.x,a.y);a.x+=a.x<b.x?1:-1;}
    while(a.y!=b.y) {carve(c,a.x,a.y);a.y+=a.y<b.y?1:-1;}
    carve(c,b.x,b.y);
}
/* Sibling partitions cover the parent exactly. Each returns a point in its
 * connected floor component; joining those points proves connectivity. */
static Point partition(Context *c, Rect r, unsigned depth) {
    int vertical=random_between(c,0,1);
    if(r.w<2*c->room) vertical=0;
    if(r.h<2*c->room) vertical=1;
    if(depth==0 || (r.w<2*c->room && r.h<2*c->room)) return room(c,r);
    Rect a=r,b=r;
    if(vertical) {
        a.w=random_between(c,c->room,r.w-c->room);
        b.x+=a.w;b.w-=a.w;
    } else {
        a.h=random_between(c,c->room,r.h-c->room);
        b.y+=a.h;b.h-=a.h;
    }
    Point p=partition(c,a,depth-1), q=partition(c,b,depth-1);
    connect(c,p,q);
    return p;
}
char *bsp_generate(int width, int height, unsigned depth, int min_room, uint32_t seed) {
    if(width<3 || height<3 || width>4096 || height>4096 || depth>32 ||
       min_room<1 || min_room>width-2 || min_room>height-2) return NULL;
    size_t size=(size_t)width*(size_t)height;
    char *cells=malloc(size);
    if(!cells) return NULL;
    memset(cells,'#',size);
    Context c={cells,width,min_room,seed};
    partition(&c,(Rect){1,1,width-2,height-2},depth);
    return cells;
}
