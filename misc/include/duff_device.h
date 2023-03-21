#ifndef __DUFF_DEVICE_H__
#define __DUFF_DEVICE_H__

#define crBegin static int state=0; switch(state) { case 0:
#define crReturn(x) do { state=__LINE__; return x; \
                         case __LINE__:; } while (0)
#define crFinish }

int duff_device_test_function(void)
{
    static int i;
    crBegin;
    for (i = 0; i < 10; i++)
        crReturn(i);
    crFinish;
}

// int function(void) {
//     static int i, state = 0;
//     switch (state) {
//         case 0: /* start of function */
//         for (i = 0; i < 10; i++) {
//             state = 1; /* so we will come back to "case 1" */
//             return i;
//             case 1:; /* resume control straight after the return */
//         }
//     }
// }

#endif
