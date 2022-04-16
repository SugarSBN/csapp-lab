/*
 * @Author: SuBonan
 * @Date: 2022-04-14 20:11:21
 * @LastEditTime: 2022-04-15 19:05:50
 * @FilePath: \bomblab\phase6.c
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
int a[6];
int *r13 = a;
int *rsi = a;
int *r14 = a;
int r12d = 0;
begin:
int *rbp = r13;
int eax = r13[0];
if (eax - 1 <= 5){
    r12d += 1;
    if (r12d == 6){ // 401153

    } else { // 401132
        int ebx = r12d;
        L1: eax = a[ebx];
        if (eax != rbp[0]) {
            ebx += 1;
            if (ebx <= 5){
                goto L1;
            } else {
                r13 += 4;
                goto begin;
            }
        } else bomb();
    }
} else bomb();