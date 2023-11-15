/*******************************************
# Author        : Retuze
# Date          : 2023-11-14 02:05:33
# LastEditors   : Retuze 
# LastEditTime  : 2023-11-14 23:47:29
# Description   :
*********************************************/
#include "xprintf.h"

#if XF_USE_OUTPUT
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
static void (*xfunc_output)(char); /* Pointer to the default output device */
static char *strptr;               /* Pointer to the output memory (used by xsprintf) */
void xdev_out(void (*func)(char))
{
    xfunc_output = func;
}

#if XF_USE_FP
/*----------------------------------------------*/
/* Floating point output                        */
/*----------------------------------------------*/

void xfputs_with_fmt(void (*func)(char), const char *str, char flag, char width)
{
    // Add padding
    char padding = width - strlen(str);
    switch (flag)
    {
    case '-':
        xfputs(func, str);
        for (int i = 0; i < padding; ++i)
        {
            xputc(' ');
        }
        break;
    case '+':
        for (int i = 0; i < padding; ++i)
        {
            xputc(' ');
        }
        xfputs(func, str);
        break;
    case ' ':
        for (int i = 0; i < padding; ++i)
        {
            xputc(' ');
        }
        xfputs(func, str);
        break;
    case '0':
        for (int i = 0; i < padding; ++i)
        {
            xputc('0');
        }
        xfputs(func, str);
        break;
    case '#':
        for (int i = 0; i < padding; ++i)
        {
            xputc(' ');
        }
        xfputs(func, str);
        break;
    default:
        for (int i = 0; i < padding; ++i)
        {
            xputc(' ');
        }
        xfputs(func, str);
        break;
    }
}

char intlen(int dec)
{
    char width = 0;
    if (dec < 0)
    {
        dec = -dec;
        width++;
    }
    while (dec != 0)
    {
        dec /= 10;
        ++width;
    }
    return width;
}

void xfputi(void (*func)(char), int dec)
{
    if (dec < 0)
    {
        xfputc(func, '-');
        dec = -dec;
    }
    if (dec == 0)
    {
        xfputc(func, '0');
        return;
    }
    if (dec / 10 == 0)
    {
        xfputc(func, (const char)(dec % 10 + '0'));
    }
    else
    {
        xfputi(func, (dec / 10));
        xfputc(func, (const char)(dec % 10 + '0'));
    }
}

void xfputi_with_fmt(void (*func)(char), int dec, char flag, char width)
{
    // Add padding
    char padding = width - intlen(dec);
    switch (flag)
    {
    case '-':
        xfputi(func, dec);
        for (int i = 0; i < padding; ++i)
        {
            xfputc(func, ' ');
        }
        break;
    case '+':
        if (dec > 0)
        {
            for (int i = 0; i < padding - 1; ++i)
            {
                xfputc(func, ' ');
            }
            xfputc(func, '+');
        }
        else
        {
            for (int i = 0; i < padding; ++i)
            {
                xfputc(func, ' ');
            }
        }
        xfputi(func, dec);
        break;
    case ' ':
        for (int i = 0; i < padding; ++i)
        {
            xfputc(func, ' ');
        }
        xfputi(func, dec);
        break;
    case '0':
        for (int i = 0; i < padding; ++i)
        {
            xputc('0');
        }
        xfputi(func, dec);
        break;
    case '#':
        for (int i = 0; i < padding; ++i)
        {
            xfputc(func, ' ');
        }
        xfputi(func, dec);
        break;
    default:
        for (int i = 0; i < padding; ++i)
        {
            xfputc(func, ' ');
        }
        xfputi(func, dec);
        break;
    }
}
void xfputf(void (*func)(char), float flt, char numDecimals)
{
    if (flt < 0)
    {
        xfputc(func, '-');
        flt = -flt;
    }
    int intPart = (int)flt;
    float fractionalPart = flt - intPart;
    xfputi(func, intPart);
    xfputc(func, (const char)('.'));
    while (numDecimals > 0)
    {
        fractionalPart *= 10;
        xfputi(func, (int)(fractionalPart));
        fractionalPart -= (int)(fractionalPart);
        numDecimals--;
    }
}
void xfputf_with_fmt(void (*func)(char), float flt, char flag, char width, char numDecimals)
{
    // Add padding
    char padding = width - (numDecimals + 1 + intlen((int)flt));
    switch (flag)
    {
    case '-':
        xfputf(func, flt, numDecimals);
        for (int i = 0; i < padding; ++i)
        {
            xfputc(func, ' ');
        }
        break;
    case '+':
        if (flt > 0)
        {
            for (int i = 0; i < padding - 1; ++i)
            {
                xfputc(func, ' ');
            }
            xfputc(func, '+');
        }
        else
        {
            for (int i = 0; i < padding; ++i)
            {
                xfputc(func, ' ');
            }
        }
        xfputf(func, flt, numDecimals);
        break;
    case ' ':
        for (int i = 0; i < padding; ++i)
        {
            xfputc(func, ' ');
        }
        xfputf(func, flt, numDecimals);
        break;
    case '0':
        for (int i = 0; i < padding; ++i)
        {
            xfputc(func, '0');
        }
        xfputf(func, flt, numDecimals);
        break;
    case '#':
        for (int i = 0; i < padding; ++i)
        {
            xfputc(func, ' ');
        }
        xfputf(func, flt, numDecimals);
        break;
    default:
        for (int i = 0; i < padding; ++i)
        {
            xfputc(func, ' ');
        }
        xfputf(func, flt, numDecimals);
        break;
    }
}
#endif /* XF_USE_FLOAT */

