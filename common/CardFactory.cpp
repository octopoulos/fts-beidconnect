#include "CardFactory.hpp"
#include "Card.hpp"
#include "BEIDCard.hpp"
#include "VirtualCard.hpp"
#include "CardReader.hpp"

/******************************************************************************************************
 **        http://smartcard-atr.appspot.com/parse?ATR=3BD218008131FE58C90316                        ***
 ******************************************************************************************************
 **  https://ludovicrousseau.blogspot.be/2015/01/os-x-yosemite-bug-t0-is-used-instead-of.html       ***
 **  Even if T1 is chosen (sierra) further cmds on cardos fail if connect is done with T0 | T1 so   ***
 **  explicitly select T=1 for Cardos when calling SCardConnect                                     ***
 ******************************************************************************************************
 ******************************************************************************************************
 ******************************************************************************************************/


#define CARD_TYPE_TABLE { \
{ "virtualcard",                          "VirtualCard",	   		CARD_TYPE_VIRTUALCARD,  PROTOCOL_T0 },   \
{ "3B9813400AA503010101AD1311",           "BEID",						CARD_TYPE_BEID,         PROTOCOL_T0 },   \
{ "3B9894400AA503010101AD1310",           "BEID",						CARD_TYPE_BEID,         PROTOCOL_T0 },   \
{ "3B989440FFA503010101AD1310",				"BEID",						CARD_TYPE_BEID,         PROTOCOL_T0 },   \
{ "3B989540FFD000480101AD1321",				"BEID",						CARD_TYPE_BEID,         PROTOCOL_T0 },   \
{ "3B9895400AA507010101AD1320",				"BEID",						CARD_TYPE_BEID,         PROTOCOL_T0 },   \
{ "3B9813402AD000480101AD1311",   			"BEID",						CARD_TYPE_BEID,         PROTOCOL_T0 },   \
{ "3BF2180002C10A31FE58C80874",				"Cardos 4.3B",             CARD_TYPE_CARDOS,       PROTOCOL_T1 },   \
{ "3BF2180002C10A31FE58C80B77",           "Cardos 4.2C",             CARD_TYPE_CARDOS,       PROTOCOL_T1 },   \
{ "3BD21802C10A31FE58C80D51",             "Cardos 4.4",              CARD_TYPE_CARDOS44,     PROTOCOL_T1 },   \
{ "3BD21802C10A31FE58C80E52",             "Cardos 4.4",              CARD_TYPE_CARDOS44,     PROTOCOL_T1 },   \
{ "3BD218008131FE58C90316",               "PKCS15",                  CARD_TYPE_PKCS15,       PROTOCOL_T1 },   \
{ "pkcs11",                               "PKCS11",              CARD_TYPE_PKCS11,       PROTOCOL_T1 },   \
NULL \
};


#define PROTOCOL_T0             1
#define PROTOCOL_T1             2

typedef struct CARD_TYPE
{
   const char* atr;
   const char* label;
   int   type;
   int   protocol;
} CARD_TYPE;

std::shared_ptr<Card>  CardFactory::createCard(CardReader::Ptr reader)
{
   CARD_TYPE *cardtype;
   CARD_TYPE cardTypeTable[] = CARD_TYPE_TABLE;
   int i = 0;
   std::string atr = reader->atr;
   std::shared_ptr<Card> card;

   for (i=0; (cardtype = &cardTypeTable[i]) && cardtype->atr; i++)
	  {
        if (atr.compare(cardtype->atr) == 0)
        {
           //the cardTable can contain different Card implementations classes for the same ATR we found
           //we have a match in ATR, but this does NOT necessarily mean that the class linked to this cardtype can handle this card correctly
           //so we have to check if this class supports this card, of not we continue in the list and return -1 "unknown" if non of the classes linked to this ATR can handle this card
           //special case: a card can be recognized as being a pkcs15 card according to its ATR, but when the card does not have a pkcs15 structure, it might be a PKCS11 card that is handled by a pkcs11-reader (slot)
           //so in that case we return -1 "unknown" for this reader, but the same reader listed as pkcs11 reader might handle this card later on.
           switch (cardtype->type) {
              case CARD_TYPE_BEID:
              {
                 card = std::make_shared<BEIDCard>(reader);
                 if (!(card->isCardSupported()))
                    continue;
                 return card;
              }
              case CARD_TYPE_PKCS15:
              {
                 return nullptr;
              }
              case CARD_TYPE_PKCS11:
              {
                 return nullptr;
              }
              case CARD_TYPE_VIRTUALCARD:
              {
                 card = std::make_shared<VirtualCard>(reader);
                 if (!(card->isCardSupported()))
                    continue;
                 return card;
              }
              default:
                 return nullptr;
           }
        }
     }
   
   return (nullptr);
}