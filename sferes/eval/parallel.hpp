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




#ifndef EVAL_PARALLEL_HPP_
#define EVAL_PARALLEL_HPP_

#include <sferes/parallel.hpp>
#include<cmath>
#include <sferes/eval/eval.hpp>

namespace sferes {

  namespace eval {
    template<typename Phen>
    struct _parallel_evaluate {
      typedef std::vector<boost::shared_ptr<Phen> > pop_t;
      typedef typename Phen::fit_t fit_t;
      pop_t _pop;
      fit_t _fit;

      ~_parallel_evaluate() { }
      _parallel_evaluate(pop_t& pop, const fit_t& fit) : _pop(pop), _fit(fit) {}
      _parallel_evaluate(const _parallel_evaluate& ev) : _pop(ev._pop), _fit(ev._fit) {}
      void operator() (const parallel::range_t& r) const {
        for (size_t i = r.begin(); i != r.end(); ++i) {
          assert(i < _pop.size());
          _pop[i]->fit() = _fit;
          _pop[i]->develop();
          _pop[i]->fit().eval(*_pop[i]);
          for (size_t j = 0; j < _pop[i]->fit().objs().size(); ++j) {
            assert(!std::isnan(_pop[i]->fit().objs()[j]));
          }
        }
      }
    };

    SFERES_CLASS(Parallel) {
    public:
      template<typename Phen>
      void eval(std::vector<boost::shared_ptr<Phen> >& pop, size_t begin, size_t end,
                const typename Phen::fit_t& fit_proto) {
        dbg::trace trace("eval", DBG_HERE);
        assert(pop.size());
        assert(begin < pop.size());
        assert(end <= pop.size());
        parallel::init();
        parallel::p_for(parallel::range_t(begin, end),
                        _parallel_evaluate<Phen>(pop, fit_proto));
      }

    };

  }
}

#endif
