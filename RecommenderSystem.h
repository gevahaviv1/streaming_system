// Created on 2/20/2022.

#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include <map>
#include <string>

struct comparator
{
    bool operator() (const sp_movie &movie_1, const sp_movie &movie_2) const
    {
      return *movie_1 < *movie_2;
    }
};

typedef std::map<sp_movie, std::vector<double>, comparator> RSData_type;

class RecommenderSystem
{
 private:
  RSData_type system;
  double get_avg (const rank_map &rank_vector);
  double get_imagination (const std::vector<double> &vec_1,
                                 const std::vector<double> &vec_2);
  std::vector<double> get_priorities_vec (rank_map rank_vector,
                                                 double vec_avg);
  static bool compare_by_val (const std::pair<sp_movie, double> &p_1,
                              const std::pair<sp_movie, double> &p_2);

 public:
  explicit RecommenderSystem ();
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year,
             const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest score
   * based on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie get_recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score based
   * on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf procedure
   * with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;

  friend std::ostream &operator<< (std::ostream &stream,
                                   const RecommenderSystem &data);

};

#endif //RECOMMENDERSYSTEM_H
