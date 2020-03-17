#include "RequestHandler.hpp"
#include "VersionRequestHandler.hpp"
#include "InfoRequestHandler.hpp"
#include "UserCertsRequestHandler.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "log.hpp"

using namespace boost::property_tree;


std::shared_ptr<RequestHandler> RequestHandler::createRequestHandler(std::shared_ptr <std::stringstream> ssRequest)
{
   log_info(ssRequest->str().c_str());
   //XXXXXX handle security related checks here
   boost::property_tree::ptree pt;
   boost::property_tree::read_json(*ssRequest, pt);

   std::shared_ptr<RequestHandler> requestHandler = nullptr;
   std::string operation = pt.get<std::string>("operation");

   if (operation == "VERSION") {
      requestHandler = std::make_shared<VersionRequestHandler>();
   }
   else if (operation == "INFO") {
      requestHandler = std::make_shared<InfoRequestHandler>();
   }
   else if (operation == "USERCERTS") {
      requestHandler = std::make_shared<UserCertsRequestHandler>();
   }
   else {
        log_error("Unknown operation <%s>", operation.c_str());
    }

   requestHandler->ssRequest = ssRequest;
   
   return (requestHandler);
}

void RequestHandler::post_process(boost::property_tree::ptree &response)
{
   if ( ssRequest) {
      std::stringstream ss(ssRequest->str());
      boost::property_tree::ptree pt;
      boost::property_tree::read_json(ss, pt);
      std::string correlationId = pt.get<std::string>("correlationId");

      if (correlationId != "") {
         response.put("correlationId", correlationId);
      }
   }
}