/*----------------------------------------------*/
/* Put a character                              */
/*----------------------------------------------*/

void xputc(char chr)
{
    xfputc(xfunc_output, chr); /* Output it to the default output device */
}

void xfputc(void (*func)(char), char chr)
{
    if (XF_CRLF && chr == '\n')
        xfputc(func, '\r'); /* CR -> CRLF */

    if (func)
    {
        func(chr); /* Write a character to the output device */
    }
    else if (strptr)
    {
        *strptr++ = chr; /* Write a character to the memory */
    }
}

/*----------------------------------------------*/
/* Put a null-terminated string                 */
/*----------------------------------------------*/

void xputs(const char *str)
{
    xfputs(xfunc_output, str);
}

void xfputs(void (*func)(char), const char *str)
{
    while (*str)
    { /* Put the string */
        xfputc(func, *str++);
    }
}

static void xvfprintf(void (*func)(char), const char *fmt, va_list arp)
{
    char flag = 0;
    char width = 0;
    char precision = 2;

    while (*fmt)
    {
        if (*fmt != '%')
        { /* Pass it through if not a % sequense */
            xfputc(func, *fmt++);
            continue;
        }
        // %[flags][width][.precision][length]specifier
        flag = *(fmt + 1);
        // printf("flag :%c\n", flag);
        // 匹配flag
        switch (*(++fmt))
        {
        case '-':
            break;
        case '+':
            break;
        case ' ':
            break;
        case '#':
            break;
        case '0':
            break;
        default:
            flag = 0;
            fmt--;
            break;
        }
        char *endptr;
        // 匹配width
        width = strtol(fmt + 1, &endptr, 10);
        fmt = endptr;
        if (*fmt == '*')
        {
            fmt++;
            width = va_arg(arp, int);
        }
        // 匹配精度
        if (*fmt == '.')
        {
            precision = strtol(fmt + 1, &endptr, 10);
            fmt = endptr;
            if (*fmt == '*')
            {
                fmt++;
                precision = va_arg(arp, int);
            }
        }
        // 匹配类型长度
        switch (*(fmt++))
        {
        case 'h':
            if (*(fmt++) == 'h')
            {
            }
            else
            {
                fmt--;
            }
            break;
        case 'l':
            if (*(fmt++) == 'l')
            {
            }
            else
            {
                fmt--;
            }
            break;
        case 'L':
            break;
        default:
            fmt--;
            break;
        }
        // 匹配类型说明符
        switch (*(fmt++))
        {
        case 's':
            xfputs_with_fmt(func, va_arg(arp, char *), flag, width);
            break;
        case 'f':
            xfputf_with_fmt(func, va_arg(arp, double), flag, width, precision);
            break;
        case 'd':
            xfputi_with_fmt(func, va_arg(arp, int), flag, width);
            break;
        case 'c':
            xfputc(func, va_arg(arp, int));
            break;
        case '%':
            xfputc(func, '%');
            break;
        case 'u':
            // printf("class : 'u'\n");
            break;
        default:
            break;
        }
    }
}

void xprintf(const char *fmt, ...)
{
    va_list arp;

    va_start(arp, fmt);
    xvfprintf(xfunc_output, fmt, arp);
    va_end(arp);
}

void xfprintf(void (*func)(char), const char *fmt, ...)
{
    va_list arp;

    va_start(arp, fmt);
    xvfprintf(func, fmt, arp);
    va_end(arp);
}

void xsprintf(char *buff, const char *fmt, ...)
{
    va_list arp;

    strptr = buff; /* Enable destination for memory */
    va_start(arp, fmt);
    xvfprintf(0, fmt, arp);
    va_end(arp);
    *strptr = 0; /* Terminate output string */
    strptr = 0;  /* Disable destination for memory */
}

#endif /* XF_USE_OUTPUT */

#if XF_USE_INPUT
int (*xfunc_input)(void); /* Pointer to the default input stream */

/*----------------------------------------------*/
/* Get a line from the input                    */
/*----------------------------------------------*/

