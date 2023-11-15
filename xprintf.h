/*******************************************
# Author        : Retuze
# Date          : 2023-11-13 05:06:28
# LastEditors   : Retuze 
# LastEditTime  : 2023-11-14 23:44:31
# Description   :
*********************************************/
/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2021     */
/*------------------------------------------------------------------------*/

#ifndef _XPRINT_H_
#define _XPRINT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define XF_USE_OUTPUT 1 /* 1: Enable output functions */
#define XF_CRLF 0       /* 1: Convert \n ==> \r\n in the output char */
#define XF_USE_LLI 0    /* 1: Enable long long integer in size prefix ll */
#define XF_USE_FP 1     /* 1: Enable support for floating point in type e and f */
#define XF_USE_INPUT 0  /* 1: Enable input functions */
#define XF_INPUT_ECHO 0 /* 1: Echo back input chars in xgets function */

#define lambda_core(return_type, function_body) \
    ({                                     \
        return_type $this function_body    \
            $this;                         \
    })
#define lambda(return_pype,parm_type,parm,body) lambda_core(return_pype, (parm_type parm){body})
#define $ lambda

#if XF_USE_OUTPUT
    void xdev_out(void (*func)(char));
    void xputc(char chr);
    void xfputc(void (*func)(char), char chr);
    void xputs(const char *str);
    void xfputs(void (*func)(char), const char *str);
    void xprintf(const char *fmt, ...);
    void xsprintf(char *buff, const char *fmt, ...);
    void xfprintf(void (*func)(char), const char *fmt, ...);
#endif

#if XF_USE_INPUT
#define xdev_in(func) xfunc_input = (int (*)(void))(func)
    extern int (*xfunc_input)(void);
    int xgets(char *buff, int len);
    int xatoi(char **str, long *res);
    int xatof(char **str, double *res);
#endif

#ifdef __cplusplus
}
#endif

#endif // _XPRINT_H_
