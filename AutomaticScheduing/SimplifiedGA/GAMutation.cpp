#include "GAFrame.h"
#include <random>

namespace GA
{
	namespace Mutation
	{
		std::vector<Solution> run(const std::vector<SolutionWithScore> &pairs, const Setting &setting)
		{
			std::vector<Solution> ret;
			double thisRate(getCurrIterMutationRate(setting));

			for (const SolutionWithScore &pair : pairs)
			{
				Solution afterMutate(mutate(pair.first, thisRate, setting));
				if (!afterMutate.empty())
				{
					ret.push_back(std::move(afterMutate));
				}
			}

			return std::move(ret);
		}

		Solution mutate(const Solution &solt, const double rate, const Setting &setting)
		{
			static std::random_device rd;
			static std::mt19937_64 gen(rd());

			std::uniform_int_distribution<> dis(setting.range.first, setting.range.second - 1);
			std::vector<unsigned int> poses(Operator::Gaussian(solt.size(), rate));

			if (poses.empty())
			{
				return Solution();
			}

			Solution ret(solt);
			for (const unsigned int pos : poses)
			{
				ret[pos] = dis(gen);
			}
			return std::move(ret);
		}

		double getCurrIterMutationRate(const Setting &setting)
		{
			currMutationRate = pow(setting.iter / k, b / setting.iter) * currMutationRate;
			return currMutationRate / 100.0;
		}

		namespace Operator
		{
			std::vector<unsigned int> Gaussian(const unsigned int size, const double rate)
			{
				static std::random_device rd;
				static std::mt19937_64 gen(rd());
				static std::normal_distribution<> dis(5, 2);

				std::vector<unsigned int> ret;
				for (unsigned int i(0); i != size; ++i)
				{
					if (dis(gen) / 10.0 < rate)
					{
						ret.push_back(i);
					}
				}

				return std::move(ret);
			}
		};
	};
};