int xgets(            /* 0:End of stream, 1:A line arrived */
          char *buff, /* Pointer to the buffer */
          int len     /* Buffer length */
)
{
    int c, i;

    if (!xfunc_input)
        return 0; /* No input function is specified */

    i = 0;
    for (;;)
    {
        c = xfunc_input(); /* Get a char from the incoming stream */
        if (c < 0 || c == '\r')
            break; /* End of stream or CR? */
        if (c == '\b' && i)
        { /* BS? */
            i--;
            if (XF_INPUT_ECHO)
                xputc(c);
            continue;
        }
        if (c >= ' ' && i < len - 1)
        { /* Visible chars? */
            buff[i++] = c;
            if (XF_INPUT_ECHO)
                xputc(c);
        }
    }
    if (XF_INPUT_ECHO)
    {
        xputc('\r');
        xputc('\n');
    }
    buff[i] = 0; /* Terminate with a \0 */
    return (int)(c == '\r');
}

/*----------------------------------------------*/
/* Get a value of integer string                */
/*----------------------------------------------*/
/*	"123 -5   0x3ff 0b1111 0377  w "
        ^                           1st call returns 123 and next ptr
           ^                        2nd call returns -5 and next ptr
                   ^                3rd call returns 1023 and next ptr
                          ^         4th call returns 15 and next ptr
                               ^    5th call returns 255 and next ptr
                                  ^ 6th call fails and returns 0
*/

int xfputi(            /* 0:Failed, 1:Successful */
           char **str, /* Pointer to pointer to the string */
           long *res   /* Pointer to the valiable to store the value */
)
{
    unsigned long val;
    unsigned char c, r, s = 0;

    *res = 0;

    while ((c = **str) == ' ')
        (*str)++; /* Skip leading spaces */

    if (c == '-')
    { /* negative? */
        s = 1;
        c = *(++(*str));
    }

    if (c == '0')
    {
        c = *(++(*str));
        switch (c)
        {
        case 'x': /* hexdecimal */
            r = 16;
            c = *(++(*str));
            break;
        case 'b': /* binary */
            r = 2;
            c = *(++(*str));
            break;
        default:
            if (c <= ' ')
                return 1; /* single zero */
            if (c < '0' || c > '9')
                return 0; /* invalid char */
            r = 8;        /* octal */
        }
    }
    else
    {
        if (c < '0' || c > '9')
            return 0; /* EOL or invalid char */
        r = 10;       /* decimal */
    }

    val = 0;
    while (c > ' ')
    {
        if (c >= 'a')
            c -= 0x20;
        c -= '0';
        if (c >= 17)
        {
            c -= 7;
            if (c <= 9)
                return 0; /* invalid char */
        }
        if (c >= r)
            return 0; /* invalid char for current radix */
        val = val * r + c;
        c = *(++(*str));
    }
    if (s)
        val = 0 - val; /* apply sign if needed */

    *res = val;
    return 1;
}

#if XF_USE_FP
/*----------------------------------------------*/
/* Get a value of the real number string        */
/*----------------------------------------------*/
/* Float version of xfputi
 */

int xatof(            /* 0:Failed, 1:Successful */
          char **str, /* Pointer to pointer to the string */
          double *res /* Pointer to the valiable to store the value */
)
{
    double val;
    int s, f, e;
    unsigned char c;

    *res = 0;
    s = f = 0;

    while ((c = **str) == ' ')
        (*str)++; /* Skip leading spaces */
    if (c == '-')
    { /* Negative? */
        c = *(++(*str));
        s = 1;
    }
    else if (c == '+')
    { /* Positive? */
        c = *(++(*str));
    }
    if (c == XF_DPC)
    {           /* Leading dp? */
        f = -1; /* Start at fractional part */
        c = *(++(*str));
    }
    if (c <= ' ')
        return 0; /* Wrong termination? */
    val = 0;
    while (c > ' ')
    { /* Get a value of decimal */
        if (c == XF_DPC)
        { /* Embedded dp? */
            if (f < 0)
                return 0; /* Wrong dp? */
            f = -1;       /* Enter fractional part */
        }
        else
        {
            if (c < '0' || c > '9')
                break; /* End of decimal? */
            c -= '0';
            if (f == 0)
            { /* In integer part */
                val = val * 10 + c;
            }
            else
            { /* In fractional part */
                val += i10x(f--) * c;
            }
        }
        c = *(++(*str));
    }
    if (c > ' ')
    { /* It may be an exponent */
        if (c != 'e' && c != 'E')
            return 0; /* Wrong character? */
        c = *(++(*str));
        if (c == '-')
        {
            c = *(++(*str));
            s |= 2; /* Negative exponent */
        }
        else if (c == '+')
        {
            c = *(++(*str)); /* Positive exponent */
        }
        if (c <= ' ')
            return 0; /* Wrong termination? */
        e = 0;
        while (c > ' ')
        { /* Get value of exponent */
            c -= '0';
            if (c > 9)
                return 0; /* Not a numeral? */
            e = e * 10 + c;
            c = *(++(*str));
        }
        val *= i10x((s & 2) ? -e : e); /* Apply exponent */
    }

    if (s & 1)
        val = -val; /* Negate sign if needed */

    *res = val;
    return 1;
}
#endif /* XF_USE_FP */

#endif /* XF_USE_INPUT */

// 测试代码

#include <stdio.h>
int main()
{
    xdev_out(lambda(void,char,_ch,{printf("%c",_ch);}));
    xprintf("hello");
}
