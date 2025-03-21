#pragma once

#include <stdio.h>
#include "Card.hpp"
#include <memory>
#include "CardReader.hpp"

class CardFactory
{
public:
	CardFactory();
	~CardFactory();

	static std::shared_ptr<Card> createCard(const std::shared_ptr<CardReader>& reader);
};
