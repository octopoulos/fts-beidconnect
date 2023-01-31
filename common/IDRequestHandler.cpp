#include "IDRequestHandler.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include "Card.hpp"
#include "CardFactory.hpp"
#include "ReaderList.hpp"
#include "CardReader.hpp"
#include "general.h"
#include "SCardException.h"

using boost::property_tree::ptree;

#define WHERE "IDRequestHandler::process()"
std::string IDRequestHandler::process()
{
    ptree response;

    ReaderList readerList;
    size_t count = readerList.readers.size();
    int countSupportedCards = 0;
    int countUnsupportedCards = 0;

    try
    {
        std::stringstream ss(ssRequest->str());
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ss, pt);
        int idflags = stoi(pt.get<std::string>("idflags"));

        if (count == 0)
        {
            response.put("result", "no_reader");
        }
        else
        {
            long status = 0;
            ptree readerInfos;
            for (int i = 0; i < (int)count; i++)
            {

                std::shared_ptr<CardReader> reader = readerList.getReaderByIndex(i);
                if (reader->atr == "")
                {
                    continue;
                }

                status = reader->connect();
                if (status)
                {
                    log_error("%s: E: reader->connect(%s) returned %08X", WHERE, reader->name.c_str(), status);
                    continue;
                }

                std::shared_ptr<Card> card = CardFactory::createCard(reader);
                if (card == nullptr)
                {
                    countUnsupportedCards++;
                    continue; // card not supported in this reader, try next reader
                }

                countSupportedCards++;
                ptree readerInfo;
                readerInfo.put("ReaderName", reader->name);
                if (reader->isPinPad())
                {
                    readerInfo.put("ReaderType", "pinpad");
                }
                else
                {
                    readerInfo.put("ReaderType", "standard");
                }
                readerInfo.put("cardType", card->strType());

                if (idflags & ID_FLAG_INCLUDE_ID)
                {
                    readerInfo.put("id", card->getFile("id").getBase64());
                }
                if (idflags & ID_FLAG_INCLUDE_ADDR)
                {
                    readerInfo.put("addr", card->getFile("address").getBase64());
                }
                if (idflags & ID_FLAG_INCLUDE_PHOTO)
                {
                    readerInfo.put("photo", card->getFile("photo").getBase64());
                }
                if (idflags & ID_FLAG_INCLUDE_INTEGRITY)
                {
                    std::vector<char> rrncert;
                    status = card->readCertificate(FORMAT_RADIX64, CERT_TYPE_RRN, rrncert);
                    readerInfo.put("idsig", card->getFile("id_sig").getBase64());
                    readerInfo.put("addrsig", card->getFile("address_sig").getBase64());
                    readerInfo.put("rrncert", std::string(rrncert.data(), rrncert.size()));
                }
                if ((idflags & ID_FLAG_INCLUDE_AUTH_CERT) || (idflags & ID_FLAG_INCLUDE_CERTS))
                {
                    std::vector<char> buf;
                    status = card->readCertificate(FORMAT_RADIX64, CERT_TYPE_AUTH, buf);
                    readerInfo.put("authcert", std::string(buf.data(), buf.size()));
                }
                if ((idflags & ID_FLAG_INCLUDE_SIGN_CERT) || (idflags & ID_FLAG_INCLUDE_CERTS))
                {
                    std::vector<char> buf;
                    status = card->readCertificate(FORMAT_RADIX64, CERT_TYPE_NONREP, buf);
                    readerInfo.put("signcert", std::string(buf.data(), buf.size()));
                }
                if ((idflags & ID_FLAG_INCLUDE_CACERTS) || (idflags & ID_FLAG_INCLUDE_CERTS))
                {
                    std::vector<char> buf;
                    status = card->readCertificate(FORMAT_RADIX64, CERT_TYPE_CA, buf);
                    readerInfo.put("cacert", std::string(buf.data(), buf.size()));
                }
                if ((idflags & ID_FLAG_INCLUDE_ROOTCERT) || (idflags & ID_FLAG_INCLUDE_CERTS))
                {
                    std::vector<char> buf;
                    status = card->readCertificate(FORMAT_RADIX64, CERT_TYPE_ROOT, buf);
                    readerInfo.put("rootcert", std::string(buf.data(), buf.size()));
                }

                readerInfos.push_back(std::make_pair("", readerInfo));
            }
            response.add_child("Readers", readerInfos);

            if (countSupportedCards > 0)
            {
                response.put("result", "OK");
            }
            else
            {
                // errors and unsupported cards result in nop_card
                response.put("result", "no_card");

                if (countUnsupportedCards > 0)
                {
                    response.put("report", "card_type_unsupported");
                }
            }
        }
    }
    catch (SCardException& e)
    {
        response.put("result", e.result());
    }
    catch (...)
    {
        response.put("result", "general_error");
    }
    post_process(response);
    std::stringstream streamResponse;
    boost::property_tree::write_json(streamResponse, response, false);
    return streamResponse.str();
}
#undef WHERE
