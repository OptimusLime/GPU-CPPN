//| This file is a part of the sferes2 framework.
//| Copyright 2009, ISIR / Universite Pierre et Marie Curie (UPMC)
//| Main contributor(s): Jean-Baptiste Mouret, mouret@isir.fr
//|
//| This software is a computer program whose purpose is to facilitate
//| experiments in evolutionary computation and evolutionary robotics.
//|
//| This software is governed by the CeCILL license under French law
//| and abiding by the rules of distribution of free software.  You
//| can use, modify and/ or redistribute the software under the terms
//| of the CeCILL license as circulated by CEA, CNRS and INRIA at the
//| following URL "http://www.cecill.info".
//|
//| As a counterpart to the access to the source code and rights to
//| copy, modify and redistribute granted by the license, users are
//| provided only with a limited warranty and the software's author,
//| the holder of the economic rights, and the successive licensors
//| have only limited liability.
//|
//| In this respect, the user's attention is drawn to the risks
//| associated with loading, using, modifying and/or developing or
//| reproducing the software by the user in light of its specific
//| status of free software, that may mean that it is complicated to
//| manipulate, and that also therefore means that it is reserved for
//| developers and experienced professionals having in-depth computer
//| knowledge. Users are therefore encouraged to load and test the
//| software's suitability as regards their requirements in conditions
//| enabling the security of their systems and/or data to be ensured
//| and, more generally, to use and operate it in the same conditions
//| as regards security.
//|
//| The fact that you are presently reading this means that you have
//| had knowledge of the CeCILL license and that you accept its terms.

#ifndef FIT_EMPTY_HPP
#define FIT_EMPTY_HPP

#include <modules/map_elite/fit_map.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>

// Headers specifics to the computations we need
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/max.hpp>


#define FIT_EMPTY(Name) SFERES_FITNESS(Name, sferes::fit::Fitness)

namespace sferes
{
  namespace fit
  {
    SFERES_FITNESS(FitEmpty, sferes::fit::Fitness)
    {
    	
    private:
    	void _setProbabilityList()
			{
				std::vector<double> values;

				boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::max> > max;

				// Clear the probability in case it is called twice
				_prob.clear();

				for(int i = 0; i < Params::image::num_categories; ++i)
					{
						float v = i;

						// Push 1000 probabilities in the list
						_prob.push_back(v);

						max(v);	// Add this mean to a list for computing the max later
					}

				float max_prob = boost::accumulators::max(max);

				// Set the fitness
				this->_value = max_prob;
			}

      public:
    	FitEmpty() : _prob(Params::image::num_categories) { }
			const std::vector<float>& desc() const { return _prob; }

			// Indiv will have the type defined in the main (phen_t)
			template<typename Indiv>
			void eval(const Indiv& ind)
			{
				_setProbabilityList();
			}

			float value(int category) const
			{
				assert(category < _prob.size());
				return _prob[category];
			}

			float value() const
			{
				return this->_value;
			}

			template<class Archive>
			void serialize(Archive & ar, const unsigned int version) {
				sferes::fit::Fitness<Params,  typename stc::FindExact<FitEmpty<Params, Exact>, Exact>::ret>::serialize(ar, version);
				ar & BOOST_SERIALIZATION_NVP(_prob);
			}

      protected:
			  std::vector<float> _prob; // List of probabilities
    };
  }
}

#endif
