#include <stdarg.h>
#include <string.h>

static int bufs(char *buffer, const char *str)
{
	int n = 0;
	while (*str) {
		buffer[n++] = *str++;
	}
	return n;
}

static int bufsp(char *buffer, const char *str, int pad, char padc)
{
	int n = 0;
	int len = strlen(str);
	while (pad > len) {
		buffer[n++] = padc;
		pad--;
	}
	return n + bufs(buffer + n, str);
}

int numfmt(char *buffer, signed long long num, int base, int pad, char pc)
{
	static char digits[] = "0123456789ABCDEFGHIJKLMOPQRSTUVWXYZ";

	int n = 0;
	char tmp[65];
	char *end = tmp + 64;

	if (base < 2 || base > 36)
		return 0;

	*end-- = 0;

	do {
		int rem = num % base;
		num = num / base;
		*end-- = digits[rem];
		pad--;
	} while (num > 0);

	while (pad-- > 0) {
		buffer[n++] = pc;
	}

	while (*(++end)) {
		buffer[n++] = *end;
	}

	buffer[n] = '\0';
	return n;
}

int strafmt(char *buffer, const char *fmt, va_list args)
{
	int  n = 0;
	signed long long  val = 0;
	int  pad = 0;
	char padc = ' ';

	while (*fmt) {
		if (*fmt == '%') {
			fmt++;

			pad = 0;
			if (*fmt == '0') {
				padc = '0';
				fmt++;
			}
			else {
				padc = ' ';
			}

			while (*fmt >= '0' && *fmt <= '9') {
				pad = pad * 10 + *fmt++ - '0';
			}

			if (*fmt == 'd' || *fmt == 'u') {
				val = va_arg(args, int);
				if (val < 0) {
					buffer[n++] = '-';
					pad--;
					val = -val;
				}
			}
			else if (*fmt == 'i' || *fmt == 'o' ||
			         *fmt == 'p' || *fmt == 'x' ||
			         *fmt == 'b')
			{
				val = va_arg(args, unsigned int);
				val = val  & 0xFFFFFFFF;
			}


			switch (*fmt) {
			case 'c':
				buffer[n++] = (char)va_arg(args, int);
				break;

			case 'b':
				n += numfmt(&buffer[n], val, 2, pad, padc);
				break;

			case 'd':
			case 'i':
			case 'u':
				n += numfmt(&buffer[n], val, 10, pad, padc);
				break;

			case 'o':
				n += numfmt(&buffer[n], val, 8, pad, padc);
				break;

			case 'p':
				padc = '0';
				pad  = 8;
				n += bufs(&buffer[n], "0x");
			case 'x':
				n += numfmt(&buffer[n], val, 16, pad, padc);
				break;

			case 's':
				n += bufsp(&buffer[n], va_arg(args, char*), pad, padc);
				break;

			case '%':
				buffer[n++] = '%';
				break;

			default:
				buffer[n++] = '%';
				buffer[n++] = *fmt;
				break;
			}
			fmt++;
		}
		else {
			buffer[n++] = *fmt++;
		}
	}

	buffer[n] = '\0';
	return n;
}

int strfmt(char *buffer, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int n = strafmt(buffer, fmt, args);
	va_end(args);
	return n;
}

#ifdef TEST

int main(int argc, char **argv)
{
	char buffer[512] = {0};

	strfmt(buffer, "%p %p 0x%x", 0xFFFFF000, 0xD00FD00F, 0xDEADBEEF);

	printf(buffer);
}

#endif
