//
// Created by Geva on 03/01/2023.
//

#include "RecommenderSystem.h"
#include <algorithm>
#include <cmath>
#include <numeric>

RecommenderSystem::RecommenderSystem ()
{}

sp_movie RecommenderSystem::add_movie (const std::string &name, int year, const
std::vector<double> &features)
{
  sp_movie movie = std::make_shared<Movie> (name, year);
  RecommenderSystem::system[movie] = features;
  return movie;
}

double RecommenderSystem::get_avg (const rank_map &rank_vector)
{
  double avg_vec = 0;
  int i = 0;
  for (const auto &it: rank_vector)
  {
    if (it.second != 0)
    {
      avg_vec += it.second;
      i++;
    }
  }
  return avg_vec / i;
}

double RecommenderSystem::get_imagination (const std::vector<double> &vec_1,
                                           const std::vector<double> &vec_2)
{
  double numerator = 0;
  for (std::size_t i = 0; i < vec_1.size (); ++i)
  { numerator += (vec_1[i] * vec_2[i]); }

  double euclidean_norm_1 = sqrt (std::inner_product (vec_1.begin (),
                                                      vec_1.end (),
                                                      vec_1.begin (),
                                                      0.0));
  double euclidean_norm_2 = sqrt (std::inner_product (vec_2.begin (),
                                                      vec_2.end (),
                                                      vec_2.begin (),
                                                      0.0));
  double denominator = euclidean_norm_1 * euclidean_norm_2;
  return numerator / denominator;
}

std::vector<double> RecommenderSystem::get_priorities_vec (rank_map
                                          rank_vector, double vec_avg)
{
  std::vector<double> priorities_vec;
  rank_map::iterator priorities_vec_size;
  priorities_vec_size = rank_vector.begin();
  priorities_vec.resize (this->system[priorities_vec_size->first].size());
  std::vector<double> temp;

  for (auto &it: rank_vector)
  {
    if (it.second != 0)
    {
      it.second -= vec_avg;
      temp = this->system[it.first];
      std::transform (temp.begin (), temp.end (), temp.begin (),
                      [&it] (double &val)
                      { return it.second * val; });
      for (size_t i = 0; i < temp.size(); ++i)
      {
        priorities_vec[i] += temp[i];
      }
    }
  }
  return priorities_vec;
}

sp_movie RecommenderSystem::get_recommend_by_content (const RSUser &user)
{
  // Stage one.
  double vec_avg = this->get_avg (user.get_ranks ());

  // Stage two.
  std::vector<double> priorities_vec = this->get_priorities_vec (
                                      user.get_ranks (), vec_avg);

  // Stage three.
  double imagination;
  std::map<sp_movie, double> unseen_movies;
  for (const auto &it: user.get_ranks ())
  {
    if (it.second == 0)
    {
      imagination = this->get_imagination (priorities_vec, system[it.first]);
      unseen_movies[it.first] = imagination;
    }
  }
  auto get_movie = std::max_element (unseen_movies.begin (), unseen_movies.end (), [] (const auto
                                                            &x, const auto &y)
                                                {return x.second < y.second;});
  return get_movie->first;
}

bool
RecommenderSystem::compare_by_val (const std::pair<sp_movie, double> &p_1,
                                   const std::pair<sp_movie, double> &p_2)
{
  return p_1.second < p_2.second;
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  double score;
  std::map<sp_movie, double> scores_map;
  for (const auto &it: user.get_ranks ())
  {
    if (it.second == 0)
    {
      score = predict_movie_score (user, it.first, k);
      scores_map[it.first] = score;
    }
  }

  auto p = std::max_element (scores_map.begin (), scores_map.end (),
                                                    compare_by_val);
  return p->first;
}

double RecommenderSystem::predict_movie_score (const RSUser &user, const
sp_movie &movie, int k)
{
  std::map<sp_movie, double> imagination_map;
  double imagination;
  for (const auto &it: user.get_ranks ())
  {
    if (it.second != 0)
    {
      imagination = this->get_imagination (system[movie], system[it.first]);
      imagination_map[it.first] = imagination;
    }
  }

  double numerator = 0, denominator = 0;
  for (int i = 0; i < k; ++i)
  {
    auto p = std::max_element (imagination_map.begin (),
                               imagination_map.end (),
                               compare_by_val);
    auto user_ranks = user.get_ranks();
    numerator += (p->second * user_ranks[p->first]);
    denominator += p->second;
    imagination_map.erase (p->first);
  }

  return numerator / denominator;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  auto it = system.find (std::make_shared<Movie> (name, year));
  if (it == system.end ())
  { return nullptr; }
  return it->first;
}

std::ostream &operator<< (std::ostream &stream, const RecommenderSystem &data)
{
  for (const auto &it: data.system)
  { stream << it.first->get_name () << "\n"; }
  stream << std::endl;
  return stream;
}