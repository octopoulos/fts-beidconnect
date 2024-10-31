#pragma once

#include "CardReader.hpp"
#include <iostream>
#include <vector>
#include <memory>

#include <stdio.h>

typedef int type;

class ReaderList
{
public:
	ReaderList() { listReaders(); }

	virtual ~ReaderList();
	std::shared_ptr<CardReader>         getReaderByName(string readername);
	std::shared_ptr<CardReader>         getReaderByIndex(unsigned int index);
	std::shared_ptr<CardReader>         getFirstReaderWithSupportedCardType(int type[], int size);
	vector<std::shared_ptr<CardReader>> readers;

private:
	void                        listReaders();
	std::shared_ptr<CardReader> returnFirstReaderWithSupportedCardType(int type[], int size);
};
