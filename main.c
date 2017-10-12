/*
 *  @brief   sample code for algo api definition
 *  @date    2017-07-12
 *  @author  SSD3 team
 *  @history v0.1: new created  2017-07-12
 *           v0.2: fix the struct pointer issue    2017-07-13
 *           v1.0: algo api redefine                2017-08-11
 *           v1.1: algo api refinement            2017-08-15
 *           v1.2: algo api refinement            2017-08-17
*/

#include <stdio.h>
#include <stdlib.h>

extern int cwm_host_main_process(void);

int main( void )
{
    while(1) {
        cwm_host_main_process();
    }
}