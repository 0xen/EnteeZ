#pragma once

namespace enteez
{
	typedef unsigned __int8 ui8;
	class BitHandler
	{
	public:
		BitHandler();
		~BitHandler();
		void Resize(unsigned int bit_size);
		void SetBit(unsigned int bit);
		void ClearBit(unsigned int bit);
		bool TestBit(unsigned int bit);
		bool Contains(const BitHandler& bm);
		void Reset();
		bool operator== (const BitHandler& bm) const
		{
			// If one of the sets have a larger "max bit" then the other, instantly they are different
			if (m_max_used_bit != bm.m_max_used_bit) return false;
			// Loop through for the smallest amount of data
			for (int i = 0; i < (m_array_size > bm.m_array_size ? bm.m_array_size : m_array_size); i++)
			{
				if (m_data[i] != bm.m_data[i]) return false;
			}
			return true;
		}
	private:
		void Reset(ui8* data, int size);
		ui8* m_data = nullptr;
		unsigned int m_max_used_bit = 0;
		unsigned int m_array_size = 0;
		unsigned int m_max_bits = 0;
	};
}