#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include <string>
#include <vector>

const unsigned char pr2six[256] = {
	/* ASCII table */
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
	64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

void do_sleep(int msec)
{
#ifdef _WIN32
	Sleep(msec);
#else
	usleep(1000 * msec);
#endif
}

int isEqual(const char* str1, int size1, const char* str2, int size2)
{
	if (size1 != size2)
		return 0;
	if (strncmp(str1, str2, size1) == 0)
		return 1;
	else
		return 0;
}

// void x_bin2hex(const unsigned char* p_bin, int l_bin, char* p_hex)
//{
//     char hexchars[] = "0123456789ABCDEF";
//     int byte;
//
//     for (byte = 0; byte < l_bin; byte++)
//     {
//         p_hex[2 * byte] = hexchars[(p_bin[byte] >> 4) & 0x0F];
//         p_hex[2 * byte + 1] = hexchars[(p_bin[byte] & 0xF)];
//     }
//
//     p_hex[2 * l_bin] = 0;
// }

// char* hex_dump(const unsigned char* p_bin, int l_bin)
//{
//     char hexchars[] = "0123456789ABCDEF";
//     int byte;
//     static char p_hex[120];
//     int size = l_bin;
//
//     if (size > 20)
//         size = 20;
//
//     for (byte = 0; byte < size; byte++)
//     {
//         p_hex[3 * byte] = hexchars[(p_bin[byte] >> 4) & 0x0F];
//         p_hex[3 * byte + 1] = hexchars[(p_bin[byte] & 0xF)];
//         p_hex[3 * byte + 2] = 0x20;
//     }
//
//     p_hex[3 * size] = 0;
//     return p_hex;
// }

// char* ascii_dump(const unsigned char* p_bin, int l_bin)
//{
//     const int MAX_STRING = 40;
//     static char p_ascii[MAX_STRING + 1];
//     int size = l_bin;
//
//     if (size > MAX_STRING)
//         size = MAX_STRING;
//
//     memset(p_ascii, 0, sizeof(p_ascii));
//     memcpy(p_ascii, p_bin, size);
//
//     return p_ascii;
// }

// void x_bin2hex_(const unsigned char* p_bin, int l_bin, char* p_hex)
//{
//     char hexchars[] = "0123456789ABCDEF";
//     int byte;
//
//     for (byte = 0; byte < l_bin; byte++)
//     {
//         p_hex[3 * byte] = hexchars[(p_bin[byte] >> 4) & 0x0F];
//         p_hex[3 * byte + 1] = hexchars[(p_bin[byte] & 0xF)];
//         p_hex[3 * byte + 2] = ' ';
//     }
//
//     p_hex[3 * l_bin] = 0;
// }

size_t base64decode_len(const std::string& bufcoded)
{
	size_t               nbytesdecoded;
	const unsigned char* bufin;
	size_t               nprbytes;

	bufin = (const unsigned char*)bufcoded.c_str();
	while (pr2six[*(bufin++)] <= 63)
		;

	nprbytes      = (int)(bufin - (const unsigned char*)bufcoded.c_str()) - 1;
	nbytesdecoded = ((nprbytes + 3) / 4) * 3;

	return nbytesdecoded + 1;
}

size_t base64decode(const std::string& bufcoded, unsigned char* bufplain)
{
	size_t               nbytesdecoded;
	const unsigned char* bufin;
	unsigned char*       bufout;
	size_t               nprbytes;

	bufin = (const unsigned char*)bufcoded.c_str();
	while (pr2six[*(bufin++)] <= 63)
		;
	nprbytes      = (int)(bufin - (const unsigned char*)bufcoded.c_str()) - 1;
	nbytesdecoded = ((nprbytes + 3) / 4) * 3;

	bufout = (unsigned char*)bufplain;
	bufin  = (const unsigned char*)bufcoded.c_str();

	while (nprbytes > 4)
	{
		*(bufout++) =
		    (unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
		*(bufout++) =
		    (unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
		*(bufout++) =
		    (unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
		bufin += 4;
		nprbytes -= 4;
	}

	/* Note: (nprbytes == 1) would be an error, so just ingore that case */
	if (nprbytes > 1)
		*(bufout++) =
		    (unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
	if (nprbytes > 2)
		*(bufout++) =
		    (unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
	if (nprbytes > 3)
		*(bufout++) =
		    (unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);

	*(bufout++) = '\0';
	nbytesdecoded -= (4 - nprbytes) & 3;
	return nbytesdecoded;
}

size_t base64encode_len(size_t len)
{
	return ((len + 2) / 3 * 4);
}

size_t base64encode(const unsigned char* string, size_t len, unsigned char* encoded)
{
	size_t              i;
	unsigned char*      p;
	const unsigned char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	p = encoded;
	for (i = 0; i < len - 2; i += 3)
	{
		*p++ = basis_64[(string[i] >> 2) & 0x3F];
		*p++ = basis_64[((string[i] & 0x3) << 4) | ((int)(string[i + 1] & 0xF0) >> 4)];
		*p++ = basis_64[((string[i + 1] & 0xF) << 2) | ((int)(string[i + 2] & 0xC0) >> 6)];
		*p++ = basis_64[string[i + 2] & 0x3F];
	}
	if (i < len)
	{
		*p++ = basis_64[(string[i] >> 2) & 0x3F];
		if (i == (len - 1))
		{
			*p++ = basis_64[((string[i] & 0x3) << 4)];
			*p++ = '=';
		}
		else
		{
			*p++ = basis_64[((string[i] & 0x3) << 4) | ((int)(string[i + 1] & 0xF0) >> 4)];
			*p++ = basis_64[((string[i + 1] & 0xF) << 2)];
		}
		*p++ = '=';
	}

	return (p - encoded);
}

std::string rawToBase64(const std::vector<unsigned char>& raw)
{
	size_t              i;
	const unsigned char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string         base64str;

	for (i = 0; i < raw.size() - 2; i += 3)
	{
		base64str.push_back(basis_64[(raw[i] >> 2) & 0x3F]);
		base64str.push_back(basis_64[((raw[i] & 0x3) << 4) | ((int)(raw[i + 1] & 0xF0) >> 4)]);
		base64str.push_back(basis_64[((raw[i + 1] & 0xF) << 2) | ((int)(raw[i + 2] & 0xC0) >> 6)]);
		base64str.push_back(basis_64[raw[i + 2] & 0x3F]);
	}
	if (i < raw.size())
	{
		base64str.push_back(basis_64[(raw[i] >> 2) & 0x3F]);
		if (i == (raw.size() - 1))
		{
			base64str.push_back(basis_64[((raw[i] & 0x3) << 4)]);
			base64str.push_back('=');
		}
		else
		{
			base64str.push_back(basis_64[((raw[i] & 0x3) << 4) | ((int)(raw[i + 1] & 0xF0) >> 4)]);
			base64str.push_back(basis_64[((raw[i + 1] & 0xF) << 2)]);
		}
		base64str.push_back('=');
	}

	return base64str;
}

const std::vector<unsigned char> base64ToRaw(const std::string& bufcoded)
{
	// int nbytesdecoded;
	const unsigned char*       bufin;
	std::vector<unsigned char> bufout;
	int                        nprbytes;

	bufin = (const unsigned char*)bufcoded.c_str();
	while (pr2six[*(bufin++)] <= 63)
		;
	nprbytes = (int)(bufin - (const unsigned char*)bufcoded.c_str()) - 1;
	// nbytesdecoded = ((nprbytes + 3) / 4) * 3;

	bufin = (const unsigned char*)bufcoded.c_str();

	while (nprbytes > 4)
	{
		bufout.push_back((unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4));
		bufout.push_back((unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2));
		bufout.push_back((unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]));
		bufin += 4;
		nprbytes -= 4;
	}

	/* Note: (nprbytes == 1) would be an error, so just ingore that case */
	if (nprbytes > 1)
		bufout.push_back((unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4));
	if (nprbytes > 2)
		bufout.push_back((unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2));
	if (nprbytes > 3)
		bufout.push_back((unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]));

	return bufout;
}
