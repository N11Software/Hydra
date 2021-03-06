#define ALWAYS_INLINE inline __attribute__ ((always_inline))

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

ALWAYS_INLINE size_t strlen(const char* str)
{
    size_t len = 0;
    while (*(str++))
        ++len;
    return len;
}

static constexpr const char* h = "0123456789abcdef";

template<typename PutChFunc>
ALWAYS_INLINE int printHex(PutChFunc putch, char*& bufptr, dword number, byte fields)
{
    int ret = 0;
    byte shr_count = fields * 4;
    while (shr_count) {
        shr_count -= 4;
        putch(bufptr, h[(number >> shr_count) & 0x0F]);
        ++ret;
    }
    return ret;
}

template<typename PutChFunc>
ALWAYS_INLINE int printNumber(PutChFunc putch, char*& bufptr, dword number, bool leftPad, bool zeroPad, dword fieldWidth)
{
    dword divisor = 1000000000;
    char ch;
    char padding = 1;
    char buf[16];
    char* p = buf;

    for (;;) {
        ch = '0' + (number / divisor);
        number %= divisor;
        if (ch != '0')
            padding = 0;
        if (!padding || divisor == 1)
            *(p++) = ch;
        if (divisor == 1)
            break;
        divisor /= 10;
    }

    size_t numlen = p - buf;
    if (!fieldWidth)
        fieldWidth = numlen;
    if (!leftPad) {
        for (unsigned i = 0; i < fieldWidth - numlen; ++i) {
            putch(bufptr, zeroPad ? '0' : ' ');
        }
    }
    for (unsigned i = 0; i < numlen; ++i) {
        putch(bufptr, buf[i]);
    }
    if (leftPad) {
        for (unsigned i = 0; i < fieldWidth - numlen; ++i) {
            putch(bufptr, ' ');
        }
    }

    return fieldWidth;
}

template<typename PutChFunc>
ALWAYS_INLINE int printString(PutChFunc putch, char*& bufptr, const char* str, bool leftPad, dword fieldWidth)
{
    size_t len = strlen(str);
    if (!fieldWidth)
        fieldWidth = len;
    if (!leftPad) {
        for (unsigned i = 0; i < fieldWidth - len; ++i)
            putch(bufptr, ' ');
    }
    for (unsigned i = 0; i < len; ++i) {
        putch(bufptr, str[i]);
    }
    if (leftPad) {
        for (unsigned i = 0; i < fieldWidth - len; ++i)
            putch(bufptr, ' ');
    }
    return fieldWidth;
}


template<typename PutChFunc>
ALWAYS_INLINE int printSignedNumber(PutChFunc putch, char*& bufptr, int number, bool leftPad, bool zeroPad, dword fieldWidth)
{
    if (number < 0) {
        putch(bufptr, '-');
        return printNumber(putch, bufptr, 0 - number, leftPad, zeroPad, fieldWidth) + 1;
    }
    return printNumber(putch, bufptr, number, leftPad, zeroPad, fieldWidth);
}

template<typename PutChFunc>
ALWAYS_INLINE int printfInternal(PutChFunc putch, char* buffer, const char*& fmt, char*& ap)
{
    const char *p;

    int ret = 0;
    char* bufptr = buffer;

    for (p = fmt; *p; ++p) {
        bool leftPad = false;
        bool zeroPad = false;
        unsigned fieldWidth = 0;
        if (*p == '%' && *(p + 1)) {
one_more:
            ++p;
            if (*p == ' ') {
                leftPad = true;
                if (*(p + 1))
                    goto one_more;
            }
            if (!zeroPad && !fieldWidth && *p == '0') {
                zeroPad = true;
                if (*(p + 1))
                    goto one_more;
            }
            if (*p >= '0' && *p <= '9') {
                fieldWidth *= 10;
                fieldWidth += *p - '0';
                if (*(p + 1))
                    goto one_more;
            }
            switch( *p )
            {
                case 's':
                    {
                        const char* sp = va_arg(ap, const char*);
                        ret += printString(putch, bufptr, sp ? sp : "(null)", leftPad, fieldWidth);
                    }
                    break;

                case 'd':
                    ret += printSignedNumber(putch, bufptr, va_arg(ap, int), leftPad, zeroPad, fieldWidth);
                    break;

                case 'u':
                    ret += printNumber(putch, bufptr, va_arg(ap, dword), leftPad, zeroPad, fieldWidth);
                    break;

                case 'x':
                    ret += printHex(putch, bufptr, va_arg(ap, dword), 8);
                    break;

                case 'w':
                    ret += printHex(putch, bufptr, va_arg(ap, int), 4);
                    break;

                case 'b':
                    ret += printHex(putch, bufptr, va_arg(ap, int), 2);
                    break;

                case 'c':
                    putch(bufptr, (char)va_arg(ap, int));
                    ++ret;
                    break;

                case 'p':
                    putch(bufptr, '0');
                    putch(bufptr, 'x');
                    ret += 2;
                    ret += printHex(putch, bufptr, va_arg(ap, dword), 8);
                    break;
            }
        }
        else {
            putch(bufptr, *p);
            ++ret;
        }
    }
    return ret;
}

