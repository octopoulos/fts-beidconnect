#pragma once

#include <iostream>
#include "RequestHandler.hpp"

class VersionRequestHandler : public RequestHandler
{
public:
	VersionRequestHandler(const std::shared_ptr<boost::property_tree::ptree>& ptreeRequest)
	    : RequestHandler(ptreeRequest) {};
	virtual ~VersionRequestHandler() {}; // without this destructor of derived handler is not called
	std::string process() override;
};
