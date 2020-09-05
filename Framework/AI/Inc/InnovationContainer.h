#pragma once

#include "Genome.h"

namespace Angazi::AI::NEAT
{
	struct Gene;

	class InnovationContainer
	{
	public:

		void Reset();
		size_t AddGene(const Gene& gene);
		size_t Number() const { return mInnovatioNumber; }

	private:
		friend class Population;

		void SetInnovationNumber(size_t num) 
		{
			mInnovatioNumber = num;
			Reset();
		}

		std::map<std::pair<size_t, size_t>, size_t> mHistory;
		size_t mInnovatioNumber = 0;
	};
}
