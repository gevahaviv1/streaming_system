#include "RSUser.h"
#include "RecommenderSystem.h"
#include <utility>

RSUser::RSUser (const std::string &_user_name, const rank_map &_rank,
                const std::shared_ptr<RecommenderSystem> &_rs)
{
  this->_user_name = _user_name;
  this->_rank = _rank;
  this->_rs = _rs;
}

const std::string &RSUser::get_name () const
{
  return this->_user_name;
}

void RSUser::add_movie_to_rs (const std::string &name, int year,
                              const std::vector<double> &features, double rate)
{
  sp_movie movie = this->_rs->add_movie (name, year, features);
  this->_rank.insert (std::make_pair (movie, rate));
}

const rank_map &RSUser::get_ranks () const
{
  return this->_rank;
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return this->_rs->get_recommend_by_content (*this);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return this->_rs->recommend_by_cf (*this, k);;
}

double RSUser::get_prediction_score_for_movie (const std::string &name,
                                               int year, int k) const
{
  return this->_rs->predict_movie_score (*this, this->_rs->get_movie
                                                    (name, year), k);
}

std::ostream &operator<< (std::ostream &os, const RSUser &rs_user)
{
  os << rs_user.get_name () << "\n";
  for (const auto &it: rs_user._rank)
  { os << it.first->get_name () << " " << it.second << "\n"; }
  os << std::endl;
  return os;
}