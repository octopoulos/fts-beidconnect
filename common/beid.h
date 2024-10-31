#pragma once

/// Read card data
/// @param idFlags: 1=>511 for normal operations, &1024: just get readers, &2048: just check if a card was inserted
std::string ReadCardData(int idFlags = 511);
