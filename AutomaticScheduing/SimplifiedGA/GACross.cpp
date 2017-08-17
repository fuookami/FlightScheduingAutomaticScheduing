#include "GAFrame.h"
#include <random>
#include <algorithm>
#include <iterator>

namespace GA
{
	namespace Cross
	{
		std::vector<Solution> run(const std::vector<SolutionWithScore> &pairs, const Setting &setting)
		{
			static std::random_device rd;
			static std::mt19937_64 gen(rd());

			std::uniform_int_distribution<> udis(0, pairs.size() - 1);
			std::vector<Solution> ret;
			std::vector<std::vector<Solution>> groups(1, std::vector<Solution>());

			for (unsigned int i(0), j(pairs.size()); i != j; ++i)
			{
				groups.back().push_back(pairs[i].first);
				if (i % 4 == 3)
				{
					groups.push_back(std::vector<Solution>());
				}
			}
			for (unsigned int i(pairs.size() % 4); i != 3; ++i)
			{
				groups.back().push_back(pairs[udis(rd)].first);
			}

			for (std::vector<Solution> &group : groups)
			{
				cross(group);
				std::move(group.begin(), group.end(), std::back_inserter(ret));
			}

			return ret;
		}

		void cross(std::vector<Solution> &pairs)
		{
			std::vector<bool> shadow(Operator::MultiPoint(pairs.front().size()));

			pairs.push_back(pairs.front());

			for (unsigned int i(0), j(pairs.size() - 1); i != j; ++i)
			{
				for (unsigned int p(0), q(shadow.size()); p != q; ++p)
				{
					if (shadow[p])
					{
						pairs[i][p] = pairs[i + 1][p];
					}
				}
			}
			pairs.pop_back();

			return;
		}

		namespace Operator
		{
			std::vector<bool> MultiPoint(const unsigned int size)
			{
				static std::random_device rd;
				static std::mt19937_64 gen(rd());
				std::poisson_distribution<> dis(1);
				std::uniform_int_distribution<> udis(0, size - 1);
				std::uniform_int_distribution<> bdis(0, 1);

				unsigned int pointNum(dis(gen) + 3);
				std::vector<bool> ret(size, false);

				std::set<unsigned int> pointSet;
				while (pointSet.size() != pointNum)
				{
					pointSet.insert(udis(gen));
				}

				std::vector<unsigned int> points(pointSet.cbegin(), pointSet.cend());
				std::sort(points.begin(), points.end());

				for (unsigned int i(0), j(points.size() - 1); i != j; ++i)
				{
					if (bdis(gen) == 1)
					{
						for (unsigned int k(points[i]), l(points[i + 1]); k != l; ++k)
						{
							ret[k] = true;
						}
					}	
				}

				return ret;
			}
		};
	};
};