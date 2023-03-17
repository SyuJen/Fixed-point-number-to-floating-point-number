#include <stdint.h>
#include <stdio.h>
#include <math.h>


int main()
{
    uint64_t sfix40_in = 0xffffffc000;
    uint8_t fraction_len = 0x13;
    uint64_t integer_part=0;
    uint64_t decimal_par=0;
    double decimal=0;
    int cnt;
    uint8_t signal=0; //默认为正
    float floating_point_num_32=0;

    // printf("Enter a 40-bit Fixed Point Number in hexadecimal format:0x");
    // scanf("%lu", &sfix40_in);
    // printf("\n");
    // printf("Enter the length of the decimal portion of this 40-bit Fixed Point Number in hexadecimal format:0x");
    // scanf("%hhu", &fraction_len); // problem
    // printf("\n");
    // if(sfix40_in & 0xFFFFFF0000000000)
    //     printf("error, input again!\n");
    // else
    // {
        /* 1. 判断正负 */
        if(sfix40_in & 0x8000000000) // 为负
        {
            sfix40_in = ( (~sfix40_in)+((uint64_t)0x1) ) & 0x7FFFFFFFFF;
            signal = 1;
        }
        else // 为正
        {
            signal=0;
        }
        /* 2. 取小数部分 */
        decimal_par = sfix40_in & ( (uint64_t)(( ((uint64_t)1) <<fraction_len) - 1) );
        //printf("decimal_par:%lld\r\n", decimal_par);

        cnt = 1;
        while(cnt<=fraction_len)
        {
            if( decimal_par & (((uint64_t)0x1) <<(fraction_len-cnt)) )
            {
                // printf("cnt:%d\r\n", cnt);
                decimal += pow(2, -1*cnt);
            }
            cnt++;
        }

        /* 3. 整数部分 */
        integer_part = (sfix40_in & 0x7fffffffff) >> fraction_len;
        if((signal == 1) && (integer_part==0) && (decimal_par==0)) // 有可能有问题
        {
            printf("aa\r\n");
            integer_part = integer_part | (((uint64_t)0x1)<<(40-fraction_len-1)) ;
        }
        // printf("integer_part:0x%llx\r\n", integer_part);

        /* 4. 打印 */
        if(signal == 1)
        {
            // printf("data:-%llu.%.35lf\r\n",integer_part, decimal);
            floating_point_num_32 = (float)(-1)* ((float)integer_part + (float)decimal);
            // printf("data: -%f\n", floating_point_num_32);

        }
        else if(signal == 0)
        {
            floating_point_num_32 = (float)integer_part + (float)decimal;
            // printf("data:+%llu.%.35lf\r\n",integer_part, decimal);
            // printf("data: %f\n", floating_point_num_32);
        }
        printf("0x%x\n", *(int *)&floating_point_num_32);
    // }



    return 0;
}