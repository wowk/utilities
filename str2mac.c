#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

int mac2str(const void* mac, char* str, int len)
{
    uint8_t* p = (uint8_t*)mac;

    if( !mac || !str || len < 18 ){
        return -1;
    }
    
    sprintf(str,"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",p[0],p[1],p[2],p[3],p[4],p[5]);

    return 0;
}

int str2mac(const char* src,void* mac)
{
    int ret;
    int len = 0;
    uint32_t buf[6] = {0};
    uint8_t tmpbuf[20] = {0};
    uint8_t* p = (uint8_t*)mac;
    const char* fmt;

    if( !src || !mac || strlen(src) < 12 ){
        return -1;
    }

    len = strlen(src);
    memcpy(tmpbuf,src, len > sizeof(tmpbuf) ? sizeof(tmpbuf) : len );
   for( int i = 0 ; tmpbuf[i] ; i ++ ) {
       if( tmpbuf[i] == ':' ){
           continue; 
       }
       if( isxdigit(tmpbuf[i]) == 0 ){
           return -1;
       }
       if( isalpha(tmpbuf[i]) ){
           tmpbuf[i] |= 0x20;
       }
   }

    if( strchr(tmpbuf,':') ) {
        /* MAC format xx:xx:xx:xx:xx:xx */
        fmt = "%2x:%2x:%2x:%2x:%2x:%2x";
    } else {
        /* MAC format AABBCCDDEEFF */
        fmt = "%2x%2x%2x%2x%2x%2x";
    }

     ret = sscanf(tmpbuf,fmt,buf,buf+1,buf+2,buf+3,buf+4,buf+5);
     for( int i = 0 ; i < 6 ; i ++ )
         ((uint8_t*)mac)[i] = (uint8_t)buf[i];

    return 0;
}

int show_mac(void* data)
{
    uint8_t n;
    if( !data  )
        return -1;
    for( int i = 0 ; i < 5 ; i ++ ) {
        n = ((uint8_t*)data)[i];
        printf("%.2X-",n);
    }
    n = ((uint8_t*)data)[5];
    printf("%.2X",n);

    return 0;
}

int main(int argc,char** argv)
{
    char mac[6] = {0};
    char strmac[20] = {0};
    if( argc < 2 )
        return -1;

    if( str2mac(argv[1],mac) < 0 ){
        printf("str2mac error\n");
        return -1;
    }

    show_mac(mac);

    mac2str(mac,strmac,sizeof(strmac));
    printf("str mac : %s\n", strmac);

    return 0;
}