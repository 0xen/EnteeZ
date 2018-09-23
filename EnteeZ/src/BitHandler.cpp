#include <EnteeZ/BitHandler.hpp>
#include <iostream>

using namespace enteez;

enteez::BitHandler::BitHandler()
{
	m_array_size = 1;
	m_data = new ui8[m_array_size];
	m_max_bits = 8;
	Reset();
}

enteez::BitHandler::~BitHandler()
{
	delete[] m_data;
}

void enteez::BitHandler::Resize(unsigned int bit_size)
{
	// Create a new bitset based on the new size + 1 
	unsigned int new_array_size = (bit_size / 8) + 1;
	// Calculate one the max amount of bits that can be stored
	unsigned int new_max_bits = new_array_size * 8;
	ui8* new_set = new ui8[new_array_size];
	Reset(new_set, new_array_size);
	// See if we are increasing 
	bool increasing_size = new_array_size > m_array_size;
	// If the new array is larger, copy all old data over, if the new array is smaller,
	// only copy enough data to fill the new data and remove the old stuff
	memcpy(new_set, m_data, increasing_size ? m_array_size : new_array_size);

	if (!increasing_size)
	{
		// If we are removing some data from the end of the set, we need to assign 'm_max_used_bit' to the new max
		if (new_max_bits <= m_max_used_bit)
		{
			bool found = false;
			for (int i = m_max_used_bit - 1; i >= 0; i--)
			{
				if (TestBit(i))
				{
					found = true;
					m_max_used_bit = i;
					break;
				}
			}
			if (!found) m_max_used_bit = 0;
		}
	}

	delete[] m_data;

	// Replace the old data with the new data
	m_array_size = new_array_size;
	m_max_bits = new_max_bits;
	m_data = new_set;
}

void enteez::BitHandler::SetBit(unsigned int bit)
{
	if (bit >= m_max_bits)
	{
		Resize(bit);
	}
	unsigned int index = bit / 8;
	unsigned int index_bit = bit % 8;
	m_data[index] = m_data[index] | (1 << index_bit);
	if (m_max_used_bit < bit) m_max_used_bit = bit;
}

void enteez::BitHandler::ClearBit(unsigned int bit)
{
	unsigned int index = bit / 8;
	unsigned int index_bit = bit % 8;
	// Reset the bit
	m_data[index] = m_data[index] & ~(1 << index_bit);
	// If this was the last largest bit, find the new largest
	if (m_max_used_bit == bit)
	{
		for (int i = m_max_used_bit - 1; i >= 0; i--)
		{
			if (TestBit(i))
			{
				m_max_used_bit = i;
				return;
			}
		}
		m_max_used_bit = 0;
	}
}

bool enteez::BitHandler::TestBit(unsigned int bit)
{
	unsigned int index = bit / 8;
	unsigned int index_bit = bit % 8;
	return (m_data[index] >> index_bit) & 1;
}

bool enteez::BitHandler::Contains(const BitHandler & bm)
{
	// If the Highest bit is already out of the bounds of the set that we are going to be comparing, return
	if (bm.m_max_used_bit > m_max_used_bit) return false;

	// If the set to check againsed is larger then this one, then we need to make sure that all bits are 0, otherwise it fails
	if (bm.m_array_size > m_array_size)
	{
		for (int i = bm.m_array_size - 1; i >= m_array_size; i--)
		{
			if (bm.m_data[i] != 0x00)return false;
		}
	}
	// Loop through for the smallest amount of data
	for (int i = 0; i < (m_array_size > bm.m_array_size ? bm.m_array_size : m_array_size); i++)
	{
		if (bm.m_data[i] == 0x00) continue;
		if (bm.m_data[i] != (bm.m_data[i] & m_data[i]))return false;
	}

	return true;
}

void enteez::BitHandler::Reset()
{
	Reset(m_data, m_array_size);
}

void enteez::BitHandler::Reset(ui8 * data, int size)
{
	for (int i = 0; i < size; i++)
	{
		data[i] = 0x00;
	}
}
