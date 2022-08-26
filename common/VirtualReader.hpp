#ifndef VirtualReader_hpp
#define VirtualReader_hpp

#include "CardReader.hpp"
#include <vector>
#include <memory>
#ifdef _WIN32
#include "windows.h"
#endif


class VirtualReader: public CardReader
{
   long beginTransaction() override { return 0; };
   long endTransaction() override{ return 0; };
public:
   VirtualReader();
   virtual ~VirtualReader();
   static int listReaders(std::vector<std::shared_ptr<CardReader>>& readers);
   long connect() override;
   long disconnect() override;
   bool isPinPad() override{ return 0; };
   long apdu(const unsigned char* apdu, unsigned int l_apdu, unsigned char* out, int* l_out, int* sw) override { return -1; };
   long verify_pinpad(unsigned char format, unsigned char PINBlock, unsigned char PINLength, unsigned int PINMaxExtraDigit, unsigned char pinAPDU[], int l_pinAPDU, int *sw)override { return -1; };
};

#endif /* VirtualReader_hpp */
